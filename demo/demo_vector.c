#include "cds.h"


static const int DEFAULT_CAPACITY = 32;

typedef struct Tuple_ {
    int first;
    int second;
} Tuple;


int CompareNumber(const void* lhs, const void* rhs)
{
    int num_lhs = *((int*)lhs);
    int num_rhs = *((int*)rhs);
    if (num_lhs == num_rhs)
        return 0;
    return (num_lhs > num_rhs)? 1 : -1;
}

int CompareObject(const void* lhs, const void* rhs)
{
    Tuple* tpl_lhs = *((Tuple**)lhs);
    Tuple* tpl_rhs = *((Tuple**)rhs);
    if (tpl_lhs->first == tpl_rhs->first)
        return 0;
    return (tpl_lhs->first > tpl_rhs->first)? 1 : -1;
}

void CleanObject(void* obj)
{
    free(obj);
}


void ManipulateNumerics()
{
    /* We should initialize the container before any operations. */
    Vector* vector = VectorInit(DEFAULT_CAPACITY);

    /* Push the integer elements. */
    VectorPushBack(vector, (void*)(intptr_t)3);
    VectorPushBack(vector, (void*)(intptr_t)4);

    /* Insert the elements at the specified indexes. */
    VectorInsert(vector, 0, (void*)(intptr_t)1);
    VectorInsert(vector, 1, (void*)(intptr_t)2);

    /*---------------------------------------------------------------*
     * Now the vector should be: [1] | [2] | [3] | [4]               *
     *---------------------------------------------------------------*/

    /* Iterate through the vector. */
    int num = 1;
    void* elem;
    VectorFirst(vector, false);
    while (VectorNext(vector, &elem)) {
        assert((intptr_t)(void*)elem == num);
        ++num;
    }

    /* Reversely iterate through the vector. */
    num = 4;
    VectorFirst(vector, true);
    while (VectorReverseNext(vector, &elem)) {
        assert((intptr_t)(void*)elem == num);
        --num;
    }

    /* Get the elements from the specified indexes. */
    VectorGet(vector, 0, &elem);
    assert((intptr_t)(void*)elem == 1);
    VectorGet(vector, 3, &elem);
    assert((intptr_t)(void*)elem == 4);

    /* Replace the elements at the specified indexes. */
    VectorSet(vector, 0, (void*)(intptr_t)10);
    VectorSet(vector, 3, (void*)(intptr_t)40);

    /*---------------------------------------------------------------*
     * Now the vector should be: [10] | [2] | [3] | [40]             *
     *---------------------------------------------------------------*/

    /* Get the number of stored elements. */
    unsigned size = VectorSize(vector);
    assert(size == 4);

    /* Sort the integer elements. */
    VectorSort(vector, CompareNumber);

    /*---------------------------------------------------------------*
     * Now the vector should be: [2] | [3] | [10] | [40]             *
     *---------------------------------------------------------------*/

    /* Remove the elements at the specified indexes. */
    VectorRemove(vector, 3);
    VectorRemove(vector, 0);

    /*---------------------------------------------------------------*
     * Now the vector should be: [3] | [10]                          *
     *---------------------------------------------------------------*/

    VectorGet(vector, 0, &elem);
    assert((int)(intptr_t)elem == 3);
    VectorGet(vector, 1, &elem);
    assert((int)(intptr_t)elem == 10);

    /* Pop the elements. */
    VectorPopBack(vector);
    VectorPopBack(vector);
    assert(VectorSize(vector) == 0);

    VectorDeinit(vector);
}

