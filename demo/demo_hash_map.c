#include "cds.h"


typedef struct Employ_ {
    int32_t iId;
    int8_t cYear;
    int8_t cLevel;
} Employ;


void PairDestroy(Entry ent)
{
    Pair *pPair = (Pair*)ent;
    free((Employ*)pPair->value);
    free((Pair*)ent);
}


int main()
{
    char *aName[3] = {"Alice\0", "Bob\0", "Wesker\0"};
    HashMap *pMap;

    /* You should initialize the DS before any operations. */
    int32_t rc = HashMapInit(&pMap, 0);
    if (rc != SUCC)
        return rc;

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

    /* Retrieve the value with the designated key. */
    Value value;
    pMap->get(pMap, (Key)aName[0], strlen(aName[0]) + 1, &value);
    assert(((Employ*)value)->iId == 1);
    assert(((Employ*)value)->cYear == 25);
    assert(((Employ*)value)->cLevel == 100);

    /* Remove the key value pair with the designated key. */
    pMap->remove(pMap, (Key)aName[1], strlen(aName[1]) + 1);

    /* Check the pair count in the map. */
    int32_t iSize = pMap->size(pMap);
    assert(iSize == 2);

    /* You should deinitialize the DS after all the relevant tasks. */
    HashMapDeinit(&pMap);

    return SUCC;
}
