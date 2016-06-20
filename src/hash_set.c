#include "container/hash_set.h"
#include "math/hash.h"


/*===========================================================================*
 *                        The container private data                         *
 *===========================================================================*/
static const uint32_t aMagicPrimes[] = {
    769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241, 786433,
    1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319,
    201326611, 402653189, 805306457, 1610612741,
};
static const int32_t iCountPrime_ = sizeof(aMagicPrimes) / sizeof(uint32_t);
static const double dLoadFactor_ = 0.75;


typedef struct _SlotNode {
    size_t sizeKey;
    Key key;
    struct _SlotNode *pNext;
} SlotNode;

struct _HashSetData {
    bool bEnd_;
    int32_t iSize_;
    int32_t iIdxPrime_;
    uint32_t uiIterIdx_;
    uint32_t uiCountSlot_;
    SlotNode **aSlot_;
    SlotNode *pIterNode_;
    uint32_t (*pHash_) (Key, size_t);
    void (*pDestroy_) (Key);
};


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
#define CHECK_INIT(self)                                                        \
            do {                                                                \
                if (!self)                                                      \
                    return ERR_NOINIT;                                          \
                if (!(self->pData))                                             \
                    return ERR_NOINIT;                                          \
                if (!(self->pData->aSlot_))                                     \
                    return ERR_NOINIT;                                          \
            } while (0);


/**
 * @brief Initialize the set with the designated slot size.
 *
 * @param ppObj         The double pointer to the to be initialized set
 * @param iIdxPrime     The index to the array of magic primes
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for set construction
 */
int32_t _HashSetInit(HashSet **ppObj, int32_t iIdxPrime);

/**
 * @brief Extend the slot array and re-distribute the stored keys.
 *
 * @param pData         The pointer to the set private data
 */
void _HashSetReHash(HashSetData *pData);


/*===========================================================================*
 *               Implementation for the exported operations                  *
 *===========================================================================*/
int32_t HashSetInit(HashSet **ppObj)
{
    return _HashSetInit(ppObj, 0);
}

void HashSetDeinit(HashSet **ppObj)
{
    if (!(*ppObj))
        goto EXIT;

    HashSet *pObj = *ppObj;
    if (!(pObj->pData))
        goto FREE_MAP;

    HashSetData *pData = pObj->pData;
    if (!(pData->aSlot_))
        goto FREE_DATA;

    uint32_t uiIdx;
    for (uiIdx = 0 ; uiIdx < pData->uiCountSlot_ ; uiIdx++) {
        SlotNode *pPred;
        SlotNode *pCurr = pData->aSlot_[uiIdx];
        while (pCurr) {
            pPred = pCurr;
            pCurr = pCurr->pNext;
            if (pData->pDestroy_)
                pData->pDestroy_(pPred->key);
            free(pPred);
        }
    }
    free(pData->aSlot_);

FREE_DATA:
    free(pObj->pData);
FREE_MAP:
    free(*ppObj);
    *ppObj = NULL;
EXIT:
    return;
}

int32_t HashSetAdd(HashSet *self, Key key, size_t size)
{
    CHECK_INIT(self);
    if (size == 0)
        return ERR_KEYSIZE;

    /* Check the loading factor for rehashing. */
    HashSetData *pData = self->pData;
    double dCurrLoad = (double)pData->iSize_ / pData->uiCountSlot_;
    if (dCurrLoad >= dLoadFactor_)
        _HashSetReHash(pData);

    /* Calculate the slot index. */
    uint32_t uiValue = pData->pHash_(key, size);
    uiValue = uiValue % pData->uiCountSlot_;

    /* Check if the key conflicts with a certain one stored in the set. If yes,
       replace that one. */
    SlotNode **aSlot = pData->aSlot_;
    SlotNode *pCurr = aSlot[uiValue];
    while (pCurr) {
        if ((pCurr->sizeKey == size) &&
            (memcmp(pCurr->key, key, size) == 0)) {
            if (pData->pDestroy_)
                pData->pDestroy_(pCurr->key);
            pCurr->key = key;
            return SUCC;
        }
        pCurr = pCurr->pNext;
    }

    /* Insert the new pair into the slot list. */
    SlotNode *pNew = (SlotNode*)malloc(sizeof(SlotNode));
    pNew->sizeKey = size;
    pNew->key = key;
    if (!(aSlot[uiValue])) {
        pNew->pNext = NULL;
        aSlot[uiValue] = pNew;
    } else {
        pNew->pNext = aSlot[uiValue];
        aSlot[uiValue] = pNew;
    }
    pData->iSize_++;

    return SUCC;
}

