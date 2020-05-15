#include "minctest.h"
#include "map.hpp"
#include "sprite.hpp"
#include <entityx/entityx.h>

void test_map() {
    entityx::EntityX ex;
    short int tilesize = 32;
    Map map(tilesize, tilesize);

    std::vector<short int> in_tilesindex, out_tilesindex;
    in_tilesindex = chapTiles[0]();
    map.loadTiles(0);
    out_tilesindex = map.getTilesindex();
    lok(in_tilesindex == out_tilesindex);
    std::vector<std::vector<short int>> in_tilemap, out_tilemap;
    in_tilemap = testTilemap();
    map.loadTilemap(0);
    out_tilemap = map.getTilemap();
    lok(in_tilemap == out_tilemap);

    lok(map.getOffset()[0] == 1);
    lok(map.getOffset()[1] == 1);

    Point tiles_pos;
    tiles_pos = map.pixel2tile(tilesize - 4, tilesize - 4);
    lok(tiles_pos.y == 0);
    lok(tiles_pos.y == 0);

    tiles_pos = map.pixel2tile(tilesize + 4, tilesize + 4);
    lok(tiles_pos.x == 0);
    lok(tiles_pos.y == 0);

    tiles_pos = map.pixel2tile(tilesize * 2 + 4, tilesize * 2 + 4);
    lok(tiles_pos.x == 1);
    lok(tiles_pos.y == 1);

    tiles_pos = map.pixel2tile(tilesize * 100 + 4, tilesize * 100 + 4);
    lok(tiles_pos.x == (map.getBounds()[1] - map.getOffset()[0]));
    lok(tiles_pos.y == (map.getBounds()[3] - map.getOffset()[1]));

    std::vector<Map_arrival> in_arrivals, out_arrivals;
    std::vector<std::vector<Inventory_item>> in_arrivalequipments, out_arrivalequipments;
    in_arrivals = mapArrivals[0]();
    in_arrivalequipments = arrivalEquipments[0]();
    map.setArrivals(in_arrivals);
    out_arrivals = map.getArrivals();

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

    map.setArrivalEquipments(in_arrivalequipments);
    out_arrivalequipments = map.getArrivalEquipments();

    if (out_arrivalequipments.size() == out_arrivalequipments.size()) {
        lok(true);

        for (short int i = 0; i < out_arrivalequipments.size(); i++) {
            if (out_arrivalequipments[i].size() == out_arrivalequipments[i].size()) {
                lok(true);

                for (short int j = 0; j < out_arrivalequipments[i].size(); j++) {
                    lok(out_arrivalequipments[i][j].id == in_arrivalequipments[i][j].id);
                    lok(out_arrivalequipments[i][j].used == in_arrivalequipments[i][j].used);
                    lok(out_arrivalequipments[i][j].infused == in_arrivalequipments[i][j].infused);
                }
            }
        }
    }

    Unit unit1, unit2;
    Unit_stats in_stats, out_stats;
    Unit_stats in_caps, out_caps;
    Unit_stats in_growths, out_growths;
    Inventory_item in_wpn, out_wpn;
    short int in_exp, out_exp;
    in_stats = {15,  4,  5,  7,  6,   8,  4,  6,  5,  5,  6};
    unit1 = Unit(UNIT::NAME::SILOU, UNIT::CLASS::MAGE, in_stats, UNIT::SEX::F);
    in_caps = {48, 14, 25, 32, 34, 28, 19, 40, 15};
    unit1.setCaps(in_caps);
    in_growths = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    unit1.setGrowths(in_growths);
    in_exp = 400;
    unit1.setBaseExp(in_exp);
    in_wpn.id = ITEM::NAME::BALL_LIGHTNING;
    unit1.addEquipment(in_wpn);

    std::string asset_name;
    asset_name = "..//assets//" +  unit1.getName() + ".png";
    entityx::Entity Uent1 = ex.entities.create();
    entityx::ComponentHandle<Unit> unithandle1, unithandle2;
    Uent1.assign<Unit>(unit1);
    Uent1.assign<Position>(6, 6);
    Uent1.assign<Sprite>(asset_name.c_str());
    map.putUnit(6, 6, Uent1.component<Unit>());
    unithandle1 = map.getUnit(6, 6);

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

    map.moveUnit(6, 6, 7, 7);
    unithandle1 = map.getUnit(6, 6);
    unithandle2 = map.getUnit(7, 7);

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

    map.writeXML("map_test.xml");
    map.writeJSON("map_test.json");

    Map test1, test2;
    test1.setManager(&ex.entities);
    test1.readXML("map_test.xml");
    test1.writeXML("map_rewrite.xml");
    lok(fequal("map_test.xml", "map_rewrite.xml"));
    test2.setManager(&ex.entities);
    test2.readJSON("map_test.json");
    test2.writeJSON("map_rewrite.json");
    lok(fequal("map_test.json", "map_rewrite.json"));
}
