#include "doer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<stdarg.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <string.h>


int main(int argc, char * argv[])
{
    char*filepath = argv[1];

    FILE*fr;
    fr = fopen(filepath, "r");
    int count = 0;
    int status = 0;

    char command[256];
    char text[256];
    pid_t wpid;
    while (fscanf(fr, "%s %s", command, text) != EOF) {
        printf("NEXT LINE\n");
        pid_t pid = fork();
        if (pid ==0)
        {
            printf("processo %i, count = %i command = %s text = %s\n", getpid(), count, command, text);
            execlp(command, command, text, NULL); }
        else if (count>3) {
            printf("wait for process, count = %i \n", count);
            wait(NULL);
            count --;
        }
        else
        {
            printf("Parent when count = %i\n", count);
        }
        count ++;

    }
    while ((wpid = wait(&status)) > 0);
    return 0;
}



//int main(int argc, char * argv[])
//{
//    char*filepath = argv[1];
//
//    FILE*fr;
//    fr = fopen(filepath, "r");
//
//    char command[256];
//    char text[256];
//
//    while (fscanf(fr, "%s %s", command, text) != EOF) {
//        pid_t pid = fork();
//        if (pid ==0)
//        {
//            execlp("/bin/echo", command, text, NULL);
//        }
//        else
//        {
//            printf("wait for process \n");
//            waitpid(pid, 0,0);
//        }
//    }
//}

/**
void dummyFork()
{
    pid_t pid = fork();
    if (pid ==0)
    {
        execl("bin/echo", command, text, NULL);
        exit(127);
    }
    else
    {
        printf("waint for process");
        waitpid(pid, 0,0);
    }

    char * args[] = {"echo", "something", NULL};
    pid_t child1 = fork();
    if (child1 == 0)
    {
        printf("CHILD: I am child process 1!\n");
        printf("CHILD: Here's my PID: %d\n", getpid());
        printf("CHILD: My parent's PID is: %d\n", getppid());
        printf("CHILD: The value of my copy of childpid is: %d\n", child1);
        execl("/bin/echo", "echo", "text", NULL);

    }
    else
    {
        printf("PARENT: I am the parent process waiting for child 1!\n");
        printf("PARENT: Here's my PID: %d\n", getpid());
        waitpid(child1, 0,0);
    }
    pid_t child2 = fork();
    if (child2==0)
    {
        printf("CHILD: I am child process 2!\n");
        printf("CHILD: Here's my PID: %d\n", getpid());
        printf("CHILD: My parent's PID is: %d\n", getppid());
        printf("CHILD: The value of my copy of childpid is: %d\n", child2);
        execlp("/bin/ls", "ls",NULL);
    }
    else
    {
        printf("PARENT: I am the parent process waiting for child 2!\n");
        printf("PARENT: Here's my PID: %d\n", getpid());
        waitpid(child2, 0,0);
    }
    execlp("/bin/echo", "echo", "FinalParentEcho",NULL);
}
*/
