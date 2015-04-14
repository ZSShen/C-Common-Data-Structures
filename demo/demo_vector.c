#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>


/* The example usage for primitive input manipulation. */
void usage_primitive();


int main()
{
    usage_primitive();

    return 0;
}


void usage_primitive()
{
    Vector *pVec;

    int32_t rc = VectorInit(&pVec);
    if (rc != SUCCESS)
        return;

    /* Push the item. */
    pVec->push_back(pVec, (Item)30);
    pVec->push_back(pVec, (Item)40);

    /* Insert the item at the designated index. */
    pVec->insert(pVec, (Item)10, 0);
    pVec->insert(pVec, (Item)20, 1);

    /* Acquire the item with direct indexing. */
    Item item;
    pVec->get(pVec, &item, 0);
    assert(item == (Item)10);
    pVec->get(pVec, &item, 3);
    assert(item == (Item)40);

    /* Replace the item with direct indexing. */
    pVec->set(pVec, (Item)100, 0);
    pVec->set(pVec, (Item)400, 3);

    /* Query the storage capacity. */
    int32_t iSize = pVec->size(pVec);
    assert(iSize == 4);
    int32_t iCap = pVec->capacity(pVec);
    assert(iCap == 4);

    /* Delete the item at the designated index. */
    pVec->delete(pVec, 3);
    pVec->delete(pVec, 0);

    /* Take care for 32/64 architecture. */
    int32_t iNum = 0;
#if __x86_64__
    pVec->get(pVec, &item, 0);
    iNum += (int32_t)(int64_t)item;
    pVec->get(pVec, &item, 1);
    iNum += (int32_t)(int64_t)item;
#else
    pVec->get(pVec, &item, 0);
    iNum += (int32_t)item;
    pVec->get(pVec, &item, 1);
    iNum += (int32_t)item;
#endif

    /* Resize the vector. */
    pVec->resize(pVec, iNum);

    /* Pop the item. */
    pVec->pop_back(pVec);
    pVec->pop_back(pVec);

    iSize = pVec->size(pVec);
    assert(iSize == 0);
    iCap = pVec->capacity(pVec);
    assert(iCap == 50);

    VectorDeinit(&pVec);
    return;
}
