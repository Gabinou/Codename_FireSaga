
#ifndef MACROS_H
#define MACROS_H

/* --- Macros --- */

/* -- Simple string manipulation -- */
/* NOTE: STRINGIZE, ENUMS AND MACROS
enum VALUES CANNOT be STRINGIZE'd
only the enum NAMES gets STRINGIZE'd
    Example: define
        #define MYANT 2
        STRINGIFY(MYANT)   -> "MYANT"
        STRINGIZE(MYANT)   -> "2"
    Example: enum
        enum foo {bar = 2};
        STRINGIFY(bar)          -> "bar"
        STRINGIZE(bar)          -> "bar"
*/
#define STRINGIFY(x) #x
#define STRINGIZE(x) STRINGIFY(x)

#define SOTA_CONCAT( arg1, arg2) SOTA_CONCAT1(arg1, arg2)
#define SOTA_CONCAT1(arg1, arg2) SOTA_CONCAT2(arg1, arg2)
#define SOTA_CONCAT2(arg1, arg2) arg1##arg2

/* -- tnecs wrappers -- */
#define IES_C_ID(name) name##_ID
#define IES_GET_C(world, entity, component) \
    tnecs_get_C(world, entity, IES_C_ID(component))
#define IES_E_HAS_C(world, entity, cID) \
    TNECS_E_HAS_C(world, entity, cID##_ID)
#define IES_E_CREATE_wC(world, ...) \
    TNECS_E_CREATE_wC(world, __VA_ARGS__)
#define IES_C_ID_2_A(...) \
    TNECS_C_IDS2A(__VA_ARGS__)

/* --- Struct member byte offset ---
**  1- Create pointer to struct at address 0 `(name *)0`
**  2- Access member, dereferencing `->member`
**  3- Get address of member `&` (effectively offset to member)
**  4- Cast to integer, without warning (int)(intptr_t) */
#define BYTE_OFFSET(name, member) \
    (long long int)(&((name *)0)->member)

/* --- Flattened arrays indices --- */
/* Note: [x -> col, y -> row, z ->depth] */
#define sota_2D_index(x, y, col_len) (y * col_len + x)
// Revert 2D_index (y * col_len + x) to [x, y]:
//  x: (index % col_len) removes the whole y part
//  y: (index / col_len) truncates the not whole x part

#define sota_3D_index(x, y, z, x_len, y_len) (z * x_len * y_len + y * x_len + x)
// Revert 3D_index (z * col_len * row_len + y * col_len + x) to [x, y, z]:
//  x: (index % col_len)                removes the whole z and y part
//  y: ((index / col_len) % row_len)    removes the whole z part, truncates the x
//  y: (index / (col_len * row_len))    truncates the not whole x, y parts

/* --- Spritesheet index --- */
/* Spritesheet example, 8 cols: */
/*          0   x -->           */
/*             --------         */
/*          y  --------         */
/*          |  --------         */
/*          v  --------         */
/*             --------         */

#define sota_ss_x(ind, col_len) (ind % col_len)
#define sota_ss_y(ind, col_len) (ind / col_len)

/* --- Flagsum --- */
#define flagsum_isIn(flag, flagsum) ((flag & flagsum) > 0)

/* -- Pixels to tiles -- */
#define SOTA_PIXEL2TILEMAP(pos, tilesize, offset, zoom) \
    (floor((pos - offset) / zoom / tilesize))
#define SOTA_TILEMAP2PIXEL(pos, tilesize, offset, zoom) \
    (pos * zoom * tilesize) + offset

/* -- Linear transformations -- */
#define SOTA_ZOOM(  in, zoom) (in * zoom)
#define SOTA_DEZOOM(in, zoom) (in / zoom)
#define SOTA_ZOOMTOPOINT(scale, x, origin) \
    (scale * (x - origin) + origin)

/* -- Computing sign from value -- */
// 1. (val == 0) -> Reduce any value to [0, 1].
// 2. If val is true, returns    1
// 3. If val is false, returns  -1
#define SIGN(val) (-2 * (val == 0) + 1)

/* -- Error check routine --
**  1. assert for debug
**      - removed if NDEBUG defined
**  2. return for release
**      - Games should keep going on error
*/
#define IES_check(cond) \
    if (!(cond)) { \
        IES_assert(0); \
        return; \
    }

#define IES_check_ret(cond, ret) \
    if (!(cond)) { \
        IES_assert(0); \
        return (ret); \
    }

#endif /* MACROS_H */
