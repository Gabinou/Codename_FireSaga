
#include "enums.h"
#include "platform.h"
#include "nourstest.h"

void test_enums() {

    // nourstest_true(UNIT_ID_NPC_EXP_ZIDINE == 0);
    /* --- TARGET --- */
    nourstest_true (TARGET_OTHER == TARGET_ENEMY +
                    TARGET_NEUTRAL +
                    TARGET_FRIENDLY);
    nourstest_true (TARGET_ANYONE ==    TARGET_OTHER +
                    TARGET_SELF);

    nourstest_true(TARGET_SELF     != TARGET_ENEMY);
    nourstest_true(TARGET_SELF     != TARGET_FRIENDLY);
    nourstest_true(TARGET_SELF     != TARGET_NEUTRAL);
    nourstest_true(TARGET_FRIENDLY != TARGET_NEUTRAL);
    nourstest_true(TARGET_FRIENDLY != TARGET_ENEMY);

    nourstest_true(TARGET_FRIENDLY  & TARGET_ANYONE);
    nourstest_true(TARGET_ENEMY     & TARGET_ANYONE);
    nourstest_true(TARGET_NEUTRAL   & TARGET_ANYONE);
    nourstest_true(TARGET_SELF      & TARGET_ANYONE);

    nourstest_true(TARGET_FRIENDLY  & TARGET_OTHER);
    nourstest_true(TARGET_ENEMY     & TARGET_OTHER);
    nourstest_true(TARGET_NEUTRAL   & TARGET_OTHER);

}