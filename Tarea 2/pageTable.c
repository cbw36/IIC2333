#include <stdlib.h>
#include "pageTable.h"


Page* page_init(int n)
{ Page* page = malloc(sizeof(Page));
  page ->size = n;
  page->fila =malloc(page->size * sizeof(Row));
  return page;
}