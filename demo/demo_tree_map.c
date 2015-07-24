#include "cds.h"


typedef struct Employ_ {
    int32_t iId;
    int8_t cYear;
    int8_t cLevel;
} Employ;


int32_t PairCompare(Entry entSrc, Entry entTge)
{
    Pair *pPairSrc = (Pair*)entSrc;
    Pair *pPairTge = (Pair*)entTge;

    int32_t iOrder = strcmp(pPairSrc->key, pPairTge->key);
    if (iOrder == 0)
        return 0;
    return (iOrder > 0)? 1 : (-1);
}

void PairDestroy(Entry ent)
{
    Pair *pPair = (Pair*)ent;
    free((Employ*)pPair->value);
    free((Pair*)ent);
}


int main()
{
    char *aName[3] = {"Alice\0", "Bob\0", "Wesker\0"};
    TreeMap *pMap;

    /* You should initialize the DS before any operations. */
    int32_t rc = TreeMapInit(&pMap);
    if (rc != SUCC)
        return rc;

    /* You should specify how to compare your items. */
    pMap->set_compare(pMap, PairCompare);

    /* If you plan to delegate the resource clean task to the DS, please set the
       custom clean method. */
    pMap->set_destroy(pMap, PairDestroy);

    /* Insert key value pairs into the map. */
    Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 1;
    pEmploy->cYear = 25;
    pEmploy->cLevel = 100;
    Pair *pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)aName[0];
    pPair->value = (Value)pEmploy;
    pMap->put(pMap, (Entry)pPair);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 2;
    pEmploy->cYear = 25;
    pEmploy->cLevel = 90;
    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)aName[1];
    pPair->value = (Value)pEmploy;
    pMap->put(pMap, (Entry)pPair);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 3;
    pEmploy->cYear = 25;
    pEmploy->cLevel = 80;
    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)aName[2];
    pPair->value = (Value)pEmploy;
    pMap->put(pMap, (Entry)pPair);

    /* Retrieve the value with the designated key. */
    Value value;
    pMap->get(pMap, (Key)aName[0], &value);
    assert(((Employ*)value)->iId == 1);
    assert(((Employ*)value)->cYear == 25);
    assert(((Employ*)value)->cLevel == 100);

    /* Remove the key value pair with the designated key. */
    pMap->remove(pMap, (Key)aName[1]);

    /* Check the pair count in the map. */
    int32_t iSize = pMap->size(pMap);
    assert(iSize == 2);

    /* You should deinitialize the DS after all the relevant tasks. */
    TreeMapDeinit(&pMap);

    return SUCC;
}

