// Esta linea sirve para que el código de este archivo solo se importe una vez
#pragma once


/** Estructura de una arraylist */
struct arraylist_list
{
    int* list;
    int size;
    int largo;
};

// Aquí le estoy poniendo un nombre más simple a la lista para no tener que
// referirme a la lista como struct arraylist_list
/** Estructura de una arraylist */
typedef struct arraylist_list ArrayList;


//////////////////////////////////////////////////////////////////////////
//                             Funciones                                //
//////////////////////////////////////////////////////////////////////////

//OJO: No se debe modificar nada de esto

/** Crea una arraylist inicialmente vacia y retorna su puntero */
ArrayList* arraylist_init();

/** Inserta un elemento al final de la arraylist */
void arraylist_append(ArrayList* list, int element);

int arraylist_delete(ArrayList* list, int position);
/** Libera todos los recursos asociados a la lista */
void arraylist_destroy(ArrayList* list);
