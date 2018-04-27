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

    pid_t * all_pids;
    all_pids = malloc(num_lines * sizeof(pid_t));
    int * proc_num_execs;
    proc_num_execs = malloc(num_lines * sizeof(int));

    pid_t wpid;
    int proc_running = 0;
    int proc_ran = 0;
    while (proc_ran<num_lines) {
        proc_running ++;
        pid_t pid = fork();
        if (pid ==0)
        {
            execvp(processes[proc_ran][0], processes[proc_ran]);
            exit(-1);
        }
        else if (proc_running==m) {
            proc_running --;
            int status;
            pid_t cur_pid = wait(&status);
            printf("STATUS = %i for pid %i \n", status, (int) cur_pid);
            if (!WIFEXITED(status)) // find process index to see if it has been executed twice
            {
                printf("FAILED PROCESS\n");
                for (int i = 0; i <num_lines;i++)
                {
                    if ((all_pids[i] == cur_pid) && (proc_num_execs[i] != 1))
                    {
                        printf("Process %i failed, run again\n", i);
                        pid_t pid = fork();
                        if (pid ==0)
                        {
                            execvp(processes[i][0], processes[i]);
                            proc_num_execs[i] = 1;
                        }
                        else
                        {
                            wait(NULL);
                        }
                    }
                }
            }
        }
        all_pids[proc_ran] = pid;
        proc_ran++;


    }
    while ((wpid = wait(&status)) > 0);
    return 0;
}
