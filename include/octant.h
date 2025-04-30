#ifndef TERNARY_H
#define TERNARY_H

#include <math.h>
#include "structs.h"

/* --- OCTANT GEOMETRY --- */

//                                                          TOPRIGHT | TOPLEFT
// Around a point, you can define four quadrants like so:  ----------|----------
//                                                          BOTRIGHT | BOTLEFT

// Around an object with size, you can define eight octants like so:
/*    X -> BELOW        X -> EQUAL         X -> ABOVE                   */
/*                    |             |                                   */
/*     ..._TOPLEFT    |   ..._TOP   |    ..._TOPRIGHT       Y -> ABOVE  */
/*                    |             |                                   */
/*  ------------------o------------- ---------------------              */
/*       ..._LEFT     |   object    |     ..._RIGHT         Y -> EQUAL  */
/*  ------------------ ------------- ---------------------              */
/*                    |             |                                   */
/*     ..._BOTLEFT    | ..._BOTTOM  |    ..._BOTRIGHT       Y -> BELOW  */
/*                    |             |                                   */
// Legend:
//      - ...   -> 'SOTA_DIRECTION'
//      - o     ->     origin

/* --- Ternary logic --- */
/*  Compute 2D direction from X/Y direction */
/*      Example:   [X = 0, Y = 1]           */
/*             2D Direction = X * 3^0 + Y * 3^1 =  1 + 0 = 1 -> SOTA_DIRECTION_LEFT     */
/*  Note: Ternary functions check if X/Y are below, above, or equal    */
int Ternary_Direction(struct Point move);
int Ternary_Direction_Index(int x, int y);
int Ternary_Direction_Straight(int direction);

struct Point Ternary_Direction_Unbalanced(struct Point move);
struct Point Ternary_Moved(int ternary);
struct Point Ternary_Direction_Ternary(int ternary);
struct Point Ternary_Direction_Octant(struct Point p, struct Point t, int w, int h);

#endif /* TERNARY_H */
