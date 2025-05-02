

#include "game/game.h"
#include "map/ontile.h"
#include "macros.h"

u32 event_Start;
#define REGISTER_ENUM(x, y) u32 event_##x;
#include "names/events.h"
#undef REGISTER_ENUM
#define REGISTER_ENUM(x, y) u32 event_Input_##x;
#include "names/input.h"
#undef REGISTER_ENUM
u32 event_End;

void test_boss_death_win(int argc, char *argv[]) {
    /* -- Startup -- */
    Names_Load_All();
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    Settings settings   = Settings_default;
    settings.window     = SDL_WINDOW_HIDDEN;
    Game *sota = Game_New(settings);
    nourstest_true(sota->state      == GAME_STATE_Title_Screen);
    nourstest_true(sota->substate   == GAME_SUBSTATE_MENU);

    /* Load Save file test/debug_map.json */
    Game_Map_Party_Load(sota, CHAPTER_TEST_V8);
    Game_Map_Reinforcements_Load(sota);
    SDL_assert(DARR_NUM(sota->map->units_onfield) > 0);

    /* Get boss */
    tnecs_entity boss_entity = Map_Unit_Get_Boss(sota->map, ARMY_ENEMY);
    SDL_assert(boss_entity > TNECS_NULL);

    /* Get killer */
    struct Point pos = {1, 1};
    Game_Party_Entity_Create(sota, UNIT_ID_SILOU, pos, &Unit_default);
    tnecs_entity killer_entity = sota->party.entities[UNIT_ID_SILOU];
    Map_Unit_Put(sota->map, pos.x, pos.y, killer_entity);
    SDL_assert(sota->party.entities[UNIT_ID_SILOU] > TNECS_NULL);
    SDL_assert(killer_entity > TNECS_NULL);
    SDL_assert(boss_entity != killer_entity);

    /* Kill boss */
    struct Unit *boss_unit = IES_GET_COMPONENT(sota->world, boss_entity, Unit);
    SDL_assert(boss_unit != NULL);
    Unit_Alive_set(boss_unit, false);
    struct Boss *boss = IES_GET_COMPONENT(sota->world, boss_entity, Boss);
    SDL_assert(boss != NULL);

    Event_Emit(__func__, SDL_USEREVENT, event_Unit_Dies, &boss_entity, &killer_entity);
    SDL_Event event;
    SDL_assert(SDL_PollEvent(&event));

    /* Events_Manage should trigger map win on boss death */
    // receive_event_Unit_Dies->Map_Conditions_Check_Death->Receive_event_Map_Win
    Events_Manage(sota);

    /* Check Win */
    nourstest_true(Map_isWon(sota->map));
    nourstest_true(!Map_isLost(sota->map));

    /* Quit game */
    Game_Free(sota);
    Names_Free();
    nourstest_true(true);
}

