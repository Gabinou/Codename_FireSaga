#include "game/game.h"
#include "map/ontile.h"

void test_boss_death_win(int argc, char *argv[]) {
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

    /* Get boss */
    tnecs_entity boss_entity = Map_Unit_Get_Boss(sota->map, ARMY_ENEMY);
    SDL_assert(boss_entity > TNECS_NULL);

    /* Get killer */
    struct Point pos = {1, 1};
    Game_Party_Entity_Create(sota, UNIT_ID_SILOU, pos);
    tnecs_entity killer_entity = sota->units_loaded[UNIT_ID_SILOU];
    SDL_assert(sota->units_loaded[UNIT_ID_SILOU] > TNECS_NULL);
    SDL_assert(killer_entity > TNECS_NULL);
    SDL_assert(boss_entity != killer_entity);

    /* Kill boss */
    struct Unit *boss_unit = TNECS_GET_COMPONENT(sota->world, boss_entity, Unit);
    SDL_assert(boss_unit != NULL);
    boss_unit->alive = false;
    struct Boss *boss = TNECS_GET_COMPONENT(sota->world, boss_entity, Boss);
    SDL_assert(boss != NULL);

    Event_Emit(__func__, SDL_USEREVENT, event_Unit_Dies, &boss_entity, &killer_entity);
    SDL_Event event;
    SDL_assert(SDL_PollEvent(&event));

    /* Events_Manage should trigger map win on boss death */
    // receive_event_Unit_Dies->Map_Conditions_Check_Death->Receive_event_Map_Win)

    Events_Manage(sota);

    /* Check Win */
    nourstest_true(sota->map->win);

    /* Quit game */
    Game_Free(sota);
    nourstest_true(true);
}

void test_main_char_death_loss(int argc, char *argv[]) {
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
    Game_Map_Load(sota, CHAPTER_TEST_V8);

    /* Send some kind of event */
    Events_Manage(sota);

    /* Check something happened */

    /* Save game to test/debug_map_2.json */

    /* Quit game */
    Game_Free(sota);
    nourstest_true(false);
}

void test_silou_death_loss(int argc, char *argv[]) {
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
    Game_Map_Load(sota, CHAPTER_TEST_V8);

    /* Send some kind of event */
    Events_Manage(sota);

    /* Check something happened */

    /* Save game to test/debug_map_2.json */

    /* Quit game */
    Game_Free(sota);
    nourstest_true(false);
}
