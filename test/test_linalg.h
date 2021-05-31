#include "minctest.h"
#include "linalg.h"

enum {
    LINALG_ROW_LEN = 21,
    LINALG_COL_LEN = 25,
};

void test_linalg() {
    SDL_Log("test_linalg");
    SDL_Log("Initting matrices");
    int16_t * temp2D1 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int16_t));
    int16_t * temp2D2 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int16_t));
    int16_t * temp2D3 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int16_t));
    int16_t * temp2D4 = calloc(LINALG_ROW_LEN * LINALG_COL_LEN, sizeof(int16_t));
    for (uint8_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (uint8_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D1[(row * LINALG_COL_LEN + col)] = 1;
            temp2D2[(row * LINALG_COL_LEN + col)] = 2;
            temp2D3[(row * LINALG_COL_LEN + col)] = 3;
        }
    }
    int16_t * out = NULL;
    // matrix_print_int16_t(temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN);
    // matrix_print_int16_t(temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN);
    // matrix_print_int16_t(temp2D3, LINALG_ROW_LEN, LINALG_COL_LEN);

    SDL_Log("First tests");
    out = matrix_plus_int16_t(temp2D3, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN, -1);

    lok(matrix_equal_int16_t(out, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));

    SDL_Log("Setting up second matrix");
    int16_t temp2D11[LINALG_COL_LEN * LINALG_ROW_LEN] = {
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };


    for (uint8_t row = 0; row < LINALG_ROW_LEN; row++) {
        for (uint8_t col = 0; col < LINALG_COL_LEN; col++) {
            temp2D4[(row * LINALG_COL_LEN + col)] = temp2D11[(row * LINALG_COL_LEN + col)];
        }
    }

    SDL_Log("More tests");
    SDL_Log("Equal1");
    lok(matrix_equal_int16_t(matrix_and_int16_t(temp2D1, temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN), temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    SDL_Log("Equal2");
    lok(matrix_equal_int16_t(matrix_and_int16_t(temp2D1, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN), temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    SDL_Log("Equal3");
    lok(matrix_equal_int16_t(matrix_and_int16_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN), temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    SDL_Log("Equal4");
    lok(matrix_equal_int16_t(matrix_or_int16_t(temp2D2, temp2D2, LINALG_ROW_LEN, LINALG_COL_LEN), temp2D1, LINALG_ROW_LEN, LINALG_COL_LEN));
    SDL_Log("Equal5");
    lok(matrix_equal_int16_t(matrix_or_int16_t(temp2D11, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN), temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    int16_t templist1[2 * 16] = {
        0, 0,
        0, 1,
        0, 2,
        0, 3,
        0, 4,
        0, 5,
        1, 0,
        1, 1,
        1, 2,
        1, 3,
        2, 0,
        2, 1,
        2, 2,
        3, 0,
        3, 1,
        4, 0
    };
    SDL_Log("list2mat");
    int16_t * matrixed2d = list2matrix_int16_t(templist1, LINALG_ROW_LEN, LINALG_COL_LEN, 16);
    // printf("\n");
    // printf("\n");
    // matrix_print_int16_t(matrixed2d, LINALG_ROW_LEN, LINALG_COL_LEN);
    // matrix_print_int16_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    SDL_Log("equal6");
    lok(matrix_equal_int16_t(matrixed2d, temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN));
    SDL_Log("mat2list");
    int16_t * listeded2d = matrix2list_int16_t(temp2D11, LINALG_ROW_LEN, LINALG_COL_LEN);
    SDL_Log("equal7");
    lok(matrix_equal_int16_t(listeded2d, listeded2d, 2, 16));
    lok(DARR_NUM(listeded2d) == 16 * 2);
}