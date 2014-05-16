#include "bin_search_tree.h"

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


/*===========================================================================*
 *               Implementation for exported functions                       *
 *===========================================================================*/
void BSTreeInit(BinSearchTree *self) {
    
    self->pRoot = NULL;

    self->compare = BSTreeNodeCompare;
    self->destroy = BSTreeNodeDestroy;

    self->insert = BSTreeInsert;
    self->delete = BSTreeDelete;
    self->search = BSTreeSearch;

    self->maximum = BSTreeMaximum;
    self->minimum = BSTreeMinimum;
    self->successor = BSTreeSuccessor;
    self->predecessor = BSTreePredecessor;

    return;
}


void BSTreeDeinit(BinSearchTree *self) {

    _BSTreeDeinit(self->pRoot, self->destroy);
    return;
}


/**
 * BSTreeNodeCompare(): The default comparison strategy for tree node comparison.
 * Note: It considers source and target items as primitive data and compares their values directly.
 */
int BSTreeNodeCompare(const void *pSrc, const void *pTge) {
    
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
 * BSTreeNodeDestroy(): The default strategy for item deallocation.
 * Note: By default, the item is a primitive data, and thus no further operations are required.
 */
void BSTreeNodeDestroy(void *pItem) {

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
void BSTreeInsert(BinSearchTree *self, TreeNode *pNode) {
    int         rc;
    bool        direction;    
    TreeNode    *curr, *parent;
    
    if (pNode == NULL)
        return;    

    parent = NULL;
    curr = self->pRoot;
    while (curr != NULL) {
        parent = curr;    
        rc = self->compare(pNode->pItem, curr->pItem);

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
            pNode->pLeft = curr->pLeft;
            pNode->pRight = curr->pRight;
            pNode->pParent = curr->pParent;
            
            if (curr->pLeft != NULL)
                curr->pLeft->pParent = pNode;
            
            if (curr->pRight != NULL)            
                curr->pRight->pParent = pNode;
            
            if (curr->pParent != NULL) {
                if (curr == curr->pParent->pLeft)
                    curr->pParent->pLeft = pNode;
                else
                    curr->pParent->pRight = pNode;            
            } else
                self->pRoot = pNode;

            self->destroy(curr->pItem);
            free(curr);            
            return;             
        }
    }

    /* Arrive at the appropriate location. */
    pNode->pParent = parent;
    if (parent != NULL) {
        if (direction == DIRECTION_LEFT)
            parent->pLeft = pNode;
        else
            parent->pRight = pNode;            
    } else
        self->pRoot = pNode;

    return;
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
            
            self->destroy(pNode->pItem);
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

                self->destroy(pNode->pItem);
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
                
                self->destroy(pNode->pItem);
                free(pNode); 
            }        
        } 
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
        rc = self->compare(pItem, curr->pItem);
        
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
