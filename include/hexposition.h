#ifndef HEXPOSITION_H
#define HEXPOSITION_H

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "narrative.h"
#include "q_math.h"
#include "stb_ds.h"

// hexmap are stored as int16_t * map[col_len*depth_len]
// map[z*col_len + x] with y = - x - z

enum HEXPOSITION_CONSTANTS {
    CUBE_DIRECTIONS_NUM = 6,
};

enum CUBE_DIRECTIONS {
    CUBE_DIRECTION_XP = 0,
    CUBE_DIRECTION_XM = 1,
    CUBE_DIRECTION_YP = 2,
    CUBE_DIRECTION_YM = 3,
    CUBE_DIRECTION_ZP = 4,
    CUBE_DIRECTION_ZM = 5,
};

typedef struct HexPosition {
    struct HexPoint pos;
    bool orientation;
    int32_t radius; // e.g. size of hexagon/side length.
} HexPosition;
extern struct HexPosition HexPosition_default;

enum ORIENTATIONS {
    FLAT_TOP = 0,
    POINTY_TOP = 1,
};

extern void HexPosition_Move(struct HexPosition * in_hexpos, uint8_t direction, uint8_t magnitude);
extern uint8_t HexDistance_HexPoints(struct HexPoint in_point1, struct HexPoint in_point2);
extern uint8_t HexDistance_Points(struct Point in_point1, struct Point in_point2);

extern struct HexPoint HexRound(struct HexPointf in_point);

extern struct Point Hex2Pixel(struct HexPoint in_hexpoint, int32_t in_radius, bool in_orientation);
extern struct Point3D Pixel2Hex(struct Point in_point, int32_t in_radius, bool in_orientation);
extern void plotHex2DArray(int16_t * in_array, size_t x_len, size_t z_len, bool in_orientation);


#endif /* HexPOSITION_H */
