#include "simple_tree.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
struct _SimTreeNode {
    Item item;
    struct _SimTreeNode *pParent;
    struct _SimTreeNode *pLeft;
    struct _SimTreeNode *pRight;
};

struct _SimTreeData {
    uint32_t _uiSize;
    SimTreeNode *_pRoot;
    int32_t (*_pCompare)(Item, Item);
    void (*_pDestroy)(Item);
};

#define DIRECTION_LEFT      (0)
#define DIRECTION_RIGHT     (1)


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * Insert the requested item into the proper location of the tree.
 */
int32_t SimTreeInsert(SimpleTree *self, Item item);

/**
 * Check whether the tree has the requested item.
 */
int32_t SimTreeSearch(SimpleTree *self, Item *pItem);

/**
 * Delete the requested item from the tree and adjust the tree structure.
 */
int32_t SimTreeDelete(SimpleTree *self, Item item);

/**
 * Return the item with the maximum order of the tree.
 */
int32_t SimTreeMaximum(SimpleTree *self, Item *pItem);

/**
 * Return the item with the minimum order of the tree.
 */
int32_t SimTreeMinimum(SimpleTree *self, Item *pItem);

/**
 * Return the item which is the successor of the requested one.
 */
int32_t SimTreeSuccessor(SimpleTree *self, Item *pItem);

/**
 * Return the item which is the predecessor of the requested one.
 */
int32_t SimTreePredecessor(SimpleTree *self, Item *pItem);

/**
 * Return the size of the tree.
 */
uint32_t SimTreeSize(SimpleTree *self);

/**
 * Set the user defined item comparison strategy.
 */
void SimTreeSetCompare(SimpleTree *self, int32_t (*pFunc) (Item, Item));

/**
 * Set the user defined item deallocation strategy.
 */
void SimTreeSetDestroy(SimpleTree *self, void (*pFunc) (Item));


/*===========================================================================*
 *                  Definition for internal functions                        *
 *===========================================================================*/
/**
 * Traverse all the nodes and release the allocated memory space.
 *
 * @param pData     The pointer to the tree private data.
 */
void _SimTreeDeinit(SimTreeData *pData);

/**
 * Return the node with maximal order in the subtree rooted by the requested node.
 *
 * @param pCurr      The pointer to the requested node.
 *
 * @return           Non-NULL: The pointer to the target node.
 *                   NULL    : The subtree is empty.
 */
SimTreeNode* _SimTreeMaximal(SimTreeNode *pCurr);

/**
 * Return the node with minimal order in the subtree rooted by the requested node.
 *
 * @param pCurr      The pointer to the requested node.
 *
 * @return           Non-NULL: The pointer to the target node.
 *                   NULL    : The subtree is empty.
 */
SimTreeNode* _SimTreeMinimal(SimTreeNode *pCurr);

/**
 * Return the successor of the requested node.
 *
 * @param pCurr      The pointer to the requested node.
 *
 * @return           Non-NULL: The pointer to the target node.
 *                   NULL    : No successor can be found.
 */
SimTreeNode* _SimTreeSuccessor(SimTreeNode *pCurr);

/**
 * Return the predecessor of the requested node.
 *
 * @param pCurr      The pointer to the requested node.
 *
 * @return           Non-NULL: The pointer to the target node.
 *                   NULL    : No predecessor can be found.
 */
SimTreeNode* _SimTreePredecessor(SimTreeNode *pCurr);

/**
 * Return the node containing the requested item.
 *
 * @param pData      The pointer to tree private data.
 * @param item       The pointer to the requested item.
 *
 * @return           Non-NULL: The pointer to the target node.
 *                   NULL    : No matched node.
 */
SimTreeNode* _SimTreeSearch(SimTreeData *pData, Item item);

/**
 * This function is the default item comparison strategy for a pair of tree nodes.
 * Note that it considers source and target items as primitive data and gives the
 * larger order to the one with larger value.
 *
 * @param itemSrc       The source item.
 * @param itemTge       The target item.
 *
 * @return        > 0 : The source item goes after the target one.
 *                  0 : The source item equals to the target one.
 *                < 0 : The source item goes before the target one.
 */