int32_t HashSetFind(HashSet *self, Key key, size_t size)
{
    CHECK_INIT(self);
    if (size == 0)
        return ERR_KEYSIZE;

    HashSetData *pData = self->pData;

    /* Calculate the slot index. */
    uint32_t uiValue = pData->pHash_(key, size);
    uiValue = uiValue % pData->uiCountSlot_;

    /* Search for the key identical to the designated one. */
    SlotNode *pCurr = pData->aSlot_[uiValue];
    while (pCurr) {
        if ((pCurr->sizeKey == size) &&
            (memcmp(pCurr->key, key, size) == 0))
            return SUCC;
        pCurr = pCurr->pNext;
    }

    return NOKEY;
}

int32_t HashSetRemove(HashSet *self, Key key, size_t size)
{
    CHECK_INIT(self);
    if (size == 0)
        return ERR_KEYSIZE;

    HashSetData *pData = self->pData;
    SlotNode **aSlot = pData->aSlot_;

    /* Calculate the slot index. */
    uint32_t uiValue = pData->pHash_(key, size);
    uiValue = uiValue % pData->uiCountSlot_;

    /* Search the slot list for the deletion target. */
    SlotNode *pPred = NULL;
    SlotNode *pCurr = aSlot[uiValue];
    while (pCurr) {
        if ((pCurr->sizeKey == size) &&
            (memcmp(pCurr->key, key, size) == 0)) {
            if (pData->pDestroy_)
                pData->pDestroy_(pCurr->key);
            if (!pPred)
                aSlot[uiValue] = pCurr->pNext;
            else
                pPred->pNext = pCurr->pNext;
            free(pCurr);
            pData->iSize_--;
            return SUCC;
        }
        pPred = pCurr;
        pCurr = pCurr->pNext;
    }

    return ERR_NODATA;
}

int32_t HashSetSize(HashSet *self)
{
    CHECK_INIT(self);
    return self->pData->iSize_;
}

int32_t HashSetIterate(HashSet *self, bool bReset, Key *pKey)
{
    CHECK_INIT(self);

    HashSetData *pData = self->pData;

    if (bReset) {
        pData->uiIterIdx_ = 0;
        pData->pIterNode_ = pData->aSlot_[0];
        pData->bEnd_ = false;
        return SUCC;
    }

    if (!pKey)
        return ERR_GET;

    if (pData->bEnd_) {
        *pKey = NULL;
        return END;
    }

    SlotNode **aSlot = pData->aSlot_;
    while (true) {
        while (pData->pIterNode_) {
            *pKey = pData->pIterNode_->key;
            pData->pIterNode_ = pData->pIterNode_->pNext;
            return CONTINUE;
        }
        pData->uiIterIdx_++;
        if (pData->uiIterIdx_ == pData->uiCountSlot_)
            break;
        pData->pIterNode_ = aSlot[pData->uiIterIdx_];
    }

    pData->bEnd_ = true;
    *pKey = NULL;
    return END;
}

int32_t HashSetSetDestroy(HashSet *self, void (*pFunc) (Key))
{
    CHECK_INIT(self);
    self->pData->pDestroy_ = pFunc;
    return SUCC;
}

int32_t HashSetSetHash(HashSet *self, uint32_t (*pFunc) (Key, size_t))
{
    CHECK_INIT(self);
    self->pData->pHash_ = pFunc;
    return SUCC;
}

