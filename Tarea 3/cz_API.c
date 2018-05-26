#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cz_API.h"



char* hex_to_bin(char hex)
{
    char* bin = malloc(4);
    if(hex == '0'){
      bin[0] = '0';
      bin[1] = '0';
      bin[2] = '0';
      bin[3] = '0';
     }
   else if(hex == '1'){
     bin[0] = '0';
     bin[1] = '0';
     bin[2] = '0';
     bin[3] = '1';
    }
    else if(hex == '2'){
      bin[0] = '0';
      bin[1] = '0';
      bin[2] = '1';
      bin[3] = '0';
     }
     else if(hex == '3'){
       bin[0] = '0';
       bin[1] = '0';
       bin[2] = '1';
       bin[3] = '1';
      }
      else if(hex == '4'){
        bin[0] = '0';
        bin[1] = '1';
        bin[2] = '0';
        bin[3] = '0';
       }
       else if(hex == '5'){
         bin[0] = '0';
         bin[1] = '1';
         bin[2] = '0';
         bin[3] = '1';
        }
        else if(hex == '6'){
          bin[0] = '0';
          bin[1] = '1';
          bin[2] = '1';
          bin[3] = '0';
         }
         else if(hex == '7'){
           bin[0] = '0';
           bin[1] = '1';
           bin[2] = '1';
           bin[3] = '1';
          }
          else if(hex == '8'){
            bin[0] = '1';
            bin[1] = '0';
            bin[2] = '0';
            bin[3] = '0';
           }
           else if(hex == '9'){
             bin[0] = '1';
             bin[1] = '0';
             bin[2] = '0';
             bin[3] = '1';
            }
            else if(hex == 'a'){
              bin[0] = '1';
              bin[1] = '0';
              bin[2] = '1';
              bin[3] = '0';
             }
             else if(hex == 'b'){
               bin[0] = '1';
               bin[1] = '0';
               bin[2] = '1';
               bin[3] = '1';
              }
              else if(hex == 'c'){
                bin[0] = '1';
                bin[1] = '1';
                bin[2] = '0';
                bin[3] = '0';
               }
               else if(hex == 'd'){
                 bin[0] = '1';
                 bin[1] = '1';
                 bin[2] = '0';
                 bin[3] = '1';
                }
                else if(hex == 'e'){
                  bin[0] = '1';
                  bin[1] = '1';
                  bin[2] = '1';
                  bin[3] = '0';
                 }
                 else if(hex == 'f'){
                   bin[0] = '1';
                   bin[1] = '1';
                   bin[2] = '1';
                   bin[3] = '1';
                  }
          return bin;
}

czFile*  cz_init(char* name)
{
    FILE *bin;
    unsigned char bitmap[8192];
    unsigned char directory[1024];
    unsigned char* new_directory = malloc(12);
    new_directory[0] = '\x01';
    for (int i = 1; i < 12; i++) {
      if (i-1 >= strlen(name)){
        new_directory[i] = '\x00';
      }
      else{
        //printf("%c\n", name[i-1]);
        new_directory[i] = name[i-1];
      }
    }

    bin = fopen("simdiskformat.bin", "r+");  // open bin to find location of next available block from bitmap

    fseek(bin, 1024, SEEK_SET);  //seek to bitmap block
    fread(bitmap, sizeof(bitmap), 1, bin); //read bitmap from file to buffer (all 8)
    int index = 0; //index inside bitmap of next available block
    int count = 0;
    int aux =0;
    while (1){
      int cur_byte = bitmap[index];
      int old_bin_name[8];
      for (int i = 0; i<8;i++)
          old_bin_name[i] = 0;

      int ind = 7;
      int * new_bin_name = dec_to_bin(cur_byte, old_bin_name, ind);
      for (int i = 0; i < 8; i++) {
        if (new_bin_name[i] == 0){
          new_bin_name[i] = 1;
          int new_add_dec = 0;
          for (int j = 0; j < 8; j++) {
            new_add_dec += pow(2,j) * new_bin_name[7-j];
          }
          int new[1]={new_add_dec};
          //printf("Updated from %i%i%i%i%i%i%i%i to  %i\n ", new_bin_name[0],new_bin_name[1],new_bin_name[2],new_bin_name[3],new_bin_name[4],new_bin_name[5],new_bin_name[6],new_bin_name[7], new[0]);
          aux = 1;
          fseek(bin, 1024+index, SEEK_SET);
          fwrite(new, sizeof(int), 1, bin);

          break;
        }
        count ++;
        }
        if (aux == 1){
          break;
      }

      index++;
    }

    int dir_index = count -9;
    fseek(bin, dir_index*16, SEEK_SET);
    fwrite(new_directory,sizeof(char), 12, bin);
    char* ind_hex = malloc(8);
    sprintf(&ind_hex[0], "%x",count);
    //printf("%s\n", ind_hex);
    fseek(bin, dir_index * 16 +14, SEEK_SET);

    int count_c[1] = {count};
    // printf("%c ", count_c[0]);
    fwrite(count_c, sizeof(int), 1, bin);
    unsigned char first[2];
    fseek(bin, dir_index * 16 +14, SEEK_SET);
    fread(first, sizeof(int), 2, bin);
    //printf("%i %i", first[0], first[1]);
    int aux1[2] = {first[1], first[0]};
    fseek(bin, dir_index * 16 +14, SEEK_SET);
    czFile* file = malloc(sizeof(czFile));
    file-> direccion_indice = malloc(2);
    file-> direccion_indice[0] = first[1];
    file-> direccion_indice[1] = first[0];

    // fwrite(aux1, sizeof(char), 2, bin);
    return file;


}

