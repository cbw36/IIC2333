#ifndef TAREA_2_TLB_H
#define TAREA_2_TLB_H


struct row{
  char direccion[20];
  char frame[8];
};

typedef struct row Row;


struct tlb
{
    Row* fila;
    int size;
};
typedef struct tlb Tlb;


Tlb* tlb_init();

#endif //TAREA_2_SIMULATOR_H