int32_t HashSetUnion(HashSet *pFst, HashSet *pSnd, HashSet **ppDst)
{
    CHECK_INIT(pFst);
    CHECK_INIT(pSnd);

    /* Predict the required slot size for the result set. */
    int32_t iSizeFst = pFst->pData->iSize_;
    int32_t iSizeSnd = pSnd->pData->iSize_;
    uint32_t uiExptSlot = (uint32_t)((double)(iSizeFst + iSizeSnd) / dLoadFactor_);
    int32_t iIdxPrime = 0;
    while (iIdxPrime < iCountPrime_) {
        if (uiExptSlot < aMagicPrimes[iIdxPrime])
            break;
        iIdxPrime++;
    }
    if (iIdxPrime == iCountPrime_)
        iIdxPrime--;

    /* Create the result set. */
    int32_t iRtn = _HashSetInit(ppDst, iIdxPrime);
    if (iRtn != SUCC)
        return iRtn;

    /* Merge the first source set. */
    HashSetData *pDataFst = pFst->pData;
    uint32_t uiIdx;
    for (uiIdx = 0 ; uiIdx < pDataFst->uiCountSlot_ ; uiIdx++) {
        SlotNode *pPred;
        SlotNode *pCurr = pDataFst->aSlot_[uiIdx];
        while (pCurr) {
            pPred = pCurr;
            pCurr = pCurr->pNext;
            iRtn = HashSetAdd(*ppDst, pPred->key, pPred->sizeKey);
            if (iRtn != SUCC) {
                HashSetDeinit(ppDst);
                return iRtn;
            }
        }
    }

    /* Merge the second source set. */
    HashSetData *pDataSnd = pSnd->pData;
    for (uiIdx = 0 ; uiIdx < pDataSnd->uiCountSlot_ ; uiIdx++) {
        SlotNode *pPred;
        SlotNode *pCurr = pDataSnd->aSlot_[uiIdx];
        while (pCurr) {
            pPred = pCurr;
            pCurr = pCurr->pNext;
            iRtn = HashSetAdd(*ppDst, pPred->key, pPred->sizeKey);
            if (iRtn != SUCC) {
                HashSetDeinit(ppDst);
                return iRtn;
            }
        }
    }

    return SUCC;
}

int32_t HashSetIntersect(HashSet *pFst, HashSet *pSnd, HashSet **ppDst)
{
    CHECK_INIT(pFst);
    CHECK_INIT(pSnd);

    /* Predict the required slot size for the result set. */
    int32_t iSizeFst = pFst->pData->iSize_;
    int32_t iSizeSnd = pSnd->pData->iSize_;
    int32_t iExptSize;
    HashSet *pSrc, *pSink;
    if (iSizeFst < iSizeSnd) {
        iExptSize = iSizeFst;
        pSrc = pFst;
        pSink = pSnd;
    } else {
        iExptSize = iSizeSnd;
        pSrc = pSnd;
        pSink = pFst;
    }
    uint32_t uiExptSlot = (uint32_t)((double)iExptSize / dLoadFactor_);
    int32_t iIdxPrime = 0;
    while (iIdxPrime < iCountPrime_) {
        if (uiExptSlot < aMagicPrimes[iIdxPrime])
            break;
        iIdxPrime++;
    }
    if (iIdxPrime == iCountPrime_)
        iIdxPrime--;

    /* Create the result set. */
    int32_t iRtn = _HashSetInit(ppDst, iIdxPrime);
    if (iRtn != SUCC)
        return iRtn;

    /* Collect the keys belonged to both source sets. */
    HashSetData *pDataSrc = pSrc->pData;
    uint32_t uiIdx;
    for (uiIdx = 0 ; uiIdx < pDataSrc->uiCountSlot_ ; uiIdx++) {
        SlotNode *pPred;
        SlotNode *pCurr = pDataSrc->aSlot_[uiIdx];
        while (pCurr) {
            pPred = pCurr;
            pCurr = pCurr->pNext;
            iRtn = HashSetFind(pSink, pPred->key, pPred->sizeKey);
            if (iRtn == NOKEY)
                continue;
            iRtn = HashSetAdd(*ppDst, pPred->key, pPred->sizeKey);
            if (iRtn != SUCC) {
                HashSetDeinit(ppDst);
                return iRtn;
            }
        }
    }

    return SUCC;
}

