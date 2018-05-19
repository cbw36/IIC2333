#include <stdio.h>
#include <stdlib.h>


int main(int argc, char * argv[])
{
    unsigned char buffer[2000];
    FILE *bin;

    bin = fopen("simdiskformat.bin","rwb");  // r for read, b for binary

    char str[] = "testing";

    fwrite(str , 1, sizeof(str) , bin );
    fseek(bin, 0, SEEK_SET);
//
//
//    fclose(bin);
//
//    bin = fopen("simdiskformat.bin","rb");  // r for read, b for binary


//    fread(buffer,sizeof(buffer),0,bin);

//    int c;
//    for (int i =0; i<100;i++) {
//        c = fgetc(bin);
//        if( feof(bin) ) {
//            break ;
//        }
//        printf("%c", c);
//    }
//    fclose(bin);


    char *tstr = "hello";

    while (*tstr) {
        printf("%c %u %x\n", *tstr, *tstr, *tstr);
        tstr++;
    }
//
//    for(int i = 0; i<10; i++)
//        printf("%u ", buffer[i]);



    return 1;

}

