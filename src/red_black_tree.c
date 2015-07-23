#include "tree/red_black_tree.h"


/*===========================================================================*
 *                Hide the private data of the container                     *
 *===========================================================================*/
typedef struct _RBTreeNode {
    Item item;
    bool bColor;
    struct _RBTreeNode *pParent;
    struct _RBTreeNode *pLeft;
    struct _RBTreeNode *pRight;
} RBTreeNode;

struct _RBTreeData {
    bool bUserDestroy_;
    int32_t iSize_;
    RBTreeNode *pRoot_;
    RBTreeNode *pNull_;
    int32_t (*pCompare_) (Item, Item);
    void (*pDestroy_) (Item);
};

#define DIRECT_LEFT      (0)
#define DIRECT_RIGHT     (1)

#define COLOR_RED        (0)
#define COLOR_BLACK      (1)


/*===========================================================================*
 *                  Definition for internal functions                        *
 *===========================================================================*/
/**
 * @brief Traverse all the nodes and clean the allocated resource.
 *
 * If the custom resource clean method is set, it also runs the clean method for
 * all the items.
 *
 * @param pData         The pointer to the tree private data
 */
void _RBTreeDeinit(RBTreeData *pData);

/**
 * @brief Return the node having the maximal order in the subtree rooted by the
 * designated node.
 *
 * @param pCurr         The pointer to the designated node
 *
 * @return              The pointer to the returned node or NULL
 */
RBTreeNode* _RBTreeMaximal(RBTreeNode *pNull, RBTreeNode *pCurr);

/**
 * @brief Return the node having the minimal order in the subtree rooted by the
 * designated node.
 *
 * @param pCurr         The pointer to the designated node
 *
 * @return              The pointer to the returned node or NULL
 */
RBTreeNode* _RBTreeMinimal(RBTreeNode *pNull, RBTreeNode *pCurr);

/**
 * @brief Return the immediate successor of the designated node.
 *
 * @param pCurr         The pointer to the designated node
 *
 * @return              The pointer to the returned node or NULL
 */
RBTreeNode* _RBTreeSuccessor(RBTreeNode *pNull, RBTreeNode *pCurr);

/**
 * @brief Return the immediate predecessor of the designated node.
 *
 * @param pCurr         The pointer to the designated node
 *
 * @return              The pointer to the returned node or NULL
 */
RBTreeNode* _RBTreePredecessor(RBTreeNode *pNull, RBTreeNode *pCurr);

/**
 * @brief Make right rotation for the subtree rooted by the designated node.
 *
 * After rotation, the designated node will be the right child of its original
 * left child.
 *
 * @param pData         The pointer to the tree private data
 * @param pCurr         The pointer to the designated node
 */
void _RBTreeRightRotate(RBTreeData *pData, RBTreeNode *pCurr);

/**
 * @brief Make left rotation for the subtree rooted by the designated node.
 *
 * After rotation, the designated node will be the left child of its original
 * right child.
 *
 * @param pData         The pointer to the tree private data
 * @param pCurr         The pointer to the designated node
 */
void _RBTreeLeftRotate(RBTreeData *pData, RBTreeNode *pCurr);

/**
 * @brief Maintain the balanced tree property after node insertion.
 *
 * @param pData         The pointer to the tree private data
 * @param pCurr         The pointer to the designated node
 */
void _RBTreeInsertFixup(RBTreeData *pData, RBTreeNode *pCurr);

/**
 * @brief Maintain the balanced tree property after node deletion.
 *
 * @param pData         The pointer to the tree private data
 * @param pCurr         The pointer to the designated node
 */
void _RBTreeDeleteFixup(RBTreeData *pData, RBTreeNode *pCurr);

/**
 * @brief Get the node which stores the item having the same order with the
 * designated one.
 *
 * @param pData         The pointer to tree private data
 * @param item          The pointer to the designated item
 *
 * @return              The pointer to the returned node or NULL
 */
RBTreeNode* _RBTreeSearch(RBTreeData *pData, Item item);

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
int32_t _RBTreeItemCompare(Item itemSrc, Item itemTge);

/**
 * @brief The default item resource clean method.
 *
 * @param item          The designated item
 */
