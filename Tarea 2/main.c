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
        pid_t pid = fork();
        if (pid ==0)
        {
            printf("processo");
            execlp("/bin/echo", command, text, NULL); }
        else if (count>2) {
          printf("waint for process \n");
          wait(NULL);
          count --;
        }
        count ++;

    }
    while ((wpid = wait(&status)) > 0);
return 0;
}
