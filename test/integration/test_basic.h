#include "game/game.h"

void test_basic(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Game *sota = SDL_malloc(sizeof(struct Game));
    *sota = Game_default;
    sota->settings = Settings_default;
    sota->settings.window = false;
    Game_Init(sota, argc, argv);

    /* Load Save file test/debug_map.json */

    /* Send some kind of event */

    /* Check something happened */
    /* Save game to test/debug_map_2.json */

    /* Quit game */
    Pre_Game_Free();
    Game_Free(sota);
    nourstest_true(true);
}