#include "cds.h"


typedef struct Employ_ {
    int8_t cYear;
    int8_t cLevel;
    int32_t iId;
} Employ;


/* This comparator will cause the item with the smallest level to appear at the
   top of the queue. And the queue can be analogy to minimum priority queue.*/
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
    PriorityQueue *pQueue;

    /* You should initialize the DS before any operations. */
    int32_t iRtnCode = PrioQueueInit(&pQueue);
    if (iRtnCode != SUCC)
        return;

    /* You should specify how to compare your items. */
    pQueue->set_compare(pQueue, CompareObject);

    /* If you plan to delegate the resource clean task to the DS, please set the
       custom clean method. */
    pQueue->set_destroy(pQueue, DestroyObject);

    /* Push items onto the queue. */
    Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 4;
    pEmploy->cLevel = 4;
    pEmploy->cYear = 4;
    pQueue->push(pQueue, (Item)pEmploy);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 3;
    pEmploy->cLevel = 3;
    pEmploy->cYear = 3;
    pQueue->push(pQueue, (Item)pEmploy);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 2;
    pEmploy->cLevel = 2;
    pEmploy->cYear = 2;
    pQueue->push(pQueue, (Item)pEmploy);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 1;
    pEmploy->cLevel = 1;
    pEmploy->cYear = 1;
    pQueue->push(pQueue, (Item)pEmploy);

    /* Pop items from the queue and check the top items. */
    Item item;
    pQueue->top(pQueue, &item);
    assert(((Employ*)item)->cLevel == 1);
    pQueue->pop(pQueue);

    pQueue->top(pQueue, &item);
    assert(((Employ*)item)->cLevel == 2);
    pQueue->pop(pQueue);

    pQueue->top(pQueue, &item);
    assert(((Employ*)item)->cLevel == 3);
    pQueue->pop(pQueue);

    /* Check the number of stored items. */
    int32_t iSize = pQueue->size(pQueue);
    assert(iSize == 1);

    /* You should deinitialize the DS after all the relevant tasks. */
    PrioQueueDeinit(&pQueue);

    return SUCC;
}