int32_t _SimTreeItemCompare(Item itemSrc, Item itemTge);

/**
 * This function is the default deallocation strategy for an item.
 *
 * @param item         The requested item.
 */
void _SimTreeItemDestroy(Item item);

#ifdef DEBUG
/**
 * This function is used for tree structure verification and is only built
 * for debug version.
 *
 * @param pData         The pointer to the tree private data.
 *
 * @return          true|false: Whether the tree has legal structure.
 */
bool _SimTreeValidate(SimTreeData *pData);
#endif

/*===========================================================================*
 *           Implementation for the exported member operations               *
 *===========================================================================*/
int32_t SimTreeInit(SimpleTree **ppObj)
{
    SimpleTree *pObj;
    *ppObj = (SimpleTree*)malloc(sizeof(SimpleTree));
    if (!(*ppObj))
        return FAIL_NO_MEMORY;
    pObj = *ppObj;

    pObj->pData = (SimTreeData*)malloc(sizeof(SimTreeData));
    if (!pObj->pData) {
        free(*ppObj);
        *ppObj = NULL;
        return FAIL_NO_MEMORY;
    }
    pObj->pData->_pRoot = NULL;
    pObj->pData->_uiSize = 0;
    pObj->pData->_pCompare = _SimTreeItemCompare;
    pObj->pData->_pDestroy = _SimTreeItemDestroy;

    pObj->insert = SimTreeInsert;
    pObj->search = SimTreeSearch;
    pObj->delete = SimTreeDelete;
    pObj->maximum = SimTreeMaximum;
    pObj->minimum = SimTreeMinimum;
    pObj->successor = SimTreeSuccessor;
    pObj->predecessor = SimTreePredecessor;
    pObj->size = SimTreeSize;
    pObj->set_compare = SimTreeSetCompare;
    pObj->set_destroy = SimTreeSetDestroy;

    return SUCCESS;
}

void SimTreeDeinit(SimpleTree **ppObj)
{
    SimpleTree *pObj;
    if (*ppObj) {
        pObj = *ppObj;
        if (pObj->pData) {
            _SimTreeDeinit(pObj->pData);
            free(pObj->pData);
        }
        free(*ppObj);
        *ppObj = NULL;
    }
    return;
}

int32_t SimTreeInsert(SimpleTree *self, Item item)
{
    int32_t iOrder;
    int8_t cDirect;
    SimTreeNode *pNew, *pCurr, *pParent;

    pNew = (SimTreeNode*)malloc(sizeof(SimTreeNode));
    if (!pNew)
        return FAIL_NO_MEMORY;
    pNew->item = item;
    pNew->pParent = NULL;
    pNew->pLeft = NULL;
    pNew->pRight = NULL;

    pParent = NULL;
    pCurr = self->pData->_pRoot;
    while (pCurr) {
        pParent = pCurr;
        iOrder = self->pData->_pCompare(item, pCurr->item);
        if (iOrder > 0) {
            pCurr = pCurr->pRight;
            cDirect = DIRECTION_RIGHT;
        }
        else if (iOrder < 0) {
            pCurr = pCurr->pLeft;
            cDirect = DIRECTION_LEFT;
        }
        else {
            free(pNew);
            return FAIL_DATA_CONFLICT;
        }
    }

    /* Arrive at the proper position. */
    pNew->pParent = pParent;
    if (pParent) {
        if (cDirect == DIRECTION_LEFT)
            pParent->pLeft = pNew;
        else
            pParent->pRight = pNew;
    } else
        self->pData->_pRoot = pNew;

    /* Increase the size. */
    self->pData->_uiSize++;

#ifdef DEBUG
    bool bLegal = _SimTreeValidate(self->pData);
    assert(bLegal == true);
#endif

    return SUCCESS;
}

int32_t SimTreeSearch(SimpleTree *self, Item *pItem)
{
    SimTreeNode *pFind;
    pFind = _SimTreeSearch(self->pData, *pItem);
    if (pFind) {
        *pItem = pFind->item;
        return SUCCESS;
    }
    return FAIL_NO_DATA;
}

