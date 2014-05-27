#include "ordered_map.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SIZE_DATA 10000

int main() {
    OrderedMap *pMap;

    /* Initialize the random seed. */
    srand(time(NULL));

    /* Create the ordered map. */
    OrderedMap_init(pMap);

    /* Free the ordered map. */
    OrderedMap_deinit(pMap);

    return 0;
}
