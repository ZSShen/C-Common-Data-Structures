#include "cds.h"


/* The example usage for simple key value pair. */
void simple_destroy(Entry);
void usage_simple();


/* The example usage for advanced key value pair. */
typedef struct Employ_ {
    int32_t iId;
    int8_t cYear;
    int8_t cLevel;
} Employ;
void advanced_destroy(Entry);
void usage_advanced();


int main()
{
    usage_simple();
    usage_advanced();

    return 0;
}


void simple_destroy(Entry ent)
{
    Pair *pPair = (Pair *)ent;
    free((int32_t*)pPair->key);
    free((int32_t*)pPair->value);
    free((Pair *)ent);
    return;
}

void usage_simple()
{
    HashMap *pMap;

    int32_t iRtnCode;
    iRtnCode = HashMapInit(&pMap, 0);
    if (iRtnCode != SUCC)
        return;

    /*--------------------------------------------------*
     * Set the pair resource clean method.              *
     *--------------------------------------------------*/
    pMap->set_destroy(pMap, simple_destroy);

    /*--------------------------------------------------*
     * Insert key value pairs into the map.             *
     *--------------------------------------------------*/
    Pair *pPair = (Pair*)malloc(sizeof(Pair));
    int32_t *pKey = (int32_t*)malloc(sizeof(int32_t));
    int32_t *pValue = (int32_t*)malloc(sizeof(int32_t));
    *pKey = 1;
    *pValue = -1;
    pPair->key = (Key)pKey;
    pPair->value = (Value)pValue;
    pMap->put(pMap, (Entry)pPair, sizeof(int32_t));

    pPair = (Pair*)malloc(sizeof(Pair));
    pKey = (int32_t*) malloc(sizeof(int32_t));
    pValue = (int32_t*) malloc(sizeof(int32_t));
    *pKey = 3;
    *pValue = -3;
    pPair->key = (Key)pKey;
    pPair->value = (Value)pValue;
    pMap->put(pMap, (Entry)pPair, sizeof(int32_t));

    pPair = (Pair*)malloc(sizeof(Pair));
    pKey = (int32_t*) malloc(sizeof(int32_t));
    pValue = (int32_t*) malloc(sizeof(int32_t));
    *pKey = 2;
    *pValue = -2;
    pPair->key = (Key)pKey;
    pPair->value = (Value)pValue;
    pMap->put(pMap, (Entry)pPair, sizeof(int32_t));

    /*--------------------------------------------------*
     * Retrive the value by with the specified key.     *
     *--------------------------------------------------*/
    int32_t iKey = 1;
    pMap->get(pMap, (Key)&iKey, sizeof(int32_t), (Value*)&pValue);
    assert(*pValue == -1);

    /*--------------------------------------------------*
     * Remove the key value pair with the specified key.*
     *--------------------------------------------------*/
    iKey = 2;
    pMap->remove(pMap, (Key)&iKey, sizeof(int32_t));

    int32_t iSize = pMap->size(pMap);
    assert(iSize == 2);

    HashMapDeinit(&pMap);
    return;
}


void advanced_destroy(Entry ent)
{
    Pair *pPair = (Pair*)ent;
    free((Employ*)pPair->value);
    free((Pair*)ent);
    return;
}

void usage_advanced()
{
    char *aName[3] = {"Alice", "Bob", "Wesker"};
    HashMap *pMap;

    int32_t iRtnCode = HashMapInit(&pMap, 0);
    if (iRtnCode != SUCC)
        return;

    /*--------------------------------------------------*
     * Set the pair resource clean method.              *
     *--------------------------------------------------*/
    pMap->set_destroy(pMap, advanced_destroy);

    /*--------------------------------------------------*
     * Insert key value pairs into the map.             *
     *--------------------------------------------------*/
    Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 1;
    pEmploy->cYear = 25;
    pEmploy->cLevel = 100;
    Pair *pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)aName[0];
    pPair->value = (Value)pEmploy;
    pMap->put(pMap, (Entry)pPair, strlen(aName[0]) + 1);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 2;
    pEmploy->cYear = 25;
    pEmploy->cLevel = 90;
    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)aName[1];
    pPair->value = (Value)pEmploy;
    pMap->put(pMap, (Entry)pPair, strlen(aName[1]) + 1);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 3;
    pEmploy->cYear = 25;
    pEmploy->cLevel = 80;
    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)aName[2];
    pPair->value = (Value)pEmploy;
    pMap->put(pMap, (Entry)pPair, strlen(aName[2]) + 1);

    /*--------------------------------------------------*
     * Retrive the value with the specified key.        *
     *--------------------------------------------------*/
    Value value;
    pMap->get(pMap, (Key)aName[0], strlen(aName[0]) + 1, &value);
    assert(((Employ*)value)->iId == 1);
    assert(((Employ*)value)->cYear == 25);
    assert(((Employ*)value)->cLevel == 100);

    /*--------------------------------------------------*
     * Remove the key value pair with the specified key.*
     *--------------------------------------------------*/
    pMap->remove(pMap, (Key)aName[1], strlen(aName[1]) + 1);

    int32_t iSize = pMap->size(pMap);
    assert(iSize == 2);

    HashMapDeinit(&pMap);
    return;
}
