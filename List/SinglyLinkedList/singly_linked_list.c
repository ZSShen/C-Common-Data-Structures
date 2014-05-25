#include "singly_linked_list.h"


void SLListInit(SinglyLinkedList *self) {

    self->pHead = NULL;
    self->pTail = NULL;
    self->iSize = 0;

    /* Let the function pointers pointing to the appropriate functions. */
    self->compare = SLListCompare;
    self->destroy = SLListDestroy;

    self->append = SLListAppend;
    self->insert = SLListInsert;
    self->concate = SLListConcate;
    self->remove = SLListRemove;
    self->reverse = SLListReverse;
    self->pop = SLListPop;

    return;
}


void SLListDeinit(SinglyLinkedList *self) {
    SinglyLinkedNode *next;

    /* Release the memory allocated by SinglyLinkedList structure.*/
    while (self->pHead != NULL) {
        next = self->pHead->pNext;
        self->destroy(self->pHead->pItem);
        free(self->pHead);        
        self->pHead = next;
    } 

    return;
}


/**
 * SLListCompare(): The default strategy for node comparison.
 * Note: It considers source and target items as primitive data and compares their values directly.
 */
int SLListCompare(const void *pSrc, const void *pTge) {
    
    if ((size_t)pSrc == (size_t)pTge)
        return 0;
    else {
        if ((size_t)pSrc > (size_t)pTge)
            return 1;
        else
            return -1;    
    }
}


/**
 * SLListDestroy(): The default strategy for item deallocation.
 * Note: By default, the item is a primitive data, and thus no further operations are required.
 */
void SLListDestroy(void *pItem) {

    return;
}


/**
 * SLListAppend(): Append an item to the tail of the list.
 */
SinglyLinkedNode* SLListAppend(SinglyLinkedList *self, void *pItem) {
    SinglyLinkedNode *new;    

    new = malloc(sizeof(SinglyLinkedNode));    
    if (new != NULL) {
        new->pItem = pItem;
        new->pNext = NULL;    

        if (self->pHead == NULL) {
            self->pHead = new;
            self->pTail = new;    
        } else {
            self->pTail->pNext = new;
            self->pTail = new;            
        }
        self->iSize++;
    }

    return new;
}


/**
 * SLListInsert(): Insert an item to the designated position of the list.
 */
SinglyLinkedNode* SLListInsert(SinglyLinkedList *self, int idx, void *pItem) {
    int i;    
    SinglyLinkedNode *pred, *new, *succ;
    
    new = NULL;
    if (idx <= self->iSize) {
        pred = NULL;
        succ = self->pHead;
        for (i = 0 ; i < idx ; i++) {
            pred = succ;
            succ = succ->pNext;
        }

        new = malloc(sizeof(SinglyLinkedNode));
        if (new != NULL) {        
            new->pItem = pItem;
            new->pNext = succ;

            if (pred != NULL)
                pred->pNext = new;
            else
                self->pHead = new;
            self->iSize++;   
        }
    }

    return new;
}


/**
 * SLListConcate(): Concate the designated list to the tail of "this" list.
 */
void SLListConcate(SinglyLinkedList *self, SinglyLinkedList *pSrc) {
    SinglyLinkedNode *curr;

    self->pTail->pNext = pSrc->pHead;        
    curr = pSrc->pHead;
    while (curr != NULL) {
        self->iSize++;
        curr = curr->pNext;        
    }

    return;
}


/*
 * SLListRemove(): Remove the first item with the key equal to the designated item from the list,
 * and return it.
 */
void* SLListRemove(SinglyLinkedList *self, void *pItem) {
    int     rc;
    void    *pReturn;
    SinglyLinkedNode *pred, *curr, *succ;

    pReturn = NULL;
    pred = NULL;    
    curr = self->pHead;
    while (curr != NULL) {
        rc = self->compare(pItem, curr->pItem);        
        
        if (rc == 0) {
            pReturn = curr->pItem;
            succ = curr->pNext;
            free(curr);        

            if (pred != NULL)
                pred->pNext = succ;
            else
                self->pHead = succ;

            self->iSize--;
            break;
        }
        pred = curr;
        curr = curr->pNext;
    }

    return pReturn;
}


/**
 * SLListPop(): Remove the item from the designated position of the list and returns it.
 */
void* SLListPop(SinglyLinkedList *self, int idx) {
    int     i;    
    void    *pReturn; 
    SinglyLinkedNode *pred, *curr, *succ;

    pReturn = NULL;
    if (idx < self->iSize) {
        pred = NULL;
        curr = self->pHead;
        for (i = 0 ; i < idx ; i++) {
            pred = curr;
            curr = curr->pNext;        
        }

        pReturn = curr->pItem;
        succ = curr->pNext;
        free(curr);        

        if (pred != NULL)
            pred->pNext = succ;
        else
            self->pHead = succ;
        self->iSize--;
    }

    return pReturn;
}


/**
 * SLListReverse(): Reverse the list in place.
 */
void SLListReverse(SinglyLinkedList *self) {
    SinglyLinkedNode *pred, *curr, *succ;    

    pred = NULL;
    while (self->pHead != NULL) {
        succ = self->pHead->pNext;
        self->pHead->pNext = pred;
        pred = self->pHead;
        self->pHead = succ;
    }
    self->pHead = pred;

    return;
}
