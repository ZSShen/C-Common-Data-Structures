#include "container/tree_map.h"


/*===========================================================================*
 *                  Hide the private data of the map                         *
 *===========================================================================*/
typedef struct _TreeNode {
    bool bColor;
    Pair *pPair;
    struct _TreeNode *pParent;
    struct _TreeNode *pLeft;
    struct _TreeNode *pRight;
} TreeNode;

struct _TreeMapData {
    bool bEnd_;
    int32_t iSize_;
    int32_t iTop_;
    TreeNode *pRoot_;
    TreeNode *pNull_;
    TreeNode *pIter_;
    TreeNode **pStack_;
    int32_t (*pCompare_) (Key, Key);
    void (*pDestroy_) (Pair*);
};

#define DIRECT_LEFT      (0)
#define DIRECT_RIGHT     (1)

#define COLOR_RED        (0)
#define COLOR_BLACK      (1)


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
/**
 * @brief Traverse all the tree nodes and clean the allocated resource.
 *
 * If the custom resource clean method is set, it also runs the clean method for
 * the pair stored in each node.
 *
 * @param pData         The pointer to the map private data
 */
void _TreeMapDeinit(TreeMapData *pData);

/**
 * @brief Return the node having the maximal order in the subtree rooted by the
 * designated node. The node order is determined by its stored key.
 *
 * @param pCurr         The pointer to the designated node
 *
 * @return              The pointer to the returned node or NULL
 */
TreeNode* _TreeMapMaximal(TreeNode *pNull, TreeNode *pCurr);

/**
 * @brief Return the node having the minimal order in the subtree rooted by the
 * designated node. The node order is determined by its stored key.
 *
 * @param pCurr         The pointer to the designated node
 *
 * @return              The pointer to the returned node or NULL
 */
TreeNode* _TreeMapMinimal(TreeNode *pNull, TreeNode *pCurr);

/**
 * @brief Return the immediate successor of the designated node.
 *
 * @param pCurr         The pointer to the designated node
 *
 * @return              The pointer to the returned node or NULL
 */
TreeNode* _TreeMapSuccessor(TreeNode *pNull, TreeNode *pCurr);

/**
 * @brief Return the immediate predecessor of the designated node.
 *
 * @param pCurr         The pointer to the designated node
 *
 * @return              The pointer to the returned node or NULL
 */
TreeNode* _TreeMapPredecessor(TreeNode *pNull, TreeNode *pCurr);

/**
 * @brief Make right rotation for the subtree rooted by the designated node.
 *
 * After rotation, the designated node will be the right child of its original
 * left child.
 *
 * @param pData         The pointer to the tree private data
 * @param pCurr         The pointer to the designated node
 */
void _TreeMapRightRotate(TreeMapData *pData, TreeNode *pCurr);

/**
 * @brief Make left rotation for the subtree rooted by the designated node.
 *
 * After rotation, the designated node will be the left child of its original
 * right child.
 *
 * @param pData         The pointer to the tree private data
 * @param pCurr         The pointer to the designated node
 */
void _TreeMapLeftRotate(TreeMapData *pData, TreeNode *pCurr);

/**
 * @brief Maintain the red black tree property after node insertion.
 *
 * @param pData         The pointer to the tree private data
 * @param pCurr         The pointer to the designated node
 */
void _TreeMapInsertFixup(TreeMapData *pData, TreeNode *pCurr);

/**
 * @brief Maintain the red black tree property after node deletion.
 *
 * @param pData         The pointer to the tree private data
 * @param pCurr         The pointer to the designated node
 */
void _TreeMapDeleteFixup(TreeMapData *pData, TreeNode *pCurr);

/**
 * @brief Get the node which stores the key having the same order with the
 * designated one.
 *
 * @param pData         The pointer to tree private data
 * @param key           The designated key
 *
 * @return              The pointer to the returned node or NULL
 */
TreeNode* _TreeMapSearch(TreeMapData *pData, Key key);

/**
 * @brief The default key comparison method.
 *
 * @param keySrc         The source key
 * @param keyTge         The target key
 *
 * @retval 1             The source key has the larger order
 * @retval 0             Both the keys have the same order
 * @retval -1            The source key has the smaller order
 */
int32_t _TreeMapCompare(Key keySrc, Key keyTge);

/**
 * @brief The default resource clean method for a key value pair.
 *
 * @param pPair          The pointer to the designated pair
 */
