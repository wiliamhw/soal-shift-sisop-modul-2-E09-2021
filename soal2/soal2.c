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
DIR *getDir(char *basePath);
void deleteFolder(char *basePath, DIR *dir, bool deleteFile);
void categorize(DIR *dir);
void command(const char *command, char *path);
/* Command list:
    * download
    * extract
    * delete
    * make_dir
    * remove
*/

int main()
{
    // Make sure that folder exist
    char petshop[50] = "/home/frain8/modul2";
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
    deleteFolder(".", dir, false);
    categorize(dir);

    closedir(dir);
    return 0;
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
            char *argv[] = {"cp", "-r", "/home/frain8/log", path, NULL};
            execv("/bin/cp", argv);
        }
        else if (strcmp(command, "move") == 0 || strcmp(command, "copy") == 0 ) {
            // Parse data in path
            char arr[2][50];
            strcpy(arr[0], strtok(path, "|"));
            strcpy(arr[1], strtok(NULL, "|"));

            if (strcmp(command, "move") == 0) {
                char *argv[] = {"mv", arr[0], arr[1], NULL};
                execv("/bin/mv", argv);
            }
            else {
                char *argv[] = {"cp", arr[0], arr[1], NULL};
                execv("/bin/cp", argv);
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
    char path[1000], data[307], filename[100], name[50], type[50];
    struct dirent *dc;
    
    while ((dc = readdir(dir)) != NULL) {
        if (strcmp(dc->d_name, ".") != 0 && strcmp(dc->d_name, "..") != 0) {
            // Construct new path from the base path
            sprintf(path, "%s/%s", basePath, dc->d_name);

            if (!isRegularFile(path)) {
                continue;
            }
            strcpy(filename, dc->d_name);

            // Get every animal in a photo
            char *save1, *save2;
            char *animal = strtok_r(filename, "_", &save1);
            while (animal) {
                strcpy(type, strtok_r(animal, ";", &save2));
                strcpy(name, strtok_r(NULL, ";", &save2));

                // Copy filename to name
                strcat(name, ".jpg");
                sprintf(data, "%s|%s", dc->d_name, name);
                command("copy", data);

                // Move file to its appropriate folder
                DIR *target = getDir(type);
                sprintf(data, "%s|%s", name, type);
                command("move", data);
                closedir(target);

                animal = strtok_r(NULL, "_", &save1);
            }
            command("delete", dc->d_name);
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