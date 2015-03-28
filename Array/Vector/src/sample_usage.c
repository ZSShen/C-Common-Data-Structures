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
    int32_t *pValue;
} RECORD;


/* The testing suit which inserts integer into the vector. */
void test_primitive();

/* The testing suit which inserts pointers to structures into the vector. */
void test_non_primitive();

/* The item deallocator for integer type. */
void func_destroy_primitive(Item);

/* The item deallocator for type of pointer to structure. */
void func_destroy_non_primitive(Item);


int main()
{
    /* Initialize the random seed. */
    srand(time(NULL));

    printf("====== Test Suit for Primitive Types ======\n");
    test_primitive();
    printf("====== Test Suit for Non-Primitive Types ======\n");
    test_non_primitive();

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
    /* Since our testing data is primitive type, we do not need the
       resource clean strategy. */

    /* Push the first half of the elements into vector. */
    uint32_t uiRange = COUNT_CASE >> 1;
    int32_t iRtnCode;
    for (i = 0 ; i < uiRange ; i++) {
        iRtnCode = pVector->push_back(pVector, aData[i]);
        if (iRtnCode != SUCCESS)
            goto EXIT;
    }

    /* Insert the second half of the elements into vector. */
    for (i = uiRange ; i < COUNT_CASE ; i++) {
        iRtnCode = pVector->insert(pVector, aData[i], i - uiRange);
        if (iRtnCode != SUCCESS)
            goto EXIT;
    }

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

void test_non_primitive()
{
    /* Prepare the testing data. */
    RECORD *aRecord = (RECORD*)malloc(sizeof(RECORD) * COUNT_CASE);
    if (!aRecord)
        return;

    uint32_t i, j;
    for (i = 0 ; i < COUNT_CASE ; i++) {
        aRecord[i].pKey = NULL;
        aRecord[i].pValue = NULL;
    }

    int32_t iKey;
    for (i = 0 ; i < COUNT_CASE ; i++) {
        iKey = rand() % SIZE_VALUE_SPACE;
        aRecord[i].pKey = (int32_t*)malloc(sizeof(int32_t));
        if (!(aRecord[i].pKey))
            goto EXIT;
        aRecord[i].pValue = (int32_t*)malloc(sizeof(int32_t));
        if (!(aRecord[i].pValue))
            goto EXIT;
        *(aRecord[i].pKey) = iKey;
        *(aRecord[i].pValue) = 0;
    }

    /* Initialize the vector. */
    Vector *pVector;
    int32_t iStat = VectorInit(&pVector);
    if (iStat != SUCCESS)
        goto EXIT;

    /* Set the deallocation function. */
    pVector->set_destroy(pVector, func_destroy_non_primitive);

    /* Push the first half of the elements into vector. */
    uint32_t uiRange = COUNT_CASE >> 1;
    int32_t iRtnCode;
    for (i = 0 ; i < uiRange ; i++) {
        iRtnCode = pVector->push_back(pVector, (Item)&aRecord[i]);
        if (iRtnCode != SUCCESS)
            goto EXIT;
    }

    /* Insert the second half of the elements into vector. */
    for (i = uiRange ; i < COUNT_CASE ; i++) {
        iRtnCode = pVector->insert(pVector, (Item)&aRecord[i], i - uiRange);
        if (iRtnCode != SUCCESS)
            goto EXIT;
    }

    /* Check for the element identity. */
    for (i = 0 ; i < uiRange ; i++) {
        RECORD *pRec = (RECORD*)pVector->at(pVector, i + uiRange);
        assert(*(aRecord[i].pKey) == *(pRec->pKey));
    }

    /* Pop the second half of the vector. */
    for (i = 0 ; i < uiRange ; i++)
        pVector->pop_back(pVector);

    /* Check for the element identity. */
    for (i = 0 ; i < uiRange ; i++) {
        RECORD *pRec = (RECORD*)pVector->at(pVector, i);
        assert(*(aRecord[i + uiRange].pKey) == *(pRec->pKey));
    }

    /* Delete the first quarter of the vector. */
    uiRange <<= 1;
    for (i = 0 ; i < uiRange ; i++)
        pVector->delete(pVector, 0);

DEINIT:
    /* Deinitialize the vector. */
    VectorDeinit(&pVector);

EXIT:
    if (aRecord) {
        for (i = 0 ; i < COUNT_CASE ; i++) {
            if (aRecord[i].pKey)
                free(aRecord[i].pKey);
            if (aRecord[i].pValue)
                free(aRecord[i].pValue);
        }
        free(aRecord);
    }

    return;
}

void func_destroy_primitive(Item item) { return; }

void func_destroy_non_primitive(Item item)
{
    RECORD *pRec = (RECORD*)item;
    free(pRec->pKey);
    free(pRec->pValue);
    pRec->pKey = NULL;
    pRec->pValue = NULL;

    return;
}
