#include "ordered_map.h"


bool OrderedMapInit(OrderedMap *self) {
    
    self->pTree = (RedBlackTree*)malloc(sizeof(RedBlackTree));
    if (self->pTree == NULL)
        return false;

    self->compare = OrderedMapPairCompare;
    self->destroy = OrderedMapPairDestroy;

    self->put = OrderedMapPut;
    self->get = OrderedMapGet;    
    
    return true;
}


void OrderedMapDeinit(OrderedMap *self) {

    if (self->pTree != NULL) {
        free(self->pTree);
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
    if (pNode != NULL)        
        return true;
    else
        return false;
}


KeyValuePair* OrderedMapGet(OrderedMap *self, void *pKey) {
    RedBlackTree *pTree;
    RedBlackNode *pNode;
    KeyValuePair pair;

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
