#include "cds.h"


typedef struct _Employ {
    char cYear;
    char cLevel;
    int id;
} Employ;


int main()
{
    /* Use string as hash key. */
    char* str = "1234567890abcdeffedcba0987654321\0";
    uint32_t value = HashMurMur32((void*)str, strlen(str));

    /* Use integer as hash key. */
    int key_int = 32767;
    value = HashMurMur32((void*)&key_int, sizeof(int));
    long long key_long = 32767;
    value = HashMurMur32((void*)&key_long, sizeof(long long));

    /* Use floating point as hash key. */
    float key_float = 32767.0;
    value = HashMurMur32((void*)&key_float, sizeof(float));
    double key_double = 32767.0;
    value = HashMurMur32((void*)&key_double, sizeof(double));

    /* Use structure as hash key. */
    Employ* employ = (Employ*)malloc(sizeof(Employ));
    employ->cYear = 1;
    employ->cLevel = 10;
    employ->id = 25692;
    value = HashMurMur32((void*)employ, sizeof(Employ));
    free(employ);

    return 0;
}