#include "minctest.h"
#include "test_convoy.hpp"
#include "test_filesystem.hpp"
#include "test_game.hpp"
#include "test_linalg.hpp"
#include "test_item.hpp"
#include "test_map.hpp"
#include "test_pathfinding.hpp"
#include "test_probability.hpp"
#include "test_position.hpp"
#include "test_sprite.hpp"
#include "test_text.hpp"
#include "test_tile.hpp"
#include "test_unit.hpp"
#include "test_utilities.hpp"
#include "test_weapon.hpp"

int main(int argc, char * argv[]) {
    freopen("test_results.txt", "w+", stdout);

    SDL_Log("Initializing utilities\n");
    loadUtilities();

    SDL_Log("Initializing filesystem\n");
    char * buildDir = SDL_GetBasePath();
    char * assetsDir = strdup(buildDir);
    char * srcDir = strdup(buildDir);
    path_removefolder(assetsDir);
    path_removefolder(srcDir);
    strncat(assetsDir, "assets\\", 6);
    FILESYSTEM::init(0, srcDir, assetsDir);

    SDL_Log("Initializing TinyMT\n");
    init_tinyMT();

    SDL_Log("Loading base weapons.");
    baseWeapons();

    printf("Running tests\n");
    lrun("Convoy ", test_convoy);
    lrun("Filesystem ", test_filesystem);
    // lrun("Game ", test_game);
    lrun("linalg", test_linalg);
    lrun("Item", test_item);
    lrun("Map ", test_map);
    lrun("Pathfinding ", test_pathfinding);
    lrun("Position ", test_position);
    lrun("Probability ", test_probability);
    lrun("Sprite ", test_sprite);
    lrun("Text ", test_text);
    lrun("Tile ", test_tile);
    lrun("Unit ", test_unit);
    lrun("Utilities ", test_utilities);
    lrun("Weapon ", test_weapon);
    lresults();

    return (lfails != 0);
}