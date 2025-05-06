
#include "unit/mount.h"

void test_mounts(void) {
    Mounts_Load();
    Mount Horsie = gl_mounts[MOUNT_HORSIE];

    nourstest_true(Horsie.type      == MOUNT_TYPE_HORSE);
    nourstest_true(Horsie.price     == 2000);
    nourstest_true(Horsie.carry     == 20);
    nourstest_true(Horsie.move      == 7);
    nourstest_true(Horsie.skill     == 0);
    nourstest_true(Horsie.bond      == 0);
    nourstest_true(Horsie.sex       == true);
    nourstest_true(Horsie.mages     == false);
    nourstest_true(Horsie.promoted  == false);
}
