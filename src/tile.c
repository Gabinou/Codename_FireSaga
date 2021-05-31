#include "tile.h"
#include "stb_sprintf.h"

struct Chest Chest_default = {
    .item = 0,
    .gold = 5000,
    .toChange_onOpen = NULL,
};

struct Breakable Breakable_default = {
    .toChange_onBreak = NULL,
    .hp = 50,
    .def = 10,
    .res = 10,
};

struct Door Door_default = {
    .chapter_open = 0,
    .toChange_onOpen = NULL,
    .event_onOpen = 0,
    .scene_onOpen = 0,
};

struct Tile Tile_default = {
    .json_element = JSON_TILE,
    // .cost_struct,
    .inside = false,
    // .name,
    .id = -1,
    // .Tile_stats stats,
    // .cost_array
    .breakable = NULL,
    .door = NULL,
    .chest = NULL,
};

void Tile_makeMvtCostarray(struct Tile * in_tile) {
    SDL_Log("Tile_makeMvtCostarray");
    SDL_assert(in_tile != NULL);
    in_tile->cost_array[UNIT_MVT_FOOT_SLOW] = in_tile->cost_struct.foot_slow;
    in_tile->cost_array[UNIT_MVT_FOOT_FAST] = in_tile->cost_struct.foot_fast;
    in_tile->cost_array[UNIT_MVT_MAGES] = in_tile->cost_struct.mages;
    in_tile->cost_array[UNIT_MVT_RIDERS_SLOW] = in_tile->cost_struct.riders_slow;
    in_tile->cost_array[UNIT_MVT_RIDERS_FAST] = in_tile->cost_struct.riders_fast;
    in_tile->cost_array[UNIT_MVT_FLIERS] = in_tile->cost_struct.fliers;
    in_tile->cost_array[UNIT_MVT_ARMORS] = in_tile->cost_struct.armors;
    in_tile->cost_array[UNIT_MVT_PIRATES] = in_tile->cost_struct.pirates;
    in_tile->cost_array[UNIT_MVT_BANDITS] = in_tile->cost_struct.bandits;
}

void Tile_readJSON(struct Tile * in_tile, cJSON * in_jtile) {
    SDL_Log("Tile_readJSON");
    SDL_assert(in_jtile != NULL);
    cJSON * jname = cJSON_GetObjectItemCaseSensitive(in_jtile, "Name");
    cJSON * jid = cJSON_GetObjectItemCaseSensitive(in_jtile, "id");
    cJSON * jinside = cJSON_GetObjectItemCaseSensitive(in_jtile, "inside");
    cJSON * jstats = cJSON_GetObjectItemCaseSensitive(in_jtile, "Stats");
    cJSON * jmvtcost = cJSON_GetObjectItemCaseSensitive(in_jtile, "MvtCost");
    in_tile->id = cJSON_GetNumberValue(jid);
    in_tile->name = cJSON_GetStringValue(jname);
    readJSON_Tilestats(jstats, &(in_tile->stats));
    in_tile->inside = cJSON_IsTrue(jinside);
    readJSON_mvtcost(jmvtcost, &(in_tile->cost_struct));
    Tile_makeMvtCostarray(in_tile);
}

void Tile_writeJSON(struct Tile * in_tile, cJSON * in_jtile) {
    SDL_Log("Tile_writeJSON");
    SDL_assert(in_jtile != NULL);
    cJSON * jtilestats = cJSON_CreateObject();
    cJSON * jcost = cJSON_CreateObject();
    cJSON * jname = cJSON_CreateString(in_tile->name);
    cJSON * jid = cJSON_CreateNumber(in_tile->id);
    writeJSON_mvtcost(jcost, &(in_tile->cost_struct));
    writeJSON_Tilestats(jtilestats, &(in_tile->stats));
    cJSON_AddItemToObject(in_jtile, "Name", jname);
    cJSON_AddItemToObject(in_jtile, "id", jid);
    cJSON_AddBoolToObject(in_jtile, "inside", in_tile->inside);
    cJSON_AddItemToObject(in_jtile, "Stats", jtilestats);
    cJSON_AddItemToObject(in_jtile, "MvtCost", jcost);
}

