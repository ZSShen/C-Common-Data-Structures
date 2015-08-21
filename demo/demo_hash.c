#include "cds.h"


typedef struct _Employ {
    int8_t cYear;
    int8_t cLevel;
    int32_t iId;
} Employ;


int main()
{
    /* Use string as hash key. */
    char *szKey = "1234567890abcdeffedcba0987654321\0";
    uint32_t value = HashMurMur32((Key)szKey, strlen(szKey));

    /* Use integer as hash key. */
    int32_t iKey = 32767;
    value = HashMurMur32((Key)&iKey, sizeof(iKey));
    int64_t lKey = 32767;
    value = HashMurMur32((Key)&lKey, sizeof(lKey));

    /* Use floating point as hash key. */
    float fKey = 32767.0;
    value = HashMurMur32((Key)&fKey, sizeof(float));
    double dKey = 32767.0;
    value = HashMurMur32((Key)&dKey, sizeof(double));

    /* Use structure as hash key. */
    Employ *pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->cYear = 1;
    pEmp->cLevel = 10;
    pEmp->iId = 25692;
    value = HashMurMur32((Key)pEmp, sizeof(Employ));
    free(pEmp);

    return SUCC;
}