void ManipulateObjects()
{
    /* We should initialize the container before any operations. */
    Vector* vector = VectorInit(DEFAULT_CAPACITY);
    VectorSetClean(vector, CleanObject);

    /* Push the object elements. */
    Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 3;
    tuple->second = -3;
    VectorPushBack(vector, tuple);
    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 4;
    tuple->second = -4;
    VectorPushBack(vector, tuple);

    /* Insert the elements at the specified indexes. */
    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 1;
    tuple->second = -1;
    VectorInsert(vector, 0, tuple);
    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 2;
    tuple->second = -2;
    VectorInsert(vector, 1, tuple);

    /*---------------------------------------------------------------*
     * Now the vector should be: [1] | [2] | [3] | [4]               *
     *---------------------------------------------------------------*/

    /* Iterate through the vector. */
    int num = 1;
    void* elem;
    VectorFirst(vector, false);
    while (VectorNext(vector, &elem)) {
        assert(((Tuple*)elem)->first == num);
        ++num;
    }

    /* Reversely iterate through the vector. */
    num = 4;
    VectorFirst(vector, true);
    while (VectorReverseNext(vector, &elem)) {
        assert(((Tuple*)elem)->first == num);
        --num;
    }

    /* Get the elements from the specified indexes. */
    VectorGet(vector, 0, &elem);
    assert(((Tuple*)elem)->first == 1);
    VectorGet(vector, 3, &elem);
    assert(((Tuple*)elem)->first == 4);

    /* Replace the elements at the specified indexes. */
    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 10;
    tuple->second = -10;
    VectorSet(vector, 0, tuple);
    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 40;
    tuple->second = -40;
    VectorSet(vector, 3, tuple);

    /*---------------------------------------------------------------*
     * Now the vector should be: [10] | [2] | [3] | [40]             *
     *---------------------------------------------------------------*/

    /* Get the number of stored elements. */
    unsigned size = VectorSize(vector);
    assert(size == 4);

    /* Sort the integer elements. */
    VectorSort(vector, CompareObject);

    /*---------------------------------------------------------------*
     * Now the vector should be: [2] | [3] | [10] | [40]             *
     *---------------------------------------------------------------*/

    /* Remove the elements at the specified indexes. */
    VectorRemove(vector, 3);
    VectorRemove(vector, 0);

    /*---------------------------------------------------------------*
     * Now the vector should be: [3] | [10]                          *
     *---------------------------------------------------------------*/

    VectorGet(vector, 0, &elem);
    assert(((Tuple*)elem)->first == 3);
    VectorGet(vector, 1, &elem);
    assert(((Tuple*)elem)->first == 10);

    /* Pop the elements. */
    VectorPopBack(vector);
    VectorPopBack(vector);
    assert(VectorSize(vector) == 0);

    VectorDeinit(vector);
}

void ManipulateNumericsCppStyle()
{
    /* We should initialize the container before any operations. */
    Vector* vector = VectorInit(DEFAULT_CAPACITY);

    /* Push the integer elements. */
    // VectorPushBack(vector, (void*)(intptr_t)3);
    // VectorPushBack(vector, (void*)(intptr_t)4);
    vector->push_back(vector, (void*)3);
    vector->push_back(vector, (void*)4);

    /* Insert the elements at the specified indexes. */
    // VectorInsert(vector, 0, (void*)(intptr_t)1);
    // VectorInsert(vector, 1, (void*)(intptr_t)2);
    vector->insert(vector, 0, (void*)1);
    vector->insert(vector, 1, (void*)2);

    /*---------------------------------------------------------------*
     * Now the vector should be: [1] | [2] | [3] | [4]               *
     *---------------------------------------------------------------*/

    /* Iterate through the vector. */
    int num = 1;
    void* elem;
    // VectorFirst(vector, false);
    vector->first(vector, false);
    // while (VectorNext(vector, &elem)) {
    while (vector->next(vector, &elem)) {
        printf ("%ld ", (intptr_t)elem);
        assert((intptr_t)(void*)elem == num);
        ++num;
    }
    printf("\n");

    /* Reversely iterate through the vector. */
    num = 4;
    // VectorFirst(vector, true);
    vector->first(vector, true);
    while (vector->reverse_next(vector, &elem)) {
        printf ("%ld ", (intptr_t)(void*)elem);
        assert((intptr_t)(void*)elem == num);
        --num;
    }
    printf("\n");


    /* Get the elements from the specified indexes. */
    // VectorGet(vector, 0, &elem);
    vector->get(vector, 0, &elem);
    assert((intptr_t)(void*)elem == 1);
    // VectorGet(vector, 3, &elem);
    vector->get(vector, 3, &elem);
    assert((intptr_t)(void*)elem == 4);

    /* Replace the elements at the specified indexes. */
    // VectorSet(vector, 0, (void*)(intptr_t)10);
    // VectorSet(vector, 3, (void*)(intptr_t)40);
    vector->set(vector, 0, (void*)10);
    vector->set(vector, 3, (void*)40);

    /*---------------------------------------------------------------*
     * Now the vector should be: [10] | [2] | [3] | [40]             *
     *---------------------------------------------------------------*/

    /* Get the number of stored elements. */
    // unsigned size = VectorSize(vector);
    unsigned size = vector->size(vector);
    assert(size == 4);

    /* Sort the integer elements. */
    // VectorSort(vector, CompareNumber);
    vector->sort(vector, CompareNumber);

    /*---------------------------------------------------------------*
     * Now the vector should be: [2] | [3] | [10] | [40]             *
     *---------------------------------------------------------------*/

    /* Remove the elements at the specified indexes. */
    // VectorRemove(vector, 3);
    // VectorRemove(vector, 0);
    vector->remove(vector, 3);
    vector->remove(vector, 0);

    /*---------------------------------------------------------------*
     * Now the vector should be: [3] | [10]                          *
     *---------------------------------------------------------------*/

    // VectorGet(vector, 0, &elem);
    vector->get(vector, 0, &elem);
    assert((int)(intptr_t)elem == 3);
    // VectorGet(vector, 1, &elem);
    vector->get(vector, 1, &elem);
    assert((int)(intptr_t)elem == 10);

    /* Pop the elements. */
    // VectorPopBack(vector);
    // VectorPopBack(vector);
    vector->pop_back(vector);
    vector->pop_back(vector);
    assert(VectorSize(vector) == 0);

    VectorDeinit(vector);
}

