/**
 *   The MIT License (MIT)
 *   Copyright (C) 2016 ZongXian Shen <andy.zsshen@gmail.com>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a
 *   copy of this software and associated documentation files (the "Software"),
 *   to deal in the Software without restriction, including without limitation
 *   the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons to whom the
 *   Software is furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *   IN THE SOFTWARE.
 */

#include "container/tree_map.h"


/*===========================================================================*
 *                        The container private data                         *
 *===========================================================================*/
static const char DIRECT_LEFT = 0;
static const char DIRECT_RIGHT = 1;

static const char COLOR_RED = 0;
static const char COLOR_BLACK = 1;

static const char STOP = 0;
static const char DOWN_LEFT = 1;
static const char DOWN_RIGHT = 2;
static const char UP_LEFT = 3;
static const char UP_RIGHT = 4;


typedef struct _TreeNode {
    char color_;
    Pair pair_;
    struct _TreeNode* parent_;
    struct _TreeNode* left_;
    struct _TreeNode* right_;
} TreeNode;

struct _TreeMapData {
    char iter_direct_;
    int size_;
    TreeNode* root_;
    TreeNode* null_;
    TreeNode* iter_node_;
    TreeMapCompare func_cmp_;
    TreeMapCleanKey func_clean_key_;
    TreeMapCleanValue func_clean_val_;
};


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

/**
 * @brief Traverse all the tree nodes and clean the allocated resource.
 *
 * @param data          The pointer to the map private data
 */
void _TreeMapDeinit(TreeMapData* data);

/**
 * @brief Return the node having the maximal order in the subtree rooted by the
 * designated node. The node order is determined by its stored key.
 *
 * @param curr          The pointer to the designated node
 *
 * @retval node         The target node
 * @retval null         The subtree is empty
 */
TreeNode* _TreeMapMaximal(TreeNode* null, TreeNode* curr);

/**
 * @brief Return the node having the minimal order in the subtree rooted by the
 * designated node. The node order is determined by its stored key.
 *
 * @param curr          The pointer to the designated node
 *
 * @retval node         The target node
 * @retval null         The subtree is empty
 */
TreeNode* _TreeMapMinimal(TreeNode* null, TreeNode* curr);

/**
 * @brief Return the immediate successor of the designated node.
 *
 * @param curr          The pointer to the designated node
 *
 * @retval node         The target node
 * @retval null         The tree is empty
 */
TreeNode* _TreeMapSuccessor(TreeNode* null, TreeNode* curr);

/**
 * @brief Return the immediate predecessor of the designated node.
 *
 * @param curr          The pointer to the designated node
 *
 * @retval node         The target node
 * @retval null         The tree is empty
 */
TreeNode* _TreeMapPredecessor(TreeNode* null, TreeNode* curr);

/**
 * @brief Make right rotation for the subtree rooted by the designated node.
 *
 * After rotation, the designated node will be the right child of its original
 * left child.
 *
 * @param data          The pointer to the tree private data
 * @param curr          The pointer to the designated node
 */
void _TreeMapRightRotate(TreeMapData* data, TreeNode* curr);

/**
 * @brief Make left rotation for the subtree rooted by the designated node.
 *
 * After rotation, the designated node will be the left child of its original
 * right child.
 *
 * @param data          The pointer to the tree private data
 * @param curr          The pointer to the designated node
 */
void _TreeMapLeftRotate(TreeMapData* data, TreeNode* curr);

/**
 * @brief Maintain the red black tree property after node insertion.
 *
 * @param data          The pointer to the tree private data
 * @param curr          The pointer to the designated node
 */
void _TreeMapInsertFixup(TreeMapData* data, TreeNode* curr);

/**
 * @brief Maintain the red black tree property after node deletion.
 *
 * @param data          The pointer to the tree private data
 * @param curr          The pointer to the designated node
 */
void _TreeMapDeleteFixup(TreeMapData* data, TreeNode* curr);

/**
 * @brief Get the node which stores the key having the same order with the
 * designated one.
 *
 * @param data          The pointer to tree private data
 * @param key           The designated key
 *
 * @retval node         The target node
 * @retval NULL         The key cannot be found
 */
TreeNode* _TreeMapSearch(TreeMapData* data, void* key);

/**
 * @brief The default hash key comparison function.
 *
 * @param lhs           The source key
 * @param rhs           The target key
 *
 * @retval  1           The source key should go after the target one.
 * @retval  0           The source key is equal to the target one.
 * @retval -1           The source key should go before the target one.
 */
