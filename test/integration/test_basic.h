#include "game/game.h"

void test_minimal(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Game *sota = SDL_malloc(sizeof(struct Game));
    *sota = Game_default;
    sota->settings = Settings_default;
    sota->settings.window = false;
    Game_Init(sota, argc, argv);
    nourstest_true(sota->state      == GAME_STATE_Title_Screen);
    nourstest_true(sota->substate   == GAME_SUBSTATE_MENU);

    /* Quit game */
    Game_Free(sota);
    nourstest_true(true);
}

void test_step(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Game *sota = SDL_malloc(sizeof(struct Game));
    *sota = Game_default;
    sota->settings = Settings_default;
    sota->settings.window = false;
    Game_Init(sota, argc, argv);
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
    struct Game *sota = SDL_malloc(sizeof(struct Game));
    *sota = Game_default;
    sota->settings = Settings_default;
    sota->settings.window = false;
    Game_Init(sota, argc, argv);
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
    struct Game *sota = SDL_malloc(sizeof(struct Game));
    *sota = Game_default;
    sota->settings = Settings_default;
    sota->settings.window = false;
    Game_Init(sota, argc, argv);
    nourstest_true(sota->state      == GAME_STATE_Title_Screen);
    nourstest_true(sota->substate   == GAME_SUBSTATE_MENU);

    /* Load Save test file */
    char *path1 = PATH_JOIN("test", "integration", "debug_save.json");
    s8 filename1 = s8_var(path1);
    _Game_loadJSON(sota, filename1);

    /* Save game to test/debug_map_2.json */
    char *path2 = PATH_JOIN("test", "integration", "debug_save2.json");
    s8 filename2 = s8_var(path2);
    _Game_saveJSON(sota, filename2);

    nourstest_true(Filesystem_fequal(path1,  path2));

    /* Quit game */
    Game_Free(sota);
    nourstest_true(true);
}
