/*
**  Copyright 2023 Gabriel Taillon
**  Licensed under the GPLv3
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
**  Testing cooldown component
**
*/

#include "game/game.h"

void test_cooldown(int argc, char *argv[]) {
    /* -- Startup -- */
    tnecs_world *world = NULL;
    tnecs_world_genesis(&world);
    gl_world = world;

#include "register_components.h"



    tnecs_entity Silou  = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Unit_ID, Position_ID);


    /* Free */
    tnecs_world_destroy(&world);
    gl_world = NULL;
}