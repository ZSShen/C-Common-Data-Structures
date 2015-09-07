#include "container/trie.h"


/*===========================================================================*
 *                        The container private data                         *
 *===========================================================================*/
typedef struct TrieNode_ {
    bool bEndStr_;
    char cToken_;
    struct TrieNode_ *pLeft_;
    struct TrieNode_ *pMiddle_;
    struct TrieNode_ *pRight_;
    struct TrieNode_ *pParent_;
} TrieNode;

typedef struct StackFrame_ {
    TrieNode *pTrieNode_;
    int32_t iDepth_;
} StackFrame;

struct TrieData_ {
    int32_t iSize_;
    int32_t iCountNode_;
    TrieNode *pRoot_;
};


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
/**
 * @brief Traverse all the trie nodes and clean the allocated resource.
 *
 * @param pData         The pointer to the trie private data
 */
void _TrieDeinit(TrieData *pData);


#define DIRECT_LEFT             (0)
#define DIRECT_MIDDLE           (1)
#define DIRECT_RIGHT            (2)

#define CHECK_INIT(self)                                                        \
            do {                                                                \
                if (!self)                                                      \
                    return ERR_NOINIT;                                          \
                if (!(self->pData))                                             \
                    return ERR_NOINIT;                                          \
            } while (0);

#define ESTIMATE_STORAGE_SIZE(__sz_store, __sz_trie, __cap_trie)                \
            do {                                                                \
                __sz_store = ((__sz_trie << 2) >= __cap_trie)?                  \
                             __sz_trie >> 2:                                    \
                             __cap_trie >> 3;                                   \
            } while (0);

#define LONGEST_PREFIX_MATCH(__str, __nd_pred, __nd_curr)                       \
            do {                                                                \
                char __ch;                                                      \
                while (__nd_curr && ((__ch = *__str) != 0)) {                   \
                    __nd_pred = __nd_curr;                                      \
                    if (__ch == __nd_curr->cToken_) {                           \
                        __nd_curr = __nd_curr->pMiddle_;                        \
                        ++__str;                                                \
                    } else {                                                    \
                        if (__ch < __nd_curr->cToken_)                          \
                            __nd_curr = __nd_curr->pLeft_;                      \
                        else                                                    \
                            __nd_curr = __nd_curr->pRight_;                     \
                    }                                                           \
                }                                                               \
            } while (0);

#define LONGEST_PREFIX_MATCH_TRACK_DIRECTION(__str, __nd_pred, __nd_curr,       \
                                             __direct)                          \
            do {                                                                \
                char __ch;                                                      \
                while (__nd_curr && ((__ch = *__str) != 0)) {                   \
                    __nd_pred = __nd_curr;                                      \
                    if (__ch == __nd_curr->cToken_) {                           \
                        __nd_curr = __nd_curr->pMiddle_;                        \
                        __direct = DIRECT_MIDDLE;                               \
                        ++__str;                                                \
                    } else {                                                    \
                        if (__ch < __nd_curr->cToken_) {                        \
                            __nd_curr = __nd_curr->pLeft_;                      \
                            __direct = DIRECT_LEFT;                             \
                        } else {                                                \
                            __nd_curr = __nd_curr->pRight_;                     \
                            __direct = DIRECT_RIGHT;                            \
                        }                                                       \
                    }                                                           \
                }                                                               \
            } while (0);

#define MALLOC_DATA_ARRAY(__ptr_arr, __cap_arr, __rtn, __label_exit)            \
            do {                                                                \
                *__ptr_arr = (char**)malloc(sizeof(char*) * __cap_arr);         \
                if (!(*__ptr_arr)) {                                            \
                    __rtn = ERR_NOMEM;                                          \
                    goto __label_exit;                                          \
                }                                                               \
            } while (0);

#define FREE_DATA_ARRAY(__ptr_arr, __sz_arr)                                    \
            do {                                                                \
                char **__arr = *__ptr_arr;                                      \
                int32_t __idx;                                                  \
                for (__idx = 0 ; __idx < __sz_arr ; ++__idx)                    \
                    free(__arr[__idx]);                                         \
                free(*__ptr_arr);                                               \
                *__ptr_arr = NULL;                                              \
            } while (0);

