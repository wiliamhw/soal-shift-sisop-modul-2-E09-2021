#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include <dirent.h>
#include <ctype.h>

DIR *getDir(char *basePath);
FILE *getLog(char *dirPath);
char *getAge(char *input);
bool isRegularFile(const char *path);
void downloadExtract();
void deleteFolder(char *basePath, DIR *dir, bool deleteFile);
void categorize(DIR *dir);
void command(const char *command, char *path); // download, extract, delete, make_dir, copy

const char *user = "frain8";

int main()
{
    // Make sure that folder exist
    char petshop[50];
    sprintf(petshop, "/home/%s/modul2", user);
    DIR *tmp = getDir(petshop);
    closedir(tmp);

    strcat(petshop, "/petshop");
    DIR *dir = getDir(petshop);

    // Change curr dir
    if ((chdir(petshop)) < 0) {
        exit(EXIT_FAILURE);
    }
    downloadExtract(); // Download and extract pets.zip
    
    // Traverse dir
    deleteFolder(".", dir, false); // Clear initial folder
    categorize(dir);

    closedir(dir);
    return 0;
}

void downloadExtract()
{
    char *filename = "../pets.zip";
    if (access(filename, F_OK) != 0) { // If pets.zip not exist
        command("download", filename);
    }
    command("extract", filename);
}

void deleteFolder(char *basePath, DIR *dir, bool deleteFile)
{
    char path[1000];
    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {

            sprintf(path, "%s/%s", basePath, dp->d_name);
            if (deleteFile || !isRegularFile(path)) {
                command("delete", path);
            }
        }
    }
    rewinddir(dir);
}

void categorize(DIR *dir)
{
    char *basePath = ".";
    char path[1000], data[360], filename[100], name[50], type[50], age[4];
    struct dirent *dp;
    
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            
            sprintf(path, "%s/%s", basePath, dp->d_name);
            if (!isRegularFile(path)) {
                continue;
            }
            strcpy(filename, dp->d_name);

            // Get every animal in a photo
            char *save1, *save2;
            char *animal = strtok_r(filename, "_", &save1);
            while (animal) {
                strcpy(type, strtok_r(animal, ";", &save2));
                strcpy(name, strtok_r(NULL, ";", &save2));
                strcpy(age, getAge(save2));

                // Make sure that directory exist
                DIR *target = getDir(type);
                closedir(target);

                // Write keterangan.txt
                FILE *log = getLog(type); // getFile(dirPath)
                fprintf(log, "nama : %s\n", name);
                fprintf(log, "umur : %s tahun\n", age);
                fputs("\n", log);
                fclose(log);

                // Copy file to its appropriate folder
                strcat(name, ".jpg");
                sprintf(data, "%s|%s/%s", dp->d_name, type, name); // data = "filename|destination"
                command("copy", data);

                animal = strtok_r(NULL, "_", &save1);
            }
            command("delete", dp->d_name);
        }
    }
    rewinddir(dir);
}

FILE *getLog(char *dirPath)
{
    char tmp[50];
    sprintf(tmp, "%s/keterangan.txt", dirPath);
    FILE *fp = fopen(tmp, "a+");
    return fp;
}

char *getAge(char *input)
{
    int i = strlen(input) - 1;
    if (!isdigit(input[i])) {
        while (--i >= 0 && !isdigit(input[i]));
        input[i + 1] = '\0';
    }
    return input;
}

bool isRegularFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return (S_ISREG(path_stat.st_mode) != 0);
}

DIR *getDir(char *basePath)
{
    DIR *dir = opendir(basePath);
    if (!dir) {
        command("make_dir", basePath);
        dir = opendir(basePath);
        deleteFolder(basePath, dir, true); // Clear initial folder
    }
    return dir;
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
        if (strcmp(command, "download") == 0) {
            char *link = "https://docs.google.com/uc?export=download&id=1g5rehatLEkqvuuK_eooHJXB57EfdnxVD";
            char *argv[] = {"wget", "--no-check-certificate", link, "-O", path, NULL};
            execv("/bin/wget", argv);
        }
        else if (strcmp(command, "extract") == 0) {
            char *argv[] = {"unzip", "-o", path, NULL};
            execv("/bin/unzip", argv);
        }
        else if (strcmp(command, "delete") == 0) {
            char *argv[] = {"rm", "-rf", path, NULL};
            execv("/bin/rm", argv);
        }
        else if (strcmp(command, "make_dir") == 0) {
            char src[50];
            sprintf(src, "/home/%s/log", user);
            char *argv[] = {"cp", "-r", src, path, NULL};
            execv("/bin/cp", argv);
        }
        else if (strcmp(command, "copy") == 0 ) {
            // Parse data in path
            char arr[2][50];
            strcpy(arr[0], strtok(path, "|"));
            strcpy(arr[1], strtok(NULL, "|"));

            char *argv[] = {"cp", arr[0], arr[1], NULL};
            execv("/bin/cp", argv);
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
