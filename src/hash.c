#include "math/hash.h"


unsigned HashMurMur32(void* key, size_t size)
{
    if (!key || size == 0)
        return 0;

    const unsigned c1 = 0xcc9e2d51;
    const unsigned c2 = 0x1b873593;
    const unsigned r1 = 15;
    const unsigned r2 = 13;
    const unsigned m = 5;
    const unsigned n = 0xe6546b64;

    unsigned hash = 0xdeadbeef;

    const int nblocks = size / 4;
    const unsigned *blocks = (const unsigned*)key;
    int i;
    for (i = 0; i < nblocks; i++) {
        unsigned k = blocks[i];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;

        hash ^= k;
        hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
    }

    const uint8_t *tail = (const uint8_t*) (key + nblocks * 4);
    unsigned k1 = 0;

    switch (size & 3) {
        case 3:
            k1 ^= tail[2] << 16;
        case 2:
            k1 ^= tail[1] << 8;
        case 1:
            k1 ^= tail[0];

            k1 *= c1;
            k1 = (k1 << r1) | (k1 >> (32 - r1));
            k1 *= c2;
            hash ^= k1;
    }

    hash ^= size;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash;
}

unsigned HashDjb2(char* key)
{
    unsigned hash = 5381;
    int c;

    while (c = *key++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}