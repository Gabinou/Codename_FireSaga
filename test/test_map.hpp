#include "minctest.h"
#include "map.hpp"
#include "sprite.hpp"
#include <entityx/entityx.h>

void test_map() {
	entityx::EntityX ex;
    Map map(32, 32);
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
			    for (short int j = 0; j < out_arrivalequipments[j].size(); i++) {
			    	lok(true);
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
    entityx::Entity Uent = ex.entities.create();;
    Uent.assign<Unit>(unit1);
    Uent.assign<Position>(6, 6);
    Uent.assign<Sprite>(asset_name.c_str());
    map.putUnit(6, 6, Uent.component<Unit>());


    map.writeXML("map_test.xml");

    Map test;
    test.setManager(&ex.entities);
    test.readXML("map_test.xml");
    test.writeXML("map_rewrite.xml");
    lok(fequal("map_test.xml", "map_rewrite.xml"));
}
