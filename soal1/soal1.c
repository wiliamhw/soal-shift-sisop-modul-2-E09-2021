#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>

bool isRegularFile(const char *path);
bool isImage(const char *ext);
bool isValid(const char *ext);
void deleteFolder(char *basePath, DIR *dir);
void dirInit(char *basePath);
void downloadExtract();
void categorize(char *basePath, char *dirName[]);
void command(const char *command, char *path); // make_dir

const int countFolder = 3;
char *tmpDir = "temp";

int main()
{
    // Change curr dir
    const char *currDir = "/home/frain8/Documents/Sisop/Modul_2/soal_shift_2/soal1";
    if ((chdir(currDir)) < 0) {
        exit(EXIT_FAILURE);
    }

    char *dirName[] = {"Musyik", "Fylm", "Pyoto"}; // Format:mp3, mp4, jpg
    for (int i = 0; i < countFolder; i++) {
        dirInit(dirName[i]); // make initial dir
    }
    dirInit(tmpDir);
    downloadExtract();
    categorize(tmpDir, dirName);
    command("delete", tmpDir);

    return 0;
}

void categorize(char *basePath, char *dirName[])
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
            categorize(path, dirName);
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
        printf("Fork failed.\n");
        exit(EXIT_FAILURE);
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
            char targetDir[50];
            sprintf(targetDir, "temp/%s", path);

            char *argv[] = {"unzip", "-o", targetDir, "-d", tmpDir, NULL};
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
                sprintf(targetDir, "temp/%s", arr[1]);

                char *argv[] = {"wget", "--no-check-certificate", link, "-O", targetDir, NULL};
                execv("/bin/wget", argv);
            } else {
                char *argv[] = {"mv", arr[0], arr[1], NULL};
                execv("/bin/mv", argv);
            }
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