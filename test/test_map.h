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

void test_map_perimeter() {
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
        00, 00, 00, 00, 00, 01, 01, 01, 00, 00,
        00, 00, 00, 00, 00, 00, 01, 00, 00, 00
    };

    struct Padding *perimeter_edges;
    perimeter_edges = Map_PerimeterM(dangermap, TEST_ROW_LEN, TEST_COL_LEN);

    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 0].right  == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 0].top    == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 0].left   == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 0].bottom == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 1].right  == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 1].top    == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 1].left   == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 1].bottom == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 2].right  == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 2].top    == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 2].left   == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 2].bottom == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 3].right  == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 3].top    == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 3].left   == 0);
    nourstest_true(perimeter_edges[0 * TEST_COL_LEN + 3].bottom == 0);

    /* Leftward point */
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 2].right  == 0);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 2].top    == 1);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 2].left   == 1);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 2].bottom == 1);
    nourstest_true(perimeter_edges[5 * TEST_COL_LEN + 3].right  == 0);
    nourstest_true(perimeter_edges[5 * TEST_COL_LEN + 3].top    == 1);
    nourstest_true(perimeter_edges[5 * TEST_COL_LEN + 3].left   == 1);
    nourstest_true(perimeter_edges[5 * TEST_COL_LEN + 3].bottom == 0);
    nourstest_true(perimeter_edges[4 * TEST_COL_LEN + 4].right  == 0);
    nourstest_true(perimeter_edges[4 * TEST_COL_LEN + 4].top    == 1);
    nourstest_true(perimeter_edges[4 * TEST_COL_LEN + 4].left   == 1);
    nourstest_true(perimeter_edges[4 * TEST_COL_LEN + 4].bottom == 0);
    nourstest_true(perimeter_edges[3 * TEST_COL_LEN + 5].right  == 0);
    nourstest_true(perimeter_edges[3 * TEST_COL_LEN + 5].top    == 1);
    nourstest_true(perimeter_edges[3 * TEST_COL_LEN + 5].left   == 1);
    nourstest_true(perimeter_edges[3 * TEST_COL_LEN + 5].bottom == 0);
    nourstest_true(perimeter_edges[2 * TEST_COL_LEN + 6].right  == 1);
    nourstest_true(perimeter_edges[2 * TEST_COL_LEN + 6].top    == 1);
    nourstest_true(perimeter_edges[2 * TEST_COL_LEN + 6].left   == 1);
    nourstest_true(perimeter_edges[2 * TEST_COL_LEN + 6].bottom == 0);

    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 3].right  == 0);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 3].top    == 0);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 3].left   == 0);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 3].bottom == 0);

    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 4].right  == 1);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 4].top    == 0);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 4].left   == 0);
    nourstest_true(perimeter_edges[6 * TEST_COL_LEN + 4].bottom == 0);


}

#undef TEST_ROW_LEN
#undef TEST_COL_LEN

void test_map() {

    test_map_simple();
    test_map_perimeter();
}