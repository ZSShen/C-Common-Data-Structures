#include "dynamic_array.h"
#include "binary_heap.h"


#define SIZE_INIT_CAPACITY      (1)
#define TIMES_EXPAND_CAPACITY   (2)
#define TIMES_SHRINK_CAPACITY   (0.5)
#define INDEX_ROOT              (1)

#define Parent(idx)             (idx / 2)
#define Left(idx)               (idx * 2)
#define Right(idx)              (idx * 2 + 1)


/*===========================================================================*
 *                  Simulation for private variables                         *
 *===========================================================================*/
static DynamicArray    *_pArray;
static unsigned long   _ulSize;
static int  (*_pCompare) (const void*, const void*);
static void (*_pDestroy) (void*);
static void (*_pUpdate)  (const void*, const void*);

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
int _BinaryHeapItemCompare(const void *pSrc, const void *pTge);


/**
 * The default function for item deallocation.
 * 
 * @param pItem         The pointer to the item which is to be deallocated.
 */
void _BinaryHeapItemDestroy(void *pItem);


/**
 * The default function for item content updating.
 *
 * @param pTge          The pointer to the item to be updated.
 * @param pRef          The pointer to the item containing reference data.
 */
void _BinaryHeapItemUpdate(const void *pTge, const void *pRef);


/*===========================================================================*
 *                Implementation for exported functions                      *
 *===========================================================================*/
bool BinaryHeapInit(BinaryHeap *self) {

    /* Initialize the DynamicArray strcture. */
    DynamicArray_init(_pArray, SIZE_INIT_CAPACITY);    
    if (_pArray == NULL)
        return false;
    _ulSize = 0;

    /* Let the function pointers point to the corresponding functions. */
    self->insert = BinaryHeapInsert;
    self->delete = BinaryHeapDelete;
    self->top = BinaryHeapTop;
    self->change = BinaryHeapChange;
    self->size = BinaryHeapSize;
    
    self->set_compare = BinaryHeapSetCompare;
    self->set_destroy = BinaryHeapSetDestroy;

    /* Set the default item comparison and deallocation strategies. */
    _pCompare = _BinaryHeapItemCompare;
    _pDestroy = _BinaryHeapItemDestroy;

    return true;
}


void BinaryHeapDeinit(BinaryHeap *self) {
    int  i;
    void *pItem;

    for (i = 0 ; i < _ulSize ; i++) {
        pItem = _pArray->get(_pArray, i);
        _pDestroy(pItem);
    }
    
    DynamicArray_deinit(_pArray);
    return;
}


/**
 * BinaryHeapInsert(): Insert an item to the appropriate position of the heap. 
 */
bool BinaryHeapInsert(BinaryHeap *self, void *pItem) {
    int             rc;    
    bool            status;    
    unsigned long   ulCapacity, idxCurr, idxParent;
    void            *pItemCurr, *pItemParent;

    /* Expand the capacity of the array if necessary. */
    ulCapacity = _pArray->capacity(_pArray);
    if ((_ulSize + 1) == ulCapacity) {
        status = _pArray->resize(_pArray, TIMES_EXPAND_CAPACITY);
        if (status == false)
            return false;    
    }

    /* Insert the item to the tail of the array. */
    _pArray->put(_pArray, pItem, _ulSize);
    _ulSize++;

    /* Maintain the attributes of binary heap. */
    idxCurr = _ulSize;
    while (idxCurr > 1) {
        idxParent = Parent(idxCurr);
        pItemCurr = _pArray->get(_pArray, idxCurr - 1);
        pItemParent = _pArray->get(_pArray, idxParent - 1);

        rc = _pCompare(pItemCurr, pItemParent);
        if (rc <= 0)
            break;

        _pArray->put(_pArray, pItemCurr, idxParent - 1);
        _pArray->put(_pArray, pItemParent, idxCurr - 1);
        idxCurr = idxParent;
    }

    return true;
}


/**
 * BinaryHeapDelete(): Delete the root node of the heap and return the item storing at that node correspondingly.
 */
