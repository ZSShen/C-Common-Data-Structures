#include "math/hash.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/*------------------------------------------------------------*
 *       Test Function Declaration for hash calculation       *
 *------------------------------------------------------------*/
typedef struct _Employ {
    int8_t cYear;
    int8_t cLevel;
    int32_t iId;
} Employ;

int32_t AddBasicSuite();
void TestMurMur32();


int32_t main()
{
    int32_t rc = SUCC;

    if (CU_initialize_registry() != CUE_SUCCESS) {
        rc = CU_get_error();
        goto EXIT;
    }

    if (AddBasicSuite() != SUCC) {
        rc = CU_get_error();
        goto CLEAN;
    }

    /* Launch all the tests. */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

CLEAN:
    CU_cleanup_registry();
EXIT:
    return rc;
}


/*------------------------------------------------------------*
 *      Test Function implementation for hash calculation     *
 *------------------------------------------------------------*/
int32_t AddBasicSuite()
{
    CU_pSuite pSuite = CU_add_suite("Hash Value Calculation", NULL, NULL);
    if (!pSuite)
        return ERR_REG;

    CU_pTest pTest = CU_add_test(pSuite, "MurMur hash V3", TestMurMur32);
    if (!pTest)
        return ERR_REG;

    return SUCC;
}

void TestMurMur32()
{
    uint32_t value = HashMurMur32(NULL, 32);
    CU_ASSERT_EQUAL(value, 0);

    value = HashMurMur32("NULL", 0);
    CU_ASSERT_EQUAL(value, 0);

    /* Test string key. */
    value = HashMurMur32((Key)"1", 1);
    value = HashMurMur32((Key)"12", 2);
    value = HashMurMur32((Key)"123", 3);
    value = HashMurMur32((Key)"1234", 4);
    value = HashMurMur32((Key)"12345", 5);

    /* Test integer key. */
    int32_t iKey = 32767;
    value = HashMurMur32((Key)&iKey, sizeof(int32_t));
    int64_t lKey = 32767;
    value = HashMurMur32((Key)&lKey, sizeof(int64_t));

    /* Test Floating point key. */
    float fKey = 32767.0;
    value = HashMurMur32((Key)&fKey, sizeof(float));
    double dKey = 32767.0;
    value = HashMurMur32((Key)&dKey, sizeof(double));

    /* Test structure key. */
    Employ *pEmp = (Employ*)malloc(sizeof(Employ));
    memset(pEmp, 0, sizeof(Employ));
    pEmp->cYear = 1;
    pEmp->cLevel = 10;
    pEmp->iId = 25692;
    value = HashMurMur32((Key)pEmp, sizeof(Employ));
    free(pEmp);

    return;
}