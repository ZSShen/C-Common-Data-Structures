#include "map/hash_map.h"
#include "array/vector.h"
#include "list/dlinked_list.h"

#define DEFAULT_CAPACITY    (32)

/*===========================================================================*
 *                  Hide the private data                        *
 *===========================================================================*/
struct _HashMapData {
    Vector *pBuckets_;
    uint32_t (*pHash_) (Key, size_t);
};


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
/**
 * @brief               The resource clean method dlist in buckets
 *
 * @param ent           The pointer to the dlist
 */
static void _BucketsDestroy(Entry ent);

/**
 * @brief               The default hash function
 *
 * @param key           The pointer to key
 * @param len           The legnth of key
 *
 * @return              The hash value of key
 */
static uint32_t _Hash(Key key, size_t len);


#define CHECK_INIT(self)                    \
    do {                                    \
        if (!self)                          \
        return ERR_NOINIT;                  \
        if (!(self->pData))                 \
        return ERR_NOINIT;                  \
        if (!(self->pData->pBuckets_))      \
        return ERR_NOINIT;                  \
    } while (0);


/*===========================================================================*
 *         Implementation for the container supporting operations            *
 *===========================================================================*/
int32_t HashMapInit(HashMap **ppObj, int32_t size)
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

    int32_t BucketSize = (size <= 0) ? DEFAULT_CAPACITY: size;
    pData->pHash_ = _Hash;
    int32_t iRtnCode; 
    iRtnCode = VectorInit(&(pData->pBuckets_), BucketSize);
    if (iRtnCode != SUCC) {
        free(pObj->pData);
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }

    Vector *Buckets = pData->pBuckets_;
    Buckets->set_destroy(Buckets, _BucketsDestroy);

    int32_t cap = VectorCapacity(Buckets);
    int32_t idx = 0;
    while (idx < cap) {
        DLinkedList *link;
        iRtnCode = DListInit(&link);
        if (iRtnCode != SUCC) {
            VectorDeinit(&Buckets);
            free(pObj->pData);
            free(*ppObj);
            *ppObj = NULL;
            return iRtnCode;
        }
        iRtnCode = Buckets->insert(Buckets, (Item)link, idx);
        if (iRtnCode != SUCC) {
            VectorDeinit(&Buckets);
            free(pObj->pData);
            free(*ppObj);
            *ppObj = NULL;
            return iRtnCode;
        }
        ++idx;
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
    if (pData->pBuckets_)
        VectorDeinit(&(pData->pBuckets_));

    free(pObj->pData);
FREE_MAP:
    free(*ppObj);
    *ppObj = NULL;
EXIT:
    return;
}

int32_t HashMapPut(HashMap *self, Entry ent, size_t key_size)
{
    CHECK_INIT(self);
    if (key_size <= 0)
        return ERR_KEYSIZE;
    int32_t iRtnCode;
    Pair *pairIn = (Pair *)ent;
    uint32_t idx = self->pData->pHash_(pairIn->key, key_size);
    DLinkedList *list;
    Vector *Buckets = self->pData->pBuckets_;
    int32_t BucketsSize = Buckets->size(Buckets);
    if (BucketsSize == ERR_NOINIT)
        return ERR_NOINIT;
    idx = idx % BucketsSize;
    if ((iRtnCode = Buckets->get(Buckets, (Item *)&list, idx)) != SUCC)
        return iRtnCode;
    int32_t list_size = list->size (list);
    Pair *pariComp;
    //find duplicate entry and delete it
    for (idx = 0; idx < list_size; ++idx) {
        if ((iRtnCode = list->get_at(list, (Item *)&pariComp, idx)) != SUCC)
            return iRtnCode;
        if (!memcmp(pairIn->key, pariComp->key, key_size)) {
            if ((iRtnCode = list->delete(list, idx)) != SUCC)
                return iRtnCode;
            break;
        }
    }
    return list->push_back(list, ent);
}

