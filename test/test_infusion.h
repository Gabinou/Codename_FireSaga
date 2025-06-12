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
**  Testing infusion component
**
*/
#include "game/game.h"
#include "structs.h"

void test_infusion() {
    /* -- Startup -- */
    tnecs_world *world = NULL;
    tnecs_world_genesis(&world);
    gl_world = world;

    /* -- Registering -- */
#include "register/components.h"
#include "register/pipelines.h"
#include "register/phases.h"
#include "register/systems.h"

    /* -- Creating weapon -- */
    /* -- Infusing weapon -- */
    /* -- Computing stats weapon -- */
    /* -- Free -- */
    tnecs_world_destroy(&world);
    gl_world = NULL;
}