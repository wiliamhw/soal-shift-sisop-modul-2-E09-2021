#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

bool isRegularFile(const char *path);
bool isImage(const char *ext);
bool isValid(const char *ext);
bool diffByHours(int hours);
int *makeDaemon(pid_t *pid, pid_t *sid);
void deleteFolder(char *basePath, DIR *dir);
void dirInit(char *basePath);
void downloadExtract();
void listFilesRecursively(char *basePath, char *dirName[]);
void command(const char *command, char *path); // make_dir, delete, extract, download, move, multi_zip

const char *currDir = "/home/frain8/Documents/Sisop/Modul_2/soal_shift_2/soal1";
const int countFolder = 3;
char *tmpDir = "temp";

int main()
{
    char *dirName[] = {"Musyik", "Fylm", "Pyoto"}; // Format:mp3, mp4, jpg
    pid_t pid, sid;
    int *status = makeDaemon(&pid, &sid);

    while (true) {
        if (diffByHours(6)) { // categorize
            for (int i = 0; i < countFolder; i++) {
                dirInit(dirName[i]); // make initial dir
            }
            dirInit(tmpDir);
            downloadExtract();
            listFilesRecursively(tmpDir, dirName);
            command("delete", tmpDir);
        } 
        else if (diffByHours(0)) { // zip folders
            // Make data packet
            char data[500] = "";
            char tmp[100];
            strcpy(data, dirName[0]);
            for (int i = 1; i < countFolder; i++) {
                sprintf(tmp, "|%s", dirName[i]);
                strcat(data, tmp);
            }
            strcat(data, "|Lopyu_Stevany.zip");
            command("multi_zip", data);
        }
        while (wait(status) > 0);
        sleep(1);
    }
    return 0;
}
// sudo date --set="2021-04-09 16:21:59"
// sudo date --set="2021-04-09 22:21:59"
// unzip Lopyu_Stevany.zip

bool diffByHours(int hours)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return (tm.tm_mday == 9 & tm.tm_mon + 1 == 4 && 22 - tm.tm_hour == hours && 
            tm.tm_min == 22 && tm.tm_sec <= 2);
}

void listFilesRecursively(char *basePath, char *dirName[])
{
    char path[300], data[350];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (dir == NULL) {
        return;
    }
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            sprintf(path, "%s/%s", basePath, dp->d_name); // Construct new path from our base path
            char *ext = strrchr(dp->d_name, '.') + 1;

            // Move file to apropriate folder
            if (isRegularFile(path) && isValid(ext)) {
                if (strcmp(ext, "mp3") == 0) {
                    sprintf(data, "%s|%s", path, dirName[0]);
                } 
                else if (strcmp(ext, "mp4") == 0) {
                    sprintf(data, "%s|%s", path, dirName[1]);
                }
                else {
                    sprintf(data, "%s|%s", path, dirName[2]);
                }
                command("move", data);
            }
            listFilesRecursively(path, dirName);
        }
    }
    closedir(dir);
}

bool isValid(const char *ext) {
    return (
        ext && (strcmp(ext, "mp3") == 0
            || strcmp(ext, "mp4") == 0
            || isImage(ext))
    );
}

bool isImage(const char *ext)
{
    return (
        strcmp(ext, "jpg") == 0
        || strcmp(ext, "jpeg") == 0
        || strcmp(ext, "png") == 0
    );
}

bool isRegularFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return (S_ISREG(path_stat.st_mode) != 0);
}

void downloadExtract()
{
    // id file | nama zip
    char data[][100] = {
        "1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J|musik.zip",
        "1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD|foto.zip",
        "1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp|film.zip"
    };
    for (int i = 0; i < countFolder; i++) {
        command("download", data[i]);
        strtok(data[i], "|");
        command("extract", strtok(NULL, "|")); // extract to temp
    }
}

void dirInit(char *basePath)
{
    DIR *dir = opendir(basePath);
    if (!dir) { // If not exist, make new folder
        command("make_dir", basePath);
        dir = opendir(basePath);
    } else {
        deleteFolder(basePath, dir); // else, clean the folder
    }
    closedir(dir);
}

void deleteFolder(char *basePath, DIR *dir)
{
    char path[1000];
    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            sprintf(path, "%s/%s", basePath, dp->d_name);
            command("delete", path);
        }
    }
    rewinddir(dir);
}

void command(const char *command, char *path)
{
    int status;
    pid_t child_id;
    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        // this is child
        if (strcmp(command, "make_dir") == 0) {
            char *argv[] = {"mkdir", path, NULL};
            execv("/bin/mkdir", argv);
        }
        else if (strcmp(command, "delete") == 0) {
            char *argv[] = {"rm", "-rf", path, NULL};
            execv("/bin/rm", argv);
        }
        else if (strcmp(command, "extract") == 0) {
            char src[100];
            sprintf(src, "temp/%s", path);
            char *argv[] = {"unzip", src, "-d", tmpDir, NULL};
            execv("/bin/unzip", argv);
        }
        else if (strcmp(command, "download") == 0 || strcmp(command, "move") == 0) {
            // Parse data in path
            char arr[2][50];
            strcpy(arr[0], strtok(path, "|"));
            strcpy(arr[1], strtok(NULL, "|"));

            if (strcmp(command, "download") == 0) {
                char link[100], targetDir[100];
                sprintf(link, "https://drive.google.com/uc?id=%s&export=download", arr[0]);
                sprintf(targetDir, "%s/%s", tmpDir, arr[1]);

                char *argv[] = {"wget", "--no-check-certificate", link, "-O", targetDir, "-q", NULL};
                execv("/bin/wget", argv);
            } else {
                char *argv[] = {"mv", arr[0], arr[1], NULL};
                execv("/bin/mv", argv);
            }
        }
        else if (strcmp(command, "multi_zip") == 0) {
            // Parse data in path
            char data[countFolder + 1][100];
            strcpy(data[0], strtok(path, "|"));
            for (int i = 1; i <= countFolder; i++) {
                strcpy(data[i], strtok(NULL, "|"));
            }
            char *argv[] = {"zip", "-rm", data[3], data[2], data[1], data[0], NULL};
            execv("/bin/zip", argv);
        }
        else {
            printf("Unrecognized command\n");
            exit(EXIT_FAILURE);
        }
    } else {
        // this is parent
        while (wait(&status) > 0);
        return;
    }
}

int *makeDaemon(pid_t *pid, pid_t *sid)
{
    int status;
    *pid = fork();

    if (*pid != 0) {
        exit(EXIT_FAILURE);
    }
    if (*pid > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0);

    *sid = setsid();
    if (*sid < 0 || chdir(currDir) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    return &status;
}
