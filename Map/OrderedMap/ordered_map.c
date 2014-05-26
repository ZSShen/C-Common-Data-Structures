#include "ordered_map.h"


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
