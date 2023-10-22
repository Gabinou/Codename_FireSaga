
#include "hexposition.h"

u8 HexDistance_HexPoints(struct nmath_hexpoint_int32_t in_point1,
                         struct nmath_hexpoint_int32_t in_point2) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u8 distance = (labs(in_point1.x - in_point2.x) + abs(in_point1.y - in_point2.y) +
                   labs(
                           in_point1.z - in_point2.z)) / 2;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (distance);
}

u8 HexDistance_Points(struct Point in_point1,
                      struct Point in_point2) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if32 z1 = -in_point1.x - in_point1.y;
    if32 z2 = -in_point2.x - in_point2.y;
    u8 distance = (labs(in_point1.x - in_point2.x) + abs(in_point1.y - in_point2.y) +
                   labs(
                           z1 - z2)) / 2;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (distance);
}

extern void plotHex2DArray(int_fast16_t *in_array, size_t depth_len, size_t col_len,
                           int in_orientation) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (in_array != NULL) {
        switch (in_orientation) {
            case POINTY_TOP:
                for (size_t z = 0; z < depth_len; z++) {
                    for (size_t z_in = 0; z_in < z; z_in++)
                        printf(" ");
                    for (size_t x = 0; x < col_len; x++)
                        printf("%ld, ", in_array[z * col_len + x]);
                    printf("\n");
                }
                break;
            case FLAT_TOP:
                for (size_t x = 0; x < col_len; x++) {
                    for (size_t x_in = 0; x_in < x; x_in++)
                        printf(" ");
                    for (size_t z = 0; z < depth_len; z++)
                        printf("%ld, ", in_array[z * col_len + x]);
                    printf("\n");
                }
                break;
        }
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

struct nmath_hexpoint_int32_t *cube_directions = NULL;
void HexPosition_makeDirections() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (cube_directions != NULL)
        DARR_FREE(cube_directions);
    DARR_INIT(cube_directions, struct nmath_hexpoint_int32_t, CUBE_DIRECTIONS_NUM);
    cube_directions[CUBE_NMATH_DIRECTION_XP] = Cube_Direction_xp;
    cube_directions[CUBE_NMATH_DIRECTION_XM] = Cube_Direction_xm;
    cube_directions[CUBE_NMATH_DIRECTION_YP] = Cube_Direction_yp;
    cube_directions[CUBE_NMATH_DIRECTION_YM] = Cube_Direction_ym;
    cube_directions[CUBE_NMATH_DIRECTION_ZP] = Cube_Direction_zp;
    cube_directions[CUBE_NMATH_DIRECTION_ZM] = Cube_Direction_zm;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void HexPosition_Move(struct HexPosition *in_hexpos, u8 in_direction,
                      u8 in_magnitude) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct nmath_hexpoint_int32_t direction = cube_directions[in_direction];
    in_hexpos->pos.x += in_magnitude * direction.x;
    in_hexpos->pos.y += in_magnitude * direction.y;
    in_hexpos->pos.z += in_magnitude * direction.z;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

struct Point Hex2Pixel(struct nmath_hexpoint_int32_t in_hexpoint,
                       if32 in_radius, bool in_orientation) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Point out;
    float sqrt3 = carmack_sqrt_int32_t(3);
    if (in_orientation == FLAT_TOP) {
        out.x = in_radius * (3.0f / 2.0f * in_hexpoint.x);
        out.y = in_radius * (sqrt3 / 2.0f * in_hexpoint.x + sqrt3 * in_hexpoint.z);
    } else {
        out.x = in_radius * (sqrt3 * in_hexpoint.x + sqrt3 / 2.0f * in_hexpoint.z);
        out.y = in_radius * (3.0f / 2.0f * in_hexpoint.z);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

struct nmath_hexpoint_int32_t HexRound(struct nmath_hexpoint_float in_point) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct nmath_hexpoint_int32_t out;
    struct nmath_hexpoint_int32_t diff;
    out.x = lroundf(in_point.x);
    out.y = lroundf(in_point.y);
    out.z = lroundf(in_point.z);
    diff.x = fabs(out.x - in_point.x);
    diff.y = fabs(out.y - in_point.y);
    diff.z = fabs(out.z - in_point.z);
    if ((diff.x > diff.y) && (diff.x > diff.z))
        out.x = -out.y - out.z;
    else if (diff.y > diff.z)
        out.y = -out.x - out.z;
    else
        out.z = -out.x - out.y;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

struct nmath_hexpoint_int32_t Pixel2Hex(struct Point in_point,
                                        if32 in_radius,
                                        bool in_orientation) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct nmath_hexpoint_float temp;
    float sqrt3 = carmack_sqrt_int32_t(3);
    if (in_orientation == FLAT_TOP) {
        temp.x = (2.0f / 3.0f * in_point.x) / in_radius;
        temp.z = (-1.0f / 3.0f * in_point.x + sqrt3 / 3.0f * in_point.y) / in_radius;
    } else {
        temp.x = (sqrt3 / 3.0f * in_point.x - 1.0f / 3.0f * in_point.y) / in_radius;
        temp.z = (2.0f / 3.0f * in_point.y) / in_radius;
    }
    temp.y = - temp.x - temp.z;
    struct nmath_hexpoint_int32_t out = HexRound(temp);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}
