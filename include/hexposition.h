#ifndef HEXPOSITION_H
#define HEXPOSITION_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "types.h"
#include "nmath.h"
#include "structs.h"

// hexmap are stored as i16 * map[col_len*depth_len]
// map[z*col_len + x] with y = - x - z

enum HEXPOSITION_ANTS {
    CUBE_DIRECTIONS_NUM = 6,
};

enum CUBE_DIRECTIONS {
    CUBE_NMATH_DIRECTION_XP = 0,
    CUBE_NMATH_DIRECTION_XM = 1,
    CUBE_NMATH_DIRECTION_YP = 2,
    CUBE_NMATH_DIRECTION_YM = 3,
    CUBE_NMATH_DIRECTION_ZP = 4,
    CUBE_NMATH_DIRECTION_ZM = 5,
};

typedef struct HexPosition {
    struct nmath_hexpoint_int32_t pos;
    b32 orientation;
    i32 radius; // e.g. size of hexagon/side length.
} HexPosition;
extern const struct HexPosition HexPosition_default;

enum ORIENTATIONS {
    FLAT_TOP = 0,
    POINTY_TOP = 1,
};

u8 HexDistance_Points(   struct Point p1, struct Point p2);
u8 HexDistance_HexPoints(struct nmath_hexpoint_int32_t p1,
                         struct nmath_hexpoint_int32_t p2);

void HexPosition_Move(struct HexPosition *in_hexpos, u8 direction, u8 magnitude);

struct nmath_hexpoint_int32_t HexRound(struct nmath_hexpoint_float p);

struct Point Hex2Pixel(struct nmath_hexpoint_int32_t hp, i32 r, b32 o);
struct nmath_hexpoint_int32_t Pixel2Hex(struct Point p,  i32 r, b32 o);
void plotHex2DArray(i16 *in_array, size_t x_len, size_t z_len, int o);

#endif /* HexPOSITION_H */
