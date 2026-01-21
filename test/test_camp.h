
#include "nourstest.h"
#include "camp.h"
#include "scene.h"
#include "RNG.h"

void test_camp() {
    struct Camp camp  = Camp_default;
    struct Camp camp2 = Camp_default;

    nourstest_true(camp.jobs.workers[CAMPJOB_STABLEHAND] == 0);
    Camp_Job_Hire(&camp, UNIT_ID_SILOU, CAMPJOB_STABLEHAND);
    nourstest_true(camp.jobs.workers[CAMPJOB_STABLEHAND] == 1);
    nourstest_true(camp.jobs.stablehands[0] == UNIT_ID_SILOU);
    nourstest_true(CAMPJOB_STABLEHAND == Camp_hasJob(&camp, UNIT_ID_SILOU));

    Camp_Job_Hire(&camp, UNIT_ID_PIROU, CAMPJOB_STABLEHAND);
    nourstest_true(camp.jobs.workers[CAMPJOB_STABLEHAND] == 2);
    nourstest_true(camp.jobs.stablehands[1] == UNIT_ID_PIROU);
    nourstest_true(CAMPJOB_STABLEHAND == Camp_hasJob(&camp, UNIT_ID_PIROU));

    Camp_Job_Hire(&camp, UNIT_ID_NICOLE, CAMPJOB_LIBRARIAN);
    nourstest_true(camp.jobs.workers[CAMPJOB_LIBRARIAN] == 1);
    nourstest_true(camp.jobs.librarians[0] == UNIT_ID_NICOLE);
    nourstest_true(CAMPJOB_LIBRARIAN == Camp_hasJob(&camp, UNIT_ID_NICOLE));

    Camp_Job_Fire(&camp, UNIT_ID_SILOU, CAMPJOB_STABLEHAND);
    nourstest_true(camp.jobs.workers[CAMPJOB_STABLEHAND] == 1);
    nourstest_true(camp.jobs.stablehands[0] == UNIT_ID_PIROU);
    nourstest_true(camp.jobs.stablehands[1] == 0);
    nourstest_true(CAMP_JOB_NULL == Camp_hasJob(&camp, UNIT_ID_SILOU));

    Camp_Job_Forbid(&camp, UNIT_ID_SILOU, CAMPJOB_STABLEHAND);

    IES_assert(!PHYSFS_exists(PATH_JOIN("data.bsa", "saves", "camp_test.json")));
    Filesystem_searchpath();
    IES_assert(PHYSFS_exists(PATH_JOIN("saves", "camp_test.json")));
    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "camp_test.json")), &camp, false);
    nourstest_true(PHYSFS_exists(PATH_JOIN("saves", "camp_test.json")));

    jsonio_readJSON(s8_literal(PATH_JOIN("saves", "camp_test.json")), &camp2);
    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "camp_rewrite.json")), &camp2, false);
    nourstest_true(PHYSFS_exists(PATH_JOIN("saves", "camp_rewrite.json")));
    nourstest_true(Filesystem_fequal(PATH_JOIN("saves", "camp_rewrite.json"),
                                     PATH_JOIN("saves", "camp_test.json")));

    Camp_Free(&camp);
    Camp_Free(&camp2);
}
