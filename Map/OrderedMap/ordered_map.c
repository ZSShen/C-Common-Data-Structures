#include "ordered_map.h"

/*===========================================================================*
 *                  Simulation for private variables                         *
 *===========================================================================*/
static unsigned long ulSize;
static int  (*pCompare) (const void*, const void*);
static void (*pDestroy) (void*);


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
 *                Implementation for external functions                      *
 *===========================================================================*/
bool OrderedMapInit(OrderedMap *self) {
    
    ulSize = 0;

    /* Let the function pointers point to the corresponding functions. */
    pCompare = _OrderedMapPairCompare;
    pDestroy = _OrderedMapPairDestroy;

    self->put = OrderedMapPut;
    self->get = OrderedMapGet;
    self->remove = OrderedMapRemove;
    self->size = OrderedMapSize;

    self->set_compare = OrderedMapSetCompare;
    self->set_destroy = OrderedMapSetDestroy;

    /* Initialize the red black tree. */    
    RedBlackTree_init(self->pTree);

    /* Replace the comparion and item deallocation strategies. */    
    self->pTree->compare = pCompare;
    self->pTree->destroy = pDestroy;

    return true;
}


void OrderedMapDeinit(OrderedMap *self) {

    if (self->pTree != NULL) {
        RedBlackTree_deinit(self->pTree);
    }

    return;
}


bool OrderedMapPut(OrderedMap *self, KeyValuePair *pPair) {
    RedBlackTree *pTree;    
    RedBlackNode *pNode;
    
    pTree = self->pTree;
    pNode = pTree->insert(pTree, pPair);
    if (pNode != NULL) {       
        ulSize++;
        return true;
    } else
        return false;
}


KeyValuePair* OrderedMapGet(OrderedMap *self, void *pKey) {
    RedBlackTree *pTree;    
    RedBlackNode *pNode;
    KeyValuePair  pair;

    /* Create a query key-value pair. */
    pair.pKey = pKey;
    pair.pValue = NULL;

    pTree = self->pTree;
    pNode = pTree->search(pTree, &pair);
    if (pNode != NULL)
        return pNode->pItem;
    else 
        return NULL;
}


bool OrderedMapRemove(OrderedMap *self, void *pKey) {
    RedBlackTree *pTree;
    RedBlackNode *pNode;
    KeyValuePair pair;    

    /* Create a query key-value pair. */
    pair.pKey = pKey;
    pair.pValue = NULL;

    pTree = self->pTree;
    pNode = pTree->search(pTree, &pair);
    if (pNode != NULL) {
        pTree->delete(pTree, pNode);
        ulSize--;
    } else
        return false;
}


unsigned long OrderedMapSize(OrderedMap *self) {

    return ulSize;
}


void OrderedMapSetCompare(OrderedMap *self, int (*pFunc) (const void*, const void*)) {

    ((RedBlackTree*)self->pTree)->compare = pFunc;
    return;
}


void OrderedMapSetDestroy(OrderedMap *self, void (*pFunc) (void*)) {

    ((RedBlackTree*)self->pTree)->destroy = pFunc;
    return;
}


/*===========================================================================*
 *                Implementation for internal functions                      *
 *===========================================================================*/
/**
 * _OrderedMapPairCompare(): The default strategy for key-value pair comparison.
 * Note: It considers the pKey member of the pair as primitive data.
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
 * _OrderedMapPairDestroy(): The default strategy for key-value pair deallocation.
 * Note: It considers the pKey and pValue members as primitive data and deallocates the pair directory.
 */
void _OrderedMapPairDestroy(void *pPair) {

    free(pPair);
    return;
}
