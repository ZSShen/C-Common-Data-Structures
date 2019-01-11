#include "cds.h"


typedef struct Employ_ {
    int year;
    int level;
    int id;
} Employ;


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
    TreeMap* map = TreeMapInit();

    /* Insert numerics into the map. */
    TreeMapPut(map, (void*)(intptr_t)1, (void*)(intptr_t)9999);
    TreeMapPut(map, (void*)(intptr_t)2, (void*)(intptr_t)999);
    TreeMapPut(map, (void*)(intptr_t)3, (void*)(intptr_t)99);
    TreeMapPut(map, (void*)(intptr_t)4, (void*)(intptr_t)9);

    /* Retrieve the value with the designated key. */
    int val = (int)(intptr_t)TreeMapGet(map, (void*)(intptr_t)1);
    assert(val == 9999);

    /* Iterate through the map. */
    Pair* ptr_pair;
    TreeMapFirst(map);
    int first = 1, second = 9999;
    while ((ptr_pair = TreeMapNext(map)) != NULL) {
        int key = (int)(intptr_t)ptr_pair->key;
        int val = (int)(intptr_t)ptr_pair->value;
        assert(key == first);
        assert(val == second);
        ++first;
        second /= 10;
    }

    first = 4;
    second = 9;
    while ((ptr_pair = TreeMapReverseNext(map)) != NULL) {
        int key = (int)(intptr_t)ptr_pair->key;
        int val = (int)(intptr_t)ptr_pair->value;
        assert(key == first);
        assert(val == second);
        --first;
        second *= 10;
        second += 9;
    }

    /* Remove the key value pair with the designated key. */
    TreeMapRemove(map, (void*)(intptr_t)2);

    /* Check the map keys. */
    assert(TreeMapFind(map, (void*)(intptr_t)1) == true);
    assert(TreeMapFind(map, (void*)(intptr_t)2) == false);
    assert(TreeMapFind(map, (void*)(intptr_t)3) == true);
    assert(TreeMapFind(map, (void*)(intptr_t)4) == true);

    /* Check the pair count in the map. */
    unsigned size = TreeMapSize(map);
    assert(size == 3);

    /* We should deinitialize the container after all the relevant operations. */
    TreeMapDeinit(map);
}

void ManipulateTexts()
{
    char* names[4] = {"Alice\0", "Bob\0", "Chris\0", "David\0"};

    /* We should initialize the container before any operations. */
    TreeMap* map = TreeMapInit();

    /* Set the custom key comparison functions. */
    TreeMapSetCompare(map, CompareKey);

    /* If we plan to delegate the resource clean task to the container, set the
       custom clean functions. */
    TreeMapSetCleanKey(map, CleanKey);
    TreeMapSetCleanValue(map, CleanValue);

    /* Insert complex data payload into the map. */
    char* key = strdup(names[0]);
    Employ* employ = (Employ*)malloc(sizeof(Employ));
    employ->id = 1;
    employ->year = 25;
    employ->level = 100;
    TreeMapPut(map, (void*)key, (void*)employ);

    key = strdup(names[1]);
    employ = (Employ*)malloc(sizeof(Employ));
    employ->id = 2;
    employ->year = 25;
    employ->level = 90;
    TreeMapPut(map, (void*)key, (void*)employ);

    key = strdup(names[2]);
    employ = (Employ*)malloc(sizeof(Employ));
    employ->id = 3;
    employ->year = 25;
    employ->level = 80;
    TreeMapPut(map, (void*)key, (void*)employ);

    key = strdup(names[3]);
    employ = (Employ*)malloc(sizeof(Employ));
    employ->id = 4;
    employ->year = 25;
    employ->level = 70;
    TreeMapPut(map, (void*)key, (void*)employ);


    /* Retrieve the value with the designated key. */
    employ = (Employ*)TreeMapGet(map, (void*)names[0]);
    assert(employ != NULL);
    assert(employ->id == 1);
    assert(employ->year == 25);
    assert(employ->level == 100);

    /* Iterate through the map. */
    Pair* ptr_pair;
    TreeMapFirst(map);
    int first = 0, second = 1;
    while ((ptr_pair = TreeMapNext(map)) != NULL) {
        char* name = (char*)ptr_pair->key;
        employ = (Employ*)ptr_pair->value;
        assert(strcmp(name, names[first]) == 0);
        assert(employ->id == second);
        ++first;
        ++second;
    }

    TreeMapFirst(map);
    first = 3;
    second = 4;
    while ((ptr_pair = TreeMapReverseNext(map)) != NULL) {
        char* name = (char*)ptr_pair->key;
        employ = (Employ*)ptr_pair->value;
        assert(strcmp(name, names[first]) == 0);
        assert(employ->id == second);
        --first;
        --second;
    }

    /* Remove the key value pair with the designated key. */
    TreeMapRemove(map, (void*)names[1]);

    /* Check the map keys. */
    assert(TreeMapFind(map, (void*)names[0]) == true);
    assert(TreeMapFind(map, (void*)names[1]) == false);
    assert(TreeMapFind(map, (void*)names[2]) == true);
    assert(TreeMapFind(map, (void*)names[3]) == true);

    /* Check the pair count in the map. */
    unsigned size = TreeMapSize(map);
    assert(size == 3);

    /* We should deinitialize the container after all the relevant operations. */
    TreeMapDeinit(map);
}

