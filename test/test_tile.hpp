#include "minctest.h"
#include "tile.hpp"

void test_tile() {
    Tile tile1;
    Tile tile2;
    Tile_stats in_tile_stats, out_tile_stats;
    Movement_cost in_cost, out_cost;

    in_tile_stats = {3, 0, 80, 0, };
    in_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
    tile1 = Tile(TILE::THRONE, "Throne", in_cost, in_tile_stats, true);
    lok(tile1.getName() == "Throne");
    lok(tile1.getid() == TILE::THRONE);
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
    // tile1.writeXML("tile_test.xml");

    // tile2 = Tile();
    // tile2.readXML("tile_test.xml");
    // tile2.writeXML("tile_rewrite.xml");

}
