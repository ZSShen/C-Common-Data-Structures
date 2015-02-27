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
 *               Implementation for exported functions                       *
 *===========================================================================*/
int32_t BSTreeInit(BinSearchTree *self)
{
    self->pData = (BSTreeData*)malloc(sizeof(BSTreeData));
    if (!self->pData)
        return FAIL;

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
 * BSTreeInsert(): Insert a new node storing the requested item to the appropriate position of the tree.
 */
TreeNode* BSTreeInsert(BinSearchTree *self, void *pItem)
{
    int32_t rc;
    bool direction;
    TreeNode *new, *curr, *parent;

    new = (TreeNode*)malloc(sizeof(TreeNode));
    if (!new)
        return new;

    new->pItem = pItem;
    new->pParent = NULL;
    new->pLeft = NULL;
    new->pRight = NULL;

    parent = NULL;
    curr = self->pData->_pRoot;
    while (curr) {
        parent = curr;
        rc = self->pData->_pCompare(pItem, curr->pItem);

        if (rc > 0) {
            curr = curr->pRight;
            direction = DIRECTION_RIGHT;
        }
        else if (rc < 0) {
            curr = curr->pLeft;
            direction = DIRECTION_LEFT;
        }
        else {
            /* The conflict occurs. The new node will replace the existing one. */
            new->pLeft = curr->pLeft;
            new->pRight = curr->pRight;
            new->pParent = curr->pParent;

            if (curr->pLeft)
                curr->pLeft->pParent = new;

            if (curr->pRight)
                curr->pRight->pParent = new;

            if (curr->pParent) {
                if (curr == curr->pParent->pLeft)
                    curr->pParent->pLeft = new;
                else
                    curr->pParent->pRight = new;
            } else
                self->pData->_pRoot = new;

            self->pData->_pDestroy(curr->pItem);
            free(curr);
            return new;
        }
    }

    /* Increase the size. */
    self->pData->_uiSize++;

    /* Arrive at the appropriate location. */
    new->pParent = parent;
    if (parent) {
        if (direction == DIRECTION_LEFT)
            parent->pLeft = new;
        else
            parent->pRight = new;
    } else
        self->pData->_pRoot = new;

    return new;
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
