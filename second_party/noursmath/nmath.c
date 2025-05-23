#include "nmath.h"

/*********************************** DTAB ************************************/


/************************* MULTIPLY N TIMES *****************************/

// float nmath_slowpow(float base, uint32_t exponent) {
//     /* Super fast for -O1/-O2 optimization */
//     if (exponent == 0)
//         return(1.0f);
//     float out = base;
//     for (int i = 0; i < (exponent - 1); i++)
//         out *= base;
//     return (out);
// }


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

size_t dtab_found(struct dtab *dtab_ptr, uint64_t in_hash) {
    size_t pos = DTAB_NULL;
    for (size_t i = 0; i < dtab_ptr->num; i++) {
        if (dtab_ptr->keys[i] == in_hash) {
            pos = i;
            break;
        }
    }
    return (pos);
}

const void *dtab_get_const(struct dtab *dtab_ptr, uint64_t in_hash) {
    return(dtab_get(dtab_ptr, in_hash));
}

void *dtab_get(struct dtab *dtab_ptr, uint64_t in_hash) {
    void *out = NULL;
    size_t pos = dtab_found(dtab_ptr, in_hash);
    if (pos) {
        dtab_byte_t * values_bytes = (dtab_byte_t *)(dtab_ptr->values);
        out = (void *)(values_bytes + (dtab_ptr->bytesize * pos));
    }
    return (out);
}

void dtab_add(struct dtab *dtab_ptr, void *value, uint64_t in_hash) {
    dtab_byte_t *values_bytes, *newvalue_bytes;
    size_t pos = dtab_found(dtab_ptr, in_hash);
    if (!pos) {
        dtab_ptr->keys[dtab_ptr->num] = in_hash;
        values_bytes    = (dtab_byte_t *)(dtab_ptr->values);
        newvalue_bytes  = values_bytes + (dtab_ptr->bytesize * dtab_ptr->num);
        dtab_ptr->num++;
    } else {
        values_bytes    = (dtab_byte_t *)(dtab_ptr->values);
        newvalue_bytes  = values_bytes + (dtab_ptr->bytesize * pos);
    }
    memcpy(newvalue_bytes, value, dtab_ptr->bytesize);
    if (dtab_ptr->num == dtab_ptr->len)
        DTAB_GROW(dtab_ptr);

}

void dtab_del(struct dtab * dtab_ptr, uint64_t in_hash) {
    size_t pos = dtab_found(dtab_ptr, in_hash);
    if ((pos) && (pos < dtab_ptr->num)) {
        memmove(dtab_ptr->keys + pos, dtab_ptr->keys + pos + 1, (dtab_ptr->num - pos - 1)*sizeof(uint64_t)) ;
        dtab_byte_t * values_bytes = (dtab_byte_t *)(dtab_ptr->values);
        memmove(values_bytes + pos * dtab_ptr->bytesize, values_bytes + (pos + 1)*dtab_ptr->bytesize, (dtab_ptr->num - pos - 1)*dtab_ptr->bytesize);
        dtab_ptr->num--;
    }

}

void dtab_del_scramble(struct dtab * dtab_ptr, uint64_t in_hash) {
    size_t pos = dtab_found(dtab_ptr, in_hash);
    if ((pos) && (pos < dtab_ptr->num))  {
        memmove(dtab_ptr->keys + pos, dtab_ptr->keys + dtab_ptr->num - 1, sizeof(uint64_t));
        dtab_byte_t * values_bytes = (dtab_byte_t *)(dtab_ptr->values);
        memmove(values_bytes + pos * dtab_ptr->bytesize, values_bytes + dtab_ptr->num * dtab_ptr->bytesize, dtab_ptr->bytesize);
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
    int32_t distance = current - destination;
    return ((int32_t)(distance / geo_factor));
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
#undef REGISTER_ENUM

/******************************* PATHFINDING ***********************************/

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
