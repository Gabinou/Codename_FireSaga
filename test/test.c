#include "minctest.h"
#include "test_AI.h"
#include "test_camp.h"
#include "test_convoy.h"
#include "test_darr.h"
#include "test_equations.h"
#include "test_enums.h"
#include "test_filesystem.h"
#include "test_game.h"
#include "test_linalg.h"
#include "test_item.h"
#include "test_map.h"
#include "test_menu.h"
#include "test_pathfinding.h"
#include "test_probability.h"
#include "test_position.h"
#include "test_q_math.h"
#include "test_sprite.h"
#include "test_narrative.h"
#include "test_str.h"
#include "test_text.h"
#include "test_tile.h"
#include "test_unit.h"
#include "test_utilities.h"
#include "test_weapon.h"

int main(int argc, char * argv[]) {
    freopen("test_results.txt", "w+", stdout);
    SDL_Log("Initializing utilities\n");
    utilities_allNames_Load();
    char * buildDir = (char *) malloc(DEFAULT_BUFFER_SIZE);
    char * assetsDir = (char *) malloc(DEFAULT_BUFFER_SIZE);
    char * srcDir = (char *) malloc(DEFAULT_BUFFER_SIZE);
    strcpy(buildDir, SDL_GetBasePath());
    strcpy(assetsDir, strdup(buildDir));
    strcpy(srcDir, strdup(buildDir)); // srcDir, future home of release.exe
    assetsDir = path_Top_Remove(assetsDir, DIR_SEPARATOR[0]);
    srcDir = path_Top_Remove(srcDir, DIR_SEPARATOR[0]);
    strcat(assetsDir, DIR_SEPARATOR);
    strcat(assetsDir, "assets");
    filesystem_init(0, srcDir, assetsDir);
    SDL_Log("Initializing TinyMT\n");
    init_tinyMT(&tinyMT_global);
    SDL_Log("Running tests\n");
    printf("Running tests\n");
    lrun("AI ", test_AI);
    // lrun("Camp ", test_camp);
    // lrun("Convoy ", test_convoy);
    lrun("Enums ", test_enums);
    lrun("Equations ", test_equations);
    lrun("Filesystem ", test_filesystem);
    lrun("Game ", test_game);
    lrun("Item", test_item);
    lrun("Map ", test_map);
    lrun("Menu ", test_menu);
    lrun("Pathfinding ", test_pathfinding);
    lrun("Quick Math ", test_q_math);
    lrun("Position ", test_position);
    lrun("Probability ", test_probability);
    lrun("Sprite ", test_sprite);
    lrun("Str ", test_str);
    lrun("Narrative ", test_narrative);
    lrun("Text ", test_text);
    lrun("Tile ", test_tile);
    lrun("Unit ", test_unit);
    lrun("Utilities ", test_utilities);
    lrun("Weapon ", test_weapon);
    lresults();
    fclose(stdout);
    return (lfails != 0);
}