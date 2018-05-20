#ifndef TAREA_3_CZ_API_H
#define TAREA_3_CZ_API_H


struct czFILE
{
    int id;
};

typedef struct czFILE czFile;


//czFile* cz_init(char* name, int size, char* data); //TODO UPDATE
void cz_init(char* name, int size, char* data);
czFile* cz_open(char* filename, char mode);
void disk_init();


#endif //TAREA_3_CZ_API_H