int32_t HashMapGet(HashMap *self, Key key, size_t key_size, Value *pValue)
{
    if (!pValue)
        return ERR_GET;
    if (key_size <= 0)
        return ERR_KEYSIZE;
    *pValue = NULL;
    CHECK_INIT(self);

    int32_t iRtnCode;
    Pair pairIn = {key, NULL};
    uint32_t idx = self->pData->pHash_(pairIn.key, key_size);
    DLinkedList *list;
    Vector *Buckets = self->pData->pBuckets_;
    int32_t BucketsSize = Buckets->size(Buckets);
    if (BucketsSize == ERR_NOINIT)
        return ERR_NOINIT;
    idx = idx % BucketsSize;
    if ((iRtnCode = Buckets->get(Buckets, (Item *)&list, idx)) != SUCC)
        return iRtnCode;
    int32_t list_size = list->size (list);
    Pair *pPairOut;
    for (idx = 0; idx < list_size; ++idx) {
        if ((iRtnCode = list->get_at(list, (Item *)&pPairOut, idx)) != SUCC)
            return iRtnCode;
        if (!memcmp(pairIn.key, pPairOut->key, key_size)) {
            *pValue = pPairOut->value;
            return SUCC;
        }
    }
    return ERR_NODATA;
}

int32_t HashMapRemove(HashMap *self, Key key, size_t key_size)
{
    CHECK_INIT(self);
    if (key_size <= 0)
        return ERR_KEYSIZE;

    int32_t iRtnCode;
    Pair pairIn = {key, NULL};
    uint32_t idx = self->pData->pHash_(pairIn.key, key_size);
    DLinkedList *list;
    Vector *Buckets = self->pData->pBuckets_;
    int32_t BucketsSize = Buckets->size(Buckets);
    if (BucketsSize == ERR_NOINIT)
        return ERR_NOINIT;
    idx = idx % BucketsSize;
    if ((iRtnCode = Buckets->get(Buckets, (Item *)&list, idx)) != SUCC)
        return iRtnCode;
    int32_t list_size = list->size(list);
    Pair *pPairOut;
    for (idx = 0; idx < list_size; ++idx) {
        if ((iRtnCode = list->get_at(list, (Item *)&pPairOut, idx)) != SUCC)
            return iRtnCode;
        if (!memcmp(pairIn.key, pPairOut->key, key_size)) {
            return list->delete(list, idx);
        }
    }
    return ERR_NODATA;
}

int32_t HashMapSize(HashMap *self)
{
    CHECK_INIT(self);

    int32_t iRtnCode;
    int32_t size = 0;
    DLinkedList *list;
    Vector *Buckets = self->pData->pBuckets_;
    int32_t BucketsSize = Buckets->size(Buckets);
    if (BucketsSize == ERR_NOINIT)
        return ERR_NOINIT;
    int32_t idx;
    for (idx = 0; idx < BucketsSize; ++idx) {
        if ((iRtnCode = Buckets->get(Buckets, (Item *)&list, idx)) != SUCC)
            return iRtnCode;
        size += list->size(list) ;
    }
    return size;
}

int32_t HashMapSetDestroy(HashMap *self, void (*pFunc) (Entry))
{
    CHECK_INIT(self);

    int32_t iRtnCode;
    int32_t size = 0;
    DLinkedList *list;
    Vector *Buckets = self->pData->pBuckets_;
    int32_t BucketsSize = Buckets->size(Buckets);
    if (BucketsSize == ERR_NOINIT)
        return ERR_NOINIT;
    int32_t idx;
    for (idx = 0; idx < BucketsSize; ++idx) {
        if ((iRtnCode = Buckets->get(Buckets, (Item *)&list, idx)) != SUCC)
            return iRtnCode;
        if ((iRtnCode = list->set_destroy(list, pFunc)) != SUCC)
            return iRtnCode;
    }
    return SUCC;
}

int32_t HashMapSetHash(HashMap *self, uint32_t (*pFunc) (Key, size_t)) {
    CHECK_INIT(self);
    self->pData->pHash_ = pFunc;
    return SUCC;
}

/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
static void _BucketsDestroy(Entry ent)
{
    DLinkedList *list = (DLinkedList *) ent;
    DListDeinit(&list);
}

/* Google MurmurHash 3 
 * https://code.google.com/p/smhasher/wiki/MurmurHash3
 */
static uint32_t _Hash(Key key, size_t len)
{
    if (key == NULL || len == 0)
        return 0;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    const int nblocks = len / 4;
    const uint32_t *blocks = (const uint32_t *) (key);
    const uint8_t *tail = (const uint8_t *) (key + (nblocks * 4));

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
    switch (len & 3) {
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

    h ^= len;

    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}
