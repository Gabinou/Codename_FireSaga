
#ifndef GLOBALS_H
#define GLOBALS_H

#include "nmath.h"
#include "tnecs.h"

/* Note: Modify dtabs only when loading from .json */
extern struct dtab *gl_items_dtab;
extern struct dtab *gl_weapons_dtab;

/* gl_world: */
extern tnecs_world *gl_world;
// Pros:
//  - No more copies of the world in the map, game, units, etc.
// Cons:
//  - Harder to debug, extend

#endif /* GLOBALS_H */
