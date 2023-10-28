#include "nourstest.h"
#include "camp.h"
#include "narrative.h"
#include "RNG.h"

void test_camp() {
    
    struct Camp camp  = Camp_default;
    struct Camp camp2 = Camp_default;

    nourstest_true(camp.workers[CAMPJOB_STABLEHAND] == 0);
    Camp_Job_Hire(&camp, UNIT_ID_SILOU, CAMPJOB_STABLEHAND);
    nourstest_true(camp.workers[CAMPJOB_STABLEHAND] == 1);
    nourstest_true(camp.stablehands[0] == UNIT_ID_SILOU);
    nourstest_true(CAMPJOB_STABLEHAND == Camp_hasJob(&camp, UNIT_ID_SILOU));

    Camp_Job_Hire(&camp, UNIT_ID_PIROU, CAMPJOB_STABLEHAND);
    nourstest_true(camp.workers[CAMPJOB_STABLEHAND] == 2);
    nourstest_true(camp.stablehands[1] == UNIT_ID_PIROU);
    nourstest_true(CAMPJOB_STABLEHAND == Camp_hasJob(&camp, UNIT_ID_PIROU));

    Camp_Job_Hire(&camp, UNIT_ID_NICOLE, CAMPJOB_LIBRARIAN);
    nourstest_true(camp.workers[CAMPJOB_LIBRARIAN] == 1);
    nourstest_true(camp.librarians[0] == UNIT_ID_NICOLE);
    nourstest_true(CAMPJOB_LIBRARIAN == Camp_hasJob(&camp, UNIT_ID_NICOLE));

    Camp_Job_Fire(&camp, UNIT_ID_SILOU, CAMPJOB_STABLEHAND);
    nourstest_true(camp.workers[CAMPJOB_STABLEHAND] == 1);
    nourstest_true(camp.stablehands[0] == UNIT_ID_PIROU);
    nourstest_true(camp.stablehands[1] == 0);
    nourstest_true(CAMP_JOB_NULL == Camp_hasJob(&camp, UNIT_ID_SILOU));

    Camp_Job_Forbid(&camp, UNIT_ID_SILOU, CAMPJOB_STABLEHAND);

    jsonio_writeJSON("saves"PHYSFS_SEPARATOR"camp_test.json", &camp, false);
    nourstest_true(PHYSFS_exists("saves"PHYSFS_SEPARATOR"camp_test.json"));

    jsonio_readJSON("saves"PHYSFS_SEPARATOR"camp_test.json", &camp2);
    jsonio_writeJSON("saves"PHYSFS_SEPARATOR"camp_rewrite.json", &camp2, false);
    nourstest_true(PHYSFS_exists("saves"PHYSFS_SEPARATOR"camp_rewrite.json"));
    nourstest_true(Filesystem_fequal("saves"PHYSFS_SEPARATOR"camp_rewrite.json",
                                     "saves"PHYSFS_SEPARATOR"camp_test.json"));
}
