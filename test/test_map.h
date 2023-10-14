#include "nourstest.h"
#include "platform.h"

#define TEST_MAP_ROW_LEN 10
#define TEST_MAP_COL_LEN 10

enum ENUM_TEST_PATHFINDING {
    ROW_LEN = 21,
    COL_LEN = 25,
};

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


void test_pathfinding_Astar() {
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);
    /* --- PRELIMINARIES --- */
    struct Point start_nmath  = {10, 6};
    struct Point end_nmath    = {15, 1};
    struct Point start        = {10, 6};
    struct Point end          = {15, 1};
    int *position;
    int move = 30;
    i32 costmapp[ROW_LEN * COL_LEN] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    /* -- Expected path -- */
    int expected_movemapp[ROW_LEN * COL_LEN] = {
        27, 26, 25, 24, 23, 22, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0,  0,  0,  0,  0, 0, 0, 0,
        26, 25, 24, 23, 22, 21, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0,  0,  0,  0, 0, 0, 0,
        25, 24, 23, 22, 21, 20, 19, 20, 21, 22, 23,  0,  0, 28, 29, 30, 31,  0,  0,  0,  0,  0, 0, 0, 0,
        24, 23, 22, 21, 20, 19, 18, 21,  0,  0,  0,  5,  0, 29, 30, 31,  0,  0,  0,  0,  0,  0, 0, 0, 0,
        23, 22, 21, 20, 19, 18, 17, 17,  8,  5,  3,  4,  0, 30, 31,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0,
        22, 21, 20, 19, 18, 17, 16, 16,  7,  4,  2,  3,  0, 31,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0,
        21, 20, 19, 18, 17, 16, 15, 15,  6,  3,  1,  2,  0, 30, 31,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0,
        20, 19, 18, 17, 16, 15, 14, 16,  7,  4,  2,  3,  0, 29, 30, 31,  0,  0,  0,  0,  0,  0, 0, 0, 0,
        19, 18, 17, 16, 15, 14, 13, 17,  8,  5,  3,  4,  0, 28, 29, 30, 31,  0,  0,  0,  0,  0, 0, 0, 0,
        18, 17, 16, 15, 14, 13, 12, 13,  9,  6,  4,  5,  0, 27, 28, 29, 30, 31,  0,  0,  0,  0, 0, 0, 0,
        17, 16, 15, 14, 13, 12, 11, 10, 10,  7,  5,  6,  0, 26, 27, 28, 29, 30, 31,  0,  0,  0, 0, 0, 0,
        16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  7,  0, 25, 26, 27, 28, 29, 30, 31,  0,  0, 0, 0, 0,
        17, 16, 15, 14, 13,  0,  0,  0,  0,  0,  0,  0,  0, 24, 25, 26, 27, 28, 29, 30, 31,  0, 0, 0, 0,
        18, 17, 16, 15, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 0, 0, 0,
        19, 18, 17, 16, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0, 0, 0, 0,
        20, 19, 18, 17, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0,  0, 0, 0, 0,
        21, 20, 19, 18, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0,  0,  0, 0, 0, 0,
        22, 21, 20, 19, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0,  0,  0,  0, 0, 0, 0,
        23, 22, 21, 20, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0,  0,  0,  0,  0, 0, 0, 0,
        24, 23, 22, 21, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0,  0,  0,  0,  0,  0, 0, 0, 0,
        25, 24, 23, 22, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0,
    };
    int expected_path[ROW_LEN * COL_LEN] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
    /* -- Test computing a movemap from a costmap -- */
    /* - Computing a movemap matrix - */
    i32 *movemapp = Map_Pathfinding_Moveto(costmapp, ROW_LEN, COL_LEN, start_nmath, move,
                                           NMATH_POINTS_MODE_MATRIX);
    // linalg_matrix_print_int32_t(movemapp, ROW_LEN, COL_LEN);

    for (size_t row = 0; row < ROW_LEN; row++) {
        for (size_t col = 0; col < COL_LEN; col++) {
            nourstest_true(movemapp[row * COL_LEN + col] == expected_movemapp[row * COL_LEN + col]);
        }
    }

    Map_Pathfinding_Moveto_noM(movemapp, costmapp, ROW_LEN, COL_LEN, start_nmath, move);

    for (size_t row = 0; row < ROW_LEN; row++) {
        for (size_t col = 0; col < COL_LEN; col++) {
            nourstest_true(movemapp[row * COL_LEN + col] == expected_movemapp[row * COL_LEN + col]);
        }
    }

    // linalg_matrix_print_int32_t(movemapp, ROW_LEN, COL_LEN);


    /* - Computing a movemap list - */
    int *movemapp_list = Map_Pathfinding_Moveto(costmapp, ROW_LEN, COL_LEN, start_nmath, move,
                                                NMATH_POINTS_MODE_LIST);
    for (size_t i = 0; i < DARR_NUM(movemapp_list) / 2; i++) {
        int temp_col = movemapp_list[i * NMATH_TWO_D + 0];
        int temp_row = movemapp_list[i * NMATH_TWO_D + 1];
        int current  = movemapp[temp_row * COL_LEN + temp_col];
        nourstest_true(current > 0);
    }

    /* -- Test computing a path -- */
    int *path_list = DARR_INIT(path_list, int, 32);
    path_list = Map_Pathfinding_Astar(path_list, costmapp, ROW_LEN, COL_LEN, start, end);

    int point_num       = DARR_NUM(path_list) / NMATH_TWO_D;
    int *computed_path  = linalg_list2matrix_int32_t(path_list, ROW_LEN, COL_LEN, point_num);
    // linalg_matrix_print_int32_t(computed_path, ROW_LEN, COL_LEN);

    for (size_t row = 0; row < ROW_LEN; row++) {
        for (size_t col = 0; col < COL_LEN; col++) {
            nourstest_true(computed_path[row * COL_LEN + col] == expected_path[row * COL_LEN + col]);
        }
    }

    free(movemapp);
    DARR_FREE(movemapp_list);
    DARR_FREE(path_list);
    free(computed_path);
}

#undef TEST_MAP_ROW_LEN
#undef TEST_MAP_COL_LEN

void test_map() {
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);
    test_map_simple();
    test_pathfinding_Astar();
}