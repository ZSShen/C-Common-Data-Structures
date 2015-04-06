#include "simple_tree.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
struct _TreeNode {
    Item item;
    struct _TreeNode *pParent;
    struct _TreeNode *pLeft;
    struct _TreeNode *pRight;
};

struct _BSTreeData {
    uint32_t _uiSize;
    TreeNode *_pRoot;
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
int32_t BSTreeInsert(BinSearchTree *self, Item item);

/**
 * Check whether the tree has the requested item.
 */
int32_t BSTreeSearch(BinSearchTree *self, Item *pItem);

/**
 * Delete the requested item from the tree and adjust the tree structure.
 */
int32_t BSTreeDelete(BinSearchTree *self, Item item);

/**
 * Return the item with the maximum order of the tree.
 */
int32_t BSTreeMaximum(BinSearchTree *self, Item *pItem);

/**
 * Return the item with the minimum order of the tree.
 */
int32_t BSTreeMinimum(BinSearchTree *self, Item *pItem);

/**
 * Return the item which is the successor of the requested one.
 */
int32_t BSTreeSuccessor(BinSearchTree *self, Item *pItem);

/**
 * Return the item which is the predecessor of the requested one.
 */
int32_t BSTreePredecessor(BinSearchTree *self, Item *pItem);

/**
 * Return the size of the tree.
 */
uint32_t BSTreeSize(BinSearchTree *self);

/**
 * Set the user defined item comparison strategy.
 */
void BSTreeSetCompare(BinSearchTree *self, int32_t (*pFunc) (Item, Item));

/**
 * Set the user defined item deallocation strategy.
 */
void BSTreeSetDestroy(BinSearchTree *self, void (*pFunc) (Item));


/*===========================================================================*
 *                  Definition for internal functions                        *
 *===========================================================================*/
/**
 * Traverse all the nodes and release the allocated memory space.
 *
 * @param pData     The pointer to the tree private data.
 */
void _BSTreeDeinit(BSTreeData *pData);

/**
 * Return the node with maximal order in the subtree rooted by the requested node.
 *
 * @param pCurr      The pointer to the requested node.
 *
 * @return           Non-NULL: The pointer to the target node.
 *                   NULL    : The subtree is empty.
 */
TreeNode* _BSTreeMaximal(TreeNode *pCurr);

/**
 * Return the node with minimal order in the subtree rooted by the requested node.
 *
 * @param pCurr      The pointer to the requested node.
 *
 * @return           Non-NULL: The pointer to the target node.
 *                   NULL    : The subtree is empty.
 */
TreeNode* _BSTreeMinimal(TreeNode *pCurr);

/**
 * Return the successor of the requested node.
 *
 * @param pCurr      The pointer to the requested node.
 *
 * @return           Non-NULL: The pointer to the target node.
 *                   NULL    : No successor can be found.
 */
TreeNode* _BSTreeSuccessor(TreeNode *pCurr);

/**
 * Return the predecessor of the requested node.
 *
 * @param pCurr      The pointer to the requested node.
 *
 * @return           Non-NULL: The pointer to the target node.
 *                   NULL    : No predecessor can be found.
 */
TreeNode* _BSTreePredecessor(TreeNode *pCurr);

/**
 * Return the node containing the requested item.
 *
 * @param pData      The pointer to tree private data.
 * @param item       The pointer to the requested item.
 *
 * @return           Non-NULL: The pointer to the target node.
 *                   NULL    : No matched node.
 */
TreeNode* _BSTreeSearch(BSTreeData *pData, Item item);

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
int32_t _BSTreeItemCompare(Item itemSrc, Item itemTge);

/**
 * This function is the default deallocation strategy for an item.
 *
 * @param item         The requested item.
 */
void _BSTreeItemDestroy(Item item);

#ifdef DEBUG
/**
 * This function is used for tree structure verification and is only built
 * for debug version.
 *
 * @param pData         The pointer to the tree private data.
 *
 * @return          true|false: Whether the tree has legal structure.
 */
bool _BSTreeValidate(BSTreeData *pData);
#endif

/*===========================================================================*
 *           Implementation for the exported member operations               *
 *===========================================================================*/
int32_t BSTreeInit(BinSearchTree **ppObj)
{
    BinSearchTree *pObj;
    *ppObj = (BinSearchTree*)malloc(sizeof(BinSearchTree));
    if (!(*ppObj))
        return FAIL_NO_MEMORY;
    pObj = *ppObj;

    pObj->pData = (BSTreeData*)malloc(sizeof(BSTreeData));
    if (!pObj->pData) {
        free(*ppObj);
        *ppObj = NULL;
        return FAIL_NO_MEMORY;
    }
    pObj->pData->_pRoot = NULL;
    pObj->pData->_uiSize = 0;
    pObj->pData->_pCompare = _BSTreeItemCompare;
    pObj->pData->_pDestroy = _BSTreeItemDestroy;

    pObj->insert = BSTreeInsert;
    pObj->search = BSTreeSearch;
    pObj->delete = BSTreeDelete;
    pObj->maximum = BSTreeMaximum;
    pObj->minimum = BSTreeMinimum;
    pObj->successor = BSTreeSuccessor;
    pObj->predecessor = BSTreePredecessor;
    pObj->size = BSTreeSize;
    pObj->set_compare = BSTreeSetCompare;
    pObj->set_destroy = BSTreeSetDestroy;

    return SUCCESS;
}

void BSTreeDeinit(BinSearchTree **ppObj)
{
    BinSearchTree *pObj;
    if (*ppObj) {
        pObj = *ppObj;
        if (pObj->pData) {
            _BSTreeDeinit(pObj->pData);
            free(pObj->pData);
        }
        free(*ppObj);
        *ppObj = NULL;
    }
    return;
}

int32_t BSTreeInsert(BinSearchTree *self, Item item)
{
    int32_t iOrder;
    int8_t cDirect;
    TreeNode *pNew, *pCurr, *pParent;

    pNew = (TreeNode*)malloc(sizeof(TreeNode));
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
    bool bLegal = _BSTreeValidate(self->pData);
    assert(bLegal == true);
#endif

    return SUCCESS;
}

int32_t BSTreeSearch(BinSearchTree *self, Item *pItem)
{
    TreeNode *pFind;
    pFind = _BSTreeSearch(self->pData, *pItem);
    if (pFind) {
        *pItem = pFind->item;
        return SUCCESS;
    }
    return FAIL_NO_DATA;
}

int32_t BSTreeDelete(BinSearchTree *self, Item item)
{
    TreeNode *pCurr, *pChild, *pSucc, *pParent;

    pCurr = _BSTreeSearch(self->pData, item);
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
            pSucc = _BSTreeSuccessor(pCurr);

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
    bool bLegal = _BSTreeValidate(self->pData);
    assert(bLegal == true);
#endif

    return SUCCESS;
}

int32_t BSTreeMaximum(BinSearchTree *self, Item *pItem)
{
    TreeNode *pFind = _BSTreeMaximal(self->pData->_pRoot);
    if (pFind) {
        *pItem = pFind->item;
        return SUCCESS;
    }
    return FAIL_NO_DATA;
}

int32_t BSTreeMinimum(BinSearchTree *self, Item *pItem)
{
    TreeNode *pFind = _BSTreeMinimal(self->pData->_pRoot);
    if (pFind) {
        *pItem = pFind->item;
        return SUCCESS;
    }
    return FAIL_NO_DATA;
}

int32_t BSTreeSuccessor(BinSearchTree *self, Item *pItem)
{
    TreeNode *pFind = _BSTreeSuccessor(self->pData->_pRoot);
    if (pFind) {
        *pItem = pFind->item;
        return SUCCESS;
    }
    return FAIL_NO_DATA;
}

int32_t BSTreePredecessor(BinSearchTree *self, Item *pItem)
{
    TreeNode *pFind = _BSTreePredecessor(self->pData->_pRoot);
    if (pFind) {
        *pItem = pFind->item;
        return SUCCESS;
    }
    return FAIL_NO_DATA;
}

uint32_t BSTreeSize(BinSearchTree *self)
{
    return self->pData->_uiSize;
}

void BSTreeSetCompare(BinSearchTree *self, int32_t (*pFunc) (Item, Item))
{
    self->pData->_pCompare = pFunc;
    return;
}

void BSTreeSetDestroy(BinSearchTree *self, void (*pFunc) (Item))
{
    self->pData->_pDestroy = pFunc;
    return;
}


/*===========================================================================*
 *               Implementation for internal functions                       *
 *===========================================================================*/
void _BSTreeDeinit(BSTreeData *pData)
{
    if (!(pData->_pRoot))
        return;

    /* Simulate the stack and apply iterative post-order tree traversal. */
    TreeNode ***stack = (TreeNode***)malloc(sizeof(TreeNode**) * pData->_uiSize);
    assert(stack != NULL);

    uint32_t uiSize = 0;
    stack[uiSize++] = &(pData->_pRoot);
    while (uiSize > 0) {
        TreeNode **ppCurr = stack[uiSize - 1];
        TreeNode *pCurr = *ppCurr;
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

TreeNode* _BSTreeMinimal(TreeNode *pCurr)
{
    TreeNode *pParent = NULL;
    while (pCurr) {
        pParent = pCurr;
        pCurr = pCurr->pLeft;
    }
    return pParent;
}

TreeNode* _BSTreeMaximal(TreeNode *pCurr)
{
    TreeNode *pParent = NULL;
    while (pCurr) {
        pParent = pCurr;
        pCurr = pCurr->pRight;
    }
    return pParent;
}

TreeNode* _BSTreeSuccessor(TreeNode *pCurr)
{
    if (pCurr) {
        /* Case 1: The minimal node of the non-null right subtree. */
        if (pCurr->pRight)
            pCurr = _BSTreeMinimal(pCurr->pRight);

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

TreeNode* _BSTreePredecessor(TreeNode *pCurr)
{
    if (pCurr) {
        /* Case 1: The maximal node of the non-null left subtree. */
        if (pCurr->pLeft)
            pCurr = _BSTreeMaximal(pCurr->pLeft);

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

TreeNode* _BSTreeSearch(BSTreeData *pData, Item item)
{
    int32_t iOrder;
    TreeNode *pCurr = pData->_pRoot;
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

int32_t _BSTreeItemCompare(Item itemSrc, Item itemTge)
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

void _BSTreeItemDestroy(Item item)
{ return; }

#ifdef DEBUG
bool _BSTreeValidate(BSTreeData *pData)
{
    bool bLegal = true;

    /* Simulate the stack and apply iterative in order tree traversal. */
    TreeNode **stack = (TreeNode**)malloc(sizeof(TreeNode*) * pData->_uiSize);
    TreeNode *pCurr = pData->_pRoot;
    TreeNode *pPred = NULL;
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
