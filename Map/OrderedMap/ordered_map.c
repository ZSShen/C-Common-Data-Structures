#include "red_black_tree.h"
#include "ordered_map.h"

/*===========================================================================*
 *                  Simulation for private variables                         *
 *===========================================================================*/
static unsigned long    _ulSize;
static RedBlackTree    *_pTree;
static int  (*_pCompare) (const void*, const void*);
static void (*_pDestroy) (void*);


/*===========================================================================*
 *                  Definition for internal functions                        *
 *===========================================================================*/
/**
 * The default function for key-value pair comparison.
 *
 * @param pSrc          The pointer to the source pair.
 * @param pDst          The pointer to the target pair.
 *
 * @return               1: The source pair goes after the target one with certain ordering criteria.
 *                       0: The source pair equals to the target one with certain ordering criteria.
 *                      -1: The source pair goes before the target one with certain ordering criteria.
 */
int _OrderedMapPairCompare(const void *pSrc, const void *pTge);


/**
 * The default function for key-value pair deallocation.
 * 
 * @param pPair         The pointer to the pair which is to be deallocated.
 */
void _OrderedMapPairDestroy(void *pPair);


/*===========================================================================*
 *                Implementation for exported functions                      *
 *===========================================================================*/
bool OrderedMapInit(OrderedMap *self) {
    
    _ulSize = 0;

    /* Let the function pointers point to the corresponding functions. */
    _pCompare = _OrderedMapPairCompare;
    _pDestroy = _OrderedMapPairDestroy;

    self->put = OrderedMapPut;
    self->get = OrderedMapGet;
    self->remove = OrderedMapRemove;
    self->size = OrderedMapSize;

    self->set_compare = OrderedMapSetCompare;
    self->set_destroy = OrderedMapSetDestroy;

    /* Initialize the red black tree. */    
    RedBlackTree_init(_pTree);
    if (_pTree == NULL)
        return false;

    /* Replace the item comparion and deallocation strategies. */    
    _pTree->set_compare(_pTree, _pCompare);
    _pTree->set_destroy(_pTree, _pDestroy);

    return true;
}


void OrderedMapDeinit(OrderedMap *self) {

    if (_pTree != NULL) {
        RedBlackTree_deinit(_pTree);
    }

    return;
}


/**
 * OrderedMapPut(): Insert a key-value pair into the appropriate position of the map.
 */
bool OrderedMapPut(OrderedMap *self, KeyValuePair *pPair) {
    RedBlackNode *pNode;
    
    pNode = _pTree->insert(_pTree, pPair);
    if (pNode != NULL) {       
        _ulSize++;
        return true;
    } else
        return false;
}


/**
 * OrderedMapGet(): Search and return a key-value pair using the specified key.
 */
KeyValuePair* OrderedMapGet(OrderedMap *self, void *pKey) {
    RedBlackNode *pNode;
    KeyValuePair  pair;

    /* Create a query key-value pair. */
    pair.pKey = pKey;
    pair.pValue = NULL;

    pNode = _pTree->search(_pTree, &pair);
    if (pNode != NULL)
        return pNode->pItem;
    else 
        return NULL;
}


/**
 * OrderedMapRemove(): Remove the key-value pair with the key equal to the specified one.
 */
bool OrderedMapRemove(OrderedMap *self, void *pKey) {
    RedBlackNode *pNode;
    KeyValuePair pair;    

    /* Create a query key-value pair. */
    pair.pKey = pKey;
    pair.pValue = NULL;

    pNode = _pTree->search(_pTree, &pair);
    if (pNode != NULL) {
        _pTree->delete(_pTree, pNode);
        _ulSize--;
    } else
        return false;
}


/**
 * OrderedMapSize(): Return the size of the map.
 */
unsigned long OrderedMapSize(OrderedMap *self) {

    return _ulSize;
}


/**
 * OrderedMapSetCompare(): Set the key-value pair comparison strategy with the one defined by user.
 */
void OrderedMapSetCompare(OrderedMap *self, int (*pFunc) (const void*, const void*)) {

    _pTree->set_compare(_pTree, pFunc);
    return;
}


/**
 * OrderedMapSetDestroy(): Set the key-value pair deallocation strategy with the one defined by user.
 */
void OrderedMapSetDestroy(OrderedMap *self, void (*pFunc) (void*)) {

    _pTree->set_destroy(_pTree, pFunc);
    return;
}


/*===========================================================================*
 *                Implementation for internal functions                      *
 *===========================================================================*/
/**
 * _OrderedMapPairCompare(): It considers the pKey members of the pairs as primitive data and 
 *                           gives the larger order to the pair with larger pKey value.
 */
int _OrderedMapPairCompare(const void *pSrc, const void *pTge) {
    
    if (((KeyValuePair*)pSrc)->pKey == ((KeyValuePair*)pTge)->pKey)
        return 0;
    else {
        if (((KeyValuePair*)pSrc)->pKey > ((KeyValuePair*)pTge)->pKey)
            return 1;
        else
            return -1;
    }
}


/**
 * _OrderedMapPairDestroy(): It considers the pKey and pValue members as primitive data and deallocates the pair directory.
 */
void _OrderedMapPairDestroy(void *pPair) {

    free(pPair);
    return;
}
