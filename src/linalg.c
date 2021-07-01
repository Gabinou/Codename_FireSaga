#include "linalg.h"

#define REGISTER_ENUM(type) type linalg_trace_##type(type * square_mat, size_t sq_len) {\
    type trace = 0;\
    for (size_t i = 0; i < sq_len; i++) {\
        trace += square_mat[linalg_index_arr2D(i, i, sq_len)];\
    }\
    return(trace);\
}

LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_crossProduct_##type(type * vec3D1, type * vec3D2) {\
    type * product = calloc(3, sizeof(type));\
    product[0] = vec3D1[1] * vec3D2[2] - vec3D1[2] * vec3D2[1];\
    product[1] = vec3D1[2] * vec3D2[0] - vec3D1[0] * vec3D2[2];\
    product[2] = vec3D1[0] * vec3D2[1] - vec3D1[1] * vec3D2[0];\
    return(product);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type linalg_dotProduct_##type(type * arr1, type * arr2, size_t arr_len) {\
    type out = 0;\
    for (size_t i = 0; i < arr_len; i++) {\
        out += arr1[i] * arr2[i];\
    }\
    return (out);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool linalg_list_isIn_1D_##type(type * list_2D, size_t list_len, type x) {\
    bool found = false;\
    for (size_t i = 0; i < list_len; i++) {\
        if (x == list_2D[i * ONE_D + 0]) {\
            found = true;\
            break;\
        }\
    }\
    return (found);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool linalg_list_isIn_2D_##type(type * list_2D, size_t list_len, type x, type y) {\
    bool found = false;\
    for (size_t i = 0; i < list_len; i++) {\
        if ((x == list_2D[i * TWO_D + 0]) && (y == list_2D[i * TWO_D + 1])) {\
            found = true;\
            break;\
        }\
    }\
    return (found);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool linalg_list_isIn_3D_##type(type * list_3D, size_t list_len, type x, type y, type z) {\
    bool found = false;\
    for (size_t i = 0; i < list_len; i++) {\
        if ((x == list_3D[i * THREE_D + 0]) && (y == list_3D[i * THREE_D + 1]) && (z == list_3D[i * THREE_D + 2])) {\
            found = true;\
            break;\
        }\
    }\
    return (found);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool linalg_isIn_##type(type * array, type to_find, size_t arr_len) {\
    bool found = false;\
    for (size_t i = 0; i < arr_len; i++) {\
        if (array[i] == to_find) {\
            found = true;\
            break;\
        }\
    }\
    return (found);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) size_t * linalg_where_##type(type * array, type to_find, size_t arr_len) {\
    size_t * found_list = DARR_INIT(found_list, size_t, arr_len);\
    for (size_t i = 0; i < arr_len; i++) {\
        if (array[i] == to_find) {\
            found_list[DARR_NUM(found_list)++]= i;\
            break;\
        }\
    }\
    DARR_LEN(found_list) = DARR_NUM(found_list);\
    found_list = DARR_REALLOC(found_list, (DARR_NUM(found_list) < LINALG_MINLEN? LINALG_MINLEN :DARR_NUM(found_list)));\
    return (found_list);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) void matrix_print_##type(type * array, size_t row_len, size_t col_len) {\
    for (size_t row = 0; row < row_len; row++) {\
        for (size_t col = 0; col < col_len; col++) {\
            printf("%02d ", array[row * col_len + col]);\
        }\
        printf("\n");\
    }\
}
LINALG_TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) void matrix_print_##type(type * array, size_t row_len, size_t col_len) {\
    for (size_t row = 0; row < row_len; row++) {\
        for (size_t col = 0; col < col_len; col++) {\
            printf("%2.4f ", array[row * col_len + col]);\
        }\
        printf("\n");\
    }\
}
LINALG_TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_equal_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] == matrix2[i]);\
    }\
return (out);\
}
LINALG_TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool * linalg_equal_##type(type * matrix1, type * matrix2, size_t arr_len, type tolerance) {\
    bool * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
        out[i] = (fabs(matrix1[i] - matrix2[i]) < tolerance);\
    }\
return (out);\
}
LINALG_TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool linalg_any_##type(type * arr, size_t arr_len) {\
    bool equal = false;\
    for (size_t i = 0; i < arr_len; i++) {\
        if (arr[i]) {\
            equal = true;\
            break;\
        }\
    }\
return (equal);\
}
LINALG_TEMPLATE_TYPES_INT
#undef REGISTER_ENUM


#define REGISTER_ENUM(type) bool linalg_all_##type(type * arr, size_t arr_len) {\
    bool equal = true;\
    for (size_t i = 0; i < arr_len; i++) {\
        if (!arr[i]) {\
            equal = false;\
            break;\
        }\
    }\
return (equal);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM


#define REGISTER_ENUM(type) type * linalg_and_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
        out[i] = (matrix1[i] && matrix2[i]);\
    }\
    return (out);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_or_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
        out[i] = (matrix1[i] || matrix2[i]);\
    }\
    return (out);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_plus_##type(type * matrix1, type * matrix2, size_t arr_len, int8_t sign) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = matrix1[i] + sign * matrix2[i];\
    }\
    return (out);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_mask_##type(type * matrix, type * mask, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
        out[i] = matrix[i] * (mask[i] > 0);\
    }\
    return (out);\
}
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_list2matrix_##type(type * list, size_t row_len, size_t col_len, size_t list_len) {\
    type * out = calloc(row_len * col_len, sizeof(type));\
    for (size_t elem = 0; elem < list_len; elem++) {\
        out[list[2 * elem + 1] * col_len + list[2 * elem + 0]] = 1;\
    }\
    return (out);\
}
LINALG_TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_matrix2list_##type(type * matrix, size_t row_len, size_t col_len) {\
    type * list = DARR_INIT(list, type, row_len*col_len*2);\
    for (size_t col = 0; col < col_len; col++) {\
        for (size_t row = 0; row < row_len; row++) {\
            if (matrix[row * col_len + col] > 0) {\
                list[DARR_NUM(list)++] = col;\
                list[DARR_NUM(list)++] = row;\
            }\
        }\
    }\
    list = DARR_REALLOC(list, (DARR_NUM(list) < LINALG_MINLEN? LINALG_MINLEN :DARR_NUM(list)));\
    return (list);\
}
LINALG_TEMPLATE_TYPES_INT
#undef REGISTER_ENUM



