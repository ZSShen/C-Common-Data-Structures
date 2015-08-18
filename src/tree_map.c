#include "map/tree_map.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
typedef struct _TreeNode {
    Pair *pPair;
    bool bColor;
    struct _TreeNode *pParent;
    struct _TreeNode *pLeft;
    struct _TreeNode *pRight;
} TreeNode;

struct _TreeMapData {
    bool bUserDestroy_;
    int32_t iSize_;
    TreeNode *pRoot_;
    TreeNode *pNull_;
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
 * @brief Traverse all the map entries and clean the allocated resource.
 *
 * If the custom resource clean method is set, it also runs the clean method for
 * each pair.
 *
 * @param pData         The pointer to the map private data
 */
void _TreeMapDeinit(TreeMapData *pData);

/**
 * @brief The default key comparison method.
 *
 * @param keySrc         The pointer to the source key
 * @param keyTge         The pointer to the target key
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

    pObj->pData->bUserDestroy_ = false;
    pObj->pData->iSize_ = 0;
    pObj->pData->pRoot_ = pData->pNull_;
    pObj->pData->pCompare_ = _TreeMapCompare;
    pObj->pData->pDestroy_ = _TreeMapDestroy;

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
    return SUCC;
}

int32_t TreeMapGet(TreeMap *self, Key key, Value *pValue)
{
    if (!pValue)
        return ERR_GET;
    *pValue = NULL;
    CHECK_INIT(self);
    return SUCC;
}

int32_t TreeMapFind(TreeMap *self, Key key)
{
    CHECK_INIT(self);
    return SUCC;
}

int32_t TreeMapRemove(TreeMap *self, Key key)
{
    CHECK_INIT(self);
    return SUCC;
}

int32_t TreeMapSize(TreeMap *self)
{
    CHECK_INIT(self);
    return SUCC;
}

int32_t TreeMapMinimum(TreeMap *self, Pair **ppPair)
{
    CHECK_INIT(self);
    return SUCC;
}

int32_t TreeMapMaximum(TreeMap *self, Pair **ppPair)
{
    CHECK_INIT(self);
    return SUCC;
}

int32_t TreeMapPredecessor(TreeMap *self, Key key, Pair **ppPair)
{
    CHECK_INIT(self);
    return SUCC;
}

int32_t TreeMapSuccessor(TreeMap *self, Key key, Pair **ppPair)
{
    CHECK_INIT(self);
    return SUCC;
}

int32_t TreeMapIterate(TreeMap *self, bool bReset, Pair **ppPair)
{
    CHECK_INIT(self);
    return SUCC;
}

int32_t TreeMapReverseIterate(TreeMap *self, bool bReset, Pair **ppPair)
{
    CHECK_INIT(self);
    return SUCC;
}

int32_t TreeMapSetCompare(TreeMap *self, int32_t (*pFunc) (Key, Key))
{
    CHECK_INIT(self);
    return SUCC;
}

int32_t TreeMapSetDestroy(TreeMap *self, void (*pFunc) (Pair*))
{
    CHECK_INIT(self);
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
            if (pData->bUserDestroy_)
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

int32_t _TreeMapCompare(Key keySrc, Key keyTge)
{
    if (keySrc == keyTge)
        return 0;
    return (keySrc > keyTge)? 1 : (-1);
}

void _TreeMapDestroy(Pair *pPair) { free(pPair); }
