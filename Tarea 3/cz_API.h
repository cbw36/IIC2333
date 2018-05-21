#ifndef TAREA_3_CZ_API_H
#define TAREA_3_CZ_API_H


struct czFILE
{
    char* direccion_indice;
};

typedef struct czFILE czFile;


//czFile* cz_init(char* name, int size, char* data); //TODO UPDATE
void disk_init();
int cz_exists(char* filename);
czFile* cz_open(char* filename, char mode);
char* hex_to_bin(char hex);
czFile*  cz_init(char* name);
int * dec_to_bin(int dec_name, int*bin_name, int ind);
int cz_mv(char* orig, char *dest);
void cz_ls();
int cz_write(struct czFILE* file_desc, void* buffer, int nbytes);


#endif //TAREA_3_CZ_API_H
