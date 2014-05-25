#include "singly_linked_list.h"
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
void test_insert(SinglyLinkedList *pList);


/* Test function for deletion-related utilities. */
void test_delete(SinglyLinkedList *pList);


/* Test function for list reversing utility. */
void test_reverse(SinglyLinkedList *pList);


int main() {
    SinglyLinkedList *pList;

    /* Initialize the random seed. */
    srand(time(NULL));

    /* Create the singly linked list. */
    SinglyLinkedList_init(pList);

    /* Customize the node item comparison and destroy functions .*/
    pList->compare = data_compare;
    pList->destroy = data_destroy;

    /* Test singly linked list utilities. */
    test_insert(pList);    
    test_reverse(pList);
    test_delete(pList);

    /* Free the singly linked list. */
    SinglyLinkedList_deinit(pList);

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


void test_insert(SinglyLinkedList *pList) {
    int             i;
    unsigned long   key;
    Data            *pData;    

    /* Test appending operation. */
    for (i = 0 ; i < SIZE_DATA ; i++) {
        key = rand();
        
        pData = (Data*)malloc(sizeof(Data));
        if (pData != NULL) {        
            pData->key = key;
            pData->value = NULL;
            pList->append(pList, pData);                    
        }
    }

    /* Test insertion operation. */
    for (i = 0 ; i < SIZE_DATA / 2 ; i++) {
        key = rand();

        pData = (Data*)malloc(sizeof(Data));
        if (pData != NULL) {
            pData->key = key;
            pData->value = NULL;
            pList->insert(pList, i, pData);
        }
    }

    return;
}		


void test_reverse(SinglyLinkedList *pList) {
    
    pList->reverse(pList);
    return;
}


void test_delete(SinglyLinkedList *pList) {
    int i;

    /* Test pop operation. */
    for (i = 0 ; i < SIZE_DATA / 2; i++) {
        pList->pop(pList, i);
    }
    
    return;
}
