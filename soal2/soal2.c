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
void listFilesRecursively(char *basePath);
void command(char *command, char *path);
/* Command list:
    * download
    * extract
    * delete
*/

int main()
{
    char *petshop = "/home/frain8/modul2/petshop";
    if ((chdir(petshop)) < 0) {
        exit(EXIT_FAILURE);
    }
    downloadExtract(); // Download and extract pets.zip
    listFilesRecursively(".");

    return 0;
}

void downloadExtract()
{
    char *filename = "../pets.zip";
    if (access(filename, F_OK) != 0) { // If pets.zip not exist
        command("download", "../");
    }
    command("extract", filename);
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
            char *argv[] = {"wget", "-P", path, "--no-check-certificate", link, "-O", "pets.zip", NULL};
            execv("/bin/wget", argv);
        }
        else if (strcmp(command, "extract") == 0) {
            char *argv[] = {"unzip", "-o", path, NULL};
            execv("/bin/unzip", argv);
        }
        else if (strcmp(command, "delete") == 0) {
            char *argv[] = {"rm", "-r", path, NULL};
            execv("/bin/rm", argv);
        }
    }
    else {
        // this is parent
        while (wait(&status) > 0);
        return;
    }
}

void listFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dc; // Store one dir content
    DIR *dir = opendir(basePath);

    if (dir == NULL) {
        return;
    }
    while ((dc = readdir(dir)) != NULL) {
        if (strcmp(dc->d_name, ".") != 0 && strcmp(dc->d_name, "..") != 0) {
            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dc->d_name);

            if (!isRegularFile(path)) {
                command("delete", path); // Delete folder
            }
        }
    }
    closedir(dir);
}


bool isRegularFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return (S_ISREG(path_stat.st_mode) != 0);
}