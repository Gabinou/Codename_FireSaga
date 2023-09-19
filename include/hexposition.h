#ifndef HEXPOSITION_H
#define HEXPOSITION_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "macros.h"
#include "types.h"
#include "debug.h"
#include "structs.h"
#include "globals.h"

// hexmap are stored as int_fast16_t * map[col_len*depth_len]
// map[z*col_len + x] with y = - x - z

enum HEXPOSITION_CONSTANTS {
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
    bool orientation;
    if32 radius; // e.g. size of hexagon/side length.
} HexPosition;
extern struct HexPosition HexPosition_default;

enum ORIENTATIONS {
    FLAT_TOP = 0,
    POINTY_TOP = 1,
};

extern void HexPosition_Move(struct HexPosition *in_hexpos, uf8 direction, uf8 magnitude);
extern uf8 HexDistance_HexPoints(struct nmath_hexpoint_int32_t p1,
                                 struct nmath_hexpoint_int32_t p2);
extern uf8 HexDistance_Points(struct Point p1, struct Point p2);

extern struct nmath_hexpoint_int32_t HexRound(struct nmath_hexpoint_float p);

extern struct Point Hex2Pixel(struct nmath_hexpoint_int32_t hp, if32 r, bool o);
extern struct nmath_hexpoint_int32_t Pixel2Hex(struct Point p,  if32 r, bool o);
extern void plotHex2DArray(int_fast16_t *in_array, size_t x_len, size_t z_len, int o);

#endif /* HexPOSITION_H */
