#include "dynamic_array.h"
#include "priority_queue.h"

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
static unsigned long    _ulSize;
static DynamicArray    *_pArray;
static int  (*_pCompare) (const void*, const void*);
static void (*_pDestroy) (void*);


/*===========================================================================*
 *                  Definition for internal functions                        *
 *===========================================================================*/

/**
 * This function is the default comparison strategy for a pair of items.
 * 
 * @param pSrc      The pointer to the source item.
 * @param pTge      The pointer to the target item.
 *
 * @return          1 : The source item goes after the target one with certain ordering criteria.
 *                  0 : The source item equals to the target one with certain ordering criteria.
 *                  -1: The source item goes before the target one with certain ordering criteria.
 */
int _PQueueItemCompare(const void *pSrc, const void *pTge);


/**
 * This function is the default deallocation strategy for an item.
 * 
 * @param pItem    The pointer to the item which is to be deallocated.
 */
void _PQueueItemDestroy(void *pItem);


/*===========================================================================*
 *                Implementation for exported functions                      *
 *===========================================================================*/
bool PQueueInit(PriorityQueue *self) {

    DynamicArray_init(_pArray, SIZE_INIT_CAPACITY);    
    if (_pArray == NULL)
        return false;

    _ulSize = 0;

    /* Let the function pointers point to the corresponding functions. */
    _pCompare = _PQueueItemCompare;
    _pDestroy = _PQueueItemDestroy;    

    self->push = PQueuePush;
    self->pop = PQueuePop;
    self->top = PQueueTop;
    self->size = PQueueSize;
    
    self->set_compare = PQueueSetCompare;
    self->set_destroy = PQueueSetDestroy;

    return true;
}


void PQueueDeinit(PriorityQueue *self) {
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
 * PQueuePush(): Insert an item to the appropriate position of the queue.
 */
bool PQueuePush(PriorityQueue *self, void *pItem) {
    int             rc;    
    bool            status;    
    unsigned long   ulCapacity, idxCurr, idxParent;
    void            *pItemCurr, *pItemParent;

    /* Expand the capacity of the internal array if necessary. */
    ulCapacity = _pArray->capacity(_pArray);
    if ((_ulSize + 1) == ulCapacity) {
        status = _pArray->resize(_pArray, TIMES_EXPAND_CAPACITY);
        if (status == false)
            return false;    
    }

    /* Insert the item to the tail of the array. */
    _pArray->put(_pArray, pItem, _ulSize);
    _ulSize++;

    /* Maintain the priority attributes. */
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
 * PQueuePop(): Retrieve and delete the top item from the queue.
 */
void* PQueuePop(PriorityQueue *self) {
    unsigned long   ulCapacity, idxCurr, idxLeft, idxRight, idxLarge;
    void            *pItemRet, *pItemTail, *pItemCurr, *pItemLeft, *pItemRight, *pItemLarge;

    /* Reject the invalid request. */
    if (_ulSize == 0)
        return NULL;

    /* Shrink the capacity of the internal array if necessary. */
    ulCapacity = _pArray->capacity(_pArray);
    if ((_ulSize + 1) < (ulCapacity * TIMES_SHRINK_CAPACITY))
        _pArray->resize(_pArray, TIMES_SHRINK_CAPACITY);

    /* Retrive the item from the head of the array. */
    pItemRet = _pArray->get(_pArray, INDEX_ROOT - 1);
    pItemTail = _pArray->get(_pArray, _ulSize - 1);    
    _pArray->put(_pArray, pItemTail, INDEX_ROOT - 1);
    _ulSize--;

    /* Maintain the priority attributes. */
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
 * PQueueTop(): Retrieve the top item from the queue.
 */
void* PQueueTop(PriorityQueue *self) {

    return _pArray->get(_pArray, INDEX_ROOT - 1);
}


/**
 * PQueueSize(): Return the size of the queue.
 */
unsigned long PQueueSize(PriorityQueue *self) {

    return _ulSize;
}


/**
 * PQueueSetCompare(): Set the item comparison strategy with the one defined by user.
 */
void PQueueSetCompare(PriorityQueue *self, int (*pFunc)(const void*, const void*)) {

    _pCompare = pFunc;
    return;
}


/**
 * PQueueSetDestroy(): Set the item deallocation strategy with the one defined by user.
 */
void PQueueSetDestroy(PriorityQueue *self, void (*pFunc)(void*)) {
    
    _pDestroy = pFunc;
    return;
}


/*===========================================================================*
 *                Implementation for internal functions                      *
 *===========================================================================*/
/**
 * _PQueueItemCompare(): The default comparison strategy for a pair of items.
 * Note: It considers source and target items as primitive data and 
 *       gives the higher priority to the one with larger value.
 */
int _PQueueItemCompare(const void *pSrc, const void *pTge) {
    
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
 * _PQueueItemDestroy(): The default deallocation strategy for an item.
 * Note: By default, the item is a primitive data, and thus no further operations are required.
 */
void _PQueueItemDestroy(void *pItem) {

    return;
}

