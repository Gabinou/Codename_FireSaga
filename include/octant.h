#ifndef TERNARY_H
#define TERNARY_H

#include <math.h>
#include "structs.h"

/* --- OCTANT GEOMETRY --- */

// Around a point, you can define four quadrants
// Around an object with size, you can define eight octants like so
/*    X -> BELOW        X -> EQUAL         X -> ABOVE                   */
/*                    |             |                                   */
/*   ..._DIAGONAL_TL  |   ..._TOP   |  ..._DIAGONAL_TR      Y -> ABOVE  */
/*                    |             |                                   */
/*  ------------------o------------- ---------------------              */
/*       ..._LEFT     |   object    | SOTA_DIRECTION_RIGHT  Y -> EQUAL  */
/*  ------------------ ------------- ---------------------              */
/*                    |             |                                   */
/*   ..._DIAGONAL_BL  | ..._BOTTOM  | ..._DIAGONAL_BR       Y -> BELOW  */
/*                    |             |                                   */
// Note: actual position of all objects is in top left corner (o)


/* --- Ternary logic --- */
/* - Applications:                                                                      */
/*   1. Compute 2D direction from X/Y ternaries + simple math                           */
/*      Example:   [X = SOTA_TERNARY_BELOW, Y = SOTA_TERNARY_EQUAL]                     */
/*             2D Direction = X * 3^0 + Y * 3^1 =  1 + 0 = 1 -> SOTA_DIRECTION_LEFT     */
enum SOTA_TERNARY {
    /* Unbalanced ternary */
    SOTA_TERNARY_EQUAL  =   0,
    SOTA_TERNARY_FALSE  =   1,
    SOTA_TERNARY_BELOW  =   1,
    SOTA_TERNARY_ABOVE  =   2,
    SOTA_TERNARY_TRUE   =   2,
};

extern int Ternary_Direction(struct Point move);
extern int Ternary_Direction_Index(int x, int y);
extern int Ternary_Direction_Straight(int direction);

extern struct Point Ternary_Direction_Unbalanced(struct Point move);
extern struct Point Ternary_Moved(int ternary);
extern struct Point Ternary_Direction_Ternary(int ternary);
extern struct Point Ternary_Direction_Octant(struct Point pos, struct Point target, int w, int h);

#endif /* TERNARY_H */
