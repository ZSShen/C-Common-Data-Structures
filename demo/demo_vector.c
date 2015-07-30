#include "cds.h"


typedef struct Employ_ {
    int8_t cYear;
    int8_t cLevel;
    int32_t iId;
} Employ;


void DestroyObject(Item item)
{
    free((Employ*)item);
}

int32_t CompareObject(const void *ppSrc, const void *ppTge)
{
    Employ *empSrc = *((Employ**)(Item*)ppSrc);
    Employ *empTge = *((Employ**)(Item*)ppTge);
    if (empSrc->iId == empTge->iId)
        return 0;
    return (empSrc->iId > empTge->iId)? 1 : (-1);
}


int main()
{
    Vector *pVec;

    /* You should initialize the DS before any operations. */
    int32_t rc = VectorInit(&pVec, 1);
    if (rc != SUCC)
        return rc;

    /* If you plan to delegate the resource clean task to the DS, please set the
       custom clean method. */
    pVec->set_destroy(pVec, DestroyObject);

    /* Push the items. */
    Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 3;
    pEmploy->cLevel = 3;
    pEmploy->cYear = 3;
    pVec->push_back(pVec, (Item)pEmploy);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 4;
    pEmploy->cLevel = 4;
    pEmploy->cYear = 4;
    pVec->push_back(pVec, (Item)pEmploy);

    /* Insert the items at the designated indexes. */
    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 1;
    pEmploy->cLevel = 1;
    pEmploy->cYear = 1;
    pVec->insert(pVec, (Item)pEmploy, 0);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 2;
    pEmploy->cLevel = 2;
    pEmploy->cYear = 2;
    pVec->insert(pVec, (Item)pEmploy, 1);

    /*---------------------------------------------------------------*
     * Now the vector should be: [1] | [2] | [3] | [4]               *
     *---------------------------------------------------------------*/

    /* Iterate through the vector. */
    Item item;
    int32_t iId = 1;
    pVec->iterate(pVec, true, NULL);
    while (pVec->iterate(pVec, false, &item) != END) {
        assert(((Employ*)item)->iId == iId);
        iId++;
    }

    /* Retrieve the items with direct indexing. */
    pVec->get(pVec, &item, 0);
    assert(((Employ*)item)->iId == 1);
    pVec->get(pVec, &item, 3);
    assert(((Employ*)item)->iId == 4);

    /* Replace the item with direct indexing. */
    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 10;
    pEmploy->cLevel = 10;
    pEmploy->cYear = 10;
    pVec->set(pVec, (Item)pEmploy, 0);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 40;
    pEmploy->cLevel = 40;
    pEmploy->cYear = 40;
    pVec->set(pVec, (Item)pEmploy, 3);

    /*---------------------------------------------------------------*
     * Now the vector should be: [10] | [2] | [3] | [40]             *
     *---------------------------------------------------------------*/

    /* Check the number of stored items and the vector capacity. */
    int32_t iSize = pVec->size(pVec);
    assert(iSize == 4);
    int32_t iCap = pVec->capacity(pVec);
    assert(iCap == 4);

    /* Sort the items with the custom item comparison method. */
    pVec->sort(pVec, CompareObject);

    /*---------------------------------------------------------------*
     * Now the vector should be: [2] | [3] | [10] | [40]             *
     *---------------------------------------------------------------*/

    /* Delete the item at the designated index. */
    pVec->delete(pVec, 3);
    pVec->delete(pVec, 0);

    /*---------------------------------------------------------------*
     * Now the vector should be: [3] | [10]                          *
     *---------------------------------------------------------------*/

    int32_t iNum = 0;
    pVec->get(pVec, &item, 0);
    iNum += ((Employ*)item)->iId;
    pVec->get(pVec, &item, 1);
    iNum += ((Employ*)item)->iId;
    iNum *= 20;

    /* Resize the vector storage. */
    pVec->resize(pVec, iNum);

    /* Pop the rest items. */
    pVec->pop_back(pVec);
    pVec->pop_back(pVec);

    iSize = pVec->size(pVec);
    assert(iSize == 0);
    iCap = pVec->capacity(pVec);
    assert(iCap == 260);

    /* You should deinitialize the DS after all the relevant tasks. */
    VectorDeinit(&pVec);

    return SUCC;
}
