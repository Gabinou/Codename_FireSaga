#include "minctest.h"
#include "tile.hpp"

void test_tile() {
    Tile tile1;
    Tile tile2;
    Tile_stats in_tile_stats, out_tile_stats;
    Movement_cost in_cost, out_cost;
    unsigned char * cost_array;
    in_tile_stats = {3, 0, 80, 0, };
    in_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
    tile1 = Tile(TILE::THRONE, "Throne", in_cost, in_tile_stats, true);
    lok(tile1.getName() == "Throne");
    lok(tile1.getid() == TILE::THRONE);
    cost_array = tile1.getCost();
    out_cost = tile1.getCoststruct();
    out_tile_stats = tile1.getStats();
    lok(in_cost.foot_slow == out_cost.foot_slow);
    lok(in_cost.foot_fast == out_cost.foot_fast);
    lok(in_cost.mages == out_cost.mages);
    lok(in_cost.riders_slow == out_cost.riders_slow);
    lok(in_cost.riders_fast == out_cost.riders_fast);
    lok(in_cost.fliers == out_cost.fliers);
    lok(in_cost.armors == out_cost.armors);
    lok(in_cost.pirates == out_cost.pirates);
    lok(in_cost.bandits == out_cost.bandits);
    lok(cost_array[UNIT::MVT::FOOT_SLOW] == out_cost.foot_slow);
    lok(cost_array[UNIT::MVT::FOOT_FAST] == out_cost.foot_fast);
    lok(cost_array[UNIT::MVT::MAGES] == out_cost.mages);
    lok(cost_array[UNIT::MVT::RIDERS_SLOW] == out_cost.riders_slow);
    lok(cost_array[UNIT::MVT::RIDERS_FAST] == out_cost.riders_fast);
    lok(cost_array[UNIT::MVT::FLIERS] == out_cost.fliers);
    lok(cost_array[UNIT::MVT::ARMORS] == out_cost.armors);
    lok(cost_array[UNIT::MVT::PIRATES] == out_cost.pirates);
    lok(cost_array[UNIT::MVT::BANDITS] == out_cost.bandits);
    lok(out_tile_stats.dodge == in_tile_stats.dodge);
    lok(out_tile_stats.Pprot == in_tile_stats.Pprot);
    lok(out_tile_stats.Mprot == in_tile_stats.Mprot);
    lok(out_tile_stats.heal == in_tile_stats.heal);
    lok(tile1.isInside() == true);
    tile1.writeXML("tile_test.xml");

    tile2 = Tile();
    // tile2.readXML("tile_test.xml");
    // tile2.writeXML("tile_rewrite.xml");

    // test_XMLtile();

}
