#include "cds.h"


typedef struct Tuple_ {
    int first;
    int second;
} Tuple;


void CleanObject(void* element)
{
    free(element);
}


void ManipulateNumerics()
{
    /* We should initialize the container before any operations. */
    Queue* queue = QueueInit();

    /* Push numeric elements to the queue. */
    QueuePush(queue, (void*)(intptr_t)1);
    QueuePush(queue, (void*)(intptr_t)2);
    QueuePush(queue, (void*)(intptr_t)3);
    QueuePush(queue, (void*)(intptr_t)4);

    /* Check the element order. */
    void* element;
    QueueFront(queue, &element);
    assert((int)(intptr_t)element == 1);

    QueueBack(queue, &element);
    assert((int)(intptr_t)element == 4);

    /* Pop elements from the queue*/
    QueuePop(queue);
    QueuePop(queue);

    /* Check the number of stored elements. */
    assert(QueueSize(queue) == 2);

    QueueDeinit(queue);
}

void ManipulateObjects()
{
    /* We should initialize the container before any operations. */
    Queue* queue = QueueInit();
    QueueSetClean(queue, CleanObject);

    /* Push numeric elements to the queue. */
    Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 1;
    tuple->second = -1;
    QueuePush(queue, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 2;
    tuple->second = -2;
    QueuePush(queue, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 3;
    tuple->second = -3;
    QueuePush(queue, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 4;
    tuple->second = -4;
    QueuePush(queue, tuple);

    /* Check the element order. */
    void* element;
    QueueFront(queue, &element);
    assert(((Tuple*)element)->first == 1);
    QueueBack(queue, &element);
    assert(((Tuple*)element)->first == 4);

    /* Pop elements from the queue*/
    QueuePop(queue);
    QueuePop(queue);

    /* Check the number of stored elements. */
    assert(QueueSize(queue) == 2);

    QueueDeinit(queue);
}

void ManipulateNumericsCppStyle()
{
    /* We should initialize the container before any operations. */
    Queue* queue = QueueInit();

    /* Push numeric elements to the queue. */
    queue->push(queue, (void*)1);
    queue->push(queue, (void*)2);
    queue->push(queue, (void*)3);
    queue->push(queue, (void*)4);

    /* Check the element order. */
    void* element;
    queue->front(queue, &element);
    assert((int)(intptr_t)element == 1);

    queue->back(queue, &element);
    assert((int)(intptr_t)element == 4);

    /* Pop elements from the queue*/
    queue->pop(queue);
    queue->pop(queue);

    /* Check the number of stored elements. */
    assert(QueueSize(queue) == 2);

    QueueDeinit(queue);
}

void ManipulateObjectsCppStyle()
{
    /* We should initialize the container before any operations. */
    Queue* queue = QueueInit();
    QueueSetClean(queue, CleanObject);

    /* Push numeric elements to the queue. */
    Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 1;
    tuple->second = -1;
    queue->push(queue, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 2;
    tuple->second = -2;
    queue->push(queue, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 3;
    tuple->second = -3;
    queue->push(queue, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 4;
    tuple->second = -4;
    queue->push(queue, tuple);

    /* Check the element order. */
    void* element;
    queue->front(queue, &element);
    assert(((Tuple*)element)->first == 1);
    queue->back(queue, &element);
    assert(((Tuple*)element)->first == 4);

    /* Pop elements from the queue*/
    queue->pop(queue);
    queue->pop(queue);

    /* Check the number of stored elements. */
    assert(QueueSize(queue) == 2);

    QueueDeinit(queue);
}

int main()
{
    ManipulateNumerics();
    ManipulateObjects();
    ManipulateNumericsCppStyle();
    ManipulateObjectsCppStyle();
    return 0;
}
