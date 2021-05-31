#ifndef LINALG_H
#define LINALG_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "darr.h"

// col->x, row->y, depth->z
#define arr2d_index(row, col, col_len) (row * col_len + col)
#define arr3d_index(row, col, depth, row_len, col_len) (row * col_len * row_len + col * row_len + depth)
enum Dimensions {
    ONE_D = 1,
    TWO_D = 2,
    THREE_D = 3,
};

#define REGISTER_ENUM(a) extern bool array_isIn_##a(a * array, a to_find, size_t arr_len);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern bool list_isIn_1D_##a(a * list_1D, size_t list_len, a x);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern bool list_isIn_2D_##a(a * list_1D, size_t list_len, a x, a y);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern bool list_isIn_3D_##a(a * list_1D, size_t list_len, a x, a y, a z);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern size_t * array_where_##a(a * array, a to_find, size_t arr_len);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern void matrix_print_##a(a * array, size_t row_len, size_t col_len);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern bool matrix_equal_##a(a * matrix1, a * matrix2, size_t row_len, size_t col_len);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern a * matrix_plus_##a(a * matrix1, a * matrix2, size_t row_len, size_t col_len, int8_t sign);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern a * matrix_and_##a(a * matrix1, a * matrix2, size_t row_len, size_t col_len);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern a * matrix_or_##a(a * matrix1, a * matrix2, size_t row_len, size_t col_len);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern a * matrix_mask_##a(a * matrix, a * mask, size_t row_len, size_t col_len);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern a * matrix2list_##a(a * matrix, size_t row_len, size_t col_len);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern a * list2matrix_##a(a * list, size_t row_len, size_t col_len, size_t list_len);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern bool matrix_isIn_##a(a * matrix, a to_find, size_t row_len, size_t col_len);
#include "names/template_types.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(a) extern size_t * matrix_where_##a(a * matrix, a to_find, size_t row_len, size_t col_len);
#include "names/template_types.h"
#undef REGISTER_ENUM

#endif /* LINALG_H */
