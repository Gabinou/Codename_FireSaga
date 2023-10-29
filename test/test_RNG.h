#include "nourstest.h"
#include "RNG.h"

void test_RNG_SequenceBreaker() {
    uint_fast16_t rate = 4;
    size_t seq_len = 2;
    nourstest_true(SB_RATE_RISE(rate,
                                seq_len + RNG_SB_SEQ_OFFSET) == (uint_fast16_t)(rate * sb_rise_table[rate /
                                        RNG_SB_BASE_NUM]*sb_rise_table[rate / RNG_SB_BASE_NUM]));
    nourstest_true(SB_RATE_RISE(rate, seq_len + RNG_SB_SEQ_OFFSET) == 5);
    nourstest_true(SB_RATE_DROP(rate,
                                seq_len + RNG_SB_SEQ_OFFSET) == (uint_fast16_t)(rate / sb_drop_table[rate / RNG_SB_BASE_NUM] /
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

void test_RNG() {
    SDL_Log("test_RNG");
    // Actually testing the box-muller transform, the tinyMT is a little more involved.
    // NOt done here because FUCK THAT.
    tinymt32_t tinyMT1;
    tinymt32_t tinyMT2;
    RNG_Init_tinymt(&tinyMT1);
    RNG_Init_tinymt(&tinyMT2);
    nourstest_true(RNG_single_roll(10, 90));
    nourstest_true(!RNG_single_roll(99, 90));
    nourstest_true(!RNG_single_roll(90, 90));
    nourstest_true(RNG_double_roll(10, 100, 90));
    nourstest_true(!RNG_double_roll(100, 100, 90));
    nourstest_true(!RNG_double_roll(80, 100, 90));
    nourstest_true(RNG_URN(&tinyMT1) == RNG_URN(&tinyMT2));
    for (uint_fast16_t i = 0; i < 10000; i++) {
        nourstest_true((RNG_URN(&tinyMT1) < 100) && (RNG_URN(&tinyMT1) >= 0));
    }
    test_RNG_SequenceBreaker();
}

#define ITERATIONS 10000000

void bench_RNG() {
    u64 before_ns = tnecs_get_ns();
    struct TINYMT32_T tinymt;
    RNG_Init_tinymt(&tinymt);
    for (int i = 0; i < ITERATIONS; ++i) {
        tinymt32_generate_uint32(&tinymt);
    }
    u64 after_ns    = tnecs_get_ns();
    u64 elapsed_ms  = (after_ns - before_ns) / SOTA_us;
    SDL_Log("Tinymt %d ms", elapsed_ms);

    before_ns = tnecs_get_ns();
    RNG_Init_xoroshiro256ss();
    for (int i = 0; i < ITERATIONS; ++i) {
        next_xoshiro256ss();
    }
    after_ns    = tnecs_get_ns();
    elapsed_ms  = (after_ns - before_ns) / SOTA_us;
    SDL_Log("xoshiro256ss %d ms", elapsed_ms);
}