int32_t HashSetDifference(HashSet *pFst, HashSet *pSnd, HashSet **ppDst)
{
    CHECK_INIT(pFst);
    CHECK_INIT(pSnd);

    /* Predict the required slot size for the result set. */
    int32_t iSizeFst = pFst->pData->iSize_;
    int32_t iSizeSnd = pSnd->pData->iSize_;
    int32_t iExptSize = (iSizeFst > iSizeSnd)? iSizeFst : iSizeSnd;
    uint32_t uiExptSlot = (uint32_t)((double)iExptSize / dLoadFactor_);
    int32_t iIdxPrime = 0;
    while (iIdxPrime < iCountPrime_) {
        if (uiExptSlot < aMagicPrimes[iIdxPrime])
            break;
        iIdxPrime++;
    }
    if (iIdxPrime == iCountPrime_)
        iIdxPrime--;

    /* Create the result set. */
    int32_t iRtn = _HashSetInit(ppDst, iIdxPrime);
    if (iRtn != SUCC)
        return iRtn;

    /* Collect the keys only belonged to the first source set. */
    HashSetData *pDataFst = pFst->pData;
    uint32_t uiIdx;
    for (uiIdx = 0 ; uiIdx < pDataFst->uiCountSlot_ ; uiIdx++) {
        SlotNode *pPred;
        SlotNode *pCurr = pDataFst->aSlot_[uiIdx];
        while (pCurr) {
            pPred = pCurr;
            pCurr = pCurr->pNext;
            iRtn = HashSetFind(pSnd, pPred->key, pPred->sizeKey);
            if (iRtn == SUCC)
                continue;
            iRtn = HashSetAdd(*ppDst, pPred->key, pPred->sizeKey);
            if (iRtn != SUCC) {
                HashSetDeinit(ppDst);
                return iRtn;
            }
        }
    }

    return SUCC;
}

/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
int32_t _HashSetInit(HashSet **ppObj, int32_t iIdxPrime)
{
    *ppObj = (HashSet*)malloc(sizeof(HashSet));
    if (!(*ppObj))
        return ERR_NOMEM;
    HashSet *pObj = *ppObj;

    pObj->pData = (HashSetData*)malloc(sizeof(HashSetData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    HashSetData *pData = pObj->pData;

    uint32_t uiCountSlot = aMagicPrimes[iIdxPrime];
    pData->aSlot_ = (SlotNode**)malloc(sizeof(SlotNode*) * uiCountSlot);
    if (!(pData->aSlot_)) {
        free(pObj->pData);
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    int32_t iIdx;
    for (iIdx = 0 ; iIdx < uiCountSlot ; iIdx++)
        pData->aSlot_[iIdx] = NULL;

    pData->iSize_ = 0;
    pData->iIdxPrime_ = iIdxPrime;
    pData->uiCountSlot_ = uiCountSlot;
    pData->pHash_ = HashMurMur32;
    pData->pDestroy_ = NULL;

    pObj->add = HashSetAdd;
    pObj->find = HashSetFind;
    pObj->remove = HashSetRemove;
    pObj->size = HashSetSize;
    pObj->iterate = HashSetIterate;
    pObj->set_destroy = HashSetSetDestroy;
    pObj->set_hash = HashSetSetHash;

    return SUCC;
}

void _HashSetReHash(HashSetData *pData)
{
    uint32_t uiCountNew;

    /* Consume the next prime for slot array extension. */
    if (pData->iIdxPrime_ < (iCountPrime_ - 1)) {
        pData->iIdxPrime_++;
        uiCountNew = aMagicPrimes[pData->iIdxPrime_];
    }
    /* If the prime list is completely consumed, we simply extend the slot array
       with treble capacity.*/
    else {
        pData->iIdxPrime_ = iCountPrime_;
        uiCountNew = pData->uiCountSlot_ * 3;
    }

    /* Try to allocate the new slot array. The rehashing should be canceled due
       to insufficient memory space.  */
    SlotNode **aSlotNew = (SlotNode**)malloc(sizeof(SlotNode*) * uiCountNew);
    if (!aSlotNew) {
        if (pData->iIdxPrime_ < iCountPrime_)
            pData->iIdxPrime_--;
        return;
    }

    uint32_t uiIdx;
    for (uiIdx = 0  ; uiIdx < uiCountNew ; uiIdx++)
        aSlotNew[uiIdx] = NULL;

    for (uiIdx = 0 ; uiIdx < pData->uiCountSlot_ ; uiIdx++) {
        SlotNode *pPred;
        SlotNode *pCurr = pData->aSlot_[uiIdx];
        while (pCurr) {
            pPred = pCurr;
            pCurr = pCurr->pNext;

            /* Migrate each pair to the new slot. */
            uint32_t uiValue = pData->pHash_(pPred->key, pPred->sizeKey);
            uiValue = uiValue % uiCountNew;
            if (!aSlotNew[uiValue]) {
                pPred->pNext = NULL;
                aSlotNew[uiValue] = pPred;
            } else {
                pPred->pNext = aSlotNew[uiValue];
                aSlotNew[uiValue] = pPred;
            }
        }
    }

    free(pData->aSlot_);
    pData->aSlot_ = aSlotNew;
    pData->uiCountSlot_ = uiCountNew;
    return;
}
