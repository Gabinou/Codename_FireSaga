#include "minctest.h"
#include "game.hpp"

void test_game() {
    SDL_Log("test_game");
    Game * testgame1 = nullptr;
    unsigned char test_RN1, test_RN3;

    Settings temp_settings;
    temp_settings.FPS.show = true;
    temp_settings.fontsize = 28;
    testgame1 = new Game(temp_settings);
    std::vector<short int> unit_inds = {UNIT::NAME::SILOU};
    testgame1->loadUnits(unit_inds);
    init_tinyMT(testgame1->getTinymt32());


    Convoy convoy;
    convoy.setWeapons(testgame1->getWeapons());

    Inventory_item temp;
    temp.id = ITEM::NAME::WOODEN_SWORD;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::KITCHEN_KNIFE;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::WRATH_SWORD;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::FLEURET;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::DAMAS_SWORD;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::DAMAS_SWORD;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::WOODEN_SWORD;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::IRON_SWORD;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::STEEL_SWORD;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::DAMAS_SWORD;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::ESPADON;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::EXSANGUE;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::SECUNDUS;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::FAUCHON;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::SABER;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::GALATINE;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::HAUTECLAIRE;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::BROADSWORD;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::MERCIFUL_BLADE;
    convoy.deposit(temp);

    temp.id = ITEM::NAME::DAMAS_LANCE;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::ACHILLES;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::VEL;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::IRON_LANCE;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::TONBOKIRI;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::DAMAS_LANCE;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::WRATH_LANCE;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::WRATH_LANCE;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::TRIDENT;
    convoy.deposit(temp);
    temp.id = ITEM::NAME::SPEAR;
    convoy.deposit(temp);

    testgame1->setConvoy(convoy);
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
    testgame1->copySave(1, 2);
    lok(PHYSFS_exists("saves//save0002.bsav") > 0);
    lok(fequal("saves//save0002.bsav", "saves//save0001.bsav"));
    testgame1->copySave(2, 3);
    lok(PHYSFS_exists("saves//save0003.bsav") > 0);
    lok(fequal("saves//save0002.bsav", "saves//save0003.bsav"));
    lok(fequal("saves//save0001.bsav", "saves//save0003.bsav"));
    testgame1->deleteSave(2);
    lok(PHYSFS_exists("saves//save0002.bsav") == 0);
    testgame1->clean();


    // Game * testgame2 = nullptr;
    // testgame2 = new Game();
    // testgame2->loadXML(1);
    // testgame2->saveXML(2);
    // lok(PHYSFS_exists("saves//save0002.bsav") > 0);
    // lok(fequal("saves//save0001.bsav", "saves//save0002.bsav"));
    // testgame2->clean();
    testgame1->getURN();
    testgame1->getURN();
    testgame1->getURN();
    testgame1->getURN();

    testgame1->saveJSON(5);
    lok(PHYSFS_exists("saves//save0005.bsav") > 0);
    Game * testgame3 = nullptr;
    testgame3 = new Game();
    testgame3->loadJSON(5);
    testgame3->saveJSON(6);
    lok(PHYSFS_exists("saves//save0006.bsav") > 0);
    lok(fequal("saves//save0005.bsav", "saves//save0006.bsav"));

    for (int i = 0; i < 10; i++) {
        test_RN1 = testgame1->getURN();
        test_RN3 = testgame3->getURN();
        lok(test_RN1 == test_RN3);
    }


    testgame3->clean();
}
