#include "dynamic_array.h"

#define DEFAULT_AUTO_RESIZE 2


/*===========================================================================*
 *                  Simulation for private variables                         *
 *===========================================================================*/
static unsigned long _ulSize;
static unsigned long _ulCapacity;
static void (*_pDestroy) (void*);


/*===========================================================================*
 *                 Definition for internal functions                         *
 *===========================================================================*/
/**
 * This function is the default strategy to deallocate an array item.
 *
 * @param ulIndex       The pointer to the item which is to be deallocated.        
 */
void _DArrayItemDestroy(void *pItem);


/*===========================================================================*
 *                Implementation for exported functions                      *
 *===========================================================================*/
bool DArrayInit(DynamicArray *self, unsigned long ulCapacity) {
    unsigned long i;

    _ulCapacity = ulCapacity;
    _ulSize = 0;
    self->array = (void**)malloc(sizeof(void*) * _ulCapacity);    
    if (self->array == NULL)
        return false;
    
    /* Let all the elements initially point to NULL. */
    for (i = 0 ; i < _ulCapacity ; i++) {
        self->array[i] = NULL;
    }

    /* Let the function pointers point to the corresponding functions. */
    _pDestroy = _DArrayItemDestroy;

    self->put = DArrayPut;
    self->get = DArrayGet;
    self->delete = DArrayDelete;
    self->resize = DArrayResize;
    self->size = DArraySize;
    self->capacity = DArrayCapacity;
    self->set_destroy = DArraySetDestroy;

    return true;
}


void DArrayDeinit(DynamicArray *self) {
    unsigned long i;
    
    for (i = 0 ; i < _ulCapacity ; i++) {
        if (self->array[i] != NULL) {
            _pDestroy(self->array[i]);
        }
    }
    free(self->array);

    return;
}


/**
 * DArrayPut(): Insert an item to the designated index of the array.
 */
bool DArrayPut(DynamicArray *self, void *pItem, unsigned long ulIndex) {

    /* If the current size equals the capacity, the array should automatically 
       extend its capacity. But if the extension is failed, the array will 
       delay the operation until next trial. */
    if (_ulSize == _ulCapacity) {
        DArrayResize(self, DEFAULT_AUTO_RESIZE);
    }

    /* Reject the invalid index*/
    if (ulIndex >= _ulCapacity) {
        return false;
    }

    self->array[ulIndex] = pItem;
    _ulSize++;

    return true;
}


/**
 * DArrayGet(): Retrieve an item from the designated index of the array.
 */
void* DArrayGet(DynamicArray *self, unsigned long ulIndex) {

    /* Reject the invalid index. */
    if (ulIndex >= _ulCapacity) {
        return NULL;
    }
    
    return self->array[ulIndex];
}


/**
 * DArrayDelete(): Delete an item from the designated index of the array.
 */
bool DArrayDelete(DynamicArray *self, unsigned long ulIndex) {
    void *pItem;

    /* Reject the invalid index. */
    if (ulIndex >= _ulCapacity)
        return false;

    pItem = self->array[ulIndex];
    _pDestroy(pItem);
    self->array[ulIndex] = NULL;
    _ulSize--;

    return true;
}


/**
 * DArrayResize(): Resize the array with the specified number of times.
 */
bool DArrayResize(DynamicArray *self, double udTimes) {
    unsigned long   i, ulNewCapacity;    
    void            **new;

    ulNewCapacity = _ulCapacity * udTimes;
    if (ulNewCapacity <= 0)
        return false;

    new = realloc(self->array, ulNewCapacity * sizeof(void*));
    if (new == NULL)
        return false;
    self->array = new;

    /* For space expansion, let all the newly allocated elements point to NULL. */
    if (ulNewCapacity > _ulCapacity) {
        for (i = _ulCapacity ; i < ulNewCapacity ; i++) {
            self->array[i] = NULL;
        }    
    }

    _ulCapacity = ulNewCapacity;

    return true;
}


/**
 * DArraySize(): Return the size of the array.
 */
unsigned long DArraySize(DynamicArray *self) {

    return _ulSize;
}


/**
 * DArrayCapacity(): Return the capacity of the array.
 */
unsigned long DArrayCapacity(DynamicArray *self) {

    return _ulCapacity;
}


/**
 * DArraySetDelete(): Set the item deletion strategy with the one defined by user.
 */
void DArraySetDestroy(DynamicArray *self, void (*pFunc)(void*)) {

    _pDestroy = pFunc;
    return;
}


/*===========================================================================*
 *                 Definition for internal functions                         *
 *===========================================================================*/
/**
 * _DArrayItemDelete(): The default deallocation strategy for an item.
 * Note: By default, the item is a primitive data, and thus no further operations are required.
 */
void _DArrayItemDestroy(void *pItem) {

    return;
}
