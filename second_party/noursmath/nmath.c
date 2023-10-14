#include "nmath.h"

/*********************************** DTAB ************************************/


/************************* MULTIPLY N TIMES *****************************/

float nmath_slowpow(float base, uint32_t exponent) {
    /* Super fast for -O1/-O2 optimization */
    if (exponent == 0)
        return(1.0f);
    float out = base;
    for (int i = 0; i < (exponent - 1); i++)
        out *= base;
    return (out);
}


/****************************** STRING HASHING *******************************/
uint64_t dtab_hash_djb2(const char * str) {
    /* djb2 hashing algorithm by Dan Bernstein.
    * Description: This algorithm (k=33) was first reported by dan bernstein many
    * years ago in comp.lang.c. Another version of this algorithm (now favored by bernstein)
    * uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number 33
    * (why it works better than many other constants, prime or not) has never been adequately explained.
    * [1] https://stackoverflow.com/questions/7666509/hash-function-for-string
    * [2] http://www.cse.yorku.ca/~oz/hash.html */
    uint64_t hash = 5381;
    int32_t str_char;
    while ((str_char = *str++)) {
        hash = ((hash << 5) + hash) + str_char; /* hash * 33 + c */
    }
    return (hash);
}

uint64_t dtab_hash_sdbm(const char * str) {
    /* sdbm hashing algorithm by Dan Bernstein.
    * Description: This algorithm was created for sdbm (a public-domain
    * reimplementation of ndbm) database library. It was found to do
    * well in scrambling bits, causing better distribution of the
    * keys and fewer splits. It also happens to be a good general hashing
    * function with good distribution. The actual function is
    *hash(i) = hash(i - 1) * 65599 + str[i]; what is included below
    * is the faster version used in gawk. [* there is even a faster,
    * duff-device version] the magic constant 65599 was picked out of
    * thin air while experimenting with different constants, and turns
    * out to be a prime. this is one of the algorithms used in
    * berkeley db (see sleepycat) and elsewhere.
    * [1] https://stackoverflow.com/questions/7666509/hash-function-for-string
    * [2] http://www.cse.yorku.ca/~oz/hash.html */
    uint64_t hash = 0;
    uint32_t str_char;
    while ((str_char = *str++)) {
        hash = str_char + (hash << 6) + (hash << 16) - hash;
    }
    return (hash);
}

size_t dtab_found(struct dtab * dtab_ptr, uint64_t in_hash) {
    size_t pos = DTAB_NULL;
    for (size_t i = 0; i < dtab_ptr->num; i++) {
        if (dtab_ptr->keys[i] == in_hash) {
            pos = i;
            break;
        }
    }
    return (pos);
}

void * dtab_get(struct dtab * dtab_ptr, uint64_t in_hash) {
    void * out = NULL;
    size_t pos = dtab_found(dtab_ptr, in_hash);
    if (pos) {
        dtab_byte_t * values_bytesptr = (dtab_byte_t *)(dtab_ptr->values);
        out = (void *)(values_bytesptr + (dtab_ptr->bytesize * pos));
    }
    return (out);
}

void dtab_add(struct dtab * dtab_ptr, void * value, uint64_t in_hash) {
    dtab_byte_t * values_bytesptr, * newvalue_bytesptr;
    size_t pos = dtab_found(dtab_ptr, in_hash);
    if (!pos) {
        dtab_ptr->keys[dtab_ptr->num] = in_hash;
        values_bytesptr = (dtab_byte_t *)(dtab_ptr->values);
        newvalue_bytesptr = values_bytesptr + (dtab_ptr->bytesize * dtab_ptr->num);
        dtab_ptr->num++;
    } else {
        values_bytesptr = (dtab_byte_t *)(dtab_ptr->values);
        newvalue_bytesptr = values_bytesptr + (dtab_ptr->bytesize * pos);
    }
    memcpy(newvalue_bytesptr, value, dtab_ptr->bytesize);
    if (dtab_ptr->num == dtab_ptr->len) {
        DTAB_GROW(dtab_ptr);
    }

}

void dtab_del(struct dtab * dtab_ptr, uint64_t in_hash) {
    size_t pos = dtab_found(dtab_ptr, in_hash);
    if ((pos) && (pos < dtab_ptr->num)) {
        memmove(dtab_ptr->keys + pos, dtab_ptr->keys + pos + 1, (dtab_ptr->num - pos - 1)*sizeof(uint64_t)) ;
        dtab_byte_t * values_bytesptr = (dtab_byte_t *)(dtab_ptr->values);
        memmove(values_bytesptr + pos * dtab_ptr->bytesize, values_bytesptr + (pos + 1)*dtab_ptr->bytesize, (dtab_ptr->num - pos - 1)*dtab_ptr->bytesize);
        dtab_ptr->num--;
    }

}

void dtab_del_scramble(struct dtab * dtab_ptr, uint64_t in_hash) {
    size_t pos = dtab_found(dtab_ptr, in_hash);
    if ((pos) && (pos < dtab_ptr->num))  {
        memmove(dtab_ptr->keys + pos, dtab_ptr->keys + dtab_ptr->num - 1, sizeof(uint64_t));
        dtab_byte_t * values_bytesptr = (dtab_byte_t *)(dtab_ptr->values);
        memmove(values_bytesptr + pos * dtab_ptr->bytesize, values_bytesptr + dtab_ptr->num * dtab_ptr->bytesize, dtab_ptr->bytesize);
        dtab_ptr->num--;
    }
}

/********************************* STRUCTS ***********************************/

#define REGISTER_ENUM(type) struct nmath_sq_neighbors_##type nmath_sq_neighbors_##type##_default = {\
.right = 0,\
.top = 0,\
.left = 0,\
.bottom = 0\
};
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) struct nmath_hex_neighbors_##type nmath_hex_neighbors_##type##_default = {\
.right = 0,\
.top = 0,\
.left = 0,\
.bottom = 0,\
.front = 0,\
.behind = 0\
};
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) struct nmath_point_##type nmath_point_##type##_default = {\
.x = 0,\
.y = 0\
};
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) struct nmath_point3D_##type nmath_point_##type3D_##type##_default = {\
.x = 0,\
.y = 0,\
.z = 0\
};
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) struct nmath_hexpoint_##type nmath_hexpoint_##type##_default = {\
.x = 0,\
.y = 0,\
.z = 0\
};
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) struct nmath_node_##type  nmath_node_##type##_default = {\
    .x = 0,\
    .y = 0,\
    .distance = 0,\
    .cost = 0\
};
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) struct nmath_node3D_##type nmath_node3D_##type##_default = {\
    .x = 0,\
    .y = 0,\
    .z = 0,\
    .distance = 0\
};
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) struct nmath_hexnode_##type nmath_hexnode_##type##_default = {\
    .x = 0,\
    .y = 0,\
    .z = 0,\
    .distance = 0\
};
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

/******************************** UTILITIES **********************************/

#define REGISTER_ENUM(type) type nmath_inbounds_##type(type pos, type boundmin, type boundmax) {\
    type out = 0;\
    out = pos < boundmin ? boundmin : pos;\
    out = out > boundmax ? boundmax : out;\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

/********************************* Q_MATH ************************************/

