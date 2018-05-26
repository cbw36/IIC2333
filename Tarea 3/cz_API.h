#ifndef TAREA_3_CZ_API_H
#define TAREA_3_CZ_API_H

#include "helperFunctions.h"


struct czFILE
{
    unsigned char addr_index_block[4];
    int read_pos;  //Number of bits read
    int mode_w;
    int mode_r;
    int cur_data_block_index;  //Stores the location of the next 4B pointer within the index block of a file
    unsigned char * metadata;
    int ** data_indices; //TODO not needed?
    int write_pos;

};
typedef struct czFILE czFile;

struct openFiles
{
    czFile* open_files;
};
typedef struct openFiles openFiles;


void disk_init();
int cz_exists(char* filename);
czFile* cz_open(char* filename, char mode);
czFile* cz_init(char* name);
int cz_mv(char* orig, char *dest);
void cz_ls();
int cz_write(struct czFILE* file_desc, char* buffer, int nbytes);
int cz_read(struct czFILE* file_desc, char* buffer, int nbytes);



#endif //TAREA_3_CZ_API_H
