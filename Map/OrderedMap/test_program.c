#include "ordered_map.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SIZE_DATA 10000

typedef struct _Key {
    void *key_major;
    void *key_minor;
} Key;


typedef struct _Value {
    void *value_major;
    void *value_minor;
} Value;


/* Define the key-value pair comparison strategy for OrderedMap. */
int PairCompare(const void*, const void*);


/* Define the key-value pair destroy strategy for OrderedMap. */
void PairDestroy(void*);


/* Test function for insertion-related features. */
void test_put(OrderedMap *pMap);


/* Test function for query-related features. */
void test_get(OrderedMap *pMap);


/* Test function for deletion-related features. */
void test_remove(OrderedMap *pMap);


int main() {
    OrderedMap *pMap;

    /* Initialize the random seed. */
    srand(time(NULL));

    /* Create the ordered map. */
    OrderedMap_init(pMap);

    /* Assign the customized comparison and destroy strategies of key-value pair. */
    pMap->set_compare(PairCompare);
    pMap->set_destroy(PairCompare);
    
    /* Test the main features of ordered map. */    
    //test_put(pMap);
    //test_get(pMap);
    //test_remove(pMap);

    /* Free the ordered map. */
    OrderedMap_deinit(pMap);

    return 0;
}


int PairCompare(const void *pSrc, const void *pTge) {
    Key *pKeySrc, *pKeyTge;
    
    pKeySrc = (Key*)(((KeyValuePair*)pSrc)->pKey);
    pKeyTge = (Key*)(((KeyValuePair*)pTge)->pKey);

    if (pKeySrc->key_major == pKeyTge->key_major)
        return 0;
    else {
        if (pKeySrc->key_major > pKeyTge->key_major)
            return 1;
        else
            return -1;    
    }
}


void PairDestroy(void *pPair) {
    Key     *pKey;
    Value   *pValue;
    
    pKey = (Key*)(((KeyValuePair*)pPair)->pKey);
    pValue = (Value*)(((KeyValuePair*)pPair)->pValue);
    
    free(pKey);
    free(pValue);
    free(pPair);
    return;
}


void test_put(OrderedMap *pMap) {
    int          i;
    Key          *pKey;
    Value        *pValue;    
    KeyValuePair *pPair;
    
    for (i = 0 ; i < SIZE_DATA ; i++) {
        pPair = (KeyValuePair*)malloc(sizeof(KeyValuePair));
        if (pPair != NULL) {
            pKey = (Key*)malloc(sizeof(Key));
            pValue = (Value*)malloc(sizeof(Value));
            if ((pKey != NULL) && (pValue != NULL)) {
                pKey->key_major = (void*)i;            
                pKey->key_minor = NULL;
                pValue->value_major = NULL;
                pValue->value_minor = NULL;
                pPair->pKey = pKey;
                pPair->pValue = pValue;
                pMap->put(pMap, pPair);
            }
        }
    }

    return;
}


void test_get(OrderedMap *pMap) {
    int          i;
    KeyValuePair *pPair;

    for (i = 0 ; i < SIZE_DATA ; i++) {
        pPair = pMap->get(pMap, (void*)i);
    }

    return;
}


void test_remove(OrderedMap *pMap) {
    int  i;
    Key *pKey;

    for (i = 0 ; i < SIZE_DATA ; i++) {
        pKey = (Key*)malloc(sizeof(Key));
        if (pKey != NULL) {        
            pKey->key_major = (void*)i;
            pKey->key_minor = NULL;            
            pMap->remove(pMap, pKey);
        }
    }

    return;
}