void _TreeMapDestroy(Pair *pPair);


#define CHECK_INIT(self)                                                        \
            do {                                                                \
                if (!self)                                                      \
                    return ERR_NOINIT;                                          \
                if (!(self->pData))                                             \
                    return ERR_NOINIT;                                          \
                if (!(self->pData->pNull_))                                     \
                    return ERR_NOINIT;                                          \
            } while (0);


/*===========================================================================*
 *               Implementation for the exported operations                  *
 *===========================================================================*/
int32_t TreeMapInit(TreeMap **ppObj)
{
    *ppObj = (TreeMap*)malloc(sizeof(TreeMap));
    if (!(*ppObj))
        return ERR_NOMEM;
    TreeMap *pObj = *ppObj;

    pObj->pData = (TreeMapData*)malloc(sizeof(TreeMapData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    TreeMapData *pData = pObj->pData;

    /* Create the dummy node representing the NULL pointer of the tree. */
    pData->pNull_ = (TreeNode*)malloc(sizeof(TreeNode));
    if (!(pData->pNull_)) {
        free(pObj->pData);
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    pData->pNull_->bColor = COLOR_BLACK;
    pData->pNull_->pPair = NULL;
    pData->pNull_->pParent = pData->pNull_;
    pData->pNull_->pRight = pData->pNull_;
    pData->pNull_->pLeft = pData->pNull_;

    pObj->pData->iSize_ = 0;
    pObj->pData->pRoot_ = pData->pNull_;
    pObj->pData->pCompare_ = _TreeMapCompare;
    pObj->pData->pDestroy_ = _TreeMapDestroy;
    pObj->pData->pStack_ = NULL;

    pObj->put = TreeMapPut;
    pObj->get = TreeMapGet;
    pObj->find = TreeMapFind;
    pObj->remove = TreeMapRemove;
    pObj->size = TreeMapSize;
    pObj->minimum = TreeMapMinimum;
    pObj->maximum = TreeMapMaximum;
    pObj->predecessor = TreeMapPredecessor;
    pObj->successor = TreeMapSuccessor;
    pObj->iterate = TreeMapIterate;
    pObj->reverse_iterate = TreeMapReverseIterate;
    pObj->set_compare = TreeMapSetCompare;
    pObj->set_destroy = TreeMapSetDestroy;

    return SUCC;
}

void TreeMapDeinit(TreeMap **ppObj)
{
    if (!(*ppObj))
        goto EXIT;

    TreeMap *pObj = *ppObj;
    if (!(pObj->pData))
        goto FREE_MAP;

    TreeMapData *pData = pObj->pData;
    if (!(pData->pNull_))
        goto FREE_DATA;

    _TreeMapDeinit(pData);
    free(pData->pNull_);
    if (pData->pStack_)
        free(pData->pStack_);

FREE_DATA:
    free(pObj->pData);
FREE_MAP:
    free(*ppObj);
    *ppObj = NULL;
EXIT:
    return;
}

int32_t TreeMapPut(TreeMap *self, Pair *pPair)
{
    CHECK_INIT(self);

    bool bDirect;
    int32_t iOrder;
    TreeNode *pNew, *pCurr, *pParent;
    pNew = (TreeNode*)malloc(sizeof(TreeNode));
    if (!pNew)
        return ERR_NOMEM;
    TreeMapData *pData = self->pData;
    pNew->pPair = pPair;
    pNew->bColor = COLOR_RED;
    pNew->pParent = pData->pNull_;
    pNew->pLeft = pData->pNull_;
    pNew->pRight = pData->pNull_;

    pParent = pData->pNull_;
    pCurr = pData->pRoot_;
    while (pCurr != pData->pNull_) {
        pParent = pCurr;
        iOrder = pData->pCompare_(pPair->key, pCurr->pPair->key);
        if (iOrder > 0) {
            pCurr = pCurr->pRight;
            bDirect = DIRECT_RIGHT;
        }
        else if (iOrder < 0) {
            pCurr = pCurr->pLeft;
            bDirect = DIRECT_LEFT;
        }
        else {
            /* Conflict with the already stored key value pair. */
            free(pNew);
            pData->pDestroy_(pCurr->pPair);
            pCurr->pPair = pPair;
            return SUCC;
        }
    }

    /* Arrive at the proper position. */
    pNew->pParent = pParent;
    if (pParent != pData->pNull_) {
        if (bDirect == DIRECT_LEFT)
            pParent->pLeft = pNew;
        else
            pParent->pRight = pNew;
    } else
        self->pData->pRoot_ = pNew;

    /* Increase the size. */
    pData->iSize_++;

    /* Maintain the red black tree structure. */
    _TreeMapInsertFixup(self->pData, pNew);

    return SUCC;
}

int32_t TreeMapGet(TreeMap *self, Key key, Value *pValue)
{
    CHECK_INIT(self);

    if (!pValue)
        return ERR_GET;
    *pValue = NULL;

    TreeNode *pFind;
    pFind = _TreeMapSearch(self->pData, key);
    if (pFind != self->pData->pNull_) {
        *pValue = pFind->pPair->value;
        return SUCC;
    }

    *pValue = NULL;
    return ERR_NODATA;
}

int32_t TreeMapFind(TreeMap *self, Key key)
{
    CHECK_INIT(self);
    TreeNode *pFind;
    pFind = _TreeMapSearch(self->pData, key);
    return (pFind != self->pData->pNull_)? SUCC : NOKEY;
}

int32_t TreeMapRemove(TreeMap *self, Key key)
{
    CHECK_INIT(self);

    TreeNode *pCurr, *pChild, *pSucc;
    TreeMapData *pData = self->pData;
    TreeNode *pNull = pData->pNull_;
    pCurr = _TreeMapSearch(pData, key);
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
        pData->pDestroy_(pCurr->pPair);
        free(pCurr);
    } else {
        /* The specified node has two children. */
        if ((pCurr->pLeft != pNull) && (pCurr->pRight != pNull)) {
            pSucc = _TreeMapSuccessor(pNull, pCurr);

            pChild = pSucc->pLeft;
            if (pChild == pNull)
                pChild = pSucc->pRight;

            pChild->pParent = pSucc->pParent;

            if (pSucc == pSucc->pParent->pLeft)
                pSucc->pParent->pLeft = pChild;
            else
                pSucc->pParent->pRight = pChild;

            bColor = pSucc->bColor;
            pData->pDestroy_(pCurr->pPair);
            pCurr->pPair = pSucc->pPair;
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
            pData->pDestroy_(pCurr->pPair);
            free(pCurr);
        }
    }

    /* Decrease the size. */
    pData->iSize_--;

    /* Maintain the balanced tree structure. */
    if (bColor == COLOR_BLACK)
        _TreeMapDeleteFixup(pData, pChild);

    return SUCC;
}

int32_t TreeMapSize(TreeMap *self)
{
    CHECK_INIT(self);
    return self->pData->iSize_;
}

int32_t TreeMapMinimum(TreeMap *self, Pair **ppPair)
{
    CHECK_INIT(self);
    if (!ppPair)
        return ERR_GET;

    TreeMapData *pData = self->pData;
    TreeNode *pFind = _TreeMapMinimal(pData->pNull_, pData->pRoot_);
    if (pFind != pData->pNull_) {
        *ppPair = pFind->pPair;
        return SUCC;
    }

    *ppPair = NULL;
    return ERR_IDX;
}

int32_t TreeMapMaximum(TreeMap *self, Pair **ppPair)
{
    CHECK_INIT(self);
    if (!ppPair)
        return ERR_GET;

    TreeMapData *pData = self->pData;
    TreeNode *pFind = _TreeMapMaximal(pData->pNull_, pData->pRoot_);
    if (pFind != pData->pNull_) {
        *ppPair = pFind->pPair;
        return SUCC;
    }

    *ppPair = NULL;
    return ERR_IDX;
}

int32_t TreeMapPredecessor(TreeMap *self, Key key, Pair **ppPair)
{
    CHECK_INIT(self);
    if (!ppPair)
        return ERR_GET;

    TreeNode *pNull = self->pData->pNull_;
    TreeNode *pCurr = _TreeMapSearch(self->pData, key);
    if (pCurr == pNull)
        return ERR_NODATA;

    TreeNode *pFind = _TreeMapPredecessor(pNull, pCurr);
    if (pFind != pNull) {
        *ppPair = pFind->pPair;
        return SUCC;
    }

    *ppPair = NULL;
    return ERR_NODATA;
}

int32_t TreeMapSuccessor(TreeMap *self, Key key, Pair **ppPair)
{
    CHECK_INIT(self);
    if (!ppPair)
        return ERR_GET;

    TreeNode *pNull = self->pData->pNull_;
    TreeNode *pCurr = _TreeMapSearch(self->pData, key);
    if (pCurr == pNull)
        return ERR_NODATA;

    TreeNode *pFind = _TreeMapSuccessor(pNull, pCurr);
    if (pFind != pNull) {
        *ppPair = pFind->pPair;
        return SUCC;
    }

    *ppPair = NULL;
    return ERR_NODATA;
}

int32_t TreeMapIterate(TreeMap *self, bool bReset, Pair **ppPair)
{
    CHECK_INIT(self);

    TreeMapData *pData = self->pData;
    if (bReset) {
        if (pData->pStack_)
            free(pData->pStack_);
        pData->pStack_ = (TreeNode**)malloc(sizeof(TreeNode*) * pData->iSize_);
        if (!(pData->pStack_)) {
            *ppPair = NULL;
            return ERR_NOMEM;
        }
        pData->pIter_ = pData->pRoot_;
        pData->iTop_ = 0;
        pData->bEnd_ = false;
        return SUCC;
    }

    if (!ppPair)
        return ERR_GET;

    while (!(pData->bEnd_)) {
        if (pData->pIter_ != pData->pNull_) {
            if (pData->iTop_ == pData->iSize_) {
                *ppPair = NULL;
                return ERR_ITER;
            }
            pData->pStack_[pData->iTop_] = pData->pIter_;
            pData->iTop_++;
            pData->pIter_ = pData->pIter_->pLeft;
        } else {
            if (pData->iTop_ != 0) {
                pData->iTop_--;
                pData->pIter_ = pData->pStack_[pData->iTop_];
                *ppPair = pData->pIter_->pPair;
                pData->pIter_ = pData->pIter_->pRight;
                return SUCC;
            } else {
                pData->bEnd_ = true;
                free(pData->pStack_);
                pData->pStack_ = NULL;
            }
        }
    }

    *ppPair = NULL;
    return END;
}

int32_t TreeMapReverseIterate(TreeMap *self, bool bReset, Pair **ppPair)
{
    CHECK_INIT(self);

    TreeMapData *pData = self->pData;
    if (bReset) {
        if (pData->pStack_)
            free(pData->pStack_);
        pData->pStack_ = (TreeNode**)malloc(sizeof(TreeNode*) * pData->iSize_);
        if (!(pData->pStack_)) {
            *ppPair = NULL;
            return ERR_NOMEM;
        }
        pData->pIter_ = pData->pRoot_;
        pData->iTop_ = 0;
        pData->bEnd_ = false;
        return SUCC;
    }

    if (!ppPair)
        return ERR_GET;

    while (!(pData->bEnd_)) {
        if (pData->pIter_ != pData->pNull_) {
            if (pData->iTop_ == pData->iSize_) {
                *ppPair = NULL;
                return ERR_ITER;
            }
            pData->pStack_[pData->iTop_] = pData->pIter_;
            pData->iTop_++;
            pData->pIter_ = pData->pIter_->pRight;
        } else {
            if (pData->iTop_ != 0) {
                pData->iTop_--;
                pData->pIter_ = pData->pStack_[pData->iTop_];
                *ppPair = pData->pIter_->pPair;
                pData->pIter_ = pData->pIter_->pLeft;
                return SUCC;
            } else {
                pData->bEnd_ = true;
                free(pData->pStack_);
                pData->pStack_ = NULL;
            }
        }
    }

    *ppPair = NULL;
    return END;
}

int32_t TreeMapSetCompare(TreeMap *self, int32_t (*pFunc) (Key, Key))
{
    CHECK_INIT(self);
    self->pData->pCompare_ = pFunc;
    return SUCC;
}

int32_t TreeMapSetDestroy(TreeMap *self, void (*pFunc) (Pair*))
{
    CHECK_INIT(self);
    self->pData->pDestroy_ = pFunc;
    return SUCC;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
void _TreeMapDeinit(TreeMapData *pData)
{
    TreeNode *pNull = pData->pNull_;
    if (pData->pRoot_ == pNull)
        return;

    /* Simulate the stack and apply iterative post-order tree traversal. */
    TreeNode ***stack = (TreeNode***)malloc(sizeof(TreeNode**) * pData->iSize_);
    assert(stack != NULL);

    int32_t iSize = 0;
    stack[iSize++] = &(pData->pRoot_);
    while (iSize > 0) {
        TreeNode **ppCurr = stack[iSize - 1];
        TreeNode *pCurr = *ppCurr;
        if (pCurr->pLeft != pNull)
            stack[iSize++] = &(pCurr->pLeft);
        else if (pCurr->pRight != pNull)
            stack[iSize++] = &(pCurr->pRight);
        else {
            pData->pDestroy_(pCurr->pPair);
            TreeNode *pParent = pCurr->pParent;
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

TreeNode* _TreeMapMinimal(TreeNode *pNull, TreeNode *pCurr)
{
    TreeNode *pParent = pNull;
    while (pCurr != pNull) {
        pParent = pCurr;
        pCurr = pCurr->pLeft;
    }
    return pParent;
}

TreeNode* _TreeMapMaximal(TreeNode *pNull, TreeNode *pCurr)
{
    TreeNode *pParent = pNull;
    while (pCurr != pNull) {
        pParent = pCurr;
        pCurr = pCurr->pRight;
    }
    return pParent;
}

TreeNode* _TreeMapSuccessor(TreeNode *pNull, TreeNode *pCurr)
{
    if (pCurr != pNull) {
        /* Case 1: The minimal node in the non-null right subtree. */
        if (pCurr->pRight != pNull)
            pCurr = _TreeMapMinimal(pNull, pCurr->pRight);

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

TreeNode* _TreeMapPredecessor(TreeNode *pNull, TreeNode *pCurr)
{
    if (pCurr != pNull) {
        /* Case 1: The maximal node in the non-null left subtree. */
        if (pCurr->pLeft != pNull)
            pCurr = _TreeMapMaximal(pNull, pCurr->pLeft);

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

void _TreeMapRightRotate(TreeMapData *pData, TreeNode *pCurr)
{
    TreeNode *pNull = pData->pNull_;
    TreeNode *pChild = pCurr->pLeft;
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

void _TreeMapLeftRotate(TreeMapData *pData, TreeNode *pCurr)
{
    TreeNode *pNull = pData->pNull_;
    TreeNode *pChild = pCurr->pRight;
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

void _TreeMapInsertFixup(TreeMapData *pData, TreeNode *pCurr)
{
    TreeNode *pUncle;

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
                    _TreeMapLeftRotate(pData, pCurr);
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
                _TreeMapRightRotate(pData, pCurr->pParent->pParent);
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
                    _TreeMapRightRotate(pData, pCurr);
                }
                /* Case 3: The color of x's uncle is black, and x is its parent's
                   right child. */
                pCurr->pParent->bColor = COLOR_BLACK;
                pCurr->pParent->pParent->bColor = COLOR_RED;
                _TreeMapLeftRotate(pData, pCurr->pParent->pParent);
            }
        }
    }

    pData->pRoot_->bColor = COLOR_BLACK;
    return;
}

void _TreeMapDeleteFixup(TreeMapData *pData, TreeNode *pCurr)
{
    TreeNode *pBrother;

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
                _TreeMapLeftRotate(pData, pCurr->pParent);
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
                    _TreeMapRightRotate(pData, pBrother);
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
                _TreeMapLeftRotate(pData, pCurr->pParent);
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
                _TreeMapRightRotate(pData, pCurr->pParent);
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
                    _TreeMapLeftRotate(pData, pBrother);
                    pBrother = pCurr->pParent->pLeft;
                }
                /* Case 4: The color of x's brother is black, and its left child
                   is red. */
                pBrother->bColor = pCurr->pParent->bColor;
                pCurr->pParent->bColor = COLOR_BLACK;
                pBrother->pLeft->bColor = COLOR_BLACK;
                _TreeMapRightRotate(pData, pCurr->pParent);
                pCurr = pData->pRoot_;
            }
        }
    }

    pCurr->bColor = COLOR_BLACK;
    return;
}

TreeNode* _TreeMapSearch(TreeMapData *pData, Key key)
{
    int32_t iOrder;
    TreeNode *pCurr = pData->pRoot_;
    while(pCurr != pData->pNull_) {
        iOrder = pData->pCompare_(key, pCurr->pPair->key);
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

int32_t _TreeMapCompare(Key keySrc, Key keyTge)
{
    if (keySrc == keyTge)
        return 0;
    return (keySrc > keyTge)? 1 : (-1);
}

void _TreeMapDestroy(Pair *pPair) { free(pPair); }
