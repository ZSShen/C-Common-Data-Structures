#include "cds.h"


int main()
{
    char *aName[5] = {"Alice\0", "Bob\0", "Carol\0", "Dave\0", "Oscar\0"};

    /* You should initialize the DS before any operations. */
    HashSet *pFst = NULL;
    HashSet *pSnd = NULL;

    int32_t rc = HashSetInit(&pFst);
    if (rc != SUCC)
        goto EXIT;
    rc = HashSetInit(&pSnd);
    if (rc != SUCC)
        goto EXIT;

    /* Insert keys into the sets. */
    pFst->add(pFst, (Key)aName[0], strlen(aName[0]));
    pFst->add(pFst, (Key)aName[1], strlen(aName[1]));
    pFst->add(pFst, (Key)aName[2], strlen(aName[2]));

    pSnd->add(pSnd, (Key)aName[2], strlen(aName[2]));
    pSnd->add(pSnd, (Key)aName[3], strlen(aName[3]));
    pSnd->add(pSnd, (Key)aName[4], strlen(aName[4]));

    /* Delete some keys from the sets. */
    pFst->remove(pFst, (Key)aName[1], strlen(aName[1]));
    pSnd->remove(pSnd, (Key)aName[3], strlen(aName[3]));

    /*-----------------------------------------------------*
     * The 1st set now contains {"Alice\0", "Carol\0"}.    *
     * The 2nd set now contains {"Carol\0", "Oscar\0"}.    *
     *-----------------------------------------------------*/

    /* Query for key existence. */
    assert(pFst->find(pFst, (Key)aName[0], strlen(aName[0])) == SUCC);
    assert(pFst->find(pFst, (Key)aName[1], strlen(aName[1])) == NOKEY);
    assert(pSnd->find(pSnd, (Key)aName[3], strlen(aName[3])) == NOKEY);
    assert(pSnd->find(pSnd, (Key)aName[4], strlen(aName[4])) == SUCC);

    /* Iterate through the set. */
    Key key;
    pFst->iterate(pFst, true, NULL);
    while (pFst->iterate(pFst, false, &key) == CONTINUE) {
        /* Do what you plan to do for the retrieved key. */
    }

    /* Check the key count in the set. */
    int32_t iSizeFst = pFst->size(pFst);
    assert(iSizeFst == 2);

    /* Perform set union operation. */
    HashSet *pUnion;
    HashSetUnion(pFst, pSnd, &pUnion);
    assert(pUnion->size(pUnion) == 3);
    assert(pUnion->find(pUnion, (Key)aName[0], strlen(aName[0])) == SUCC);
    assert(pUnion->find(pUnion, (Key)aName[1], strlen(aName[1])) == NOKEY);
    assert(pUnion->find(pUnion, (Key)aName[2], strlen(aName[2])) == SUCC);
    assert(pUnion->find(pUnion, (Key)aName[3], strlen(aName[3])) == NOKEY);
    assert(pUnion->find(pUnion, (Key)aName[4], strlen(aName[4])) == SUCC);

    /* Perform set intersection operation. */
    HashSet *pInter;
    HashSetIntersect(pFst, pSnd, &pInter);
    assert(pInter->size(pInter) == 1);
    assert(pInter->find(pInter, (Key)aName[0], strlen(aName[0])) == NOKEY);
    assert(pInter->find(pInter, (Key)aName[1], strlen(aName[1])) == NOKEY);
    assert(pInter->find(pInter, (Key)aName[2], strlen(aName[2])) == SUCC);
    assert(pInter->find(pInter, (Key)aName[3], strlen(aName[3])) == NOKEY);
    assert(pInter->find(pInter, (Key)aName[4], strlen(aName[4])) == NOKEY);

    /* Perform set difference operation. */
    HashSet *pOnlyFst;
    HashSetDifference(pFst, pSnd, &pOnlyFst);
    assert(pOnlyFst->size(pOnlyFst) == 1);
    assert(pOnlyFst->find(pOnlyFst, (Key)aName[0], strlen(aName[0])) == SUCC);
    assert(pOnlyFst->find(pOnlyFst, (Key)aName[1], strlen(aName[1])) == NOKEY);
    assert(pOnlyFst->find(pOnlyFst, (Key)aName[2], strlen(aName[2])) == NOKEY);
    assert(pOnlyFst->find(pOnlyFst, (Key)aName[3], strlen(aName[3])) == NOKEY);
    assert(pOnlyFst->find(pOnlyFst, (Key)aName[4], strlen(aName[4])) == NOKEY);

    HashSet *pOnlySnd;
    HashSetDifference(pSnd, pFst, &pOnlySnd);
    assert(pOnlySnd->size(pOnlySnd) == 1);
    assert(pOnlySnd->find(pOnlySnd, (Key)aName[0], strlen(aName[0])) == NOKEY);
    assert(pOnlySnd->find(pOnlySnd, (Key)aName[1], strlen(aName[1])) == NOKEY);
    assert(pOnlySnd->find(pOnlySnd, (Key)aName[2], strlen(aName[2])) == NOKEY);
    assert(pOnlySnd->find(pOnlySnd, (Key)aName[3], strlen(aName[3])) == NOKEY);
    assert(pOnlySnd->find(pOnlySnd, (Key)aName[4], strlen(aName[4])) == SUCC);

EXIT:
    /* You should deinitialize the DS after all the relevant tasks. */
    HashSetDeinit(&pOnlySnd);
    HashSetDeinit(&pOnlyFst);
    HashSetDeinit(&pInter);
    HashSetDeinit(&pUnion);
    HashSetDeinit(&pFst);
    HashSetDeinit(&pSnd);

    return SUCC;
}
