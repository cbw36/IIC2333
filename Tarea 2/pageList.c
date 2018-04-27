#include "pageList.h"
// Libreria estandar de C
#include <stdlib.h>

PageList* pageList_init(n)
{ PageList* array = malloc(sizeof(PageList));
  array->size = n;
  array->list =malloc(array->size * sizeof(Page));
  return array;
}
