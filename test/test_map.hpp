#include "minctest.h"
#include "map.hpp"
#include "sprite.hpp"
#include <entityx/entityx.h>

void test_map() {
	entityx::EntityX ex;
    Map map(32, 32);
    map.loadTiles(0);
    map.loadTilemap(0);
    map.setArrivals(mapArrivals[0]());
    map.setArrivalEquipments(arrivalEquipments[0]());
    std::string asset_name;
    entityx::Entity Uent = ex.entities.create();;
    Unit temp_unit;
    Unit_stats temp;
    Inventory_item temp_wpn;
    std::vector<short int> temp_supports;
    Equipped temp_equipped;
    temp = {15,  4,  5,  7,  6,   8,  4,  6,  5,  5,  6};
    temp_unit = Unit(UNIT::NAME::SILOU, UNIT::CLASS::MAGE, temp, UNIT::SEX::F);
    temp = {48, 14, 25, 32, 34,  28, 19, 40, 15};
    temp_unit.setCaps(temp);
    temp = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    temp_unit.setGrowths(temp);
    temp_unit.setBaseExp(400);
    temp_wpn.id = ITEM::NAME::BALL_LIGHTNING;
    temp_unit.addEquipment(temp_wpn);

    asset_name = "..//assets//" +  temp_unit.getName() + ".png";
    Uent.assign<Unit>(temp_unit);
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
