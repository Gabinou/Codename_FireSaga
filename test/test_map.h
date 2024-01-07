#include "nourstest.h"
#include "platform.h"
#include "pathfinding.h"

#define TEST_ROW_LEN 10
#define TEST_COL_LEN 10

void test_map_simple() {
    i32 x = 6, y = 6;
    size_t row_len = TEST_ROW_LEN, col_len = TEST_COL_LEN;
    struct Range range = {2, 4};
    i32 *list           = DARR_INIT(list, i32, 16);
    i32 *test_matrix    = calloc(row_len * col_len, sizeof(*test_matrix));

    list        = Taxicab_Circle_List(list, test_matrix, 1, x,  y, row_len, col_len, &range);
    memset(test_matrix, 0, row_len * col_len * sizeof(*test_matrix));
    Taxicab_Circle(test_matrix, 1, x, y, row_len, col_len, &range);

    i32 matrix[TEST_COL_LEN * TEST_ROW_LEN] = {
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
    for (i32 i = 0; i < col_len * row_len; i++) {
        nourstest_true(matrix[i] == test_matrix[i]);
    }
    DARR_FREE(list);
    SDL_free(test_matrix);
}

#undef TEST_ROW_LEN
#undef TEST_COL_LEN

#define TEST_ROW_LEN 11
#define TEST_COL_LEN 10
void test_map_simple() {
    i32 dangermap[TEST_COL_LEN * TEST_ROW_LEN] = {
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
        00, 00, 00, 00, 00, 00, 01, 00, 00, 00
    };

    struct Padding *perimeter_edges;
    perimeter_edges = Map_PerimeterM(dangermap, TEST_ROW_LEN, TEST_COL_LEN);
}

#undef TEST_ROW_LEN
#undef TEST_COL_LEN

void test_map() {

    test_map_simple();
}