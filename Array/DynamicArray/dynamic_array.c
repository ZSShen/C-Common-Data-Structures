#include "dynamic_array.h"

/*===========================================================================*
 *                  Simulation for private variables                         *
 *===========================================================================*/
static unsigned long _ulCapacity;


/*===========================================================================*
 *                Implementation for exported functions                      *
 *===========================================================================*/
bool DArrayInit(DynamicArray *self, unsigned long ulCapacity) {

    _ulCapacity = ulCapacity;
    self->array = (void**)malloc(sizeof(void*) * _ulCapacity);    
    if (self->array == NULL)
        return false;
    
    /* Let the function pointers point to the corresponding functions. */
    self->put = DArrayPut;
    self->get = DArrayGet;
    self->resize = DArrayResize;
    self->capacity = DArrayCapacity;

    return true;
}


void DArrayDeinit(DynamicArray *self) {

    free(self->array);
    return;
}


/**
 * DArrayPut(): Insert an item to the designated position.
 */
bool DArrayPut(DynamicArray *self, void *pItem, unsigned long ulIndex) {

    /* Reject the invalid index*/
    if (ulIndex >= _ulCapacity)
        return false;

    self->array[ulIndex] = pItem;
    return true;
}


/**
 * DArrayGet(): Retrieve an item from the designated position.
 */
void* DArrayGet(DynamicArray *self, unsigned long ulIndex) {

    /* Reject the invalid index. */
    if (ulIndex >= _ulCapacity)
        return NULL;
    
    return self->array[ulIndex];
}


/**
 * DArrayResize(): Resize the array with the specified number of times.
 */
bool DArrayResize(DynamicArray *self, double udTimes) {
    unsigned long   ulNewCapacity;    
    void            **new;

    ulNewCapacity = _ulCapacity * udTimes;
    if (ulNewCapacity <= 0)
        return false;

    new = realloc(self->array, ulNewCapacity);
    if (new == NULL)
        return false;

    self->array = new;
    _ulCapacity = ulNewCapacity;

    return true;
}


/**
 * DArraySize(): Return the size of the array.
 */
unsigned long DArrayCapacity(DynamicArray *self) {

    return _ulCapacity;
}


