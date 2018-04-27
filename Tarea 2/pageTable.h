// Esta linea sirve para que el código de este archivo solo se importe una vez

/** Estructura de una arraylist */
#include "tlb.h"


struct page
{
  Row* fila;
  int size;
};

typedef struct page Page;

Page* page_init(int n);
