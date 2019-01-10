#include "cds.h"


unsigned HashKey(void* key)
{
    return HashDjb2((char*)key);
}

int CompareKey(void* lhs, void* rhs)
{
    return strcmp((char*)lhs, (char*)rhs);
}

void CleanKey(void* key)
{
    free(key);
}


void ManipulateNumerics()
{
    /* We should initialize the container before any operations. */
    HashSet* set_lhs = HashSetInit();
    HashSet* set_rhs = HashSetInit();

    /* Insert numerics into the sets. */
    HashSetAdd(set_lhs, (void*)(intptr_t)1);
    HashSetAdd(set_lhs, (void*)(intptr_t)2);
    HashSetAdd(set_lhs, (void*)(intptr_t)3);
    HashSetAdd(set_lhs, (void*)(intptr_t)4);

    HashSetAdd(set_rhs, (void*)(intptr_t)3);
    HashSetAdd(set_rhs, (void*)(intptr_t)4);
    HashSetAdd(set_rhs, (void*)(intptr_t)5);
    HashSetAdd(set_rhs, (void*)(intptr_t)6);

    /* Remove some elements from the sets. */
    HashSetRemove(set_lhs, (void*)(intptr_t)1);
    HashSetRemove(set_rhs, (void*)(intptr_t)6);

    /* Query for element existence. */
    assert(HashSetFind(set_lhs, (void*)(intptr_t)1) == false);
    assert(HashSetFind(set_lhs, (void*)(intptr_t)2) == true);
    assert(HashSetFind(set_rhs, (void*)(intptr_t)5) == true);
    assert(HashSetFind(set_rhs, (void*)(intptr_t)6) == false);

    /* Iterate through the set. */
    void* element;
    HashSetFirst(set_lhs);
    while ((element = HashSetNext(set_lhs)) != NULL) {
        // Consume the element.
    }

    /* Check the element count in the set. */
    assert(HashSetSize(set_lhs) == 3);
    assert(HashSetSize(set_rhs) == 3);

    /* Perform set union operation. */
    HashSet* merge = HashSetUnion(set_lhs, set_rhs);
    assert(HashSetSize(merge) == 4);
    assert(HashSetFind(merge, (void*)(intptr_t)1) == false);
    assert(HashSetFind(merge, (void*)(intptr_t)2) == true);
    assert(HashSetFind(merge, (void*)(intptr_t)3) == true);
    assert(HashSetFind(merge, (void*)(intptr_t)4) == true);
    assert(HashSetFind(merge, (void*)(intptr_t)5) == true);
    assert(HashSetFind(merge, (void*)(intptr_t)6) == false);

    /* Perform set intersection operation. */
    HashSet* inter = HashSetIntersect(set_lhs, set_rhs);
    assert(HashSetSize(inter) == 2);
    assert(HashSetFind(inter, (void*)(intptr_t)1) == false);
    assert(HashSetFind(inter, (void*)(intptr_t)2) == false);
    assert(HashSetFind(inter, (void*)(intptr_t)3) == true);
    assert(HashSetFind(inter, (void*)(intptr_t)4) == true);
    assert(HashSetFind(inter, (void*)(intptr_t)5) == false);
    assert(HashSetFind(inter, (void*)(intptr_t)6) == false);

    /* Perform set difference operation. */
    HashSet* only_lhs = HashSetDifference(set_lhs, set_rhs);
    assert(HashSetSize(only_lhs) == 1);
    assert(HashSetFind(only_lhs, (void*)(intptr_t)1) == false);
    assert(HashSetFind(only_lhs, (void*)(intptr_t)2) == true);
    assert(HashSetFind(only_lhs, (void*)(intptr_t)3) == false);
    assert(HashSetFind(only_lhs, (void*)(intptr_t)4) == false);
    assert(HashSetFind(only_lhs, (void*)(intptr_t)5) == false);
    assert(HashSetFind(only_lhs, (void*)(intptr_t)6) == false);

    HashSet* only_rhs = HashSetDifference(set_rhs, set_lhs);
    assert(HashSetSize(only_rhs) == 1);
    assert(HashSetFind(only_rhs, (void*)(intptr_t)1) == false);
    assert(HashSetFind(only_rhs, (void*)(intptr_t)2) == false);
    assert(HashSetFind(only_rhs, (void*)(intptr_t)3) == false);
    assert(HashSetFind(only_rhs, (void*)(intptr_t)4) == false);
    assert(HashSetFind(only_rhs, (void*)(intptr_t)5) == true);
    assert(HashSetFind(only_rhs, (void*)(intptr_t)6) == false);

    /* We should deinitialize the container after all the relevant operations. */
    HashSetDeinit(only_lhs);
    HashSetDeinit(only_rhs);
    HashSetDeinit(inter);
    HashSetDeinit(merge);
    HashSetDeinit(set_lhs);
    HashSetDeinit(set_rhs);
}

