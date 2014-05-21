#include "red_black_tree.h"

/*===========================================================================*
 *                  Definition for internal functions                        *
 *===========================================================================*/
#define NODE_COLOR_RED      0
#define NODE_COLOR_BLACK    1

#define DIRECTION_LEFT      0
#define DIRECTION_RIGHT     1

/**
 * This function traverses all the nodes and releases the allocated memory space.
 *
 * @param pNull     The pointer to the dummy node.
 * @param curr      The pointer to the node currently traversed.
 * @param func      The pointer to the function which describes the memory deallocation strategy.
 */
void _RBTreeDeinit(RedBlackNode *pNull, RedBlackNode *curr, void (*func) (void*));


/**
 * This function makes the left rotation for the designated node "curr".
 * After rotation, "curr" will be the left child of its original right child.
 *
 * @param self      The pointer to the RedBlackTree structure.
 * @param curr      The pointer to the node which is to be rotated.
 */
void _RBTreeLeftRotate(RedBlackTree *self, RedBlackNode *curr);


/**
 * This function makes the right rotation for the designated node "curr".
 * After rotation, "curr" will be the right child of its original left child.
 *
 * @param self      The pointer to the RedBlackTree structure.
 * @param curr      The pointer to the node which is to be rotated.
 */
void _RBTreeRightRotate(RedBlackTree *self, RedBlackNode *curr);


/**
 * This function adjusts the tree structure to maintain the red black attributes.
 *
 * @param self      The pointer to the RedBlackTree structure.
 * @param curr      The pointer to the node which is the origin for cascading adjustments.
 */
void _RBTreeInsertFixup(RedBlackTree *self, RedBlackNode *curr);


/**
 * This function locates the node containing minimal key within the subtree rooted by the
 * specified node.
 *
 * @param curr      The pointer to the root node for minimal key traversal.
 *
 * @return          Non-Sentinel: The pointer to the target node.
 *                  Sentinel    : The subtree is empty.
 */
RedBlackNode* _RBTreeMinimal(RedBlackNode *curr);


/**
 * This function locates the node containing maximal key within the subtree rooted by the
 * specified node.
 *
 * @param curr      The pointer to the root node for maximal key traversal.
 *
 * @return          Non-Sentinel: The pointer to the target node.
 *                  Sentinel    : The subtree is empty.
 */
ReeBlackNode* _BSTreeMaximal(RedBlackNode *curr);


/**
 * This function locates the successor of the specified node.
 *
 * @param curr      The pointer to the node for successor search.
 *
 * @return          Non-Sentinel: The pointer to the target node.
 *                  Sentinel    : The size of the tree is less than two.
 */
RedBlackNode* _RBTreeSuccessor(RedBlackNode *curr);


/**
 * This function locates the predecessor of the specified node.
 *
 * @param curr      The pointer to the node for predecessor search.
 *
 * @return          Non-Sentinel: The pointer to the target node.
 *                  Sentinel    : The size of the tree is less than two.
 */
RedBlackNode* _RBTreePredecessor(RedBlackNode *curr);


/*===========================================================================*
 *               Implementation for exported functions                       *
 *===========================================================================*/
void RBTreeInit(RedBlackTree *self) {
    
    /* Create a dummy node representing the "NULL" pointer of the red black tree. */
    self->pNull = (RedBlackNode*)malloc(sizeof(RedBlackNode));
    self->pNull->bColor = NODE_COLOR_BLACK;
    self->pNull->pItem = NULL;
    self->pNull->pParent = self->pNull;
    self->pNull->pRight = self->pNull;
    self->pNull->pLeft = self->pNull;

    /* Let the initial root point to the dummy node. */
    self->pRoot = self->pNull;

    /* Let the function pointers pointing to the appropriate functions. */
    self->compare = RBTreeNodeCompare;
    self->destroy = RBTreeNodeDestroy;

    self->insert = RBTreeInsert;

    self->maximum = RBTreeMaximum;
    self->minimum = RBTreeMinimum;
    self->successor = RBTreeSuccessor;
    self->predecessor = RBTreePredecessor;
    
    return;
}


void RBTreeDeinit(RedBlackTree *self) {

    _RBTreeDeinit(self->pNull, self->pRoot, self->destroy);
    free(self->pNull);
    return;
}


/**
 * RBTreeNodeCompare(): The default comparison strategy for red black node comparison.
 * Note: It considers source and target items as primitive data and compares their values directly.
 */
