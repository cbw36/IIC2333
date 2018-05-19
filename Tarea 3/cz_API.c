#include <stdio.h>
#include <stdlib.h>
#include "cz_API.h"


czFile* cz_init(char* name, int size, char* data){
    unsigned char bitmap[8192];
    FILE *bin;
    bin = fopen("simdiskformat.bin","wb");  // r for read, b for binary
    fseek(bin, 1024, SEEK_SET);
    char one[] = "1";
    fwrite(one , 1, sizeof(one) , bin );
    fclose(bin);

    bin = fopen("simdiskformat.bin","rb");  // r for read, b for binary
    fseek(bin, 1024, SEEK_SET);
    fread(bitmap, sizeof(bitmap), 1, bin);
    int index = 0;
    while (bitmap[index]!= 0) {
      index++;
    }
    fclose(bin);
    bin = fopen("simdiskformat.bin","wb");  // r for read, b for binary
    fseek(bin, 1024+index, SEEK_SET);
    fwrite(one , 1, sizeof(one) , bin );
    int pos_directorio = (index-9)*128;
    char valid[] = "00000001";
    fseek(bin, pos_directorio, SEEK_SET);
    fwrite(valid , 1, sizeof(valid) , bin);

    char *ascii_name;
    for (int i =0; i<10;i++)
    {
      if (*name == 0){
        break;
      }
      sprintf(ascii_name, "%u ", *name);
      printf("%s\n", ascii_name);
      int dec = atoi(ascii_name);
      *(ascii_name+5) = '\0';
      int mask = 0x10 << 1;
      while(mask >>= 1)
      {printf("segundo Print%i\n", mask);
      printf("Tercer print:%d\n", !!(mask & dec) + '0');
      *ascii_name++ = !!(mask & dec) + '0';
    }


      //printf("%s", ascii_name);
      name++;
      // fwrite(name[i],);
    }



    // while (*name){
    //   printf("%c %u %x\n", *name, *name, *name);
    //   name++;
    // }






}
