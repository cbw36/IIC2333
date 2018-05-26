#include <stdio.h>
#include <stdlib.h>
#include "cz_API.h"

int main(int argc, char * argv[])
{
//    disk_init();
    char* file_name = "hola10";
    czFile *file = cz_init(file_name);

    char * buf = malloc(2045);
    for (int i = 0;i<2045;i++)
      buf[i] = 'a';
    int nbytes = 2045;
    int written_bytes = cz_write(file, buf, nbytes);
    file->mode_w = 0;
    int rbytes = 1025;
    char rbuff[rbytes];

    int out =  cz_read(file, rbuff, rbytes);  //TODO TEST!
//    czFile* file = cz_init(file_name);
//    int exists = cz_exists(file_name);
   // cz_mv("hola4", "hola3");
   // sleep(4);
   // cz_open(file_name, 'w');
   // cz_ls();

//  int nbit = 10;
//    FILE * prueba;
//    prueba = fopen("prueba.txt", "r");
//  cz_write(file, prueba, nbit);
    // char lectura[nbit];
    // fread(lectura,sizeof(char),nbit,prueba);
    // printf("%s\n",lectura );


    return 0;
}
