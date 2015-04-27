#include "tree/balanced_tree.h"


/*===========================================================================*
 *                Hide the private data of the container                     *
 *===========================================================================*/
typedef struct _BalTreeNode {
    Item item;
    bool bColor;
    struct _BalTreeNode *pParent;
    struct _BalTreeNode *pLeft;
    struct _BalTreeNode *pRight;
} BalTreeNode;

struct _BalTreeData {
    int32_t _iSize;
    BalTreeNode *_pRoot;
    BalTreeNode *_pNull;
    int32_t (*_pCompare)(Item, Item);
    void (*_pDestroy)(Item);
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
 * @param pData         The pointer to the tree private data
 */
void _BalTreeDeinit(BalTreeData *pData);

/**
 * @brief Return the node with maximal order in the subtree rooted by the
 * requested node.
 *
 * @param pNull         The pointer to the dummy node
 * @param pCurr         The pointer to the requested node
 *
 * @return Non-NULL     The pointer to the returned node
 * @return NULL         The subtree is empty
 */
BalTreeNode* _BalTreeMaximal(BalTreeNode *pNull, BalTreeNode *pCurr);

/**
 * @brief Return the node with minimal order in the subtree rooted by the
 * requested node.
 *
 * @param pNull         The pointer to the dummy node
 * @param pCurr         The pointer to the requested node
 *
 * @return Non-NULL     The pointer to the target node
 * @return NULL         The subtree is empty
 */
BalTreeNode* _BalTreeMinimal(BalTreeNode *pNull, BalTreeNode *pCurr);

/**
 * @brief Return the immediate successor of the requested node.
 *
 * @param pNull         The pointer to the dummy node
 * @param pCurr         The pointer to the requested node
 *
 * @return Non-NULL     The pointer to the returned node
 * @return NULL         The immediate successor cannot be found
 */
BalTreeNode* _BalTreeSuccessor(BalTreeNode *pNull, BalTreeNode *pCurr);

/**
 * @brief Return the immediate predecessor of the requested node.
 *
 * @param pNull         The pointer to the dummy node
 * @param pCurr         The pointer to the requested node
 *
 * @return Non-NULL     The pointer to the returned node
 * @return NULL         The immediate predecessor cannot be found
 */
BalTreeNode* _BalTreePredecessor(BalTreeNode *pNull, BalTreeNode *pCurr);

/**
 * @brief Make the right rotation for the requested node.
 *
 * After rotation, the requested node will be the right child of its original
 * left child.
 *
 * @param pData         The pointer to the tree private data
 * @param pCurr         The pointer to the requested node
 */
void _BalTreeRightRotate(BalTreeData *pData, BalTreeNode *pCurr);

/**
 * @brief Make the left rotation for the requested node.
 *
 * After rotation, the requested node will be the left child of its original
 * right child.
 *
 * @param pData         The pointer to the tree private data
 * @param pCurr         The pointer to the requested node
 */
void _BalTreeLeftRotate(BalTreeData *pData, BalTreeNode *pCurr);

/**
 * @brief Maintain the balanced tree property after node insertion.
 *
 * @param pData         The pointer to the tree private data
 * @param pCurr         The pointer to the requested node
 */
void _BalTreeInsertFixup(BalTreeData *pData, BalTreeNode *pCurr);

/**
 * @brief Maintain the balanced tree property after node deletion.
 *
 * @param pData         The pointer to the tree private data
 * @param pCurr         The pointer to the requested node
 */
void _BalTreeDeleteFixup(BalTreeData *pData, BalTreeNode *pCurr);

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
BalTreeNode* _BalTreeSearch(BalTreeData *pData, Item item);

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
int32_t _BalTreeItemCompare(Item itemSrc, Item itemTge);

/**
 * @brief The default clean strategy for the resource hold by an item.
 *
 * @param item         The requested item
 */
void _BalTreeItemDestroy(Item item);

/**
 * @brief The internal debug function to verify the tree structure invariant.
 *
 * @param pData         The pointer to the tree private data
 *
 * @return true|false   Whether the tree has legal structure.
 */
bool _BalTreeValidate(BalTreeData *pData);


/*===========================================================================*
 *           Implementation for the exported member operations               *
 *===========================================================================*/
int32_t BalTreeInit(BalancedTree **ppObj)
{
    BalancedTree *pObj;
    *ppObj = (BalancedTree*)malloc(sizeof(BalancedTree));
    if (!(*ppObj))
        return ERR_NOMEM;
    pObj = *ppObj;

    pObj->pData = (BalTreeData*)malloc(sizeof(BalTreeData));
    if (!pObj->pData) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }

