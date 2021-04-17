#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#define NUM_FOLDER 3

void dl_drive(pid_t child_id, int *status, char *file_name[]);
void makedir(pid_t child_id, int *status, char *fl_name[], char *fl2[]);
void for_stev(pid_t child_id,char *fl2[]);
void browse(int *status, char fl_name[], char fl2[]);
void move(struct dirent * drent, int *status, char *fl2, char fl_name[]);
void del_fl(pid_t child_id, int *status, char *fl_name[]);
void zipfl(pid_t child_id, int *status, char *fl2[], char zipname[]);
void godaemon(int *status);

int main()
{
    const char workdir[]="/home/sisop";
    pid_t child_id, sid;
    int status;
    if ((child_id=fork())>0) exit (EXIT_SUCCESS);
    umask(0);
    sid = setsid();
    if (sid<0||chdir(workdir)) exit (EXIT_FAILURE);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    godaemon(&status);
    return 0;
}

void dl_drive(pid_t child_id, int *status, char *file_name[])
{
    char *links[]= {"https://drive.google.com/file/d/1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD/view","https://drive.google.com/file/d/1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J/view","https://drive.google.com/file/d/1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp/view"};
    for(int a=0;a<NUM_FOLDER;a++)
    {
        if((child_id = fork())==0)
        {
            char *argv[] = {"wget", "--no-check-certificate", links[a], "-O", file_name[a], "-q", NULL};
            execv("/bin/wget", argv);
            while(wait(status)>0);
        }
        if((child_id = fork())==0)
        {
            char *argv[] = {"unzip", "-qq", file_name[a], NULL};
            execv("/bin/unzip", argv); 
            while(wait(status)>0);
        }                 
    }
}

void makedir(pid_t child_id, int *status, char *fl_name[], char *fl2[])
{
    if((child_id = fork())==0)
    {
        char *argv[] = {"mkdir", "-p", fl2[0], fl2[1], fl2[2], NULL};
        execv("/bin/mkdir", argv);
    } while(wait(status)>0);
}

void for_stev(pid_t child_id,char *fl2[])
{
    int status;
    char *file_name[]={"FOTO.zip","MUSIK.zip","FILM.zip"};
    char *fl_name[]={"FOTO","MUSIK","FILM"};
    makedir(child_id,&status,fl_name,fl2);
    dl_drive(child_id,&status,file_name);
    for(int b=0;b<NUM_FOLDER;b++)browse(&status,fl_name[b],fl2[b]);
    del_fl(child_id,&status,fl_name);
}

void browse(int *status, char fl_name[], char fl2[])
{
    DIR *dir=opendir(fl_name);
    struct dirent *drent;
    if(dir != NULL)
    {
        while((drent = readdir(dir)))move(drent,status,fl2,fl_name);
        (void)closedir(dir);
    }else perror ("ERROR: Could not open directory");
}

void move(struct dirent * drent, int *status, char *fl2, char fl_name[])
{
    pid_t child_move = fork();
    if(child_move==0 && (strcmp(drent->d_name, "..")==0)) exit(EXIT_SUCCESS);
    if(child_move==0)
    {
        char pathfile[512];
        strcpy(pathfile,fl_name); strcat(pathfile,"/"); strcat(pathfile,drent->d_name);
        char *argv[] = {"mv",pathfile, fl2,NULL};
        execv("/bin/mv", argv);
    } while(wait(status)>0);
}

void del_fl(pid_t child_id, int *status, char *fl_name[])
{
    if((child_id = fork())==0)
    {
        char *argv[] = {"rm", "-rf", fl_name[0], fl_name[1], fl_name[2], NULL};
        execv("/bin/rm",argv);
    } while(wait(status)>0);
}

void zipfl(pid_t child_id, int *status, char *fl2[], char zipname[])
{
    if((child_id = fork())==0)
    {
        char *argv[] = {"zip","-rmvq",zipname,fl2[0],fl2[1],fl2[2],NULL};
        execv("/bin/zip",argv);
    } while(wait(status)>0);
}

void godaemon(int *status)
{
    pid_t child_id;
    const unsigned sleep_interval=1;
    const int bmonth = 3;
    const int bday = 9;
    const int bhour = 22;
    const int bmin = 22;
    char *fl2[]={"Fyoto","Musyik","Fylm"};
    while(1)
    {
        time_t now=time(NULL);
        struct tm *nowloc=localtime(&now);
        if (bmonth == nowloc->tm_mon && bday == nowloc->tm_mday && bhour == nowloc->tm_hour&&bmin==nowloc->tm_min&& nowloc->tm_sec==0)
        zipfl(child_id, status, fl2, "Lopyu_Stefany.zip");
        if (bmonth == nowloc->tm_mon && bday == nowloc->tm_mday && bhour == nowloc->tm_hour&&bmin==nowloc->tm_min&& nowloc->tm_sec==0)
        for_stev(child_id,fl2);
        while(wait(status)>0);

        sleep(sleep_interval);
    }
}