void* BinaryHeapDelete(BinaryHeap *self) {
    unsigned long   ulCapacity, idxCurr, idxLeft, idxRight, idxLarge;
    void            *pItemRet, *pItemTail, *pItemCurr, *pItemLeft, *pItemRight, *pItemLarge;

    /* Reject the invalid request. */
    if (_ulSize == 0)
        return NULL;

    /* Shrink the capacity of the array if necessary. */
    ulCapacity = _pArray->capacity(_pArray);
    if ((_ulSize + 1) < (ulCapacity * TIMES_SHRINK_CAPACITY))
        _pArray->resize(_pArray, TIMES_SHRINK_CAPACITY);

    /* Retrive the item from the head of the array. */
    pItemRet = _pArray->get(_pArray, INDEX_ROOT - 1);
    pItemTail = _pArray->get(_pArray, _ulSize - 1);    
    _pArray->put(_pArray, pItemTail, INDEX_ROOT - 1);
    _ulSize--;

    /* Maintain the attributes of binary heap. */
    idxCurr = INDEX_ROOT;
    do {        
        idxLeft = Left(idxCurr);
        idxRight = Right(idxCurr);
        pItemCurr = _pArray->get(_pArray, idxCurr - 1);
        pItemLeft = _pArray->get(_pArray, idxLeft - 1);
        pItemRight = _pArray->get(_pArray, idxRight - 1);
        if ((idxLeft <= _ulSize) && (_pCompare(pItemLeft, pItemCurr) == 1))
            idxLarge = idxLeft;
        else
            idxLarge = idxCurr;

        pItemLarge = _pArray->get(_pArray, idxLarge - 1);
        if ((idxRight <= _ulSize) && (_pCompare(pItemRight, pItemLarge) == 1))
            idxLarge = idxRight;
                    
        if (idxCurr == idxLarge)
            break;
        
        pItemLarge = _pArray->get(_pArray, idxLarge - 1);
        _pArray->put(_pArray, pItemCurr, idxLarge - 1);    
        _pArray->put(_pArray, pItemLarge, idxCurr - 1);
        idxCurr = idxLarge;
    } while (true);

    return pItemRet;
}


/**
 * BinaryHeapTop(): Return the item stored in the top of the heap.
 */
void* BinaryHeapTop(BinaryHeap *self) {

    if (_ulSize == 0)
        return NULL;
    
    return _pArray->get(_pArray, INDEX_ROOT - 1);    
}


/**
 * BinaryHeapChange(): Update the content of the designated item.
 * Note: The updating must not violate the heap property.
 */
bool BinaryHeapChange(BinaryHeap *self, int idx, void *pTge) {
    
    return true;
}


/**
 * BinaryHeapSize(): Return the size of the heap.
 */
unsigned long BinaryHeapSize(BinaryHeap *self) {

    return _ulSize;
}


/**
 * BinaryHeapSetCompare(): Set the user-defined item comparison strategy for the heap.
 */
void BinaryHeapSetCompare(BinaryHeap *self, int (*pFunc)(const void*, const void*)) {

    _pCompare = pFunc;
    return;
}


/**
 * BinaryHeapSetDestroy(): Set the user-defined item deallocation strategy for the heap.
 */
void BinaryHeapSetDestroy(BinaryHeap *self, void (*pFunc)(void*)) {

    _pDestroy = pFunc;
    return;
}


/**
 * BinaryHeapSetUpdate(): Set the user-defined item content updating strategy for the heap.
 */
void BinaryHeapSetUpdate(BinaryHeap *self, void (*pFunc)(const void*, const void*)) {

    _pUpdate = pFunc;
    return;
}

/*===========================================================================*
 *                Implementation for internal functions                      *
 *===========================================================================*/
/**
 * _BinaryHeapItemCompare(): The default item comparison strategy.
 * Note: It considers the item as primitive data and gives the larger order to 
 *       the item with larger integer value.
 */
int _BinaryHeapItemCompare(const void *pSrc, const void *pTge) {
    
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
 * _BinaryHeapItemDestroy(): The default item deallocation strategy.
 * Note: By default, the item is a primitive data, and thus no further operations are required
 */
void _BinaryHeapItemDestroy(void *pItem) {

    return;
}


/**
 * The default function for item content updating.
 * Note: By default, the item is a primitive data, and thus we just update the data pointed by
 * pTge with the one pointed by pRef.
 */
void _BinaryHeapItemUpdate(const void *pTge, const void *pRef) {

    *(size_t*)pTge = *(size_t*)pRef;
    return;
}

