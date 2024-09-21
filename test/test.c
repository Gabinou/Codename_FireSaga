#include "filesystem.h"
#include "nourstest.h"
#include "test_AI.h"
#include "test_arrow.h"
#include "test_bitfields.h"
#include "test_camp.h"
#include "test_events.h"
#include "test_combat.h"
#include "test_convoy.h"
#include "test_crt.h"
#include "test_equations.h"
#include "test_party.h"
#include "test_enums.h"
#include "test_filesystem.h"
#include "test_conditions.h"
#include "test_game.h"
#include "test_item.h"
#include "test_map.h"
#include "test_menu.h"
#include "test_nstr.h"
#include "test_RNG.h"
#include "test_pathfinding.h"
#include "test_position.h"
#include "test_pixelfonts.h"
#include "test_scene.h"
#include "test_sprite.h"
#include "test_octant.h"
#include "test_text.h"
#include "test_tile.h"
#include "test_unit.h"
#include "test_utilities.h"
#include "test_weapon.h"

#include "bars/test_slider_bar.h"

#include "menu/test_deployment.h"
#include "menu/test_growths.h"
#include "menu/test_item_drop.h"
#include "menu/test_player_select.h"
#include "menu/test_pre_combat.h"
#include "menu/test_stats.h"
#include "menu/test_weapon_select.h"

#include "popup/test_loadout_stats.h"
#include "popup/test_map_combat.h"
#include "popup/test_objective.h"
#include "popup/test_text_bubble.h"
#include "popup/test_unit.h"

#include "integration/test_aura.h"
#include "integration/test_basic.h"
#include "integration/test_conditions.h"

void render_tests() {
    SDL_Log("Running rendering tests\n");
    printf("Running rendering tests\n");
    test_arrow();
    test_menu_deployment();
    test_menu_growths();
    test_menu_loadout_select();
    test_menu_player_select();
    test_menu_pre_combat();
    test_menu_item_drop();
    test_menu_stats();
    test_popup_loadout_stats();
    test_popup_map_combat();
    test_popup_objective();
    test_popup_unit();
    test_bars_slider_bar();
    test_text_bubble();
}

void unit_tests() {
    SDL_Log("Running unit tests\n");
    printf("Running unit tests\n");
    nourstest_run("AI ",            test_AI);
    nourstest_run("Bitfields ",     test_bitfields);
    nourstest_run("Camp ",          test_camp);
    nourstest_run("Combat ",        test_combat);
    nourstest_run("Conditions ",    test_conditions);
    nourstest_run("Convoy ",        test_convoy);
    // nourstest_run("CRT ",        test_crt); // CRT filter too slow
    nourstest_run("Enums ",         test_enums);
    nourstest_run("Equations ",     test_equations);
    nourstest_run("Filesystem ",    test_filesystem);
    nourstest_run("Game ",          test_game);
    nourstest_run("Item",           test_item);
    nourstest_run("Map ",           test_map);
    nourstest_run("Pathfinding ",   test_pathfinding);
    nourstest_run("Menu ",          test_menu);
    nourstest_run("nstr ",          test_nstr);
    nourstest_run("Octant ",        test_octant);
    nourstest_run("Party ",         test_party);
    nourstest_run("Position ",      test_position);
    nourstest_run("Pixelfonts ",    test_pixelfonts);
    nourstest_run("RNG ",           test_RNG);
    nourstest_run("Scene ",         test_scene);
    nourstest_run("Sprite ",        test_sprite);
    nourstest_run("Text ",          test_text);
    nourstest_run("Tile ",          test_tile);
    nourstest_run("Unit ",          test_unit);
    nourstest_run("Utilities ",     test_utilities);
    nourstest_run("Weapon ",        test_weapon);
}

void integration_tests(int argc, char *argv[]) {
    SDL_Log("Running integration tests\n");
    printf("Running integration tests\n");

    nourstest_run_args("Minimal",       test_minimal,               argc, argv);
    nourstest_run_args("Step",          test_step,                  argc, argv);
    nourstest_run_args("Load/Save",     test_load_save,             argc, argv);
    nourstest_run_args("Loss Death 1",  test_main_char_death_loss,  argc, argv);
    nourstest_run_args("Loss Death 2",  test_silou_death_loss,      argc, argv);
    nourstest_run_args("Win Boss",      test_boss_death_win,        argc, argv);
    nourstest_run_args("Aura apply",    test_aura_apply,            argc, argv);
    nourstest_run_args("Aura decay",    test_aura_decay,            argc, argv);
    nourstest_run_args("Aura fsm",      test_aura_fsm,              argc, argv);
}

void random_checks() {
#if HAVE_STDIO_H
    SDL_Log("Printing all names\n");
    // Names_Print_All("");
#endif /* HAVE_STDIO_H */

    SDL_LogDebug(SOTA_LOG_SYSTEM, "Running on "PLATFORM_NAME);
    SDL_LogDebug(SOTA_LOG_SYSTEM, "SDL version  %d %d %d ", SDL_MAJOR_VERSION,  SDL_MINOR_VERSION,
                 SDL_PATCHLEVEL);
    SDL_LogDebug(SOTA_LOG_SYSTEM, "SotA version %d %d %d ", SOTA_VER_MAJOR,    SOTA_VER_MINOR,
                 SOTA_VER_PATCH);
    if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        SDL_LogDebug(SOTA_LOG_SYSTEM, "SDL endianness is SDL_LIL_ENDIAN");
    else
        SDL_LogDebug(SOTA_LOG_SYSTEM, "SDL endianness is SDL_BIG_ENDIAN");
}

int main(int argc, char *argv[]) {
    /* -- Checks -- */
    random_checks();

    /* -- Preliminaries -- */
#ifdef SOTA_ASSERT_ALWAYS_BREAK
    SDL_SetAssertionHandler(sota_assert_handler, NULL);
#endif /* SOTA_ASSERT_ALWAYS_BREAK */

    Log_Init();
    freopen("test_results.txt", "w+", stdout);
    SDL_Log("Filesystem_Init\n");
    Filesystem_Init(0);
    SDL_Log("Initializing utilities\n");
    Utilities_Load();
    SDL_Log("Initializing RNG\n");
    RNG_Init_xoroshiro256ss();
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        SDL_Log("SDL_Init failed\n");
        exit(1);
    }

    /* -- Running tests -- */
    // test_events();
    unit_tests();
    render_tests();
    // integration_tests(argc, argv);
    nourstest_results();

    /* -- Postliminaries -- */
    Game_Post_Free();
    SDL_Quit();
    fclose(stdout);
    return (0);
}