#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char * argv[])
{
    char *s = "hello world";
    *s = 'H';
    return 0;
}