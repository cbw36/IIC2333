#include "helperFunctions.h"


/**
 * Helper functions
 */
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

int * dec_to_bin(int dec_name, int*bin_name, int ind)
{
    if (dec_name == 0){
      return bin_name;
    }
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

/**
 *
 * @param index of current block or -1 to search bitmap for a free block
 * @return index of next available block in bits
 * Update value of available block to unavailable
 */
int findNextAvailable(int data_bool)
{
    FILE *bin;
    bin = fopen(disk_path, "r+");  // open bin to find location of next available block from bitmap

    if (data_bool == 0){
        unsigned char bitmap[8192];
        //Find next available bit (block) in bitmap to use for this file
        fseek(bin, 1024, SEEK_SET);  //seek to bitmap block
        fread(bitmap, sizeof(bitmap), 1, bin); //read bitmaps from file to buffer
        int byte_index = 0; //index in bytes of next available block inside bitmap
        int block_index = 0; //Index of next available block in bits (index * 8 + offset in final byte)
        int found_block = 0;
        while (byte_index< sizeof(bitmap)) {
            int cur_byte = bitmap[byte_index]; //decimal representation of current byte of bitmap
            int old_bin_name[8] = {0, 0, 0, 0, 0, 0, 0, 0};  //Convert name to binary
            int *bin_name;
            if (cur_byte ==0)
                bin_name = old_bin_name;
            else
                bin_name = dec_to_bin(cur_byte, old_bin_name, 7);
            for (int i = 0; i < 8; i++) {
                if (bin_name[i] == 0) {
                    bin_name[i] = 1;
                    int addr_dec = 0;
                    for (int j = 0; j < 8; j++)
                        addr_dec += pow(2, j) * bin_name[7 - j];

                    unsigned char arr_addr_dec[1] = {addr_dec};
                    found_block = 1;
                    fseek(bin, 1024 + byte_index, SEEK_SET);
                    fwrite(arr_addr_dec, sizeof(unsigned char), 1, bin);
                    break;
                }
                block_index++;
            }
            if (found_block == 1)
                break;
            byte_index++;
        }

      fclose(bin);
      return block_index;
    }
    else
    {
      unsigned char bitmap[8192];
      //Find next available bit (block) in bitmap to use for this file
      fseek(bin, 1024, SEEK_SET);  //seek to bitmap block
      fread(bitmap, sizeof(bitmap), 1, bin); //read bitmaps from file to buffer
      int byte_index = 8; //index in bytes of next available block inside bitmap
      int block_index = 64; //Index of next available block in bits (index * 8 + offset in final byte)
      int found_block = 0;
      while (byte_index< sizeof(bitmap)) {
        int cur_byte = bitmap[byte_index]; //decimal representation of current byte of bitmap
        int old_bin_name[8] = {0, 0, 0, 0, 0, 0, 0, 0};  //Convert name to binary
        int *bin_name;
        if (cur_byte ==0)
          bin_name = old_bin_name;
        else
          bin_name = dec_to_bin(cur_byte, old_bin_name, 7);
        for (int i = 0; i < 8; i++) {
          if (bin_name[i] == 0) {
            bin_name[i] = 1;
            int addr_dec = 0;
            for (int j = 0; j < 8; j++)
              addr_dec += pow(2, j) * bin_name[7 - j];

            int arr_addr_dec[1] = {addr_dec};
            found_block = 1;
            fseek(bin, 1024 + byte_index, SEEK_SET);
            fwrite(arr_addr_dec, sizeof(int), 1, bin);
            break;
          }
          block_index++;
        }
        if (found_block == 1)
          break;
        byte_index++;
      }
      fclose(bin);
      return block_index;
    }

}
