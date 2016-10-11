#include "cds.h"


typedef struct Employ_ {
    int year;
    int level;
    int id;
} Employ;


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

void CleanValue(void* value)
{
    free(value);
}


void ManipulateNumerics()
{
    /* We should initialize the container before any operations. */
    HashMap* map = HashMapInit();

    /* Insert numerics into the map. */
    HashMapPut(map, (void*)(intptr_t)1, (void*)(intptr_t)999);
    HashMapPut(map, (void*)(intptr_t)2, (void*)(intptr_t)99);
    HashMapPut(map, (void*)(intptr_t)3, (void*)(intptr_t)9);

    /* Retrieve the value with the designated key. */
    int val = (int)(intptr_t)HashMapGet(map, (void*)(intptr_t)1);
    assert(val == 999);

    /* Iterate through the map. */
    Pair* ptr_pair;
    HashMapFirst(map);
    while ((ptr_pair = HashMapNext(map)) != NULL) {
        int key = (int)(intptr_t)ptr_pair->key;
        int val = (int)(intptr_t)ptr_pair->value;
    }

    /* Remove the key value pair with the designated key. */
    HashMapRemove(map, (void*)(intptr_t)2);

    /* Check the map keys. */
    assert(HashMapContain(map, (void*)(intptr_t)1) == true);
    assert(HashMapContain(map, (void*)(intptr_t)2) == false);
    assert(HashMapContain(map, (void*)(intptr_t)3) == true);

    /* Check the pair count in the map. */
    unsigned size = HashMapSize(map);
    assert(size == 2);

    /* We should deinitialize the container after all the relevant operations. */
    HashMapDeinit(map);
}

void ManipulateTexts()
{
    char* names[3] = {"Alice\0", "Bob\0", "Chris\0"};

    /* We should initialize the container before any operations. */
    HashMap* map = HashMapInit();

    /* Set the custom hash value generator and key comparison functions. */
    HashMapSetHash(map, HashKey);
    HashMapSetCompare(map, CompareKey);

    /* If we plan to delegate the resource clean task to the container, set the
       custom clean functions. */
    HashMapSetCleanKey(map, CleanKey);
    HashMapSetCleanValue(map, CleanValue);

    /* Insert complex data payload into the map. */
    char* key = strdup(names[0]);
    Employ* employ = (Employ*)malloc(sizeof(Employ));
    employ->id = 1;
    employ->year = 25;
    employ->level = 100;
    HashMapPut(map, (void*)key, (void*)employ);

    key = strdup(names[1]);
    employ = (Employ*)malloc(sizeof(Employ));
    employ->id = 2;
    employ->year = 25;
    employ->level = 90;
    HashMapPut(map, (void*)key, (void*)employ);

    key = strdup(names[2]);
    employ = (Employ*)malloc(sizeof(Employ));
    employ->id = 3;
    employ->year = 25;
    employ->level = 80;
    HashMapPut(map, (void*)key, (void*)employ);

    /* Retrieve the value with the designated key. */
    employ = (Employ*)HashMapGet(map, (void*)names[0]);
    assert(employ != NULL);
    assert(employ->id == 1);
    assert(employ->year == 25);
    assert(employ->level == 100);

    /* Iterate through the map. */
    Pair* ptr_pair;
    HashMapFirst(map);
    while ((ptr_pair = HashMapNext(map)) != NULL) {
        char* name = (char*)ptr_pair->key;
        employ = (Employ*)ptr_pair->value;
    }

    /* Remove the key value pair with the designated key. */
    HashMapRemove(map, (void*)names[1]);

    /* Check the map keys. */
    assert(HashMapContain(map, (void*)names[0]) == true);
    assert(HashMapContain(map, (void*)names[1]) == false);
    assert(HashMapContain(map, (void*)names[2]) == true);

    /* Check the pair count in the map. */
    unsigned size = HashMapSize(map);
    assert(size == 2);

    /* We should deinitialize the container after all the relevant operations. */
    HashMapDeinit(map);
}

int main()
{
    ManipulateNumerics();
    ManipulateTexts();
    return 0;
}
