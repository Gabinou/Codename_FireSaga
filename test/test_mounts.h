
#include "unit/mount.h"

void test_mounts(void) {
    Mounts_Load();
    Mount Horsie = gl_mounts[MOUNT_HORSIE];

    nourstest_true(Horsie.type              == MOUNT_HORSE);
    nourstest_true(Horsie.id                == MOUNT_HORSIE);
    nourstest_true(Horsie.stats.price       == 2000);
    nourstest_true(Horsie.rider.carry       == 20);
    nourstest_true(Horsie.bonus.skill       == 0);
    nourstest_true(Horsie.bonus.bond        == 0);
    nourstest_true(Horsie.bonus.bond_unit   == 0);
    nourstest_true(Horsie.stats.sex         == true);
    nourstest_true(Horsie.rider.mages       == false);
    nourstest_true(Horsie.rider.promoted    == false);

    Mount_Bonus Horsie_Bonus = mount_bonuses[MOUNT_HORSIE];
    nourstest_true(Horsie_Bonus.unit_stats.move == 7);
    Mount_Bonus Eagle_Bonus = mount_bonuses[MOUNT_EAGLE];
    nourstest_true(Eagle_Bonus.unit_stats.move == 8);
}
