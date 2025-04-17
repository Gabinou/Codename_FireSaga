
#include "unit/mount.h"

void test_mounts(void) {
    Mounts_Load();
    Mount Horsie = gl_mounts[MOUNT_HORSIE];

    nourstest_true(Horsie.price == 2000);
}
