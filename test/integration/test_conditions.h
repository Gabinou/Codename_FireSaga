#include "game/game.h"

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
    Game_Map_Load(sota, CHAPTER_TEST_V8);

    /* Send some kind of event */
    Events_Manage(sota);

    /* Check something happened */

    /* Save game to test/debug_map_2.json */

    /* Quit game */
    Game_Free(sota);
    nourstest_true(false);
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