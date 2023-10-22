#include "nourstest.h"
#include "bitfields.h"

void test_bitfields() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    nourstest_true(BITFIELD_LEN(1) == 1);
    nourstest_true(BITFIELD_LEN(2) == 1);
    nourstest_true(BITFIELD_LEN(3) == 1);
    nourstest_true(BITFIELD_LEN(4) == 1);
    nourstest_true(BITFIELD_LEN(5) == 1);
    nourstest_true(BITFIELD_LEN(6) == 1);
    nourstest_true(BITFIELD_LEN(7) == 1);
    nourstest_true(BITFIELD_LEN(8) == 1);
    nourstest_true(BITFIELD_LEN(9) == 1);
    nourstest_true(BITFIELD_LEN(10) == 1);
    nourstest_true(BITFIELD_LEN(11) == 1);
    nourstest_true(BITFIELD_LEN(12) == 1);
    nourstest_true(BITFIELD_LEN(13) == 1);
    nourstest_true(BITFIELD_LEN(14) == 1);
    nourstest_true(BITFIELD_LEN(15) == 1);
    nourstest_true(BITFIELD_LEN(16) == 1);
    nourstest_true(BITFIELD_LEN(17) == 1);
    nourstest_true(BITFIELD_LEN(18) == 1);
    nourstest_true(BITFIELD_LEN(19) == 1);
    nourstest_true(BITFIELD_LEN(20) == 1);
    nourstest_true(BITFIELD_LEN(21) == 1);
    nourstest_true(BITFIELD_LEN(22) == 1);
    nourstest_true(BITFIELD_LEN(23) == 1);
    nourstest_true(BITFIELD_LEN(24) == 1);
    nourstest_true(BITFIELD_LEN(25) == 1);
    nourstest_true(BITFIELD_LEN(26) == 1);
    nourstest_true(BITFIELD_LEN(27) == 1);
    nourstest_true(BITFIELD_LEN(28) == 1);
    nourstest_true(BITFIELD_LEN(29) == 1);
    nourstest_true(BITFIELD_LEN(30) == 1);
    nourstest_true(BITFIELD_LEN(31) == 1);
    nourstest_true(BITFIELD_LEN(32) == 1);
    nourstest_true(BITFIELD_LEN(33) == 2);
    nourstest_true(BITFIELD_LEN(34) == 2);
    nourstest_true(BITFIELD_LEN(35) == 2);
    nourstest_true(BITFIELD_LEN(36) == 2);
    nourstest_true(BITFIELD_LEN(37) == 2);
    nourstest_true(BITFIELD_LEN(38) == 2);
    nourstest_true(BITFIELD_LEN(39) == 2);
    nourstest_true(BITFIELD_LEN(40) == 2);
    nourstest_true(BITFIELD_LEN(63) == 2);
    nourstest_true(BITFIELD_LEN(64) == 2);
    nourstest_true(BITFIELD_LEN(65) == 3);
    nourstest_true(BITFIELD_LEN(127) == 4);
    nourstest_true(BITFIELD_LEN(128) == 4);
    size_t bits_test = 128;
    size_t len = BITFIELD_LEN(bits_test);
    nourstest_true(len == 4);

    u32 *bitfield = BITFIELD_INIT_BITS(bits_test);
    u32 *allones = BITFIELD_INIT_BITS(bits_test);
    u32 *allzeros = BITFIELD_INIT_BITS(bits_test);
    allones[0] = 0xFFFFFFFF;
    allones[1] = 0xFFFFFFFF;
    allones[2] = 0xFFFFFFFF;
    allones[3] = 0xFFFFFFFF;

    allzeros[0] = 0;
    allzeros[1] = 0;
    allzeros[2] = 0;
    allzeros[3] = 0;

    bitfield[0] = 0x00000001;
    bitfield[1] = 0x00000010;
    bitfield[2] = 0x00000100;
    bitfield[3] = 0x00001000;
    nourstest_true(Bitfield_isIn(bitfield, allones, len) == true);
    nourstest_true(Bitfield_isIn(bitfield, allzeros, len) == false);
    nourstest_true(Bitfield_isIn(allones, bitfield, len) == false);

    bitfield[0] = 0;
    bitfield[1] = 0;
    bitfield[2] = 0;
    bitfield[3] = 0;
    Bitfield_On(bitfield, 0);
    nourstest_true(bitfield[0] == 1);
    Bitfield_On(bitfield, 1);
    nourstest_true(bitfield[0] == 3);
    Bitfield_Off(bitfield, 0);
    nourstest_true(bitfield[0] == 2);

    BITFIELD_FREE(bitfield);
    BITFIELD_FREE(allones);
    BITFIELD_FREE(allzeros);
}
