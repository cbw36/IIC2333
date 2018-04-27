#include "tlb.h"
#include <stdlib.h>

Tlb* tlb_init()
{ Tlb* tlb = malloc(sizeof(Tlb));
  tlb ->size = 64;
  tlb->fila =malloc(tlb->size * sizeof(Row));
  return tlb;
}
