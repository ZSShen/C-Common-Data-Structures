#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>


#define COUNT_CASE          (2 << 12)
#define SIZE_VALUE_SPACE    (2 << 20)
#define SIZE_PRINT_BUF      (2 << 8)


typedef struct RECORD_ {
    int32_t *pKey;
    struct VALUE_ {
        int32_t iMajor;
        int32_t iMinor;
    } *pValue;
} RECORD;


/* The testing suit which inserts integer into the vector. */
void test_primitive();

/* The testing suit which inserts pointers to structures into the vector. */
void test_non_primitive();

/* The item comparator for integer type. */
int32_t func_compare_primitive(Item, Item);

/* The item deallocator for integer type. */
void func_destroy_primitive(Item);

/* The item comparator for type of pointer to structure. */
int32_t func_compare_non_primitive(Item, Item);

/* The item deallocator for type of pointer to structure. */
void func_destroy_non_primitive(Item);


int main()
{
    /* Initialize the random seed. */
    srand(time(NULL));

    printf("====== Test Suit for Primitive Types ======\n");
    test_primitive();
    //printf("====== Test Suit for Non-Primitive Types ======\n");
    //test_non_primitive();

    return 0;
}

void test_primitive()
{
    /* Prepare the testing data. */
    Item *aData = (Item*)malloc(sizeof(Item) * COUNT_CASE);
    if (!aData)
        return;
    uint32_t i, j;
    #if __x86_64__
        int64_t nCase;
    #else
        int32_t nCase;
    #endif
    for (i = 0 ; i < COUNT_CASE ; i++) {
        nCase = rand() % SIZE_VALUE_SPACE;
        aData[i] = (Item)nCase;
    }

    /* Initialize the vector. */
    Vector *pVector;
    int32_t iStat = VectorInit(&pVector);
    if (iStat != SUCCESS)
        goto EXIT;

    /* Set the deallocation function. */
    pVector->set_destroy(pVector, func_destroy_primitive);

    /* Push the first half of the elements into vector. */
    uint32_t uiRange = COUNT_CASE >> 1;
    for (i = 0 ; i < uiRange ; i++)
        pVector->push_back(pVector, aData[i]);

    /* Insert the second half of the elements into vector. */
    for (i = uiRange ; i < COUNT_CASE ; i++)
        pVector->insert(pVector, aData[i], i - uiRange);

    /* Check for the element identity. */
    for (i = 0 ; i < uiRange ; i++)
        assert(aData[i] == pVector->at(pVector, i + uiRange));
    for (i = uiRange ; i < COUNT_CASE ; i++)
        assert(aData[i] == pVector->at(pVector, i - uiRange));

EXIT:
    /* Deinitialize the vector. */
    VectorDeinit(&pVector);

    if (aData)
        free(aData);

    return;
}

int32_t func_compare_primitive(Item itemSrc, Item itemTge)
{
    int32_t iSrc, iTge;

#if __x86_64__
    int64_t lTmp;
    lTmp = (int64_t)itemSrc;
    iSrc = (int32_t)lTmp;
    lTmp = (int64_t)itemTge;
    iTge = (int32_t)lTmp;
#else
    iSrc = (int32_t)itemSrc;
    iTge = (int32_t)itemTge;
#endif

    return iSrc - iTge;
}

void func_destroy_primitive(Item item) { return; }