void Tiles_list_writeJSON(struct Tiles_list * in_tiles_list, cJSON * in_jtiles_list) {
    SDL_assert(in_jtiles_list != NULL);
    cJSON * jposition;
    cJSON * jposx;
    cJSON * jposy;
    cJSON * jtile;
    cJSON * jtile_id;
    cJSON * jnum = cJSON_CreateNumber(in_tiles_list->num);
    cJSON_AddItemToObject(in_jtiles_list, "Tilenum", jtile);
    for (uint8_t i = 0; i < in_tiles_list->num; i++) {
        jposition = cJSON_CreateObject();
        jtile = cJSON_CreateObject();
        jposx = cJSON_CreateNumber(in_tiles_list->pos_relative[i].x);
        jposy = cJSON_CreateNumber(in_tiles_list->pos_relative[i].y);
        jtile_id = cJSON_CreateNumber(in_tiles_list->tiles_index[i]);
        cJSON_AddItemToObject(jtile, "id", jtile_id);
        cJSON_AddItemToObject(jposition, "x", jposx);
        cJSON_AddItemToObject(jposition, "y", jposy);
        cJSON_AddItemToObject(jtile, "Position", jposition);
        cJSON_AddItemToObject(in_jtiles_list, "Tile", jtile);
    }
}

void Breakable_writeJSON(struct Breakable * in_breakable, cJSON * in_jbreakable) {
    SDL_assert(in_jbreakable != NULL);
    cJSON * jtiles_list = cJSON_CreateObject();
    cJSON * jhp = cJSON_CreateNumber(in_breakable->hp);
    cJSON * jdef = cJSON_CreateNumber(in_breakable->def);
    cJSON * jres = cJSON_CreateNumber(in_breakable->res);
    Tiles_list_writeJSON(in_breakable->toChange_onBreak, jtiles_list);
    cJSON_AddItemToObject(in_jbreakable, "Tiles list", jtiles_list);
    cJSON_AddItemToObject(in_jbreakable, "hp", jhp);
    cJSON_AddItemToObject(in_jbreakable, "def", jdef);
    cJSON_AddItemToObject(in_jbreakable, "res", jres);
}

void Door_writeJSON(struct Door * in_door, cJSON * in_jdoor) {
    SDL_assert(in_jdoor != NULL);
    cJSON * jchapter = cJSON_CreateNumber(in_door->chapter_open);
    cJSON * jevent = cJSON_CreateNumber(in_door->event_onOpen);
    cJSON * jscene = cJSON_CreateNumber(in_door->scene_onOpen);
    cJSON * jtiles_list = cJSON_CreateObject();
    Tiles_list_writeJSON(in_door->toChange_onOpen, jtiles_list);
    cJSON_AddItemToObject(in_jdoor, "chapter", jchapter);
    cJSON_AddItemToObject(in_jdoor, "event", jevent);
    cJSON_AddItemToObject(in_jdoor, "scene", jscene);
}

void Chest_writeJSON(struct Chest * in_chest, cJSON * in_jchest) {
    SDL_assert(in_jchest != NULL);
    cJSON * jtiles_list = cJSON_CreateObject();
    cJSON * jgold = cJSON_CreateNumber(in_chest->gold);
    cJSON * jitem = cJSON_CreateNumber(in_chest->item);
    Tiles_list_writeJSON(in_chest->toChange_onOpen, jtiles_list);
    cJSON_AddItemToObject(in_jchest, "Tiles list", jtiles_list);
    cJSON_AddItemToObject(in_jchest, "gold", jgold);
    cJSON_AddItemToObject(in_jchest, "item", jitem);
}