void ManipulateTexts()
{
    char* names[6] = {"Alice\0", "Bob\0", "Carol\0", "Dave\0", "Oscar\0", "Will\0"};

    /* We should initialize the container before any operations. */
    HashSet* set_lhs = HashSetInit();
    HashSetSetHash(set_lhs, HashKey);
    HashSetSetCompare(set_lhs, CompareKey);
    HashSetSetCleanKey(set_lhs, CleanKey);

    HashSet* set_rhs = HashSetInit();
    HashSetSetHash(set_rhs, HashKey);
    HashSetSetCompare(set_rhs, CompareKey);
    HashSetSetCleanKey(set_rhs, CleanKey);

    /* Insert numerics into the sets. */
    HashSetAdd(set_lhs, strdup(names[0]));
    HashSetAdd(set_lhs, strdup(names[1]));
    HashSetAdd(set_lhs, strdup(names[2]));
    HashSetAdd(set_lhs, strdup(names[3]));

    HashSetAdd(set_rhs, strdup(names[2]));
    HashSetAdd(set_rhs, strdup(names[3]));
    HashSetAdd(set_rhs, strdup(names[4]));
    HashSetAdd(set_rhs, strdup(names[5]));

    /* Remove some elements from the sets. */
    HashSetRemove(set_lhs, names[0]);
    HashSetRemove(set_rhs, names[5]);

    /* Query for element existence. */
    assert(HashSetFind(set_lhs, names[0]) == false);
    assert(HashSetFind(set_lhs, names[1]) == true);
    assert(HashSetFind(set_rhs, names[4]) == true);
    assert(HashSetFind(set_rhs, names[5]) == false);

    /* Iterate through the set. */
    void* element;
    HashSetFirst(set_lhs);
    while ((element = HashSetNext(set_lhs)) != NULL) {
        // Consume the element.
    }

    /* Check the element count in the set. */
    assert(HashSetSize(set_lhs) == 3);
    assert(HashSetSize(set_rhs) == 3);

    /* Perform set union operation. */
    HashSet* merge = HashSetUnion(set_lhs, set_rhs);
    assert(HashSetSize(merge) == 4);
    assert(HashSetFind(merge, names[0]) == false);
    assert(HashSetFind(merge, names[1]) == true);
    assert(HashSetFind(merge, names[2]) == true);
    assert(HashSetFind(merge, names[3]) == true);
    assert(HashSetFind(merge, names[4]) == true);
    assert(HashSetFind(merge, names[5]) == false);

    /* Perform set intersection operation. */
    HashSet* inter = HashSetIntersect(set_lhs, set_rhs);
    assert(HashSetSize(inter) == 2);
    assert(HashSetFind(inter, names[0]) == false);
    assert(HashSetFind(inter, names[1]) == false);
    assert(HashSetFind(inter, names[2]) == true);
    assert(HashSetFind(inter, names[3]) == true);
    assert(HashSetFind(inter, names[4]) == false);
    assert(HashSetFind(inter, names[5]) == false);

    /* Perform set difference operation. */
    HashSet* only_lhs = HashSetDifference(set_lhs, set_rhs);
    assert(HashSetSize(only_lhs) == 1);
    assert(HashSetFind(only_lhs, names[0]) == false);
    assert(HashSetFind(only_lhs, names[1]) == true);
    assert(HashSetFind(only_lhs, names[2]) == false);
    assert(HashSetFind(only_lhs, names[3]) == false);
    assert(HashSetFind(only_lhs, names[4]) == false);
    assert(HashSetFind(only_lhs, names[5]) == false);

    HashSet* only_rhs = HashSetDifference(set_rhs, set_lhs);
    assert(HashSetSize(only_rhs) == 1);
    assert(HashSetFind(only_rhs, names[0]) == false);
    assert(HashSetFind(only_rhs, names[1]) == false);
    assert(HashSetFind(only_rhs, names[2]) == false);
    assert(HashSetFind(only_rhs, names[3]) == false);
    assert(HashSetFind(only_rhs, names[4]) == true);
    assert(HashSetFind(only_rhs, names[5]) == false);

    /* We should deinitialize the container after all the relevant operations. */
    HashSetDeinit(only_lhs);
    HashSetDeinit(only_rhs);
    HashSetDeinit(inter);
    HashSetDeinit(merge);
    HashSetDeinit(set_lhs);
    HashSetDeinit(set_rhs);
}