// sequence_geometric: Geometrically decreasing integer/float
// distance/geo_factor -> distance/geo_factor**2 -> distance/geo_factor**3
// Ex: q_sequence_geometric(32,1,2) = 16->8->4 ...
// sign*distance more elegant than std_abs()
#define REGISTER_ENUM(type) type q_sequence_geometric_##type(type current, type destination, type geo_factor) {\
    type distance = current - destination;\
    type sign = (type)copysign(1, distance);\
    type out = (sign * (distance / geo_factor) < 1) ? sign : (distance / geo_factor);\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

int32_t q_sequence_fgeometric_int32_t(int32_t current, int32_t destination, float geo_factor) {
    int32_t distance = current - destination;\
    int32_t sign = (int32_t )copysign(1, distance);\
    int32_t out = (sign * (int32_t)(distance / geo_factor) < 1) ? sign : (int32_t)(distance / geo_factor);\
    return (out);\

}


// sequence_pingpong: oscillating integer sequence. upper is excluded.
// modulo: x % 2 = 0,1,2,0,1,2,0...for x++
// pingpong(x, 2, 0) = 0,1,2,1,0,1,2... for x++
#define REGISTER_ENUM(type) type q_sequence_pingpong_##type(type current, type upper, type lower) {\
    type mod_factor = (2 * (upper - lower) - 2);\
    type term1 = mod_factor - (current % mod_factor);\
    type term2 = current % mod_factor;\
    return (fmin(term1, term2) + lower);\
}
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

// Sometimes attributed to John Carmack...
#define REGISTER_ENUM(type) float q_sqrt_##type(type in_int) {\
    const float x2 = in_int * 0.5f;\
    const float threehalfs = 1.5f;\
    union {\
        float f;\
        unsigned long i;\
    } conv  = { .f = in_int };\
    conv.i  = 0x5f3759df - (conv.i >> 1);\
    conv.f  *= (threehalfs - (x2 * conv.f * conv.f));\
    return conv.f;\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

/********************************* LINALG ************************************/

#define REGISTER_ENUM(type) type linalg_trace_##type(type * square_mat, size_t sq_len) {\
    type trace = 0;\
    for (size_t i = 0; i < sq_len; i++) {\
        trace += square_mat[linalg_index_arr2D(i, i, sq_len)];\
    }\
    return(trace);\
}

TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_crossProduct_##type(type * vec3D1, type * vec3D2) {\
    type * product = calloc(3, sizeof(type));\
    product[0] = vec3D1[1] * vec3D2[2] - vec3D1[2] * vec3D2[1];\
    product[1] = vec3D1[2] * vec3D2[0] - vec3D1[0] * vec3D2[2];\
    product[2] = vec3D1[0] * vec3D2[1] - vec3D1[1] * vec3D2[0];\
    return(product);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_crossProduct_noM_##type(type * product, type * vec3D1, type * vec3D2) {\
    product[0] = vec3D1[1] * vec3D2[2] - vec3D1[2] * vec3D2[1];\
    product[1] = vec3D1[2] * vec3D2[0] - vec3D1[0] * vec3D2[2];\
    product[2] = vec3D1[0] * vec3D2[1] - vec3D1[1] * vec3D2[0];\
    return(product);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type linalg_dotProduct_##type(type * arr1, type * arr2, size_t arr_len) {\
    type out = 0;\
    for (size_t i = 0; i < arr_len; i++) {\
        out += arr1[i] * arr2[i];\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool linalg_list_isIn_1D_##type(type * list_2D, size_t list_len, type x) {\
    bool found = false;\
    for (size_t i = 0; i < list_len; i++) {\
        if (x == list_2D[i * NMATH_ONE_D + 0]) {\
            found = true;\
            break;\
        }\
    }\
    return (found);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool linalg_list_isIn_2D_##type(type * list_2D, size_t list_len, type x, type y) {\
    bool found = false;\
    for (size_t i = 0; i < list_len; i++) {\
        if ((x == list_2D[i * NMATH_TWO_D + 0]) && (y == list_2D[i * NMATH_TWO_D + 1])) {\
            found = true;\
            break;\
        }\
    }\
    return (found);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool linalg_list_isIn_3D_##type(type * list_3D, size_t list_len, type x, type y, type z) {\
    bool found = false;\
    for (size_t i = 0; i < list_len; i++) {\
        if ((x == list_3D[i * NMATH_THREE_D + 0]) && (y == list_3D[i * NMATH_THREE_D + 1]) && (z == list_3D[i * NMATH_THREE_D + 2])) {\
            found = true;\
            break;\
        }\
    }\
    return (found);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_uniques_##type(type * array, size_t arr_len) {\
    type * uniques_list = DARR_INIT(uniques_list, type, arr_len);\
    for (size_t i = 0; i < arr_len; i++) {\
        if (!linalg_isIn_##type(uniques_list, array[i], DARR_NUM(uniques_list))) {\
            DARR_PUT(uniques_list, array[i]);\
        }\
    }\
    DARR_LEN(uniques_list) = DARR_NUM(uniques_list);\
    uniques_list = DARR_REALLOC(uniques_list, (DARR_NUM(uniques_list) < NMATH_MINLEN? NMATH_MINLEN : DARR_NUM(uniques_list)));\
    return (uniques_list);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
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
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) size_t * linalg_where_##type(type * array, type to_find, size_t arr_len) {\
    size_t * found_list = DARR_INIT(found_list, size_t, arr_len);\
    for (size_t i = 0; i < arr_len; i++) {\
        if (array[i] == to_find) {\
            DARR_PUT(found_list, i);\
            break;\
        }\
    }\
    DARR_LEN(found_list) = DARR_NUM(found_list);\
    found_list = DARR_REALLOC(found_list, (DARR_NUM(found_list) < NMATH_MINLEN? NMATH_MINLEN :DARR_NUM(found_list)));\
    return (found_list);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_draw_circ_noM_##type(type * out_mat, type origin_x, type origin_y, size_t diameter, size_t row_len, size_t col_len){\
    size_t radius = diameter / 2;\
    size_t row_min = (origin_y - radius - 1) < 0 ? 0 : (origin_y - radius - 1);\
    size_t row_max = (origin_y + radius + 1) > row_len ? row_len : (origin_y + radius + 1);\
    size_t col_min = (origin_x - radius - 1) < 0 ? 0 : (origin_x - radius - 1);\
    size_t col_max = (origin_x + radius + 1) > col_len ? col_len : (origin_x + radius + 1);\
    for (int64_t row = row_min; row < row_max; row++) {\
        for (int64_t col = col_min; col < col_max; col++) {\
            if (((row-origin_y) * (row-origin_y) + (col-origin_x) * (col-origin_x)) < (diameter * diameter / 4)) {\
                out_mat[row * col_len + col] = 1;\
            }\
        }\
    }\
    return (out_mat);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_draw_circ_##type(type origin_x, type origin_y, size_t diameter, size_t row_len, size_t col_len){\
    type * out_mat = calloc(row_len*col_len, sizeof(type));\
    size_t radius = diameter / 2;\
    size_t row_min = (origin_y - radius - 1) < 0 ? 0 : (origin_y - radius - 1);\
    size_t row_max = (origin_y + radius + 1) > row_len ? row_len : (origin_y + radius + 1);\
    size_t col_min = (origin_x - radius - 1) < 0 ? 0 : (origin_x - radius - 1);\
    size_t col_max = (origin_x + radius + 1) > col_len ? col_len : (origin_x + radius + 1);\
    for (int64_t row = row_min; row < row_max; row++) {\
        for (int64_t col = col_min; col < col_max; col++) {\
            if (((row-origin_y) * (row-origin_y) + (col-origin_x) * (col-origin_x)) < (diameter * diameter / 4)) {\
                out_mat[row * col_len + col] = 1;\
            }\
        }\
    }\
    return (out_mat);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_draw_rect_noM_##type(type * out_mat, type origin_x, type origin_y, size_t width,size_t height, size_t row_len, size_t col_len){\
    size_t row_min = origin_y < 0 ? 0 : origin_y;\
    size_t row_max = (origin_y + height) > row_len ? row_len : (origin_y + height);\
    size_t col_min = origin_x < 0 ? 0 : origin_x;\
    size_t col_max = (origin_x + width) > col_len ? col_len : (origin_x + width);\
    for (size_t row = row_min; row < row_max; row++) {\
        for (size_t col = col_min; col < col_max; col++) {\
            out_mat[row * col_len + col] = 1;\
        }\
    }\
    return (out_mat);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_draw_rect_##type(type origin_x, type origin_y, size_t width, size_t height, size_t row_len, size_t col_len){\
    type * out_mat = calloc(row_len*col_len, sizeof(type));\
    size_t row_min = origin_y < 0 ? 0 : origin_y;\
    size_t row_max = (origin_y + height) > row_len ? row_len : (origin_y + height);\
    size_t col_min = origin_x < 0 ? 0 : origin_x;\
    size_t col_max = (origin_x + width) > col_len ? col_len : (origin_x + width);\
    for (size_t row = row_min; row < row_max; row++) {\
        for (size_t col = col_min; col < col_max; col++) {\
            out_mat[row * col_len + col] = 1;\
        }\
    }\
    return (out_mat);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM


