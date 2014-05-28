#include "ordered_map.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SIZE_DATA 10000

typedef struct _Data {
    
} Data;


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

    /* Test the main features of ordered map. */    
    test_put(pMap);
    test_get(pMap);
    test_remove(pMap);

    /* Free the ordered map. */
    OrderedMap_deinit(pMap);

    return 0;
}


void test_put(OrderedMap *pMap) {
    int          i;    
    KeyValuePair *pPair;
    
    for (i = 0 ; i < SIZE_DATA ; i++) {
        pPair = (KeyValuePair*)malloc(sizeof(KeyValuePair));
        pPair->pKey = (void*)i;
        pPair->pValue = NULL;
    
        pMap->put(pMap, pPair);
    }

    return;
}


void test_get(OrderedMap *pMap) {
    int          i;
    KeyValuePair *pPair;

    for (i = 0 ; i < SIZE_DATA ; i++) {
        pPair = pMap->get(pMap, (void*)i);
        //printf("%d\n", pPair->pKey);
    }

    return;
}


void test_remove(OrderedMap *pMap) {
    int i;

    for (i = 0 ; i < SIZE_DATA ; i++) {
        pMap->remove(pMap, (void*)i);
    }

    return;
}