    /* Create the dummy node representing as the NULL pointer of the tree. */
    BalTreeData *pData = pObj->pData;
    pData->_pNull = (BalTreeNode*)malloc(sizeof(BalTreeNode));
    if (!pData->_pNull) {
        free(pObj->pData);
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    pData->_pNull->bColor = COLOR_BLACK;
    pData->_pNull->item = NULL;
    pData->_pNull->pParent = pData->_pNull;
    pData->_pNull->pRight = pData->_pNull;
    pData->_pNull->pLeft = pData->_pNull;

    pObj->pData->_pRoot = pData->_pNull;
    pObj->pData->_iSize = 0;
    pObj->pData->_pCompare = _BalTreeItemCompare;
    pObj->pData->_pDestroy = _BalTreeItemDestroy;

    pObj->insert = BalTreeInsert;
    pObj->search = BalTreeSearch;
    //pObj->delete = BalTreeDelete;
    pObj->maximum = BalTreeMaximum;
    pObj->minimum = BalTreeMinimum;
    pObj->successor = BalTreeSuccessor;
    pObj->predecessor = BalTreePredecessor;
    pObj->size = BalTreeSize;
    pObj->set_compare = BalTreeSetCompare;
    pObj->set_destroy = BalTreeSetDestroy;

    return SUCC;
}

void BalTreeDeinit(BalancedTree **ppObj)
{
    BalancedTree *pObj;
    if (*ppObj) {
        pObj = *ppObj;
        if (pObj->pData) {
            if (pObj->pData->_pNull) {
                _BalTreeDeinit(pObj->pData);
                free(pObj->pData->_pNull);
                free(pObj->pData);
            }
        }
        free(*ppObj);
        *ppObj = NULL;
    }
    return;
}

int32_t BalTreeInsert(BalancedTree *self, Item item)
{
    int32_t iOrder;
    int8_t cDirect;
    BalTreeNode *pNew, *pCurr, *pParent;

    pNew = (BalTreeNode*)malloc(sizeof(BalTreeNode));
    if (!pNew)
        return ERR_NOMEM;
    BalTreeData *pData = self->pData;
    pNew->item = item;
    pNew->bColor = COLOR_RED;
    pNew->pParent = pData->_pNull;
    pNew->pLeft = pData->_pNull;
    pNew->pRight = pData->_pNull;

    pParent = pData->_pNull;
    pCurr = pData->_pRoot;
    while (pCurr != pData->_pNull) {
        pParent = pCurr;
        iOrder = pData->_pCompare(item, pCurr->item);
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
            pData->_pDestroy(pCurr->item);
            pCurr->item = item;
            return SUCC;
        }
    }

    /* Arrive at the proper position. */
    pNew->pParent = pParent;
    if (pParent != pData->_pNull) {
        if (cDirect == DIRECT_LEFT)
            pParent->pLeft = pNew;
        else
            pParent->pRight = pNew;
    } else
        self->pData->_pRoot = pNew;

    /* Increase the size. */
    self->pData->_iSize++;

    /* Maintain the balanced tree structure. */
    _BalTreeInsertFixup(self->pData, pNew);

