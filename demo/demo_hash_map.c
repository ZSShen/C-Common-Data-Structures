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
    free((int32_t *)pPair->key);
    free((int32_t *)pPair->value);
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
     * Set the comparison and resource clean methods.   *
     *--------------------------------------------------*/
    iRtnCode = pMap->set_destroy(pMap, simple_destroy);
    assert(iRtnCode == SUCC);

    /*--------------------------------------------------*
     * Insert key value pairs into the map.             *
     *--------------------------------------------------*/
    Pair *pPair;
    int32_t *key;
    int32_t *value;
    int32_t iKey;
    pPair = (Pair *)malloc(sizeof(Pair));
    key = (int32_t *)malloc(sizeof(int32_t));
    *key = 1;
    pPair->key = (Key)key;
    value = (int32_t *)malloc(sizeof(int32_t));
    *value = -1;
    pPair->value = (Value)value;
    iRtnCode = pMap->put(pMap, (Entry)pPair, sizeof(int32_t));
    assert(iRtnCode == SUCC);

    pPair = (Pair*)malloc(sizeof(Pair));
    key = (int32_t *) malloc(sizeof(int32_t));
    *key = 3;
    pPair->key = (Key) key;
    value = (int32_t *) malloc(sizeof(int32_t));
    *value = -3;
    pPair->value = (Value) value;
    pMap->put(pMap, (Entry)pPair, sizeof(int32_t));
    assert(iRtnCode == SUCC);

    pPair = (Pair*)malloc(sizeof(Pair));
    key = (int32_t *) malloc(sizeof(int32_t));
    *key = 2;
    pPair->key = (Key) key;
    value = (int32_t *) malloc(sizeof(int32_t));
    *value = -2;
    pPair->value = (Value) value;
    pMap->put(pMap, (Entry)pPair, sizeof(int32_t));
    assert(iRtnCode == SUCC);

    /*--------------------------------------------------*
     * Retrive the value by with the specified key.     *
     *--------------------------------------------------*/
    iKey = 1;
    pMap->get(pMap, (Key)&iKey, sizeof(int32_t), (void *)&value);
    assert(iRtnCode == SUCC);
    assert(*value == -1);

    /*--------------------------------------------------*
     * Remove the key value pair with the specified key.*
     *--------------------------------------------------*/
    iKey = 2;
    pMap->remove(pMap, (Key)&iKey, sizeof(int32_t));
    assert(iRtnCode == SUCC);

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
     * Set the comparison and resource clean methods.   *
     *--------------------------------------------------*/
    iRtnCode = pMap->set_destroy(pMap, advanced_destroy);
    assert(iRtnCode == SUCC);

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
    iRtnCode = pMap->put(pMap, (Entry)pPair, strlen(aName[0]) + 1);
    assert(iRtnCode == SUCC);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 2;
    pEmploy->cYear = 25;
    pEmploy->cLevel = 90;
    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)aName[1];
    pPair->value = (Value)pEmploy;
    iRtnCode = pMap->put(pMap, (Entry)pPair, strlen(aName[1]) + 1);
    assert(iRtnCode == SUCC);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 3;
    pEmploy->cYear = 25;
    pEmploy->cLevel = 80;
    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)aName[2];
    pPair->value = (Value)pEmploy;
    iRtnCode = pMap->put(pMap, (Entry)pPair, strlen(aName[2]) + 1);
    assert(iRtnCode == SUCC);

    /*--------------------------------------------------*
     * Retrive the value with the specified key.        *
     *--------------------------------------------------*/
    Value value;
    iRtnCode = pMap->get(pMap, (Key)aName[0], strlen(aName[0]) + 1, &value);
    assert(iRtnCode == SUCC);
    assert(((Employ*)value)->iId == 1);
    assert(((Employ*)value)->cYear == 25);
    assert(((Employ*)value)->cLevel == 100);

    /*--------------------------------------------------*
     * Remove the key value pair with the specified key.*
     *--------------------------------------------------*/
    iRtnCode = pMap->remove(pMap, (Key)aName[1], strlen(aName[1]) + 1);
    assert(iRtnCode == SUCC);

    int32_t iSize = pMap->size(pMap);
    assert(iSize == 2);

    HashMapDeinit(&pMap);
    return;
}
