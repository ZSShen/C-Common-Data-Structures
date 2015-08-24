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


int main()
{
    Queue *pQueue;

    /* You should initialize the DS before any operations. */
    int32_t iRtnCode = QueueInit(&pQueue);
    if (iRtnCode != SUCC)
        return iRtnCode;

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

    /* Check the queue order. */
    Item item;
    pQueue->front(pQueue, &item);
    assert(((Employ*)item)->cLevel == 4);
    pQueue->back(pQueue, &item);
    assert(((Employ*)item)->cLevel == 1);

    /* Pop items from the queue */
    pQueue->pop(pQueue);
    pQueue->pop(pQueue);

    /* Check the number of stored items. */
    int32_t iSize = pQueue->size(pQueue);
    assert(iSize == 2);

    /* You should deinitialize the DS after all the relevant tasks. */
    QueueDeinit(&pQueue);

    return SUCC;
}
