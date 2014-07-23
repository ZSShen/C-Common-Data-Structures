#ifndef _H_DYNAMIC_ARRAY_
#define _H_DYNAMIC_ARRAY_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* Wrapper for DynamicArray initialization. */
#define DynamicArray_init(p, q)     p = (DynamicArray*)malloc(sizeof(DynamicArray)); \
                                    if (p != NULL) { \
                                        bool check; \
                                        check = DArrayInit(p, q); \
                                        if (check == false) { \
                                            free(p); \
                                            p = NULL; \
                                        } \
                                    }
                                    

/* Wrapper for DynamicArray deinitialization. */
#define DynamicArray_deinit(p)      if (p != NULL) { \
                                        DArrayDeinit(p); \
                                        free(p); \
                                        p = NULL; \
                                    }


typedef struct _DynamicArray {
    void **array;

    bool          (*put)      (struct _DynamicArray*, void*, unsigned long);
    void*         (*get)      (struct _DynamicArray*, unsigned long);    
    bool          (*resize)   (struct _DynamicArray*, double);
    unsigned long (*capacity) (struct _DynamicArray*);
} DynamicArray;


/**
 * This function is the constructor of DynamicArray structure.
 * @param self          The pointer to the DynamicArray structure.
 * @param ulCapacity    The initial capacity of the requested array. 
 *
 * @return              true : The array is successfully allocated.
 *                      false: The array cannot be allocated due to insufficient memory space.
 */
bool DArrayInit(DynamicArray *self, unsigned long ulCapacity);


/*
 * This function is the destructor of DynamicArray structure.
 *
 * @param self          The pointer to the DynamicArray structure.
 */
void DArrayDeinit(DynamicArray *self);


/**
 * This function inserts an item to the designated index of the array.
 * 
 * @param self          The pointer to the DynamicArray structure.
 * @param pItem         The pointer to the item which is to be inserted.
 * @param ulIndex       The designated index.   
 *
 * @return              true : The item is inserted successfully.
 *                      false: The item cannot be inserted due to illegally designated array index.
 */
bool DArrayPut(DynamicArray *self, void *pItem, unsigned long ulIndex);


/**
 * This function retrives an item from the designated index of the array.
 * @param self          The pointer to the DynamicArray structure.
 * @param ulIndex       The designated index.
 *
 * @return              Non-NULL: The requested item.
 *                      NULL    : The item cannot be retrieved due to illegally designated array index.
 */
void* DArrayGet(DynamicArray *self, unsigned long ulIndex);


/**
 * This function resizes the array with the specified number of times.
 * Note: The number of times should be larger than zero.
 *
 * @param self          The pointer to the DynamicArray structure.
 * @param udTimes       The specified number of times.
 * 
 * @return              true : The array is resized successfully.
 *                      false: The array cannot be resized due to insufficient memory space.
 */
bool DArrayResize(DynamicArray *self, double udTimes);


/**
 * This function returns the capacity of the array.

 * @param self          The pointer to the DynamicArray structure.
 * 
 * @return              The capacity.
 */
unsigned long DArrayCapacity(DynamicArray *self);


#endif
