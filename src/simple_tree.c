#include "simple_tree.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
typedef struct _SimTreeNode {
    Item item;
    struct _SimTreeNode *pParent;
    struct _SimTreeNode *pLeft;
    struct _SimTreeNode *pRight;
} SimTreeNode;

struct _SimTreeData {
    int32_t _iSize;
    SimTreeNode *_pRoot;
    int32_t (*_pCompare)(Item, Item);
    void (*_pDestroy)(Item);
};

#define DIRECT_LEFT      (0)
#define DIRECT_RIGHT     (1)


/*===========================================================================*
 *                  Definition for internal functions                        *
 *===========================================================================*/
/**
 * @brief Traverse all the nodes and clean the allocated resource.
 *
 * @param pData         The pointer to the tree private data
 */
void _SimTreeDeinit(SimTreeData *pData);

/**
 * @brief Return the node with maximal order in the subtree rooted by the
 * requested node.
 *
 * @param pCurr         The pointer to the requested node
 *
 * @return Non-NULL     The pointer to the returned node
 * @return NULL         The subtree is empty
 */
SimTreeNode* _SimTreeMaximal(SimTreeNode *pCurr);

/**
 * @brief Return the node with minimal order in the subtree rooted by the
 * requested node.
 *
 * @param pCurr         The pointer to the requested node
 *
 * @return Non-NULL     The pointer to the target node
 * @return NULL         The subtree is empty
 */
SimTreeNode* _SimTreeMinimal(SimTreeNode *pCurr);

/**
 * @brief Return the immediate successor of the requested node.
 *
 * @param pCurr         The pointer to the requested node
 *
 * @return Non-NULL     The pointer to the returned node
 * @return NULL         The immediate successor cannot be found
 */
SimTreeNode* _SimTreeSuccessor(SimTreeNode *pCurr);

/**
 * @brief Return the immediate predecessor of the requested node.
 *
 * @param pCurr         The pointer to the requested node
 *
 * @return Non-NULL     The pointer to the returned node
 * @return NULL         The immediate predecessor cannot be found
 */
SimTreeNode* _SimTreePredecessor(SimTreeNode *pCurr);

/**
 * @brief Get the node which stores the item having the same order with the
 * requested one.
 *
 * @param pData         The pointer to tree private data
 * @param item          The pointer to the requested item
 *
 * @return Non-NULL     The pointer to the target node
 * @return NULL         No matched node
 */
SimTreeNode* _SimTreeSearch(SimTreeData *pData, Item item);

/**
 * @brief The default order comparison strategy for a pair of items.
 *
 * This function considers the source and target items as primitive data and
 * gives the larger order to the item having the larger value.
 *
 * @param itemSrc       The source item
 * @param itemTge       The target item
 *
 * @retval 1            The source item has the larger order
 * @retval 0            Both the items have the same order
 * @retval -1           The source item has the smaller order
 */
int32_t _SimTreeItemCompare(Item itemSrc, Item itemTge);

/**
 * @brief The default clean strategy for the resource hold by an item.
 *
 * @param item         The requested item
 */
void _SimTreeItemDestroy(Item item);

/**
 * @brief The internal debug function to verify the tree structure invariant.
 *
 * @param pData         The pointer to the tree private data
 *
 * @return true|false   Whether the tree has legal structure.
 */
bool _SimTreeValidate(SimTreeData *pData);


/*===========================================================================*
 *           Implementation for the exported member operations               *
 *===========================================================================*/
int32_t SimTreeInit(SimpleTree **ppObj)
{
    SimpleTree *pObj;
    *ppObj = (SimpleTree*)malloc(sizeof(SimpleTree));
    if (!(*ppObj))
        return ERR_NOMEM;
    pObj = *ppObj;

    pObj->pData = (SimTreeData*)malloc(sizeof(SimTreeData));
    if (!pObj->pData) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    pObj->pData->_pRoot = NULL;
    pObj->pData->_iSize = 0;
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

    return SUCC;
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
        return ERR_NOMEM;
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
            cDirect = DIRECT_RIGHT;
        }
        else if (iOrder < 0) {
            pCurr = pCurr->pLeft;
            cDirect = DIRECT_LEFT;
        }
        else {
            /* Conflict with the already stored item. */
            free(pNew);
            self->pData->_pDestroy(pCurr->item);
            pCurr->item = item;
            return SUCC;
        }
    }

    /* Arrive at the proper position. */
    pNew->pParent = pParent;
    if (pParent) {
        if (cDirect == DIRECT_LEFT)
            pParent->pLeft = pNew;
        else
            pParent->pRight = pNew;
    } else
        self->pData->_pRoot = pNew;

    /* Increase the size. */
    self->pData->_iSize++;

    return SUCC;
}

