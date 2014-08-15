#include "binomial_heap.h"

/*===========================================================================*
 *                  Simulation for private variables                         *
 *===========================================================================*/
static unsigned long _ulSize;
HeapNode *_pRootList;
static int  (*_pCompare) (const void*, const void*);
static void (*_pDestroy) (void*);

/*===========================================================================*
 *                  Definition for internal functions                        *
 *===========================================================================*/
/**
 * The default function for item comparison.
 *
 * @param pSrc          The pointer to the source item.
 * @param pTge          The pointer to the target item.
 *
 * @return               1: The source item goes after the target one with certain ordering criteria.
 *                       0: The source item equals to the target one with certain ordering criteria.
 *                      -1: The source item goes before the target one with certain ordering criteria.
 */
int _BinomialHeapItemCompare(const void *pSrc, const void *pTge);


/**
 * The default function for item deallocation.
 * 
 * @param pItem         The pointer to the item which is to be deallocated.
 */
void _BinomialHeapItemDestroy(void *pItem);


/**
 * The default function for item content updating.
 *
 * @param pTge          The pointer to the item to be updated.
 * @param pRef          The pointer to the item containing reference data.
 */
void _BinomialHeapItemUpdate(const void *pTge, const void *pRef);


/** 
 * The operation to link the parent-child relation for two heap nodes.
 *
 * @param pChild        The pointer to the node which should be the child.
 * @param pParent       The pointer to the node which should be the parent.
 */
void _BinomialHeapLink(HeapNode *pChild, HeapNode *pParent);


/**
 * The operation to merge two binomial heaps into a single root list with root nodes 
 * sorted by increasing degree.
 *
 * @param pListSrc      The pointer to the root list of source heap.
 * @param pListTge      The pointer to the root list of target heap.
 *
 * @return              true : The operations are completed successfully.
 *                      false: Exception occurs due to insufficient memory.
 */
HeapNode* _BinomialHeapMerge(HeapNode *pListSrc, HeapNode *pListTge);


/**
 * The operation to unite two binomial heaps with proper maintenance operations.
 *
 * @param pListSrc      The pointer to the root list of source heap.
 * @param pListTge      The pointer to the root list of target heap.
 * 
 * @return              true : The operations are completed successfully.
 *                      false: Exception occurs due to insufficient memory.
 */
void _BinomialHeapUnion(HeapNode *pListSrc, HeapNode *pListTge);


/*===========================================================================*
 *                Implementation for exported functions                      *
 *===========================================================================*/
void BinomialHeapInit(BinomialHeap *self) {

    _ulSize = 0;
    _pRootList = NULL;
    _pCompare = _BinomialHeapItemCompare;
    _pDestroy = _BinomialHeapItemDestroy;    

    self->insert = BinomialHeapInsert;
    self->set_compare = BinomialHeapSetCompare;
    self->set_destroy = BinomialHeapSetDestroy;

    return;
}


void BinomialHeapDeinit(BinomialHeap *self) {

    return;
}


/**
 * BinomialHeapInsert(): Insert an item to the appropriate position of the heap. 
 */
bool BinomialHeapInsert(BinomialHeap *self, void *pItem) {
    HeapNode *pNewHead;

    pNewHead = (HeapNode*)malloc(sizeof(HeapNode));
    if (pNewHead == NULL)
        return false;    
    pNewHead->pItem = pItem;
    pNewHead->ulDegree = 0;
    pNewHead->pParent = NULL;
    pNewHead->pChild = NULL;
    pNewHead->pSibling = NULL;

    /* Apply union operation to move the newly created node to the proper position. */
    _BinomialHeapUnion(_pRootList, pNewHead);
    _ulSize++;

    return false;
}


/**
 * BinomailHeapDelete(): Delete the root node of the heap and returns the item storing at that node correspondingly.
 */
void* BinomialHeapDelete(BinomialHeap *self);


/**
 * BinomialHeapTop(): Return the item stored in the top of the heap.
 */
void* BinomialHeapTop(BinomialHeap *self);


/**
 * BinomialHeapChange(): Update the content of the designated item.
 * Note that the updating must not violate the heap property. 
 */
bool BinomialHeapChange(BinomialHeap *self, int idx, void *pTge);


/**
 * BinomialHeapSize(): Return the size of the heap.
 */
unsigned long BinomialHeapSize(BinomialHeap *self) {

    return _ulSize;
}


/**
 * BinomialHeapSetCompare(): Set the user-defined item comparison strategy for the heap.
 */
void BinomialHeapSetCompare(BinomialHeap *self, int (*pFunc)(const void*, const void*)) {

    _pCompare = pFunc;
    return;
}


/**
 * BinomialHeapSetDestroy(): Set the user-defined item deallocation strategy for the heap.
 */
void BinomialHeapSetDestroy(BinomialHeap *self, void (*pFunc)(void*)) {

    _pDestroy = pFunc;
    return;
}


/**
 * BinomialHeapSetUpdate(): Set the user-defined item content updating strategy for the heap.
 */
void BinomialHeapSetUpdate(BinomialHeap *self, void (*pFunc)(const void*, const void*));


/*===========================================================================*
 *                Implementation for internal functions                      *
 *===========================================================================*/
