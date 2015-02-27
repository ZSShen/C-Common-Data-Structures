#include "bin_search_tree.h"

/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
struct _TreeNode {
    void *pItem;
    struct _TreeNode *pParent;
    struct _TreeNode *pLeft;
    struct _TreeNode *pRight;
};

struct _BSTreeData {
    uint32_t _uiSize;
    TreeNode *_pRoot;
    int32_t (*_pCompare)(const void*, const void*);
    void (*_pDestroy)(void*);
};


#define DIRECTION_LEFT      (0)
#define DIRECTION_RIGHT     (1)


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * The constructor for BinSearchTree.
 * 
 * @param self          The pointer to the BinSearchTree structure.
 *
 * @return              SUCCESS: Operation success.
 *                      FAIL_NO_MEMORY   : Insufficient memory space.
 */
int32_t BSTreeInit(BinSearchTree *self);

/**
 * The destructor for BinSearchTree.
 */
void BSTreeDeinit(BinSearchTree *self);

/**
 * This function allocates a node to store the requested item and inserts it
 * into the proper position of the tree.
 *
 * @param self          The pointer to the BinSearchTree structure.
 * @param pItem         The pointer to the requested item.
 *
 * @return              SUCCESS
 *                      FAIL_NO_MEMORY
 *                      FAIL_DATA_CONFLICT
 */
int32_t BSTreeInsert(BinSearchTree *self, void *pItem);


/*===========================================================================*
 *                  Definition for internal functions                        *
 *===========================================================================*/
/**
 * This function traverses all the nodes and releases the allocated memory space.
 *
 * @param curr      The pointer to the node currently traversed.
 * @param func      The pointer to the function which describes the node item deallocation strategy.
 */
void _BSTreeDeinit(TreeNode *curr, void (*func) (void*));

/**
 * This function returns the node with minimal order within the subtree rooted by the
 * specified node.
 *
 * @param curr      The pointer to the root node of a specific subtree.
 *
 * @return          Non-NULL: The pointer to the target node.
 *                  NULL    : The subtree is empty.
 */
TreeNode* _BSTreeMinimal(TreeNode *curr);


/**
 * This function returns the node with maximal order within the subtree rooted by the
 * specified node.
 *
 * @param curr      The pointer to the root node of a specific subtree.
 *
 * @return          Non-NULL: The pointer to the target node.
 *                  NULL    : The subtree is empty.
 */
TreeNode* _BSTreeMaximal(TreeNode *curr);

/**
 * This function returns the successor of the designated node.
 *
 * @param curr      The pointer to the node for successor search.
 *
 * @return          Non-NULL: The pointer to the target node.
 *                  NULL    : There is no successor for the designated node.
 */
TreeNode* _BSTreeSuccessor(TreeNode *curr);

/**
 * This function returns the predecessor of the designated node.
 *
 * @param curr      The pointer to the node for predecessor search.
 *
 * @return          Non-NULL: The pointer to the target node.
 *                  NULL    : There is no predecessor for the designated node.
 */
TreeNode* _BSTreePredecessor(TreeNode *curr);


/**
 * This function is the default comparison strategy for the items of a pair of tree nodes.
 *
 * @param pSrc          The pointer to the source item.
 * @param pDst          The pointer to the target item.
 *
 * @return        > 0 : The source item goes after the target one with certain ordering criteria.
 *                  0 : The source item equals to the target one with certain ordering criteria.
 *                < 0 : The source item goes before the target one with certain ordering criteria.
 */
int32_t _BSTreeItemCompare(const void *pSrc, const void *pTge);


/**
 * This function is the default deallcation strategy for an item.
 *
 * @param pItem         The pointer to the item which is to be deallocated.
 */
void _BSTreeItemDestroy(void *pItem);


/*===========================================================================*
 *           Implementation for the exported member operations               *
 *===========================================================================*/
