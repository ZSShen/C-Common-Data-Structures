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

struct TrieData_ {
    int32_t iSize_;
    int32_t iCountNode_;
    TrieNode *pRoot_;
};

typedef struct StackFrame_ {
    bool bVisit_;
    int32_t iDepth_;
    TrieNode *pTrieNode_;
} StackFrame;


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

#define ESTIMATE_STORAGE_SIZE(_size_blk, _size_trie, _cap_trie)                 \
            do {                                                                \
                _size_blk = ((_size_trie << 2) >= _cap_trie)?                   \
                             (_size_trie >> 2) : (_cap_trie >> 3);              \
            } while (0);

#define LONGEST_PREFIX_MATCH(_str, _node_pred, _node_curr)                      \
            do {                                                                \
                char _ch;                                                       \
                while (_node_curr && ((_ch = *_str) != 0)) {                    \
                    _node_pred = _node_curr;                                    \
                    if (_ch == _node_curr->cToken_) {                           \
                        _node_curr = _node_curr->pMiddle_;                      \
                        ++_str;                                                 \
                    } else {                                                    \
                        if (_ch < _node_curr->cToken_)                          \
                            _node_curr = _node_curr->pLeft_;                    \
                        else                                                    \
                            _node_curr = _node_curr->pRight_;                   \
                    }                                                           \
                }                                                               \
            } while (0);

#define LONGEST_PREFIX_MATCH_TRACK_FLOW(_str, _node_pred, _node_curr, _direct)  \
            do {                                                                \
                char _ch;                                                       \
                while (_node_curr && ((_ch = *_str) != 0)) {                    \
                    _node_pred = _node_curr;                                    \
                    if (_ch == _node_curr->cToken_) {                           \
                        _node_curr = _node_curr->pMiddle_;                      \
                        _direct = DIRECT_MIDDLE;                                \
                        ++_str;                                                 \
                    } else {                                                    \
                        if (_ch < _node_curr->cToken_) {                        \
                            _node_curr = _node_curr->pLeft_;                    \
                            _direct = DIRECT_LEFT;                              \
                        } else {                                                \
                            _node_curr = _node_curr->pRight_;                   \
                            _direct = DIRECT_RIGHT;                             \
                        }                                                       \
                    }                                                           \
                }                                                               \
            } while (0);

#define MALLOC_DATA_ARRAY(_ptr_arr, _cap_arr, _rtn, _label_exit)                \
            do {                                                                \
                *_ptr_arr = (char**)malloc(sizeof(char*) * _cap_arr);           \
                if (!(*_ptr_arr)) {                                             \
                    _rtn = ERR_NOMEM;                                           \
                    goto _label_exit;                                           \
                }                                                               \
            } while (0);

#define MALLOC_BLOCK(_ptr_blk, _cap_blk, TYPE, _rtn, _label_exit, ...)          \
            do {                                                                \
                _ptr_blk = (TYPE*)malloc(sizeof(TYPE) * _cap_blk);              \
                if (!(_ptr_blk)) {                                              \
                    _rtn = ERR_NOMEM;                                           \
                    __VA_ARGS__;                                                \
                    goto _label_exit;                                           \
                }                                                               \
            } while (0);

#define REALLOC_BLOCK(_ptr_blk, _cap_blk, TYPE, _rtn, _label_exit, ...)         \
            do {                                                                \
                TYPE *_ptr_blk_new = (TYPE*)realloc(_ptr_blk,                   \
                                     sizeof(TYPE) *_cap_blk);                   \
                if (!_ptr_blk_new) {                                            \
                    _rtn = ERR_NOMEM;                                           \
                    __VA_ARGS__;                                                \
                    goto _label_exit;                                           \
                }                                                               \
                _ptr_blk = _ptr_blk_new;                                        \
            } while (0);

#define FREE_BLOCK(_ptr_blk, _size_blk)                                         \
            do {                                                                \
                int32_t _idx;                                                   \
                for (_idx = 0 ; _idx < _size_blk ; ++_idx)                      \
                    free((_ptr_blk)[_idx]);                                     \
                free(_ptr_blk);                                                 \
                _ptr_blk = NULL;                                                \
            } while (0);

