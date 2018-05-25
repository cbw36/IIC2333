#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cz_API.h"
#include "helperFunctions.c"
#include <time.h>


void disk_init()
{
    FILE *bin;
    bin = fopen("simdiskformat.bin","r+");  // Write first 9 bitmaps as 1 (FF80) for the directory and bitmap blocks
    unsigned char directory[1024];
    unsigned char bitmap[8094];

    char* first = malloc(2);
    fseek(bin, 1024, SEEK_SET);
    first[0] = '\xFF';
    first[1] = '\x80';
    fwrite(first , 1, 2, bin);

    fseek(bin, 1024, SEEK_SET);
    fread(bitmap, sizeof(bitmap), 1, bin); //read bitmaps from file to buffer

    fclose(bin);
}

czFile* cz_init(char* name) {
    FILE *bin;
    unsigned char bitmap[8192];
    unsigned char directory[1024];

    //Create directory entry for this file: 1st byte valid, 2-12 name 13-16 index of its block
    unsigned char *new_directory = malloc(12);
    new_directory[0] = '\x01';
    for (int i = 1; i < 12; i++) { //read first 10 bytes of name and store 11th as \x00
        if (i - 1 >= strlen(name)) {
            new_directory[i] = '\x00';
        } else {
            new_directory[i] = name[i - 1];
        }
    }

    bin = fopen("simdiskformat.bin", "r+");  // open bin to find location of next available block from bitmap

    int block_index = findNextAvailable(0);  //Address of the index block for the file
    printf("New file's block index at location %i or %x\n", block_index,block_index);

    int dir_index = block_index -9;  /**Update directory to include new file data*/
    fseek(bin, dir_index*16, SEEK_SET);
    fwrite(new_directory,sizeof(char), 12, bin);

    fseek(bin, dir_index * 16 +12, SEEK_SET);
    unsigned char count_c[1] = {block_index};
    fwrite(count_c, sizeof(unsigned char), 1, bin); //TODO CHECK

    unsigned char first[4];  //reverse the order
    fseek(bin, dir_index * 16 +12, SEEK_SET);
    fread(first, sizeof(unsigned char), 4, bin);
    unsigned char aux1[4] = {first[3], first[2], first[1], first[0]};
    fseek(bin, dir_index * 16 +12, SEEK_SET);
    fwrite(aux1, sizeof(unsigned char), 4, bin);


    fseek(bin, block_index*1024, SEEK_SET);
    int t = time(NULL);

    unsigned char metadata[12];  //Metadata excluding time because 0 on initialization
    metadata[0] = 0; metadata[1] = 0; metadata[2] = 0; metadata[3] = 0;
    metadata[4] = (t >> 24) & 0xFF;  metadata[5] = (t >> 16) & 0xFF;  metadata[6] = (t >> 8) & 0xFF;
    metadata[7] = t & 0xFF;  metadata[8] = (t >> 24) & 0xFF;  metadata[9] = (t >> 16) & 0xFF;
    metadata[10] = (t >> 8) & 0xFF;  metadata[11] = t & 0xFF;

    fwrite(metadata, sizeof(unsigned char), 12, bin);

    int first_data_block = findNextAvailable(-1);
    printf("File's first data block at location = %i or %x\n", first_data_block, first_data_block);
    unsigned char first_data_block_arr[4];
    first_data_block_arr[0] = (first_data_block >> 24) & 0xFF;
    first_data_block_arr[1] = first_data_block >> 16 & 0xFF;
    first_data_block_arr[2] = (first_data_block >> 8) & 0xFF;
    first_data_block_arr[3] = first_data_block & 0xFF;
    fwrite(first_data_block_arr, sizeof(unsigned char), 4, bin);


    czFile* file = malloc(sizeof(czFile));
//    file-> addr_index_block = malloc(4 * sizeof(unsigned char));
    file-> addr_index_block[0] = aux1[0];
    file-> addr_index_block[1] = aux1[1];
    file-> addr_index_block[2] = aux1[2];
    file-> addr_index_block[3] = aux1[3];

    file->read_pos = 0;
    file->write_pos = 0;
    file->mode_r=0;
    file->mode_w=0;
    file->cur_data_block_index =first_data_block;
    file->metadata = malloc(12 * sizeof(unsigned char));
    for (int i = 4; i<12;i++)
      file->metadata[i] = metadata[i];
    file->data_indices = malloc(252*4);
    for (int i = 0; i<252;i++)
      file->data_indices[i] = malloc(4);
    for (int i = 0; i<4; i++)
      file->data_indices[0][i]=  first_data_block_arr[i];
    fclose(bin);
    return file;
} /** Finished, still testing */


