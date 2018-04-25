#include "arraylist.h"
// Libreria estandar de C
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
//                             Funciones                                //
//////////////////////////////////////////////////////////////////////////

// TODO: Debes completar las siguientes funciones
// Puedes crear otras funciones aca para el
// funcionamiento interno del arreglo dinamico

/** Crea una arraylist inicialmente vacia y retorna su puntero */
ArrayList* arraylist_init()
{ ArrayList* array = malloc(sizeof(ArrayList));
  array->size = 2;
  array->largo = 0;
  array->list =malloc(array->size * sizeof(int));
  return array;
}

/** Inserta un elemento al final de la arraylist */
void arraylist_append(ArrayList* list, int element)
{if (list->largo == list->size){
    list->list = realloc(list->list, list->size*2* sizeof(int));
    list->size *= 2;
  }
  list->list[list->largo] = element;
  list->largo += 1;

}


/** Libera todos los recursos asociados a la lista */
void arraylist_destroy(ArrayList* list)
{free(list);
  free(list->list);
}