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
    Stack *pStack;

    /* You should initialize the DS before any operations. */
    int32_t rc = StackInit(&pStack);
    if (rc != SUCC)
        return rc;

    /* If you plan to delegate the resource clean task to the DS, please set the
       custom clean method. */
    pStack->set_destroy(pStack, DestroyObject);

    /* Push items onto the queue. */
    Employ* pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 3;
    pEmploy->cLevel = 3;
    pEmploy->cYear = 3;
    pStack->push(pStack, (Item)pEmploy);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 2;
    pEmploy->cLevel = 2;
    pEmploy->cYear = 2;
    pStack->push(pStack, (Item)pEmploy);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 1;
    pEmploy->cLevel = 1;
    pEmploy->cYear = 1;
    pStack->push(pStack, (Item)pEmploy);

    /* Check the number of stored items. */
    int32_t iSize = pStack->size(pStack);
    assert(iSize == 3);

    /* Retrieve the stack top item and delete it. */
    Item item;
    pStack->top(pStack, &item);
    assert(((Employ*)item)->cLevel == 1);
    pStack->pop(pStack);

    pStack->top(pStack, &item);
    assert(((Employ*)item)->cLevel == 2);
    pStack->pop(pStack);

    pStack->top(pStack, &item);
    assert(((Employ*)item)->cLevel == 3);
    pStack->pop(pStack);

    /* You should deinitialize the DS after all the relevant tasks. */
    StackDeinit(&pStack);

    return SUCC;
}
