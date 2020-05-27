#include "minctest.h"
#include "map.hpp"
#include "sprite.hpp"
#include <entityx/entityx.h>

void aremapsequal(Map map1, Map map2) {
    std::vector<short int> in_tilesindex, out_tilesindex;
    in_tilesindex = map1.getTilesindex();
    out_tilesindex = map2.getTilesindex();

    if (out_tilesindex.size() == in_tilesindex.size()) {
        for (short int i = 0; i < in_tilesindex.size(); i++) {
            // SDL_Log("%d %d", in_tilesindex[i], out_tilesindex[i]);
            lok(in_tilesindex[i] == out_tilesindex[i]);
        }
    } else {
        lok(false);
    }

    std::vector<std::vector<short int>> in_tilemap, out_tilemap;
    in_tilemap = map1.getTilemap();
    out_tilemap = map2.getTilemap();
    // SDL_Log("%d %d", out_tilemap.size(), in_tilemap.size());

    if (in_tilemap.size() == out_tilemap.size()) {
        for (int row = 0; row < in_tilemap.size(); row++) {// This loop cache friendly.
            if (in_tilemap[row].size() == out_tilemap[row].size()) {

                for (int col = 0; col < in_tilemap[row].size(); col++) {
                    lok(in_tilemap[row][col] == out_tilemap[row][col]);
                }
            } else {
                lok(false);
            }
        }
    } else {
        lok(false);
    }

    lok(map1.getOffset()[0] == map2.getOffset()[0]);
    lok(map1.getOffset()[1] == map2.getOffset()[1]);

    std::vector<Map_arrival> in_arrivals, out_arrivals;
    std::vector<std::vector<Inventory_item>> in_arrivalequipments, out_arrivalequipments;
    in_arrivals = map1.getArrivals();
    out_arrivals = map2.getArrivals();

    if (out_arrivals.size() == in_arrivals.size()) {
        lok(true);

        for (short int i = 0; i < in_arrivals.size(); i++) {
            lok(in_arrivals[i].turn == out_arrivals[i].turn);
            lok(in_arrivals[i].levelups == out_arrivals[i].levelups);
            lok(in_arrivals[i].id == out_arrivals[i].id);
            lok(in_arrivals[i].position.x == out_arrivals[i].position.x);
            lok(in_arrivals[i].position.y == out_arrivals[i].position.y);
        }
    }

    in_arrivalequipments = map1.getArrivalEquipments();
    out_arrivalequipments = map2.getArrivalEquipments();

    if (out_arrivalequipments.size() == in_arrivalequipments.size()) {
        lok(true);

        for (short int i = 0; i < out_arrivalequipments.size(); i++) {
            if (out_arrivalequipments[i].size() == in_arrivalequipments[i].size()) {
                lok(true);

                for (short int j = 0; j < out_arrivalequipments[i].size(); j++) {
                    lok(out_arrivalequipments[i][j].id == in_arrivalequipments[i][j].id);
                    lok(out_arrivalequipments[i][j].used == in_arrivalequipments[i][j].used);
                    lok(out_arrivalequipments[i][j].infused == in_arrivalequipments[i][j].infused);
                }
            }
        }
    }
}

std::vector<short int> testTiles() {
    std::vector<short int> temp_vector = {TILE::PLAIN * DEFAULT::TILE_DIVISOR + 0, TILE::FOREST * DEFAULT::TILE_DIVISOR + 0, TILE::SEA * DEFAULT::TILE_DIVISOR + 0, TILE::HILL * DEFAULT::TILE_DIVISOR + 0};
    return (temp_vector);
}


std::vector<Map_arrival> chapTestArrivals() {
    std::vector<Map_arrival> out;
    Map_arrival temp;
    temp.id = UNIT::NAME::BANDIT;
    temp.turn = 0;
    temp.position = {6, 10};
    temp.levelups = 0;
    out.push_back(temp);

    temp.id = UNIT::NAME::BANDIT;
    temp.turn = 0;
    temp.position = {12, 4};
    temp.levelups = 0;
    out.push_back(temp);

    temp.id = UNIT::NAME::SEBASTIAN;
    temp.turn = 0;
    temp.position = {20, 12};
    temp.levelups = 0;
    out.push_back(temp);
    return (out);
}

std::vector<Point> chapTestStartingPos() {
    std::vector<Point> out;
    return (out);
}

std::vector<std::vector<Inventory_item>> chapTestEquipments() {
    std::vector<std::vector<Inventory_item>> out;
    std::vector<Inventory_item> temp_equipment;
    Inventory_item temp_item;

    temp_item.id = ITEM::NAME::IRON_AXE;
    temp_equipment.push_back(temp_item);
    temp_item.id = ITEM::NAME::WOODEN_SHIELD;
    temp_equipment.push_back(temp_item);
    out.push_back(temp_equipment);
    temp_equipment.clear();
    temp_item.id = ITEM::NAME::IRON_AXE;
    temp_equipment.push_back(temp_item);
    temp_item.id = ITEM::NAME::WOODEN_SHIELD;
    temp_equipment.push_back(temp_item);
    out.push_back(temp_equipment);
    temp_equipment.clear();
    temp_item.id = ITEM::NAME::IRON_AXE;
    temp_equipment.push_back(temp_item);
    temp_item.id = ITEM::NAME::WOODEN_SHIELD;
    temp_equipment.push_back(temp_item);
    out.push_back(temp_equipment);
    temp_equipment.clear();
    return (out);
}

std::vector<std::vector<short int>> testTilemap() {
    std::vector<std::vector<short int>> tilemap = {
        {100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 300, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 300, 100, 300, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 300, 300, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 200, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 120, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 300, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 120, 120, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100, 100, 100, 100, 100, 200, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}
    };
    return (tilemap);
}

