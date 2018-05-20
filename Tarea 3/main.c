#include <stdio.h>
#include <stdlib.h>
#include "cz_API.h"

int main(int argc, char * argv[])
{
    disk_init();
    cz_init("name", 5, "data");
    return 1;
}

