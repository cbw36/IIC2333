#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cz_API.h"



//czFile* cz_init(char* name, int size, char* data) //TODO UPDATE
void cz_init(char* name, int size, char* data) {
    FILE *bin;
    unsigned char bitmap[8192];
    unsigned char directory[1024];

    char valid[] = "1";
    char invalid[] = "0";

    bin = fopen("simdiskformat.bin", "r+");  // open bin to find location of next available block from bitmap

    fseek(bin, 1024, SEEK_SET);  //seek to bitmap block
    fread(bitmap, sizeof(bitmap), 1, bin); //read bitmap from file to buffer (all 8)
    int index = 0; //index inside bitmap of next available block
    while (bitmap[index] != 0)
        index++;

    fseek(bin, 1024 + index, SEEK_SET);  //change bitmap at index from 0 to 1
    fwrite(valid, 1, 1, bin);
    int ind_directory = (index - 9) * 16;  //index of file in directory block
    fseek(bin, ind_directory, SEEK_SET);

    fwrite(valid, 1, 1, bin); //First byte: valid

    char ascii_name[1];  //Next 11 bytes: convert name to ascii
    for (int i = 0; i < strlen(name); i++) {  //write name as next 11 bytes TODO truncate name if too long
        ascii_name[0] = name[i];
        fwrite(ascii_name, 1, 1, bin);
    }
    for (int i = strlen(name); i < 11; i++) { //If name less than 11 bytes add 0's
        fwrite(invalid, 1, 1, bin);
    }

    char ind_hex_[6];  // Next 4 bytes: index of its block
    char ind_hex[4];
    sprintf(ind_hex_, "%x ", index);
    for (int i = 0; i<4; i++) {
        if ((ind_hex_[i] == 32) || (ind_hex_[i] == 0))
            ind_hex[i] = NULL;
        else
            ind_hex[i] = ind_hex_[i];
    }
    fwrite(ind_hex, 1, sizeof(ind_hex), bin);


    //Write to the files index block
    fseek(bin, index * 1024, SEEK_SET);
    char size_hex_[6];  // First 4 bytes: size
    char size_hex[4];
    sprintf(size_hex_, "%x ", size);
    for (int i = 0; i<4; i++) {
        if ((size_hex_[i] == 32) || (size_hex_[i] == 0))
            size_hex[i] = NULL;
        else
            size_hex[i] = size_hex_[i];
    }
    fwrite(size_hex, 1, sizeof(size_hex), bin);


    unsigned t = time(NULL); //Next 4 bytes: time
    char time[] = "0000";  //TODO ACTUALLY GET TIME
    fwrite(time, 1, sizeof(time), bin);

    fwrite(time, 1, sizeof(time), bin);//Next 4 time of modification TODO

    //Next 1008 bytes is pointers to indices of free blocks
    while(1) //Find a free block, fill it with data, repeat until no more data
    {
        fseek(bin, 1024, SEEK_SET);  //Find next free block
        fread(bitmap, sizeof(bitmap), 1, bin); //read bitmap from file to buffer (all 8)
        index = 0; //index inside bitmap of next available block
        while (bitmap[index] != 0)
            index++;
        
        if (data == NULL)
            break;
    }





    //TESTING
    fseek(bin, 0, SEEK_SET);  //seek to bitmap block
    fread(directory, sizeof(directory), 1, bin); //read bitmap from file to buffer
    printf("DIRECTORY");
    for (int i = 0; i<300; i++)
        printf("%d ", directory[i]);
    printf("\n BITMAP");
    fread(bitmap,1000, 1, bin); //read bitmap from file to buffer
    for (int i = 0; i<sizeof(bitmap); i++)
        printf("%d ", bitmap[i]);
    fclose(bin);


}


void disk_init()
{
    FILE *bin;
    char valid[] = "1";
    bin = fopen("simdiskformat.bin","r+");  // Write first 9 bitmaps as 1 (directory/bitmap) and then a few more for artificial files
    fseek(bin, 1024, SEEK_SET);
    for (int i = 0; i<9;i++)
        fwrite(valid , 1, 1 , bin);
    fclose(bin);
}

//Binary conversion
//Commented binary conversion
//    char dec_name;
//    for (int i =0; i<strlen(name);i++){
//        char * bin_name[8];
//        for (int i = 0; i<8;i++)
//            bin_name[i] = 0;
//        sprintf(ascii_name, "%d ", name[i]);
//        dec_name = atoi(ascii_name);
//        int ind = 7;
//        while (ind != -1)
//        {
//            if (dec_name == 1)
//            {
//                bin_name[ind] = 1;
//                ind = -1;
//            }
//
//            else if ((dec_name %2) ==0)
//            {
//                dec_name = dec_name/2;
//                ind --;
//            }
//            else
//            {
//                dec_name = dec_name/2;
//                bin_name[ind] = 1;
//                ind --;
//            }
//        }
//    }
//