void linalg_matrix_print_bool(bool * array, size_t row_len, size_t col_len) {
    for (size_t row = 0; row < row_len; row++) {
        for (size_t col = 0; col < col_len; col++) {
            printf("%d", array[row * col_len + col]);
        }
        printf("\n");
    }
}

void linalg_matrix_print_uint8_t(uint8_t * array, size_t row_len, size_t col_len) {
    for (size_t row = 0; row < row_len; row++) {
        for (size_t col = 0; col < col_len; col++) {
            printf("%02u", array[row * col_len + col]);
        }
        printf("\n");
    }
}

void linalg_matrix_print_uint16_t(uint16_t * array, size_t row_len, size_t col_len) {
    for (size_t row = 0; row < row_len; row++) {
        for (size_t col = 0; col < col_len; col++) {
            printf("%02u", array[row * col_len + col]);
        }
        printf("\n");
    }
}

void linalg_matrix_print_uint32_t(uint32_t * array, size_t row_len, size_t col_len) {
    for (size_t row = 0; row < row_len; row++) {
        for (size_t col = 0; col < col_len; col++) {
            printf("%02u", array[row * col_len + col]);
        }
        printf("\n");
    }
}

void linalg_matrix_print_uint64_t(uint64_t * array, size_t row_len, size_t col_len) {
    for (size_t row = 0; row < row_len; row++) {
        for (size_t col = 0; col < col_len; col++) {
            printf("%02lu", array[row * col_len + col]);
        }
        printf("\n");
    }
}


void linalg_matrix_print_int8_t(int8_t * array, size_t row_len, size_t col_len) {
    for (size_t row = 0; row < row_len; row++) {
        for (size_t col = 0; col < col_len; col++) {
            printf("%02d", array[row * col_len + col]);
        }
        printf("\n");
    }
}

void linalg_matrix_print_int16_t(int16_t * array, size_t row_len, size_t col_len) {
    for (size_t row = 0; row < row_len; row++) {
        for (size_t col = 0; col < col_len; col++) {
            printf("%02d", array[row * col_len + col]);
        }
        printf("\n");
    }
}

void linalg_matrix_print_int32_t(int32_t * array, size_t row_len, size_t col_len) {
    for (size_t row = 0; row < row_len; row++) {
        for (size_t col = 0; col < col_len; col++) {
            printf("%02d", array[row * col_len + col]);
        }
        printf("\n");
    }
}

void linalg_matrix_print_int64_t(int64_t * array, size_t row_len, size_t col_len) {
    for (size_t row = 0; row < row_len; row++) {
        for (size_t col = 0; col < col_len; col++) {
            printf("%02ld", array[row * col_len + col]);
        }
        printf("\n");
    }
}



#define REGISTER_ENUM(type) void linalg_matrix_print_##type(type * array, size_t row_len, size_t col_len) {\
    for (size_t row = 0; row < row_len; row++) {\
        for (size_t col = 0; col < col_len; col++) {\
            printf("%2.6f ", array[row * col_len + col]);\
        }\
        printf("\n");\
    }\
}
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_smaller_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] < matrix2[i]);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_smaller_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] < matrix2[i]);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_sseq_noM_##type(type * out, type * matrix1, type tocompare, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] <= tocompare);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_sseq_##type(type * matrix1, type tocompare, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] <= tocompare);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_sgeq_noM_##type(type * out, type * matrix1, type tocompare, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] >= tocompare);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_sgeq_##type(type * matrix1, type tocompare, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] >= tocompare);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_ssmaller_noM_##type(type * out, type * matrix1, type tocompare, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] < tocompare);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_ssmaller_##type(type * matrix1, type tocompare, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] < tocompare);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_sgreater_noM_##type(type * out, type * matrix1, type tocompare, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] > tocompare);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM


#define REGISTER_ENUM(type) type * linalg_sgreater_##type(type * matrix1, type tocompare, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] > tocompare);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_seq_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] <= matrix2[i]);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_seq_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] <= matrix2[i]);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_greater_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] > matrix2[i]);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_greater_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] > matrix2[i]);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_geq_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] >= matrix2[i]);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_geq_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] >= matrix2[i]);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_equal_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] == matrix2[i]);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_equal_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] == matrix2[i]);\
    }\
return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool * linalg_equal_noM_##type(bool * out, type * matrix1, type * matrix2, size_t arr_len, type tolerance) {\
    for (size_t i = 0; i < arr_len; i++) {\
        out[i] = (fabs(matrix1[i] - matrix2[i]) < tolerance);\
    }\
return (out);\
}
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool * linalg_equal_##type(type * matrix1, type * matrix2, size_t arr_len, type tolerance) {\
    bool * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
        out[i] = (fabs(matrix1[i] - matrix2[i]) < tolerance);\
    }\
