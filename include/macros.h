#ifndef MACROS_H
#define MACROS_H

/* --- Macros --- */
/* Mostly for reference */

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

#define SOTA_CONCATENATE( arg1, arg2) SOTA_CONCATENATE1(arg1, arg2)
#define SOTA_CONCATENATE1(arg1, arg2) SOTA_CONCATENATE2(arg1, arg2)
#define SOTA_CONCATENATE2(arg1, arg2) arg1##arg2

/* --- Flattened array indices --- */
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
#define SOTA_PIXEL2TILEMAP(pos, tilesize, offset, zoom) (floor((pos - offset) / zoom / tilesize))
#define SOTA_TILEMAP2PIXEL(pos, tilesize, offset, zoom) (pos * zoom * tilesize) + offset

/* -- Linear transformations -- */

#define SOTA_ZOOM(  in, zoom) (in * zoom)
#define SOTA_DEZOOM(in, zoom) (in / zoom)
#define SOTA_ZOOMTOPOINT(scale, x, origin) (scale * (x - origin) + origin)

#endif /* MACROS_H */
