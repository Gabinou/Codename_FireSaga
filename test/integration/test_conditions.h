
#include "game/game.h"
#include "map/ontile.h"
#include "macros.h"

void test_boss_death_win(int argc, char *argv[]) {
    /* -- Startup -- */
    Names_Load_All();
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    Settings settings   = Settings_default;
    settings.window     = SDL_WINDOW_HIDDEN;
    Game *sota = Game_New(settings);
    nourstest_true(Game_State_Current(sota)      == GAME_STATE_Title_Screen);
    nourstest_true(Game_Substate_Current(sota)   == GAME_SUBSTATE_MENU);

    /* Load Save file test/0001.json */
    Game_Save_Load(sota, SOTA_SAVE_DEBUG_1);
    Game_Map_Reinforcements_Load(sota);
    Map *map = Game_Map(sota);
    SDL_assert(DARR_NUM(map->units.onfield.arr) > 0);

    /* Get boss */
    tnecs_E boss_entity = Map_Unit_Get_Boss(map, ARMY_ENEMY);
    SDL_assert(boss_entity > TNECS_NULL);

    /* Get killer */
    i32 id;
    struct Point pos = {1, 1};
    tnecs_E killer_entity = Game_Party_Entity_Create(sota);
    SDL_assert(killer_entity > TNECS_NULL);
    Unit *silou = IES_GET_C(gl_world, killer_entity, Unit);
    Position *silou_pos = IES_GET_C(gl_world, killer_entity, Position);
    silou_pos->tilemap_pos = pos;
    Unit_id_set(silou, id = UNIT_ID_SILOU);
    s8 filename = {0};
    Game_Party_Entity_Init(sota, killer_entity, filename);
    Map_Unit_Put(map, pos.x, pos.y, killer_entity);
    SDL_assert(sota->party.Es[UNIT_ID_SILOU] > TNECS_NULL);
    SDL_assert(killer_entity > TNECS_NULL);
    SDL_assert(boss_entity != killer_entity);

    /* Kill boss */
    struct Unit *boss_unit = IES_GET_C(gl_world, boss_entity, Unit);
    SDL_assert(boss_unit != NULL);
    Unit_Alive_set(boss_unit, false);
    Unit_Army_set(boss_unit, ARMY_ENEMY);
    struct Boss *boss = IES_GET_C(gl_world, boss_entity, Boss);
    SDL_assert(boss != NULL);

    Event_Emit(__func__, SDL_USEREVENT, event_Unit_Dies, &boss_entity, &killer_entity);
    SDL_Event event;
    SDL_assert(SDL_PollEvent(&event));

    /* Events_Manage should trigger map win on boss death */
    // receive_event_Unit_Dies->Map_Conditions_Check_Death->Receive_event_Map_Win
    Events_Manage(sota);

    /* Check Win */
    nourstest_true(Map_isWon(map));
    nourstest_true(!Map_isLost(map));

    /* Quit game */
    Game_Free(sota);
    Names_Free();
    nourstest_true(true);
}

