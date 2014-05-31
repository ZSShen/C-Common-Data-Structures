#include "dynamic_array.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

int main() {
    int     i, iInitCapacity, iRound;
    double  dExpand, dShrink;
    DynamicArray *pArray;

    /* Initialize the random seed. */
    srand(time(NULL));

    /* Create the DynamicArray structure. */
    iRound = 16;
    iInitCapacity = 1;
    dExpand = 2;
    dShrink = 0.5;
    DynamicArray_init(pArray, iInitCapacity);

    /* Test the resizing feature. */
    for (i = 0 ; i < iRound ; i++) {
        pArray->resize(pArray, dExpand);
        printf("Expand: %d -> %lu\n", i + 1, pArray->capacity(pArray));
    }
    for (i = 0 ; i <= iRound ; i++) {
        pArray->resize(pArray, dShrink);
        printf("Shrink: %d -> %lu\n", i + 1, pArray->capacity(pArray));    
    }

    /* Free the DynamicArray structure. */
    DynamicArray_deinit(pArray);

    return 0;
}
