#include "doer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<stdarg.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <string.h>

const int m = 3;

int getNumLines(char* filepath)
{
    FILE*fr;
    fr = fopen(filepath, "r");
    int num_lines = 0;
    char line[256];

    while (fgets(line, 256, fr))
    {
        num_lines++;
    }
    fclose(fr);
    return num_lines;
}

char *** processTextFile(char ** all_lines, int n)
{
    char *** processes;
    processes = malloc(sizeof(char**)*n);

    for (int i =0; i<n;i++)
    {
        if(strchr(all_lines[i], '"')!=NULL){ //Keep string inside "" together
            processes[i] = malloc(sizeof(char *) * 20); //Assume less than 20 arguments
            int ctr = 0;
            int ind = 0;
            processes[i][ctr] = malloc(sizeof(char) * 256);
            for (int j = 0; j < strlen(all_lines[i]); j++) {
                if (all_lines[i][j] == '"')
                {
                    j++;
                    while (all_lines[i][j] != '"')
                    {
                        processes[i][ctr][ind] = all_lines[i][j];
                        ind++;
                        j++;
                    }
                    ctr++;
                    j++;
                }
                else if (all_lines[i][j] == ' ' || all_lines[i][j] == '\0' || all_lines[i][j] == '\n') {
                    processes[i][ctr][ind] = '\0';
                    ctr++;
                    processes[i][ctr] = malloc(sizeof(char) * 256);
                    ind = 0;
                } else {
                    processes[i][ctr][ind] = all_lines[i][j];
                    ind++;
                }
                processes[i][ctr + 1] = NULL;
            }
        }        else { // Read each value separated by space into the array
            processes[i] = malloc(sizeof(char *) * 20); //Assume less than 20 arguments
            int ctr = 0;
            int ind = 0;
            processes[i][ctr] = malloc(sizeof(char) * 256);
            for (int j = 0; j < strlen(all_lines[i]); j++) {
                if (all_lines[i][j] == ' ' || all_lines[i][j] == '\0' || all_lines[i][j] == '\n') {
                    processes[i][ctr][ind] = '\0';
                    ctr++;
                    processes[i][ctr] = malloc(sizeof(char) * 256);
                    ind = 0;
                } else {
                    processes[i][ctr][ind] = all_lines[i][j];
                    ind++;
                }
                processes[i][ctr + 1] = NULL;
            }
        }
    }
    return processes;
}


int main(int argc, char * argv[])
{
    char*filepath = argv[1];
    int num_lines = getNumLines(filepath);

    FILE*fr;
    fr = fopen(filepath, "r");

    int status = 0;
    char ** all_lines;
    all_lines = malloc(sizeof(char*)*num_lines);

    for (int i =0; i<num_lines;i++)
    {
        all_lines[i]= malloc(256* sizeof(char));
        fgets(all_lines[i],256, fr);
    }

    char*** processes = processTextFile(all_lines, num_lines);


    pid_t wpid;
    int proc_running = 0;
    int proc_ran = 0;
    while (proc_ran<num_lines) {
        printf("NEXT LINE\n");
        proc_running ++;
        pid_t pid = fork();
        if (pid ==0)
        {
            execvp(processes[proc_ran][0], processes[proc_ran]);
            exit(0);
        }
        else if (proc_running==m) {
            printf("wait for process, count = %i \n", proc_running);
            proc_running --;
            wait(NULL);

        }
        else
        {
            printf("Parent when count = %i\n", proc_running);
        }

        proc_ran++;


    }
    while ((wpid = wait(&status)) > 0);
    return 0;
}
