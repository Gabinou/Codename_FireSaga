
#include "nourstest.h"
#include "tile.h"
#include "jsonio.h"
#include "platform.h"
#include "utilities.h"
#include "scene.h"

void test_tile() {

    if (PHYSFS_stat("saves", NULL) == 0) {
        PHYSFS_mkdir("saves");
    }
    struct Tile tile2 = Tile_default;
    struct Tile tile3 = Tile_default;
    struct Tile_stats in_tile_stats = {0, 30, 0, 8};
    struct Tile_stats out_tile_stats = Tile_stats_default;
    struct Movement_cost in_cost = {2, 2, 2, 3, 3, 1, 3, 2, 2};
    struct Tile tile1 =  {
        .jsonio_header.json_element   = JSON_TILE,
        .cost           = in_cost,
        .inside         = true,
        .id             = TILE_THRONE,
        .stats          = in_tile_stats
    };
    tile1.name = s8_mut("Throne");
    nourstest_true(tile1.inside == true);
    struct Movement_cost out_cost = Movement_cost_default;
    nourstest_true(s8equal(tile1.name, s8_literal("Throne")));
    nourstest_true(tile1.id == TILE_THRONE);
    out_cost        = tile1.cost;
    out_tile_stats  = tile1.stats;
    nourstest_true(in_cost.foot_slow    == out_cost.foot_slow);
    nourstest_true(in_cost.foot_fast    == out_cost.foot_fast);
    nourstest_true(in_cost.mages        == out_cost.mages);
    nourstest_true(in_cost.riders_slow  == out_cost.riders_slow);
    nourstest_true(in_cost.riders_fast  == out_cost.riders_fast);
    nourstest_true(in_cost.fliers       == out_cost.fliers);
    nourstest_true(in_cost.armors       == out_cost.armors);
    nourstest_true(in_cost.pirates      == out_cost.pirates);
    nourstest_true(in_cost.bandits      == out_cost.bandits);
    i32* cost_array =  Tile_Cost_Array(&tile1);
    nourstest_true(cost_array[UNIT_MVT_FOOT_SLOW]     == out_cost.foot_slow);
    nourstest_true(cost_array[UNIT_MVT_FOOT_FAST]     == out_cost.foot_fast);
    nourstest_true(cost_array[UNIT_MVT_MAGES]         == out_cost.mages);
    nourstest_true(cost_array[UNIT_MVT_RIDERS_SLOW]   == out_cost.riders_slow);
    nourstest_true(cost_array[UNIT_MVT_RIDERS_FAST]   == out_cost.riders_fast);
    nourstest_true(cost_array[UNIT_MVT_FLIERS]        == out_cost.fliers);
    nourstest_true(cost_array[UNIT_MVT_ARMORS]        == out_cost.armors);
    nourstest_true(cost_array[UNIT_MVT_PIRATES]       == out_cost.pirates);
    nourstest_true(cost_array[UNIT_MVT_BANDITS]       == out_cost.bandits);
    nourstest_true(out_tile_stats.dodge == in_tile_stats.dodge);
    nourstest_true(out_tile_stats.Pprot == in_tile_stats.Pprot);
    nourstest_true(out_tile_stats.Mprot == in_tile_stats.Mprot);
    nourstest_true(out_tile_stats.heal  == in_tile_stats.heal);
    nourstest_true(tile1.inside == true);
    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "tile_test.json")), &tile1, false);
    jsonio_readJSON(s8_literal(PATH_JOIN("saves", "tile_test.json")), &tile3);
    nourstest_true(tile3.stats.dodge == in_tile_stats.dodge);
    nourstest_true(tile3.stats.Pprot == in_tile_stats.Pprot);
    nourstest_true(tile3.stats.Mprot == in_tile_stats.Mprot);
    nourstest_true(tile3.stats.heal  == in_tile_stats.heal);
    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "tile_rewrite.json")), &tile3, false);
    nourstest_true(Filesystem_fequal(PATH_JOIN("saves", "tile_test.json"),
                                     PATH_JOIN("saves", "tile_rewrite.json")));
    nourstest_true(PHYSFS_exists(PATH_JOIN("saves", "tile_rewrite.json")));
    nourstest_true(PHYSFS_exists(PATH_JOIN("saves", "tile_test.json")));
    nourstest_true(s8equal(tile1.name, tile3.name));
    nourstest_true(tile1.id == tile3.id);
    out_cost        = tile1.cost;
    out_tile_stats  = tile1.stats;
    in_cost         = tile3.cost;
    in_tile_stats   = tile3.stats;
    nourstest_true(in_cost.foot_slow    == out_cost.foot_slow);
    nourstest_true(in_cost.foot_fast    == out_cost.foot_fast);
    nourstest_true(in_cost.mages        == out_cost.mages);
    nourstest_true(in_cost.riders_slow  == out_cost.riders_slow);
    nourstest_true(in_cost.riders_fast  == out_cost.riders_fast);
    nourstest_true(in_cost.fliers       == out_cost.fliers);
    nourstest_true(in_cost.armors       == out_cost.armors);
    nourstest_true(in_cost.pirates      == out_cost.pirates);
    nourstest_true(in_cost.bandits      == out_cost.bandits);
    nourstest_true(out_tile_stats.dodge == in_tile_stats.dodge);
    nourstest_true(out_tile_stats.Pprot == in_tile_stats.Pprot);
    nourstest_true(out_tile_stats.Mprot == in_tile_stats.Mprot);
    nourstest_true(out_tile_stats.heal  == in_tile_stats.heal);
    Tile_Free(&tile3);
}
