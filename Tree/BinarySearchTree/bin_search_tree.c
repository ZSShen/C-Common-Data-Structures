#include "bin_search_tree.h"

/*===========================================================================*
 *                  Simulation for private variables                         *
 *===========================================================================*/
static unsigned long _ulSize;
static int (*_pCompare)(const void*, const void*);
static void (*_pDestroy)(void*);


/*===========================================================================*
 *                  Definition for internal functions                        *
 *===========================================================================*/
#define DIRECTION_LEFT      0
#define DIRECTION_RIGHT     1


/**
 * This function traverses all the nodes and releases the allocated memory space.
 *
 * @param curr      The pointer to the node currently traversed.
 * @param func      The pointer to the function which describes the memory deallocation strategy.
 */
void _BSTreeDeinit(TreeNode *curr, void (*func) (void*));


/**
 * This function locates the node containing minimal key within the subtree rooted by the
 * specified node.
 *
 * @param curr      The pointer to the root node for minimal key traversal.
 *
 * @return          Non-NULL: The pointer to the target node.
 *                  NULL    : The subtree is empty.
 */
TreeNode* _BSTreeMinimal(TreeNode *curr);


/**
 * This function locates the node containing maximal key within the subtree rooted by the
 * specified node.
 *
 * @param curr      The pointer to the root node for maximal key traversal.
 *
 * @return          Non-NULL: The pointer to the target node.
 *                  NULL    : The subtree is empty.
 */
TreeNode* _BSTreeMaximal(TreeNode *curr);


/**
 * This function locates the successor of the specified node.
 *
 * @param curr      The pointer to the node for successor search.
 *
 * @return          Non-NULL: The pointer to the target node.
 *                  NULL    : The size of the tree is less than two.
 */
TreeNode* _BSTreeSuccessor(TreeNode *curr);


/**
 * This function locates the predecessor of the specified node.
 *
 * @param curr      The pointer to the node for predecessor search.
 *
 * @return          Non-NULL: The pointer to the target node.
 *                  NULL    : The size of the tree is less than two.
 */
TreeNode* _BSTreePredecessor(TreeNode *curr);


/**
 * This function is the default comparison strategy for the items of a pair of tree nodes.
 *
 * @param pSrc          The pointer to the source item.
 * @param pDst          The pointer to the target item.
 *
 * @return          1 : The source item goes after the target one with certain ordering criteria.
 *                  0 : The source item equals to the target one with certain ordering criteria.
 *                  -1: The source item goes before the target one with certain ordering criteria.
 */
int _BSTreeItemCompare(const void *pSrc, const void *pTge);


/**
 * This function is the default deallcation strategy for an item.
 * 
 * @param pItem         The pointer to the item which is to be deallocated.
 */
void _BSTreeItemDestroy(void *pItem);


/*===========================================================================*
 *               Implementation for exported functions                       *
 *===========================================================================*/
void BSTreeInit(BinSearchTree *self) {
    
    self->pRoot = NULL;
    _ulSize = 0;

    /* Let the function pointers point to the corresponding functions */
    _pCompare = _BSTreeItemCompare;
    _pDestroy = _BSTreeItemDestroy;    

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

    return;
}


void BSTreeDeinit(BinSearchTree *self) {

    _BSTreeDeinit(self->pRoot, _pDestroy);
    return;
}


/*
 * BSTreeMinimum(): Locate the node containing the minimum key of the tree.
 */
TreeNode* BSTreeMinimum(BinSearchTree *self) {

    return _BSTreeMinimal(self->pRoot);
}


/*
 * BSTreeMaximum(): Locate the node containing the maximum key of the tree.
 */
TreeNode* BSTreeMaximum(BinSearchTree *self) {

    return _BSTreeMaximal(self->pRoot);
}


/*
 * BSTreeSuccessor(): Locate the successor of the designated node.
 */
TreeNode* BSTreeSuccessor(BinSearchTree *self, TreeNode *pCurNode) {

    return _BSTreeSuccessor(pCurNode);    
}


/*
 * BSTreePredecessor(): Locate the predecessor of the designated node.
 */
TreeNode* BSTreePredecessor(BinSearchTree *self, TreeNode *pCurNode) {

    return _BSTreePredecessor(pCurNode);
}


/**
 * BSTreeInsert(): Insert the new node to the appropriate location of the tree.
 */
