#include "map/hash_map.h"
#include "array/vector.h"
#include "list/dlinked_list.h"


/*===========================================================================*
 *                        The container private data                         *
 *===========================================================================*/
struct _HashMapData {
    int32_t iSize_;
    Vector *vecBucket_;
    uint32_t (*pHash_) (Key, size_t);
};

#define DEFAULT_COUNT    (32)


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
/**
 * @brief               The resource clean method for each table bucket
 *
 * @param item          The pointer to the designated bucket
 */
static void _HashMapCleanBucket(Item item);

/**
 * @brief               The default hash function
 *
 * Name: Google MurmurHash 3
 * Site: https://code.google.com/p/smhasher/wiki/MurmurHash3
 *
 * @param key           The designated key
 * @param sizeKey       The key size
 *
 * @return              The hash value of the key
 */
static uint32_t _HashMapHasher(Key key, size_t sizeKey);


#define FREE_MAP_DATA()                                                         \
            do {                                                                \
                free(pObj->pData);                                              \
                free(*ppObj);                                                   \
                *ppObj = NULL;                                                  \
            } while (0);

#define CHECK_INIT(self)                                                        \
            do {                                                                \
                if (!self)                                                      \
                    return ERR_NOINIT;                                          \
                if (!(self->pData))                                             \
                    return ERR_NOINIT;                                          \
                if (!(self->pData->vecBucket_))                                 \
                    return ERR_NOINIT;                                          \
            } while (0);


/*===========================================================================*
 *         Implementation for the container supporting operations            *
 *===========================================================================*/