void _RBTreeItemDestroy(Item item);

/**
 * @brief The internal debug function to verify the structure correctness.
 *
 * @param pData         The pointer to the tree private data
 *
 * @return              Whether the tree has legal structure
 */
bool _RBTreeValidate(RBTreeData *pData);


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
int32_t RBTreeInit(RedBlackTree **ppObj)
{
    RedBlackTree *pObj;
    *ppObj = (RedBlackTree*)malloc(sizeof(RedBlackTree));
    if (!(*ppObj))
        return ERR_NOMEM;
    pObj = *ppObj;

    pObj->pData = (RBTreeData*)malloc(sizeof(RBTreeData));
    if (!pObj->pData) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }

    /* Create the dummy node representing as the NULL pointer of the tree. */
    RBTreeData *pData = pObj->pData;
    pData->pNull_ = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    if (!pData->pNull_) {
        free(pObj->pData);
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    pData->pNull_->bColor = COLOR_BLACK;
    pData->pNull_->item = NULL;
    pData->pNull_->pParent = pData->pNull_;
    pData->pNull_->pRight = pData->pNull_;
    pData->pNull_->pLeft = pData->pNull_;

    pObj->pData->bUserDestroy_ = false;
    pObj->pData->iSize_ = 0;
    pObj->pData->pRoot_ = pData->pNull_;
    pObj->pData->pCompare_ = _RBTreeItemCompare;
    pObj->pData->pDestroy_ = _RBTreeItemDestroy;

    pObj->insert = RBTreeInsert;
    pObj->search = RBTreeSearch;
    pObj->delete = RBTreeDelete;
    pObj->maximum = RBTreeMaximum;
    pObj->minimum = RBTreeMinimum;
    pObj->successor = RBTreeSuccessor;
    pObj->predecessor = RBTreePredecessor;
    pObj->size = RBTreeSize;
    pObj->set_compare = RBTreeSetCompare;
    pObj->set_destroy = RBTreeSetDestroy;

    return SUCC;
}

void RBTreeDeinit(RedBlackTree **ppObj)
{
    RedBlackTree *pObj;
    if (*ppObj) {
        pObj = *ppObj;
        if (pObj->pData) {
            if (pObj->pData->pNull_) {
                _RBTreeDeinit(pObj->pData);
                free(pObj->pData->pNull_);
                free(pObj->pData);
            }
        }
        free(*ppObj);
        *ppObj = NULL;
    }
    return;
}

