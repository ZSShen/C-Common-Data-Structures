#include "red_black_tree.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SIZE_DATA 10000

typedef struct _Data {
    unsigned long key;
    void *value;
} Data;


/* Define the node item comparison function for red black tree. */
int data_compare(const void *pSrc, const void *pTge);

/* Define the node item delete function for red black tree. */
void data_destroy(void *pItem);

/* Test function for insertion-related utilites. */
void test_insert(RedBlackTree *pTree);

/* Test function for deletion-related utilities. */
void test_delete(RedBlackTree *pTree);

int main() {
    RedBlackTree *pTree;

    /* Initialize the random seed. */
    srand(time(NULL));

    /* Create the red black tree. */
    RedBlackTree_init(pTree);

    /* Customize the node item comparison and destroy functions .*/
    pTree->set_compare(pTree, data_compare);
    pTree->set_destroy(pTree, data_destroy);    

    /* Test red black tree utilities. */
    test_insert(pTree);
    test_delete(pTree);

    /* Free the red black tree. */
    RedBlackTree_deinit(pTree);

    return 0;
}


int data_compare(const void *pSrc, const void *pTge) {

    if (((Data*)pSrc)->key == ((Data*)pTge)->key)
        return 0;
    else {
        if (((Data*)pSrc)->key > ((Data*)pTge)->key)
            return 1;
        else
            return -1;
    }
}


void data_destroy(void *pItem) {

    free(pItem);
    return;
}


void test_insert(RedBlackTree *pTree) {
    int             i;
    unsigned long   key;
    Data            *pData;

    for (i = 0 ; i < SIZE_DATA ; i++) {
        key = rand();
        
        pData = (Data*)malloc(sizeof(Data));
        if (pData != NULL) {
            pData->key = key;
            pData->value = NULL;
            
            pTree->insert(pTree, pData);
        }
    }

    return;
}


void test_delete(RedBlackTree *pTree) {
    int          turn;    
    RedBlackNode *pNode;

    turn = 1;
    while (true) {
        if (turn % 2 == 0)
            pNode = pTree->maximum(pTree);
        else
            pNode = pTree->minimum(pTree);

        if (pNode == NULL)
            break;
        
        pTree->delete(pTree, pNode);
        //printf("%lu\n", pTree->size(pTree));
        turn++;    
    }

    return;
}
