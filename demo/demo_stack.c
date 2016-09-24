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

int main()
{
    ManipulateNumerics();
    ManipulateObjects();
    return 0;
}