void ManipulateNumericsCppStyle()
{
    /* We should initialize the container before any operations. */
    HashSet* set_lhs = HashSetInit();
    HashSet* set_rhs = HashSetInit();

    /* Insert numerics into the sets. */
    set_lhs->add(set_lhs, (void*)1);
    set_lhs->add(set_lhs, (void*)2);
    set_lhs->add(set_lhs, (void*)3);
    set_lhs->add(set_lhs, (void*)4);

    set_rhs->add(set_rhs, (void*)3);
    set_rhs->add(set_rhs, (void*)4);
    set_rhs->add(set_rhs, (void*)5);
    set_rhs->add(set_rhs, (void*)6);

    /* Remove some elements from the sets. */
    set_lhs->remove(set_lhs, (void*)1);
    set_rhs->remove(set_rhs, (void*)6);

    /* Query for element existence. */
    assert(set_lhs->find(set_lhs, (void*)1) == false);
    assert(set_lhs->find(set_lhs, (void*)2) == true);
    assert(set_rhs->find(set_rhs, (void*)5) == true);
    assert(set_rhs->find(set_rhs, (void*)6) == false);

    /* Iterate through the set. */
    void* element;
    set_lhs->first(set_lhs);
    while ((element = set_lhs->next(set_lhs)) != NULL) {
        // Consume the element.
    }

    /* Check the element count in the set. */
    assert(set_lhs->size(set_lhs) == 3);
    assert(set_lhs->size(set_rhs) == 3);

    /* Perform set union operation. */
    HashSet* merge = HashSetUnion(set_lhs, set_rhs);
    assert(merge->size(merge) == 4);
    assert(merge->find(merge, (void*)1) == false);
    assert(merge->find(merge, (void*)2) == true);
    assert(merge->find(merge, (void*)3) == true);
    assert(merge->find(merge, (void*)4) == true);
    assert(merge->find(merge, (void*)5) == true);
    assert(merge->find(merge, (void*)6) == false);

    /* Perform set intersection operation. */
    HashSet* inter = HashSetIntersect(set_lhs, set_rhs);
    assert(inter->size(inter) == 2);
    assert(inter->find(inter, (void*)1) == false);
    assert(inter->find(inter, (void*)2) == false);
    assert(inter->find(inter, (void*)3) == true);
    assert(inter->find(inter, (void*)4) == true);
    assert(inter->find(inter, (void*)5) == false);
    assert(inter->find(inter, (void*)6) == false);

    /* Perform set difference operation. */
    HashSet* only_lhs = HashSetDifference(set_lhs, set_rhs);
    assert(only_lhs->size(only_lhs) == 1);
    assert(only_lhs->find(only_lhs, (void*)1) == false);
    assert(only_lhs->find(only_lhs, (void*)2) == true);
    assert(only_lhs->find(only_lhs, (void*)3) == false);
    assert(only_lhs->find(only_lhs, (void*)4) == false);
    assert(only_lhs->find(only_lhs, (void*)5) == false);
    assert(only_lhs->find(only_lhs, (void*)6) == false);

    HashSet* only_rhs = HashSetDifference(set_rhs, set_lhs);
    assert(only_rhs->size(only_rhs) == 1);
    assert(only_rhs->find(only_rhs, (void*)1) == false);
    assert(only_rhs->find(only_rhs, (void*)2) == false);
    assert(only_rhs->find(only_rhs, (void*)3) == false);
    assert(only_rhs->find(only_rhs, (void*)4) == false);
    assert(only_rhs->find(only_rhs, (void*)5) == true);
    assert(only_rhs->find(only_rhs, (void*)6) == false);

    /* We should deinitialize the container after all the relevant operations. */
    HashSetDeinit(only_lhs);
    HashSetDeinit(only_rhs);
    HashSetDeinit(inter);
    HashSetDeinit(merge);
    HashSetDeinit(set_lhs);
    HashSetDeinit(set_rhs);
}

