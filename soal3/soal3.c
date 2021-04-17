#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int mode = 1;
    int pid_z;

    if (argc > 1) {
        if (strcmp(argv[1], "-z") == 0) {
            mode = 1;
        }
        else if (strcmp(argv[1], "-x") == 0) {
            mode = 2;
        }
    }

    FILE *killerx = fopen("./Killer.sh", "w");
    if (mode == 1) {
        pid_z = getpid();
    }
    else if (mode == 2) {
        fprintf(killerx, "kill %d\n", getpid());
        fprintf(killerx, "rm -- \"$0\"\n");
    }
    fclose(killerx);

    int limit_time_dir;
    time_t prev_time_dir = time(NULL);
    time_t curr_time_dir = time(NULL);

    int first_dir = 1;
    int still_dir = 1;

    while (still_dir) {
        curr_time_dir = time(NULL);

        limit_time_dir = (int)difftime(curr_time_dir, prev_time_dir);
        if (first_dir || limit_time_dir >= 40) {
            prev_time_dir = curr_time_dir;

            pid_t child;

            if (!(child = fork())) {
                if (mode == 1) {
                    FILE *killerz = fopen("./Killer.sh", "w");
                    fprintf(killerz, "kill %d\n", pid_z);
                    fprintf(killerz, "kill %d\n", getpid());
                    fprintf(killerz, "rm -- \"$0\"\n");
                    fclose(killerz);
                }
                
                char dirname[200];
                time_t now_dir = time(NULL);
                struct tm *t_dir = localtime(&now_dir);
                strftime(dirname, sizeof(dirname)-1, "%d-%m-%Y_%H:%M:%S", t_dir);

                pid_t child_dir;

                if (!(child_dir = fork())) {
                    char *arg1[]={"/bin/mkdir", dirname, NULL};
                    execv("/bin/mkdir", arg1);
                }

                char link[200];
                int epoch = ((int)time(NULL) % 1000) + 50;
                sprintf(link, "https://picsum.photos/%d", epoch);
                
                int limit_time_ph;
                time_t prev_time_ph = time(NULL);
                time_t curr_time_ph = time(NULL);
                int count = 1;
                int first_ph = 1;
                int still_ph = 1;

                while (still_ph) {
                    curr_time_ph = time(NULL);

                    limit_time_ph = (int)difftime(curr_time_ph, prev_time_ph);
                    if (first_ph || limit_time_ph >= 5) {
                        prev_time_ph = curr_time_ph;

                        if (count > 10) {
                            still_ph = 0;
                            break;
                        }

                        pid_t child_ph;

                        if (!(child_ph = fork())) {
                            char filename[200];
                            time_t now_ph = time(NULL);
                            struct tm *t_ph = localtime(&now_ph);
                            strftime(filename, sizeof(filename)-1, "%d-%m-%Y_%H:%M:%S", t_ph);

                            char path[200];
                            sprintf(path, "%s/%s.jpg", dirname, filename);

                            char *arg2[]={"/usr/bin/wget", link, "-O", path, NULL};
                            execv("/usr/bin/wget", arg2);
                            exit(0);
                        }

                        first_ph = 0;
                        count++;
                    }
                }
                char path_status[200];
                char text[] = "Download Success";
                char ch;
                int i, shift = 5;

                sprintf(path_status, "%s/status.txt", dirname);

                for (i = 0; text[i] != '\0'; ++i) {
                    ch = text[i];
                    
                    if (ch >= 'a' && ch <= 'z') {
                        ch = ch + shift;

                        if (ch > 'z') {
                            ch = ch - 'z' + 'a' - 1;
                        }

                        text[i] = ch;
                    }
                    else if (ch >= 'A' && ch <= 'Z') {
                        ch = ch + shift;

                        if (ch > 'Z') {
                            ch = ch - 'Z' + 'A' - 1;
                        }

                        text[i] = ch;
                    }
                }
                
                FILE *statustext = fopen(path_status, "w");

                if (statustext) {
                    fprintf(statustext, "%s\n", text);
                    fclose(statustext);
                }

                char zipname[200];
                char path_zip[200];

                sprintf(zipname, "%s.zip", dirname);
                sprintf(path_zip, "./%s", dirname);

                char *arg3[]={"/usr/bin/zip", "-rm", zipname, path_zip, NULL};
                execv("/usr/bin/zip", arg3);

                exit(0);
            }
            first_dir = 0;
        }
    }
}
