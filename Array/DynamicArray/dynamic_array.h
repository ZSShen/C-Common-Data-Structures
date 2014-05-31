#ifndef _H_DYNAMIC_ARRAY_
#define _H_DYNAMIC_ARRAY_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* Wrapper for dynamic array initialization. */
#define DynamicArray_init(p)        p = malloc(sizeof(DynamicArray)); \
                                    if (p != NULL) { \
                                        bool check; \
                                        check = DArrayInit(p); \
                                        if (check == false) { \
                                            free(p); \
                                            p = NULL; \
                                        } \
                                    }
                                    

/* Wrapper for dynamic array deinitialization. */
#define DynamicArray_deinit(p)      if (p != NULL) { \
                                        DArrayDeinit(p); \
                                        p = NULL; \
                                    }


typedef struct _DynamicArray {
    void **array;

    bool          (*put) (struct _DynamicArray*, void*, unsigned long);
    void*         (*get) (struct _DynamicArray*, unsigned long);    
    unsigned long (*size)(struct _DynamicArray*);
} DynamicArray;


/* Constructor for dynamic array. */
bool DArrayInit(DynamicArray *self);


/* Destructor for dynamic array. */
void DArrayDeinit(DynamicArray *self);


bool DArrayPut(DynamicArray *self, void *pItem, unsigned long ulIndex);


void* DArrayGet(DynamicArray *self, unsigned long ulIndex);


unsigned long DArraySize(DynamicArray *self);


#endif
