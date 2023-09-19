#include "nourstest.h"
#include "crt.h"

enum MAT_LENS {
    MAT_ROW_LEN = 21,
    SMALL_ROW_LEN = 11,
    MAT_COL_LEN = 25,
    SMALL_COL_LEN = 12,
};

void test_crt() {
    SDL_Log("test_crt");
    const int cell_width = 2;
    const int cell_height = 5;
    const int cell_blank_R = 1;
    const int cell_blank_G = 1;
    const int cell_blank_B = 2;
    const int stagger = 3;
    const int cell_Vblank = 1;
    const int cell_Hblank = cell_blank_R + cell_blank_G + cell_blank_B + cell_width * 2;
    const int x_offset_R = 0;
    const int y_offset_R = 0;
    const int x_offset_G = cell_width + cell_blank_R;
    const int y_offset_G = 0;
    const int x_offset_B = cell_width * 2 + cell_blank_R + cell_blank_G;
    const int y_offset_B = 0;

    /* --- LANCZOS --- */

    /* MY LANCZOS FILTER FUCKS UPS COLORS*/
    nourstest_true(Lanczos_Kernel(0.0f, LANCZOS_A) == 1.0f);
    // nourstest_true(Lanczos_Kernel(0.5f, LANCZOS_A) == 0.5731591682507563f);
    printf("%f \n", Lanczos_Kernel(0.5f, LANCZOS_A));
    float small[SMALL_ROW_LEN * SMALL_COL_LEN] = {
        0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f,
        0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f,
        0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f,
        0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f,
        0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f,
        0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f,
        0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f,
        0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f,
        0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f,
        0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f,
        0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f
    };

    float filtered[MAT_ROW_LEN * MAT_COL_LEN] = {0};
    /* -- Upscaling -- */
    /* !!! upscaling basically works !!! */

    Lanczos_Filter_2D(small, filtered, SMALL_COL_LEN, SMALL_ROW_LEN, MAT_COL_LEN, MAT_ROW_LEN,
                      LANCZOS_A);
    // linalg_matrix_print_float(small, SMALL_ROW_LEN, SMALL_COL_LEN);
    // printf("\n");
    // linalg_matrix_print_float(filtered, MAT_ROW_LEN, MAT_COL_LEN);

    /* -- Downscaling -- */
    /* !!! downscaling basically works !!! */
    float downscaled[SMALL_ROW_LEN * SMALL_COL_LEN] = {0};
    Lanczos_Filter_2D(filtered, downscaled, MAT_COL_LEN, MAT_ROW_LEN, SMALL_COL_LEN, SMALL_ROW_LEN,
                      LANCZOS_A);

    // linalg_matrix_print_float(small, SMALL_ROW_LEN, SMALL_COL_LEN);
    // printf("\n");
    // linalg_matrix_print_float(downscaled, SMALL_ROW_LEN, SMALL_COL_LEN);

    /* --- NEAREST --- */

    /* -- Upscaling -- */
    Nearest_Scanline_Filter_2D(small, filtered, SMALL_COL_LEN, SMALL_ROW_LEN, MAT_COL_LEN, MAT_ROW_LEN);
    // linalg_matrix_print_float(small, SMALL_ROW_LEN, SMALL_COL_LEN);
    // printf("\n");
    // linalg_matrix_print_float(filtered, MAT_ROW_LEN, MAT_COL_LEN);

    /* -- Downscaling -- */
    Nearest_Scanline_Filter_2D(filtered, downscaled, MAT_COL_LEN, MAT_ROW_LEN, SMALL_COL_LEN,
                               SMALL_ROW_LEN);
    // linalg_matrix_print_float(filtered, MAT_ROW_LEN, MAT_COL_LEN);
    // printf("\n");
    // linalg_matrix_print_float(downscaled, SMALL_ROW_LEN, SMALL_COL_LEN);

    /* --- GAUSSIAN BLUR FILTER --- */
    float small2[SMALL_ROW_LEN * SMALL_COL_LEN] = {
        200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f,
        250.0f, 250.0f, 250.0f, 250.0f, 250.0f, 250.0f, 250.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f,
        200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f,
        200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f,
        200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f,
        200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f,
        200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f,
        200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f,
        200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f,
        200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f,
        200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f, 200.0f
    };
    float temp[SMALL_ROW_LEN * SMALL_COL_LEN] = {0};
    float small_blurred[SMALL_ROW_LEN * SMALL_COL_LEN] = {0};

    linalg_matrix_print_float(small2, SMALL_ROW_LEN, SMALL_COL_LEN);
    printf("\n");
    linalg_matrix_print_float(small_blurred, SMALL_ROW_LEN, SMALL_COL_LEN);
    printf("\n");
    linalg_matrix_print_float(temp, SMALL_ROW_LEN, SMALL_COL_LEN);

    blur(small2, small_blurred, temp, SMALL_COL_LEN, SMALL_ROW_LEN, 8.0f, 3);

    gaussBlur_4(small2, small_blurred, SMALL_ROW_LEN, SMALL_COL_LEN, 2.0f, GAMMA_N);
    linalg_matrix_print_float(small2, SMALL_ROW_LEN, SMALL_COL_LEN);
    printf("\n");
    linalg_matrix_print_float(small_blurred, SMALL_ROW_LEN, SMALL_COL_LEN);
    printf("\n");
    linalg_matrix_print_float(temp, SMALL_ROW_LEN, SMALL_COL_LEN);

    /* --- SHAsublDOW MASK --- */
    uint8_t mask_R[MAT_ROW_LEN * MAT_COL_LEN] = {
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0
    };

    uint8_t mask_G[MAT_ROW_LEN * MAT_COL_LEN] = {
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0
    };

    uint8_t mask_B[MAT_ROW_LEN * MAT_COL_LEN] = {
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    uint8_t mask_R_test[MAT_ROW_LEN * MAT_COL_LEN];
    uint8_t mask_G_test[MAT_ROW_LEN * MAT_COL_LEN];
    uint8_t mask_B_test[MAT_ROW_LEN * MAT_COL_LEN];

    CRT_Shadow_Mask(mask_R_test, MAT_COL_LEN, MAT_ROW_LEN, cell_height, cell_width, cell_Hblank,
                    cell_Vblank, stagger,  x_offset_R,  y_offset_R);
    CRT_Shadow_Mask(mask_G_test, MAT_COL_LEN, MAT_ROW_LEN, cell_height, cell_width, cell_Hblank,
                    cell_Vblank, stagger,  x_offset_G,  y_offset_G);
    CRT_Shadow_Mask(mask_B_test, MAT_COL_LEN, MAT_ROW_LEN, cell_height, cell_width, cell_Hblank,
                    cell_Vblank, stagger,  x_offset_G,  y_offset_B);

    // linalg_matrix_print_uint8_t(mask_R, MAT_ROW_LEN, MAT_COL_LEN);
    // printf("\n");
    // linalg_matrix_print_uint8_t(mask_R_test, MAT_ROW_LEN, MAT_COL_LEN);

    /* !!! Shadow Filtering works  !!! */
    nourstest_true(linalg_equal_uint8_t(mask_R, mask_R_test, MAT_ROW_LEN * MAT_COL_LEN));
    nourstest_true(linalg_equal_uint8_t(mask_G, mask_G_test, MAT_ROW_LEN * MAT_COL_LEN));
    nourstest_true(linalg_equal_uint8_t(mask_B, mask_B_test, MAT_ROW_LEN * MAT_COL_LEN));

    // linalg_matrix_print_float(filtered, MAT_ROW_LEN, MAT_COL_LEN);
    // CRT_Shadow_Filter(mask_R_test, filtered, MAT_ROW_LEN, MAT_COL_LEN);
    // printf("\n");
    // linalg_matrix_print_float(filtered, MAT_ROW_LEN, MAT_COL_LEN);

    /* --- BLOOM --- */
    // // CRT_Gamma_Uncorrection WORKS
    // linalg_matrix_print_float(small, MAT_ROW_LEN, MAT_COL_LEN);
    // printf("\n");
    // CRT_Gamma_Uncorrection(small, SMALL_ROW_LEN * SMALL_COL_LEN);
    // linalg_matrix_print_float(small, MAT_ROW_LEN, MAT_COL_LEN);

    // // CRT_Gamma_Correction WORKS
    // linalg_matrix_print_float(small, MAT_ROW_LEN, MAT_COL_LEN);
    // printf("\n");
    // CRT_Gamma_Correction(small, SMALL_ROW_LEN * SMALL_COL_LEN);
    // linalg_matrix_print_float(small, MAT_ROW_LEN, MAT_COL_LEN);

    // // CRT_Bloom_Correction WORKS
    // linalg_matrix_print_float(small, MAT_ROW_LEN, MAT_COL_LEN);
    // printf("\n");
    // CRT_Bloom_Correction(small, SMALL_ROW_LEN * SMALL_COL_LEN);
    // linalg_matrix_print_float(small, MAT_ROW_LEN, MAT_COL_LEN);

    /* --- PIXEL2FLOATS --- */
    // WORK,CONFIRMED BY MY EYES

}