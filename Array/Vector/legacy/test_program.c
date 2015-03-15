#include "dynamic_array.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SIZE_DATA           10000
#define DIVISION_POLICY     0.5

typedef struct _MinorData {
    int keyMinor;
    int valMinor;
} MinorData;


typedef struct _MajorData {
    int       keyMajor;
    MinorData *pValMajor;
} MajorData;


/* Define the item deallocation strategy for DynamicArray. */
void data_destroy(void *pItem);

/* Test function for insertion-related utilites. */
void test_insert(DynamicArray *pArray);

/* Test function for deletion-related utilities. */
void test_delete(DynamicArray *pArray);


int main() {
    int     i, iInitCapacity;
    DynamicArray *pArray;

    /* Initialize the random seed. */
    srand(time(NULL));

    /* Create the DynamicArray. */
    iInitCapacity = 1;
    DynamicArray_init(pArray, iInitCapacity);

    if (pArray != NULL) {
        /* Customize the item deallocation function. */
        pArray->set_destroy(pArray, data_destroy);

        /* Test the primary features of DynamicArray. */
        test_insert(pArray);
        test_delete(pArray);

        /* Free the DynamicArray. */
        DynamicArray_deinit(pArray);
    }

    return 0;
}


void data_destroy(void *pItem) {
    MajorData *pMajor;

    if (pItem != NULL) {
        pMajor = (MajorData*)pItem;
        if (pMajor->pValMajor != NULL) {
            free(pMajor->pValMajor);
        }
        free(pMajor);
    }

    return;
}


void test_insert(DynamicArray *pArray) {
    unsigned long i;
    MajorData *pMajor;    
    
    for (i = 0 ; i < SIZE_DATA ; i++) {
        pMajor = (MajorData*)malloc(sizeof(MajorData));
        pMajor->keyMajor = i;
        pMajor->pValMajor = (MinorData*)malloc(sizeof(MinorData));
        pMajor->pValMajor->keyMinor = i;
        pMajor->pValMajor->valMinor = i;
        
        pArray->put(pArray, pMajor, i);
    }

    return;
}


void test_delete(DynamicArray *pArray) {
    unsigned long i, ulSize, ulCapacity;
    MajorData *pMajor;

    /* Delete the front half of the array elements. */
    ulSize = pArray->size(pArray);
    ulCapacity = pArray->capacity(pArray);
    for (i = ulSize - 1 ; i >= ulCapacity * DIVISION_POLICY ; i--) {
        pMajor = pArray->get(pArray, i);
        /* printf("Major Key:%5d\tMinor Key:%5d\n", pMajor->keyMajor, pMajor->pValMajor->keyMinor); */
        pArray->delete(pArray, i);
    }

    /* Shrink the array. */
    pArray->resize(pArray, DIVISION_POLICY);

    /* Delete the rear half of the array elements. */    
    ulCapacity = pArray->capacity(pArray);
    for (i = ulCapacity - 1 ; i >= 1 ; i--) {
        pMajor = pArray->get(pArray, i);
        /* printf("Major Key:%5d\tMinor Key:%5d\n", pMajor->keyMajor, pMajor->pValMajor->keyMinor); */
        pArray->delete(pArray, i);
    }

    return;
}
