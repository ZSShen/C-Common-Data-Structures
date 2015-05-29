#include "cds.h"


/* The example usage for simple key value pair. */
int32_t simple_compare(Entry, Entry);
void simple_destroy(Entry);
void usage_simple();


int main()
{
    usage_simple();

    return 0;
}


int32_t simple_compare(Entry entSrc, Entry entTge)
{
    Pair *pPairSrc = (Pair*)entSrc;
    Pair *pPairTge = (Pair*)entTge;
    if (pPairSrc->key == pPairTge->key)
        return 0;
    return (pPairSrc->key > pPairTge->key)? 1 : (-1);
}

void simple_destroy(Entry ent)
{
    free((Pair*)ent);
    return;
}

void usage_simple()
{
    OrderedMap *pMap;

    int32_t iRtnCode = OdrMapInit(&pMap);
    if (iRtnCode != SUCC)
        return;

    /* Set the comparison and resource clean methods. */
    pMap->set_compare(pMap, simple_compare);
    pMap->set_destroy(pMap, simple_destroy);

    /*--------------------------------------------------*
     * Insert key value pairs into the map.             *
     *--------------------------------------------------*/
    Pair *pPair;
    pPair = (Pair*)malloc(sizeof(Pair));
    #if __x86_64__
        pPair->key = (Key)(int64_t)1;
        pPair->value = (Key)(int64_t)-1;
    #else
        pPair->key = (Key)1;
        pPair->value = (Key)-1;
    #endif
    pMap->put(pMap, (Entry)pPair);

    pPair = (Pair*)malloc(sizeof(Pair));
    #if __x86_64__
        pPair->key = (Key)(int64_t)3;
        pPair->value = (Key)(int64_t)-3;
    #else
        pPair->key = (Key)3;
        pPair->value = (Key)-3;
    #endif
    pMap->put(pMap, (Entry)pPair);

    pPair = (Pair*)malloc(sizeof(Pair));
    #if __x86_64__
        pPair->key = (Key)(int64_t)2;
        pPair->value = (Key)(int64_t)-2;
    #else
        pPair->key = (Key)2;
        pPair->value = (Key)-2;
    #endif
    pMap->put(pMap, (Entry)pPair);

    /*--------------------------------------------------*
     * Retrive the value by specifying the key.         *
     *--------------------------------------------------*/
    Value value;
    #if __x86_64__
        pMap->get(pMap, (Key)(int64_t)1, &value);
        assert((int64_t)value == -1);
    #else
        pMap->get(pMap, (Key)1, &value);
        assert(value == -1);
    #endif

    /*--------------------------------------------------*
     * Remove the key value pair by specifying the key. *
     *--------------------------------------------------*/
    #if __x86_64__
        pMap->remove(pMap, (Key)(int64_t)2);
    #else
        pMap->remove(pMap, (Key)2);
    #endif

    int32_t iSize = pMap->size(pMap);
    assert(iSize == 2);

    OdrMapDeinit(&pMap);
    return;
}