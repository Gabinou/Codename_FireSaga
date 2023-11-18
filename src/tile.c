#include "tile.h"

struct Mobj_Link Mobj_Link_default = {
    .num_linked     = 0,
    .relpos_linked  = NULL,
    .abspos_linked  = NULL,
};

struct Chest Chest_default = {
    .item = 0,
    .gold = 5000,
};

struct Breakable Breakable_default = {
    .hp  = 50,
    .def = 10,
    .res = 10,
};

struct Door Door_default = {
    .chapter_open   = 0,
    .chapter_close  = 0,
    .event          = 0,
    .scene          = 0,
};

struct Tile Tile_default = {
    .json_element = JSON_TILE,
    .inside       = false,
    .name         = NULL,
    .id           = -1,
    .stats        = {0},
    .cost_array   = {0},
    .cost_struct  = {0},
};

void Mobj_Link_Init(struct Mobj_Link *mobj) {
    Mobj_Link_Free(mobj);
    if (mobj->num_linked > 0) {
        mobj->relpos_linked = calloc(mobj->num_linked * TWO_D, sizeof(*mobj->relpos_linked));
        mobj->abspos_linked = calloc(mobj->num_linked * TWO_D, sizeof(*mobj->abspos_linked));
    }
}

void Mobj_Link_Free(struct Mobj_Link *mobj) {
    if (mobj->relpos_linked != NULL)
        SDL_free(mobj->relpos_linked);
    if (mobj->abspos_linked != NULL)
        SDL_free(mobj->abspos_linked);
}

void Mobj_Link_writeJSON(void *_input, cJSON *jmobj) {
    struct Mobj_Link *_mobj = (struct Mobj_Link *) _input;
    SDL_assert(jmobj != NULL);
    cJSON *jnum, *jrelpos, *jabspos;
    cJSON *jlinked = cJSON_CreateObject();
    cJSON *jnum_linked = cJSON_CreateNumber(_mobj->num_linked);
    jrelpos = cJSON_CreateArray();
    jabspos = cJSON_CreateArray();
    for (size_t i = 0; i < (_mobj->num_linked * 2); i++) {
        if (_mobj->relpos_linked != NULL) {
            jnum = cJSON_CreateNumber(_mobj->relpos_linked[i]);
            cJSON_AddItemToArray(jrelpos, jnum);
        }
        if (_mobj->abspos_linked != NULL) {
            jnum = cJSON_CreateNumber(_mobj->abspos_linked[i]);
            cJSON_AddItemToArray(jabspos, jnum);
        }
    }
    cJSON_AddItemToObject(jlinked, "absolute position", jabspos);
    cJSON_AddItemToObject(jlinked, "relative position", jrelpos);
    cJSON_AddItemToObject(jlinked, "num", jnum_linked);
    cJSON_AddItemToObject(jmobj, "linked", jlinked);
    cJSON *j2change = cJSON_CreateObject();
    // cJSON * jnum2change = cJSON_CreateNumber(_mobj->num2change);
    jrelpos = cJSON_CreateArray();
    jabspos = cJSON_CreateArray();
}

void Mobj_Link_readJSON(void *input, cJSON *_jmobj) {
    struct Mobj_Link *mobj = (struct Mobj_Link *)input;
    cJSON *jlinked      = cJSON_GetObjectItemCaseSensitive(_jmobj, "linked");
    cJSON *jlinked_num  = cJSON_GetObjectItemCaseSensitive(jlinked, "num");
    mobj->num_linked    = cJSON_GetNumberValue(jlinked_num);
    cJSON *jabspos, *jrelpos;
    jabspos = cJSON_GetObjectItemCaseSensitive(jlinked, "absolute position");
    Mobj_Link_Init(mobj);
    jrelpos = cJSON_GetObjectItemCaseSensitive(jlinked, "relative position");
    if (mobj->num_linked > 0) {
        jsonio_Read_Array(jrelpos, mobj->relpos_linked);
        jsonio_Read_Array(jabspos, mobj->abspos_linked);
        cJSON *j2change     = cJSON_GetObjectItemCaseSensitive(_jmobj,   "2change");
        cJSON *j2change_num = cJSON_GetObjectItemCaseSensitive(j2change, "num");
    }
}

