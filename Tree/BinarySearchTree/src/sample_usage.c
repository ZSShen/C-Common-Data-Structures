#include "bin_search_tree.h"
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


/* The item comparator for integer type. */
int32_t func_compare_primitive(Item, Item);

/* The item deallocator for integer type. */
void func_destroy_primitive(Item);

/* The testing suit which inserts integer into the tree. */
void test_primitive();

/* The testing suit which inserts pointers to structures into the tree. */
void test_non_primitive();

/* The item comparator for type of pointer to structure. */
int32_t func_compare_non_primitive(Item, Item);

/* The item deallocator for type of pointer to structure. */
void func_destroy_non_primitive(Item);


int main()
{
    /* Initialize the random seed. */
    srand(time(NULL));

    //test_primitive();
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
        do {
            nCase = rand() % SIZE_VALUE_SPACE;
            bool bDup = false;
            for (j = 0 ; j < i ; j++) {
                if (aData[j] == (Item)nCase) {
                    bDup = true;
                    break;
                }
            }
            if (bDup == false)
                break;
        } while (true);
        aData[i] = (Item)nCase;
    }

    /* Initialize the tree. */
    BinSearchTree *pTree;
    int32_t iStat = BSTreeInit(&pTree);
    if (iStat != SUCCESS)
        goto EXIT;

    /* Set the comparison and deallocation functions. */
    pTree->set_compare(pTree, func_compare_primitive);
    pTree->set_destroy(pTree, func_destroy_primitive);

    /* Insert integers into the tree. */
    for (i = 0 ; i < COUNT_CASE ; i++) {
        iStat = pTree->insert(pTree, (Item)aData[i]);
        if (iStat == FAIL_DATA_CONFLICT);
            /* Handle data conflict if necessary. */
        else if (iStat == FAIL_NO_MEMORY)
            goto EXIT;
    }

    /* Search integers stored in the tree. */
    for (i = 0 ; i < COUNT_CASE ; i++) {
        iStat = pTree->search(pTree, (Item*)&aData[i]);
        assert(iStat == SUCCESS);
    }

    /* Delete parts of the integers stored in the tree. */
    uint32_t uiSize = pTree->size(pTree);
    for (i = 0 ; i < uiSize / 2 ; i++) {
        pTree->delete(pTree, (Item)aData[i]);
        aData[i] = (Item)-1; // Mark it as deleted element.
    }

    /* Search integers stored in the tree again. */
    for (i = 0 ; i < COUNT_CASE ; i++) {
        if (aData[i] != (Item)-1) {
            iStat = pTree->search(pTree, (Item*)&aData[i]);
            assert(iStat == SUCCESS);
        }
    }

EXIT:
    /* Deinitialize the tree. */
    BSTreeDeinit(&pTree);

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

void func_destroy_primitive(Item item)
{ return; }


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
        do {
            iKey = rand() % SIZE_VALUE_SPACE;
            bool bDup = false;
            for (j = 0 ; j < i ; j++) {
                if (*(aRecord[j].pKey) == iKey) {
                    bDup = true;
                    break;
                }
            }
            if (bDup == false)
                break;
        } while (true);
        aRecord[i].pKey = (int32_t*)malloc(sizeof(int32_t));
        if (!(aRecord[i].pKey))
            goto EXIT;
        aRecord[i].pValue = (struct VALUE_*)malloc(sizeof(struct VALUE_));
        if (!(aRecord[i].pValue))
            goto EXIT;
        *(aRecord[i].pKey) = iKey;
        aRecord[i].pValue->iMajor = 1;
        aRecord[i].pValue->iMinor = 0;
    }

    /* Initialize the tree. */
    BinSearchTree *pTree;
    int32_t iStat = BSTreeInit(&pTree);
    if (iStat != SUCCESS)
        goto EXIT;

    /* Set the comparison and deallocation functions. */
    pTree->set_compare(pTree, func_compare_non_primitive);
    pTree->set_destroy(pTree, func_destroy_non_primitive);

    /* Insert integers into the tree. */
    for (i = 0 ; i < COUNT_CASE ; i++) {
        iStat = pTree->insert(pTree, (Item)(&aRecord[i]));
        if (iStat == FAIL_DATA_CONFLICT);
            /* Handle data conflict if necessary. */
        else if (iStat == FAIL_NO_MEMORY)
            goto DEINIT;
    }

    /* Search integers stored in the tree. */
    for (i = 0 ; i < COUNT_CASE ; i++) {
        RECORD *pRec = &aRecord[i];
        iStat = pTree->search(pTree, (Item*)(&pRec));
        assert(iStat == SUCCESS);
    }

    /* Delete parts of the integers stored in the tree. */
    uint32_t uiSize = pTree->size(pTree);
    for (i = 0 ; i < uiSize / 2 ; i++)
        pTree->delete(pTree, (Item)(&aRecord[i]));

    /* Search integers stored in the tree again. */
    for (i = 0 ; i < COUNT_CASE ; i++) {
        if (aRecord[i].pKey != NULL) {
            RECORD *pRec = &aRecord[i];
            iStat = pTree->search(pTree, (Item*)(&pRec));
            assert(iStat == SUCCESS);
        }
    }


DEINIT:
    /* Deinitialize the tree. */
    BSTreeDeinit(&pTree);
    
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

int32_t func_compare_non_primitive(Item itemSrc, Item itemTge)
{
    RECORD *pRecSrc = (RECORD*)itemSrc;
    RECORD *pRecTge = (RECORD*)itemTge;

    return pRecSrc->pKey - pRecTge->pKey;
}

void func_destroy_non_primitive(Item item)
{
    RECORD *pRec = (RECORD*)item;
    free(pRec->pKey);
    free(pRec->pValue);
    pRec->pKey = NULL;
    pRec->pValue = NULL;

    return;
}
