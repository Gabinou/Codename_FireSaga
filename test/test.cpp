#include "minctest.h"
#include "test_linalg.hpp"
#include "test_pathfinding.hpp"
#include "test_utilities.hpp"
#include "test_position.hpp"
#include "test_tile.hpp"
#include "test_map.hpp"
#include "test_unit.hpp"
#include "test_probability.hpp"
#include "test_filesystem.hpp"
#include "test_weapon.hpp"

int main(int argc, char * argv[]) {
    freopen("test_results.txt", "w+", stdout);

    SDL_Log("Initializing utilities\n");
    loadUtilities();

    SDL_Log("Initializing Filesystem\n");
    char * buildDir = SDL_GetBasePath();
    FILESYSTEM::init(0, buildDir, buildDir);

    SDL_Log("Initializing TinyMT\n");
    init_tinyMT();

    SDL_Log("Loading base weapons.");
    baseWeapons();

    printf("Running tests\n");
    lrun("linalg", test_linalg);
    lrun("Pathfinding ", test_pathfinding);
    lrun("Utilities ", test_utilities);
    lrun("Position ", test_position);
    lrun("Tile ", test_tile);
    lrun("Map ", test_map);
    lrun("Unit ", test_unit);
    lrun("Weapon ", test_weapon);
    lrun("Probability ", test_probability);
    lrun("Filesystem ", test_filesystem);
    lresults();

    return (lfails != 0);
}