int _TreeMapCompare(void* lhs, void* rhs);


/*===========================================================================*
 *               Implementation for the exported operations                  *
 *===========================================================================*/
TreeMap* TreeMapInit()
{
    TreeMap* obj = (TreeMap*)malloc(sizeof(TreeMap));
    if (unlikely(!obj))
        return NULL;

    TreeMapData* data = (TreeMapData*)malloc(sizeof(TreeMapData));
    if (unlikely(!data)) {
        free(obj);
        return NULL;
    }

    /* Create the dummy node representing the NULL pointer of the tree. */
    TreeNode* null = (TreeNode*)malloc(sizeof(TreeNode));
    if (unlikely(!null)) {
        free(data);
        free(obj);
        return NULL;
    }

    null->color_ = COLOR_BLACK;
    null->parent_ = NULL;
    null->parent_ = null;
    null->right_ = null;
    null->left_ = null;

    data->size_ = 0;
    data->null_ = null;
    data->root_ = null;
    data->func_cmp_ = _TreeMapCompare;
    data->func_clean_key_ = NULL;
    data->func_clean_val_ = NULL;

    obj->data = data;
    obj->put = TreeMapPut;
    obj->get = TreeMapGet;
    obj->find = TreeMapFind;
    obj->remove = TreeMapRemove;
    obj->size = TreeMapSize;
    obj->minimum = TreeMapMinimum;
    obj->maximum = TreeMapMaximum;
    obj->predecessor = TreeMapPredecessor;
    obj->successor = TreeMapSuccessor;
    obj->first = TreeMapFirst;
    obj->next = TreeMapNext;
    obj->reverse_next = TreeMapReverseNext;
    obj->set_compare = TreeMapSetCompare;
    obj->set_clean_key = TreeMapSetCleanKey;
    obj->set_clean_value = TreeMapSetCleanValue;

    return obj;
}

void TreeMapDeinit(TreeMap* obj)
{
    if (unlikely(!obj))
        return;

    TreeMapData* data = obj->data;
    _TreeMapDeinit(data);
    free(data->null_);
    free(data);
    free(obj);
    return;
}

bool TreeMapPut(TreeMap* self, void* key, void* value)
{
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (unlikely(!node))
        return false;

    TreeMapData* data = self->data;
    TreeNode* null = data->null_;
    node->pair_.key = key;
    node->pair_.value = value;
    node->color_ = COLOR_RED;
    node->parent_ = null;
    node->left_ = null;
    node->right_ = null;

    TreeMapCompare func_cmp = data->func_cmp_;
    TreeNode* parent = null;
    TreeNode* curr = data->root_;
    char direct;
    while (curr != null) {
        parent = curr;
        int order = func_cmp(key, curr->pair_.key);
        if (order > 0) {
            curr = curr->right_;
            direct = DIRECT_RIGHT;
        }
        else if (order < 0) {
            curr = curr->left_;
            direct = DIRECT_LEFT;
        }
        else {
            /* Conflict with the already stored key value pair. */
            free(node);
            if (data->func_clean_key_)
                data->func_clean_key_(curr->pair_.key);
            if (data->func_clean_val_)
                data->func_clean_val_(curr->pair_.value);

            curr->pair_.key = key;
            curr->pair_.value = value;
            return true;
        }
    }

    /* Arrive at the proper position. */
    node->parent_ = parent;
    if (parent != null) {
        if (direct == DIRECT_LEFT)
            parent->left_ = node;
        else
            parent->right_ = node;
    } else
        data->root_ = node;

    data->size_++;

    /* Maintain the red black tree structure. */
    _TreeMapInsertFixup(data, node);

    return true;
}

void* TreeMapGet(TreeMap* self, void* key)
{
    TreeNode* node = _TreeMapSearch(self->data, key);
    if (node != self->data->null_)
        return node->pair_.value;
    return NULL;
}

bool TreeMapFind(TreeMap* self, void* key)
{
    TreeNode* node = _TreeMapSearch(self->data, key);
    return (node != self->data->null_)? true : false;
}

