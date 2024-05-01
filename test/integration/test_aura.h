#include "game/game.h"
#include "map/ontile.h"

void test_aura(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Game *sota = SDL_malloc(sizeof(struct Game));
    *sota = Game_default;
    sota->settings = Settings_default;
    sota->settings.window = false;
    Game_Init(sota, argc, argv);
    nourstest_true(sota->state      == GAME_STATE_Title_Screen);
    nourstest_true(sota->substate   == GAME_SUBSTATE_MENU);

    /* Load Save file test/debug_map.json */
    // Game_Map_Load(sota, CHAPTER_TEST_V8);
    Game_debugMap_Load(sota);
    Game_Map_Reinforcements_Load(sota);
    SDL_assert(DARR_NUM(sota->map->units_onfield) > 0);

    /* -- Place all friendlies close together -- */
    /* Place Friendly 1 inside */
    struct Point pos = {1, 1};
    Game_Party_Entity_Create(sota, UNIT_ID_SILOU, pos);
    tnecs_entity killer_entity = sota->units_loaded[UNIT_ID_SILOU];
    Map_Unit_Put(sota->map, pos.x, pos.y, killer_entity);
    SDL_assert(sota->units_loaded[UNIT_ID_SILOU] > TNECS_NULL);
    SDL_assert(killer_entity > TNECS_NULL);
    SDL_assert(boss_entity != killer_entity);
    /* Place Friendly 2 inside */

    /* Place Friendly 3 outside */

    
    /* Check Aura in bonus stack */
    // nourstest_true(sota->map->win);
    // nourstest_true(!sota->map->loss);

    /* Check effective stats */

    /* Quit game */
    Game_Free(sota);
    nourstest_true(true);
}