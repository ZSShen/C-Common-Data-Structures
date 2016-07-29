#include "cds.h"


typedef struct Employ_ {
    char year;
    char level;
    int id;
} Employ;

void CleanValue(void* value)
{
    free(value);
}


int main()
{
    char* names[3] = {"Alice\0", "Bob\0", "Wesker\0"};

    /* We should initialize the DS before any operations. */
    HashMap* map = HashMapInit();
    if (!map)
        return -1;

    /* If we plan to delegate the resource clean task to the DS, please set the
       custom clean functions. */
    HashMapSetCleanValue(map, CleanValue);

    /* Insert key value pairs into the map. */
    Employ* employ = (Employ*)malloc(sizeof(Employ));
    employ->id = 1;
    employ->year = 25;
    employ->level = 100;
    HashMapPut(map, (void*)names[0], (void*)employ);

    employ = (Employ*)malloc(sizeof(Employ));
    employ->id = 2;
    employ->year = 25;
    employ->level = 90;
    HashMapPut(map, (void*)names[1], (void*)employ);

    employ = (Employ*)malloc(sizeof(Employ));
    employ->id = 3;
    employ->year = 25;
    employ->level = 80;
    HashMapPut(map, (void*)names[2], (void*)employ);

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
    assert(HashMapFind(map, (void*)names[0]) == true);
    assert(HashMapFind(map, (void*)names[1]) == false);
    assert(HashMapFind(map, (void*)names[2]) == true);

    /* Check the pair count in the map. */
    unsigned size = HashMapSize(map);
    assert(size == 2);

    /* You should deinitialize the DS after all the relevant tasks. */
    HashMapDeinit(map);

    return 0;
}