bool TreeMapRemove(TreeMap* self, void* key)
{
    TreeMapData* data = self->data;
    TreeNode* null = data->null_;
    TreeNode* curr = _TreeMapSearch(data, key);
    if (curr == null)
        return false;

    TreeNode* child;
    char color;
    /* The specified node has no child. */
    if ((curr->left_ == null) && (curr->right_ == null)) {
        if (curr->parent_ != null) {
            if (curr == curr->parent_->left_)
                curr->parent_->left_ = null;
            else
                curr->parent_->right_ = null;
        } else
            data->root_ = null;

        color = curr->color_;
        child = null;
        child->parent_ = curr->parent_;
        if (data->func_clean_key_)
            data->func_clean_key_(curr->pair_.key);
        if (data->func_clean_val_)
            data->func_clean_val_(curr->pair_.value);
        free(curr);
    } else {
        /* The specified node has two children. */
        if ((curr->left_ != null) && (curr->right_ != null)) {
            TreeNode* succ = _TreeMapSuccessor(null, curr);

            child = succ->left_;
            if (child == null)
                child = succ->right_;

            child->parent_ = succ->parent_;

            if (succ == succ->parent_->left_)
                succ->parent_->left_ = child;
            else
                succ->parent_->right_ = child;

            color = succ->color_;
            if (data->func_clean_key_)
                data->func_clean_key_(curr->pair_.key);
            if (data->func_clean_val_)
                data->func_clean_val_(curr->pair_.value);
            curr->pair_.key = succ->pair_.key;
            curr->pair_.value = succ->pair_.value;
            free(succ);
        }
        /* The specified node has one child. */
        else {
            child = curr->left_;
            if (child == null)
                child = curr->right_;

            child->parent_ = curr->parent_;

            if (curr->parent_ != null) {
                if (curr == curr->parent_->left_)
                    curr->parent_->left_ = child;
                else
                    curr->parent_->right_ = child;
            } else
                data->root_ = child;

            color = curr->color_;
            if (data->func_clean_key_)
                data->func_clean_key_(curr->pair_.key);
            if (data->func_clean_val_)
                data->func_clean_val_(curr->pair_.value);
            free(curr);
        }
    }

    /* Decrease the size. */
    data->size_--;

    /* Maintain the balanced tree structure. */
    if (color == COLOR_BLACK)
        _TreeMapDeleteFixup(data, child);

    return true;
}

unsigned TreeMapSize(TreeMap* self)
{
    return self->data->size_;
}

Pair* TreeMapMinimum(TreeMap* self)
{
    TreeNode* node = _TreeMapMinimal(self->data->null_, self->data->root_);
    if (node != self->data->null_)
        return &(node->pair_);
    return NULL;
}

Pair* TreeMapMaximum(TreeMap* self)
{
    TreeNode* node = _TreeMapMaximal(self->data->null_, self->data->root_);
    if (node != self->data->null_)
        return &(node->pair_);
    return NULL;
}

Pair* TreeMapPredecessor(TreeMap* self, void* key)
{
    TreeNode* curr = _TreeMapSearch(self->data, key);
    if (curr == self->data->null_)
        return NULL;

    TreeNode* node = _TreeMapPredecessor(self->data->null_, curr);
    if (node != self->data->null_)
        return &(node->pair_);

    return NULL;
}

Pair* TreeMapSuccessor(TreeMap* self, void* key)
{
    TreeNode* curr = _TreeMapSearch(self->data, key);
    if (curr == self->data->null_)
        return NULL;

    TreeNode* node = _TreeMapSuccessor(self->data->null_, curr);
    if (node != self->data->null_)
        return &(node->pair_);

    return NULL;
}

void TreeMapFirst(TreeMap* self)
{
    self->data->iter_direct_ = DOWN_LEFT;
    self->data->iter_node_ = self->data->root_;
}

Pair* TreeMapNext(TreeMap* self)
{
    char direct = self->data->iter_direct_;
    TreeNode* null = self->data->null_;
    TreeNode* curr = self->data->iter_node_;

    while (curr != null) {
        if (direct == DOWN_LEFT || direct == DOWN_RIGHT) {
            if (curr->left_ != null) {
                curr = curr->left_;
                direct = DOWN_LEFT;
                continue;
            }

            Pair* pair = &(curr->pair_);

            if (curr->right_ != null) {
                self->data->iter_node_ = curr->right_;
                self->data->iter_direct_ = DOWN_RIGHT;
            } else {
                TreeNode* temp = curr;
                curr = curr->parent_;
                self->data->iter_node_ = curr;
                if (curr != null) {
                    if (temp == curr->left_)
                        self->data->iter_direct_ = UP_LEFT;
                    else
                        self->data->iter_direct_ = UP_RIGHT;
                }
            }
            return pair;
        }

        if (direct == UP_LEFT) {
            Pair* pair = &(curr->pair_);

            if (curr->right_ != null) {
                self->data->iter_node_ = curr->right_;
                self->data->iter_direct_ = DOWN_RIGHT;
            } else {
                TreeNode* temp = curr;
                curr = curr->parent_;
                self->data->iter_node_ = curr;
                if (curr != null) {
                    if (temp == curr->left_)
                        self->data->iter_direct_ = UP_LEFT;
                    else
                        self->data->iter_direct_ = UP_RIGHT;
                }
            }
            return pair;
        }

        TreeNode* temp = curr;
        curr = curr->parent_;
        if (curr != null) {
            if (temp == curr->left_)
                direct = UP_LEFT;
            else
                direct = UP_RIGHT;
        }
    }

    self->data->iter_node_ = null;
    return NULL;
}