void Tile_makeMvtCostarray(struct Tile *tile) {
    SDL_assert(tile != NULL);
    tile->cost_array[UNIT_MVT_FOOT_SLOW]   = tile->cost_struct.foot_slow;
    tile->cost_array[UNIT_MVT_FOOT_FAST]   = tile->cost_struct.foot_fast;
    tile->cost_array[UNIT_MVT_MAGES]       = tile->cost_struct.mages;
    tile->cost_array[UNIT_MVT_RIDERS_SLOW] = tile->cost_struct.riders_slow;
    tile->cost_array[UNIT_MVT_RIDERS_FAST] = tile->cost_struct.riders_fast;
    tile->cost_array[UNIT_MVT_FLIERS]      = tile->cost_struct.fliers;
    tile->cost_array[UNIT_MVT_ARMORS]      = tile->cost_struct.armors;
    tile->cost_array[UNIT_MVT_PIRATES]     = tile->cost_struct.pirates;
    tile->cost_array[UNIT_MVT_BANDITS]     = tile->cost_struct.bandits;
}

void Tile_Free(struct Tile *tile) {
    if (tile->name != NULL) {
        SDL_free(tile->name);
        tile->name = NULL;
    }
    s8_free(&tile->json_filename);
}

void Tile_readJSON(void *input, cJSON *_jtile) {
    struct Tile *tile = (struct Tile *)input;
    SDL_assert(_jtile != NULL);
    cJSON *jname    = cJSON_GetObjectItemCaseSensitive(_jtile, "Name");
    cJSON *jid      = cJSON_GetObjectItemCaseSensitive(_jtile, "id");
    cJSON *jinside  = cJSON_GetObjectItemCaseSensitive(_jtile, "inside");
    cJSON *jstats   = cJSON_GetObjectItemCaseSensitive(_jtile, "Stats");
    cJSON *jmvtcost = cJSON_GetObjectItemCaseSensitive(_jtile, "MvtCost");
    tile->id = cJSON_GetNumberValue(jid);
    char *temp_str = cJSON_GetStringValue(jname);
    if (temp_str != NULL) {
        tile->name = malloc(strlen(temp_str) + 1);
        memcpy(tile->name, temp_str, strlen(temp_str) + 1);
    }
    jsonio_Read_Tilestats(jstats, &(tile->stats));
    tile->inside = cJSON_IsTrue(jinside);
    jsonio_Read_mvtcost(jmvtcost, &(tile->cost_struct));
    Tile_makeMvtCostarray(tile);
}

void Tile_writeJSON(void *_input, cJSON *jtile) {
    struct Tile *_tile = (struct Tile *)_input;
    SDL_assert(jtile != NULL);
    cJSON *jtilestats   = cJSON_CreateObject();
    cJSON *jcost        = cJSON_CreateObject();
    cJSON *jname        = cJSON_CreateString(_tile->name);
    cJSON *jid          = cJSON_CreateNumber(_tile->id);
    jsonio_Write_mvtcost(jcost, &(_tile->cost_struct));
    jsonio_Write_Tilestats(jtilestats, &(_tile->stats));
    cJSON_AddItemToObject(jtile, "Name",   jname);
    cJSON_AddItemToObject(jtile, "id",      jid);
    cJSON_AddBoolToObject(jtile, "inside",  _tile->inside);
    cJSON_AddItemToObject(jtile, "Stats",   jtilestats);
    cJSON_AddItemToObject(jtile, "MvtCost", jcost);
}

