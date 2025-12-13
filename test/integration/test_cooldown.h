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
    Names_Load_All();
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    Settings settings   = Settings_default;
    settings.window     = SDL_WINDOW_HIDDEN;
    Game *IES = Game_New(settings);
    nourstest_true(Game_State_Current(IES)      == GAME_STATE_Title_Screen);
    nourstest_true(Game_Substate_Current(IES)   == GAME_SUBSTATE_MENU);

    /* Load Save file test/0001.json */
    Game_Save_Load(IES, SOTA_SAVE_DEBUG_1);
    Game_Map_Reinforcements_Load(IES);
    Map *map = Game_Map(IES);
    SDL_assert(DARR_NUM(map->units.onfield.arr) > 0);

    /* Create items */
    tnecs_E friendly_fleuret = IES_E_CREATE_wC(gl_world,
                                               InvItem_ID,
                                               Cooldown_ID);
    tnecs_E neutral_fleuret = IES_E_CREATE_wC(gl_world,
                                              InvItem_ID,
                                              Cooldown_ID);
    tnecs_E enemy_fleuret = IES_E_CREATE_wC(gl_world,
                                            InvItem_ID,
                                            Cooldown_ID);

    Cooldown *friendly_cooldown = IES_GET_C(gl_world, friendly_fleuret, Cooldown);
    Cooldown *neutral_cooldown  = IES_GET_C(gl_world, neutral_fleuret,    Cooldown);
    Cooldown *enemy_cooldown    = IES_GET_C(gl_world, enemy_fleuret,  Cooldown);
    InvItem *friendly_item = IES_GET_C(gl_world, friendly_fleuret, InvItem);
    InvItem *neutral_item  = IES_GET_C(gl_world, neutral_fleuret,    InvItem);
    InvItem *enemy_item    = IES_GET_C(gl_world, enemy_fleuret,  InvItem);
    friendly_item->army = ARMY_FRIENDLY;
    enemy_item->army    = ARMY_ENEMY;
    neutral_item->army  = ARMY_NEUTRAL;
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
    tnecs_step_Pi(gl_world, 0, IES, TNECS_PIPELINE_MAP_END);
    nourstest_true(friendly_cooldown->left  == friendly_cooldown->ticks);
    nourstest_true(neutral_cooldown->left   == neutral_cooldown->ticks);
    nourstest_true(enemy_cooldown->left     == enemy_cooldown->ticks);
    nourstest_true(isOnCooldown(friendly_cooldown));
    nourstest_true(isOnCooldown(neutral_cooldown));
    nourstest_true(isOnCooldown(enemy_cooldown));

    /* Stepping correct phase: tick once */
    IES->turn_end.army  = ARMY_FRIENDLY;
    tnecs_step_Pi(gl_world, 0, IES, TNECS_PIPELINE_TURN_END);
    nourstest_true(friendly_cooldown->left  == (friendly_cooldown->ticks - 1));
    nourstest_true(neutral_cooldown->left   == neutral_cooldown->ticks);
    nourstest_true(enemy_cooldown->left     == enemy_cooldown->ticks);
    nourstest_true(isOnCooldown(friendly_cooldown));
    nourstest_true(isOnCooldown(neutral_cooldown));
    nourstest_true(isOnCooldown(enemy_cooldown));

    IES->turn_end.army  = ARMY_NEUTRAL;
    tnecs_step_Pi(gl_world, 0, IES, TNECS_PIPELINE_TURN_END);
    nourstest_true(friendly_cooldown->left  == (friendly_cooldown->ticks - 1));
    nourstest_true(neutral_cooldown->left   == neutral_cooldown->ticks - 1);
    nourstest_true(enemy_cooldown->left     == enemy_cooldown->ticks);
    nourstest_true(isOnCooldown(friendly_cooldown));
    nourstest_true(isOnCooldown(neutral_cooldown));
    nourstest_true(isOnCooldown(enemy_cooldown));

    IES->turn_end.army  = ARMY_ENEMY;
    tnecs_step_Pi(gl_world, 0, IES, TNECS_PIPELINE_TURN_END);
    nourstest_true(friendly_cooldown->left  == (friendly_cooldown->ticks - 1));
    nourstest_true(neutral_cooldown->left   == neutral_cooldown->ticks - 1);
    nourstest_true(enemy_cooldown->left     == enemy_cooldown->ticks - 1);
    nourstest_true(isOnCooldown(friendly_cooldown));
    nourstest_true(isOnCooldown(neutral_cooldown));
    nourstest_true(isOnCooldown(enemy_cooldown));

    IES->turn_end.army  = ARMY_FRIENDLY;
    tnecs_step_Pi(gl_world, 0, IES, TNECS_PIPELINE_TURN_END);
    nourstest_true(friendly_cooldown->left  == (friendly_cooldown->ticks - 2));
    nourstest_true(neutral_cooldown->left   == neutral_cooldown->ticks - 1);
    nourstest_true(enemy_cooldown->left     == enemy_cooldown->ticks - 1);
    nourstest_true(!isOnCooldown(friendly_cooldown));
    nourstest_true(isOnCooldown(neutral_cooldown));
    nourstest_true(isOnCooldown(enemy_cooldown));

    IES->turn_end.army  = ARMY_NEUTRAL;
    tnecs_step_Pi(gl_world, 0, IES, TNECS_PIPELINE_TURN_END);
    nourstest_true(friendly_cooldown->left  == (friendly_cooldown->ticks - 2));
    nourstest_true(neutral_cooldown->left   == neutral_cooldown->ticks - 2);
    nourstest_true(enemy_cooldown->left     == enemy_cooldown->ticks - 1);
    nourstest_true(!isOnCooldown(friendly_cooldown));
    nourstest_true(!isOnCooldown(neutral_cooldown));
    nourstest_true(isOnCooldown(enemy_cooldown));

    IES->turn_end.army  = ARMY_ENEMY;
    tnecs_step_Pi(gl_world, 0, IES, TNECS_PIPELINE_TURN_END);
    nourstest_true(friendly_cooldown->left  == (friendly_cooldown->ticks - 2));
    nourstest_true(neutral_cooldown->left   == neutral_cooldown->ticks - 2);
    nourstest_true(enemy_cooldown->left     == enemy_cooldown->ticks - 2);
    nourstest_true(!isOnCooldown(friendly_cooldown));
    nourstest_true(!isOnCooldown(neutral_cooldown));
    nourstest_true(!isOnCooldown(enemy_cooldown));

    /* Free */
    Game_Free(IES);
    Names_Free();
}