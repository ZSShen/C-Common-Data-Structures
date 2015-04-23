#include "red_black_tree.h"


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
 * @param pCurr         The pointer to the requested node
 *
 * @return Non-NULL     The pointer to the returned node
 * @return NULL         The subtree is empty
 */
BalTreeNode* _BalTreeMaximal(BalTreeNode *pCurr);

/**
 * @brief Return the node with minimal order in the subtree rooted by the
 * requested node.
 *
 * @param pCurr         The pointer to the requested node
 *
 * @return Non-NULL     The pointer to the target node
 * @return NULL         The subtree is empty
 */
BalTreeNode* _BalTreeMinimal(BalTreeNode *pCurr);

/**
 * @brief Return the immediate successor of the requested node.
 *
 * @param pCurr         The pointer to the requested node
 *
 * @return Non-NULL     The pointer to the returned node
 * @return NULL         The immediate successor cannot be found
 */
BalTreeNode* _BalTreeSuccessor(BalTreeNode *pCurr);

/**
 * @brief Return the immediate predecessor of the requested node.
 *
 * @param pCurr         The pointer to the requested node
 *
 * @return Non-NULL     The pointer to the returned node
 * @return NULL         The immediate predecessor cannot be found
 */
BalTreeNode* _BalTreePredecessor(BalTreeNode *pCurr);

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
 *               Implementation for internal functions                       *
 *===========================================================================*/