int RBTreeNodeCompare(const void *pSrc, const void *pTge) {
    
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
 * RBTreeNodeDestroy(): The default strategy for item deallocation.
 * Note: By default, the item is a primitive data, and thus no further operations are required.
 */
void RBTreeNodeDestroy(void *pItem) {

    return;
}


/*
 * RBTreeMinimum(): Locate the node containing the minimum key of the tree.
 */
RedBlackNode* BSTreeMinimum(RedBlackTree *self) {
    RedBlackNode *tge;

    tge = _RBTreeMinimal(self->pRoot);
    if (tge == self->pNull)
        tge = NULL;
    
    return tge;
}


/*
 * RBTreeMaximum(): Locate the node containing the maximum key of the tree.
 */
RedBlackNode* RBTreeMaximum(RedBlackTree *self) {
    RedBlackNode *tge;
    
    tge = _RBTreeMaximal(self->pRoot);
    if (tge == self->pNull)
        tge = NULL;
    
    return tge;
}


/*
 * BSTreeSuccessor(): Locate the successor of the designated node.
 */
RedBlackNode* RBTreeSuccessor(RedBlackTree *self, RedBlackNode *pCurNode) {
    RedBlackNode *tge;
    
    tge = _RBTreeSuccessor(pCurNode);
    if (tge == self->pNull)
        tge = NULL;
    
    return tge;    
}


/*
 * BSTreePredecessor(): Locate the predecessor of the designated node.
 */
RedBlackNode* RBTreePredecessor(RedBlackTree *self, RedBlackNode *pCurNode) {
    RedBlacknode *tge;
    
    tge = _RBTreePredecessor(pCurNode);
    if (tge == self->pNull)
        tge = NULL;
    
    return tge;
}


/**
 * BSTreeInsert(): Insert the new red black node to the appropriate location of the tree.
 * Note: Tree structure adjustment is necessary to maintain the attributes of red black tree.
 */
RedBlackNode* RBTreeInsert(RedBlackTree *self, void *pItem) {
    int             rc;    
    bool            direction;
    RedBlackNode    *new, *curr, *parent;

    /* Allocate the space for the new node. */
    new = (RedBlackNode*)malloc(sizeof(RedBlackNode));
    if (new == NULL)
        return new;

    new->pItem = pItem;
    new->bColor = NODE_COLOR_RED;
    new->pLeft = self->pNull;
    new->pRight = self->pNull;

    /* Search for the appropriate location for the new node. */
    curr = self->pRoot;
    parent = self->pNull;
    while (curr != self->pNull) {
        parent = curr;        
        rc = self->compare(pItem, curr->pItem);

        if (rc == 1) {
            curr = curr->pRight;
            direction = DIRECTION_RIGHT;        
        }        
        else if (rc == -1) {
            curr = curr->pLeft;
            direction = DIRECTION_LEFT;        
        }
        else {
            /* The conflict occurs. The new node will replace the existing one. */
            new->pLeft = curr->pLeft;
            new->pRight = curr->pRight;
            new->pParent = curr->pParent;

            if (curr->pLeft != self->pNull)
                curr->pLeft->pParent = new;

            if (curr->pRight != self->pNull)
                curr->pRight->pParent = new;

            if (curr->pParent != self->pNull) {
                if (curr == curr->pParent->pLeft)
                    curr->pParent->pLeft = new;
                else
                    curr->pParent->pRight = new;            
            } else
                self->pRoot = new;

            self->destroy(curr->pItem);
            free(curr);
            return new;
        }
    }

    /* Arrivate at the appropriate location. */
    new->pParent = parent;
    if (parent != self->pNull) {
        if (direction == DIRECTION_LEFT)
            parent->pLeft = new;
        else      
            parent->pRight = new;
    } else
        self->pRoot = new;

    /* Maintain the attributes of red black tree. */
    _RBTreeInsertFixup(self, new);

    return new;
}


/*===========================================================================*
 *               Implementation for internal functions                       *
 *===========================================================================*/
void _RBTreeDeinit(RedBlackNode *pNull, RedBlackNode *curr, void (*func) (void*)) {

    if (curr != pNull) {
        _RBTreeDeinit(pNull, curr->pLeft, func);
        _RBTreeDeinit(pNull, curr->pRight, func);
        func(curr->pItem);
        free(curr);
    }

    return;
}


void _RBTreeLeftRotate(RedBlackTree *self, RedBlackNode *curr) {
    RedBlackNode *child;

    child = curr->pRight;

    /* Let "curr" handle the left child of its original right child, and
       the left child should point to "curr" if it is not dummy node. */    
    curr->pRight = child->pLeft;
    if (child->pLeft != self->pNull)
        child->pLeft->pParent = curr;

    /* Let "curr"'s original right child point to "curr"'s parent,
       and "curr"'s parent should point to the original right child if the parent is not dummy node. */
    child->pParent = curr->pParent;
    if (curr->pParent != self->pNull) {
        if (curr == curr->pParent->pLeft)
            curr->pParent->pLeft = child;
        else
            curr->pParent->pRight = child;
    } else {
        self->pRoot = child;
    }

    /* Adjust the direct relation between "curr" and its original right child. */
    curr->pParent = child;
    child->pLeft = curr;

    return;
}


void _RBTreeRightRotate(RedBlackTree *self, RedBlackNode *curr) {
    RedBlackNode *child;

    child = curr->pLeft;

    /* Let "curr" handle the right child of its original left child, 
       and the right child should point to "curr" if it is not dummy node. */
    curr->pLeft = child->pRight;
    if (child->pRight != self->pNull)
        child->pRight->pParent = child;

    /* Let "curr"'s original left child point to "curr"'s parent,
       and "curr"'s parent should point to the original left child if it is not dummy node. */
    child->pParent = curr->pParent;
    if (curr->pParent != self->pNull) {
        if (curr == curr->pParent->pLeft)
            curr->pParent->pLeft = child;
        else
            curr->pParent->pRight = child;
    } else {
        self->pRoot = child;
    }

    /* Adjust the direction relation between "curr" and its original left child. */
    curr->pParent = child;
    child->pRight = curr;

    return;
}


void _RBTreeInsertFixup(RedBlackTree *self, RedBlackNode *curr) {
    RedBlackNode *uncle;    
    
    while (curr->pParent->bColor == NODE_COLOR_RED) {
        /* "curr"'s parent is its grand parent's left child. */        
        if (curr->pParent == curr->pParent->pParent->pLeft) {
            uncle = curr->pParent->pParent->pRight;            
        
            /* Case 1: The color of uncle node is also red. */            
            if (uncle->bColor == NODE_COLOR_RED) {
                curr->pParent->bColor = NODE_COLOR_BLACK;
                uncle->bColor = NODE_COLOR_BLACK;
                curr->pParent->pParent->bColor = NODE_COLOR_RED;                
                curr = curr->pParent->pParent;
            } else {
                /* Case 2: The color of uncle node is black, and "curr" is its parent's right child. */
                if (curr == curr->pParent->pRight) {
                    curr = curr->pParent;
                    _RBTreeLeftRotate(self, curr);                       
                }
                /* Case 3: The color of uncle node is black, and "curr" is its parent's left child. */
                curr->pParent->bColor = NODE_COLOR_BLACK;
                curr->pParent->pParent->bColor = NODE_COLOR_RED;
                _RBTreeRightRotate(self, curr->pParent->pParent);
            }
        } 
        /* "curr"'s parent is its grand parent's right child. */
        else {
            uncle = curr->pParent->pParent->pLeft;

            /* Case 1: The color of uncle node is also red. */    
            if (uncle->bColor == NODE_COLOR_RED) {
                curr->pParent->bColor = NODE_COLOR_BLACK;
                uncle->bColor = NODE_COLOR_BLACK;
                curr->pParent->pParent->bColor = NODE_COLOR_RED;
                curr = curr->pParent->pParent;

            } else {
                /* Case 2: The color of uncle node is black, and "curr" is its parent's left child. */
                if (curr == curr->pParent->pLeft) {
                    curr = curr->pParent;
                    _RBTreeRightRotate(self, curr);
                }
                /* Case 3: The color of uncle node is black, and "curr" is its parent's right child. */
                curr->pParent->bColor = NODE_COLOR_BLACK;
                curr->pParent->pParent->bColor = NODE_COLOR_RED;
                _RBTreeLeftRotate(self, curr->pParent->pParent);
            }
        }
    }

    self->pRoot->bColor = NODE_COLOR_BLACK;
    return;
}


RedBlackNode* _RBTreeMinimal(RedBlackTree *self, RedBlackNode *curr) {
    RedBlackNode *parent;
    
    parent = self->pNull;
    while (curr != self->pNull) {
        parent = curr;
        curr = curr->pLeft;
    }
    
    return parent;
}


RedBlackNode* _RBTreeMaximal(RedBlackTree *self, RedBlackNode *curr) {
    RedBlackNode *parent;
    
    parent = self->pNull;
    while (curr != self->pNull) {
        parent = curr;
        curr = curr->pRight;
    }

    return parent;
}


RedBlacknode* _RBTreePredecessor(RedBlackTree *self, RedBlackNode *curr) {

    if (curr != self->pNull) {
        /* Case 1: The maximal node of the non-empty left subtree. */
        if (curr->pLeft != self->pNull)
            curr = _BSTreeMaximal(curr->pLeft);
        
        /* Case 2: The ancestor which considers the designated node as the minimal node of 
           its right subtree. */
        else {
            while ((curr->pParent != self->pNull) && (curr == curr->pParent->pLeft)) 
                curr = curr->pParent;
            curr = curr->pParent;
        }
    }

    return curr;
}


RedBlackNode* _RBTreeSuccessor(RedBlackTree *self, RedBlackNode *curr) {

    if (curr != self->pNull) {
        /* Case 1: The minimal node of the non-empty right subtree. */
        if (curr->pRight != self->pNull)
            curr = _RBTreeMinimal(curr->pRight);
    
        /* Case 2: The ancestor which considers the designated node as the maximal node of
           its left subtree. */
        else {
            while ((curr->pParent != self->pNull) && (curr == curr->pParent->pRight))
                curr = curr->pParent;
            curr = curr->pParent;
        }
    }

    return curr;
}