Pair* TreeMapReverseNext(TreeMap* self)
{
    char direct = self->data->iter_direct_;
    TreeNode* null = self->data->null_;
    TreeNode* curr = self->data->iter_node_;

    while (curr != null) {
        if (direct == DOWN_LEFT || direct == DOWN_RIGHT) {
            if (curr->right_ != null) {
                curr = curr->right_;
                direct = DOWN_RIGHT;
                continue;
            }

            Pair* pair = &(curr->pair_);

            if (curr->left_ != null) {
                self->data->iter_node_ = curr->left_;
                self->data->iter_direct_ = DOWN_LEFT;
            } else {
                TreeNode* temp = curr;
                curr = curr->parent_;
                self->data->iter_node_ = curr;
                if (curr != null) {
                    if (temp == curr->left_)
                        self->data->iter_direct_ = UP_LEFT;
                    else
                        self->data->iter_direct_ = UP_RIGHT;
                }
            }
            return pair;
        }

        if (direct == UP_RIGHT) {
            Pair* pair = &(curr->pair_);

            if (curr->left_ != null) {
                self->data->iter_node_ = curr->left_;
                self->data->iter_direct_ = DOWN_LEFT;
            } else {
                TreeNode* temp = curr;
                curr = curr->parent_;
                self->data->iter_node_ = curr;
                if (curr != null) {
                    if (temp == curr->left_)
                        self->data->iter_direct_ = UP_LEFT;
                    else
                        self->data->iter_direct_ = UP_RIGHT;
                }
            }
            return pair;
        }

        TreeNode* temp = curr;
        curr = curr->parent_;
        if (curr != null) {
            if (temp == curr->left_)
                direct = UP_LEFT;
            else
                direct = UP_RIGHT;
        }
    }

    self->data->iter_node_ = null;
    return NULL;
}

void TreeMapSetCompare(TreeMap* self, TreeMapCompare func)
{
    self->data->func_cmp_ = func;
}

void TreeMapSetCleanKey(TreeMap* self, TreeMapCleanKey func)
{
    self->data->func_clean_key_ = func;
}

