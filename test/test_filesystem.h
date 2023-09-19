#include "nourstest.h"
#include "filesystem.h"

void test_filesystem() {
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);
    // XML realted files are not necessary to test.
    // actually, many functions in filesystem have no need to be tested...
}
