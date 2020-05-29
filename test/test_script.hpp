#include "minctest.h"
#include "script.hpp"

void test_script() {
    SDL_Log("test_script");
    Scene scene1, scene2, scene3;
    scene1.readJSON("scenes//scene_test.json");
    lok(PHYSFS_exists("scenes//scene_test.json") != 0);




    scene1.writeJSON("scenes//scene_rewrite.json");
    lok(PHYSFS_exists("scenes//scene_rewrite.json") != 0);

}
