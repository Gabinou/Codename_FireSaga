
#include "game/game.h"

void test_minimal(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    Settings settings   = Settings_default;
    settings.window     = SDL_WINDOW_HIDDEN;
    struct Game *sota   = Game_New(settings);
    nourstest_true(sota->state      == GAME_STATE_Title_Screen);
    nourstest_true(sota->substate   == GAME_SUBSTATE_MENU);

    /* Quit game */
    Game_Free(sota);
    nourstest_true(true);
}

void test_step(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Settings settings    = Settings_default;
    settings.window             = SDL_WINDOW_HIDDEN;
    struct Game *sota           = Game_New(settings);
    nourstest_true(sota->state      == GAME_STATE_Title_Screen);
    nourstest_true(sota->substate   == GAME_SUBSTATE_MENU);

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
    nourstest_true(sota->state      == GAME_STATE_Title_Screen);
    nourstest_true(sota->substate   == GAME_SUBSTATE_MENU);

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
    nourstest_true(sota->state      == GAME_STATE_Title_Screen);
    nourstest_true(sota->substate   == GAME_SUBSTATE_MENU);
    struct Map map = Map_default;
    sota->map = &map;
    sota->map->size.grid.x = 21;
    sota->map->size.grid.y = 21;

    /* Load Save test file */
    char *path1 = PATH_JOIN("saves", "debug_save.json");
    s8 filename1 = s8_var(path1);
    _Game_loadJSON(sota, filename1);

    /* Save game to test/debug_map_2.json */
    char *path2 = PATH_JOIN("saves", "debug_save2.json");
    s8 filename2 = s8_var(path2);
    _Game_saveJSON(sota, filename2);

    nourstest_true(jsonio_Equal(path1,  path2, true));
    nourstest_true(jsonio_Equal(path1,  path2, false));

    /* Quit game */
    sota->map = NULL;
    Game_Free(sota);
    nourstest_true(true);
}
