
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

+