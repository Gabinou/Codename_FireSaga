
#include "octant.h"


/* --- Ternary --- */
/* Return unique index from two ternary values. Balanced or not. */
int Ternary_Direction_Index(int x, int y) {
    return (x + 3 * y);
}

struct Point Ternary_Direction_Unbalanced(const struct Point move) {
    struct Point point;
    point.x = SOTA_TERNARY_ABOVE * (move.x > 0) + SOTA_TERNARY_BELOW * (move.x < 0);
    point.y = SOTA_TERNARY_ABOVE * (move.y < 0) + SOTA_TERNARY_BELOW * (move.y > 0);
    return (point);
}

struct Point Ternary_Direction_Ternary(int ternary) {
    struct Point point;
    point.x = ternary % 3;
    point.y = (ternary - point.x) / 3;
    return (point);
}

struct Point Ternary_Moved(int ternary) {
    struct Point point = Ternary_Direction_Ternary(ternary);
    /* Convert to balanced, better for movement */
    point.x =  1 * (point.x == SOTA_TERNARY_ABOVE) - 1 * (point.x == SOTA_TERNARY_BELOW);
    /* Invert Y to make it intuitive e.g. to screen top */
    point.y =  - 1 * (point.y == SOTA_TERNARY_ABOVE) + 1 * (point.y == SOTA_TERNARY_BELOW);
    return (point);
}

struct Point Ternary_Direction_Octant(struct Point pos, struct Point target, int width,
                                      int height) {
    struct Point out;
    int max = (pos.y + height);
    /* Invert y Above/Below to become more intuitive */
    out.y = SOTA_TERNARY_ABOVE * (target.y < pos.y) + SOTA_TERNARY_BELOW * (target.y > max);
    max   = (pos.x + width);
    out.x = SOTA_TERNARY_BELOW * (target.x < pos.x) + SOTA_TERNARY_ABOVE * (target.x > max);
    return (out);
}

int Ternary_Direction_Straight(int direction) {
    /* Collapse diagonals to straight direction */
    int out = direction;
    SDL_assert(direction > SOTA_DIRECTION_NULL);
    SDL_assert(direction < SOTA_DIRECTIONS_NUM + 1); // cause ternary starts at 1
    switch (direction) {
        case SOTA_DIRECTION_DIAGONAL_TR:
            out = SOTA_DIRECTION_TOP;
            break;
        case SOTA_DIRECTION_DIAGONAL_TL:
            out = SOTA_DIRECTION_TOP;
            break;
        case SOTA_DIRECTION_DIAGONAL_BR:
            out = SOTA_DIRECTION_BOTTOM;
            break;
        case SOTA_DIRECTION_DIAGONAL_BL:
            out = SOTA_DIRECTION_BOTTOM;
            break;
    }
    return (out);
}

int Ternary_Direction(struct Point move) {
    struct Point point = Ternary_Direction_Unbalanced(move);
    return (Ternary_Direction_Index(point.x, point.y));
}
