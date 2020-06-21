#include "minctest.h"
#include "unit.hpp"

void baseUnits(std::unordered_map<int, Unit> * in_units, std::vector<short int> toload) {
    SDL_Log("Making base units \n");
    Unit temp_unit;
    Unit_stats temp;
    Inventory_item temp_wpn;
    std::vector<short int> temp_supports;
    int index;

    for (short unsigned int i = 0; i < toload.size(); i++) {
        index = toload[i];
        temp_unit = Unit();

        switch (index) {
            case UNIT::NAME::ERWIN:
                //hp,str,mag,agi,dex,luck,def,res,con,move,prof
                temp = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5, 4};
                temp_unit = Unit(UNIT::NAME::ERWIN, UNIT::CLASS::MERCENARY, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15, 7, 20};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_wpn.id = ITEM::NAME::FLEURET;
                temp_unit.addItem(temp_wpn);
                temp_wpn.id = ITEM::NAME::KITCHEN_KNIFE;
                temp_unit.addItem(temp_wpn);
                temp_wpn.id = ITEM::NAME::POT_LID;
                temp_unit.addItem(temp_wpn);
                temp_supports = {UNIT::NAME::KIARA};
                temp_unit.setSupports(temp_supports);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::SEBASTIAN:
                temp = {18,  6,  2,  7,  7,   7,  4,  5,  6, 7};
                temp_unit = Unit(UNIT::NAME::SEBASTIAN, UNIT::CLASS::CAVALIER, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(100);
                temp_wpn.id = ITEM::NAME::IRON_LANCE;
                temp_unit.addItem(temp_wpn);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::MICHAEL:
                temp = {19,  6,  2,  7,  7,   7,  4,  5,  6,  7};
                temp_unit = Unit(UNIT::NAME::MICHAEL, UNIT::CLASS::CAVALIER, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(200);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::CHASSE:
                temp = {20,  6,  2,  7,  7,   7,  4,  5,  6,  6};
                temp_unit = Unit(UNIT::NAME::CHASSE, UNIT::CLASS::MOUSQUETAIRE, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(2200);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::KIARA:
                temp = {14,  6,  2,  7,  7,   7,  4,  5,  6,  5};
                temp_unit = Unit(UNIT::NAME::KIARA, UNIT::CLASS::CLERIC, temp, UNIT::SEX::F);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(100);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::PIROU:
                temp = {16,  6,  2,  7,  7,   7,  4,  5,  6,  6};
                temp_unit = Unit(UNIT::NAME::PIROU, UNIT::CLASS::PICKPOCKET, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(2200);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::SERVIL:
                temp = {22,  4,  5,  7,  6,   8,  4,  6,  5, 5}; // 4 or 5?
                temp_unit = Unit(UNIT::NAME::SERVIL, UNIT::CLASS::KNIGHT, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(500);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::PERIGNON:
                temp = {34,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::PERIGNON, UNIT::CLASS::MAGE, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(1200);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::LAUZON:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::LAUZON, UNIT::CLASS::MAGE, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(400);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::SILOU:
                //hp,str,mag,agi,dex,luck,def,res,con,move,prof
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5,  5,  6};
                temp_unit = Unit(UNIT::NAME::SILOU, UNIT::CLASS::MAGE, temp, UNIT::SEX::F);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(400);
                temp_wpn.id = ITEM::NAME::BALL_LIGHTNING;
                temp_unit.addItem(temp_wpn);
                temp_unit.equips(UNIT::HAND::RIGHT);
                temp_unit.setArmy(UNIT::ARMY::ERWIN);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::BANDIT:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::BANDIT, UNIT::CLASS::BANDIT, temp, 1);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_wpn.id = ITEM::NAME::IRON_AXE;
                temp_unit.addItem(temp_wpn);
                temp_unit.equips(UNIT::HAND::RIGHT);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::DUELIST:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::DUELIST, UNIT::CLASS::DUELIST, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::PICKPOCKET:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::PICKPOCKET, UNIT::CLASS::PICKPOCKET, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::THIEF:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::THIEF, UNIT::CLASS::THIEF, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::ASSASSIN:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::ASSASSIN, UNIT::CLASS::ASSASSIN, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::ARCHER:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::ARCHER, UNIT::CLASS::ARCHER, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::MARKSMAN:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::MARKSMAN, UNIT::CLASS::MARKSMAN, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::MERCENARY:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::MERCENARY, UNIT::CLASS::MERCENARY, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::HERO:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::HERO, UNIT::CLASS::HERO, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::CORSAIR:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::CORSAIR, UNIT::CLASS::CORSAIR, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::VIKING:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::VIKING, UNIT::CLASS::VIKING, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::RAVAGER:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::RAVAGER, UNIT::CLASS::RAVAGER, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::CAVALIER:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::CAVALIER, UNIT::CLASS::CAVALIER, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::PALADIN:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::PALADIN, UNIT::CLASS::PALADIN, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::FENCER:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::FENCER, UNIT::CLASS::FENCER, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;

            case UNIT::NAME::MOUSQUETAIRE:
                temp = {15,  4,  5,  7,  6,   8,  4,  6,  5, 5};
                temp_unit = Unit(UNIT::NAME::MOUSQUETAIRE, UNIT::CLASS::MOUSQUETAIRE, temp, UNIT::SEX::M);
                temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
                temp_unit.setCaps(temp);
                temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
                temp_unit.setGrowths(temp);
                temp_unit.setBaseExp(0);
                temp_unit.setArmy(UNIT::ARMY::ENEMY);
                in_units->erase(index);
                in_units->insert({index, temp_unit});
                break;
        }
    }
}

void baseUnits(std::unordered_map<int, Unit> * in_units) {
    SDL_Log("Making all base units \n");
    std::unordered_map<int, Unit> temp_units;
    std::vector<short int> toload = cpprange((short int)UNIT::NAME::ERWIN, (short int)(UNIT::NAME::PC_END - 1));
    baseUnits(in_units, toload);
}

void writeUnits_NPC(const char * filename) {
    Unit Utemp;
    std::unordered_map<int, Unit> units;
    std::vector<short int> temp_int = {0};

    for (int i = UNIT::NAME::PC_END; i < UNIT::NAME::NPC_END; i++) {
        temp_int[0] = i;
        baseUnits(&units, temp_int);
        Utemp = units[i];
        Utemp.writeXML(filename, true);
    }
}

void writeUnits_PC(const char * filename) {
    Unit Utemp;
    std::unordered_map<int, Unit> units;
    std::vector<short int> temp_int = {0};

    for (int i = UNIT::NAME::ERWIN; i < UNIT::NAME::PC_END; i++) {
        temp_int[0] = i;
        baseUnits(&units, temp_int);
        Utemp = units[i];
        Utemp.writeXML(filename, true);
    }
}

void writeAllUnits(const char * filename) {
    writeUnits_PC(filename);
    writeUnits_NPC(filename);
}

void test_unit() {
    SDL_Log("test_unit");
    Unit unit1, unit2, unit3, unit4, unit5;
    Unit_stats in_stats, out_stats;
    Unit_stats in_caps, out_caps;
    Unit_stats in_growths, out_growths;
    Inventory_item in_stats_wpn, out_stats_wpn;

    in_stats = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    unit1 = Unit(UNIT::NAME::SILOU, UNIT::CLASS::MERCENARY, in_stats, UNIT::SEX::M);
    in_caps = {48, 14, 25, 32, 34,  28, 19, 40, 15, 0};
    unit1.setCaps(in_caps);
    in_growths = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    unit1.setGrowths(in_growths);
    unit1.setBaseExp(0);
    in_stats_wpn.id = ITEM::NAME::FLEURET;
    unit1.addItem(in_stats_wpn);
    in_stats_wpn.id = ITEM::NAME::KITCHEN_KNIFE;
    unit1.addItem(in_stats_wpn);
    in_stats_wpn.id = ITEM::NAME::POT_LID;
    unit1.addItem(in_stats_wpn);
    out_stats = unit1.getStats();
    lok(unit1.getName() == "Silou");
    lok(unit1.getSex() == 1);
    lok(in_stats.hp == out_stats.hp);
    lok(in_stats.str == out_stats.str);
    lok(in_stats.mag == out_stats.mag);
    lok(in_stats.agi == out_stats.agi);
    lok(in_stats.dex == out_stats.dex);
    lok(in_stats.luck == out_stats.luck);
    lok(in_stats.def == out_stats.def);
    lok(in_stats.res == out_stats.res);
    lok(in_stats.con == out_stats.con);
    lok(in_stats.move == out_stats.move);
    lok(in_stats.prof == out_stats.prof);
    out_growths = unit1.getGrowths();
    lok(in_growths.hp == out_growths.hp);
    lok(in_growths.str == out_growths.str);
    lok(in_growths.mag == out_growths.mag);
    lok(in_growths.agi == out_growths.agi);
    lok(in_growths.dex == out_growths.dex);
    lok(in_growths.luck == out_growths.luck);
    lok(in_growths.def == out_growths.def);
    lok(in_growths.res == out_growths.res);
    lok(in_growths.con == out_growths.con);
    lok(in_growths.move == out_growths.move);
    lok(in_growths.prof == out_growths.prof);

    unit1.levelUp();
    unit1.levelUp();
    out_stats = unit1.getBases();
    lok(in_stats.hp == out_stats.hp);
    lok(in_stats.str == out_stats.str);
    lok(in_stats.mag == out_stats.mag);
    lok(in_stats.agi == out_stats.agi);
    lok(in_stats.dex == out_stats.dex);
    lok(in_stats.luck == out_stats.luck);
    lok(in_stats.def == out_stats.def);
    lok(in_stats.res == out_stats.res);
    lok(in_stats.con == out_stats.con);
    lok(in_stats.move == out_stats.move);
    lok(in_stats.prof == out_stats.prof);
    out_stats = unit1.getStats();
    lok(in_stats.hp <= out_stats.hp);
    lok(in_stats.str <= out_stats.str);
    lok(in_stats.mag <= out_stats.mag);
    lok(in_stats.agi <= out_stats.agi);
    lok(in_stats.dex <= out_stats.dex);
    lok(in_stats.luck <= out_stats.luck);
    lok(in_stats.def <= out_stats.def);
    lok(in_stats.res <= out_stats.res);
    lok(in_stats.con <= out_stats.con);
    lok(in_stats.move <= out_stats.move);
    lok(in_stats.prof <= out_stats.prof);

    if (PHYSFS_stat("saves", nullptr) == 0) {
        PHYSFS_mkdir("saves");
    }

    unit1.writeXML("saves//unit_test.xml");
    unit1.writeXML("saves//unit_test.binou");
    unit2 = Unit();
    unit2.readXML("saves//unit_test.xml");
    unit2.writeXML("saves//unit_rewrite.xml");
    unit3 = Unit();
    unit3.readXML("saves//unit_test.binou");
    unit3.writeXML("saves//unit_rewrite.binou");

    lok(fequal("saves//unit_test.xml", "saves//unit_test.binou"));
    lok(fequal("saves//unit_test.xml", "saves//unit_rewrite.xml"));
    lok(fequal("saves//unit_test.binou", "saves//unit_rewrite.binou"));

    unit1.writeJSON("saves//unit_test.json");
    unit1.writeJSON("saves//unit_testj.binou");
    unit4 = Unit();
    unit4.readJSON("saves//unit_test.json");
    unit4.writeJSON("saves//unit_rewrite.json");
    unit5 = Unit();
    unit5.readJSON("saves//unit_testj.binou");
    unit5.writeJSON("saves//unit_rewritej.binou");

    lok(PHYSFS_exists("saves//unit_test.json") != 0);
    lok(PHYSFS_exists("saves//unit_rewrite.json") != 0);
    lok(PHYSFS_exists("saves//unit_rewritej.binou") != 0);
    lok(fequal("saves//unit_test.json", "saves//unit_testj.binou"));
    lok(fequal("saves//unit_test.json", "saves//unit_rewrite.json"));
    lok(fequal("saves//unit_testj.binou", "saves//unit_rewritej.binou"));

}
