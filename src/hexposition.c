
#include "hexposition.h"

uint8_t HexDistance_HexPoints(struct HexPoint in_point1, struct HexPoint in_point2) {
    uint8_t distance = (abs(in_point1.x - in_point2.x) + abs(in_point1.y - in_point2.y) + abs(in_point1.z - in_point2.z)) / 2;
    return (distance);
}

uint8_t HexDistance_Points(struct Point in_point1, struct Point in_point2) {
    int32_t z1 = -in_point1.x - in_point1.y;
    int32_t z2 = -in_point2.x - in_point2.y;
    uint8_t distance = (abs(in_point1.x - in_point2.x) + abs(in_point1.y - in_point2.y) + abs(z1 - z2)) / 2;
    return (distance);
}

extern void plotHex2DArray(int16_t * in_array, size_t depth_len, size_t col_len, bool in_orientation) {
    printf("plotHexArray \n");
    if (in_array != NULL) {
        switch (in_orientation) {
            case POINTY_TOP:
                for (size_t z = 0; z < depth_len; z++) {
                    for (size_t z_in = 0; z_in < z; z_in++) {
                        printf(" ");
                    }
                    for (size_t x = 0; x < col_len; x++) {
                        printf("%d, ", in_array[z * col_len + x]);
                    }
                    printf("\n");
                }
                break;
            case FLAT_TOP:
                for (size_t x = 0; x < col_len; x++) {
                    for (size_t x_in = 0; x_in < x; x_in++) {
                        printf(" ");
                    }
                    for (size_t z = 0; z < depth_len; z++) {
                        printf("%d, ", in_array[z * col_len + x]);
                    }
                    printf("\n");
                }
                break;
        }
    }
}


struct HexPoint * cube_directions = NULL;
void HexPosition_makeDirections() {
    if (cube_directions != NULL) {
        arrfree(cube_directions);
    }
    arrsetlen(cube_directions, CUBE_DIRECTIONS_NUM);
    cube_directions[CUBE_DIRECTION_XP] = Cube_Direction_xp;
    cube_directions[CUBE_DIRECTION_XM] = Cube_Direction_xm;
    cube_directions[CUBE_DIRECTION_YP] = Cube_Direction_yp;
    cube_directions[CUBE_DIRECTION_YM] = Cube_Direction_ym;
    cube_directions[CUBE_DIRECTION_ZP] = Cube_Direction_zp;
    cube_directions[CUBE_DIRECTION_ZM] = Cube_Direction_zm;
}

void HexPosition_Move(struct HexPosition * in_hexpos, uint8_t in_direction, uint8_t in_magnitude) {
    struct HexPoint direction = cube_directions[in_direction];
    in_hexpos->pos.x += in_magnitude * direction.x;
    in_hexpos->pos.y += in_magnitude * direction.y;
    in_hexpos->pos.z += in_magnitude * direction.z;
}

struct Point Hex2Pixel(struct HexPoint in_hexpoint, int32_t in_radius, bool in_orientation) {
    struct Point out;
    float sqrt3 = carmack_sqrt_int32_t(3);
    if (in_orientation == FLAT_TOP) {
        out.x = in_radius * (3.0f / 2.0f * in_hexpoint.x);
        out.y = in_radius * (sqrt3 / 2.0f * in_hexpoint.x + sqrt3 * in_hexpoint.z);
    } else {
        out.x = in_radius * (sqrt3 * in_hexpoint.x + sqrt3 / 2.0f * in_hexpoint.z);
        out.y = in_radius * (3.0f / 2.0f * in_hexpoint.z);
    }
    return (out);
}

struct HexPoint HexRound(struct Point3Df in_point) {
    struct HexPoint out;
    struct HexPoint diff;

    out.x = lroundf(in_point.x);
    out.y = lroundf(in_point.y);
    out.z = lroundf(in_point.z);

    diff.x = fabs(out.x - in_point.x);
    diff.y = fabs(out.y - in_point.y);
    diff.z = fabs(out.z - in_point.z);

    if ((diff.x > diff.y) && (diff.x > diff.z)) {
        out.x = -out.y - out.z;
    } else if (diff.y > diff.z) {
        out.y = -out.x - out.z;
    } else {
        out.z = -out.x - out.y;
    }

    return (out);
}

struct Point3D Pixel2Hex(struct Point in_point, int32_t in_radius, bool in_orientation) {
    struct Point3Df temp;
    float sqrt3 = carmack_sqrt_int32_t(3);
    if (in_orientation == FLAT_TOP) {
        temp.x = (2.0f / 3.0f * in_point.x) / in_radius;
        temp.z = (-1.0f / 3.0f * in_point.x + sqrt3 / 3.0f * in_point.y) / in_radius;
    } else {
        temp.x = (sqrt3 / 3.0f * in_point.x - 1.0f / 3.0f * in_point.y) / in_radius;
        temp.z = (2.0f / 3.0f * in_point.y) / in_radius;
    }
    temp.y = - temp.x - temp.z;
    struct HexPoint out = HexRound(temp);
    return (out);
}


