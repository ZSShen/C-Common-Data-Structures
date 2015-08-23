#include "cds.h"


typedef struct Employ_ {
    int8_t cYear;
    int8_t cLevel;
    int32_t iId;
} Employ;


int32_t CompareKey(Key keySrc, Key keyTge)
{
    char *nameSrc = (char*)keySrc;
    char *nameTge = (char*)keyTge;

    int32_t iOrder = strcmp(nameSrc, nameTge);
    if (iOrder == 0)
        return 0;
    return (iOrder > 0)? 1 : (-1);
}

void DestroyPair(Pair *pPair)
{
    free((Employ*)pPair->value);
    free(pPair);
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
    pMap->set_compare(pMap, CompareKey);

    /* If you plan to delegate the resource clean task to the DS, please set the
       custom clean method. */
    pMap->set_destroy(pMap, DestroyPair);

    /* Insert key value pairs into the map. */
    Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 1;
    pEmploy->cYear = 25;
    pEmploy->cLevel = 100;
    Pair *pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)aName[0];
    pPair->value = (Value)pEmploy;
    pMap->put(pMap, pPair);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 2;
    pEmploy->cYear = 25;
    pEmploy->cLevel = 90;
    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)aName[1];
    pPair->value = (Value)pEmploy;
    pMap->put(pMap, pPair);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 3;
    pEmploy->cYear = 25;
    pEmploy->cLevel = 80;
    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)aName[2];
    pPair->value = (Value)pEmploy;
    pMap->put(pMap, pPair);

    /* Retrieve the value with the designated key. */
    Value value;
    pMap->get(pMap, (Key)aName[0], &value);
    assert(((Employ*)value)->iId == 1);
    assert(((Employ*)value)->cYear == 25);
    assert(((Employ*)value)->cLevel == 100);

    /* Retrieve the pairs with minimum and maximum key orders from the map. */
    pMap->minimum(pMap, &pPair);
    assert(strcmp((char*)pPair->key, aName[0]) == 0);
    assert(((Employ*)pPair->value)->cLevel == 100);
    pMap->maximum(pMap, &pPair);
    assert(strcmp((char*)pPair->key, aName[2]) == 0);
    assert(((Employ*)pPair->value)->cLevel == 80);

    /* Reference the predecessor and successor pairs with the designated key. */
    pMap->predecessor(pMap, (Key)aName[1], &pPair);
    assert(strcmp((char*)pPair->key, aName[0]) == 0);
    assert(((Employ*)pPair->value)->iId == 1);
    pMap->successor(pMap, (Key)aName[1], &pPair);
    assert(strcmp((char*)pPair->key, aName[2]) == 0);
    assert(((Employ*)pPair->value)->iId == 3);

    /* Iterate through the map. */
    int32_t idx = 0;
    pMap->iterate(pMap, true, NULL);
    while (pMap->iterate(pMap, false, &pPair) == CONTINUE) {
        assert(strcmp((char*)pPair->key, aName[idx]) == 0);
        idx++;
    }

    /* Reversely iterate through the map.*/
    idx = 2;
    pMap->reverse_iterate(pMap, true, NULL);
    while (pMap->reverse_iterate(pMap, false, &pPair) == CONTINUE) {
        assert(strcmp((char*)pPair->key, aName[idx]) == 0);
        idx--;
    }

    /* Remove the key value pair with the designated key. */
    pMap->remove(pMap, (Key)aName[1]);

    /* Check the key existence. */
    assert(pMap->find(pMap, (Key)aName[0]) == SUCC);
    assert(pMap->find(pMap, (Key)aName[1]) == NOKEY);
    assert(pMap->find(pMap, (Key)aName[2]) == SUCC);

    /* Check the pair count in the map. */
    int32_t iSize = pMap->size(pMap);
    assert(iSize == 2);

    /* You should deinitialize the DS after all the relevant tasks. */
    TreeMapDeinit(&pMap);

    return SUCC;
}

