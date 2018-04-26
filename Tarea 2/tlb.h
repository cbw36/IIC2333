typedef struct row{
  char direccion[20];
  char frame[8];
}Row;


struct tlb
{
    row* fila;
    int size;
};
typedef struct tlb Tlb;


Tlb* tlb_init();
