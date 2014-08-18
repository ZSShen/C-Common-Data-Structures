#include "binomial_heap.h"
#include <stdio.h>
#include <string.h>
#include <time.h>


#define SIZE_DATA 10000


typedef struct _Key {
    short key_major;
    short key_minor;
} Key;


typedef struct _Data {
    Key  *pKey;
    char *pValue;
} Data;


/* Define the item comparison strategy for maximum BinomialHeap. */
int ItemCompareMax(const void *pSrc, const void *pTge);


/* Define the item comparison strategy for minimum BinomialHeap. */
int ItemCompareMin(const void *pSrc, const void *pTge);


/* Define the item deallocation strategy for BinomialHeap. */
void ItemDestroy(void *pItem);


/* Test function for insertion-related features. */
void test_insert(BinomialHeap *pHeap);


/* Test function for deletion-related features. */
void test_delete(BinomialHeap *pHeap);


int main() {
    BinomialHeap *pHeap;

    /* Initialize the random seed. */
    srand(time(NULL));

    /*-----------------------------------------------*
     *      Test for maximum binomial heap.          *
     *-----------------------------------------------*/

    /* Create the maximum BinomialHeap structure. */
    BinomialHeap_init(pHeap);

    /* Customize the item comparison and deallocation strategies. */
    pHeap->set_compare(pHeap, ItemCompareMax);
    pHeap->set_destroy(pHeap, ItemDestroy);

    /* Test the main features of BinomialHeap. */    
    test_insert(pHeap);    
    test_delete(pHeap);

    /* Free the BinomialHeap structure. */
    BinomialHeap_deinit(pHeap);

    /*-----------------------------------------------*
     *      Test for minimum binomial heap.          *
     *-----------------------------------------------*/
    BinomialHeap_init(pHeap);

    pHeap->set_compare(pHeap, ItemCompareMin);
    pHeap->set_destroy(pHeap, ItemDestroy);

    test_insert(pHeap);
    test_delete(pHeap);

    BinomialHeap_deinit(pHeap);

    return 0;
}


int ItemCompareMax(const void *pSrc, const void *pTge) {
    Key *pKeySrc, *pKeyTge;

    pKeySrc = (Key*)(((Data*)pSrc)->pKey);
    pKeyTge = (Key*)(((Data*)pTge)->pKey);
    
    return pKeySrc->key_major - pKeyTge->key_major;    
}


int ItemCompareMin(const void *pSrc, const void *pTge) {
    Key *pKeySrc, *pKeyTge;

    pKeySrc = (Key*)(((Data*)pSrc)->pKey);
    pKeyTge = (Key*)(((Data*)pTge)->pKey);
    
    return -(pKeySrc->key_major - pKeyTge->key_major);
} 


void ItemDestroy(void *pItem) {
    
    free(((Data*)pItem)->pKey);
    free(((Data*)pItem)->pValue);
    free(pItem);
    return;
}


void test_insert(BinomialHeap *pHeap) {
    int  i;    
    Key  *pKey;
    Data *pData;
    int nKey;
    
    for (i = 0 ; i < SIZE_DATA ; i++) {
        pKey = (Key*)malloc(sizeof(Key));        
        pKey->key_major = rand() % SIZE_DATA;        
        pKey->key_minor = 0;

        pData = (Data*)malloc(sizeof(Data));
        pData->pKey = pKey;
        pData->pValue = (char*)malloc(sizeof(char));    
    
        pHeap->insert(pHeap, (void*)pData);            
    }

    return;
}


void test_delete(BinomialHeap *pHeap) {
    int  i;
    Key  *pKey;
    Data *pData;
    int nKey;

    for (i = 0 ; i < SIZE_DATA / 2; i++) {
        pData = (Data*)pHeap->delete(pHeap);
        /* printf("%d\n", pData->pKey->key_major); */
        ItemDestroy(pData);
    }

    return;
}
