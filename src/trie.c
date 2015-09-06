#include "container/trie.h"


/*===========================================================================*
 *                        The container private data                         *
 *===========================================================================*/
typedef struct _TrieNode {
    bool bEndStr_;
    char cToken_;
    struct _TrieNode *pLeft_;
    struct _TrieNode *pMiddle_;
    struct _TrieNode *pRight_;
    struct _TrieNode *pParent_;
} TrieNode;

struct _TrieData {
    int32_t iSize_;
    int32_t iCountNode_;
    TrieNode *pRoot_;
};

#define DIRECT_LEFT             (0)
#define DIRECT_MIDDLE           (1)
#define DIRECT_RIGHT            (2)
#define INIT_STACK_SIZE(size)   (size >> 2)


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
/**
 * @brief Traverse all the trie nodes and clean the allocated resource.
 *
 * @param pData         The pointer to the trie private data
 */
void _TrieDeinit(TrieData *pData);


#define CHECK_INIT(self)                                                        \
            do {                                                                \
                if (!self)                                                      \
                    return ERR_NOINIT;                                          \
                if (!(self->pData))                                             \
                    return ERR_NOINIT;                                          \
            } while (0);

#define LONGEST_PREFIX_MATCH()                                                  \
            do {                                                                \
                char ch;                                                        \
                while (pCurr && ((ch = *str) != 0)) {                           \
                    pPred = pCurr;                                              \
                    if (ch == pCurr->cToken_) {                                 \
                        pCurr = pCurr->pMiddle_;                                \
                        ++str;                                                  \
                    } else {                                                    \
                        if (ch < pCurr->cToken_)                                \
                            pCurr = pCurr->pLeft_;                              \
                        else                                                    \
                            pCurr = pCurr->pRight_;                             \
                    }                                                           \
                }                                                               \
            } while (0);

#define LONGEST_PREFIX_MATCH_TRACK_DIRECTION()                                  \
            do {                                                                \
                char ch;                                                        \
                while (pCurr && ((ch = *str) != 0)) {                           \
                    pPred = pCurr;                                              \
                    if (ch == pCurr->cToken_) {                                 \
                        pCurr = pCurr->pMiddle_;                                \
                        cDirect = DIRECT_MIDDLE;                                \
                        ++str;                                                  \
                    } else {                                                    \
                        if (ch < pCurr->cToken_) {                              \
                            pCurr = pCurr->pLeft_;                              \
                            cDirect = DIRECT_LEFT;                              \
                        } else {                                                \
                            pCurr = pCurr->pRight_;                             \
                            cDirect = DIRECT_RIGHT;                             \
                        }                                                       \
                    }                                                           \
                }                                                               \
            } while (0);


/*===========================================================================*
 *               Implementation for the exported operations                  *
 *===========================================================================*/
