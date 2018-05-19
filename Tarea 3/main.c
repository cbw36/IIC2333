#include <stdio.h>
#include <stdlib.h>


int main(int argc, char * argv[])
{
    unsigned char buffer[2048000];
    FILE *bin;

    bin = fopen("simdiskformat.bin","rb");  // r for read, b for binary

    fread(buffer,sizeof(buffer),1,bin);


    for(int i = 0; i<10; i++)
        printf("%u ", buffer[i]);
    printf("%u ", buffer[1000]);
    printf("%u ", buffer[2000]);
    printf("%u ", buffer[3000]);
    printf("%u ", buffer[4000]);



    return 1;

}

