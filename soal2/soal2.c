#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include <dirent.h>

bool isRegularFile(const char *path);
void downloadExtract();
void deleteFolder(char *basePath, DIR *dir, bool deleteFile);
void categorize(DIR *dir);
void command(char *command, char *path);
/* Command list:
    * download
    * extract
    * delete
    * make_dir
    * remove
*/

int main()
{
    char *petshop = "/home/frain8/modul2/petshop";
    DIR *dir = opendir(petshop);
    if (dir == NULL) {
        command("make_dir", petshop);
        dir = opendir(petshop);
    }

    // Change curr dir
    if ((chdir(petshop)) < 0) {
        exit(EXIT_FAILURE);
    }
    downloadExtract(); // Download and extract pets.zip
    
    // Traverse dir
    deleteFolder(".", dir, false);
    categorize(dir);

    closedir(dir);
    return 0;
}

void command(char *command, char *path)
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
            char *argv[] = {"cp", "-r", "/home/frain8/log", path, NULL};
            execv("/bin/cp", argv);
        }
        else if (strcmp(command, "move") == 0) {
            // Parse data in path
            char arr[2][50];
            strcpy(arr[0], strtok(path, "|"));
            strcpy(arr[1], strtok(NULL, "|"));

            char *argv[] = {"mv", arr[0], arr[1], NULL};
            execv("/bin/mv", argv);
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
    struct dirent *dc;
    while ((dc = readdir(dir)) != NULL) {
        if (strcmp(dc->d_name, ".") != 0 && strcmp(dc->d_name, "..") != 0) {
            // Construct new path from the base path
            sprintf(path, "%s/%s", basePath, dc->d_name);

            if (deleteFile || !isRegularFile(path)) { // If path is a folder
                command("delete", path); // Delete folder
            }
        }
    }
    rewinddir(dir);
}

void categorize(DIR *dir)
{
    char *basePath = ".";
    char path[1000];
    struct dirent *dc;
    while ((dc = readdir(dir)) != NULL) {
        if (strcmp(dc->d_name, ".") != 0 && strcmp(dc->d_name, "..") != 0) {
            // Construct new path from the base path
            sprintf(path, "%s/%s", basePath, dc->d_name);

            if (!isRegularFile(path)) {
                continue;
            }

            // Initialize filename and its type
            char filename[sizeof(dc->d_name)], type[sizeof(dc->d_name)];
            strcpy(filename, dc->d_name);
            strcpy(type, strtok(dc->d_name, ";"));

            // Move file to its appropriate folder
            DIR *dir = opendir(type);
            if (dir == NULL) {
                command("make_dir", type);
                dir = opendir(type);
                deleteFolder(type, dir, true); // Clear initial folder
            }
            char data[sizeof(filename) + sizeof(type) + 1];
            sprintf(data, "%s|%s", filename, type);
            command("move", data);
            closedir(dir);
        }
    }
    rewinddir(dir);
}

bool isRegularFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return (S_ISREG(path_stat.st_mode) != 0);
}