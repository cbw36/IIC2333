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
