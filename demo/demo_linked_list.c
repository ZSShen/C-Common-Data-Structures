#include "cds.h"


typedef struct Tuple_ {
    int first;
    int second;
} Tuple;


void CleanObject(void* obj)
{
    free(obj);
}


void ManipulateNumerics()
{
    /* We should initialize the container before any operations. */
    List* list = ListInit();

    /* Push the integer elements. */
    ListPushFront(list, (void*)(intptr_t)20);
    ListPushBack(list, (void*)(intptr_t)40);

    /* Insert the elements with the specified indexes. */
    ListInsert(list, 0, (void*)(intptr_t)10);
    ListInsert(list, 3, (void*)(intptr_t)50);
    ListInsert(list, 2, (void*)(intptr_t)30);

    /*---------------------------------------------------------------*
     * Now the list should be: (10)<-->(20)<-->(30)<-->(40)<-->(50)  *
     *---------------------------------------------------------------*/

    /* Iterate through the list. */
    void* element;
    int num = 10;
    ListFirst(list, false);
    while (ListNext(list, &element)) {
        assert((int)(intptr_t)element == num);
        num += 10;
    }

    /* Iterate through the list in the reversed order. */
    num = 50;
    ListFirst(list, true);
    while (ListReverseNext(list, &element)) {
        assert((int)(intptr_t)element == num);
        num -= 10;
    }

    /* Get the element from the list head. */
    ListGetFront(list, &element);
    assert((int)(intptr_t)element == 10);

    /* Get the element from the list tail. */
    ListGetBack(list, &element);
    assert((int)(intptr_t)element == 50);

    /* Get the elements from the specified indexes. */
    ListGetAt(list, 2, &element);
    assert((int)(intptr_t)element == 30);
    ListGetAt(list, 3, &element);
    assert((int)(intptr_t)element == 40);

    /* Replace the element residing at the list head. */
    ListSetFront(list, (void*)(intptr_t)-1);

    /* Replace the element residing at the list tail. */
    ListSetBack(list, (void*)(intptr_t)-5);

    /* Replace the elements residing at the specified indexes. */
    ListSetAt(list, 1, (void*)(intptr_t)-2);
    ListSetAt(list, 2, (void*)(intptr_t)-3);
    ListSetAt(list, 3, (void*)(intptr_t)-4);

    /* Reverse the list. */
    ListReverse(list);

    /*---------------------------------------------------------------*
     * Now the list should be: (-5)<-->(-4)<-->(-3)<-->(-2)<-->(-1)  *
     *---------------------------------------------------------------*/

    /* Remove the element from the list head. */
    ListPopFront(list);

    /* Remove the element from the list tail. */
    ListPopBack(list);

    /* Remove the elements from the specified indexes. */
    ListRemove(list, 1);
    ListRemove(list, 1);

    /* Get the list size. And the remaining element should be (-4). */
    unsigned size = ListSize(list);
    assert(size == 1);

    ListGetFront(list, &element);
    assert((int)(intptr_t)element == -4);

    ListDeinit(list);
}


void ManipulateObjects()
{
    /* We should initialize the container before any operations. */
    List* list = ListInit();
    ListSetClean(list, CleanObject);

    /* Push the object elements. */
    Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 20;
    tuple->second = -20;
    ListPushFront(list, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 40;
    tuple->second = -40;
    ListPushBack(list, tuple);

    /* Insert the elements with the specified indexes. */
    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 10;
    tuple->second = -10;
    ListInsert(list, 0, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 50;
    tuple->second = -50;
    ListInsert(list, 3, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 30;
    tuple->second = -30;
    ListInsert(list, 2, tuple);

    /*---------------------------------------------------------------*
     * Now the list should be: (10)<-->(20)<-->(30)<-->(40)<-->(50)  *
     *---------------------------------------------------------------*/

    /* Iterate through the list. */
    void* element;
    int num = 10;
    ListFirst(list, false);
    while (ListNext(list, &element)) {
        assert(((Tuple*)element)->first == num);
        num += 10;
    }

    /* Iterate through the list in the reversed order. */
    num = 50;
    ListFirst(list, true);
    while (ListReverseNext(list, &element)) {
        assert(((Tuple*)element)->first == num);
        num -= 10;
    }

    /* Get the element from the list head. */
    ListGetFront(list, &element);
    assert(((Tuple*)element)->first == 10);

    /* Get the element from the list tail. */
    ListGetBack(list, &element);
    assert(((Tuple*)element)->first == 50);

    /* Get the elements from the specified indexes. */
    ListGetAt(list, 2, &element);
    assert(((Tuple*)element)->first == 30);
    ListGetAt(list, 3, &element);
    assert(((Tuple*)element)->first == 40);

    /* Replace the element residing at the list head. */
    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = -1;
    tuple->second = 1;
    ListSetFront(list, tuple);

    /* Replace the element residing at the list tail. */
    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = -5;
    tuple->second = 5;
    ListSetBack(list, tuple);

    /* Replace the elements residing at the specified indexes. */
    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = -2;
    tuple->second = 2;
    ListSetAt(list, 1, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = -3;
    tuple->second = 3;
    ListSetAt(list, 2, tuple);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = -4;
    tuple->second = 4;
    ListSetAt(list, 3, tuple);

    /* Reverse the list. */
    ListReverse(list);

    /*---------------------------------------------------------------*
     * Now the list should be: (-5)<-->(-4)<-->(-3)<-->(-2)<-->(-1)  *
     *---------------------------------------------------------------*/

    /* Remove the element from the list head. */
    ListPopFront(list);

    /* Remove the element from the list tail. */
    ListPopBack(list);

    /* Remove the elements from the specified indexes. */
    ListRemove(list, 1);
    ListRemove(list, 1);

    /* Get the list size. And the remaining element should be (-4). */
    unsigned size = ListSize(list);
    assert(size == 1);

    ListGetFront(list, &element);
    assert(((Tuple*)element)->first == -4);

    ListDeinit(list);
}


int main()
{
    ManipulateNumerics();
    ManipulateObjects();
    return 0;
}
