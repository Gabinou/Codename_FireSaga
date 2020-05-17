#include "minctest.h"
#include "game.hpp"

void test_game() {
    Game * testgame1 = nullptr;
    Settings temp_settings;

    temp_settings.FPS.show = true;
    temp_settings.fontsize = 28;
    testgame1 = new Game(temp_settings);
    testgame1->makeUnits(0);

    // testgame1->loadMap(0);

    // std::vector<short unsigned int> unit_inds = {UNIT::NAME::SILOU};
    // std::vector<std::vector<int>> positions_list = {{6, 6}};
    // testgame1->loadMapUnits(unit_inds, positions_list);

    // testgame1->loadCursor();

    // testgame1->loadMapArrivals();

    if (PHYSFS_stat("saves", nullptr) == 0) {
        PHYSFS_mkdir("saves");
    }

    testgame1->saveXML(1);
    lok(PHYSFS_exists("saves//save0001.bsav") > 0);
    testgame1->copySaveXML(1, 2);
    lok(PHYSFS_exists("saves//save0002.bsav") > 0);
    lok(fequal("saves//save0002.bsav", "saves//save0001.bsav"));
    testgame1->copySaveXML(2, 3);
    lok(PHYSFS_exists("saves//save0003.bsav") > 0);
    lok(fequal("saves//save0002.bsav", "saves//save0003.bsav"));
    lok(fequal("saves//save0001.bsav", "saves//save0003.bsav"));
    testgame1->deleteSaveXML(2);
    lok(PHYSFS_exists("saves//save0002.bsav") == 0);
    testgame1->clean();

    Game * testgame2 = nullptr;
    testgame2 = new Game();
    testgame2->loadXML(1);
    testgame2->saveXML(2);
    lok(PHYSFS_exists("saves//save0002.bsav") > 0);
    lok(fequal("saves//save0001.bsav", "saves//save0002.bsav"));
    testgame2->clean();
}