void ManipulateNumericsCppStyle()
{
    /* We should initialize the container before any operations. */
    TreeMap* map = TreeMapInit();

    /* Insert numerics into the map. */
    map->put(map, (void*)(intptr_t)1, (void*)(intptr_t)9999);
    map->put(map, (void*)(intptr_t)2, (void*)(intptr_t)999);
    map->put(map, (void*)(intptr_t)3, (void*)(intptr_t)99);
    map->put(map, (void*)(intptr_t)4, (void*)(intptr_t)9);

    /* Retrieve the value with the designated key. */
    int val = (int)(intptr_t)map->get(map, (void*)(intptr_t)1);
    assert(val == 9999);

    /* Iterate through the map. */
    Pair* ptr_pair;
    map->first(map);
    int first = 1, second = 9999;
    while ((ptr_pair = map->next(map)) != NULL) {
        int key = (int)(intptr_t)ptr_pair->key;
        int val = (int)(intptr_t)ptr_pair->value;
        assert(key == first);
        assert(val == second);
        ++first;
        second /= 10;
    }

    first = 4;
    second = 9;
    while ((ptr_pair = map->reverse_next(map)) != NULL) {
        int key = (int)(intptr_t)ptr_pair->key;
        int val = (int)(intptr_t)ptr_pair->value;
        assert(key == first);
        assert(val == second);
        --first;
        second *= 10;
        second += 9;
    }

    /* Remove the key value pair with the designated key. */
    map->remove(map, (void*)(intptr_t)2);

    /* Check the map keys. */
    assert(map->find(map, (void*)(intptr_t)1) == true);
    assert(map->find(map, (void*)(intptr_t)2) == false);
    assert(map->find(map, (void*)(intptr_t)3) == true);
    assert(map->find(map, (void*)(intptr_t)4) == true);

    /* Check the pair count in the map. */
    unsigned size = map->size(map);
    assert(size == 3);

    /* We should deinitialize the container after all the relevant operations. */
    TreeMapDeinit(map);
}

void ManipulateTextsCppStyle()
{
    char* names[4] = {"Alice\0", "Bob\0", "Chris\0", "David\0"};

    /* We should initialize the container before any operations. */
    TreeMap* map = TreeMapInit();

    /* Set the custom key comparison functions. */
    TreeMapSetCompare(map, CompareKey);

    /* If we plan to delegate the resource clean task to the container, set the
       custom clean functions. */
    TreeMapSetCleanKey(map, CleanKey);
    TreeMapSetCleanValue(map, CleanValue);

    /* Insert complex data payload into the map. */
    char* key = strdup(names[0]);
    Employ* employ = (Employ*)malloc(sizeof(Employ));
    employ->id = 1;
    employ->year = 25;
    employ->level = 100;
    map->put(map, (void*)key, (void*)employ);

    key = strdup(names[1]);
    employ = (Employ*)malloc(sizeof(Employ));
    employ->id = 2;
    employ->year = 25;
    employ->level = 90;
    map->put(map, (void*)key, (void*)employ);

    key = strdup(names[2]);
    employ = (Employ*)malloc(sizeof(Employ));
    employ->id = 3;
    employ->year = 25;
    employ->level = 80;
    map->put(map, (void*)key, (void*)employ);

    key = strdup(names[3]);
    employ = (Employ*)malloc(sizeof(Employ));
    employ->id = 4;
    employ->year = 25;
    employ->level = 70;
    map->put(map, (void*)key, (void*)employ);


    /* Retrieve the value with the designated key. */
    employ = (Employ*)map->get(map, (void*)names[0]);
    assert(employ != NULL);
    assert(employ->id == 1);
    assert(employ->year == 25);
    assert(employ->level == 100);

    /* Iterate through the map. */
    Pair* ptr_pair;
    map->first(map);
    int first = 0, second = 1;
    while ((ptr_pair = map->next(map)) != NULL) {
        char* name = (char*)ptr_pair->key;
        employ = (Employ*)ptr_pair->value;
        assert(strcmp(name, names[first]) == 0);
        assert(employ->id == second);
        ++first;
        ++second;
    }

    map->first(map);
    first = 3;
    second = 4;
    while ((ptr_pair = map->reverse_next(map)) != NULL) {
        char* name = (char*)ptr_pair->key;
        employ = (Employ*)ptr_pair->value;
        assert(strcmp(name, names[first]) == 0);
        assert(employ->id == second);
        --first;
        --second;
    }

    /* Remove the key value pair with the designated key. */
    map->remove(map, (void*)names[1]);

    /* Check the map keys. */
    assert(map->find(map, (void*)names[0]) == true);
    assert(map->find(map, (void*)names[1]) == false);
    assert(map->find(map, (void*)names[2]) == true);
    assert(map->find(map, (void*)names[3]) == true);

    /* Check the pair count in the map. */
    unsigned size = map->size(map);
    assert(size == 3);

    /* We should deinitialize the container after all the relevant operations. */
    TreeMapDeinit(map);
}

int main()
{
    ManipulateNumerics();
    ManipulateTexts();
    ManipulateNumericsCppStyle();
    ManipulateTextsCppStyle();
    return 0;
}