    return SUCC;
}

int32_t BalTreeSearch(BalancedTree *self, Item itemIn, Item *pItemOut)
{
    BalTreeNode *pFind;
    pFind = _BalTreeSearch(self->pData, itemIn);
    if (pFind != self->pData->_pNull) {
        *pItemOut = pFind->item;
        return SUCC;
    }
    *pItemOut = NULL;
    return ERR_NODATA;
}

int32_t BalTreeMaximum(BalancedTree *self, Item *pItem)
{
    BalTreeData *pData = self->pData;
    BalTreeNode *pFind = _BalTreeMaximal(pData->_pNull, pData->_pRoot);
    if (pFind != pData->_pNull) {
        *pItem = pFind->item;
        return SUCC;
    }
    return ERR_IDX;
}

int32_t BalTreeMinimum(BalancedTree *self, Item *pItem)
{
    BalTreeData *pData = self->pData;
    BalTreeNode *pFind = _BalTreeMinimal(pData->_pNull, pData->_pRoot);
    if (pFind != pData->_pNull) {
        *pItem = pFind->item;
        return SUCC;
    }
    return ERR_IDX;
}

int32_t BalTreeSuccessor(BalancedTree *self, Item itemIn, Item *pItemOut)
{
    BalTreeNode *pNull = self->pData->_pNull;
    BalTreeNode *pCurr = _BalTreeSearch(self->pData, itemIn);
    if (pCurr == pNull)
        return ERR_NODATA;

    BalTreeNode *pFind = _BalTreeSuccessor(pNull, pCurr);
    if (pFind != pNull) {
        *pItemOut = pFind->item;
        return SUCC;
    }
    return ERR_NODATA;
}

int32_t BalTreePredecessor(BalancedTree *self, Item itemIn, Item *pItemOut)
{
    BalTreeNode *pNull = self->pData->_pNull;
    BalTreeNode *pCurr = _BalTreeSearch(self->pData, itemIn);
    if (pCurr == pNull)
        return ERR_NODATA;

    BalTreeNode *pFind = _BalTreePredecessor(pNull, pCurr);
    if (pFind != pNull) {
        *pItemOut = pFind->item;
        return SUCC;
    }
    return ERR_NODATA;
}

int32_t BalTreeSize(BalancedTree *self)
{
    return self->pData->_iSize;
}

void BalTreeSetCompare(BalancedTree *self, int32_t (*pFunc) (Item, Item))
{
    self->pData->_pCompare = pFunc;
    return;
}

void BalTreeSetDestroy(BalancedTree *self, void (*pFunc) (Item))
{
    self->pData->_pDestroy = pFunc;
    return;
}


/*===========================================================================*
 *               Implementation for internal functions                       *
 *===========================================================================*/