void ManipulateTextsCppStyle()
{
    char* names[6] = {"Alice\0", "Bob\0", "Carol\0", "Dave\0", "Oscar\0", "Will\0"};

    /* We should initialize the container before any operations. */
    HashSet* set_lhs = HashSetInit();
    HashSetSetHash(set_lhs, HashKey);
    HashSetSetCompare(set_lhs, CompareKey);
    HashSetSetCleanKey(set_lhs, CleanKey);

    HashSet* set_rhs = HashSetInit();
    HashSetSetHash(set_rhs, HashKey);
    HashSetSetCompare(set_rhs, CompareKey);
    HashSetSetCleanKey(set_rhs, CleanKey);

    /* Insert numerics into the sets. */
    set_lhs->add(set_lhs, strdup(names[0]));
    set_lhs->add(set_lhs, strdup(names[1]));
    set_lhs->add(set_lhs, strdup(names[2]));
    set_lhs->add(set_lhs, strdup(names[3]));

    set_rhs->add(set_rhs, strdup(names[2]));
    set_rhs->add(set_rhs, strdup(names[3]));
    set_rhs->add(set_rhs, strdup(names[4]));
    set_rhs->add(set_rhs, strdup(names[5]));

    /* Remove some elements from the sets. */
    set_lhs->remove(set_lhs, names[0]);
    set_rhs->remove(set_rhs, names[5]);

    /* Query for element existence. */
    assert(set_lhs->find(set_lhs, names[0]) == false);
    assert(set_lhs->find(set_lhs, names[1]) == true);
    assert(set_rhs->find(set_rhs, names[4]) == true);
    assert(set_rhs->find(set_rhs, names[5]) == false);

    /* Iterate through the set. */
    void* element;
    set_lhs->first(set_lhs);
    while ((element = set_rhs->next(set_lhs)) != NULL) {
        // Consume the element.
    }

    /* Check the element count in the set. */
    assert(set_lhs->size(set_lhs) == 3);
    assert(set_rhs->size(set_rhs) == 3);

    /* Perform set union operation. */
    HashSet* merge = HashSetUnion(set_lhs, set_rhs);
    assert(merge->size(merge) == 4);
    assert(merge->find(merge, names[0]) == false);
    assert(merge->find(merge, names[1]) == true);
    assert(merge->find(merge, names[2]) == true);
    assert(merge->find(merge, names[3]) == true);
    assert(merge->find(merge, names[4]) == true);
    assert(merge->find(merge, names[5]) == false);

    /* Perform set intersection operation. */
    HashSet* inter = HashSetIntersect(set_lhs, set_rhs);
    assert(inter->size(inter) == 2);
    assert(inter->find(inter, names[0]) == false);
    assert(inter->find(inter, names[1]) == false);
    assert(inter->find(inter, names[2]) == true);
    assert(inter->find(inter, names[3]) == true);
    assert(inter->find(inter, names[4]) == false);
    assert(inter->find(inter, names[5]) == false);

    /* Perform set difference operation. */
    HashSet* only_lhs = HashSetDifference(set_lhs, set_rhs);
    assert(only_lhs->size(only_lhs) == 1);
    assert(only_lhs->find(only_lhs, names[0]) == false);
    assert(only_lhs->find(only_lhs, names[1]) == true);
    assert(only_lhs->find(only_lhs, names[2]) == false);
    assert(only_lhs->find(only_lhs, names[3]) == false);
    assert(only_lhs->find(only_lhs, names[4]) == false);
    assert(only_lhs->find(only_lhs, names[5]) == false);

    HashSet* only_rhs = HashSetDifference(set_rhs, set_lhs);
    assert(only_rhs->size(only_rhs) == 1);
    assert(only_rhs->find(only_rhs, names[0]) == false);
    assert(only_rhs->find(only_rhs, names[1]) == false);
    assert(only_rhs->find(only_rhs, names[2]) == false);
    assert(only_rhs->find(only_rhs, names[3]) == false);
    assert(only_rhs->find(only_rhs, names[4]) == true);
    assert(only_rhs->find(only_rhs, names[5]) == false);

    /* We should deinitialize the container after all the relevant operations. */
    HashSetDeinit(only_lhs);
    HashSetDeinit(only_rhs);
    HashSetDeinit(inter);
    HashSetDeinit(merge);
    HashSetDeinit(set_lhs);
    HashSetDeinit(set_rhs);
}

int main()
{
    ManipulateNumerics();
    ManipulateTexts();
    ManipulateNumericsCppStyle();
    ManipulateTextsCppStyle();
    return 0;
}
