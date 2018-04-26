// Esta linea sirve para que el código de este archivo solo se importe una vez

/** Estructura de una arraylist */
struct page
{
    char* direccion;
    char* frame;
};

typedef struct page Page;

Page* page_init();