#define COLLECT_PREFIX(_prefix, _len, _ptr_arr, _size_arr, _cap_arr, _rtn,      \
                       _label_exit)                                             \
            do {                                                                \
                char *_cand;                                                    \
                MALLOC_BLOCK(_cand, (_len + 1), char, _rtn, _label_exit,        \
                            FREE_BLOCK(*_ptr_arr, _size_arr));                  \
                strncpy(_cand, _prefix, _len);                                  \
                _cand[_len] = 0;                                                \
                                                                                \
                if (_size_arr < _cap_arr) {                                     \
                    (*_ptr_arr)[_size_arr++] = _cand;                           \
                    break;                                                      \
                }                                                               \
                                                                                \
                int32_t _cap_arr_new = _cap_arr << 1;                           \
                REALLOC_BLOCK(*_ptr_arr, _cap_arr_new, char*, _rtn,             \
                             _label_exit, FREE_BLOCK(*_ptr_arr, _size_arr));    \
                _cap_arr = _cap_arr_new;                                        \
                (*_ptr_arr)[_size_arr++] = _cand;                               \
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

    int32_t iRtn = SUCC;
    TrieData *pData = self->pData;
    TrieNode *pCurr = pData->pRoot_;
    TrieNode *pPred = NULL;
    char cDirect;
    LONGEST_PREFIX_MATCH_TRACK_FLOW(str, pPred, pCurr, cDirect);

    while (*str) {
        TrieNode *pNew;
        MALLOC_BLOCK(pNew, 1, TrieNode, iRtn, EXIT);
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

EXIT:
    return iRtn;
}

int32_t TrieBulkInsert(Trie *self, char **aStr, int iNum)
{
    CHECK_INIT(self);
    TrieData *pData = self->pData;

    int iRtn = SUCC, iIdx;
    for (iIdx = 0 ; iIdx < iNum ; ++iIdx) {
        char *str = aStr[iIdx];
        if (!str)
            continue;
        if (*str == 0)
            continue;

        TrieNode *pCurr = pData->pRoot_;
        TrieNode *pPred = NULL;
        char cDirect;
        LONGEST_PREFIX_MATCH_TRACK_FLOW(str, pPred, pCurr, cDirect);

        while (*str) {
            TrieNode *pNew;
            MALLOC_BLOCK(pNew, 1, TrieNode, iRtn, EXIT);
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

EXIT:
    return iRtn;
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

    /* At the tail of a certain string. */
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

    /* The designated prefix is legal. */
    if (*str == 0) {
        if (!pCurr)
            return (pPred && pPred->bEndStr_)? SUCC : NOKEY;
        /* Short cut if this prefix is a certain string stored in the trie. */
        if (pCurr && pPred && pPred->bEndStr_)
            return SUCC;
    } else
        return NOKEY;

    /* The slow trie traversal to find any node marked as string tail. */
    int32_t iRtn, iCap;
    ESTIMATE_STORAGE_SIZE(iCap, pData->iSize_, pData->iCountNode_);
    TrieNode **stack;
    MALLOC_BLOCK(stack, iCap, TrieNode*, iRtn, EXIT);

    iRtn = NOKEY;
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
            REALLOC_BLOCK(stack, iCap, TrieNode*, iRtn, FREE_STACK);
        }

        if (pCurr->pRight_)
            stack[iTop++] = pCurr->pRight_;
        if (pCurr->pMiddle_)
            stack[iTop++] = pCurr->pMiddle_;
        if (pCurr->pLeft_)
            stack[iTop++] = pCurr->pLeft_;
    }

FREE_STACK:
    free(stack);
EXIT:
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

    /* Prepare the prefix path record which is extended or shrunk during trie
       traversal to represent a certain string stored in the trie. */
    int32_t iRtn;
    int32_t iLenPrefix = strlen(str);
    int32_t iCapPrefix = iLenPrefix << 1;
    char *szPrefix;
    MALLOC_BLOCK(szPrefix, iCapPrefix, char, iRtn, EXIT);
    memset(szPrefix, 0, sizeof(char) * iCapPrefix);
    strcpy(szPrefix, str);

    TrieData *pData = self->pData;
    TrieNode *pCurr = pData->pRoot_;
    TrieNode *pPred = NULL;
    LONGEST_PREFIX_MATCH(str, pPred, pCurr);

    int32_t iSizeArr = 0, iCapArr;
    if (*str == 0) {
        /* The designated prefix is legal, and we try to search for the strings
           matching it. */
        if (!pCurr && !(pPred->bEndStr_)) {
            iRtn = NOKEY;
            goto FREE_PREFIX;
        }

        ESTIMATE_STORAGE_SIZE(iCapArr, pData->iSize_, pData->iCountNode_);
        MALLOC_BLOCK(*paStr, iCapArr, char*, iRtn, FREE_PREFIX);

        if (pPred->bEndStr_)
            COLLECT_PREFIX(szPrefix, iLenPrefix, paStr, iSizeArr, iCapArr, \
                           iRtn, FREE_PREFIX);
    } else {
        iRtn = NOKEY;
        goto FREE_PREFIX;
    }

    /* Prepare the stack for DFS-like traversal. */
    int32_t iTop = 0, iCapStack;
    ESTIMATE_STORAGE_SIZE(iCapStack, pData->iSize_, pData->iCountNode_);
    StackFrame *stack;
    MALLOC_BLOCK(stack, iCapStack, StackFrame, iRtn, FREE_PREFIX, \
                 FREE_BLOCK(*paStr, iSizeArr));

    if (pCurr) {
        stack[iTop].bVisit_ = false;
        stack[iTop].iDepth_ = iLenPrefix;
        stack[iTop++].pTrieNode_ = pCurr;
    }

    while (iTop > 0) {
        StackFrame frame = stack[--iTop];
        int32_t iDepth = frame.iDepth_;
        TrieNode *pCurr = frame.pTrieNode_;

        /* Update the prefix path record only when a certain node is visited in
           the second time. And also collect the record if it represents a certain
           string stored in the trie. */
        if (frame.bVisit_) {
            if (iDepth == iCapPrefix) {
                int32_t iCapPrefixNew = iCapPrefix << 1;
                REALLOC_BLOCK(szPrefix, iCapPrefixNew, char, iRtn, FREE_STACK, \
                              FREE_BLOCK(*paStr, iSizeArr));
                iCapPrefix = iCapPrefixNew;
            }
            szPrefix[iDepth] = pCurr->cToken_;
            if (pCurr->bEndStr_)
                COLLECT_PREFIX(szPrefix, iDepth + 1, paStr, iSizeArr, iCapArr, \
                               iRtn, FREE_STACK);
            continue;
        }

        if (iTop == iCapStack) {
            int32_t iCapStackNew = iCapStack << 1;
            REALLOC_BLOCK(stack, iCapStackNew, StackFrame, iRtn, FREE_STACK, \
                          FREE_BLOCK(*paStr, iSizeArr));
            iCapStack = iCapStackNew;
        }

        /*--------------------------------------------------------------------*
         * Given the prefix "01":                                             *
         *                    The traversal order should be:                  *
         *          0         (a) The sub-trie rooted by the node containing  *
         *          |             character #1.                               *
         *          1         (b) The node containing character #5.           *
         *          |         (c) The sub-trie rooted by the node containing  *
         *          5             character #2.                               *
         *      __/ | \__     (d) The sub-trie rooted by the node containing  *
         *     /    2    \        character #9.                               *
         *    1    / \    9                                                   *
         *   / \  /___\  / \  Thus the stack pushing order should be:         *
         *  /___\       /___\     (d) -> (c) -> (b) -> (a)                    *
         *--------------------------------------------------------------------*/
        if (pCurr->pRight_) {
            stack[iTop].bVisit_ = false;
            stack[iTop].iDepth_ = iDepth;
            stack[iTop++].pTrieNode_ = pCurr->pRight_;
        }

        if (pCurr->pMiddle_) {
            stack[iTop].bVisit_ = false;
            stack[iTop].iDepth_ = iDepth + 1;
            stack[iTop++].pTrieNode_ = pCurr->pMiddle_;
        }

        stack[iTop].bVisit_ = true;
        stack[iTop].iDepth_ = iDepth;
        stack[iTop++].pTrieNode_ = pCurr;

        if (pCurr->pLeft_) {
            stack[iTop].bVisit_ = false;
            stack[iTop].iDepth_ = iDepth;
            stack[iTop++].pTrieNode_ = pCurr->pLeft_;
        }
    }

    /* Arrange the returned data. */
    if (iSizeArr > 0) {
        REALLOC_BLOCK(*paStr, iSizeArr, char*, iRtn, FREE_STACK, \
                      FREE_BLOCK(*paStr, iSizeArr));
        iRtn = SUCC;
    } else {
        free(*paStr);
        *paStr = NULL;
        iRtn = NOKEY;
    }
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

    /* At the tail of a certain string. */
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