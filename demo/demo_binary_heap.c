#include "cds.h"


typedef struct Employ_ {
    int8_t cYear;
    int8_t cLevel;
    int32_t iId;
} Employ;


/* This comparator will cause the item with the smallest level to appear at the
   top of the heap. And the heap can be analogy to minimum heap. */
int32_t CompareObject(Item itemSrc, Item itemTge)
{
    Employ *empSrc = (Employ*)itemSrc;
    Employ *empTge = (Employ*)itemTge;

    if (empSrc->cLevel == empTge->cLevel)
        return 0;
    return (empSrc->cLevel < empTge->cLevel)? 1 : (-1);
}

void DestroyObject(Item item)
{
    free((Employ*)item);
}


int main()
{
    BinaryHeap *pHeap;

    /* You should initialize the DS before any operations. */
    int32_t rc = BinHeapInit(&pHeap);
    if (rc != SUCC)
        return rc;

    /* You should specify how to compare your items. */
    pHeap->set_compare(pHeap, CompareObject);

    /* If you plan to delegate the resource clean task to the DS, please set the
       custom clean method. */
    pHeap->set_destroy(pHeap, DestroyObject);

    /* Push items onto the heap. */
    Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 4;
    pEmploy->cLevel = 4;
    pEmploy->cYear = 4;
    pHeap->push(pHeap, (Item)pEmploy);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 3;
    pEmploy->cLevel = 3;
    pEmploy->cYear = 3;
    pHeap->push(pHeap, (Item)pEmploy);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 2;
    pEmploy->cLevel = 2;
    pEmploy->cYear = 2;
    pHeap->push(pHeap, (Item)pEmploy);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 1;
    pEmploy->cLevel = 1;
    pEmploy->cYear = 1;
    pHeap->push(pHeap, (Item)pEmploy);

    /* Pop items from the heap and check the top items. */
    Item item;
    pHeap->top(pHeap, &item);
    assert(((Employ*)item)->cLevel == 1);
    pHeap->pop(pHeap);

    pHeap->top(pHeap, &item);
    assert(((Employ*)item)->cLevel == 2);
    pHeap->pop(pHeap);

    pHeap->top(pHeap, &item);
    assert(((Employ*)item)->cLevel == 3);
    pHeap->pop(pHeap);

    /* Check the number of stored items. */
    int32_t iSize = pHeap->size(pHeap);
    assert(iSize == 1);

    /* You should deinitialize the DS after all the relevant tasks. */
    BinHeapDeinit(&pHeap);

    return SUCC;
}
