#include "cds.h"


/* The example to manipulate primitive type input. */
int32_t simple_compare(Item, Item);
void usage_simple();


/* The example to mainpulate non-primitive type input. */
typedef struct Employ_ {
    int8_t cYear;
    int8_t cLevel;
    int32_t iId;
} Employ;
int32_t advanced_compare(Item, Item);
void advanced_destroy(Item);
void usage_advanced();


int main()
{
    usage_simple();
    usage_advanced();

    return 0;
}


int32_t simple_compare(Item itemSrc, Item itemTge)
{
    if (itemSrc == itemTge)
        return 0;
    return (itemSrc > itemTge)? 1 : (-1);
}

void usage_simple()
{
    PrioQueue *pQueue;

    int32_t iRtnCode = PrioQueueInit(&pQueue);
    if (iRtnCode != SUCC)
        return;

    /*----------------------------------------------------*
     * Set the item comparison method for priority queue. *
     *----------------------------------------------------*/
    pQueue->set_compare(pQueue, simple_compare);

    /*----------------------------------------------------*
     * Push items onto the queue.                         *
     *----------------------------------------------------*/
    pQueue->push(pQueue, (Item)1);
    pQueue->push(pQueue, (Item)2);
    pQueue->push(pQueue, (Item)3);
    pQueue->push(pQueue, (Item)4);

    int32_t iSize = pQueue->size(pQueue);
    assert(iSize == 4);

    /*----------------------------------------------------*
     * Pop items from the queue and check the order.      *
     *----------------------------------------------------*/
    Item item;
    pQueue->top(pQueue, &item);
    #if __x86_64__
        assert((int64_t)item == 4);
    #else
        assert(item == 4);
    #endif
    pQueue->pop(pQueue);

    pQueue->top(pQueue, &item);
    #if __x86_64__
        assert((int64_t)item == 3);
    #else
        assert(item == 3);
    #endif
    pQueue->pop(pQueue);

    pQueue->top(pQueue, &item);
    #if __x86_64__
        assert((int64_t)item == 2);
    #else
        assert(item == 2);
    #endif
    pQueue->pop(pQueue);

    iSize = pQueue->size(pQueue);
    assert(iSize == 1);

    PrioQueueDeinit(&pQueue);
    return;
}


int32_t advanced_compare(Item itemSrc, Item itemTge)
{
    Employ *empSrc = (Employ*)itemSrc;
    Employ *empTge = (Employ*)itemTge;

    if (empSrc->cLevel == empTge->cLevel)
        return 0;
    return (empSrc->cLevel < empTge->cLevel)? 1 : (-1);
}

void advanced_destroy(Item item)
{
    free((Employ*)item);
}

void usage_advanced()
{
    PrioQueue *pQueue;

    int32_t iRtnCode = PrioQueueInit(&pQueue);
    if (iRtnCode != SUCC)
        return;

    /*----------------------------------------------------*
     * Set the item comparison method for priority queue. *
     * And set the item clean method.                     *
     *----------------------------------------------------*/
    pQueue->set_compare(pQueue, advanced_compare);
    pQueue->set_destroy(pQueue, advanced_destroy);

    /*----------------------------------------------------*
     * Push items onto the queue.                         *
     *----------------------------------------------------*/
    Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 1;
    pEmploy->cLevel = 4;
    pEmploy->cYear = 0;
    pQueue->push(pQueue, (Item)pEmploy);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 2;
    pEmploy->cLevel = 3;
    pEmploy->cYear = 0;
    pQueue->push(pQueue, (Item)pEmploy);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 3;
    pEmploy->cLevel = 2;
    pEmploy->cYear = 0;
    pQueue->push(pQueue, (Item)pEmploy);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 4;
    pEmploy->cLevel = 1;
    pEmploy->cYear = 0;
    pQueue->push(pQueue, (Item)pEmploy);

    int32_t iSize = pQueue->size(pQueue);
    assert(iSize == 4);

    /*--------------------------------------------------*
     * Pop items from the queue and check the order.    *
     *--------------------------------------------------*/
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

    iSize = pQueue->size(pQueue);
    assert(iSize == 1);

    PrioQueueDeinit(&pQueue);
    return;
}
