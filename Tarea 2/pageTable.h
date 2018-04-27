#ifndef TAREA_2_PAGETABLE_H
#define TAREA_2_PAGETABLE_H

#include "tlb.h"


struct page
{
  Row* fila;
  int size;
};

typedef struct page Page;

Page* page_init(int n);


#endif //TAREA_2_SIMULATOR_H
