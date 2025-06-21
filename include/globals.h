
#ifndef GLOBALS_H
#define GLOBALS_H

#include "nmath.h"
#include "tnecs.h"

/* Items/Weapons dtabs
**  - For stats not unique to item/weapon instance
**      - Basically only *used* is unique
**  - Modify dtabs only when loading from .json 
**  - Otherwise: *always use DTAB_GET_CONST*        */
extern struct dtab *gl_items_dtab;
extern struct dtab *gl_weapons_dtab;

/* Only instance of world
**  - Global is better because:
**      - Shared to EVERYTHING, including components
**      - API to pass, copy pointer everywhere
**          - Super dumb, annoying                      
**      - Not harder to debug                       */  
extern tnecs_world *gl_world;

#endif /* GLOBALS_H */
