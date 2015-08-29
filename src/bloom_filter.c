#include "container/bloom_filter.h"


/*===========================================================================*
 *                        The container private data                         *
 *===========================================================================*/

struct _BloomFilterData {
    int32_t iSize_;
    char *aBit_;
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
                if (!(self->pData->aBit_))                                      \
                    return ERR_NOINIT;                                          \
            } while (0);


/*===========================================================================*
 *               Implementation for the exported operations                  *
 *===========================================================================*/
int32_t BloomFilterInit(BloomFilter **ppObj)
{
    /* Hopefully, we should let user specify:
       1. The number of expected elements for insertion.
       2. The acceptable FA rate.

       Based on this information, we create the internal bit array and prepare
       the relevant abount of hash functions. */

    #define TEMP_SIZE_BIT_ARRAY (1)

    *ppObj = (BloomFilter*)malloc(sizeof(BloomFilter));
    if (!(*ppObj))
        return ERR_NOMEM;

    BloomFilter *pObj = *ppObj;
    pObj->pData = (BloomFilterData*)malloc(sizeof(BloomFilterData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }

    BloomFilterData *pData = pObj->pData;
    pData->aBit_ = (char*)malloc(sizeof(char) * TEMP_SIZE_BIT_ARRAY);
    if (!(pData->aBit_)) {
        free(pObj->pData);
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }

    pData->iSize_ = 0;
    pObj->insert = BloomFilterInsert;
    pObj->query = BloomFilterQuery;
    pObj->size = BloomFilterSize;

    return SUCC;
}

void BloomFilterDeinit(BloomFilter **ppObj)
{
    if (!(*ppObj))
        goto EXIT;

    BloomFilter *pObj = *ppObj;
    if (!(pObj->pData))
        goto FREE_FILTER;

    BloomFilterData *pData = pObj->pData;
    if (!(pData->aBit_))
        goto FREE_DATA;

    free(pData->aBit_);

FREE_DATA:
    free(pObj->pData);
FREE_FILTER:
    free(*ppObj);
    *ppObj = NULL;
EXIT:
    return;
}

int32_t BloomFilterInsert(BloomFilter *self, Key key, size_t size)
{
    CHECK_INIT(self);
    return SUCC;
}

int32_t BloomFilterQuery(BloomFilter *self, Key key, size_t size)
{
    CHECK_INIT(self);
    return SUCC;
}

int32_t BloomFilterSize(BloomFilter *self)
{
    CHECK_INIT(self);
    return self->pData->iSize_;
}