void TreeMapSetCleanValue(TreeMap* self, TreeMapCleanValue func)
{
    self->data->func_clean_val_ = func;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
void _TreeMapDeinit(TreeMapData* data)
{
    TreeNode* null = data->null_;
    if (data->root_ == null)
        return;

    TreeMapCleanKey func_clean_key = data->func_clean_key_;
    TreeMapCleanValue func_clean_val = data->func_clean_val_;

    char direct = DOWN_LEFT;
    TreeNode* curr = data->root_;
    while (direct != STOP) {
        if (direct == DOWN_LEFT || direct == DOWN_RIGHT) {
            if (curr->left_ != null) {
                curr = curr->left_;
                direct = DOWN_LEFT;
                continue;
            }
            if (curr->right_ != null) {
                curr = curr->right_;
                direct = DOWN_RIGHT;
                continue;
            }

            TreeNode* temp = curr;
            curr = curr->parent_;
            if (curr == null)
                direct = STOP;
            else
                direct = (temp == curr->left_)? UP_LEFT : UP_RIGHT;

            if (func_clean_key)
                func_clean_key(temp->pair_.key);
            if (func_clean_val)
                func_clean_val(temp->pair_.value);
            free(temp);
            continue;
        }

        if (direct == UP_LEFT) {
            if (curr->right_ != null) {
                curr = curr->right_;
                direct = DOWN_RIGHT;
                continue;
            }

            TreeNode* temp = curr;
            curr = curr->parent_;
            if (curr == null)
                direct = STOP;
            else
                direct = (temp == curr->left_)? UP_LEFT : UP_RIGHT;

            if (func_clean_key)
                func_clean_key(temp->pair_.key);
            if (func_clean_val)
                func_clean_val(temp->pair_.value);
            free(temp);
            continue;
        }

        TreeNode* temp = curr;
        curr = curr->parent_;
        if (curr == null)
            direct = STOP;
        else
            direct = (temp == curr->left_)? UP_LEFT : UP_RIGHT;

        if (func_clean_key)
            func_clean_key(temp->pair_.key);
        if (func_clean_val)
            func_clean_val(temp->pair_.value);
        free(temp);
    }

    return;
}

TreeNode* _TreeMapMaximal(TreeNode* null, TreeNode* curr)
{
    TreeNode* parent = null;
    while (curr != null) {
        parent = curr;
        curr = curr->right_;
    }
    return parent;
}

TreeNode* _TreeMapMinimal(TreeNode* null, TreeNode* curr)
{
    TreeNode* parent = null;
    while (curr != null) {
        parent = curr;
        curr = curr->left_;
    }
    return parent;
}

TreeNode* _TreeMapSuccessor(TreeNode* null, TreeNode* curr)
{
    if (curr != null) {
        /* Case 1: The minimal node in the non-null right subtree. */
        if (curr->right_ != null)
            curr = _TreeMapMinimal(null, curr->right_);

        /* Case 2: The ancestor which considers the designated node as the
           maximal node of its left subtree. */
        else {
            while((curr->parent_ != null) && (curr == curr->parent_->right_))
                curr = curr->parent_;
            curr = curr->parent_;
        }
    }
    return curr;
}

TreeNode* _TreeMapPredecessor(TreeNode* null, TreeNode* curr)
{
    if (curr != null) {
        /* Case 1: The maximal node in the non-null left subtree. */
        if (curr->left_ != null)
            curr = _TreeMapMaximal(null, curr->left_);

        /* Case 2: The ancestor which considers the designated node as the
           minimal node of its right subtree. */
        else {
            while((curr->parent_ != null) && (curr == curr->parent_->left_))
                curr = curr->parent_;
            curr = curr->parent_;
        }
    }
    return curr;
}

void _TreeMapRightRotate(TreeMapData* data, TreeNode* curr)
{
    TreeNode* null = data->null_;
    TreeNode* child = curr->left_;
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
    curr->left_ = child->right_;
    if (child->right_ != null)
        child->right_->parent_ = curr;

    /* Let x link y's parent as its parent.
       If y's parent is not dummy node, let it link x as its child. */
    child->parent_ = curr->parent_;
    if (curr->parent_ != null) {
        if (curr == curr->parent_->left_)
            curr->parent_->left_ = child;
        else
            curr->parent_->right_ = child;
    } else
        data->root_ = child;

    /* Let y link x as its parent.
       And let x link y as its right child. */
    curr->parent_ = child;
    child->right_ = curr;

    return;
}

void _TreeMapLeftRotate(TreeMapData* data, TreeNode* curr)
{
    TreeNode* null = data->null_;
    TreeNode* child = curr->right_;
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
    curr->right_ = child->left_;
    if (child->left_ != null)
        child->left_->parent_ = curr;

    /* Let y link x's parent as its parent.
       If x's parent is not dummy node, let it link y as its child. */
    child->parent_ = curr->parent_;
    if (curr->parent_ != null) {
        if (curr == curr->parent_->left_)
            curr->parent_->left_ = child;
        else
            curr->parent_->right_ = child;
    } else
        data->root_ = child;

    /* Let x link y as its parent.
       And let y link x as its left child. */
    curr->parent_ = child;
    child->left_ = curr;

    return;
}

void _TreeMapInsertFixup(TreeMapData* data, TreeNode* curr)
{
    TreeNode* uncle;

    /* Denote the current node as x. */
    while (curr->parent_->color_ == COLOR_RED) {
        /* x's parent is its grandparent's left child. */
        if (curr->parent_ == curr->parent_->parent_->left_) {
            uncle = curr->parent_->parent_->right_;
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
            if (uncle->color_ == COLOR_RED) {
                curr->parent_->color_ = COLOR_BLACK;
                uncle->color_ = COLOR_BLACK;
                curr->parent_->parent_->color_ = COLOR_RED;
                curr = curr->parent_->parent_;
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
                if (curr == curr->parent_->right_) {
                    curr = curr->parent_;
                    _TreeMapLeftRotate(data, curr);
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
                curr->parent_->color_ = COLOR_BLACK;
                curr->parent_->parent_->color_ = COLOR_RED;
                _TreeMapRightRotate(data, curr->parent_->parent_);
            }
        }
        /* x's parent is its grandparent's right child. */
        else {
            uncle = curr->parent_->parent_->left_;

            /* Case 1: The color of x's uncle is also red. */
            if (uncle->color_ == COLOR_RED) {
                curr->parent_->color_ = COLOR_BLACK;
                uncle->color_ = COLOR_BLACK;
                curr->parent_->parent_->color_ = COLOR_RED;
                curr = curr->parent_->parent_;
            } else {
                /* Case 2: The color of x's uncle is black, and x is its parent's
                   left child. */
                if (curr == curr->parent_->left_) {
                    curr = curr->parent_;
                    _TreeMapRightRotate(data, curr);
                }
                /* Case 3: The color of x's uncle is black, and x is its parent's
                   right child. */
                curr->parent_->color_ = COLOR_BLACK;
                curr->parent_->parent_->color_ = COLOR_RED;
                _TreeMapLeftRotate(data, curr->parent_->parent_);
            }
        }
    }

    data->root_->color_ = COLOR_BLACK;
    return;
}

void _TreeMapDeleteFixup(TreeMapData* data, TreeNode* curr)
{
    TreeNode* brother;

    /* Denote the current node as x. */
    while ((curr != data->root_) && (curr->color_ == COLOR_BLACK)) {
        /* x is its parent's left child. */
        if (curr == curr->parent_->left_) {
            brother = curr->parent_->right_;
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
            if (brother->color_ == COLOR_RED) {
                brother->color_ = COLOR_BLACK;
                curr->parent_->color_ = COLOR_RED;
                _TreeMapLeftRotate(data, curr->parent_);
                brother = curr->parent_->right_;
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
            if ((brother->left_->color_ == COLOR_BLACK) &&
                (brother->right_->color_ == COLOR_BLACK)) {
                brother->color_ = COLOR_RED;
                curr = curr->parent_;
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
                if (brother->right_->color_ == COLOR_BLACK) {
                    brother->left_->color_ = COLOR_BLACK;
                    brother->color_ = COLOR_RED;
                    _TreeMapRightRotate(data, brother);
                    brother = curr->parent_->right_;
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
                brother->color_ = curr->parent_->color_;
                curr->parent_->color_ = COLOR_BLACK;
                brother->right_->color_ = COLOR_BLACK;
                _TreeMapLeftRotate(data, curr->parent_);
                curr = data->root_;
            }
        }
        /* x is its parent's right child */
        else {
            brother = curr->parent_->left_;
            /* Case 1: The color of x's brother is red. */
            if (brother->color_ == COLOR_RED) {
                brother->color_ = COLOR_BLACK;
                curr->parent_->color_ = COLOR_RED;
                _TreeMapRightRotate(data, curr->parent_);
                brother = curr->parent_->left_;
            }
            /* Case 2: The color of x's brother is black, and both of its
               children are also black. */
            if ((brother->left_->color_ == COLOR_BLACK) &&
                (brother->right_->color_ == COLOR_BLACK)) {
                brother->color_ = COLOR_RED;
                curr = curr->parent_;
            } else {
                /* Case 3: The color of x's brother is black and the colors of its
                   right and left child are red and black respectively. */
                if (brother->left_->color_ == COLOR_BLACK) {
                    brother->right_->color_ = COLOR_BLACK;
                    brother->color_ = COLOR_RED;
                    _TreeMapLeftRotate(data, brother);
                    brother = curr->parent_->left_;
                }
                /* Case 4: The color of x's brother is black, and its left child
                   is red. */
                brother->color_ = curr->parent_->color_;
                curr->parent_->color_ = COLOR_BLACK;
                brother->left_->color_ = COLOR_BLACK;
                _TreeMapRightRotate(data, curr->parent_);
                curr = data->root_;
            }
        }
    }

    curr->color_ = COLOR_BLACK;
    return;
}

TreeNode* _TreeMapSearch(TreeMapData* data, void* key)
{
    TreeMapCompare func_cmp = data->func_cmp_;
    TreeNode* null = data->null_;
    TreeNode* curr = data->root_;
    while(curr != null) {
        int order = func_cmp(key, curr->pair_.key);
        if (order == 0)
            break;
        else {
            if (order > 0)
                curr = curr->right_;
            else
                curr = curr->left_;
        }
    }
    return curr;
}

int _TreeMapCompare(void* lhs, void* rhs)
{
    if ((intptr_t)lhs == (intptr_t)rhs)
        return 0;
    return ((intptr_t)lhs >= (intptr_t)rhs)? 1 : (-1);
}
