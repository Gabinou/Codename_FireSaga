
#ifndef GLOBALS_H
#define GLOBALS_H
/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** Globals:
**  - Variables used too much to pass to functions
**
*/

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
**      - Pass to every function: super dumb, annoying
**      - Not harder to debug                       */
extern tnecs_W *gl_world;

#endif /* GLOBALS_H */
