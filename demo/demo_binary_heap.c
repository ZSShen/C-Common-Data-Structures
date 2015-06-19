#include "cds.h"


/* The example for primitive data manipulation. */
int32_t simple_compare(Item, Item);
void usage_simple();


/* The example for non-primitive data manipulation. */
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
    BinHeap *pHeap;

    int32_t iRtnCode = BinHeapInit(&pHeap);
    if (iRtnCode != SUCC)
        return;

    /*--------------------------------------------------*
     * Set the item comparison method for maximum heap. *
     *--------------------------------------------------*/
    pHeap->set_compare(pHeap, simple_compare);

    /*--------------------------------------------------*
     * Push items onto the heap.                        *
     *--------------------------------------------------*/
    pHeap->push(pHeap, (Item)1);
    pHeap->push(pHeap, (Item)2);
    pHeap->push(pHeap, (Item)3);
    pHeap->push(pHeap, (Item)4);

    int32_t iSize = pHeap->size(pHeap);
    assert(iSize == 4);

    /*--------------------------------------------------*
     * Pop items from the heap and check the order.     *
     *--------------------------------------------------*/
    Item item;
    pHeap->top(pHeap, &item);
    #if __x86_64__
        assert((int64_t)item == 4);
    #else
        assert(item == 4);
    #endif
    pHeap->pop(pHeap);

    pHeap->top(pHeap, &item);
    #if __x86_64__
        assert((int64_t)item == 3);
    #else
        assert(item == 3);
    #endif
    pHeap->pop(pHeap);

    pHeap->top(pHeap, &item);
    #if __x86_64__
        assert((int64_t)item == 2);
    #else
        assert(item == 2);
    #endif
    pHeap->pop(pHeap);

    iSize = pHeap->size(pHeap);
    assert(iSize == 1);

    BinHeapDeinit(&pHeap);
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
    BinHeap *pHeap;

    int32_t iRtnCode = BinHeapInit(&pHeap);
    if (iRtnCode != SUCC)
        return;

    /*--------------------------------------------------*
     * Set the item comparison method for minimum heap. *
     * And set the item clean method.                   *
     *--------------------------------------------------*/
    pHeap->set_compare(pHeap, advanced_compare);
    pHeap->set_destroy(pHeap, advanced_destroy);

    /*--------------------------------------------------*
     * Push items onto the heap.                        *
     *--------------------------------------------------*/
    Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 1;
    pEmploy->cLevel = 4;
    pEmploy->cYear = 0;
    pHeap->push(pHeap, (Item)pEmploy);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 2;
    pEmploy->cLevel = 3;
    pEmploy->cYear = 0;
    pHeap->push(pHeap, (Item)pEmploy);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 3;
    pEmploy->cLevel = 2;
    pEmploy->cYear = 0;
    pHeap->push(pHeap, (Item)pEmploy);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 4;
    pEmploy->cLevel = 1;
    pEmploy->cYear = 0;
    pHeap->push(pHeap, (Item)pEmploy);

    int32_t iSize = pHeap->size(pHeap);
    assert(iSize == 4);

    /*--------------------------------------------------*
     * Pop items from the heap and check the order.     *
     *--------------------------------------------------*/
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

    iSize = pHeap->size(pHeap);
    assert(iSize == 1);

    BinHeapDeinit(&pHeap);
    return;
}