void test_map() {
    SDL_Log("test_map");
    entityx::EntityX ex;
    short int tilesize = 32;
    Map map1(tilesize, tilesize);
    Map map2, map3;

    std::vector<Point> in_startingpositions, out_startingpositions;
    Point temp = {0, 1};
    std::vector<short int> in_tilesindex, out_tilesindex;
    in_tilesindex = testTiles();
    map1.loadTiles(in_tilesindex);
    out_tilesindex = map1.getTilesindex();
    lok(in_tilesindex == out_tilesindex);
    std::vector<std::vector<short int>> in_tilemap, out_tilemap;
    in_tilemap = testTilemap();
    map1.setTilemap(in_tilemap);
    out_tilemap = map1.getTilemap();
    lok(in_tilemap == out_tilemap);

    lok(map1.getOffset()[0] == 1);
    lok(map1.getOffset()[1] == 1);

    std::vector<Map_arrival> in_arrivals, out_arrivals;
    std::vector<std::vector<Inventory_item>> in_arrivalequipments, out_arrivalequipments;
    in_arrivals = chapTestArrivals();
    in_arrivalequipments = chapTestEquipments();
    lok(in_arrivalequipments.size() == in_arrivals.size());

    map1.setArrivals(in_arrivals);
    out_arrivals = map1.getArrivals();

    if (out_arrivals.size() == in_arrivals.size()) {
        lok(true);

        for (short int i = 0; i < in_arrivals.size(); i++) {
            lok(in_arrivals[i].turn == out_arrivals[i].turn);
            lok(in_arrivals[i].levelups == out_arrivals[i].levelups);
            lok(in_arrivals[i].id == out_arrivals[i].id);
            lok(in_arrivals[i].position.x == out_arrivals[i].position.x);
            lok(in_arrivals[i].position.y == out_arrivals[i].position.y);
        }
    }

    map1.setArrivalEquipments(in_arrivalequipments);

    out_arrivalequipments = map1.getArrivalEquipments();

    if (out_arrivalequipments.size() == in_arrivalequipments.size()) {
        lok(true);

        for (short int i = 0; i < out_arrivalequipments.size(); i++) {
            if (out_arrivalequipments[i].size() == in_arrivalequipments[i].size()) {
                lok(true);

                for (short int j = 0; j < out_arrivalequipments[i].size(); j++) {
                    lok(out_arrivalequipments[i][j].id == in_arrivalequipments[i][j].id);
                    lok(out_arrivalequipments[i][j].used == in_arrivalequipments[i][j].used);
                    lok(out_arrivalequipments[i][j].infused == in_arrivalequipments[i][j].infused);
                }
            }
        }
    }

    std::unordered_map<short int, Weapon> weapons;
    Unit unit1, unit2;
    Unit_stats in_stats, out_stats;
    Unit_stats in_caps, out_caps;
    Unit_stats in_growths, out_growths;
    Inventory_item in_wpn, out_wpn;
    short int in_exp, out_exp;
    in_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5,  5,  6};
    unit1 = Unit(UNIT::NAME::SILOU, UNIT::CLASS::MAGE, in_stats, UNIT::SEX::F);
    unit1.setWeapons(&weapons);
    in_caps = {48, 14, 25, 32, 34, 28, 19, 40, 15};
    unit1.setCaps(in_caps);
    in_growths = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    unit1.setGrowths(in_growths);
    in_exp = 400;
    unit1.setBaseExp(in_exp);
    in_wpn.id = ITEM::NAME::BALL_LIGHTNING;
    unit1.addEquipment(in_wpn);

    entityx::Entity Uent1 = ex.entities.create();
    entityx::ComponentHandle<Unit> unithandle1, unithandle2;
    Uent1.assign<Unit>();
    Uent1.component<Unit>()->setWeapons(&weapons);
    Uent1.component<Unit>()->copyUnit(unit1);
    Uent1.assign<Position>(6, 6);
    unithandle1 = Uent1.component<Unit>();
    map1.putUnit(6, 6, unithandle1);
    unithandle1 = map1.getUnit(6, 6);

    if (unithandle1) {
        out_stats = unithandle1->getStats();
        lok(true);
        lok(unithandle1->getName() == "Silou");
        lok(unithandle1->getSex() == 0);
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
        out_growths = unithandle1->getGrowths();
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
    } else {
        lok(false);
    }

    map1.moveUnit(6, 6, 7, 7);
    unithandle1 = map1.getUnit(6, 6);
    unithandle2 = map1.getUnit(7, 7);

    if (unithandle2) {
        lok(true);
        out_stats = unithandle2->getStats();
        lok(unithandle2->getName() == "Silou");
        lok(unithandle2->getSex() == 0);
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
        out_growths = unithandle2->getGrowths();
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
    } else {
        lok(false);
    }

    if (PHYSFS_stat("saves", nullptr) == 0) {
        PHYSFS_mkdir("saves");
    }

    // map1.writeXML("saves//map_test.xml");
    map1.writeJSON("saves//map_test.json");

    // map2.setManager(&ex.entities);
    // map2.readXML("saves//map_test.xml");
    // map2.writeXML("saves//map_rewrite.xml");
    // lok(fequal("saves//map_test.xml", "saves//map_rewrite.xml"));
    map3.setManager(&ex.entities);
    map3.readJSON("saves//map_test.json");
    map3.writeJSON("saves//map_rewrite.json");
    lok(fequal("saves//map_test.json", "saves//map_rewrite.json"));
    lok(PHYSFS_exists("saves//map_rewrite.json"));
    lok(PHYSFS_exists("saves//map_test.json"));
    // aremapsequal(map1, map2); // XML will not be used.
    // aremapsequal(map2, map3);
    aremapsequal(map1, map3);
}
