#include "cds.h"


typedef struct Tuple_ {
    int first;
    int second;
} Tuple;


/* Let the integer with minimum value reside at the top position. */
int CompareNumerics(const void* lhs, const void* rhs)
{
    if ((intptr_t)lhs == (intptr_t)rhs)
        return 0;
    return ((intptr_t)lhs >= (intptr_t)rhs)? 1 : (-1);
}

/* Let the object with maximum value reside at the top position. */
int CompareObjects(const void* lhs, const void* rhs)
{
    Tuple* tpl_lhs = (Tuple*)lhs;
    Tuple* tpl_rhs = (Tuple*)rhs;
    if (tpl_lhs->first == tpl_rhs->first)
        return 0;
    return (tpl_lhs->first > tpl_rhs->first)? (-1) : 1;
}

void CleanObject(void* element)
{
    free(element);
}


void ManipulateNumerics()
{
    /* We should initialize the container before any operations. */
    PriorityQueue* queue = PriorityQueueInit();
    queue->set_compare(queue, CompareNumerics);

    /* Push integer elements to the queue. */
    PriorityQueuePush(queue, (void*)(intptr_t)4);
    PriorityQueuePush(queue, (void*)(intptr_t)3);
    PriorityQueuePush(queue, (void*)(intptr_t)2);
    PriorityQueuePush(queue, (void*)(intptr_t)1);

    /* Pop elements from the queue and check the top elements. */
    void* elem;
    PriorityQueueTop(queue, &elem);
    assert((int)(intptr_t)elem == 1);
    PriorityQueuePop(queue);

    PriorityQueueTop(queue, &elem);
    assert((int)(intptr_t)elem == 2);
    PriorityQueuePop(queue);

    PriorityQueueTop(queue, &elem);
    assert((int)(intptr_t)elem == 3);
    PriorityQueuePop(queue);

    /* Check the number of stored elements. */
    unsigned size = PriorityQueueSize(queue);
    assert(size == 1);

    PriorityQueueDeinit(queue);
}

void ManipulateObjects()
{
    /* We should initialize the container before any operations. */
    PriorityQueue* queue = PriorityQueueInit();
    queue->set_compare(queue, CompareObjects);
    queue->set_clean(queue, CleanObject);

    /* Push integer elements to the queue. */
    Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 4;
    tuple->second = -4;
    PriorityQueuePush(queue, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 3;
    tuple->second = -3;
    PriorityQueuePush(queue, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 2;
    tuple->second = -2;
    PriorityQueuePush(queue, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 1;
    tuple->second = -1;
    PriorityQueuePush(queue, tuple);

    /* Pop elements from the queue and check the top elements. */
    void* elem;
    PriorityQueueTop(queue, &elem);
    assert(((Tuple*)elem)->first == 4);
    PriorityQueuePop(queue);

    PriorityQueueTop(queue, &elem);
    assert(((Tuple*)elem)->first == 3);
    PriorityQueuePop(queue);

    PriorityQueueTop(queue, &elem);
    assert(((Tuple*)elem)->first == 2);
    PriorityQueuePop(queue);

    /* Check the number of stored elements. */
    unsigned size = PriorityQueueSize(queue);
    assert(size == 1);

    PriorityQueueDeinit(queue);
}

void ManipulateNumericsCppStyle()
{
    /* We should initialize the container before any operations. */
    PriorityQueue* queue = PriorityQueueInit();
    queue->set_compare(queue, CompareNumerics);

    /* Push integer elements to the queue. */
    queue->push(queue, (void*)(intptr_t)4);
    queue->push(queue, (void*)(intptr_t)3);
    queue->push(queue, (void*)(intptr_t)2);
    queue->push(queue, (void*)(intptr_t)1);

    /* Pop elements from the queue and check the top elements. */
    void* elem;
    queue->top(queue, &elem);
    assert((int)(intptr_t)elem == 1);
    queue->pop(queue);

    queue->top(queue, &elem);
    assert((int)(intptr_t)elem == 2);
    queue->pop(queue);

    queue->top(queue, &elem);
    assert((int)(intptr_t)elem == 3);
    queue->pop(queue);

    /* Check the number of stored elements. */
    unsigned size = queue->size(queue);
    assert(size == 1);

    PriorityQueueDeinit(queue);
}

void ManipulateObjectsCppStyle()
{
    /* We should initialize the container before any operations. */
    PriorityQueue* queue = PriorityQueueInit();
    queue->set_compare(queue, CompareObjects);
    queue->set_clean(queue, CleanObject);

    /* Push integer elements to the queue. */
    Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 4;
    tuple->second = -4;
    queue->push(queue, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 3;
    tuple->second = -3;
    queue->push(queue, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 2;
    tuple->second = -2;
    queue->push(queue, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 1;
    tuple->second = -1;
    queue->push(queue, tuple);

    /* Pop elements from the queue and check the top elements. */
    void* elem;
    queue->top(queue, &elem);
    assert(((Tuple*)elem)->first == 4);
    queue->pop(queue);

    queue->top(queue, &elem);
    assert(((Tuple*)elem)->first == 3);
    queue->pop(queue);

    queue->top(queue, &elem);
    assert(((Tuple*)elem)->first == 2);
    queue->pop(queue);

    /* Check the number of stored elements. */
    unsigned size = queue->size(queue);
    assert(size == 1);

    PriorityQueueDeinit(queue);
}

int main()
{
    ManipulateNumerics();
    ManipulateObjects();
    ManipulateNumericsCppStyle();
    ManipulateObjectsCppStyle();
    return 0;
}
