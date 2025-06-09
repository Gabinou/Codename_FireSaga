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

    /* Registering */
#include "register/components.h"
#include "register/pipelines.h"
#include "register/phases.h"
#include "register/systems.h"

    /* Create items */
    tnecs_entity fleuret = TNECS_ENTITY_CREATE_wCOMPONENTS(world,
                                                           Inventory_item_ID,
                                                           Cooldown_ID,
                                                           Alignment_Friendly_ID);
    Cooldown *cooldown  = IES_GET_COMPONENT(IESworld, fleuret, Cooldown);
    Cooldown_Set(cooldown, 2);
    Cooldown_Start(cooldown);
    nourstest(cooldown->left == cooldown->ticks);

    tnecs_pipeline_step(world, 0, NULL, TNECS_PIPELINE_MAP);
    nourstest(cooldown->ticks == (cooldown->left - 1));

    /* Free */
    tnecs_world_destroy(&world);
    gl_world = NULL;
}