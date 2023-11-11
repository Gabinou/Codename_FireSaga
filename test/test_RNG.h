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
    PHYSFS_file *fp = PHYSFS_openWrite(PATH_JOIN("build", "RNG.csv"));
    char buffer[6] = {0};
    URN_debug = -1;

    for (size_t i = 0; i < ITERATIONS; i++) {
        i16 out = RNG_URN();
        nourstest_true((out < 100) && (out >= 0));
        stbsp_sprintf(buffer, "%d\n\0", out);
        PHYSFS_writeBytes(fp, buffer, strlen(buffer));
    }
    PHYSFS_close(fp);

    test_RNG_SequenceBreaker();
}

#undef ITERATIONS
#define ITERATIONS 10000000

void bench_RNG() {
    u64 before_ns = tnecs_get_ns();
    struct TINYMT32_T tinymt;
    RNG_Init_tinymt(&tinymt);
    for (int i = 0; i < ITERATIONS; ++i) {
        tinymt32_generate_uint32(&tinymt);
    }
    u64 after_ns    = tnecs_get_ns();
    u64 elapsed_ns  = (after_ns - before_ns);
    u64 elapsed_ms  = elapsed_ns / SOTA_us;
    SDL_Log("Tinymt %d ns", elapsed_ns);
    SDL_Log("Tinymt %d ns / RN", elapsed_ns / ITERATIONS);
    SDL_Log("Tinymt %d ms", elapsed_ms);

    before_ns = tnecs_get_ns();
    for (int i = 0; i < ITERATIONS; ++i) {
        RNG_openBSD_u32(&tinymt, 0, 99);
    }
    after_ns    = tnecs_get_ns();
    elapsed_ns  = (after_ns - before_ns);
    elapsed_ms  = elapsed_ns / SOTA_us;
    SDL_Log("RNG_openBSD_u32 %d ns", elapsed_ns);
    SDL_Log("RNG_openBSD_u32 %d ns / RN", elapsed_ns / ITERATIONS);
    SDL_Log("RNG_openBSD_u32 %d ms", elapsed_ms);

    before_ns = tnecs_get_ns();
    RNG_Init_xoroshiro256ss();
    for (int i = 0; i < ITERATIONS; ++i) {
        next_xoshiro256ss();
    }
    after_ns    = tnecs_get_ns();
    elapsed_ns  = (after_ns - before_ns);
    elapsed_ms  = elapsed_ns / SOTA_us;
    SDL_Log("xoshiro256ss %d ns", elapsed_ns);
    SDL_Log("xoshiro256ss %d ns / RN", elapsed_ns / ITERATIONS);
    SDL_Log("xoshiro256ss %d ms", elapsed_ms);

    before_ns = tnecs_get_ns();
    for (int i = 0; i < ITERATIONS; ++i) {
        RNG_openBSD_u64(0ULL, 99ULL);
    }
    after_ns    = tnecs_get_ns();
    elapsed_ns  = (after_ns - before_ns);
    elapsed_ms  = elapsed_ns / SOTA_us;
    SDL_Log("RNG_openBSD_u64 %d ns", elapsed_ns);
    SDL_Log("RNG_openBSD_u64 %d ns / RN", elapsed_ns / ITERATIONS);
    SDL_Log("RNG_openBSD_u64 %d ms", elapsed_ms);

    /* --- CONCLUSION --- */
    /* xoshiro256ss < 2x faster than tinymt32 */
}