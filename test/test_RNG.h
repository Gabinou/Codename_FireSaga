#include "nourstest.h"
#include "RNG.h"

void test_RNG_SequenceBreaker() {
    u16 rate = 4;
    size_t seq_len = 2;
    nourstest_true(SB_RATE_RISE(rate,
                                seq_len + RNG_SB_SEQ_OFFSET) == (u16)(rate * sb_rise_table[rate /
                                        RNG_SB_BASE_NUM]*sb_rise_table[rate / RNG_SB_BASE_NUM]));
    nourstest_true(SB_RATE_RISE(rate, seq_len + RNG_SB_SEQ_OFFSET) == 5);
    nourstest_true(SB_RATE_DROP(rate,
                                seq_len + RNG_SB_SEQ_OFFSET) == (u16)(rate / sb_drop_table[rate / RNG_SB_BASE_NUM] /
                                        sb_drop_table[rate / RNG_SB_BASE_NUM]));
    nourstest_true(SB_RATE_DROP(rate, seq_len + RNG_SB_SEQ_OFFSET) == 3);
    nourstest_true(SB_Rate_Drop(rate, 1) == rate);
    nourstest_true(SB_RATE_DROP(rate, 1) == rate);

    struct RNG_Sequence test_sequence = {0};

    RNG_checkSequence_twoWay(&test_sequence, 0);
    nourstest_true(test_sequence.len == 1);
    nourstest_true(test_sequence.hit == 0);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    nourstest_true(test_sequence.len == 1);
    nourstest_true(test_sequence.hit == 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    nourstest_true(test_sequence.len == 2);
    nourstest_true(test_sequence.hit == 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    nourstest_true(test_sequence.len == 3);
    nourstest_true(test_sequence.hit == 1);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    nourstest_true(test_sequence.len == 1);
    nourstest_true(test_sequence.hit == 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    nourstest_true(test_sequence.len == 2);
    nourstest_true(test_sequence.hit == 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    nourstest_true(test_sequence.len == 3);
    nourstest_true(test_sequence.hit == 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    nourstest_true(test_sequence.len == 4);
    nourstest_true(test_sequence.hit == 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    nourstest_true(test_sequence.len == RNG_SEQ_MAX);
    nourstest_true(test_sequence.hit == 0);
    RNG_checkSequence_twoWay(&test_sequence, 0);
    nourstest_true(test_sequence.len == RNG_SEQ_MAX);
    nourstest_true(test_sequence.hit == 0);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    nourstest_true(test_sequence.len == 1);
    nourstest_true(test_sequence.hit == 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    RNG_checkSequence_twoWay(&test_sequence, 1);
    nourstest_true(test_sequence.len == RNG_SEQ_MAX);
    nourstest_true(test_sequence.hit == 1);
}

#undef ITERATIONS
#define ITERATIONS 1000000
void test_RNG() {
    SDL_Log("test_RNG");
    PHYSFS_file *fp = PHYSFS_openWrite(PATH_JOIN("build", "RNG.txt"));
    char buffer[8] = {0};
    URN_debug = -1;

    for (size_t i = 0; i < ITERATIONS; i++) {
        i16 out = RNG_URN();
        nourstest_true((out < 100) && (out >= 0));
        sprintf(buffer, "%d", out);
        PHYSFS_writeBytes(fp, buffer, strlen(buffer));
        PHYSFS_writeBytes(fp, "\n", strlen("\n"));
    }
    PHYSFS_close(fp);

    test_RNG_SequenceBreaker();
}