int32_t SimTreeDelete(SimpleTree *self, Item item)
{
    SimTreeNode *pCurr, *pChild, *pSucc, *pParent;

    pCurr = _SimTreeSearch(self->pData, item);
    if (!pCurr)
        return FAIL_NO_DATA;

    /* The specified node has no child. */
    if ((!pCurr->pLeft) && (!pCurr->pRight)) {
        if (pCurr->pParent) {
            if (pCurr == pCurr->pParent->pLeft)
                pCurr->pParent->pLeft = NULL;
            else
                pCurr->pParent->pRight = NULL;
        } else
            self->pData->_pRoot = NULL;

        self->pData->_pDestroy(pCurr->item);
        free(pCurr);
    } else {
        /* The specified node has two children. */
        if ((pCurr->pLeft) && (pCurr->pRight)) {
            pSucc = _SimTreeSuccessor(pCurr);

            pChild = pSucc->pLeft;
            if (!pChild)
                pChild = pSucc->pRight;

            if (pChild)
                pChild->pParent = pSucc->pParent;

            if (pSucc == pSucc->pParent->pLeft)
                pSucc->pParent->pLeft = pChild;
            else
                pSucc->pParent->pRight = pChild;

            self->pData->_pDestroy(pCurr->item);
            pCurr->item = pSucc->item;
            free(pSucc);
        }
        /* The specified node has one child. */
        else {
            pChild = pCurr->pLeft;
            if (!pChild)
                pChild = pCurr->pRight;

            pChild->pParent = pCurr->pParent;

            if (pCurr->pParent) {
                if (pCurr == pCurr->pParent->pLeft)
                    pCurr->pParent->pLeft = pChild;
                else
                    pCurr->pParent->pRight = pChild;
            } else
                self->pData->_pRoot = pChild;

            self->pData->_pDestroy(pCurr->item);
            free(pCurr);
        }
    }

    /* Decrease the size. */
    self->pData->_uiSize--;

#ifdef DEBUG
    bool bLegal = _SimTreeValidate(self->pData);
    assert(bLegal == true);
#endif

    return SUCCESS;
}

int32_t SimTreeMaximum(SimpleTree *self, Item *pItem)
{
    SimTreeNode *pFind = _SimTreeMaximal(self->pData->_pRoot);
    if (pFind) {
        *pItem = pFind->item;
        return SUCCESS;
    }
    return FAIL_NO_DATA;
}

int32_t SimTreeMinimum(SimpleTree *self, Item *pItem)
{
    SimTreeNode *pFind = _SimTreeMinimal(self->pData->_pRoot);
    if (pFind) {
        *pItem = pFind->item;
        return SUCCESS;
    }
    return FAIL_NO_DATA;
}

int32_t SimTreeSuccessor(SimpleTree *self, Item *pItem)
{
    SimTreeNode *pFind = _SimTreeSuccessor(self->pData->_pRoot);
    if (pFind) {
        *pItem = pFind->item;
        return SUCCESS;
    }
    return FAIL_NO_DATA;
}

int32_t SimTreePredecessor(SimpleTree *self, Item *pItem)
{
    SimTreeNode *pFind = _SimTreePredecessor(self->pData->_pRoot);
    if (pFind) {
        *pItem = pFind->item;
        return SUCCESS;
    }
    return FAIL_NO_DATA;
}

uint32_t SimTreeSize(SimpleTree *self)
{
    return self->pData->_uiSize;
}

void SimTreeSetCompare(SimpleTree *self, int32_t (*pFunc) (Item, Item))
{
    self->pData->_pCompare = pFunc;
    return;
}

void SimTreeSetDestroy(SimpleTree *self, void (*pFunc) (Item))
{
    self->pData->_pDestroy = pFunc;
    return;
}


/*===========================================================================*
 *               Implementation for internal functions                       *
 *===========================================================================*/
void _SimTreeDeinit(SimTreeData *pData)
{
    if (!(pData->_pRoot))
        return;

    /* Simulate the stack and apply iterative post-order tree traversal. */
    SimTreeNode ***stack = (SimTreeNode***)malloc(sizeof(SimTreeNode**) * pData->_uiSize);
    assert(stack != NULL);

    uint32_t uiSize = 0;
    stack[uiSize++] = &(pData->_pRoot);
    while (uiSize > 0) {
        SimTreeNode **ppCurr = stack[uiSize - 1];
        SimTreeNode *pCurr = *ppCurr;
        if (pCurr->pLeft)
            stack[uiSize++] = &(pCurr->pLeft);
        else if (pCurr->pRight)
            stack[uiSize++] = &(pCurr->pRight);
        else {
            pData->_pDestroy(pCurr->item);
            free(pCurr);
            *ppCurr = NULL;
            uiSize--;
        }
    }
    free(stack);

    return;
}

