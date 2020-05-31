#include "minctest.h"
#include "script.hpp"
#include "enums.hpp"

void test_script() {
    SDL_Log("test_script");
    Scene scene1, scene2, scene3;
    Narrative narr1, narr2, narr3;
    narr1.recruited[UNIT::NAME::ERWIN] = true;
    narr1.death[UNIT::NAME::ERWIN] = false;
    narr1.recruited[UNIT::NAME::KIARA] = true;
    narr1.death[UNIT::NAME::KIARA] = false;

    scene1.readJSON("scenes//scene_test.json");
    SDL_Log("read scene");
    lok(PHYSFS_exists("scenes//scene_test.json") != 0);
    scene1.setNarrative(&narr1);
    scene1.makeLines();
    lok(strcmp(scene1.nextLine().line.c_str(), "!!!") == 0);
    lok(strcmp(scene1.nextLine().line.c_str(), "sniff") == 0);

    scene1.writeJSON("scenes//scene_rewrite.json");
    lok(PHYSFS_exists("scenes//scene_rewrite.json") != 0);
    lok(fequal("scenes//scene_rewrite.json", "scenes//scene_test.json"));
}
