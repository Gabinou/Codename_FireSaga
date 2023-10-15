#include "nourstest.h"
#include "platform.h"
#include "pathfinding.h"

#define TEST_MAP_ROW_LEN 10
#define TEST_MAP_COL_LEN 10

void test_map_simple() {
    i32 x = 6, y = 6;
    size_t row_len = TEST_MAP_ROW_LEN, col_len = TEST_MAP_COL_LEN;
    struct Range range = {2, 4};
    i32 *list           = DARR_INIT(list, i32, 16);
    i32 *test_matrix    = calloc(row_len * col_len, sizeof(*test_matrix));

    list        = Taxicab_Circle_List(list, test_matrix, x,  y, row_len, col_len, &range);
    memset(test_matrix, 0, row_len * col_len * sizeof(*test_matrix));
    test_matrix = Taxicab_Circle(test_matrix, x, y, row_len, col_len, &range);

    i32 matrix[TEST_MAP_COL_LEN * TEST_MAP_ROW_LEN] = {
        00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
        00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
        00, 00, 00, 00, 00, 00, 01, 00, 00, 00,
        00, 00, 00, 00, 00, 01, 01, 01, 00, 00,
        00, 00, 00, 00, 01, 01, 01, 01, 01, 00,
        00, 00, 00, 01, 01, 01, 00, 01, 01, 01,
        00, 00, 01, 01, 01, 00, 00, 00, 01, 01,
        00, 00, 00, 01, 01, 01, 00, 01, 01, 01,
        00, 00, 00, 00, 01, 01, 01, 01, 01, 00,
        00, 00, 00, 00, 00, 01, 01, 01, 00, 00
    };

    for (i32 i = 0; i < (DARR_NUM(list) / 2); i++) {
        nourstest_true(matrix[list[TWO_D * i] * col_len + list[TWO_D * i + 1]] == 1);
    }
    for (i32 col = 0; col < col_len; col++) {
        for (i32 row = 0; row < row_len; row++) {
            nourstest_true(matrix[row * col_len + col] == test_matrix[row * col_len + col]);
        }
    }
    DARR_FREE(list);
    free(test_matrix);
}

#undef TEST_MAP_ROW_LEN
#undef TEST_MAP_COL_LEN

void test_map() {
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);
    test_map_simple();
}