void test_main_char_death_loss(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_assert(event_names == NULL);
    // Names_Load_All();
    SDL_assert(_Unit_Name_Order(UNIT_ORDER_CORSAIR).num == strlen("Corsair"));

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Settings settings    = Settings_default;
    settings.window             = SDL_WINDOW_HIDDEN;
    struct Game *sota           = Game_New(settings);
    nourstest_true(Game_State_Current(sota)      == GAME_STATE_Title_Screen);
    nourstest_true(Game_Substate_Current(sota)   == GAME_SUBSTATE_MENU);

    /* Load Save file test/debug_map.json */
    Game_Save_Load(sota, SOTA_SAVE_DEBUG_1);
    Map *map = Game_Map(sota);
    SDL_assert(map->stack.dangermap != NULL);
    Game_Map_Reinforcements_Load(sota);
    SDL_assert(map->stack.dangermap != NULL);
    SDL_assert(DARR_NUM(map->units.onfield.arr) > 0);

    /* Get boss */
    tnecs_E boss_entity = Map_Unit_Get_Boss(map, ARMY_ENEMY);
    SDL_assert(boss_entity > TNECS_NULL);

    /* Get Silou */
    i32 id;
    struct Point pos = {1, 1};
    tnecs_E main_char_entity = Game_Party_Entity_Create(sota);
    struct Unit *erwin = IES_GET_C(gl_world, main_char_entity, Unit);
    Unit_Army_set(erwin, ARMY_FRIENDLY);
    struct Position *erwin_pos = IES_GET_C(gl_world, main_char_entity, Position);
    erwin_pos->tilemap_pos = pos;
    Unit_id_set(erwin, id = UNIT_ID_ERWIN);
    Unit_Class_set(erwin, UNIT_CLASS_STANDARD_BEARER);
    SDL_assert(main_char_entity > TNECS_NULL);
    Map_Unit_Put(map, pos.x, pos.y, main_char_entity);
    s8 filename = {0};
    Game_Party_Entity_Init(sota, main_char_entity, filename);

    SDL_assert(sota->party.Es[UNIT_ID_ERWIN] > TNECS_NULL);
    SDL_assert(main_char_entity > TNECS_NULL);
    SDL_assert(boss_entity != main_char_entity);

    /* Kill Silou */
    struct Unit *unit = IES_GET_C(gl_world, main_char_entity, Unit);
    SDL_assert(unit != NULL);
    Unit_Alive_set(unit, false);
    Unit_Army_set(unit, ARMY_FRIENDLY);
    struct Position *posptr = IES_GET_C(gl_world, main_char_entity, Position);
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
    nourstest_true(!Map_isWon(map));
    nourstest_true(Map_isLost(map));

    /* Quit game */
    SDL_assert(event_names != NULL);
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
    nourstest_true(Game_State_Current(sota)      == GAME_STATE_Title_Screen);
    nourstest_true(Game_Substate_Current(sota)   == GAME_SUBSTATE_MENU);

    /* Load Save file test/debug_map.json */
    Game_Save_Load(sota, SOTA_SAVE_DEBUG_1);

    Game_Map_Reinforcements_Load(sota);
    Map *map = Game_Map(sota);
    SDL_assert(DARR_NUM(map->units.onfield.arr) > 0);

    /* Get boss */
    tnecs_E boss_entity = Map_Unit_Get_Boss(map, ARMY_ENEMY);
    SDL_assert(boss_entity > TNECS_NULL);

    /* Get Silou */
    i32 id;
    struct Point pos = {1, 1};
    tnecs_E silou_entity = Game_Party_Entity_Create(sota);
    SDL_assert(silou_entity > TNECS_NULL);
    Unit *silou = IES_GET_C(gl_world, silou_entity, Unit);
    Position *silou_pos = IES_GET_C(gl_world, silou_entity, Position);
    silou_pos->tilemap_pos = pos;
    Unit_id_set(silou, id = UNIT_ID_SILOU);
    s8 filename = {0};
    Game_Party_Entity_Init(sota, silou_entity, filename);
    Map_Unit_Put(map, pos.x, pos.y, silou_entity);
    SDL_assert(sota->party.Es[UNIT_ID_SILOU] > TNECS_NULL);
    SDL_assert(silou_entity > TNECS_NULL);
    SDL_assert(boss_entity != silou_entity);

    // Game_Party_Entity_Create(sota, UNIT_ID_SILOU, pos, &Unit_default);
    // tnecs_E silou_entity = sota->party.Es[UNIT_ID_SILOU];
    // Map_Unit_Put(map, pos.x, pos.y, silou_entity);

    SDL_assert(sota->party.Es[UNIT_ID_SILOU] > TNECS_NULL);
    SDL_assert(silou_entity > TNECS_NULL);
    SDL_assert(boss_entity != silou_entity);

    /* Kill Silou */
    struct Unit *unit = IES_GET_C(gl_world, silou_entity, Unit);
    SDL_assert(unit != NULL);
    Unit_Alive_set(unit, false);
    struct Position *posptr = IES_GET_C(gl_world, silou_entity, Position);
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
    nourstest_true(!Map_isWon(map));
    nourstest_true(Map_isLost(map));

    /* Quit game */
    Game_Free(sota);
    Names_Free();
    nourstest_true(true);
}