void _BalTreeDeinit(BalTreeData *pData)
{
    BalTreeNode *pNull = pData->_pNull;
    if (pData->_pRoot == pNull)
        return;

    /* Simulate the stack and apply iterative post-order tree traversal. */
    BalTreeNode ***stack = (BalTreeNode***)malloc(sizeof(BalTreeNode**) * pData->_iSize);
    assert(stack != NULL);

    int32_t iSize = 0;
    stack[iSize++] = &(pData->_pRoot);
    while (iSize > 0) {
        BalTreeNode **ppCurr = stack[iSize - 1];
        BalTreeNode *pCurr = *ppCurr;
        if (pCurr->pLeft != pNull)
            stack[iSize++] = &(pCurr->pLeft);
        else if (pCurr->pRight != pNull)
            stack[iSize++] = &(pCurr->pRight);
        else {
            pData->_pDestroy(pCurr->item);
            BalTreeNode *pParent = pCurr->pParent;
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

BalTreeNode* _BalTreeMinimal(BalTreeNode *pNull, BalTreeNode *pCurr)
{
    BalTreeNode *pParent = pNull;
    while (pCurr != pNull) {
        pParent = pCurr;
        pCurr = pCurr->pLeft;
    }
    return pParent;
}

BalTreeNode* _BalTreeMaximal(BalTreeNode *pNull, BalTreeNode *pCurr)
{
    BalTreeNode *pParent = pNull;
    while (pCurr != pNull) {
        pParent = pCurr;
        pCurr = pCurr->pRight;
    }
    return pParent;
}

BalTreeNode* _BalTreeSuccessor(BalTreeNode *pNull, BalTreeNode *pCurr)
{
    if (pCurr != pNull) {
        /* Case 1: The minimal node in the non-null right subtree. */
        if (pCurr->pRight != pNull)
            pCurr = _BalTreeMinimal(pNull, pCurr->pRight);

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

BalTreeNode* _BalTreePredecessor(BalTreeNode *pNull, BalTreeNode *pCurr)
{
    if (pCurr != pNull) {
        /* Case 1: The maximal node in the non-null left subtree. */
        if (pCurr->pLeft != pNull)
            pCurr = _BalTreeMaximal(pNull, pCurr->pLeft);

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

void _BalTreeRightRotate(BalTreeData *pData, BalTreeNode *pCurr) {
    BalTreeNode *pNull = pData->_pNull;
    BalTreeNode *pChild = pCurr->pLeft;
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
        pData->_pRoot = pChild;

    /* Let y link x as its parent.
       And let x link y as its right child. */
    pCurr->pParent = pChild;
    pChild->pRight = pCurr;

    return;
}

void _BalTreeLeftRotate(BalTreeData *pData, BalTreeNode *pCurr) {
    BalTreeNode *pNull = pData->_pNull;
    BalTreeNode *pChild = pCurr->pRight;
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
        pData->_pRoot = pChild;

    /* Let x link y as its parent.
       And let y link x as its left child. */
    pCurr->pParent = pChild;
    pChild->pLeft = pCurr;

    return;
}

void _BalTreeInsertFixup(BalTreeData *pData, BalTreeNode *pCurr)
{
    BalTreeNode *pUncle;

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
                    _BalTreeLeftRotate(pData, pCurr);
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
                _BalTreeRightRotate(pData, pCurr->pParent->pParent);
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
                    _BalTreeRightRotate(pData, pCurr);
                }
                /* Case 3: The color of x's uncle is black, and x is its parent's
                   right child. */
                pCurr->pParent->bColor = COLOR_BLACK;
                pCurr->pParent->pParent->bColor = COLOR_RED;
                _BalTreeLeftRotate(pData, pCurr->pParent->pParent);
            }
        }
    }

    pData->_pRoot->bColor = COLOR_BLACK;
    return;
}

BalTreeNode* _BalTreeSearch(BalTreeData *pData, Item item)
{
    int32_t iOrder;
    BalTreeNode *pCurr = pData->_pRoot;
    while(pCurr != pData->_pNull) {
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

int32_t _BalTreeItemCompare(Item itemSrc, Item itemTge)
{
    if (itemSrc == itemTge)
        return 0;
    else
        return (itemSrc > itemTge)? 1 : -1;
}

void _BalTreeItemDestroy(Item item) {}

bool _BalTreeValidate(BalTreeData *pData)
{
    BalTreeNode *pNull = pData->_pNull;
    bool bLegal = true;

    /* Simulate the stack and apply iterative in-order tree traversal. */
    BalTreeNode **stack = (BalTreeNode**)malloc(sizeof(BalTreeNode*) * pData->_iSize);
    BalTreeNode *pCurr = pData->_pRoot;
    BalTreeNode *pPred = pNull;
    int32_t iSize = 0;

    while ((pCurr != pNull) || (iSize > 0)) {
        if (pCurr != pNull) {
            stack[iSize++] = pCurr;
            pCurr = pCurr->pLeft;
        } else {
            if (pPred != pNull) {
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