SimTreeNode* _SimTreeMinimal(SimTreeNode *pCurr)
{
    SimTreeNode *pParent = NULL;
    while (pCurr) {
        pParent = pCurr;
        pCurr = pCurr->pLeft;
    }
    return pParent;
}

SimTreeNode* _SimTreeMaximal(SimTreeNode *pCurr)
{
    SimTreeNode *pParent = NULL;
    while (pCurr) {
        pParent = pCurr;
        pCurr = pCurr->pRight;
    }
    return pParent;
}

SimTreeNode* _SimTreeSuccessor(SimTreeNode *pCurr)
{
    if (pCurr) {
        /* Case 1: The minimal node of the non-null right subtree. */
        if (pCurr->pRight)
            pCurr = _SimTreeMinimal(pCurr->pRight);

        /* Case 2: The ancestor which considers the designated node as the maximal node of
           its left subtree. */
        else {
            while((pCurr->pParent) && (pCurr == pCurr->pParent->pRight))
                pCurr = pCurr->pParent;
            pCurr = pCurr->pParent;
        }
    }
    return pCurr;
}

SimTreeNode* _SimTreePredecessor(SimTreeNode *pCurr)
{
    if (pCurr) {
        /* Case 1: The maximal node of the non-null left subtree. */
        if (pCurr->pLeft)
            pCurr = _SimTreeMaximal(pCurr->pLeft);

        /* Case 2: The ancestor which considers the designated node as the minimal node of
           its right subtree. */
        else {
            while((pCurr->pParent) && (pCurr == pCurr->pParent->pLeft))
                pCurr = pCurr->pParent;
            pCurr = pCurr->pParent;
        }
    }
    return pCurr;
}

SimTreeNode* _SimTreeSearch(SimTreeData *pData, Item item)
{
    int32_t iOrder;
    SimTreeNode *pCurr = pData->_pRoot;
    while(pCurr) {
        iOrder = pData->_pCompare(item, pCurr->item);
        if (iOrder == 0)
            break;
        else {
            if (iOrder > 0)
                pCurr = pCurr->pRight;
            else
                pCurr = pCurr->pLeft;
        }
    }
    return pCurr;
}

int32_t _SimTreeItemCompare(Item itemSrc, Item itemTge)
{
    int32_t iSrc, iTge;

#if __x86_64__
    int64_t lTmp;
    lTmp = (int64_t)itemSrc;
    iSrc = (int32_t)lTmp;
    lTmp = (int64_t)itemTge;
    iTge = (int32_t)lTmp;
#else
    iSrc = (int32_t)itemSrc;
    iTge = (int32_t)itemTge;
#endif

    return iSrc - iTge;
}

void _SimTreeItemDestroy(Item item)
{ return; }

#ifdef DEBUG
bool _SimTreeValidate(SimTreeData *pData)
{
    bool bLegal = true;

    /* Simulate the stack and apply iterative in order tree traversal. */
    SimTreeNode **stack = (SimTreeNode**)malloc(sizeof(SimTreeNode*) * pData->_uiSize);
    SimTreeNode *pCurr = pData->_pRoot;
    SimTreeNode *pPred = NULL;
    uint32_t uiSize = 0;

    while (pCurr || (uiSize > 0)) {
        if (pCurr) {
            stack[uiSize++] = pCurr;
            pCurr = pCurr->pLeft;
        } else {
            if (pPred) {
                pCurr = stack[uiSize - 1];
                int32_t iOrder = pData->_pCompare(pPred->item, pCurr->item);
                if (iOrder >= 0)
                    bLegal = false;
            }
            pPred = stack[--uiSize];
            pCurr = pPred->pRight;
        }
    }
    free(stack);

    return bLegal;
}
#endif
