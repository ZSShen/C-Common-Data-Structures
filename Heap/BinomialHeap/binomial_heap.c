#include "binomial_heap.h"

/*===========================================================================*
 *                  Simulation for private variables                         *
 *===========================================================================*/
static unsigned long _ulSize;
HeapNode *_pRootList;


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
bool _BinomialHeapMerge(HeapNode *pListSrc, HeapNode *pListTge);


/**
 * The operation to unite two binomial heaps with proper maintenance operations.
 *
 * @param pListSrc      The pointer to the root list of source heap.
 * @param pListTge      The pointer to the root list of target heap.
 * 
 * @return              true : The operations are completed successfully.
 *                      false: Exception occurs due to insufficient memory.
 */
bool _BinomialHeapUnion(HeapNode *pListSrc, HeapNode *pListTge);


/*===========================================================================*
 *                Implementation for exported functions                      *
 *===========================================================================*/
void BinomialHeapInit(BinomialHeap *self) {

    _pRootList = NULL;
    
    return;
}


void BinomialHeapDeinit(BinomialHeap *self) {

    return;
}


/**
 * BinomialHeapInsert(): Insert an item to the appropriate position of the heap. 
 */
bool BinomialHeapInsert(BinomialHeap *self, void *pItem);


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
unsigned long BinomialHeapSize(BinomialHeap *self);


/**
 * BinomialHeapSetCompare(): Set the user-defined item comparison strategy for the heap.
 */
void BinomialHeapSetCompare(BinomialHeap *self, int (*pFunc)(const void*, const void*));


/**
 * BinomialHeapSetDestroy(): Set the user-defined item deallocation strategy for the heap.
 */
void BinomialHeapSetDestroy(BinomialHeap *self, void (*pFunc)(void*));


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
bool _BinomialHeapMerge(HeapNode *pListSrc, HeapNode *pListTge);


/**
 * _BinomialHeapUnion(): The operation to unite two binomial heaps with proper maintenance operations.
 */
bool _BinomialHeapUnion(HeapNode *pListSrc, HeapNode *pListTge);


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
