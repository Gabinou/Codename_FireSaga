
#include "game/game.h"

void test_minimal(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    Settings settings   = Settings_default;
    settings.window     = SDL_WINDOW_HIDDEN;
    struct Game *sota   = Game_New(settings);
    nourstest_true(Game_State_Current(sota)      == GAME_STATE_Title_Screen);
    nourstest_true(Game_Substate_Current(sota)   == GAME_SUBSTATE_MENU);

    /* Quit game */
    SDL_assert(sota != NULL);
    Game_Free(sota);
    nourstest_true(true);
}

void test_step(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Settings settings    = Settings_default;
    settings.window             = SDL_WINDOW_HIDDEN;
    struct Game *sota           = Game_New(settings);
    nourstest_true(Game_State_Current(sota)      == GAME_STATE_Title_Screen);
    nourstest_true(Game_Substate_Current(sota)   == GAME_SUBSTATE_MENU);

    /* Stepping */
    Game_Step(sota);

    /* Quit game */
    Game_Free(sota);
    nourstest_true(true);
}

void test_map_load(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Settings settings    = Settings_default;
    settings.window             = SDL_WINDOW_HIDDEN;
    struct Game *sota           = Game_New(settings);
    nourstest_true(Game_State_Current(sota)      == GAME_STATE_Title_Screen);
    nourstest_true(Game_Substate_Current(sota)   == GAME_SUBSTATE_MENU);

    /* Load Map */
    Game_Map_Load(sota, CHAPTER_TEST_V8);

    /* Quit game */
    Game_Free(sota);
    nourstest_true(true);
}

void test_load_save(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Settings settings    = Settings_default;
    settings.window             = SDL_WINDOW_HIDDEN;
    struct Game *sota           = Game_New(settings);
    SDL_assert(sota->party.jsonio_header.json_element   == JSON_PARTY);
    nourstest_true(Game_State_Current(sota)      == GAME_STATE_Title_Screen);
    nourstest_true(Game_Substate_Current(sota)   == GAME_SUBSTATE_MENU);

    /* Load Save test file */
    char *path1 = PATH_JOIN("saves", "debug_save.json");
    s8 filename1 = s8_var(path1);
    _Game_loadJSON(sota, filename1);
    // Map *map = Game_Map(sota);
    // SDL_assert(map != NULL);
    // map->size.grid.x = 21;
    // map->size.grid.y = 21;

    /* Save game to test/debug_map_2.json */
    char *path2 = PATH_JOIN("saves", "debug_save2.json");
    s8 filename2 = s8_var(path2);
    _Game_saveJSON(sota, filename2);

    nourstest_true(jsonio_Equal(path1,  path2, true));
    nourstest_true(jsonio_Equal(path1,  path2, false));

    /* set party.save_filename to a file, check it gets written */
    char *path3 = PATH_JOIN("saves", "debug_party.json");
    sota->party.save_filename = s8_mut(path3);
    char *path4 = PATH_JOIN("saves", "debug_save4.json");
    char *path5 = PATH_JOIN("saves", "expected_party.json");
    s8 filename4 = s8_var(path4);
    SDL_assert(sota->party.jsonio_header.json_element   == JSON_PARTY);
    _Game_saveJSON(sota, filename4);

    nourstest_true(jsonio_Equal(path3,  path5, true));

    /* Quit game */
    Game_Free(sota);
    nourstest_true(true);
}