int32_t TrieInit(Trie **ppObj)
{
    *ppObj = (Trie*)malloc(sizeof(Trie));
    if (!(*ppObj)) {
        *ppObj = NULL;
        return ERR_NOINIT;
    }

    Trie *pObj = *ppObj;
    pObj->pData = (TrieData*)malloc(sizeof(TrieData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOINIT;
    }

    TrieData *pData = pObj->pData;
    pData->iSize_ = pData->iCountNode_ = 0;
    pData->pRoot_ = NULL;

    pObj->insert = TrieInsert;
    pObj->bulk_insert = TrieBulkInsert;
    pObj->has_exact = TrieHasExact;
    pObj->has_prefix_as = TrieHasPrefixAs;
    pObj->get_prefix_as = TrieGetPrefixAs;
    pObj->delete = TrieDelete;
    pObj->size = TrieSize;

    return SUCC;
}

void TrieDeinit(Trie **ppObj)
{
    if (!ppObj)
        goto EXIT;

    Trie *pObj = *ppObj;
    if (!(pObj->pData))
        goto FREE_TRIE;

    TrieData *pData = pObj->pData;
    if (!(pData->pRoot_))
        goto FREE_DATA;

    _TrieDeinit(pData);

FREE_DATA:
    free(pObj->pData);
FREE_TRIE:
    free(*ppObj);
EXIT:
    return;
}

int32_t TrieInsert(Trie *self, char *str)
{
    CHECK_INIT(self);
    if (!str)
        return SUCC;
    if (*str == 0)
        return SUCC;

    TrieData *pData = self->pData;
    TrieNode *pCurr = pData->pRoot_;
    TrieNode *pPred = NULL;
    char cDirect;
    LONGEST_PREFIX_MATCH_TRACK_DIRECTION();

    while (*str) {
        TrieNode *pNew = (TrieNode*)malloc(sizeof(TrieNode));
        if (!pNew)
            return ERR_NOMEM;
        pNew->pMiddle_ = pNew->pLeft_ = pNew->pRight_ = NULL;
        pNew->pParent_ = pPred;
        pNew->cToken_ = *str;
        pNew->bEndStr_ = false;

        if (!pPred)
            pData->pRoot_ = pNew;
        else {
            switch (cDirect) {
                case DIRECT_LEFT:
                    pPred->pLeft_ = pNew;
                    break;
                case DIRECT_MIDDLE:
                    pPred->pMiddle_ = pNew;
                    break;
                case DIRECT_RIGHT:
                    pPred->pRight_ = pNew;
            }
        }
        pPred = pNew;
        cDirect = DIRECT_MIDDLE;
        pData->iCountNode_++;
        ++str;
    }

    if (!(pPred->bEndStr_)) {
        pPred->bEndStr_ = true;
        pData->iSize_++;
    }

    return SUCC;
}

int32_t TrieBulkInsert(Trie *self, char **aStr, int iNum)
{
    CHECK_INIT(self);
    TrieData *pData = self->pData;

    int iIdx;
    for (iIdx = 0 ; iIdx < iNum ; ++iIdx) {
        char *str = aStr[iIdx];
        if (!str)
            continue;
        if (*str == 0)
            continue;

        TrieNode *pCurr = pData->pRoot_;
        TrieNode *pPred = NULL;
        char cDirect;
        LONGEST_PREFIX_MATCH_TRACK_DIRECTION();

        while (*str) {
            TrieNode *pNew = (TrieNode*)malloc(sizeof(TrieNode));
            if (!pNew)
                return ERR_NOMEM;
            pNew->pMiddle_ = pNew->pLeft_ = pNew->pRight_ = NULL;
            pNew->pParent_ = pPred;
            pNew->cToken_ = *str;
            pNew->bEndStr_ = false;

            if (!pPred)
                pData->pRoot_ = pNew;
            else {
                switch (cDirect) {
                    case DIRECT_LEFT:
                        pPred->pLeft_ = pNew;
                        break;
                    case DIRECT_MIDDLE:
                        pPred->pMiddle_ = pNew;
                        break;
                    case DIRECT_RIGHT:
                        pPred->pRight_ = pNew;
                }
            }
            pPred = pNew;
            cDirect = DIRECT_MIDDLE;
            pData->iCountNode_++;
            ++str;
        }

        if (!(pPred->bEndStr_)) {
            pPred->bEndStr_ = true;
            pData->iSize_++;
        }
    }

    return SUCC;
}

int32_t TrieHasExact(Trie *self, char *str)
{
    CHECK_INIT(self);
    if (!str)
        return NOKEY;
    if (*str == 0)
        return NOKEY;

    TrieData *pData = self->pData;
    TrieNode *pCurr = pData->pRoot_;
    TrieNode *pPred = NULL;
    LONGEST_PREFIX_MATCH();

    /* Locate at the exact tail of a certain string. */
    return (pPred && pPred->bEndStr_ && *str == 0)? SUCC : NOKEY;
}

int32_t TrieHasPrefixAs(Trie *self, char *str)
{
    CHECK_INIT(self);
    if (!str)
        return NOKEY;
    if (*str == 0)
        return NOKEY;

    TrieData *pData = self->pData;
    TrieNode *pCurr = pData->pRoot_;
    TrieNode *pPred = NULL;
    LONGEST_PREFIX_MATCH();

    if (*str == 0) { /* Legal prefix */
        if (!pCurr)
            return (pPred && pPred->bEndStr_)? SUCC : NOKEY;
        if (pCurr && pPred && pPred->bEndStr_)
            /* Shortcut if the prefix is just a string. */
            return SUCC;
    } else
        return NOKEY;

    /* Otherwise, the slow iterative preorder traversal is necessary to find any
       node marked as string tail. */
    int32_t iCap = ((pData->iSize_ << 2) >= pData->iCountNode_)? \
                   INIT_STACK_SIZE(pData->iSize_) : \
                   INIT_STACK_SIZE(pData->iCountNode_ >> 1);
    TrieNode **stack = (TrieNode**)malloc(sizeof(TrieNode*) * iCap);
    if (!stack)
        return ERR_NOMEM;

    int32_t iRtn = NOKEY;
    int32_t iTop = 0;
    stack[iTop++] = pCurr;
    while (iTop > 0) {
        pCurr = stack[--iTop];
        if (pCurr->bEndStr_) {
            iRtn = SUCC;
            break;
        }

        if (iTop == iCap) {
            iCap <<= 1;
            TrieNode **extend = (TrieNode**)realloc(stack, iCap);
            if (!extend) {
                iRtn = ERR_NOMEM;
                break;
            }
            stack = extend;
        }

        if (pCurr->pRight_)
            stack[iTop++] = pCurr->pRight_;
        if (pCurr->pMiddle_)
            stack[iTop++] = pCurr->pMiddle_;
        if (pCurr->pLeft_)
            stack[iTop++] = pCurr->pLeft_;
    }
    free(stack);

    return iRtn;
}

int32_t TrieGetPrefixAs(Trie *self, char* str, char ***paStr, int *piNum)
{
    CHECK_INIT(self);
    return SUCC;
}

int32_t TrieDelete(Trie *self, char *str)
{
    CHECK_INIT(self);
    if (!str)
        return NOKEY;
    if (*str == 0)
        return NOKEY;

    TrieData *pData = self->pData;
    TrieNode *pCurr = pData->pRoot_;
    TrieNode *pPred = NULL;
    LONGEST_PREFIX_MATCH();

    /* Locate at the exact tail of a certain string. */
    if (pPred && pPred->bEndStr_ && *str == 0) {
        pPred->bEndStr_ = false;
        pData->iSize_--;
        return SUCC;
    }
    return NOKEY;
}

int32_t TrieSize(Trie *self)
{
    CHECK_INIT(self);
    return self->pData->iSize_;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
void _TrieDeinit(TrieData *pData)
{
    if (pData->pRoot_ == NULL)
        return;

    /* Simulate the stack and apply iterative post-order trie traversal. */
    TrieNode ***stack = (TrieNode***)malloc(sizeof(TrieNode**) * pData->iCountNode_);
    assert(stack != NULL);

    int32_t iSize = 0;
    stack[iSize++] = &(pData->pRoot_);
    while (iSize > 0) {
        TrieNode **ppCurr = stack[iSize - 1];
        TrieNode *pCurr = *ppCurr;
        if (pCurr->pLeft_)
            stack[iSize++] = &(pCurr->pLeft_);
        else if (pCurr->pMiddle_)
            stack[iSize++] = &(pCurr->pMiddle_);
        else if (pCurr->pRight_)
            stack[iSize++] = &(pCurr->pRight_);
        else {
            TrieNode *pParent = pCurr->pParent_;
            if (pParent) {
                if (pCurr == pParent->pLeft_)
                    pParent->pLeft_ = NULL;
                else if (pCurr == pParent->pMiddle_)
                    pParent->pMiddle_ = NULL;
                else
                    pParent->pRight_ = NULL;
            }
            free(pCurr);
            iSize--;
        }
    }

    free(stack);
    return;
}