int32_t BSTreeInit(BinSearchTree *self)
{
    self->pData = (BSTreeData*)malloc(sizeof(BSTreeData));
    if (!self->pData)
        return FAIL_NO_MEMORY;

    self->pData->_pRoot = NULL;
    self->pData->_uiSize = 0;
    self->pData->_pCompare = _BSTreeItemCompare;
    self->pData->_pDestroy = _BSTreeItemDestroy;

    self->insert = BSTreeInsert;
    self->delete = BSTreeDelete;
    self->search = BSTreeSearch;
    self->size = BSTreeSize;

    self->maximum = BSTreeMaximum;
    self->minimum = BSTreeMinimum;
    self->successor = BSTreeSuccessor;
    self->predecessor = BSTreePredecessor;

    self->set_compare = BSTreeSetCompare;
    self->set_destroy = BSTreeSetDestroy;

    return SUCCESS;
}

void BSTreeDeinit(BinSearchTree *self)
{
    if (self) {
        if (self->pData) {
            _BSTreeDeinit(self->pData->_pRoot, self->pData->_pDestroy);
            free(self->pData);
        }
        free(self);
    }
    return;
}

int32_t BSTreeInsert(BinSearchTree *self, void *pItem)
{
    int32_t iOrder;
    int8_t cDirect;
    TreeNode *pNew, *pCurr, *pParent;

    pNew = (TreeNode*)malloc(sizeof(TreeNode));
    if (!pNew)
        return FAIL_NO_MEMORY;
    pNew->pItem = pItem;
    pNew->pParent = NULL;
    pNew->pLeft = NULL;
    pNew->pRight = NULL;

    pParent = NULL;
    pCurr = self->pData->_pRoot;
    while (pCurr) {
        pParent = pCurr;
        iOrder = self->pData->_pCompare(pItem, pCurr->pItem);
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

    return SUCCESS;
}


/**
 * BSTreeMinimum(): Return the node with minimum order of the tree.
 */
TreeNode* BSTreeMinimum(BinSearchTree *self)
{
    return _BSTreeMinimal(self->pData->_pRoot);
}


/**
 * BSTreeMaximum(): Return the node with maximum order of the tree.
 */
TreeNode* BSTreeMaximum(BinSearchTree *self)
{
    return _BSTreeMaximal(self->pData->_pRoot);
}


/**
 * BSTreeSuccessor(): Return the successor of the designated node.
 */
TreeNode* BSTreeSuccessor(BinSearchTree *self, TreeNode *pCurNode)
{
    return _BSTreeSuccessor(pCurNode);
}


/**
 * BSTreePredecessor(): Return the predecessor of the designated node.
 */
TreeNode* BSTreePredecessor(BinSearchTree *self, TreeNode *pCurNode)
{
    return _BSTreePredecessor(pCurNode);
}


/**
 * BSTreeDelete(): Delete the specified node from the tree and adjust the tree structure.
 */
void BSTreeDelete(BinSearchTree *self, TreeNode *pNode)
{
    TreeNode *child, *succ, *parent;

    if (pNode) {
        /* The specified node has no child. */
        if ((pNode->pLeft == NULL) && (pNode->pRight == NULL)) {
            if (pNode->pParent != NULL) {
                if (pNode == pNode->pParent->pLeft)
                    pNode->pParent->pLeft = NULL;
                else
                    pNode->pParent->pRight = NULL;
            } else
                self->pData->_pRoot = NULL;

            self->pData->_pDestroy(pNode->pItem);
            free(pNode);
        } else {
            /* The specified node has two children. */
            if ((pNode->pLeft != NULL) && (pNode->pRight != NULL)) {
                succ = _BSTreeSuccessor(pNode);

                child = succ->pLeft;
                if (child == NULL)
                    child = succ->pRight;

                if (child != NULL)
                    child->pParent = succ->pParent;

                if (succ == succ->pParent->pLeft)
                    succ->pParent->pLeft = child;
                else
                    succ->pParent->pRight = child;

                self->pData->_pDestroy(pNode->pItem);
                pNode->pItem = succ->pItem;
                free(succ);
            }
            /* The specified node has one child. */
            else {
                child = pNode->pLeft;
                if (child == NULL)
                    child = pNode->pRight;

                child->pParent = pNode->pParent;

                if (pNode->pParent != NULL) {
                    if (pNode == pNode->pParent->pLeft)
                        pNode->pParent->pLeft = child;
                    else
                        pNode->pParent->pRight = child;
                } else
                    self->pData->_pRoot = child;

                self->pData->_pDestroy(pNode->pItem);
                free(pNode);
            }
        }

        /* Decrease the size. */
        self->pData->_uiSize--;
    }

    return;
}


/**
 * BSTreeSearch(): Check whethere the tree has the specified item.
 */
bool BSTreeSearch(BinSearchTree *self, void *pItem)
{
    int32_t rc;
    TreeNode *curr;

    curr = self->pData->_pRoot;
    while(curr) {
        rc = self->pData->_pCompare(pItem, curr->pItem);
        if (rc == 0)
            return true;
        else {
            if (rc > 0)
                curr = curr->pRight;
            else
                curr = curr->pLeft;
        }
    }

    return false;
}


/**
 * BSTreeSize(): Return the size of the tree.
 */
uint32_t BSTreeSize(BinSearchTree *self)
{
    return self->pData->_uiSize;
}


/**
 * BSTreeSetCompare(): Set the item comparison strategy with the one defined by user.
 */
void BSTreeSetCompare(BinSearchTree *self, int32_t (*pFunc)(const void*, const void*))
{
    self->pData->_pCompare = pFunc;
    return;
}


/**
 * BSTreeSetDestroy(): Set the item deallocation strategy with the one defined by user.
 */
void BSTreeSetDestroy(BinSearchTree *self, void (*pFunc)(void*))
{
    self->pData->_pDestroy = pFunc;
    return;
}


/*===========================================================================*
 *               Implementation for internal functions                       *
 *===========================================================================*/
void _BSTreeDeinit(TreeNode *curr, void (*func) (void*))
{
    if (curr) {
        _BSTreeDeinit(curr->pLeft, func);
        _BSTreeDeinit(curr->pRight, func);
        func(curr->pItem);
        free(curr);
    }
    return;
}


TreeNode* _BSTreeMinimal(TreeNode *curr)
{
    TreeNode *parent;
    parent = NULL;
    while (curr) {
        parent = curr;
        curr = curr->pLeft;
    }
    return parent;
}


TreeNode* _BSTreeMaximal(TreeNode *curr)
{
    TreeNode *parent;
    parent = NULL;
    while (curr) {
        parent = curr;
        curr = curr->pRight;
    }
    return parent;
}


TreeNode* _BSTreeSuccessor(TreeNode *curr)
{
    if (curr) {
        /* Case 1: The minimal node of the non-null right subtree. */
        if (curr->pRight != NULL)
            curr = _BSTreeMinimal(curr->pRight);

        /* Case 2: The ancestor which considers the designated node as the maximal node of
           its left subtree. */
        else {
            while((curr->pParent != NULL) && (curr == curr->pParent->pRight))
                curr = curr->pParent;
            curr = curr->pParent;
        }
    }

    return curr;
}


TreeNode* _BSTreePredecessor(TreeNode *curr)
{
    if (curr) {
        /* Case 1: The maximal node of the non-null left subtree. */
        if (curr->pLeft != NULL)
            curr = _BSTreeMaximal(curr->pLeft);

        /* Case 2: The ancestor which considers the designated node as the minimal node of
           its right subtree. */
        else {
            while((curr->pParent != NULL) && (curr == curr->pParent->pLeft))
                curr = curr->pParent;
            curr = curr->pParent;
        }
    }

    return curr;
}


/**
 * _BSTreeItemCompare(): The default comparison strategy for the items of a pair of tree nodes.
 * Note: It considers source and target items as primitive data and
 *       gives the larger order to the one with larger value.
 */
int32_t _BSTreeItemCompare(const void *pSrc, const void *pTge)
{
    int32_t nSrc, nTge;

#if __x86_64__
    int64_t nTmp;
    nTmp = (int64_t)pSrc;
    nSrc = (int32_t)nTmp;
    nTmp = (int64_t)pTge;
    nTge = (int32_t)nTmp;
#else
    nSrc = (int32_t)pSrc;
    nTge = (int32_t)pTge;
#endif

    return nSrc - nTge;
}


/**
 * _BSTreeItemDestroy(): The default deallocation strategy for a node item.
 * Note: By default, the item is a primitive data, and thus no further operations are required.
 */
void _BSTreeItemDestroy(void *pItem)
{ return; }