int cz_exists(char* filename) /** Tested and Finished*/
{
    FILE *bin;
    unsigned char directory[1024];
    bin = fopen("simdiskformat.bin","r+");
    fread(directory, sizeof(directory), 1, bin);
    for (int i = 0; i < 64; i++) {
        int bit_actual = 16*i;
        int equal = 1;
        if (directory[bit_actual]=='\x01'){ //Valid directory entry
            bit_actual++;
            int size;
            if (strlen(filename)< 11)
                size = strlen(filename);
            else
                size = 10;

            for (int j = 0; j < size; j++) {
                if (filename[j] != directory[bit_actual+j])
                    equal = 0;
            }
            if (directory[bit_actual+size]!= '\x00')
                equal = 0;
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

/** Tested and Finished*/
czFile* cz_open(char* filename, char mode) //TODO What does it mean to be open? Add to a list somewhere... Need to restric behaviors (other than cant write<->read)
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
          if (filename[j] != directory[bit_actual+j])
            equal = 0;
        }
        if (directory[bit_actual+size]!= '\x00')
            equal = 0;
        bit_actual = bit_actual + 11;
        if (equal == 1){  //If file exists, return return czFILE* that represents it
          czFile* current_file = malloc(sizeof(czFile));
//          current_file -> addr_index_block = malloc(4 * sizeof(unsigned char));
          for (int j = 0; j < 4; j++)
            current_file -> addr_index_block[j] = directory[bit_actual+j];
          current_file->mode_w = 0;
          current_file->mode_r = 0;  //TODO GET WRITE/READ POSITIONS (NEEDED TO EXECUTE WRITE/READ)

          return current_file;
        }
      }
    }
    if (equal == 0){  //If file doesnt exist, return NULL
      fclose(bin);
      return NULL;
    }
  }

  else if (mode =='w'){
    if (!cz_exists(filename)){
        printf("filename doesnt exist yet\n");
        czFile* current_file = cz_init(filename);
        fclose(bin);
        return current_file;
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
  return 0;  //TODO should be 1?

}

void cz_ls() /** Tested and Finished*/
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

int cz_write(struct czFILE* file_desc, char* buffer, int nbytes)  /**Functional, but lacks check that there is free blocks, just creates a new one every time. And doesnt use indirect addresing and doesnt update modification time*/
{
  if (file_desc->mode_r==1){
    printf("Can't write because in read mode");
    return -1;
  }

  else {
    int rem_bytes = nbytes;
    file_desc->mode_w = 1;

    FILE *bin;
    bin = fopen("simdiskformat.bin","r+");
    char index_block[1024];
    char * cur_data_block;


    int write_pos = file_desc->write_pos;
    int index_block_addr = (file_desc->addr_index_block[0]*16777216 +file_desc->addr_index_block[1] * 65536 + file_desc->addr_index_block[2] * 256
                            + file_desc->addr_index_block[3]) * 1024;
    fseek(bin, index_block_addr, SEEK_SET);  //Seek to index block and read it
    fread(index_block, sizeof(index_block), 1, bin);
    int data_block_addr;
    int num_blocks = nbytes / 1024;  //Number of blocks needed to parse to read nbytes of data
    char * ready_buf;  //Buffer to read subset of entire buffer on each iteration
    int num_iter = 0;
    while (num_blocks > -1 ) {
      cur_data_block = malloc(1024 * sizeof(char));
      if (num_iter>0)
      {
        data_block_addr= findNextAvailable(1);
        printf("Create new block for data at location = %i or %x\n", data_block_addr, data_block_addr);
        unsigned char  next_data_block_arr[4];
        next_data_block_arr[0] = (data_block_addr >> 24) & 0xFF;
        next_data_block_arr[1] = (data_block_addr >> 16) & 0xFF;
        next_data_block_arr[2] = (data_block_addr >> 8) & 0xFF;
        next_data_block_arr[3] = data_block_addr & 0xFF;
        fseek(bin, index_block_addr + 4*num_iter + 12, SEEK_SET);
        fwrite(next_data_block_arr, sizeof(unsigned char), 4, bin);
      }
      else
      {
        int data_block_ind = (write_pos / 1024) * 4;  //Each data block can store 1024 bytes of data
        data_block_addr =  (index_block[data_block_ind+12]*16777216 + index_block[data_block_ind+13]*65536 +
                                index_block[data_block_ind+14]*256 + index_block[data_block_ind+15]);
        printf("dont create new data block.  Location = %i or %x\n", data_block_addr,data_block_addr);
      }
      fread(cur_data_block, sizeof(cur_data_block), 1, bin);

      if (((write_pos%1024)>rem_bytes) || ((write_pos%1024 == 0)&&(rem_bytes<1024)))
      {
        fseek(bin, data_block_addr * 1024, SEEK_SET);
        ready_buf = malloc(rem_bytes* sizeof(char));
        int i = 0;
        for (int ind = nbytes - rem_bytes; ind<nbytes; ind ++)
        {
          ready_buf[i] = buffer[ind];
          i++;
        }
        printf("ready buff = %i\n",rem_bytes);
        fwrite(ready_buf, rem_bytes, 1, bin);
        write_pos += rem_bytes;
        printf("Written all bytes succesfully\n");
        file_desc->write_pos = write_pos;
        return nbytes;
      }
      else if (write_pos%1024 != 0)
      {
        fseek(bin, data_block_addr * 1024, SEEK_SET);
        int num_bytes_write = 1024 - write_pos%1024;
        printf("write %i bytes\n", num_bytes_write);
        int ind = 0;
        ready_buf = malloc(num_bytes_write* sizeof(char));
        for (int i = sizeof(buffer)/nbytes - rem_bytes; i<num_bytes_write; i ++)
        {
          ready_buf[ind] = buffer[i];
          ind ++;
        }
        fwrite(ready_buf, num_bytes_write, 1, bin);
        rem_bytes -= num_bytes_write;
        write_pos += num_bytes_write;
      }
      else
      {
        //Write 1024 bytes
        fseek(bin, data_block_addr * 1024, SEEK_SET);
        ready_buf = malloc(1024);
        int ind = 0;
        for (int i = nbytes - rem_bytes; i< nbytes - rem_bytes+1024; i ++)
        {
          ready_buf[ind] = buffer[i];
          ind++;
        }
        fwrite(ready_buf, 1024,1, bin);

        rem_bytes -= 1024;
        write_pos += 1024;
      }
      num_iter++;
      num_blocks--;
    }
    file_desc->write_pos = write_pos;
    int num_read = nbytes - num_read;
    return num_read;

  }
}

int cz_read(struct czFILE* file_desc, char* buffer, int nbytes)  //TODO TEST!
{
    if (file_desc->mode_w==1){
        printf("Cannot read file.  Perhaps in write mode");
        return 1;
    }
    else
    {
        int rem_bytes = nbytes;
        buffer = malloc(nbytes * sizeof(char));
        file_desc->mode_r = 1;

        FILE *bin;
        bin = fopen("simdiskformat.bin","r+");
        char index_block[1024];
        char *cur_data_block;

        int read_pos = file_desc->read_pos;
        int index_block_addr = (file_desc->addr_index_block[0]*16777216 +file_desc->addr_index_block[1] * 65536 + file_desc->addr_index_block[2]
                                                                         * 256 + file_desc->addr_index_block[3]) * 1024;

        fseek(bin, index_block_addr, SEEK_SET);  //Seek to index block and read it
        fread(index_block, sizeof(index_block), 1, bin);

        int num_blocks = nbytes / 1024;  //Number of blocks needed to parse to read nbytes of data
        while (num_blocks > -1 )
        {
            cur_data_block = malloc(1024 * sizeof(char));
            int data_block_ind = (read_pos / 1024)*4;  //Each data block can store 1024 bytes of data
            int data_block_addr =  (index_block[data_block_ind+12]*16777216 + index_block[data_block_ind+13]*65536 +
                  index_block[data_block_ind+14]*256 + index_block[data_block_ind+15])* 1024;

            fseek(bin, data_block_addr, SEEK_SET);
            fread(cur_data_block, sizeof(cur_data_block), 1, bin);

            if (((read_pos%1024)>rem_bytes) || ((read_pos%1024 == 0)&&(rem_bytes<1024)))
            {
                printf("11111111\n");
                fread(buffer, rem_bytes, 1, bin);
                read_pos += rem_bytes;
                rem_bytes = 0;
                printf("Read all bytes succesfully\n");
                for (int i = 0;i<nbytes;i++)
                  printf("%c\n",buffer[i]);
                file_desc->read_pos = read_pos;
                return nbytes;
            }
            else if (read_pos%1024 != 0)
            {
                printf("2222222\n");
                fread(buffer, read_pos%1024, 1, bin);
                rem_bytes -= read_pos % 1024;
                read_pos += (read_pos%1024);

            }
            else
            {
              printf("333333\n");
              fread(buffer, 1024, 1, bin);
              rem_bytes -= 1024;
              read_pos += 1024;
            }

            num_blocks--;
        }
        printf("Ran out of memory space.  Read %i bytes\n", nbytes-rem_bytes);
        file_desc->read_pos = read_pos;
        return (nbytes-rem_bytes);
    }
}

int cz_close(struct czFILE* file_desc)  //TODO Update
{
    //TODO not sure what else to do. It says make sure changes are updated, but shouldnt that be done in the read/write/open functions internally?
    file_desc->mode_w = 0;
    file_desc->mode_r = 0;
    return 0;

}


int cz_cp(char* orig, char* dest)  //TODO
{
  return -1;
}

int cz_rm(char* filename){ //TODO
  return -1;
}







//printf("%i%i%i%i%i%i%i%i \n", bin_name[0], bin_name[1],bin_name[2], bin_name[3],bin_name[4], bin_name[5],bin_name[6], bin_name[7]);
