#include "math/hash.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/*------------------------------------------------------------*
 *       Test Function Declaration for hash calculation       *
 *------------------------------------------------------------*/
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

    value = HashMurMur32("1", 1);
    printf("%u\n", value);
    value = HashMurMur32("12", 2);
    printf("%u\n", value);
    value = HashMurMur32("123", 3);
    printf("%u\n", value);
    value = HashMurMur32("1234", 4);
    printf("%u\n", value);

    return;
}