int32_t SimTreeSearch(SimpleTree *self, Item itemIn, Item *pItemOut)
{
    SimTreeNode *pFind;
    pFind = _SimTreeSearch(self->pData, itemIn);
    if (pFind) {
        *pItemOut = pFind->item;
        return SUCC;
    }
    *pItemOut = NULL;
    return ERR_NODATA;
}

int32_t SimTreeDelete(SimpleTree *self, Item item)
{
    SimTreeNode *pCurr, *pChild, *pSucc, *pParent;

    pCurr = _SimTreeSearch(self->pData, item);
    if (!pCurr)
        return ERR_NODATA;

    /* The target node has no child. */
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
        /* The target node has two children. */
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
        /* The target node has one child. */
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
    self->pData->_iSize--;

    return SUCC;
}

int32_t SimTreeMaximum(SimpleTree *self, Item *pItem)
{
    SimTreeNode *pFind = _SimTreeMaximal(self->pData->_pRoot);
    if (pFind) {
        *pItem = pFind->item;
        return SUCC;
    }
    return ERR_IDX;
}

int32_t SimTreeMinimum(SimpleTree *self, Item *pItem)
{
    SimTreeNode *pFind = _SimTreeMinimal(self->pData->_pRoot);
    if (pFind) {
        *pItem = pFind->item;
        return SUCC;
    }
    return ERR_IDX;
}

int32_t SimTreeSuccessor(SimpleTree *self, Item itemIn, Item *pItemOut)
{
    SimTreeNode *pCurr = _SimTreeSearch(self->pData, itemIn);
    if (!pCurr)
        return ERR_NODATA;

    SimTreeNode *pFind = _SimTreeSuccessor(pCurr);
    if (pFind) {
        *pItemOut = pFind->item;
        return SUCC;
    }
    return ERR_NODATA;
}

int32_t SimTreePredecessor(SimpleTree *self, Item itemIn, Item *pItemOut)
{
    SimTreeNode *pCurr = _SimTreeSearch(self->pData, itemIn);
    if (!pCurr)
        return ERR_NODATA;

    SimTreeNode *pFind = _SimTreePredecessor(pCurr);
    if (pFind) {
        *pItemOut = pFind->item;
        return SUCC;
    }
    return ERR_NODATA;
}

int32_t SimTreeSize(SimpleTree *self)
{
    return self->pData->_iSize;
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
    SimTreeNode ***stack = (SimTreeNode***)malloc(sizeof(SimTreeNode**) * pData->_iSize);
    assert(stack != NULL);

    int32_t iSize = 0;
    stack[iSize++] = &(pData->_pRoot);
    while (iSize > 0) {
        SimTreeNode **ppCurr = stack[iSize - 1];
        SimTreeNode *pCurr = *ppCurr;
        if (pCurr->pLeft)
            stack[iSize++] = &(pCurr->pLeft);
        else if (pCurr->pRight)
            stack[iSize++] = &(pCurr->pRight);
        else {
            pData->_pDestroy(pCurr->item);
            free(pCurr);
            *ppCurr = NULL;
            iSize--;
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
        /* Case 1: The minimal node in the non-null right subtree. */
        if (pCurr->pRight)
            pCurr = _SimTreeMinimal(pCurr->pRight);

        /* Case 2: The ancestor which considers the designated node as the
           maximal node of its left subtree. */
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
        /* Case 1: The maximal node in the non-null left subtree. */
        if (pCurr->pLeft)
            pCurr = _SimTreeMaximal(pCurr->pLeft);

        /* Case 2: The ancestor which considers the designated node as the
           minimal node of its right subtree. */
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
    if (itemSrc == itemTge)
        return 0;
    else
        return (itemSrc > itemTge)? 1 : -1;
}

void _SimTreeItemDestroy(Item item) {}

bool _SimTreeValidate(SimTreeData *pData)
{
    bool bLegal = true;

    /* Simulate the stack and apply iterative in order tree traversal. */
    SimTreeNode **stack = (SimTreeNode**)malloc(sizeof(SimTreeNode*) * pData->_iSize);
    SimTreeNode *pCurr = pData->_pRoot;
    SimTreeNode *pPred = NULL;
    int32_t iSize = 0;

    while (pCurr || (iSize > 0)) {
        if (pCurr) {
            stack[iSize++] = pCurr;
            pCurr = pCurr->pLeft;
        } else {
            if (pPred) {
                pCurr = stack[iSize - 1];
                int32_t iOrder = pData->_pCompare(pPred->item, pCurr->item);
                if (iOrder >= 0)
                    bLegal = false;
            }
            pPred = stack[--iSize];
            pCurr = pPred->pRight;
        }
    }
    free(stack);

    return bLegal;
}