void Breakable_readJSON(void *input, cJSON *_jbreakable) {
    struct Breakable *breakable = (struct Breakable *) input;
    cJSON *jhp      = cJSON_GetObjectItemCaseSensitive(_jbreakable, "hp");
    cJSON *jdef     = cJSON_GetObjectItemCaseSensitive(_jbreakable, "def");
    cJSON *jres     = cJSON_GetObjectItemCaseSensitive(_jbreakable, "res");
    cJSON *jmobj    = cJSON_GetObjectItemCaseSensitive(_jbreakable, "Map Object");
    breakable->hp   = cJSON_GetNumberValue(jhp);
    breakable->def  = cJSON_GetNumberValue(jdef);
    breakable->res  = cJSON_GetNumberValue(jres);
    // Mobj_Link_readJSON(breakable->link, jmobj);
}

void Breakable_writeJSON(void *_input, cJSON *jbreakable) {
    struct Breakable *_breakable = (struct Breakable *) _input;
    SDL_assert(jbreakable != NULL);
    cJSON *jhp  = cJSON_CreateNumber(_breakable->hp);
    cJSON *jdef = cJSON_CreateNumber(_breakable->def);
    cJSON *jres = cJSON_CreateNumber(_breakable->res);
    cJSON_AddItemToObject(jbreakable, "hp",  jhp);
    cJSON_AddItemToObject(jbreakable, "def", jdef);
    cJSON_AddItemToObject(jbreakable, "res", jres);
    cJSON *jmobj = cJSON_CreateObject();
    // Mobj_Link_writeJSON(breakable->link, jmobj);
}

void Door_readJSON(void *input, cJSON *jdoor) {
    struct Door *door = (struct Door *) input;
    cJSON *jchapter = cJSON_GetObjectItemCaseSensitive(jdoor, "chapter");
    cJSON *jevent   = cJSON_GetObjectItemCaseSensitive(jdoor, "event");
    cJSON *jscene   = cJSON_GetObjectItemCaseSensitive(jdoor, "scene");
    cJSON *jmobj    = cJSON_GetObjectItemCaseSensitive(jdoor, "Map Object");
    door->chapter_open  = cJSON_GetNumberValue(jchapter);
    door->event         = cJSON_GetNumberValue(jchapter);
    door->scene         = cJSON_GetNumberValue(jscene);
    // Mobj_Link_readJSON(door->link, jmobj);
}

void Door_writeJSON( void *input, cJSON *jdoor) {
    struct Door *door = (struct Door *) input;
    SDL_assert(jdoor != NULL);
    cJSON *jchapter_open    = cJSON_CreateNumber(door->chapter_open);
    cJSON *jchapter_close   = cJSON_CreateNumber(door->chapter_close);
    cJSON *jevent           = cJSON_CreateNumber(door->event);
    cJSON *jscene           = cJSON_CreateNumber(door->scene);
    cJSON_AddItemToObject(jdoor, "chapter_open",    jchapter_open);
    cJSON_AddItemToObject(jdoor, "chapter_close",   jchapter_close);
    cJSON_AddItemToObject(jdoor, "event",           jevent);
    cJSON_AddItemToObject(jdoor, "scene",           jscene);
    cJSON *jmobj = cJSON_CreateObject();
    // Mobj_Link_writeJSON(door->link, jmobj);
}

void Chest_readJSON(void *input, cJSON *jchest) {
    struct Chest *chest = (struct Chest *) input;
    cJSON *jgold = cJSON_GetObjectItemCaseSensitive(jchest, "gold");
    chest->gold  = cJSON_GetNumberValue(jgold);
    cJSON *jitem = cJSON_GetObjectItemCaseSensitive(jchest, "item");
    chest->item  = cJSON_GetNumberValue(jitem);
    cJSON *jmobj = cJSON_GetObjectItemCaseSensitive(jchest, "Map Object");
}

void Chest_writeJSON( void *input, cJSON *jchest) {
    struct Chest *chest = (struct Chest *) input;
    SDL_assert(jchest != NULL);
    cJSON *jgold = cJSON_CreateNumber(chest->gold);
    cJSON *jitem = cJSON_CreateNumber(chest->item);
    cJSON_AddItemToObject(jchest, "gold", jgold);
    cJSON_AddItemToObject(jchest, "item", jitem);
    cJSON *jmobj = cJSON_CreateObject();
}