int32_t RBTreeInsert(RedBlackTree *self, Item item)
{
    CHECK_INIT(self);

    int32_t iOrder;
    int8_t cDirect;
    RBTreeNode *pNew, *pCurr, *pParent;
    pNew = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    if (!pNew)
        return ERR_NOMEM;
    RBTreeData *pData = self->pData;
    pNew->item = item;
    pNew->bColor = COLOR_RED;
    pNew->pParent = pData->pNull_;
    pNew->pLeft = pData->pNull_;
    pNew->pRight = pData->pNull_;

    pParent = pData->pNull_;
    pCurr = pData->pRoot_;
    while (pCurr != pData->pNull_) {
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
    if (pParent != pData->pNull_) {
        if (cDirect == DIRECT_LEFT)
            pParent->pLeft = pNew;
        else
            pParent->pRight = pNew;
    } else
        self->pData->pRoot_ = pNew;

    /* Increase the size. */
    self->pData->iSize_++;

    /* Maintain the balanced tree structure. */
    _RBTreeInsertFixup(self->pData, pNew);

    return SUCC;
}

int32_t RBTreeSearch(RedBlackTree *self, Item itemIn, Item *pItemOut)
{
    CHECK_INIT(self);

    RBTreeNode *pFind;
    pFind = _RBTreeSearch(self->pData, itemIn);
    if (pFind != self->pData->pNull_) {
        *pItemOut = pFind->item;
        return SUCC;
    }
    *pItemOut = NULL;
    return ERR_NODATA;
}

int32_t RBTreeDelete(RedBlackTree *self, Item item)
{
    CHECK_INIT(self);

    RBTreeNode *pCurr, *pChild, *pSucc;
    RBTreeData *pData = self->pData;
    RBTreeNode *pNull = pData->pNull_;
    pCurr = _RBTreeSearch(pData, item);
    if (pCurr == pNull)
        return ERR_NODATA;

    bool bColor;
    /* The specified node has no child. */
    if ((pCurr->pLeft == pNull) && (pCurr->pRight == pNull)) {
        if (pCurr->pParent != pNull) {
            if (pCurr == pCurr->pParent->pLeft)
                pCurr->pParent->pLeft = pNull;
            else
                pCurr->pParent->pRight = pNull;
        } else
            pData->pRoot_ = pNull;

        bColor = pCurr->bColor;
        pChild = pNull;
        pChild->pParent = pCurr->pParent;
        if (pData->bUserDestroy_)
            pData->pDestroy_(pCurr->item);
        free(pCurr);
    } else {
        /* The specified node has two children. */
        if ((pCurr->pLeft != pNull) && (pCurr->pRight != pNull)) {
            pSucc = _RBTreeSuccessor(pNull, pCurr);

            pChild = pSucc->pLeft;
            if (pChild == pNull)
                pChild = pSucc->pRight;

            pChild->pParent = pSucc->pParent;

            if (pSucc == pSucc->pParent->pLeft)
                pSucc->pParent->pLeft = pChild;
            else
                pSucc->pParent->pRight = pChild;

            bColor = pSucc->bColor;
            if (pData->bUserDestroy_)
                pData->pDestroy_(pCurr->item);
            pCurr->item = pSucc->item;
            free(pSucc);
        }
        /* The specified node has one child. */
        else {
            pChild = pCurr->pLeft;
            if (pChild == pNull)
                pChild = pCurr->pRight;

            pChild->pParent = pCurr->pParent;

            if (pCurr->pParent != pNull) {
                if (pCurr == pCurr->pParent->pLeft)
                    pCurr->pParent->pLeft = pChild;
                else
                    pCurr->pParent->pRight = pChild;
            } else
                pData->pRoot_ = pChild;

            bColor = pCurr->bColor;
            if (pData->bUserDestroy_)
                pData->pDestroy_(pCurr->item);
            free(pCurr);
        }
    }

    /* Decrease the size. */
    pData->iSize_--;

    /* Maintain the balanced tree structure. */
    if (bColor == COLOR_BLACK)
        _RBTreeDeleteFixup(pData, pChild);

    return SUCC;
}

int32_t RBTreeMaximum(RedBlackTree *self, Item *pItem)
{
    CHECK_INIT(self);
    if (!pItem)
        return ERR_GET;

    RBTreeData *pData = self->pData;
    RBTreeNode *pFind = _RBTreeMaximal(pData->pNull_, pData->pRoot_);
    if (pFind != pData->pNull_) {
        *pItem = pFind->item;
        return SUCC;
    }
    return ERR_IDX;
}

int32_t RBTreeMinimum(RedBlackTree *self, Item *pItem)
{
    CHECK_INIT(self);
    if (!pItem)
        return ERR_GET;

    RBTreeData *pData = self->pData;
    RBTreeNode *pFind = _RBTreeMinimal(pData->pNull_, pData->pRoot_);
    if (pFind != pData->pNull_) {
        *pItem = pFind->item;
        return SUCC;
    }
    return ERR_IDX;
}

int32_t RBTreeSuccessor(RedBlackTree *self, Item itemIn, Item *pItemOut)
{
    CHECK_INIT(self);
    if (!pItemOut)
        return ERR_GET;

    RBTreeNode *pNull = self->pData->pNull_;
    RBTreeNode *pCurr = _RBTreeSearch(self->pData, itemIn);
    if (pCurr == pNull)
        return ERR_NODATA;

    RBTreeNode *pFind = _RBTreeSuccessor(pNull, pCurr);
    if (pFind != pNull) {
        *pItemOut = pFind->item;
        return SUCC;
    }
    return ERR_NODATA;
}

int32_t RBTreePredecessor(RedBlackTree *self, Item itemIn, Item *pItemOut)
{
    CHECK_INIT(self);
    if (!pItemOut)
        return ERR_GET;

    RBTreeNode *pNull = self->pData->pNull_;
    RBTreeNode *pCurr = _RBTreeSearch(self->pData, itemIn);
    if (pCurr == pNull)
        return ERR_NODATA;

    RBTreeNode *pFind = _RBTreePredecessor(pNull, pCurr);
    if (pFind != pNull) {
        *pItemOut = pFind->item;
        return SUCC;
    }
    return ERR_NODATA;
}

int32_t RBTreeSize(RedBlackTree *self)
{
    CHECK_INIT(self);
    return self->pData->iSize_;
}

int32_t RBTreeSetCompare(RedBlackTree *self, int32_t (*pFunc) (Item, Item))
{
    CHECK_INIT(self);
    self->pData->pCompare_ = pFunc;
    return SUCC;
}

int32_t RBTreeSetDestroy(RedBlackTree *self, void (*pFunc) (Item))
{
    CHECK_INIT(self);
    self->pData->bUserDestroy_ = true;
    self->pData->pDestroy_ = pFunc;
    return SUCC;
}


/*===========================================================================*
 *               Implementation for internal functions                       *
 *===========================================================================*/
void _RBTreeDeinit(RBTreeData *pData)
{
    RBTreeNode *pNull = pData->pNull_;
    if (pData->pRoot_ == pNull)
        return;

    /* Simulate the stack and apply iterative post-order tree traversal. */
    RBTreeNode ***stack = (RBTreeNode***)malloc(sizeof(RBTreeNode**) * pData->iSize_);
    assert(stack != NULL);

    int32_t iSize = 0;
    stack[iSize++] = &(pData->pRoot_);
    while (iSize > 0) {
        RBTreeNode **ppCurr = stack[iSize - 1];
        RBTreeNode *pCurr = *ppCurr;
        if (pCurr->pLeft != pNull)
            stack[iSize++] = &(pCurr->pLeft);
        else if (pCurr->pRight != pNull)
            stack[iSize++] = &(pCurr->pRight);
        else {
            if (pData->bUserDestroy_)
                pData->pDestroy_(pCurr->item);
            RBTreeNode *pParent = pCurr->pParent;
            if (pCurr == pParent->pLeft)
                pParent->pLeft = pNull;
            else
                pParent->pRight = pNull;
            free(pCurr);
            iSize--;
        }
    }
    free(stack);

    return;
}

RBTreeNode* _RBTreeMinimal(RBTreeNode *pNull, RBTreeNode *pCurr)
{
    RBTreeNode *pParent = pNull;
    while (pCurr != pNull) {
        pParent = pCurr;
        pCurr = pCurr->pLeft;
    }
    return pParent;
}

RBTreeNode* _RBTreeMaximal(RBTreeNode *pNull, RBTreeNode *pCurr)
{
    RBTreeNode *pParent = pNull;
    while (pCurr != pNull) {
        pParent = pCurr;
        pCurr = pCurr->pRight;
    }
    return pParent;
}

RBTreeNode* _RBTreeSuccessor(RBTreeNode *pNull, RBTreeNode *pCurr)
{
    if (pCurr != pNull) {
        /* Case 1: The minimal node in the non-null right subtree. */
        if (pCurr->pRight != pNull)
            pCurr = _RBTreeMinimal(pNull, pCurr->pRight);

        /* Case 2: The ancestor which considers the designated node as the
           maximal node of its left subtree. */
        else {
            while((pCurr->pParent != pNull) && (pCurr == pCurr->pParent->pRight))
                pCurr = pCurr->pParent;
            pCurr = pCurr->pParent;
        }
    }
    return pCurr;
}

RBTreeNode* _RBTreePredecessor(RBTreeNode *pNull, RBTreeNode *pCurr)
{
    if (pCurr != pNull) {
        /* Case 1: The maximal node in the non-null left subtree. */
        if (pCurr->pLeft != pNull)
            pCurr = _RBTreeMaximal(pNull, pCurr->pLeft);

        /* Case 2: The ancestor which considers the designated node as the
           minimal node of its right subtree. */
        else {
            while((pCurr->pParent != pNull) && (pCurr == pCurr->pParent->pLeft))
                pCurr = pCurr->pParent;
            pCurr = pCurr->pParent;
        }
    }
    return pCurr;
}

void _RBTreeRightRotate(RBTreeData *pData, RBTreeNode *pCurr) {
    RBTreeNode *pNull = pData->pNull_;
    RBTreeNode *pChild = pCurr->pLeft;
    /**
     *  Right rotation for the current node denoted as y
     *     y          x
     *    / \        / \
     *   x   c  =>  a   y
     *  / \            / \
     * a   b          b   c
     */

    /* Let y link b as its left child.
       If b is not dummy node, let b link y as its parent. */
    pCurr->pLeft = pChild->pRight;
    if (pChild->pRight != pNull)
        pChild->pRight->pParent = pCurr;

    /* Let x link y's parent as its parent.
       If y's parent is not dummy node, let it link x as its child. */
    pChild->pParent = pCurr->pParent;
    if (pCurr->pParent != pNull) {
        if (pCurr == pCurr->pParent->pLeft)
            pCurr->pParent->pLeft = pChild;
        else
            pCurr->pParent->pRight = pChild;
    } else
        pData->pRoot_ = pChild;

    /* Let y link x as its parent.
       And let x link y as its right child. */
    pCurr->pParent = pChild;
    pChild->pRight = pCurr;

    return;
}

void _RBTreeLeftRotate(RBTreeData *pData, RBTreeNode *pCurr) {
    RBTreeNode *pNull = pData->pNull_;
    RBTreeNode *pChild = pCurr->pRight;
    /**
     *  Left rotation for the current node denoted as x
     *     x          y
     *    / \        / \
     *   a   y  =>  x   c
     *      / \    / \
     *     b   c  a   b
     */

    /* Let x link b as its right child.
       If b is not dummy node, let b link x as its parent. */
    pCurr->pRight = pChild->pLeft;
    if (pChild->pLeft != pNull)
        pChild->pLeft->pParent = pCurr;

    /* Let y link x's parent as its parent.
       If x's parent is not dummy node, let it link y as its child. */
    pChild->pParent = pCurr->pParent;
    if (pCurr->pParent != pNull) {
        if (pCurr == pCurr->pParent->pLeft)
            pCurr->pParent->pLeft = pChild;
        else
            pCurr->pParent->pRight = pChild;
    } else
        pData->pRoot_ = pChild;

    /* Let x link y as its parent.
       And let y link x as its left child. */
    pCurr->pParent = pChild;
    pChild->pLeft = pCurr;

    return;
}

void _RBTreeInsertFixup(RBTreeData *pData, RBTreeNode *pCurr)
{
    RBTreeNode *pUncle;

    /* Denote the current node as x. */
    while (pCurr->pParent->bColor == COLOR_RED) {
        /* x's parent is its grandparent's left child. */
        if (pCurr->pParent == pCurr->pParent->pParent->pLeft) {
            pUncle = pCurr->pParent->pParent->pRight;
            /**
             * Case 1: The color of x's uncle is also red.
             * Set the colors of x's parent and x's uncle to black.
             * Set the color of x's grandparent to red.
             *
             *       g(B)               g(R)
             *      / \                / \
             *     /   \              /   \
             * (R)p     u(R)      (B)p     u(B)
             *   / \   / \    =>    / \   / \
             *  A   \ D   E        A   \ D   E
             *    (R)x               (R)x
             *      / \                / \
             *     B   C              B   C
             */
            if (pUncle->bColor == COLOR_RED) {
                pCurr->pParent->bColor = COLOR_BLACK;
                pUncle->bColor = COLOR_BLACK;
                pCurr->pParent->pParent->bColor = COLOR_RED;
                pCurr = pCurr->pParent->pParent;
            } else {
                /**
                 * Case 2: The color of x's uncle is black, and x is its parent's
                 *         right child.
                 * Make a left rotation for x's parent.
                 *
                 *        g(B)                g(B)
                 *       / \                 / \
                 *      /   \               /   \
                 *  (R)p     u(B)       (R)x     u(B)
                 *    / \   / \    =>     / \   / \
                 *   A   \ D   E         /   C D   E
                 *     (R)x          (R)p
                 *       / \           / \
                 *      B   C         A   B
                 */
                if (pCurr == pCurr->pParent->pRight) {
                    pCurr = pCurr->pParent;
                    _RBTreeLeftRotate(pData, pCurr);
                }
                /**
                 * Case 3: The color of x's uncle is black, and x is its parent's
                 *         left child.
                 * Set the color of x's parent to black.
                 * Set the color of x's grandparent to red.
                 * Make a right rotation for x's grandparent.
                 *
                 *          g(B)               p(B)
                 *         / \                / \
                 *        /   \              /   \
                 *    (R)p     u(B)      (R)x     g(R)
                 *      / \   / \          / \   / \
                 *     /   C D   E   =>   A   B /   \
                 * (R)x                        C     u(B)
                 *   / \                            / \
                 *  A   B                          D   E
                 */
                pCurr->pParent->bColor = COLOR_BLACK;
                pCurr->pParent->pParent->bColor = COLOR_RED;
                _RBTreeRightRotate(pData, pCurr->pParent->pParent);
            }
        }
        /* x's parent is its grandparent's right child. */
        else {
            pUncle = pCurr->pParent->pParent->pLeft;

            /* Case 1: The color of x's uncle is also red. */
            if (pUncle->bColor == COLOR_RED) {
                pCurr->pParent->bColor = COLOR_BLACK;
                pUncle->bColor = COLOR_BLACK;
                pCurr->pParent->pParent->bColor = COLOR_RED;
                pCurr = pCurr->pParent->pParent;
            } else {
                /* Case 2: The color of x's uncle is black, and x is its parent's
                   left child. */
                if (pCurr == pCurr->pParent->pLeft) {
                    pCurr = pCurr->pParent;
                    _RBTreeRightRotate(pData, pCurr);
                }
                /* Case 3: The color of x's uncle is black, and x is its parent's
                   right child. */
                pCurr->pParent->bColor = COLOR_BLACK;
                pCurr->pParent->pParent->bColor = COLOR_RED;
                _RBTreeLeftRotate(pData, pCurr->pParent->pParent);
            }
        }
    }

    pData->pRoot_->bColor = COLOR_BLACK;
    return;
}

void _RBTreeDeleteFixup(RBTreeData *pData, RBTreeNode *pCurr)
{
    RBTreeNode *pBrother;

    /* Denote the current node as x. */
    while ((pCurr != pData->pRoot_) && (pCurr->bColor == COLOR_BLACK)) {
        /* x is its parent's left child. */
        if (pCurr == pCurr->pParent->pLeft) {
            pBrother = pCurr->pParent->pRight;
            /**
             * Case 1: The color of x's brother is red.
             * Set the color of x's brother to black.
             * Set the color of x's parent to red.
             * Make a left rotation for x's parent.
             *
             *        p(B)                b(B)
             *      /   \               /   \
             *     x     b(R)  =>   (R)p     D
             *    / \   / \           / \
             *   A   B C   D         x   C
             *                      / \
             *                     A  B
             */
            if (pBrother->bColor == COLOR_RED) {
                pBrother->bColor = COLOR_BLACK;
                pCurr->pParent->bColor = COLOR_RED;
                _RBTreeLeftRotate(pData, pCurr->pParent);
                pBrother = pCurr->pParent->pRight;
            }
            /**
             * Case 2: The color of x's brother is black, and both of its
             *         children are also black.
             * Set the color of x's brother to red.
             *
             *         p                  p
             *       /   \              /   \
             *      /     \            /     \
             *     x       b(B)  =>   x       b(R)
             *    / \     / \        / \     / \
             *   A   B (B)y z(B)    A   B (B)y z(B)
             */
            if ((pBrother->pLeft->bColor == COLOR_BLACK) &&
                (pBrother->pRight->bColor == COLOR_BLACK)) {
                pBrother->bColor = COLOR_RED;
                pCurr = pCurr->pParent;
            } else {
                /**
                 * Case 3: The color of x's brother is black, and the colors of
                 *         its left and right child are red and black respectively.
                 * Set the color of x's brother to red.
                 * Set the color of that brother's left child to black.
                 * Make a right rotation for x's brother.
                 *
                 *         p                     p
                 *       /   \                 /   \
                 *      /     \               /     \
                 *     x       b(B)          x       y(B)
                 *    / \     / \     =>    / \     / \
                 *   A   B (R)y z(B)       A   B   C   b(R)
                 *           / \                      / \
                 *          C   D                    D   z(B)
                 */
                if (pBrother->pRight->bColor == COLOR_BLACK) {
                    pBrother->pLeft->bColor = COLOR_BLACK;
                    pBrother->bColor = COLOR_RED;
                    _RBTreeRightRotate(pData, pBrother);
                    pBrother = pCurr->pParent->pRight;
                }
                /**
                 * Case 4: The color of x's brother is black, and its right child
                 *         is red.
                 * Set the color of x's brother to the one of x's parent.
                 * Set the color of x's parent to black.
                 * Set the color of that brother's right child to black.
                 * Make a left rotation for x's parent.
                 *
                 *       p(C')              b(C')
                 *      /   \              /   \
                 *     /     \            /     \
                 *    x      b(B)   =>   p(B)   z(B)
                 *   / \    / \         / \
                 *  A   B  y   z(R)    x   y
                 *                    / \
                 *                   A   B
                 */
                pBrother->bColor = pCurr->pParent->bColor;
                pCurr->pParent->bColor = COLOR_BLACK;
                pBrother->pRight->bColor = COLOR_BLACK;
                _RBTreeLeftRotate(pData, pCurr->pParent);
                pCurr = pData->pRoot_;
            }
        }
        /* x is its parent's right child */
        else {
            pBrother = pCurr->pParent->pLeft;
            /* Case 1: The color of x's brother is red. */
            if (pBrother->bColor == COLOR_RED) {
                pBrother->bColor = COLOR_BLACK;
                pCurr->pParent->bColor = COLOR_RED;
                _RBTreeRightRotate(pData, pCurr->pParent);
                pBrother = pCurr->pParent->pLeft;
            }
            /* Case 2: The color of x's brother is black, and both of its
               children are also black. */
            if ((pBrother->pLeft->bColor == COLOR_BLACK) &&
                (pBrother->pRight->bColor == COLOR_BLACK)) {
                pBrother->bColor = COLOR_RED;
                pCurr = pCurr->pParent;
            } else {
                /* Case 3: The color of x's brother is black and the colors of its
                   right and left child are red and black respectively. */
                if (pBrother->pLeft->bColor == COLOR_BLACK) {
                    pBrother->pRight->bColor = COLOR_BLACK;
                    pBrother->bColor = COLOR_RED;
                    _RBTreeLeftRotate(pData, pBrother);
                    pBrother = pCurr->pParent->pLeft;
                }
                /* Case 4: The color of x's brother is black, and its left child
                   is red. */
                pBrother->bColor = pCurr->pParent->bColor;
                pCurr->pParent->bColor = COLOR_BLACK;
                pBrother->pLeft->bColor = COLOR_BLACK;
                _RBTreeRightRotate(pData, pCurr->pParent);
                pCurr = pData->pRoot_;
            }
        }
    }

    pCurr->bColor = COLOR_BLACK;
    return;
}

RBTreeNode* _RBTreeSearch(RBTreeData *pData, Item item)
{
    int32_t iOrder;
    RBTreeNode *pCurr = pData->pRoot_;
    while(pCurr != pData->pNull_) {
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

int32_t _RBTreeItemCompare(Item itemSrc, Item itemTge)
{
    if (itemSrc == itemTge)
        return 0;
    return (itemSrc > itemTge)? 1 : -1;
}

void _RBTreeItemDestroy(Item item) {}

bool _RBTreeValidate(RBTreeData *pData)
{
    RBTreeNode *pNull = pData->pNull_;
    bool bLegal = true;

    /* Simulate the stack and apply iterative in-order tree traversal. */
    RBTreeNode **stack = (RBTreeNode**)malloc(sizeof(RBTreeNode*) * pData->iSize_);
    RBTreeNode *pCurr = pData->pRoot_;
    RBTreeNode *pPred = pNull;
    int32_t iSize = 0;

    while ((pCurr != pNull) || (iSize > 0)) {
        if (pCurr != pNull) {
            stack[iSize++] = pCurr;
            pCurr = pCurr->pLeft;
        } else {
            if (pPred != pNull) {
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
