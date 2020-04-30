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
    lrun("Test_linalg", test_linalg);
    lrun("Test_pathfinding ", test_pathfinding);
    lrun("Test_utilities ", test_utilities);
    lrun("Test_position ", test_position);
    lrun("Test_tile ", test_tile);
    lrun("Test_map ", test_map);
    lrun("Test_unit ", test_unit);
    lrun("test_probability ", test_probability);
    lrun("test_filesystem ", test_filesystem);
    lresults();

    return (lfails != 0);
}