void ManipulateObjectsCppStyle()
{
    /* We should initialize the container before any operations. */
    Vector* vector = VectorInit(DEFAULT_CAPACITY);
    VectorSetClean(vector, CleanObject);

    /* Push the object elements. */
    Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 3;
    tuple->second = -3;
    // VectorPushBack(vector, tuple);
    vector->push_back(vector, tuple);
    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 4;
    tuple->second = -4;
    // VectorPushBack(vector, tuple);
    vector->push_back(vector, tuple);

    /* Insert the elements at the specified indexes. */
    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 1;
    tuple->second = -1;
    // VectorInsert(vector, 0, tuple);
    vector->insert(vector, 0, tuple);
    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 2;
    tuple->second = -2;
    // VectorInsert(vector, 1, tuple);
    vector->insert(vector, 1, tuple);

    /*---------------------------------------------------------------*
     * Now the vector should be: [1] | [2] | [3] | [4]               *
     *---------------------------------------------------------------*/

    /* Iterate through the vector. */
    int num = 1;
    void* elem;
    // VectorFirst(vector, false);
    vector->first(vector, false);
    // while (VectorNext(vector, &elem)) {
    while (vector->next(vector, &elem)) {
        assert(((Tuple*)elem)->first == num);
        ++num;
    }

    /* Reversely iterate through the vector. */
    num = 4;
    // VectorFirst(vector, true);
    vector->first(vector, true);
    // while (VectorReverseNext(vector, &elem)) {
    while (vector->reverse_next(vector, &elem)) {
        assert(((Tuple*)elem)->first == num);
        --num;
    }

    /* Get the elements from the specified indexes. */
    // VectorGet(vector, 0, &elem);
    vector->get(vector, 0, &elem);
    assert(((Tuple*)elem)->first == 1);
    // VectorGet(vector, 3, &elem);
    vector->get(vector, 3, &elem);
    assert(((Tuple*)elem)->first == 4);

    /* Replace the elements at the specified indexes. */
    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 10;
    tuple->second = -10;
    // VectorSet(vector, 0, tuple);
    vector->set(vector, 0, tuple);
    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->first = 40;
    tuple->second = -40;
    // VectorSet(vector, 3, tuple);
    vector->set(vector, 3, tuple);

    /*---------------------------------------------------------------*
     * Now the vector should be: [10] | [2] | [3] | [40]             *
     *---------------------------------------------------------------*/

    /* Get the number of stored elements. */
    // unsigned size = VectorSize(vector);
    unsigned size = vector->size(vector);
    assert(size == 4);

    /* Sort the integer elements. */
    // VectorSort(vector, CompareObject);
    vector->sort(vector, CompareObject);

    /*---------------------------------------------------------------*
     * Now the vector should be: [2] | [3] | [10] | [40]             *
     *---------------------------------------------------------------*/

    /* Remove the elements at the specified indexes. */
    // VectorRemove(vector, 3);
    // VectorRemove(vector, 0);
    vector->remove(vector, 3);
    vector->remove(vector, 0);

    /*---------------------------------------------------------------*
     * Now the vector should be: [3] | [10]                          *
     *---------------------------------------------------------------*/

    // VectorGet(vector, 0, &elem);
    vector->get(vector, 0, &elem);
    assert(((Tuple*)elem)->first == 3);
    // VectorGet(vector, 1, &elem);
    vector->get(vector, 1, &elem);
    assert(((Tuple*)elem)->first == 10);

    /* Pop the elements. */
    // VectorPopBack(vector);
    // VectorPopBack(vector);
    vector->pop_back(vector);
    vector->pop_back(vector);
    assert(VectorSize(vector) == 0);

    VectorDeinit(vector);
}

int main()
{
    ManipulateNumerics();
    ManipulateObjects();
    ManipulateNumericsCppStyle();
    ManipulateObjectsCppStyle();
    return 0;
}
