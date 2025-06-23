/*
**  Copyright 2023 Gabriel Taillon
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
    tnecs_entity friendly_fleuret = TNECS_ENTITY_CREATE_wCOMPONENTS(world,
                                    Inventory_item_ID,
                                    Cooldown_ID,
                                    Alignment_Friendly_ID);
    tnecs_entity neutral_fleuret = TNECS_ENTITY_CREATE_wCOMPONENTS(world,
                                   Inventory_item_ID,
                                   Cooldown_ID,
                                   Alignment_Neutral_ID);
    tnecs_entity enemy_fleuret = TNECS_ENTITY_CREATE_wCOMPONENTS(world,
                                 Inventory_item_ID,
                                 Cooldown_ID,
                                 Alignment_Enemy_ID);

    Cooldown *friendly_cooldown = IES_GET_COMPONENT(world, friendly_fleuret, Cooldown);
    Cooldown *neutral_cooldown  = IES_GET_COMPONENT(world, neutral_fleuret,    Cooldown);
    Cooldown *enemy_cooldown    = IES_GET_COMPONENT(world, enemy_fleuret,  Cooldown);
    Cooldown_Set(friendly_cooldown, 2);
    Cooldown_Set(neutral_cooldown,  2);
    Cooldown_Set(enemy_cooldown,    2);
    Cooldown_Start(friendly_cooldown);
    Cooldown_Start(neutral_cooldown);
    Cooldown_Start(enemy_cooldown);
    nourstest_true(friendly_cooldown->left  == friendly_cooldown->ticks);
    nourstest_true(neutral_cooldown->left   == neutral_cooldown->ticks);
    nourstest_true(enemy_cooldown->left     == enemy_cooldown->ticks);
    nourstest_true(isOnCooldown(friendly_cooldown));
    nourstest_true(isOnCooldown(neutral_cooldown));
    nourstest_true(isOnCooldown(enemy_cooldown));

    /* Stepping another phase: no tick*/
    tnecs_pipeline_step(world, 0, NULL, TNECS_PIPELINE_MAP_END);
    nourstest_true(friendly_cooldown->left  == friendly_cooldown->ticks);
    nourstest_true(neutral_cooldown->left   == neutral_cooldown->ticks);
    nourstest_true(enemy_cooldown->left     == enemy_cooldown->ticks);
    nourstest_true(isOnCooldown(friendly_cooldown));
    nourstest_true(isOnCooldown(neutral_cooldown));
    nourstest_true(isOnCooldown(enemy_cooldown));

    /* Stepping correct phase: tick once */
    tnecs_pipeline_step_phase(world, 0, NULL, TNECS_PIPELINE_TURN_END, TNECS_TURN_END_PHASE_FRIENDLY);
    nourstest_true(friendly_cooldown->left  == (friendly_cooldown->ticks - 1));
    nourstest_true(neutral_cooldown->left   == neutral_cooldown->ticks);
    nourstest_true(enemy_cooldown->left     == enemy_cooldown->ticks);
    nourstest_true(isOnCooldown(friendly_cooldown));
    nourstest_true(isOnCooldown(neutral_cooldown));
    nourstest_true(isOnCooldown(enemy_cooldown));

    /* Stepping correct phase: tick twice */
    tnecs_pipeline_step_phase(world, 0, NULL, TNECS_PIPELINE_TURN_END, TNECS_TURN_END_PHASE_FRIENDLY);
    nourstest_true(friendly_cooldown->left  == (friendly_cooldown->ticks - 2));
    nourstest_true(neutral_cooldown->left   == neutral_cooldown->ticks);
    nourstest_true(enemy_cooldown->left     == enemy_cooldown->ticks);
    nourstest_true(!isOnCooldown(friendly_cooldown));
    nourstest_true(isOnCooldown(neutral_cooldown));
    nourstest_true(isOnCooldown(enemy_cooldown));

    /* Change to neutral phase */
    // Cooldown_Start(friendly_cooldown);
    // Cooldown_Start(neutral_cooldown);
    // Cooldown_Start(enemy_cooldown);

    /* Stepping correct phase: tick once */
    tnecs_pipeline_step_phase(world, 0, NULL, TNECS_PIPELINE_TURN_END, TNECS_TURN_END_PHASE_NEUTRAL);
    nourstest_true(friendly_cooldown->left  == (friendly_cooldown->ticks - 2));
    nourstest_true(neutral_cooldown->left   == (neutral_cooldown->ticks - 1));
    nourstest_true(enemy_cooldown->left     == enemy_cooldown->ticks);
    nourstest_true(!isOnCooldown(friendly_cooldown));
    nourstest_true(isOnCooldown(neutral_cooldown));
    nourstest_true(isOnCooldown(enemy_cooldown));

    /* Stepping correct phase: tick twice */
    tnecs_pipeline_step_phase(world, 0, NULL, TNECS_PIPELINE_TURN_END, TNECS_TURN_END_PHASE_NEUTRAL);
    nourstest_true(friendly_cooldown->left  == (friendly_cooldown->ticks - 2));
    nourstest_true(neutral_cooldown->left   == (neutral_cooldown->ticks - 2));
    nourstest_true(enemy_cooldown->left     == enemy_cooldown->ticks);
    nourstest_true(!isOnCooldown(friendly_cooldown));
    nourstest_true(!isOnCooldown(neutral_cooldown));
    nourstest_true(isOnCooldown(enemy_cooldown));

    /* Stepping correct phase: tick once */
    tnecs_pipeline_step_phase(world, 0, NULL, TNECS_PIPELINE_TURN_END, TNECS_TURN_END_PHASE_ENEMY);
    nourstest_true(friendly_cooldown->left  == (friendly_cooldown->ticks - 2));
    nourstest_true(neutral_cooldown->left   == (neutral_cooldown->ticks - 2));
    nourstest_true(enemy_cooldown->left     == (enemy_cooldown->ticks - 1));
    nourstest_true(!isOnCooldown(friendly_cooldown));
    nourstest_true(!isOnCooldown(neutral_cooldown));
    nourstest_true(isOnCooldown(enemy_cooldown));

    /* Stepping correct phase: tick twice */
    tnecs_pipeline_step_phase(world, 0, NULL, TNECS_PIPELINE_TURN_END, TNECS_TURN_END_PHASE_ENEMY);
    nourstest_true(friendly_cooldown->left  == (friendly_cooldown->ticks - 2));
    nourstest_true(neutral_cooldown->left   == (neutral_cooldown->ticks - 2));
    nourstest_true(enemy_cooldown->left     == (enemy_cooldown->ticks - 2));
    nourstest_true(!isOnCooldown(friendly_cooldown));
    nourstest_true(!isOnCooldown(neutral_cooldown));
    nourstest_true(!isOnCooldown(enemy_cooldown));

    /* Free */
    tnecs_world_destroy(&world);
    gl_world = NULL;
}