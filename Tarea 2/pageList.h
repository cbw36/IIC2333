#ifndef TAREA_2_PAGELIST_H
#define TAREA_2_PAGELIST_H

#pragma once
#include "pageTable.h"


struct pageList
{
    Page* list;
    int size;
};

typedef struct pageList PageList;


PageList* pageList_init(int n);

#endif //TAREA_2_SIMULATOR_H