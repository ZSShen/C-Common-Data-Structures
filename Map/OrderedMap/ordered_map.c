#include "ordered_map.h"

static unsigned long ulSize;

bool OrderedMapInit(OrderedMap *self) {
    
    ulSize = 0;

    /* Let the pointers of ordered map point to the corresponding functions. */
    self->compare = OrderedMapPairCompare;
    self->destroy = OrderedMapPairDestroy;

    self->put = OrderedMapPut;
    self->get = OrderedMapGet;
    self->remove = OrderedMapRemove;
    self->size = OrderedMapSize;

    self->set_compare = OrderedMapSetCompare;
    self->set_destroy = OrderedMapSetDestroy;

    /* Initialize the red black tree. */    
    RedBlackTree_init(self->pTree);

    /* Replace the comparion and item deallocation strategies. */    
    self->pTree->compare = self->compare;
    self->pTree->destroy = self->destroy;

    return true;
}


void OrderedMapDeinit(OrderedMap *self) {

    if (self->pTree != NULL) {
        RedBlackTree_deinit(self->pTree);
    }

    return;
}


/**
 * OrderedMapPairCompare(): The default strategy for key-value pair comparison.
 * Note: It considers the pKey member of the pair as primitive data.
 */
int OrderedMapPairCompare(const void *pSrc, const void *pTge) {
    
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
 * RBTreeNodeDestroy(): The default strategy for key-value pair deallocation.
 * Note: It considers the pKey and pValue members as primitive data and frees the pair directory.
 */
void OrderedMapPairDestroy(void *pPair) {

    free(pPair);
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