TreeNode* BSTreeInsert(BinSearchTree *self, void *pItem) {
    int         rc;
    bool        direction;    
    TreeNode    *new, *curr, *parent;
    
    new = (TreeNode*)malloc(sizeof(TreeNode));
    if (new == NULL)
        return new;

    new->pItem = pItem;
    new->pParent = NULL;
    new->pLeft = NULL;
    new->pRight = NULL;

    parent = NULL;
    curr = self->pRoot;
    while (curr != NULL) {
        parent = curr;    
        rc = _pCompare(pItem, curr->pItem);

        if (rc == 1) {
            curr = curr->pRight;
            direction = DIRECTION_RIGHT;        
        }        
        else if (rc == -1) {
            curr = curr->pLeft;
            direction = DIRECTION_LEFT;        
        }
    
        /* The conflict occurs. The new node will replace the existing one. */
        else {
            new->pLeft = curr->pLeft;
            new->pRight = curr->pRight;
            new->pParent = curr->pParent;
            
            if (curr->pLeft != NULL)
                curr->pLeft->pParent = new;
            
            if (curr->pRight != NULL)            
                curr->pRight->pParent = new;
            
            if (curr->pParent != NULL) {
                if (curr == curr->pParent->pLeft)
                    curr->pParent->pLeft = new;
                else
                    curr->pParent->pRight = new;            
            } else
                self->pRoot = new;

            _pDestroy(curr->pItem);
            free(curr);            
            return new;             
        }
    }

    /* Increase the size. */
    _ulSize++;

    /* Arrive at the appropriate location. */
    new->pParent = parent;
    if (parent != NULL) {
        if (direction == DIRECTION_LEFT)
            parent->pLeft = new;
        else
            parent->pRight = new;            
    } else
        self->pRoot = new;

    return new;
}


/**
 * BSTreeDelete(): Delete the specified node from the tree and adjust the tree structure.
 */
void BSTreeDelete(BinSearchTree *self, TreeNode *pNode) {
    TreeNode *child, *succ, *parent;
    
    if (pNode != NULL) {
        /* The specified node has no child. */
        if ((pNode->pLeft == NULL) && (pNode->pRight == NULL)) {
            if (pNode->pParent != NULL) {
                if (pNode == pNode->pParent->pLeft)
                    pNode->pParent->pLeft = NULL;            
                else
                    pNode->pParent->pRight = NULL;
            } else
                self->pRoot = NULL;
            
            _pDestroy(pNode->pItem);
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

                _pDestroy(pNode->pItem);
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
                    self->pRoot = child;
                
                _pDestroy(pNode->pItem);
                free(pNode); 
            }        
        } 

        /* Decrease the size. */
        _ulSize--;
    }

    return;
}


/*
 * BSTreeSearch(): Check whethere the tree has the designated item.
 */
bool BSTreeSearch(BinSearchTree *self, void *pItem) {
    int      rc;    
    TreeNode *curr;

    curr = self->pRoot;
    while(curr != NULL) {
        rc = _pCompare(pItem, curr->pItem);
        
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


/*
 * BSTreeSize(): Return the size of the tree.
 */
unsigned long BSTreeSize(BinSearchTree *self) {

    return _ulSize;
}


/**
 * BSTreeSetCompare(): Set the item comparison strategy with the one defined by user.
 */
void BSTreeSetCompare(BinSearchTree *self, int (*pFunc)(const void*, const void*)) {
    
    _pCompare = pFunc;
    return;
}


/**
 * BSTreeSetDestroy(): Set the item deallocation strategy with the one defined by user.
 */
void BSTreeSetDestroy(BinSearchTree *self, void (*pFunc)(void*)) {

    _pDestroy = pFunc;
    return;
}


/*===========================================================================*
 *               Implementation for internal functions                       *
 *===========================================================================*/
void _BSTreeDeinit(TreeNode *curr, void (*func) (void*)) {

    if (curr != NULL) {
        _BSTreeDeinit(curr->pLeft, func);
        _BSTreeDeinit(curr->pRight, func);
        func(curr->pItem);
        free(curr);
    }

    return;
}


TreeNode* _BSTreeMinimal(TreeNode *curr) {
    TreeNode *parent;
    
    parent = NULL;
    while (curr != NULL) {
        parent = curr;     
        curr = curr->pLeft;
    }

    return parent;
}


TreeNode* _BSTreeMaximal(TreeNode *curr) {
    TreeNode *parent;

    parent = NULL;
    while (curr != NULL) {
        parent = curr;
        curr = curr->pRight;
    }

    return parent;
}


TreeNode* _BSTreeSuccessor(TreeNode *curr) {
        
    if (curr != NULL) {
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


TreeNode* _BSTreePredecessor(TreeNode *curr) {

    if (curr != NULL) {
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
 * _BSTreeItemCompare(): The default comparison strategy for the items of a pair of nodes.
 * Note: It considers source and target items as primitive data and compares their values directly.
 */
int _BSTreeItemCompare(const void *pSrc, const void *pTge) {
    
    if ((size_t)pSrc == (size_t)pTge)
        return 0;
    else {
        if ((size_t)pSrc > (size_t)pTge)
            return 1;
        else
            return -1;    
    }
}

/**
 * _BSTreeItemDestroy(): The default deallocation strategy for an item.
 * Note: By default, the item is a primitive data, and thus no further operations are required.
 */
void _BSTreeItemDestroy(void *pItem) {

    return;
}

