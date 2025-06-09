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
#include "structs.h"
#include "systems/control.h"
#include "systems/turn_end.h"

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
    Cooldown *cooldown  = IES_GET_COMPONENT(world, fleuret, Cooldown);
    Cooldown_Set(cooldown, 2);
    Cooldown_Start(cooldown);
    nourstest_true(cooldown->left == cooldown->ticks);
    nourstest_true(isOnCooldown(cooldown));

    /* Stepping another phase: no tick*/
    tnecs_pipeline_step(world, 0, NULL, TNECS_PIPELINE_MAP);
    nourstest_true(cooldown->left == cooldown->ticks);
    nourstest_true(isOnCooldown(cooldown));

    /* Stepping correct phase: tick once */
    tnecs_pipeline_step(world, 0, NULL, TNECS_PIPELINE_TURN_END);
    nourstest_true(cooldown->left  == (cooldown->ticks - 1));
    nourstest_true(isOnCooldown(cooldown));

    /* Stepping correct phase: tick twice */
    tnecs_pipeline_step(world, 0, NULL, TNECS_PIPELINE_TURN_END);
    nourstest_true(cooldown->left  == (cooldown->ticks - 2));
    nourstest_true(!isOnCooldown(cooldown));

    /* Free */
    tnecs_world_destroy(&world);
    gl_world = NULL;
}