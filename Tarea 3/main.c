#include <stdio.h>
#include <stdlib.h>
#include "cz_API.h"

int main(int argc, char * argv[])
{
  char* file_name = "hola4";
  //disk_init();
   czFile* file = cz_open(file_name, 'r');
   // cz_mv("hola4", "hola3");
   // sleep(4);
   // cz_open(file_name, 'w');
   // cz_ls();

  int nbit = 10;
  FILE * prueba = fopen("prueba.txt", "r");
  cz_write(file, prueba, nbit);
    // char lectura[nbit];
    // fread(lectura,sizeof(char),nbit,prueba);
    // printf("%s\n",lectura );

}
