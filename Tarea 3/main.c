#include <stdio.h>
#include <stdlib.h>
#include "cz_API.h"


char* disk_path;
int main(int argc, char * argv[])
{
    disk_path = argv[1];
    disk_init();
    char* file_name = "prueba";
    char* file_name1 = "prueba2";
    printf("exists prueba:%i\n", cz_exists("prueba"));
    czFile* file = cz_open(file_name, 'w');
    cz_mv("prueba", "prueba3");
    cz_ls();
    char* buffer = "hola como estas";
    char* buffer2;
    czFile* file2 = cz_open(file_name1, 'w');
    cz_write(file, buffer, 15);
    cz_read(file, buffer2, 15);
    cz_rm("prueba");
    printf("buffer2:%s\n", buffer2);
    cz_cp("prueba2", "prueba4");
    cz_ls();





    return 0;
}
    // char * buf = malloc(2045);
    // for (int i = 0;i<2045;i++)
    //   buf[i] = 'a';
    // int nbytes = 2045;
    // int written_bytes = cz_write(file, buf, nbytes);
    // file->mode_w = 0;
    // int rbytes = 1025;
    // char rbuff[rbytes];
    //
    // int out =  cz_read(file, rbuff, rbytes);  //TODO TEST!
   //int exists = cz_exists(file_name);
   //cz_open(file_name, 'w');


//  int nbit = 10;
//    FILE * prueba;
//    prueba = fopen("prueba.txt", "r");
//  cz_write(file, prueba, nbit);
    // char lectura[nbit];
    // fread(lectura,sizeof(char),nbit,prueba);
    // printf("%s\n",lectura );
