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
