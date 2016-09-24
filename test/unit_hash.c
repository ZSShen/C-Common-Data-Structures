#include "math/hash.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/*------------------------------------------------------------*
 *       Test Function Declaration for hash calculation       *
 *------------------------------------------------------------*/
typedef struct _Employ {
    char year;
    char level;
    int id;
} Employ;

bool AddBasicSuite();
void TestMurMur32();


int main()
{
    int rc = 0;

    if (CU_initialize_registry() != CUE_SUCCESS) {
        rc = CU_get_error();
        goto EXIT;
    }

    if (AddBasicSuite() == false) {
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
bool AddBasicSuite()
{
    CU_pSuite suite = CU_add_suite("Hash Value Calculation", NULL, NULL);
    if (!suite)
        return false;

    CU_pTest test = CU_add_test(suite, "MurMur hash V3", TestMurMur32);
    if (!test)
        return false;

    return true;
}

void TestMurMur32()
{
    unsigned value = HashMurMur32(NULL, 32);
    CU_ASSERT_EQUAL(value, 0);

    value = HashMurMur32("NULL", 0);
    CU_ASSERT_EQUAL(value, 0);

    /* Test string key. */
    value = HashMurMur32((void*)"1", 1);
    value = HashMurMur32((void*)"12", 2);
    value = HashMurMur32((void*)"123", 3);
    value = HashMurMur32((void*)"1234", 4);
    value = HashMurMur32((void*)"12345", 5);

    /* Test integer key. */
    int key_int = 32767;
    value = HashMurMur32((void*)&key_int, sizeof(int));
    long long key_long = 32767;
    value = HashMurMur32((void*)&key_long, sizeof(long long));

    /* Test Floating point key. */
    float key_float = 32767.0;
    value = HashMurMur32((void*)&key_float, sizeof(float));
    double key_double = 32767.0;
    value = HashMurMur32((void*)&key_double, sizeof(double));

    /* Test structure key. */
    Employ* employ = (Employ*)malloc(sizeof(Employ));
    memset(employ, 0, sizeof(Employ));
    employ->year = 1;
    employ->level = 10;
    employ->id = 25692;
    value = HashMurMur32((void*)employ, sizeof(Employ));
    free(employ);

    return;
}