return (out);\
}
TEMPLATE_TYPES_FLOAT
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
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
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
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_and_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
        out[i] = (matrix1[i] && matrix2[i]);\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_and_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
        out[i] = (matrix1[i] && matrix2[i]);\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_or_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
        out[i] = (matrix1[i] || matrix2[i]);\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_or_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
        out[i] = (matrix1[i] || matrix2[i]);\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_sub_noM_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            matrix1[i] -= matrix2[i];\
    }\
    return (matrix1);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_plus_noM_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            matrix1[i] += matrix2[i];\
    }\
    return (matrix1);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_plus_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = matrix1[i] + matrix2[i];\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_plus_scalar_noM_##type(type * out, type * matrix, type value, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = matrix[i] + value;\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_plus_scalar_##type(type * matrix, type value, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = matrix[i] + value;\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_minus_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] - matrix2[i]);\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_minus_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix1[i] - matrix2[i]);\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_minus_scalar_noM_##type(type * out, type * matrix, type value, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix[i] - value);\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_minus_scalar_##type(type * matrix, type value, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = (matrix[i] - value);\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_mult_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = matrix1[i] * matrix2[i];\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_mult_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = matrix1[i] * matrix2[i];\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_mult_scalar_noM_##type(type * out, type * matrix, type mult, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = matrix[i] * mult;\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_mult_scalar_##type(type * matrix, type mult, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = matrix[i] * mult;\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_div_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = matrix1[i] / matrix2[i];\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_div_##type(type * matrix1, type * matrix2, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = matrix1[i] / matrix2[i];\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_div_scalar_noM_##type(type * out, type * matrix, type mult, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = matrix[i] / mult;\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_div_scalar_##type(type * matrix, type mult, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
            out[i] = matrix[i] / mult;\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_mask_noM_##type(type * out, type * matrix, type * mask, size_t arr_len) {\
    for (size_t i = 0; i < arr_len; i++) {\
        out[i] = matrix[i] && (mask[i] > 0);\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_mask_##type(type * matrix, type * mask, size_t arr_len) {\
    type * out = calloc(arr_len, sizeof(type));\
    for (size_t i = 0; i < arr_len; i++) {\
        out[i] = matrix[i] && (mask[i] > 0);\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_list2matrix_noM_##type(type * out, type * list, size_t row_len, size_t col_len, size_t list_len) {\
    for (size_t elem = 0; elem < list_len; elem++) {\
        out[list[2 * elem + 1] * col_len + list[2 * elem + 0]] = 1;\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_list2matrix_##type(type * list, size_t row_len, size_t col_len, size_t list_len) {\
    type * out = calloc(row_len * col_len, sizeof(type));\
    for (size_t elem = 0; elem < list_len; elem++) {\
        out[list[2 * elem + 1] * col_len + list[2 * elem + 0]] = 1;\
    }\
    return (out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_matrix2list_noM_##type(type * matrix, type * list, size_t row_len, size_t col_len) {\
    DARR_NUM(list) = 0;\
    for (size_t col = 0; col < col_len; col++) {\
        for (size_t row = 0; row < row_len; row++) {\
            if (matrix[row * col_len + col] > 0) {\
                DARR_PUT(list, col);\
                DARR_PUT(list, row);\
            }\
        }\
    }\
    return (list);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * linalg_matrix2list_##type(type * matrix, size_t row_len, size_t col_len) {\
    type * list = DARR_INIT(list, type, row_len*col_len*2);\
    for (size_t col = 0; col < col_len; col++) {\
        for (size_t row = 0; row < row_len; row++) {\
            if (matrix[row * col_len + col] > 0) {\
                DARR_PUT(list, col);\
                DARR_PUT(list, row);\
            }\
        }\
    }\
    list = DARR_REALLOC(list, (DARR_NUM(list) < NMATH_MINLEN? NMATH_MINLEN :DARR_NUM(list)));\
    return (list);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

/***************************** FIRST SET BIT **********************************/
// twos complement mathemagics:
// 9 == 0b1001
// 2 == 0b1001
#define REGISTER_ENUM(type) type nmath_firstsetBit_##type(type flags) {\
    return(flags & -flags);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM


/***************************** SWAPPING **********************************/
#define REGISTER_ENUM(type) void nmath_swap_##type(type * arr, type i1, type i2) {\
    type buffer = arr[i1];\
    arr[i1] = arr[i2];\
    arr[i2] = buffer;\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM


/***************************** INDICES&ORDERS **********************************/

#define REGISTER_ENUM(type) type * sparseOrders2indices_##type(type * in_orders_sparse, size_t orders_len) {\
    type num_indices = 0;\
    for (size_t i = 0; i < orders_len; i++) {\
        num_indices = in_orders_sparse[i] > 0 ? (num_indices + 1) : num_indices;\
    }\
    type * out = DARR_INIT(out, type, num_indices);\
    size_t j = 0;\
    for (size_t i = 0; i < orders_len; i++) {\
        if ((in_orders_sparse[i] > 0) && (j < num_indices)) {\
            out[in_orders_sparse[i] - 1] = i;\
        }\
    }\
        return (out);\
    }
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * indices2sparseOrders_##type(type * in_indices, size_t num_indices) {\
    type orders_len = 0;\
    for(size_t i = 0; i < num_indices; i++) {\
        orders_len = in_indices[i] > orders_len ? in_indices[i] : orders_len;\
    }\
    orders_len++;\
    type * out = DARR_INIT(out, type, orders_len); \
    memset(out, 0, sizeof(*out)*orders_len);\
        \
    for(size_t i = 0; i < num_indices; i++) {\
        out[in_indices[i]] = i + 1;\
    }\
    return(out);\
}
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

/******************************* PATHFINDING ***********************************/

#define REGISTER_ENUM(type) type * pathfinding_Map_unitGradient_noM_##type(type * unitgradientmap, type * in_costmap, size_t row_len, size_t col_len, struct nmath_point_##type * in_targets, size_t unit_num) {\
    for (type  col = 0; col < col_len; col++) {\
        for (type  row = 0; row < row_len; row++) {\
            if (in_costmap[row * col_len + col] < NMATH_PUSHPULLMAP_BLOCKED) {\
                unitgradientmap[row * col_len + col] = NMATH_GRADIENTMAP_BLOCKED;\
            } else {\
                unitgradientmap[row * col_len + col] = row_len + col_len;\
            }\
        }\
    }\
    struct nmath_node_##type * open = DARR_INIT(open, struct nmath_node_##type, row_len * col_len);\
    struct nmath_node_##type * closed = DARR_INIT(closed, struct nmath_node_##type, row_len * col_len);\
    struct nmath_node_##type current, neighbor;\
    for (type  i = 0; i < unit_num; i++) {\
        unitgradientmap[in_targets[i].y * col_len + in_targets[i].x] = NMATH_GRADIENTMAP_UNIT;\
        current.x = in_targets[i].x;\
        current.y = in_targets[i].y;\
        current.distance = 1;\
        DARR_PUT(open, current);\
    }\
    while (DARR_NUM(open) > 0) {\
        current = DARR_POP(open);\
        DARR_PUT(closed, current);\
        for (type  sq_neighbor = 0; sq_neighbor < NMATH_SQUARE_NEIGHBOURS; sq_neighbor++) {\
            neighbor.x = nmath_inbounds_##type(q_cycle4_mzpz(sq_neighbor) + current.x, 0, col_len - 1);\
            neighbor.y = nmath_inbounds_##type(q_cycle4_zmzp(sq_neighbor) + current.y, 0, row_len - 1);\
            neighbor.distance = unitgradientmap[current.y * col_len + current.x] + 1;\
            if (in_costmap[neighbor.y * col_len + neighbor.x] >= NMATH_COSTMAP_MOVEABLEMIN) {\
                if (neighbor.distance < unitgradientmap[neighbor.y * col_len + neighbor.x]) {\
                    unitgradientmap[neighbor.y * col_len + neighbor.x] = neighbor.distance;\
                }\
                bool neighbor_inclosed = false;\
                for (int32_t k = 0; k < DARR_NUM(closed); k++) {\
                    if ((neighbor.x == closed[k].x) && (neighbor.y == closed[k].y)) {\
                        neighbor_inclosed = true;\
                        if (neighbor.distance < closed[k].distance) {\
                            neighbor_inclosed = false;\
                            DARR_DEL(closed, k);\
                        }\
                        break;\
                    }\
                }\
                if (!neighbor_inclosed) {\
                    DARR_PUT(open, neighbor);\
                }\
            }\
        }\
    }\
    return (unitgradientmap);\
}
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * pathfinding_Map_unitGradient_##type(type * in_costmap, size_t row_len, size_t col_len, struct nmath_point_##type * in_targets, size_t unit_num) {\
    type * unitgradientmap = calloc(row_len * col_len, sizeof(type));\
    for (type  col = 0; col < col_len; col++) {\
        for (type  row = 0; row < row_len; row++) {\
            if (in_costmap[row * col_len + col] < NMATH_PUSHPULLMAP_BLOCKED) {\
                unitgradientmap[row * col_len + col] = NMATH_GRADIENTMAP_BLOCKED;\
            } else {\
                unitgradientmap[row * col_len + col] = row_len + col_len;\
            }\
        }\
    }\
    struct nmath_node_##type * open = DARR_INIT(open, struct nmath_node_##type, row_len * col_len);\
    struct nmath_node_##type * closed = DARR_INIT(closed, struct nmath_node_##type, row_len * col_len);\
    struct nmath_node_##type current, neighbor;\
    for (type  i = 0; i < unit_num; i++) {\
        unitgradientmap[in_targets[i].y * col_len + in_targets[i].x] = NMATH_GRADIENTMAP_UNIT;\
        current.x = in_targets[i].x;\
        current.y = in_targets[i].y;\
        current.distance = 1;\
        DARR_PUT(open, current);\
    }\
    while (DARR_NUM(open) > 0) {\
        current = DARR_POP(open);\
        DARR_PUT(closed, current);\
        for (type  sq_neighbor = 0; sq_neighbor < NMATH_SQUARE_NEIGHBOURS; sq_neighbor++) {\
            neighbor.x = nmath_inbounds_##type(q_cycle4_mzpz(sq_neighbor) + current.x, 0, col_len - 1);\
            neighbor.y = nmath_inbounds_##type(q_cycle4_zmzp(sq_neighbor) + current.y, 0, row_len - 1);\
            neighbor.distance = unitgradientmap[current.y * col_len + current.x] + 1;\
            if (in_costmap[neighbor.y * col_len + neighbor.x] >= NMATH_COSTMAP_MOVEABLEMIN) {\
                if (neighbor.distance < unitgradientmap[neighbor.y * col_len + neighbor.x]) {\
                    unitgradientmap[neighbor.y * col_len + neighbor.x] = neighbor.distance;\
                }\
                bool neighbor_inclosed = false;\
                for (int32_t k = 0; k < DARR_NUM(closed); k++) {\
                    if ((neighbor.x == closed[k].x) && (neighbor.y == closed[k].y)) {\
                        neighbor_inclosed = true;\
                        if (neighbor.distance < closed[k].distance) {\
                            neighbor_inclosed = false;\
                            DARR_DEL(closed, k);\
                        }\
                        break;\
                    }\
                }\
                if (!neighbor_inclosed) {\
                    DARR_PUT(open, neighbor);\
                }\
            }\
        }\
    }\
    return (unitgradientmap);\
}
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type linalg_distance_manhattan_point_##type(struct nmath_point_##type start, struct nmath_point_##type end) {\
    /* Does not include endpoints */ \
    type  distance = labs(start.x - end.x) + labs(start.y - end.y);\
    return (distance);\
}
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type linalg_distance_manhattan_##type(type x_0, type y_0, type x_1, type y_1) {\
    /* Does not include endpoints */ \
    type distance = labs(x_0 - x_1) + labs(y_0 - y_1);\
    return (distance);\
}
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type distance_euclidian_##type(struct nmath_point_##type start, struct nmath_point_##type end) {\
    type term_x = (start.x - end.x) > type##_MAX / (start.x - end.x) ? type##_MAX : (start.x - end.x) * (start.x - end.x);\
    type term_y = (start.y - end.y) > type##_MAX / (start.y - end.y) ? type##_MAX : (start.y - end.y) * (start.y - end.y);\
    type distance = (type)carmack_sqrt_int32_t(term_x) + (type)carmack_sqrt_int32_t(term_y);\
    return (distance);\
}
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) bool pathfinding_isReachable_##type(struct nmath_point_##type in_nmath_point_##type, type  * in_movemap, size_t ROW_LEN, size_t COL_LEN) {\
    bool out;\
    if ((nmath_inbounds_##type(in_nmath_point_##type.x, 0, COL_LEN)) && (nmath_inbounds_##type(in_nmath_point_##type.y, 0, ROW_LEN))) {\
        out = (in_movemap[in_nmath_point_##type.y * COL_LEN + in_nmath_point_##type.x] > 0);\
    }\
    return (out);\
}
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type  * pathfinding_Map_Attackfrom_noM_##type(type * attackfrommap, type * in_movemap, size_t row_len, size_t col_len, struct nmath_point_##type in_target, uint8_t range[2]) {\
    struct nmath_point_##type perimeter_nmath_point_##type, delta;\
    for (uint8_t row = 0; row < row_len; row++) {\
        for (uint8_t col = 0; col < col_len; col++) {\
            attackfrommap[(row * col_len + col)] = NMATH_ATTACKFROM_BLOCKED;\
        }\
    }\
    for (type i_range = range[0]; i_range <= range[1]; i_range++) {\
        for (type  sq_neighbor = 0; sq_neighbor < (i_range * NMATH_SQUARE_NEIGHBOURS); sq_neighbor++) {\
            delta.x = nmath_inbounds_##type(i_range * q_cycle4_mzpz(sq_neighbor) + (sq_neighbor / NMATH_SQUARE_NEIGHBOURS) * q_cycle4_pmmp(sq_neighbor), -in_target.x, col_len - in_target.x);\
            delta.y = nmath_inbounds_##type(i_range * q_cycle4_zmzp(sq_neighbor) + (sq_neighbor / NMATH_SQUARE_NEIGHBOURS) * q_cycle4_ppmm(sq_neighbor), -in_target.y, row_len - in_target.y);\
            perimeter_nmath_point_##type.x = in_target.x + delta.x;\
            perimeter_nmath_point_##type.y = in_target.y + delta.y;\
            if (in_movemap[perimeter_nmath_point_##type.y * col_len + perimeter_nmath_point_##type.x] >= NMATH_MOVEMAP_MOVEABLEMIN) {\
                attackfrommap[perimeter_nmath_point_##type.y * col_len + perimeter_nmath_point_##type.x] = i_range;\
                }\
            }\
        }\
    return (attackfrommap);\
}
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type  * pathfinding_Map_Attackfrom_##type(type  * in_movemap, size_t row_len, size_t col_len, struct nmath_point_##type in_target, uint8_t range[2], uint8_t mode_output) {\
    struct nmath_point_##type perimeter_nmath_point_##type, delta;\
    type  * attackfrommap = NULL;\
    switch (mode_output) {\
        case (NMATH_POINTS_MODE_LIST):\
            attackfrommap = DARR_INIT(attackfrommap, type, row_len * col_len * NMATH_TWO_D);\
            break;\
        case (NMATH_POINTS_MODE_MATRIX):\
            attackfrommap = calloc(row_len * col_len, sizeof(type));\
            for (uint8_t row = 0; row < row_len; row++) {\
                for (uint8_t col = 0; col < col_len; col++) {\
                    attackfrommap[(row * col_len + col)] = NMATH_ATTACKFROM_BLOCKED;\
                }\
            }\
            break;\
    }\
    for (type i_range = range[0]; i_range <= range[1]; i_range++) {\
        for (type  sq_neighbor = 0; sq_neighbor < (i_range * NMATH_SQUARE_NEIGHBOURS); sq_neighbor++) {\
            delta.x = nmath_inbounds_##type(i_range * q_cycle4_mzpz(sq_neighbor) + (sq_neighbor / NMATH_SQUARE_NEIGHBOURS) * q_cycle4_pmmp(sq_neighbor), -in_target.x, col_len - in_target.x);\
            delta.y = nmath_inbounds_##type(i_range * q_cycle4_zmzp(sq_neighbor) + (sq_neighbor / NMATH_SQUARE_NEIGHBOURS) * q_cycle4_ppmm(sq_neighbor), -in_target.y, row_len - in_target.y);\
            perimeter_nmath_point_##type.x = in_target.x + delta.x;\
            perimeter_nmath_point_##type.y = in_target.y + delta.y;\
            if (in_movemap[perimeter_nmath_point_##type.y * col_len + perimeter_nmath_point_##type.x] >= NMATH_MOVEMAP_MOVEABLEMIN) {\
                switch (mode_output) {\
                    case NMATH_POINTS_MODE_LIST:\
                        if (!linalg_list_isIn_2D_##type(attackfrommap, DARR_NUM(attackfrommap) / NMATH_TWO_D, perimeter_nmath_point_##type.x, perimeter_nmath_point_##type.y)) {\
                            DARR_PUT(attackfrommap, perimeter_nmath_point_##type.x);\
                            DARR_PUT(attackfrommap, perimeter_nmath_point_##type.y);\
                        }\
                        break;\
                    case NMATH_POINTS_MODE_MATRIX:\
                        attackfrommap[perimeter_nmath_point_##type.y * col_len + perimeter_nmath_point_##type.x] = i_range;\
                        break;\
                }\
            }\
        }\
    }\
    return (attackfrommap);\
}
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * pathfinding_Map_Attackto_noM_##type(type * attackmap, type * move_matrix, size_t row_len, size_t col_len, type  move, uint8_t range[2], uint8_t mode_movetile) {\
    type *move_list = NULL;\
    type  subrangey_min, subrangey_max;\
    struct nmath_point_##type temp_nmath_point_##type;\
    move_list = linalg_matrix2list_##type(move_matrix, row_len, col_len);\
    size_t list_len = DARR_NUM(move_list) / NMATH_TWO_D;\
    for (uint8_t row = 0; row < row_len; row++) {\
        for (uint8_t col = 0; col < col_len; col++) {\
            attackmap[(row * col_len + col)] = NMATH_ATTACKMAP_BLOCKED;\
        }\
    }\
    bool add_nmath_point_##type;\
    switch (mode_movetile) {\
        case NMATH_MOVETILE_INCLUDE:\
            add_nmath_point_##type = true;\
            break;\
        default:\
            add_nmath_point_##type = true;\
            break;\
    }\
    for (type i = 0; i < list_len; i++) {\
        for (type rangex = 0; rangex <= range[1]; rangex++) {\
            subrangey_min = (rangex > range[0]) ? 0 : (range[0] - rangex);\
            subrangey_max = (rangex > range[1]) ? 0 : (range[1] - rangex);\
            for (type  rangey = subrangey_min; rangey <= subrangey_max; rangey++) {\
                for (int8_t sq_neighbor = 0; sq_neighbor < NMATH_SQUARE_NEIGHBOURS; sq_neighbor++) {\
                    temp_nmath_point_##type.x = nmath_inbounds_##type(move_list[i * NMATH_TWO_D + 0] + q_cycle4_pmmp(sq_neighbor) * rangex, 0, col_len - 1);\
                    temp_nmath_point_##type.y = nmath_inbounds_##type(move_list[i * NMATH_TWO_D + 1] + q_cycle4_ppmm(sq_neighbor) * rangey, 0, row_len - 1);\
                    switch (mode_movetile) {\
                        case NMATH_MOVETILE_EXCLUDE:\
                            add_nmath_point_##type = (move_matrix[temp_nmath_point_##type.y * col_len + temp_nmath_point_##type.x] == NMATH_MOVEMAP_BLOCKED);\
                            break;\
                    }\
                    if (add_nmath_point_##type) {\
                        attackmap[temp_nmath_point_##type.y * col_len + temp_nmath_point_##type.x] = NMATH_ATTACKMAP_MOVEABLEMIN;\
                    }\
                }\
            }\
        }\
    }\
    return (attackmap);\
}
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM


#define REGISTER_ENUM(type) type * pathfinding_Map_Attackto_##type(type * move_matrix, size_t row_len, size_t col_len, type  move, uint8_t range[2], uint8_t mode_output, uint8_t mode_movetile) {\
    type * attackmap = NULL, *move_list = NULL;\
    type  subrangey_min, subrangey_max;\
    struct nmath_point_##type temp_nmath_point_##type;\
    move_list = linalg_matrix2list_##type(move_matrix, row_len, col_len);\
    size_t list_len = DARR_NUM(move_list) / NMATH_TWO_D;\
    switch (mode_output) {\
        case (NMATH_POINTS_MODE_LIST):\
            attackmap = DARR_INIT(attackmap, type, row_len * col_len * NMATH_TWO_D);\
            break;\
        case (NMATH_POINTS_MODE_MATRIX):\
                attackmap = calloc(row_len * col_len, sizeof(type));\
            for (uint8_t row = 0; row < row_len; row++) {\
                for (uint8_t col = 0; col < col_len; col++) {\
                    attackmap[(row * col_len + col)] = NMATH_ATTACKMAP_BLOCKED;\
                }\
            }\
            break;\
    }\
    bool add_nmath_point_##type;\
    switch (mode_movetile) {\
        case NMATH_MOVETILE_INCLUDE:\
            add_nmath_point_##type = true;\
            break;\
        default:\
            add_nmath_point_##type = true;\
            break;\
    }\
    for (type i = 0; i < list_len; i++) {\
        for (type rangex = 0; rangex <= range[1]; rangex++) {\
            subrangey_min = (rangex > range[0]) ? 0 : (range[0] - rangex);\
            subrangey_max = (rangex > range[1]) ? 0 : (range[1] - rangex);\
            for (type  rangey = subrangey_min; rangey <= subrangey_max; rangey++) {\
                for (int8_t sq_neighbor = 0; sq_neighbor < NMATH_SQUARE_NEIGHBOURS; sq_neighbor++) {\
                    temp_nmath_point_##type.x = nmath_inbounds_##type(move_list[i * NMATH_TWO_D + 0] + q_cycle4_pmmp(sq_neighbor) * rangex, 0, col_len - 1);\
                    temp_nmath_point_##type.y = nmath_inbounds_##type(move_list[i * NMATH_TWO_D + 1] + q_cycle4_ppmm(sq_neighbor) * rangey, 0, row_len - 1);\
                    switch (mode_movetile) {\
                        case NMATH_MOVETILE_EXCLUDE:\
                            add_nmath_point_##type = (move_matrix[temp_nmath_point_##type.y * col_len + temp_nmath_point_##type.x] == NMATH_MOVEMAP_BLOCKED);\
                            break;\
                    }\
                    if (add_nmath_point_##type) {\
                        switch (mode_output) {\
                            case NMATH_POINTS_MODE_LIST:\
                                if (!linalg_list_isIn_2D_##type(attackmap, DARR_NUM(attackmap) / NMATH_TWO_D, temp_nmath_point_##type.x, temp_nmath_point_##type.y)) {\
                                    DARR_PUT(attackmap, temp_nmath_point_##type.x);\
                                    DARR_PUT(attackmap, temp_nmath_point_##type.y);\
                                }\
                                break;\
                            case NMATH_POINTS_MODE_MATRIX:\
                                attackmap[temp_nmath_point_##type.y * col_len + temp_nmath_point_##type.x] = NMATH_ATTACKMAP_MOVEABLEMIN;\
                                break;\
                        }\
                    }\
                }\
            }\
        }\
    }\
    free(move_list);\
    return (attackmap);\
}
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type  * pathfinding_Map_Moveto_Hex_##type(type  * cost_matrix, size_t depth_len, size_t col_len, struct nmath_hexpoint_##type start, type move, uint8_t mode_output) {\
    type  * move_matrix = NULL;\
    switch (mode_output) {\
        case (NMATH_POINTS_MODE_LIST):\
            move_matrix = DARR_INIT(move_matrix, type, depth_len * col_len * NMATH_TWO_D);\
            break;\
        case (NMATH_POINTS_MODE_MATRIX):\
            move_matrix = calloc(depth_len * col_len, sizeof(type));\
            for (uint8_t depth = 0; depth < depth_len; depth++) {\
                for (uint8_t col = 0; col < col_len; col++) {\
                    move_matrix[(depth * col_len + col)] = NMATH_MOVEMAP_BLOCKED;\
                }\
            }\
            break;\
    }\
    struct nmath_hexnode_##type * open = DARR_INIT(open, struct nmath_hexnode_##type, depth_len * col_len);\
    struct nmath_hexnode_##type * closed = DARR_INIT(closed, struct nmath_hexnode_##type, depth_len * col_len);\
    struct nmath_hexnode_##type current = {start.x, start.y, start.z, 0}, neighbor = {0};\
    DARR_PUT(open, current);\
    bool found;\
    while (DARR_NUM(open) > 0) {\
        current = DARR_POP(open);\
        DARR_PUT(closed, current);\
        switch (mode_output) {\
            case NMATH_POINTS_MODE_MATRIX:\
                if ((move_matrix[current.z * col_len + current.x] == 0) || (move_matrix[current.z * col_len + current.x] > (current.distance + 1))) {\
                    move_matrix[current.z * col_len + current.x] = current.distance + 1;\
                }\
                break;\
            case NMATH_POINTS_MODE_LIST:\
                found = linalg_list_isIn_2D_##type(move_matrix, DARR_NUM(move_matrix) / NMATH_TWO_D, current.x, current.z);\
                if (!found) {\
                    DARR_PUT(move_matrix, current.x);\
                    DARR_PUT(move_matrix, current.z);\
                }\
                break;\
        }\
        for (type hex_neighbor = 0; hex_neighbor < NMATH_HEXAGON_NEIGHBOURS; hex_neighbor++) {\
            neighbor.x = nmath_inbounds_##type(current.x + q_cycle6_mppmzz(hex_neighbor), 0, col_len - 1);\
            neighbor.z = nmath_inbounds_##type(current.z + q_cycle6_pmzzmp(hex_neighbor), 0, depth_len - 1);\
            if (cost_matrix[current.z * col_len + current.x] >= 0) {\
                neighbor.distance = current.distance + cost_matrix[current.z * col_len + current.x];\
                if ((neighbor.distance <= move) && (cost_matrix[neighbor.z * col_len + neighbor.x] >= NMATH_COSTMAP_MOVEABLEMIN)) {\
                    bool neighbor_inclosed = false;\
                    for (int32_t k = 0; k < DARR_NUM(closed); k++) {\
                        if ((neighbor.x == closed[k].x) && (neighbor.y == closed[k].y)  && (neighbor.z == closed[k].z)) {\
                            neighbor_inclosed = true;\
                            if (neighbor.distance < closed[k].distance) {\
                                neighbor_inclosed = false;\
                                DARR_DEL(closed, k);\
                            }\
                            break;\
                        }\
                    }\
                    if (!neighbor_inclosed) {\
                        DARR_PUT(open, neighbor);\
                    }\
                }\
            }\
        }\
    }\
    return (move_matrix);\
}
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * pathfinding_Map_Visible_noM_##type(type * sightmap, type * block_matrix, size_t row_len, size_t col_len, struct nmath_point_##type start, type sight) {\
     struct nmath_point_##type perimeter_nmath_point_##type = {0, 0}, delta = {0, 0}, interpolated = {0, 0};\
    bool visible;\
    for (uint8_t row = 0; row < row_len; row++) {\
        for (uint8_t col = 0; col < col_len; col++) {\
            sightmap[(row * col_len + col)] = NMATH_SIGHTMAP_BLOCKED;\
        }\
    }\
    sightmap[start.y * col_len + start.x] = NMATH_SIGHTMAP_OBSERVER;\
    for (type  distance = 1; distance <= sight; distance++) {\
        for (type  sq_neighbor = 0; sq_neighbor < (distance * NMATH_SQUARE_NEIGHBOURS); sq_neighbor++) {\
            delta.x = nmath_inbounds_##type(distance * q_cycle4_mzpz(sq_neighbor) + (sq_neighbor / NMATH_SQUARE_NEIGHBOURS) * q_cycle4_pmmp(sq_neighbor), -start.x, col_len - start.x);\
            delta.y = nmath_inbounds_##type(distance * q_cycle4_zmzp(sq_neighbor) + (sq_neighbor / NMATH_SQUARE_NEIGHBOURS) * q_cycle4_ppmm(sq_neighbor), -start.y, row_len - start.y);\
            perimeter_nmath_point_##type.x = start.x + delta.x;\
            perimeter_nmath_point_##type.y = start.y + delta.y;\
            visible = true;\
            for (int32_t interp_dist = 1; interp_dist < distance; interp_dist++) {\
                interpolated.x = start.x + (delta.x == 0 ? 0 : (int32_t)lround(interp_dist * delta.x * (1.0f / distance)));\
                interpolated.y = start.y + (delta.y == 0 ? 0 : (int32_t)lround(interp_dist * delta.y * (1.0f / distance)));\
                if ((interpolated.x != start.x) || (interpolated.y != start.y)) {\
                    if (block_matrix[interpolated.y * col_len + interpolated.x] >= NMATH_BLOCKMAP_MIN) {\
                        visible = false;\
                        break;\
                    }\
                }\
            }\
            if (visible) {\
                switch (block_matrix[perimeter_nmath_point_##type.y * col_len + perimeter_nmath_point_##type.x]) {\
                    case NMATH_BLOCKMAP_BLOCKED:\
                        sightmap[perimeter_nmath_point_##type.y * col_len + perimeter_nmath_point_##type.x] = NMATH_SIGHTMAP_VISIBLE;\
                        break;\
                    default:\
                        sightmap[perimeter_nmath_point_##type.y * col_len + perimeter_nmath_point_##type.x] = NMATH_SIGHTMAP_WALL;\
                        break;\
                }\
            }\
        }\
    }\
    return (sightmap);\
}
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * pathfinding_Map_Visible_##type(type  * block_matrix, size_t row_len, size_t col_len, struct nmath_point_##type start, type  sight, uint8_t mode_output) {\
    type  * sightmap = NULL;\
    struct nmath_point_##type perimeter_nmath_point_##type = {0, 0}, delta = {0, 0}, interpolated = {0, 0};\
    bool visible;\
    switch (mode_output) {\
        case (NMATH_POINTS_MODE_LIST):\
            sightmap = DARR_INIT(sightmap, type, row_len * col_len * NMATH_TWO_D);\
            break;\
        case (NMATH_POINTS_MODE_MATRIX):\
            sightmap = calloc(row_len * col_len, sizeof(type));\
            for (uint8_t row = 0; row < row_len; row++) {\
                for (uint8_t col = 0; col < col_len; col++) {\
                    sightmap[(row * col_len + col)] = NMATH_SIGHTMAP_BLOCKED;\
                }\
            }\
            break;\
    }\
    sightmap[start.y * col_len + start.x] = NMATH_SIGHTMAP_OBSERVER;\
    for (type  distance = 1; distance <= sight; distance++) {\
        for (type  sq_neighbor = 0; sq_neighbor < (distance * NMATH_SQUARE_NEIGHBOURS); sq_neighbor++) {\
            delta.x = nmath_inbounds_##type(distance * q_cycle4_mzpz(sq_neighbor) + (sq_neighbor / NMATH_SQUARE_NEIGHBOURS) * q_cycle4_pmmp(sq_neighbor), -start.x, col_len - start.x);\
            delta.y = nmath_inbounds_##type(distance * q_cycle4_zmzp(sq_neighbor) + (sq_neighbor / NMATH_SQUARE_NEIGHBOURS) * q_cycle4_ppmm(sq_neighbor), -start.y, row_len - start.y);\
            perimeter_nmath_point_##type.x = start.x + delta.x;\
            perimeter_nmath_point_##type.y = start.y + delta.y;\
            visible = true;\
            for (int32_t interp_dist = 1; interp_dist < distance; interp_dist++) {\
                interpolated.x = start.x + (delta.x == 0 ? 0 : (int32_t)lround(interp_dist * delta.x * (1.0f / distance)));\
                interpolated.y = start.y + (delta.y == 0 ? 0 : (int32_t)lround(interp_dist * delta.y * (1.0f / distance)));\
                if ((interpolated.x != start.x) || (interpolated.y != start.y)) {\
                    if (block_matrix[interpolated.y * col_len + interpolated.x] >= NMATH_BLOCKMAP_MIN) {\
                        visible = false;\
                        break;\
                    }\
                }\
            }\
            if (visible) {\
                switch (mode_output) {\
                    case (NMATH_POINTS_MODE_MATRIX):\
                        switch (block_matrix[perimeter_nmath_point_##type.y * col_len + perimeter_nmath_point_##type.x]) {\
                            case NMATH_BLOCKMAP_BLOCKED:\
                                sightmap[perimeter_nmath_point_##type.y * col_len + perimeter_nmath_point_##type.x] = NMATH_SIGHTMAP_VISIBLE;\
                                break;\
                            default:\
                                sightmap[perimeter_nmath_point_##type.y * col_len + perimeter_nmath_point_##type.x] = NMATH_SIGHTMAP_WALL;\
                                break;\
                        }\
                        break;\
                }\
            }\
        }\
    }\
    return (sightmap);\
}
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * pathfinding_Map_Visible_Hex_##type(type  * block_matrix, size_t depth_len, size_t col_len, struct nmath_hexpoint_##type start, type sight, uint8_t mode_output) {\
    type  * sightmap = NULL;\
    struct nmath_hexpoint_##type perimeter_nmath_point_##type = {0, 0, 0}, delta = {0, 0, 0}, interpolated = {0, 0, 0};\
    bool visible;\
    switch (mode_output) {\
        case (NMATH_POINTS_MODE_LIST):\
            sightmap = DARR_INIT(sightmap, type, depth_len * col_len * NMATH_TWO_D);\
            break;\
        case (NMATH_POINTS_MODE_MATRIX):\
            sightmap = calloc(depth_len * col_len, sizeof(type));\
            for (uint8_t depth = 0; depth < depth_len; depth++) {\
                for (uint8_t col = 0; col < col_len; col++) {\
                    sightmap[(depth * col_len + col)] = NMATH_SIGHTMAP_BLOCKED;\
                }\
            }\
            break;\
    }\
    sightmap[start.z * col_len + start.x] = NMATH_SIGHTMAP_OBSERVER;\
    for (type  distance = 1; distance <= sight; distance++) {\
        for (type  perimeter_tile = 0; perimeter_tile < (distance * NMATH_HEXAGON_NEIGHBOURS); perimeter_tile++) {/*iterates perimeter tiles at \distance */\
            delta.x = nmath_inbounds_##type(distance * q_cycle6_mppmzz(perimeter_tile) + perimeter_tile / NMATH_HEXAGON_NEIGHBOURS * q_cycle6_pmzzmp(perimeter_tile), -start.x, col_len - start.x);\
            delta.z = nmath_inbounds_##type(distance * q_cycle6_pmzzmp(perimeter_tile) + perimeter_tile / NMATH_HEXAGON_NEIGHBOURS * q_cycle6_zzmppm(perimeter_tile), -start.z, depth_len - start.z);\
            perimeter_nmath_point_##type.x = start.x + delta.x;\
            perimeter_nmath_point_##type.z = start.z + delta.z;\
            visible = true;\
            for (int32_t interp_dist = 1; interp_dist < distance; interp_dist++) {\
                interpolated.x = start.x + (delta.x == 0 ? 0 : (int32_t)lround(interp_dist * delta.x * (1.0f / distance)));\
                interpolated.z = start.z + (delta.z == 0 ? 0 : (int32_t)lround(interp_dist * delta.z * (1.0f / distance)));\
                if ((interpolated.x != start.x) || (interpolated.z != start.z)) {\
                    if (block_matrix[interpolated.z * col_len + interpolated.x] >= NMATH_BLOCKMAP_MIN) {\
                        visible = false;\
                        break;\
                    }\
                }\
            }\
            if (visible) {\
                switch (block_matrix[perimeter_nmath_point_##type.z * col_len + perimeter_nmath_point_##type.x]) {\
            case NMATH_BLOCKMAP_BLOCKED:\
                        sightmap[perimeter_nmath_point_##type.z * col_len + perimeter_nmath_point_##type.x] = NMATH_SIGHTMAP_VISIBLE;\
                        break;\
                    default:\
                        sightmap[perimeter_nmath_point_##type.z * col_len + perimeter_nmath_point_##type.x] = NMATH_SIGHTMAP_WALL;\
                        break;\
                }\
            }\
        }\
    }\
    return (sightmap);\
}
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) type * pathfinding_Path_step2position_##type(type  * step_list, size_t list_len, struct nmath_point_##type start) {\
    type  * path_position = DARR_INIT(path_position, type, ((list_len + 1) * 2));\
    DARR_PUT(path_position, start.x);\
    DARR_PUT(path_position, start.y);\
    for (type  i = 0; i < list_len; i++) {\
        DARR_PUT(path_position, (path_position[i * NMATH_TWO_D + 0] + step_list[i * NMATH_TWO_D + 0]));\
        DARR_PUT(path_position, (path_position[i * NMATH_TWO_D + 1] + step_list[i * NMATH_TWO_D + 1]));\
    }\
    return (path_position);\
}
TEMPLATE_TYPES_SINT
#undef REGISTER_ENUM
