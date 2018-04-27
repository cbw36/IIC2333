#pragma once
#include "pageTable.h"


/** Estructura de una arraylist */
struct pageList
{
    Page* list;
    int size;
};

// Aquí le estoy poniendo un nombre más simple a la lista para no tener que
// referirme a la lista como struct arraylist_list
/** Estructura de una arraylist */
typedef struct pageList PageList;


//////////////////////////////////////////////////////////////////////////
//                             Funciones                                //
//////////////////////////////////////////////////////////////////////////

//OJO: No se debe modificar nada de esto

/** Crea una arraylist inicialmente vacia y retorna su puntero */
PageList* pageList_init(int n);