/**
 * _BinomialHeapItemCompare(): The default item comparison strategy.
 * Note: It considers the item as primitive data and gives the larger order to 
 *       the item with larger integer value.
 */
int _BinomialHeapItemCompare(const void *pSrc, const void *pTge) {
    size_t nSrc, nTge;    
    
    nSrc = (size_t)pSrc;
    nTge = (size_t)pTge;
    
    return nSrc - nTge;
}


/**
 * _BinomialHeapItemDestroy(): The default item deallocation strategy.
 * Note: By default, the item is a primitive data, and thus no further operations are required
 */
void _BinomialHeapItemDestroy(void *pItem) {

    return;
}


/**
 * _BinomialHeapItemUpdate(): The default function for item content updating.
 * Note: By default, the item is a primitive data, and thus we just update the data pointed by
 * pTge with the one pointed by pRef.
 */
void _BinomialHeapItemUpdate(const void *pTge, const void *pRef) {

    *(size_t*)pTge = *(size_t*)pRef;
    return;
}


/** 
 * _BinomialHeapLink(): The operation to link the parent-child relation for two heap nodes.
 */
void _BinomialHeapLink(HeapNode *pChild, HeapNode *pParent) {

    pChild->pParent = pParent;
    pChild->pSibling = pParent->pChild;
    pParent->pChild = pChild;
    pParent->ulDegree++;

    return;
}


/**
 * _BinomialHeapMerge(): The operation to merge two binomial heaps into a single root list with 
 * root nodes sorted by increasing degree.
 */
HeapNode* _BinomialHeapMerge(HeapNode *pListSrc, HeapNode *pListTge) {
    unsigned long ulDegSrc, ulDegTge;
    HeapNode *pHead, *pSiblingSrc, *pSiblingTge;

    /* Select the new head for the merged root list. */
    ulDegSrc = ulDegTge = -1;
    if (pListSrc != NULL) {
        ulDegSrc = pListSrc->ulDegree;
    }
    if (pListTge != NULL) {
        ulDegTge = pListTge->ulDegree;
    }

    if ((ulDegSrc != -1) && (ulDegTge == -1)) {
        pHead = pListSrc;
    } else if ((ulDegSrc == -1) && (ulDegTge != -1)) {
        pHead = pListTge;
    } else if ((ulDegSrc == -1) && (ulDegTge == -1)) {
        pHead = NULL;
    } else {
        if (ulDegSrc <= ulDegTge) {
            pHead = pListSrc;
        } else {
            pHead = pListTge;
        }
        
        /* Merge two lists by examining the degrees of root nodes. */
        while ((pListSrc != NULL) && (pListTge != NULL)) {
            if (pListSrc->ulDegree <= pListTge->ulDegree) {
                pSiblingSrc = pListSrc->pSibling;
                pListSrc->pSibling = pListTge;
                pListSrc = pSiblingSrc;                
            } else {
                pSiblingTge = pListTge->pSibling;
                pListTge->pSibling = pListSrc;
                pListTge = pSiblingTge;
            }
        }            
    }

    return pHead;
}


/**
 * _BinomialHeapUnion(): The operation to unite two binomial heaps with proper maintenance operations.
 */
void _BinomialHeapUnion(HeapNode *pListSrc, HeapNode *pListTge) {
    int rc;
    HeapNode *pPred, *pCurr, *pSucc;

    /* Merge two lists into a unique one. */
    _pRootList = _BinomialHeapMerge(pListSrc, pListTge);
    if (_pRootList == NULL) {
        return;
    }

    /* Adjust the heap structure by sliding through the merged root list. */
    pPred = NULL;
    pCurr = _pRootList;
    pSucc = pCurr->pSibling;

    while (pSucc != NULL) {
        /* Case 1: degree[curr] != degree[succ] */
        if (pCurr->ulDegree != pSucc->ulDegree) {
            pPred = pCurr;
            pCurr = pSucc;
        } 
        /* Case 2: degree[curr] == degree[succ] == degree[succ->sibling] */
        else if ((pSucc->pSibling != NULL) && (pSucc->pSibling->ulDegree == pCurr->ulDegree)) {
            pPred = pCurr;
            pCurr = pSucc;
        } else {
            /* Case 3 & 4: degree[curr] == degree[succ] != degree[succ->sibling]. */
            rc = _pCompare(pCurr->pItem, pSucc->pItem);

            /* Case 3: order(curr) > order(succ) */            
            if (rc > 0) {
                pCurr->pSibling = pSucc->pSibling;
                _BinomialLink(pSucc, pCurr);
            } 
            /* Case 4: order(curr) <= order(succ) */
            else {
                if (pPred == NULL) {
                    _pRootList = pSucc;
                } else {
                    pPred->pSibling = pSucc;
                }
                _BinomialLink(pCurr, pSucc);
            }
            pCurr = pSucc;
        }
        pSucc = pCurr->pSibling;
    }

    return;
}


/*===========================================================================*
 *                 The interface definition of plugin                        *
 *===========================================================================*/
#ifdef BUILD_PLUGIN
bool init();

void deinit();

bool insert(void*);

void* delete();

void* top();

unsigned long size();

void set_compare(int(*)(const void*, const void*));

void set_destroy(void(*)(void*));
#endif

