#include "priority_queue.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <getopt.h>


#define SIZE_DATA           10000
#define BUF_SIZE_SMALL      64
#define OPT_IMPLEMENT       'i'
#define OPT_HELP            'h'
#define OPT_LONG_IMPLEMENT  "implement"
#define OPT_LONG_HELP       "help"


typedef struct _Key {
    short key_major;
    short key_minor;
} Key;


typedef struct _Data {
    Key  *pKey;
    char *pValue;
} Data;


/* Print the help message to use this program. */
void print_usage();


/* Define the item comparison strategy for maximum PriorityQueue. */
int ItemCompareMax(const void *pSrc, const void *pTge);


/* Define the item comparison strategy for minimum PriorityQueue. */
int ItemCompareMin(const void *pSrc, const void *pTge);


/* Define the item deallocation strategy for PriorityQueue. */
void ItemDestroy(void *pItem);


/* Test function for insertion-related features. */
void test_push(PriorityQueue *pQueue);


/* Test function for deletion-related features. */
void test_pop(PriorityQueue *pQueue);


int main(int argc, char **argv) {
    int idxOpt, rc;
    char opt;
    char *cszStructure;
    PriorityQueue *pQueue;
    char szOrder[BUF_SIZE_SMALL];    

    /* Determine the implementation for priority queue. */
    static struct option Options[] = {
        {OPT_LONG_HELP,      no_argument, 0, OPT_HELP},
        {OPT_LONG_IMPLEMENT, required_argument, 0, OPT_IMPLEMENT},
    };
    memset(szOrder, 0, sizeof(char) * BUF_SIZE_SMALL);
    sprintf(szOrder, "%c%c:", OPT_HELP, OPT_IMPLEMENT);

    rc = 0;
    idxOpt = 0;
    cszStructure = NULL;
    while ((opt = getopt_long(argc, argv, szOrder, Options, &idxOpt)) != -1) {
        switch (opt) {
            case OPT_IMPLEMENT: {
                cszStructure = optarg;
                break;
            }
            case OPT_HELP: {            
                print_usage(); 
                rc = -1;            
                goto EXIT;
            }
            default: {
                print_usage();
                rc = -1;
                goto EXIT;
            }
        }
    }

    if (cszStructure == NULL) {
        print_usage();
        rc = -1;
        goto EXIT;
    }


    /* Initialize the random seed. */
    srand(time(NULL));

    /*-----------------------------------------------*/
    /*      Test for maximum priority queue.         */
    /*-----------------------------------------------*/
    /* Create the maximum PriorityQueue structure. */
    PriorityQueue_init(pQueue, cszStructure);
    if (pQueue == NULL) {
        rc = -1;
        goto EXIT;
    }

    /* Customize the item comparison and deallocation strategies. */
    pQueue->set_compare(pQueue, ItemCompareMax);
    pQueue->set_destroy(pQueue, ItemDestroy);
    
    /* Test the main features of PriorityQueue. */    
#ifdef ELAPSE
    clock_t tBegin, tEnd, tTotal;
    tBegin = clock();
#endif

    test_push(pQueue);    
    test_pop(pQueue);

#ifdef ELAPSE
    tEnd = clock();
    tTotal = tEnd - tBegin;
#endif
    
    /* Free the PriorityQueue structure. */
    PriorityQueue_deinit(pQueue);

    /*-----------------------------------------------*/
    /*      Test for minimum priority queue.         */
    /*-----------------------------------------------*/
    PriorityQueue_init(pQueue, cszStructure);
    if (pQueue == NULL) {
        rc = -1;
        goto EXIT;
    }

    pQueue->set_compare(pQueue, ItemCompareMin);
    pQueue->set_destroy(pQueue, ItemDestroy);

#ifdef ELAPSE
    tBegin = clock();
#endif

    test_push(pQueue);
    test_pop(pQueue);

#ifdef ELAPSE
    tEnd = clock();
    tTotal += tEnd - tBegin;
    printf("Real Processing Time: %d microseconds.\n", tTotal);
#endif

    PriorityQueue_deinit(pQueue);

EXIT:
    return rc;
}


void print_usage() {
    
    const char *cszMsg = "Usage: test_program --implement data_structure.\n"
                         "       test_program -i          data_structure.\n\n"
                         "       data_structure: The implementation for priority queue.\n"
                         "                       1. binary heap\n"
                         "                       2. binomial heap\n"
                         "-------------------------------------------------------------\n"
                         "Example: test_program --implement binary_heap\n"
                         "         test_program -i          binary_heap\n\n";
    printf("%s", cszMsg);
    return;
}


int ItemCompareMax(const void *pSrc, const void *pTge) {
    Key *pKeySrc, *pKeyTge;

    pKeySrc = (Key*)(((Data*)pSrc)->pKey);
    pKeyTge = (Key*)(((Data*)pTge)->pKey);

    return pKeySrc->key_major

    if (pKeySrc->key_major == pKeyTge->key_major)
        return 0;
    else {
        if (pKeySrc->key_major > pKeyTge->key_major)
            return 1;
        else
            return -1;
    }
}


int ItemCompareMin(const void *pSrc, const void *pTge) {
    Key *pKeySrc, *pKeyTge;

    pKeySrc = (Key*)(((Data*)pSrc)->pKey);
    pKeyTge = (Key*)(((Data*)pTge)->pKey);
    if (pKeySrc->key_major == pKeyTge->key_major)
        return 0;
    else {
        if (pKeySrc->key_major < pKeyTge->key_major)
            return 1;
        else
            return -1;
    }
} 


void ItemDestroy(void *pItem) {
    
    free(((Data*)pItem)->pKey);
    free(((Data*)pItem)->pValue);
    free(pItem);
    return;
}


void test_push(PriorityQueue *pQueue) {
    int  i;    
    Key  *pKey;
    Data *pData;
    
    for (i = 0 ; i < SIZE_DATA ; i++) {
        pKey = (Key*)malloc(sizeof(Key));        
        pKey->key_major = rand() % SIZE_DATA;        
        pKey->key_minor = 0;

        pData = (Data*)malloc(sizeof(Data));
        pData->pKey = pKey;
        pData->pValue = (char*)malloc(sizeof(char));    
    
        pQueue->push(pQueue, (void*)pData);            
    }

    return;
}


void test_pop(PriorityQueue *pQueue) {
    int  i;
    Key  *pKey;
    Data *pData;

    for (i = 0 ; i < SIZE_DATA / 2; i++) {
        pData = (Data*)pQueue->pop(pQueue);
        //printf("%d\n", pData->pKey->key_major);
        ItemDestroy(pData);
    }

    return;
}
