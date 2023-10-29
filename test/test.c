
#include "filesystem.h"
#include "nourstest.h"
#include "test_AI.h"
#include "test_arrow.h"
#include "test_bitfields.h"
#include "test_camp.h"
#include "test_combat.h"
#include "test_convoy.h"
#include "test_crt.h"
#include "test_equations.h"
#include "test_enums.h"
#include "test_filesystem.h"
#include "test_game.h"
#include "test_item.h"
#include "test_map.h"
#include "test_menu.h"
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

#include "menu/test_growths.h"
#include "menu/test_weapon_select.h"
#include "menu/test_player_select.h"
#include "menu/test_pre_combat.h"
#include "menu/test_item_drop.h"
#include "menu/test_stats.h"
#include "popup/test_loadout_stats.h"
#include "popup/test_map_combat.h"
#include "popup/test_objective.h"
#include "popup/test_text_bubble.h"

void render_tests() {
    SDL_Log("Running rendering tests\n");
    printf("Running rendering tests\n");
    test_menu_growths();
    test_menu_weapon_select();
    test_menu_player_select();
    test_menu_pre_combat();
    test_menu_item_drop();
    test_menu_stats();
    test_popup_loadout_stats();
    test_popup_map_combat();
    // test_popup_objective();
    test_popup_objective();
    test_text_bubble();
}

void benchmarks() {
    bench_RNG();
}

void unit_tests() {
    SDL_Log("Running nourstests\n");
    printf("Running nourstests\n");
    nourstest_run("AI ",            test_AI);
    nourstest_run("Bitfields ",     test_bitfields);
    nourstest_run("Camp ",          test_camp);
    nourstest_run("Combat ",        test_combat);
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
    nourstest_run("Octant ",        test_octant);
    nourstest_run("Position ",      test_position);
    nourstest_run("Pixelfonts ",    test_pixelfonts);
    nourstest_run("RNG ",           test_RNG);
    // nourstest_run("Scene ",      test_scene);
    nourstest_run("Sprite ",        test_sprite);
    nourstest_run("Text ",          test_text);
    nourstest_run("Tile ",          test_tile);
    nourstest_run("Unit ",          test_unit);
    nourstest_run("Utilities ",     test_utilities);
    nourstest_run("Weapon ",        test_weapon);
    nourstest_results();
}

int main(int argc, char *argv[]) {
    /* -- Preliminaries -- */
    // SDL_LogSetOutputFunction(Filesystem_Log2file, NULL);
    freopen("test_results.txt", "w+", stdout);
    SDL_Log("Filesystem_Init\n");
    Filesystem_Init(0);
    SDL_Log("Initializing utilities\n");
    Utilities_Load();
    SDL_Log("Initializing TinyMT\n");
    tinyMT_global = malloc(sizeof(*tinyMT_global));
    RNG_Init_tinymt(tinyMT_global);

    /* -- Running benchmarks -- */
    // benchmarks();
    // getchar();

    /* -- Running tests -- */
    unit_tests();
    render_tests();

    /* -- Postliminaries -- */
    Utilities_Free();
    Filesystem_Free();
    free(tinyMT_global);
    SDL_Quit();
    fclose(stdout);
    return (0);
}