int cz_exists(char* filename)
{
  FILE *bin;
  unsigned char directory[1024];
  bin = fopen("simdiskformat.bin","r+");
  fread(directory, sizeof(directory), 1, bin);
  for (int i = 0; i < 64; i++) {
    int bit_actual = 16*i;
    int equal = 1;
    if (directory[bit_actual]=='\x01'){
      bit_actual++;
      int size;
      if (strlen(filename)< 11){
        size = strlen(filename);
      }
      else {
        size = 10;
      }
      for (int j = 0; j < size; j++) {
        if (filename[j] != directory[bit_actual+j]){
          equal = 0;
        }
      }
    if (directory[bit_actual+size]!= '\x00'){
      equal = 0;
    }
    bit_actual = bit_actual + 11;
    if (equal == 1){
      fclose(bin);
    return 1;
    }
    }
  }
  fclose(bin);
  return 0;
}

void disk_init()
{
    FILE *bin;
    bin = fopen("simdiskformat.bin","r+");  // Write first 9 bitmaps as 1 (directory/bitmap) and then a few more for artificial files
    unsigned char directory[1024];
    unsigned char bitmap[8094];

    char* first = malloc(2);
    fseek(bin, 1024, SEEK_SET);
    first[0] = '\xFF';
    first[1] = '\x80';
    fwrite(first , 1, 2, bin);

    fclose(bin);
}

czFile* cz_open(char* filename, char mode)
{
  FILE *bin;
  unsigned char directory[1024];
  bin = fopen("simdiskformat.bin","r+");
  fread(directory, sizeof(directory), 1, bin);
  int equal = 0;
  if (mode =='r'){
    for (int i = 0; i < 64; i++) {
      int bit_actual = 16*i;
      int equal = 1;
      if (directory[bit_actual]=='\x01'){
        bit_actual++;
        int size;
        if (strlen(filename)< 11){
          size = strlen(filename);
        }
        else {
          size = 10;
        }
        for (int j = 0; j < size; j++) {
          if (filename[j] != directory[bit_actual+j]){
            equal = 0;
          }
        }
      if (directory[bit_actual+size]!= '\x00'){
        equal = 0;

      }
      bit_actual = bit_actual + 11;
      if (equal == 1){
      czFile* current_file = malloc(sizeof(czFile));
      current_file -> direccion_indice = malloc(4);
      for (int j = 0; j < 4; j++) {
        current_file -> direccion_indice[j] = directory[bit_actual+j];
      }
      return current_file;
      }
      }
    }
    if (equal == 0){
      fclose(bin);
      return NULL;
    }
  }
  else if (mode =='w'){
    if (!cz_exists(filename)){
        czFile* current_file = cz_init(filename);
    }
    else
    {
      fclose(bin);
      return NULL;
    }

  }
  fclose(bin);
  return NULL;
}

int cz_mv(char* orig, char *dest)
{
  if (cz_exists(orig) && !cz_exists(dest)) {
    FILE *bin;
    unsigned char directory[1024];
    bin = fopen("simdiskformat.bin","r+");
    fread(directory, sizeof(directory), 1, bin);
    int equal = 0;
    for (int i = 0; i < 64; i++) {
      int bit_actual = 16*i;
      int equal = 1;
      if (directory[bit_actual]=='\x01'){
        bit_actual++;
        int size;
        if (strlen(orig)< 11){
          size = strlen(orig);
        }
        else {
          size = 10;
        }
        for (int j = 0; j < size; j++) {
          if (orig[j] != directory[bit_actual+j]){
            equal = 0;
          }
        }
      if (directory[bit_actual+size]!= '\x00'){
        equal = 0;
      }
      bit_actual = bit_actual + 11;
      if (equal == 1){
        fseek(bin, i*16+1, SEEK_SET);
        char* new_name = malloc(11);
        for (int k = 0; k < 11; k++) {
          if (k >= strlen(dest)){
            new_name[k] = '\x00';
          }
          else{
            new_name[k] = dest[k];
          }
        }
        fwrite(new_name,sizeof(char),11,bin);
        return 1;


      }
      }
    }
  }
  return 0;

}

int * dec_to_bin(int dec_name, int*bin_name, int ind)
{
  if (dec_name == 1)
  {
      bin_name[ind] = 1;
      return bin_name;
  }

  else if ((dec_name %2) ==0)
  {
      dec_name = dec_name/2;
      ind --;
      bin_name = dec_to_bin(dec_name, bin_name, ind);
  }
  else
  {
      dec_name = dec_name/2;
      bin_name[ind] = 1;
      ind --;
      bin_name = dec_to_bin(dec_name, bin_name, ind);
  }
  return bin_name;
}

void cz_ls()
{
  FILE *bin;
  unsigned char directory[1024];
  bin = fopen("simdiskformat.bin","r+");
  fread(directory, sizeof(directory), 1, bin);
  for (int i = 0; i < 64; i++) {
    int bit_actual = 16*i;
    if (directory[bit_actual]=='\x01'){
      bit_actual++;
      int size;
      size = 10;
      for (int j = 0; j < size; j++) {
        printf("%c",directory[bit_actual+j]);

      }
      printf("\n");
    }
  }
}

int cz_write(struct czFILE* file_desc, void* buffer, int nbytes){
   //printf("%c\n", file_desc-> direccion_indice[0]);
}
