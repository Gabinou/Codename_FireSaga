#include "minctest.h"
#include "darr.h"

void test_darr() {
    SDL_Log("test_darr");
    // lok(DARR_NUM(listeded2d) == 16 * 2);
    size_t row_len = 21;
    size_t col_len = 21;
    int16_t * list = DARR_INIT(int16_t, row_len * col_len * 2);
    DARR_LEN(list) = row_len * col_len * 2;
    DARR_NUM(list) = 0;
    for (size_t col = 0; col < col_len; col++) {
        for (size_t row = 0; row < row_len; row++) {
            if (row / 2) {

                list[DARR_NUM(list)++] = row;
                list[DARR_NUM(list)++] = col;
            }
        }
    }
    int16_t * out = DARR_INIT(int16_t, DARR_NUM(list));
    DARR_LEN(out) = DARR_NUM(list);
    DARR_NUM(out) = DARR_NUM(list);
    out = memcpy(out, list, DARR_NUM(list) * sizeof(int16_t));

    DARR_FREE(list);
    DARR_FREE(out);

    size_t test_len = 2;
    int16_t * list_grow = DARR_INIT(int16_t, test_len);
    DARR_LEN(list_grow) = test_len;
    lok(DARR_LEN(list_grow) == test_len);
    DARR_NUM(list_grow) = 0;
    DARR_GROW(list_grow);
    lok(DARR_LEN(list_grow) == test_len * DARR_GROWTH_FACTOR);
    lok(DARR_NUM(list_grow) == 0);
    DARR_PUT(list_grow, 9);
    lok(DARR_NUM(list_grow) == 1);
    DARR_PUT(list_grow, 8);
    lok(DARR_NUM(list_grow) == 2);
    DARR_PUT(list_grow, 7);
    lok(DARR_NUM(list_grow) == 3);
    lok(DARR_LEN(list_grow) == test_len * DARR_GROWTH_FACTOR);
    DARR_PUT(list_grow, 6);
    lok(DARR_NUM(list_grow) == 4);
    lok(DARR_LEN(list_grow) == test_len * DARR_GROWTH_FACTOR * DARR_GROWTH_FACTOR);
    lok(list_grow[0] == 9);
    lok(list_grow[1] == 8);
    lok(list_grow[2] == 7);
    lok(list_grow[3] == 6);

    DARR_FREE(list_grow);

    int16_t * list_grow2 = DARR_INIT(int16_t, test_len);
    DARR_LEN(list_grow2) = test_len;
    DARR_NUM(list_grow2) = 0;
    lok(DARR_NUM(list_grow2) == 0);
    lok(DARR_LEN(list_grow2) == test_len);
    DARR_PUT(list_grow2, 9);
    lok(DARR_NUM(list_grow2) == 1);
    DARR_PUT(list_grow2, 8);
    lok(DARR_NUM(list_grow2) == 2);
    lok(DARR_LEN(list_grow2) == test_len * DARR_GROWTH_FACTOR);
}