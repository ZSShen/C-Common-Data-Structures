#include "vector.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


int main()
{
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    assert(CU_get_registry() != NULL);
    assert(!CU_is_test_running());

    CU_cleanup_registry();

    return 0;
}
