#ifndef TAREA_3_CZ_API_H
#define TAREA_3_CZ_API_H


struct czFILE
{
    int id;
};

typedef struct czFILE czFile;



czFile* cz_open(char* filename, char mode);


#endif //TAREA_3_CZ_API_H
