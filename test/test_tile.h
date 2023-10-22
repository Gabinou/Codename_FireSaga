#include "nourstest.h"
#include "tile.h"
#include "jsonio.h"
#include "platform.h"
#include "utilities.h"
#include "narrative.h"

void test_tile() {
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);
    if (PHYSFS_stat("saves", NULL) == 0) {
        PHYSFS_mkdir("saves");
    }
    struct Tile tile2 = Tile_default;
    struct Tile tile3 = Tile_default;
    struct Tile_stats in_tile_stats = {0, 30, 0, 8};
    struct Tile_stats out_tile_stats = Tile_stats_default;
    struct fMovement_cost in_cost = {2.0f, 2.0f, 2.0f, 3.0f, 3.0f, 1.0f, 3.0f, 2.0f, 2.0f};
    struct Tile tile1 =  {
        .json_element = JSON_TILE,
        .cost_struct = in_cost,
        .inside = true,
        .name = "Throne",
        .id = TILE_THRONE,
        .stats = in_tile_stats
    };
    Tile_makeMvtCostarray(&tile1);
    struct fMovement_cost out_cost = fMovement_cost_default;
    u8 cost_array[UNIT_MVT_END];
    nourstest_true(strcmp(tile1.name, "Throne") == 0);
    nourstest_true(tile1.id == TILE_THRONE);
    out_cost        = tile1.cost_struct;
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
    nourstest_true(tile1.cost_array[UNIT_MVT_FOOT_SLOW]     == out_cost.foot_slow);
    nourstest_true(tile1.cost_array[UNIT_MVT_FOOT_FAST]     == out_cost.foot_fast);
    nourstest_true(tile1.cost_array[UNIT_MVT_MAGES]         == out_cost.mages);
    nourstest_true(tile1.cost_array[UNIT_MVT_RIDERS_SLOW]   == out_cost.riders_slow);
    nourstest_true(tile1.cost_array[UNIT_MVT_RIDERS_FAST]   == out_cost.riders_fast);
    nourstest_true(tile1.cost_array[UNIT_MVT_FLIERS]        == out_cost.fliers);
    nourstest_true(tile1.cost_array[UNIT_MVT_ARMORS]        == out_cost.armors);
    nourstest_true(tile1.cost_array[UNIT_MVT_PIRATES]       == out_cost.pirates);
    nourstest_true(tile1.cost_array[UNIT_MVT_BANDITS]       == out_cost.bandits);
    nourstest_true(out_tile_stats.dodge == in_tile_stats.dodge);
    nourstest_true(out_tile_stats.Pprot == in_tile_stats.Pprot);
    nourstest_true(out_tile_stats.Mprot == in_tile_stats.Mprot);
    nourstest_true(out_tile_stats.heal  == in_tile_stats.heal);
    nourstest_true(tile1.inside == true);
    jsonio_writeJSON(PATH_JOIN("saves", "tile_test.json"), &tile1, false);
    jsonio_readJSON(PATH_JOIN("saves", "tile_test.json"), &tile3);
    nourstest_true(tile3.stats.dodge == in_tile_stats.dodge);
    nourstest_true(tile3.stats.Pprot == in_tile_stats.Pprot);
    nourstest_true(tile3.stats.Mprot == in_tile_stats.Mprot);
    nourstest_true(tile3.stats.heal  == in_tile_stats.heal);
    jsonio_writeJSON(PATH_JOIN("saves", "tile_rewrite.json"), &tile3, false);
    nourstest_true(Filesystem_fequal(PATH_JOIN("saves", "tile_test.json"),
                                     PATH_JOIN("saves", "tile_rewrite.json")));
    nourstest_true(PHYSFS_exists(PATH_JOIN("saves", "tile_rewrite.json")));
    nourstest_true(PHYSFS_exists(PATH_JOIN("saves", "tile_test.json")));
    nourstest_true(strcmp(tile1.name, tile3.name) == 0);
    nourstest_true(tile1.id == tile3.id);
    memcpy(cost_array, tile1.cost_array, sizeof(*cost_array) * (UNIT_MVT_END));
    out_cost        = tile1.cost_struct;
    out_tile_stats  = tile1.stats;
    in_cost         = tile3.cost_struct;
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
