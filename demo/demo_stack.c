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
    Stack* stack = StackInit();

    /* Push elements to the stack. */
    StackPush(stack, (void*)(intptr_t)3);
    StackPush(stack, (void*)(intptr_t)2);
    StackPush(stack, (void*)(intptr_t)1);

    /* Check the number of stored elements. */
    assert(StackSize(stack) == 3);

    /* Retrieve the stack top element and remove it. */
    void* element;
    StackTop(stack, &element);
    assert((int)(intptr_t)element == 1);
    StackPop(stack);

    StackTop(stack, &element);
    assert((int)(intptr_t)element == 2);
    StackPop(stack);

    StackTop(stack, &element);
    assert((int)(intptr_t)element == 3);
    StackPop(stack);

    StackDeinit(stack);
}

void ManipulateObjects()
{
    /* We should initialize the container before any operations. */
    Stack* stack = StackInit();
    StackSetClean(stack, CleanObject);

    /* Push elements to the stack. */
    Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 3;
    tuple->second = -3;
    StackPush(stack, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 2;
    tuple->second = -2;
    StackPush(stack, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 1;
    tuple->second = -1;
    StackPush(stack, tuple);

    /* Check the number of stored elements. */
    assert(StackSize(stack) == 3);

    /* Retrieve the stack top element and remove it. */
    void* element;
    StackTop(stack, &element);
    assert(((Tuple*)element)->first == 1);
    StackPop(stack);

    StackTop(stack, &element);
    assert(((Tuple*)element)->first == 2);
    StackPop(stack);

    StackTop(stack, &element);
    assert(((Tuple*)element)->first == 3);
    StackPop(stack);

    StackDeinit(stack);
}

void ManipulateNumericsCppStyle()
{
    /* We should initialize the container before any operations. */
    Stack* stack = StackInit();

    /* Push elements to the stack. */
    stack->push(stack, (void*)3);
    stack->push(stack, (void*)2);
    stack->push(stack, (void*)1);

    /* Check the number of stored elements. */
    assert(StackSize(stack) == 3);

    /* Retrieve the stack top element and remove it. */
    void* element;
    stack->top(stack, &element);
    assert((int)(intptr_t)element == 1);
    stack->pop(stack);

    stack->top(stack, &element);
    assert((int)(intptr_t)element == 2);
    stack->pop(stack);

    stack->top(stack, &element);
    assert((int)(intptr_t)element == 3);
    stack->pop(stack);

    StackDeinit(stack);
}

void ManipulateObjectsCppStyle()
{
    /* We should initialize the container before any operations. */
    Stack* stack = StackInit();
    StackSetClean(stack, CleanObject);

    /* Push elements to the stack. */
    Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 3;
    tuple->second = -3;
    stack->push(stack, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 2;
    tuple->second = -2;
    stack->push(stack, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 1;
    tuple->second = -1;
    stack->push(stack, tuple);

    /* Check the number of stored elements. */
    assert(StackSize(stack) == 3);

    /* Retrieve the stack top element and remove it. */
    void* element;
    stack->top(stack, &element);
    assert(((Tuple*)element)->first == 1);
    stack->pop(stack);

    stack->top(stack, &element);
    assert(((Tuple*)element)->first == 2);
    stack->pop(stack);

    stack->top(stack, &element);
    assert(((Tuple*)element)->first == 3);
    stack->pop(stack);

    StackDeinit(stack);
}

int main()
{
    ManipulateNumerics();
    ManipulateObjects();
    ManipulateNumericsCppStyle();
    ManipulateObjectsCppStyle();
    return 0;
}