#define COLLECT_PREFIX(__prefix, __len, __ptr_arr, __sz_arr, __cap_arr,         \
                       __rtn, __label_exit)                                     \
            do {                                                                \
                char *__cand = (char*)malloc(sizeof(char) * (__len + 1));       \
                if (!__cand) {                                                  \
                    FREE_DATA_ARRAY(__ptr_arr, __sz_arr);                       \
                    __rtn = ERR_NOMEM;                                          \
                    goto __label_exit;                                          \
                }                                                               \
                strncpy(__cand, __prefix, __len);                               \
                __cand[__len] = 0;                                              \
                                                                                \
                if (__sz_arr < __cap_arr) {                                     \
                    (*__ptr_arr)[__sz_arr++] = __cand;                          \
                    break;                                                      \
                }                                                               \
                                                                                \
                int32_t __cap_arr_new = __cap_arr << 1;                         \
                char **__arr_new = (char**)realloc(*__ptr_arr, __cap_arr_new);  \
                if (!__arr_new) {                                               \
                    FREE_DATA_ARRAY(__ptr_arr, __sz_arr);                       \
                    __rtn = ERR_NOMEM;                                          \
                    goto __label_exit;                                          \
                }                                                               \
                *__ptr_arr = __arr_new;                                         \
                __cap_arr = __cap_arr_new;                                      \
                (*__ptr_arr)[__sz_arr++] = __cand;                              \
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
    LONGEST_PREFIX_MATCH_TRACK_DIRECTION(str, pPred, pCurr, cDirect);

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
        LONGEST_PREFIX_MATCH_TRACK_DIRECTION(str, pPred, pCurr, cDirect);

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
    LONGEST_PREFIX_MATCH(str, pPred, pCurr);

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
    LONGEST_PREFIX_MATCH(str, pPred, pCurr);

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
    int32_t iCap;
    ESTIMATE_STORAGE_SIZE(iCap, pData->iSize_, pData->iCountNode_);
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
    if (!paStr || !piNum)
        return ERR_GET;

    *paStr = NULL;
    *piNum = 0;
    if (!str)
        return NOKEY;
    if (*str == 0)
        return NOKEY;

    /* Prepare the prefix string for trie traversal. */
    int32_t iRtn;
    int32_t iLenPrefix = strlen(str);
    int32_t iCapPrefix = iLenPrefix << 1;
    char *szPrefix = (char*)malloc(sizeof(char) * iCapPrefix);
    if (!szPrefix) {
        iRtn = ERR_NOMEM;
        goto EXIT;
    }
    memset(szPrefix, 0, sizeof(char) * iCapPrefix);
    strcpy(szPrefix, str);

    TrieData *pData = self->pData;
    TrieNode *pCurr = pData->pRoot_;
    TrieNode *pPred = NULL;
    LONGEST_PREFIX_MATCH(str, pPred, pCurr);

    int32_t iSizeArr = 0, iCapArr;

    if (*str == 0) {
        if (!pCurr) {
            if (!(pPred->bEndStr_)) {
                iRtn = NOKEY;
                goto FREE_PREFIX;
            }
            MALLOC_DATA_ARRAY(paStr, 1, iRtn, FREE_PREFIX);
            COLLECT_PREFIX(szPrefix, iLenPrefix, paStr, iSizeArr, iCapArr, \
                           iRtn, FREE_PREFIX);
            *piNum = 1;
            iRtn = SUCC;
            goto FREE_PREFIX;
        }
        if (pPred->bEndStr_) {
            ESTIMATE_STORAGE_SIZE(iCapArr, pData->iSize_, pData->iCountNode_);
            MALLOC_DATA_ARRAY(paStr, iCapArr, iRtn, FREE_PREFIX);
            COLLECT_PREFIX(szPrefix, iLenPrefix, paStr, iSizeArr, iCapArr, \
                           iRtn, FREE_PREFIX);
        }
    } else {
        iRtn = NOKEY;
        goto FREE_PREFIX;
    }

    /* Prepare the stack for iterative preorder traversal which finds all the
       strings matching the designated prefix. */
    int32_t iTop = 0, iCapStack;
    ESTIMATE_STORAGE_SIZE(iCapStack, pData->iSize_, pData->iCountNode_);
    StackFrame *stack = (StackFrame*)malloc(sizeof(StackFrame) * iCapStack);
    if (!stack) {
        FREE_DATA_ARRAY(paStr, iSizeArr);
        goto FREE_PREFIX;
    }
    if (pCurr->pRight_) {
        stack[iTop].pTrieNode_ = pCurr->pRight_;
        stack[iTop++].iDepth_ = iLenPrefix;
    }
    stack[iTop].pTrieNode_ = pCurr;
    stack[iTop++].iDepth_ = iLenPrefix;
    if (pCurr->pLeft_) {
        stack[iTop].pTrieNode_ = pCurr->pLeft_;
        stack[iTop++].iDepth_ = iLenPrefix;
    }

    while (iTop > 0) {
        StackFrame frame = stack[--iTop];
        TrieNode *pCurr = frame.pTrieNode_;
        int32_t iDepth = frame.iDepth_;

        if ((iDepth + 1) == iCapPrefix) {
            int32_t iCapPrefixNew = iCapPrefix << 1;
            char *szPrefixNew = (char*)malloc(sizeof(char) * iCapPrefixNew);
            if (!szPrefixNew) {
                FREE_DATA_ARRAY(paStr, iSizeArr);
                goto FREE_STACK;
            }
            szPrefix = szPrefixNew;
            iCapPrefix = iCapPrefixNew;
        }
        szPrefix[iDepth] = pCurr->cToken_;
        if (pCurr->bEndStr_)
            COLLECT_PREFIX(szPrefix, iDepth, paStr, iSizeArr, iCapArr, \
                           iRtn, FREE_STACK);

        if (iTop == iCapStack) {
            int32_t iCapStackNew = iCapStack << 1;
            StackFrame *stackNew = (StackFrame*)realloc(stack, iCapStackNew);
            if (!stackNew) {
                FREE_DATA_ARRAY(paStr, iSizeArr);
                goto FREE_STACK;
            }
            stack = stackNew;
            iCapStack = iCapStackNew;
        }

        if (pCurr->pRight_) {
            stack[iTop].pTrieNode_ = pCurr->pRight_;
            stack[iTop++].iDepth_ = iDepth + 1;
        }
        if (pCurr->pMiddle_) {
            stack[iTop].pTrieNode_ = pCurr->pMiddle_;
            stack[iTop++].iDepth_ = iDepth + 1;
        }
        if (pCurr->pLeft_) {
            stack[iTop].pTrieNode_ = pCurr->pLeft_;
            stack[iTop++].iDepth_ = iDepth + 1;
        }
    }
    iRtn = SUCC;
    *piNum = iSizeArr;

FREE_STACK:
    free(stack);
FREE_PREFIX:
    free(szPrefix);
EXIT:
    return iRtn;
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
    LONGEST_PREFIX_MATCH(str, pPred, pCurr);

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

    /* Simulate the stack and apply iterative postorder trie traversal. */
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