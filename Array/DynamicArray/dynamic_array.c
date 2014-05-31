#include "dynamic_array.h"

/*===========================================================================*
 *                  Simulation for private variables                         *
 *===========================================================================*/
static unsigned long ulCapacity, ulSize;


/*===========================================================================*
 *                Implementation for exported functions                      *
 *===========================================================================*/
bool DArrayInit(DynamicArray *self) {

    ulCapacity = 1;
    ulSize = 0;
    self->array = (void**)malloc(sizeof(void*) * ulCapacity);    
    if (self->array == NULL)
        return false;
    
    /* Let the function pointers point to the corresponding functions. */
    self->put = DArrayPut;
    self->get = DArrayGet;
    self->size = DArraySize;

    return true;
}


void DArrayDeinit(DynamicArray *self) {

    free(self->array);
    return;
}


bool DArrayPut(DynamicArray *self, void *pItem, unsigned long ulIndex);


void* DArrayGet(DynamicArray *self, unsigned long ulIndex);


unsigned long DArraySize(DynamicArray *self);