void test_main_char_death_loss(int argc, char *argv[]) {
    /* -- Startup -- */
    Names_Load_All();
    Events_Receivers_Declare();
    SDL_assert(global_unitNames[UNIT_ORDER_CORSAIR].num == strlen("Corsair"));

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Settings settings    = Settings_default;
    settings.window             = SDL_WINDOW_HIDDEN;
    struct Game *sota           = Game_New(settings);
    nourstest_true(sota->state      == GAME_STATE_Title_Screen);
    nourstest_true(sota->substate   == GAME_SUBSTATE_MENU);

    /* Load Save file test/debug_map.json */
    Game_Map_Party_Load(sota, CHAPTER_TEST_V8);
    SDL_assert(sota->map->stack.dangermap != NULL);
    Game_Map_Reinforcements_Load(sota);
    SDL_assert(sota->map->stack.dangermap != NULL);
    SDL_assert(DARR_NUM(sota->map->units_onfield) > 0);

    /* Get boss */
    tnecs_entity boss_entity = Map_Unit_Get_Boss(sota->map, ARMY_ENEMY);
    SDL_assert(boss_entity > TNECS_NULL);

    /* Get Silou */
    struct Point pos = {1, 1};
    Game_Party_Entity_Create(sota, UNIT_ID_ERWIN, pos, &Unit_default);
    tnecs_entity main_char_entity = sota->party.entities[UNIT_ID_ERWIN];
    Map_Unit_Put(sota->map, pos.x, pos.y, main_char_entity);
    SDL_assert(sota->party.entities[UNIT_ID_ERWIN] > TNECS_NULL);
    SDL_assert(main_char_entity > TNECS_NULL);
    SDL_assert(boss_entity != main_char_entity);

    /* Kill Silou */
    struct Unit *unit = IES_GET_COMPONENT(sota->world, main_char_entity, Unit);
    SDL_assert(unit != NULL);
    Unit_Alive_set(unit, false);
    struct Position *posptr = IES_GET_COMPONENT(sota->world, main_char_entity, Position);
    SDL_assert(posptr != NULL);
    SDL_assert(posptr->tilemap_pos.x == 1);
    SDL_assert(posptr->tilemap_pos.y == 1);

    // SDL_assert(event_Unit_Dies);
    // SDL_assert(SDL_USEREVENT);
    // SDL_assert(&main_char_entity);
    // SDL_assert(&boss_entity);
    Event_Emit(__func__, SDL_USEREVENT, event_Unit_Dies, &main_char_entity, &boss_entity);
    SDL_Event event;
    SDL_assert(SDL_PollEvent(&event));

    /* Events_Manage should trigger map win on boss death */
    // receive_event_Unit_Dies->Map_Conditions_Check_Death->Receive_event_Map_Win
    Events_Manage(sota);

    /* Check Win */
    nourstest_true(!Map_isWon(sota->map));
    nourstest_true(Map_isLost(sota->map));

    /* Quit game */
    Game_Free(sota);
    nourstest_true(true);
    Names_Free();
}

void test_silou_death_loss(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    Names_Load_All();

    struct Settings settings    = Settings_default;
    settings.window             = SDL_WINDOW_HIDDEN;
    struct Game *sota           = Game_New(settings);
    nourstest_true(sota->state      == GAME_STATE_Title_Screen);
    nourstest_true(sota->substate   == GAME_SUBSTATE_MENU);

    /* Load Save file test/debug_map.json */
    Game_Map_Party_Load(sota, CHAPTER_TEST_V8);
    Game_Map_Reinforcements_Load(sota);
    SDL_assert(DARR_NUM(sota->map->units_onfield) > 0);

    /* Get boss */
    tnecs_entity boss_entity = Map_Unit_Get_Boss(sota->map, ARMY_ENEMY);
    SDL_assert(boss_entity > TNECS_NULL);

    /* Get Silou */
    struct Point pos = {1, 1};
    Game_Party_Entity_Create(sota, UNIT_ID_SILOU, pos, &Unit_default);
    tnecs_entity silou_entity = sota->party.entities[UNIT_ID_SILOU];
    Map_Unit_Put(sota->map, pos.x, pos.y, silou_entity);
    SDL_assert(sota->party.entities[UNIT_ID_SILOU] > TNECS_NULL);
    SDL_assert(silou_entity > TNECS_NULL);
    SDL_assert(boss_entity != silou_entity);

    /* Kill Silou */
    struct Unit *unit = IES_GET_COMPONENT(sota->world, silou_entity, Unit);
    SDL_assert(unit != NULL);
    Unit_Alive_set(unit, false);
    struct Position *posptr = IES_GET_COMPONENT(sota->world, silou_entity, Position);
    SDL_assert(posptr != NULL);
    SDL_assert(posptr->tilemap_pos.x == 1);
    SDL_assert(posptr->tilemap_pos.y == 1);

    Event_Emit(__func__, SDL_USEREVENT, event_Unit_Dies, &silou_entity, &boss_entity);
    SDL_Event event;
    SDL_assert(SDL_PollEvent(&event));

    /* Events_Manage should trigger map win on boss death */
    // receive_event_Unit_Dies->Map_Conditions_Check_Death->Receive_event_Map_Win
    Events_Manage(sota);

    /* Check Loss */
    nourstest_true(!Map_isWon(sota->map));
    nourstest_true(Map_isLost(sota->map));

    /* Quit game */
    Game_Free(sota);
    Names_Free();
    nourstest_true(true);
}
