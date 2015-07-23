#include "tree/binary_tree.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
typedef struct _BinTreeNode {
    Item item;
    struct _BinTreeNode *pParent;
    struct _BinTreeNode *pLeft;
    struct _BinTreeNode *pRight;
} BinTreeNode;

struct _BinTreeData {
    bool bUserDestroy_;
    int32_t iSize_;
    BinTreeNode *pRoot_;
    int32_t (*pCompare_) (Item, Item);
    void (*pDestroy_) (Item);
};

#define DIRECT_LEFT      (0)
#define DIRECT_RIGHT     (1)


/*===========================================================================*
 *                  Definition for internal functions                        *
 *===========================================================================*/
/**
 * @brief Delete all the tree nodes.
 *
 * If the custom resource clean method is set, it also runs the clean method for
 * all the items.
 *
 * @param pData         The pointer to the tree private data
 */
void _BinTreeDeinit(BinTreeData *pData);

/**
 * @brief Return the node having the maximal order in the subtree rooted by the
 * designated node.
 *
 * @param pCurr         The pointer to the designated node
 *
 * @return              The pointer to the returned node or NULL
 */
BinTreeNode* _BinTreeMaximal(BinTreeNode *pCurr);

/**
 * @brief Return the node having the minimal order in the subtree rooted by the
 * designated node.
 *
 * @param pCurr         The pointer to the designated node
 *
 * @return              The pointer to the returned node or NULL
 */
BinTreeNode* _BinTreeMinimal(BinTreeNode *pCurr);

/**
 * @brief Return the immediate successor of the designated node.
 *
 * @param pCurr         The pointer to the designated node
 *
 * @return              The pointer to the returned node or NULL
 */
BinTreeNode* _BinTreeSuccessor(BinTreeNode *pCurr);

/**
 * @brief Return the immediate predecessor of the designated node.
 *
 * @param pCurr         The pointer to the designated node
 *
 * @return              The pointer to the returned node or NULL
 */
BinTreeNode* _BinTreePredecessor(BinTreeNode *pCurr);

/**
 * @brief Get the node which stores the item having the same order with the
 * designated one.
 *
 * @param pData         The pointer to tree private data
 * @param item          The pointer to the designated item
 *
 * @return              The pointer to the returned node or NULL
 */
BinTreeNode* _BinTreeSearch(BinTreeData *pData, Item item);

/**
 * @brief The default order comparison method for a pair of items.
 *
 * @param itemSrc       The source item
 * @param itemTge       The target item
 *
 * @retval 1            The source item has the larger order
 * @retval 0            Both the items have the same order
 * @retval -1           The source item has the smaller order
 */
int32_t _BinTreeItemCompare(Item itemSrc, Item itemTge);

/**
 * @brief The default item resource clean method.
 *
 * @param item         The designated item
 */
void _BinTreeItemDestroy(Item item);

/**
 * @brief The internal debug function to verify the structure correctness.
 *
 * @param pData         The pointer to the tree private data
 *
 * @return              Whether the tree has legal structure
 */
bool _BinTreeValidate(BinTreeData *pData);


#define CHECK_INIT(self)                                                        \
            do {                                                                \
                if (!self)                                                      \
                    return ERR_NOINIT;                                          \
                if (!(self->pData))                                             \
                    return ERR_NOINIT;                                          \
            } while (0);


/*===========================================================================*
 *           Implementation for the exported member operations               *
 *===========================================================================*/
int32_t BinTreeInit(BinaryTree **ppObj)
{
    BinaryTree *pObj;
    *ppObj = (BinaryTree*)malloc(sizeof(BinaryTree));
    if (!(*ppObj))
        return ERR_NOMEM;
    pObj = *ppObj;

    pObj->pData = (BinTreeData*)malloc(sizeof(BinTreeData));
    if (!pObj->pData) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }

    pObj->pData->bUserDestroy_ = false;
    pObj->pData->iSize_ = 0;
    pObj->pData->pRoot_ = NULL;
    pObj->pData->pCompare_ = _BinTreeItemCompare;
    pObj->pData->pDestroy_ = _BinTreeItemDestroy;

    pObj->insert = BinTreeInsert;
    pObj->search = BinTreeSearch;
    pObj->delete = BinTreeDelete;
    pObj->maximum = BinTreeMaximum;
    pObj->minimum = BinTreeMinimum;
    pObj->successor = BinTreeSuccessor;
    pObj->predecessor = BinTreePredecessor;
    pObj->size = BinTreeSize;
    pObj->set_compare = BinTreeSetCompare;
    pObj->set_destroy = BinTreeSetDestroy;

    return SUCC;
}