int32_t HashMapInit(HashMap **ppObj, uint32_t uiCount)
{
    *ppObj = (HashMap*)malloc(sizeof(HashMap));
    if (!(*ppObj))
        return ERR_NOMEM;
    HashMap *pObj = *ppObj;

    pObj->pData = (HashMapData*)malloc(sizeof(HashMapData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    HashMapData *pData = pObj->pData;

    uiCount = (uiCount == 0)? DEFAULT_COUNT : uiCount;
    pData->iSize_ = 0;
    pData->pHash_ = _HashMapHasher;
    int32_t iRtnCode = VectorInit(&(pData->vecBucket_), uiCount);
    if (iRtnCode != SUCC) {
        FREE_MAP_DATA();
        return ERR_NOMEM;
    }

    Vector *vecBucket = pData->vecBucket_;
    vecBucket->set_destroy(vecBucket, _HashMapCleanBucket);

    int32_t iCap = vecBucket->capacity(vecBucket);
    int32_t iIdx = 0;
    while (iIdx < iCap) {
        DLinkedList *list;
        iRtnCode = DListInit(&list);
        if (iRtnCode != SUCC) {
            VectorDeinit(&vecBucket);
            FREE_MAP_DATA();
            return iRtnCode;
        }
        iRtnCode = vecBucket->insert(vecBucket, (Item)list, iIdx);
        if (iRtnCode != SUCC) {
            VectorDeinit(&vecBucket);
            FREE_MAP_DATA();
            return iRtnCode;
        }
        iIdx++;
    }

    pObj->put = HashMapPut;
    pObj->get = HashMapGet;
    pObj->remove = HashMapRemove;
    pObj->size = HashMapSize;
    pObj->set_destroy = HashMapSetDestroy;
    pObj->set_hash = HashMapSetHash;

    return SUCC;
}

void HashMapDeinit(HashMap **ppObj)
{
    if (!(*ppObj))
        goto EXIT;

    HashMap *pObj = *ppObj;
    if (!(pObj->pData))
        goto FREE_MAP;

    HashMapData *pData = pObj->pData;
    if (!(pData->vecBucket_))
        goto FREE_DATA;

    VectorDeinit(&(pData->vecBucket_));

FREE_DATA:
    free(pObj->pData);
FREE_MAP:
    free(*ppObj);
    *ppObj = NULL;
EXIT:
    return;
}

int32_t HashMapPut(HashMap *self, Entry ent, size_t sizeKey)
{
    if (sizeKey == 0)
        return ERR_KEYSIZE;
    CHECK_INIT(self);
    HashMapData *pData = self->pData;

    /* Resolve the bucket index with modulo arithmetics. */
    Pair *pairIn = (Pair*)ent;
    Vector *vecBucket = pData->vecBucket_;
    int32_t iSize = vecBucket->size(vecBucket);
    uint32_t uiValue = pData->pHash_(pairIn->key, sizeKey);
    int32_t iIdx = (int32_t)uiValue;
    iIdx = iIdx % iSize;
    iIdx = (iIdx < 0)? iIdx + iSize : iIdx;

    /* Retrieve the slot list of the located bucket. */
    DLinkedList *listSlot;
    vecBucket->get(vecBucket, (Item*)&listSlot, iIdx);

    /* Replace the existing duplicated entry. */
    Pair *pairDup;
    iSize = listSlot->size(listSlot);
    for (iIdx = 0 ; iIdx < iSize ; iIdx++) {
        listSlot->get_at(listSlot, (Item*)&pairDup, iIdx);
        if (memcmp(pairIn->key, pairDup->key, sizeKey) == 0) {
            listSlot->delete(listSlot, iIdx);
            pData->iSize_--;
            break;
        }
    }

    /* Insert the new entry into the slot list. */
    int32_t iRtnCode = listSlot->push_back(listSlot, ent);
    if (iRtnCode == SUCC)
        pData->iSize_++;
    return iRtnCode;
}

int32_t HashMapGet(HashMap *self, Key key, size_t sizeKey, Value *pValue)
{
    if (!pValue)
        return ERR_GET;
    if (sizeKey == 0)
        return ERR_KEYSIZE;
    *pValue = NULL;
    CHECK_INIT(self);

    /* Resolve the bucket index with modulo arithmetics. */
    Vector *vecBucket = self->pData->vecBucket_;
    int32_t iSize = vecBucket->size(vecBucket);
    uint32_t uiValue = self->pData->pHash_(key, sizeKey);
    int32_t iIdx = (int32_t)uiValue;
    iIdx = iIdx % iSize;
    iIdx = (iIdx < 0)? iIdx + iSize : iIdx;

    /* Retrieve the slot list of the located bucket. */
    DLinkedList *listSlot;
    vecBucket->get(vecBucket, (Item*)&listSlot, iIdx);

    /* Retrieve the value corresponding to the designated key. */
    Pair *pPairOut;
    iSize = listSlot->size(listSlot);
    for (iIdx = 0 ; iIdx < iSize ; iIdx++) {
        listSlot->get_at(listSlot, (Item*)&pPairOut, iIdx);
        if (memcmp(key, pPairOut->key, sizeKey) == 0) {
            *pValue = pPairOut->value;
            return SUCC;
        }
    }

    return ERR_NODATA;
}

int32_t HashMapRemove(HashMap *self, Key key, size_t sizeKey)
{
    if (sizeKey == 0)
        return ERR_KEYSIZE;
    CHECK_INIT(self);
    HashMapData *pData = self->pData;

    /* Resolve the bucket index with modulo arithmetics. */
    Vector *vecBucket = pData->vecBucket_;
    int32_t iSize = vecBucket->size(vecBucket);
    uint32_t uiValue = pData->pHash_(key, sizeKey);
    int32_t iIdx = (int32_t)uiValue;
    iIdx = iIdx % iSize;
    iIdx = (iIdx < 0)? iIdx + iSize : iIdx;

    /* Retrieve the slot list of the located bucket. */
    DLinkedList *listSlot;
    vecBucket->get(vecBucket, (Item*)&listSlot, iIdx);

    /* Remove the key value pair having the designated key. */
    Pair *pairDel;
    iSize = listSlot->size(listSlot);
    for (iIdx = 0 ; iIdx < iSize ; iIdx++) {
        listSlot->get_at(listSlot, (Item*)&pairDel, iIdx);
        if (memcmp(key, pairDel->key, sizeKey) == 0) {
            listSlot->delete(listSlot, iIdx);
            pData->iSize_--;
            return SUCC;
        }
    }

    return ERR_NODATA;
}

int32_t HashMapSize(HashMap *self)
{
    CHECK_INIT(self);
    return self->pData->iSize_;
}

int32_t HashMapSetDestroy(HashMap *self, void (*pFunc) (Entry))
{
    CHECK_INIT(self);

    Vector *vecBucket = self->pData->vecBucket_;
    int32_t iSize = vecBucket->size(vecBucket);

    DLinkedList *listSlot;
    int32_t iIdx = 0;
    while (iIdx < iSize) {
        vecBucket->get(vecBucket, (Item*)&listSlot, iIdx);
        listSlot->set_destroy(listSlot, pFunc);
        iIdx++;
    }

    return SUCC;
}

int32_t HashMapSetHash(HashMap *self, uint32_t (*pFunc) (Key, size_t))
{
    CHECK_INIT(self);
    self->pData->pHash_ = pFunc;
    return SUCC;
}

/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
static void _HashMapCleanBucket(Item item)
{
    DLinkedList *list = (DLinkedList*)item;
    DListDeinit(&list);
}

static uint32_t _HashMapHasher(Key key, size_t sizeKey)
{
    if (key == NULL || sizeKey == 0)
        return 0;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    const int nblocks = sizeKey / 4;
    const uint32_t *blocks = (const uint32_t*) (key);
    const uint8_t *tail = (const uint8_t*) (key + (nblocks * 4));

    uint32_t h = 0;

    int i;
    uint32_t k;
    for (i = 0; i < nblocks; i++) {
        k = blocks[i];

        k *= c1;
        k = (k << 15) | (k >> (32 - 15));
        k *= c2;

        h ^= k;
        h = (h << 13) | (h >> (32 - 13));
        h = (h * 5) + 0xe6546b64;
    }

    k = 0;
    switch (sizeKey & 3) {
        case 3:
            k ^= tail[2] << 16;
        case 2:
            k ^= tail[1] << 8;
        case 1:
            k ^= tail[0];
            k *= c1;
            k = (k << 15) | (k >> (32 - 15));
            k *= c2;
            h ^= k;
    };

    h ^= sizeKey;

    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}