void BinTreeDeinit(BinaryTree **ppObj)
{
    BinaryTree *pObj;
    if (*ppObj) {
        pObj = *ppObj;
        if (pObj->pData) {
            _BinTreeDeinit(pObj->pData);
            free(pObj->pData);
        }
        free(*ppObj);
        *ppObj = NULL;
    }
    return;
}

int32_t BinTreeInsert(BinaryTree *self, Item item)
{
    CHECK_INIT(self);

    int32_t iOrder;
    int8_t cDirect;
    BinTreeNode *pNew, *pCurr, *pParent;
    pNew = (BinTreeNode*)malloc(sizeof(BinTreeNode));
    if (!pNew)
        return ERR_NOMEM;
    pNew->item = item;
    pNew->pParent = NULL;
    pNew->pLeft = NULL;
    pNew->pRight = NULL;

    BinTreeData *pData = self->pData;
    pParent = NULL;
    pCurr = pData->pRoot_;
    while (pCurr) {
        pParent = pCurr;
        iOrder = pData->pCompare_(item, pCurr->item);
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
            if (pData->bUserDestroy_)
                pData->pDestroy_(pCurr->item);
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
        pData->pRoot_ = pNew;

    /* Increase the size. */
    pData->iSize_++;

    return SUCC;
}

int32_t BinTreeSearch(BinaryTree *self, Item itemIn, Item *pItemOut)
{
    CHECK_INIT(self);
    if (!pItemOut)
        return ERR_GET;

    BinTreeNode *pFind;
    pFind = _BinTreeSearch(self->pData, itemIn);
    if (pFind) {
        *pItemOut = pFind->item;
        return SUCC;
    }
    *pItemOut = NULL;
    return ERR_NODATA;
}

int32_t BinTreeDelete(BinaryTree *self, Item item)
{
    CHECK_INIT(self);

    BinTreeNode *pCurr, *pChild, *pSucc, *pParent;
    pCurr = _BinTreeSearch(self->pData, item);
    if (!pCurr)
        return ERR_NODATA;

    BinTreeData *pData = self->pData;

    /* The target node has no child. */
    if ((!pCurr->pLeft) && (!pCurr->pRight)) {
        if (pCurr->pParent) {
            if (pCurr == pCurr->pParent->pLeft)
                pCurr->pParent->pLeft = NULL;
            else
                pCurr->pParent->pRight = NULL;
        } else
            pData->pRoot_ = NULL;

        if (pData->bUserDestroy_)
            pData->pDestroy_(pCurr->item);
        free(pCurr);
    } else {
        /* The target node has two children. */
        if ((pCurr->pLeft) && (pCurr->pRight)) {
            pSucc = _BinTreeSuccessor(pCurr);

            pChild = pSucc->pLeft;
            if (!pChild)
                pChild = pSucc->pRight;

            if (pChild)
                pChild->pParent = pSucc->pParent;

            if (pSucc == pSucc->pParent->pLeft)
                pSucc->pParent->pLeft = pChild;
            else
                pSucc->pParent->pRight = pChild;

            if (pData->bUserDestroy_)
                pData->pDestroy_(pCurr->item);
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
                pData->pRoot_ = pChild;

            if (pData->bUserDestroy_)
                pData->pDestroy_(pCurr->item);
            free(pCurr);
        }
    }

    /* Decrease the size. */
    pData->iSize_--;

    return SUCC;
}

int32_t BinTreeMaximum(BinaryTree *self, Item *pItem)
{
    CHECK_INIT(self);
    if (!pItem)
        return ERR_GET;

    BinTreeNode *pFind = _BinTreeMaximal(self->pData->pRoot_);
    if (pFind) {
        *pItem = pFind->item;
        return SUCC;
    }

    *pItem = NULL;
    return ERR_IDX;
}

int32_t BinTreeMinimum(BinaryTree *self, Item *pItem)
{
    CHECK_INIT(self);
    if (!pItem)
        return ERR_GET;

    BinTreeNode *pFind = _BinTreeMinimal(self->pData->pRoot_);
    if (pFind) {
        *pItem = pFind->item;
        return SUCC;
    }

    *pItem = NULL;
    return ERR_IDX;
}

int32_t BinTreeSuccessor(BinaryTree *self, Item itemIn, Item *pItemOut)
{
    CHECK_INIT(self);
    if (!pItemOut)
        return ERR_GET;

    BinTreeNode *pCurr = _BinTreeSearch(self->pData, itemIn);
    if (!pCurr)
        return ERR_NODATA;

    BinTreeNode *pFind = _BinTreeSuccessor(pCurr);
    if (pFind) {
        *pItemOut = pFind->item;
        return SUCC;
    }

    *pItemOut = NULL;
    return ERR_NODATA;
}

int32_t BinTreePredecessor(BinaryTree *self, Item itemIn, Item *pItemOut)
{
    CHECK_INIT(self);
    if (!pItemOut)
        return ERR_GET;

    BinTreeNode *pCurr = _BinTreeSearch(self->pData, itemIn);
    if (!pCurr)
        return ERR_NODATA;

    BinTreeNode *pFind = _BinTreePredecessor(pCurr);
    if (pFind) {
        *pItemOut = pFind->item;
        return SUCC;
    }

    *pItemOut = NULL;
    return ERR_NODATA;
}

int32_t BinTreeSize(BinaryTree *self)
{
    CHECK_INIT(self);
    return self->pData->iSize_;
}

int32_t BinTreeSetCompare(BinaryTree *self, int32_t (*pFunc) (Item, Item))
{
    CHECK_INIT(self);
    self->pData->pCompare_ = pFunc;
    return SUCC;
}

int32_t BinTreeSetDestroy(BinaryTree *self, void (*pFunc) (Item))
{
    CHECK_INIT(self);
    self->pData->bUserDestroy_ = true;
    self->pData->pDestroy_ = pFunc;
    return SUCC;
}


/*===========================================================================*
 *               Implementation for internal functions                       *
 *===========================================================================*/
void _BinTreeDeinit(BinTreeData *pData)
{
    if (!(pData->pRoot_))
        return;

    /* Simulate the stack and apply iterative post-order tree traversal. */
    BinTreeNode ***stack = (BinTreeNode***)malloc(sizeof(BinTreeNode**) * pData->iSize_);
    assert(stack != NULL);

    int32_t iSize = 0;
    stack[iSize++] = &(pData->pRoot_);
    while (iSize > 0) {
        BinTreeNode **ppCurr = stack[iSize - 1];
        BinTreeNode *pCurr = *ppCurr;
        if (pCurr->pLeft)
            stack[iSize++] = &(pCurr->pLeft);
        else if (pCurr->pRight)
            stack[iSize++] = &(pCurr->pRight);
        else {
            if (pData->bUserDestroy_)
                pData->pDestroy_(pCurr->item);
            free(pCurr);
            *ppCurr = NULL;
            iSize--;
        }
    }
    free(stack);

    return;
}

BinTreeNode* _BinTreeMinimal(BinTreeNode *pCurr)
{
    BinTreeNode *pParent = NULL;
    while (pCurr) {
        pParent = pCurr;
        pCurr = pCurr->pLeft;
    }
    return pParent;
}

BinTreeNode* _BinTreeMaximal(BinTreeNode *pCurr)
{
    BinTreeNode *pParent = NULL;
    while (pCurr) {
        pParent = pCurr;
        pCurr = pCurr->pRight;
    }
    return pParent;
}

BinTreeNode* _BinTreeSuccessor(BinTreeNode *pCurr)
{
    if (pCurr) {
        /* Case 1: The minimal node in the non-null right subtree. */
        if (pCurr->pRight)
            pCurr = _BinTreeMinimal(pCurr->pRight);

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

BinTreeNode* _BinTreePredecessor(BinTreeNode *pCurr)
{
    if (pCurr) {
        /* Case 1: The maximal node in the non-null left subtree. */
        if (pCurr->pLeft)
            pCurr = _BinTreeMaximal(pCurr->pLeft);

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

BinTreeNode* _BinTreeSearch(BinTreeData *pData, Item item)
{
    int32_t iOrder;
    BinTreeNode *pCurr = pData->pRoot_;
    while(pCurr) {
        iOrder = pData->pCompare_(item, pCurr->item);
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

int32_t _BinTreeItemCompare(Item itemSrc, Item itemTge)
{
    if (itemSrc == itemTge)
        return 0;
    return (itemSrc > itemTge)? 1 : -1;
}

void _BinTreeItemDestroy(Item item) {}

bool _BinTreeValidate(BinTreeData *pData)
{
    bool bLegal = true;

    /* Simulate the stack and apply iterative in-order tree traversal. */
    BinTreeNode **stack = (BinTreeNode**)malloc(sizeof(BinTreeNode*) * pData->iSize_);
    BinTreeNode *pCurr = pData->pRoot_;
    BinTreeNode *pPred = NULL;
    int32_t iSize = 0;

    while (pCurr || (iSize > 0)) {
        if (pCurr) {
            stack[iSize++] = pCurr;
            pCurr = pCurr->pLeft;
        } else {
            if (pPred) {
                pCurr = stack[iSize - 1];
                int32_t iOrder = pData->pCompare_(pPred->item, pCurr->item);
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
