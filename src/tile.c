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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Mobj_Link_Free(mobj);
    if (mobj->num_linked > 0) {
        mobj->relpos_linked = calloc(mobj->num_linked * TWO_D, sizeof(*mobj->relpos_linked));
        mobj->abspos_linked = calloc(mobj->num_linked * TWO_D, sizeof(*mobj->abspos_linked));
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Mobj_Link_Free(struct Mobj_Link *mobj) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (mobj->relpos_linked != NULL)
        free(mobj->relpos_linked);
    if (mobj->abspos_linked != NULL)
        free(mobj->abspos_linked);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Mobj_Link_writeJSON(const void *input, cJSON *jmobj) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Mobj_Link *mobj = (struct Mobj_Link *) input;
    SDL_assert(jmobj != NULL);
    cJSON *jnum, *jrelpos, *jabspos;
    cJSON *jlinked = cJSON_CreateObject();
    cJSON *jnum_linked = cJSON_CreateNumber(mobj->num_linked);
    jrelpos = cJSON_CreateArray();
    jabspos = cJSON_CreateArray();
    for (size_t i = 0; i < (mobj->num_linked * 2); i++) {
        if (mobj->relpos_linked != NULL) {
            jnum = cJSON_CreateNumber(mobj->relpos_linked[i]);
            cJSON_AddItemToArray(jrelpos, jnum);
        }
        if (mobj->abspos_linked != NULL) {
            jnum = cJSON_CreateNumber(mobj->abspos_linked[i]);
            cJSON_AddItemToArray(jabspos, jnum);
        }
    }
    cJSON_AddItemToObject(jlinked, "absolute position", jabspos);
    cJSON_AddItemToObject(jlinked, "relative position", jrelpos);
    cJSON_AddItemToObject(jlinked, "num", jnum_linked);
    cJSON_AddItemToObject(jmobj, "linked", jlinked);
    cJSON *j2change = cJSON_CreateObject();
    // cJSON * jnum2change = cJSON_CreateNumber(mobj->num2change);
    jrelpos = cJSON_CreateArray();
    jabspos = cJSON_CreateArray();
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Mobj_Link_readJSON(void *input, const cJSON *jmobj) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Mobj_Link *mobj = (struct Mobj_Link *)input;
    cJSON *jlinked = cJSON_GetObjectItemCaseSensitive(jmobj, "linked");
    cJSON *jlinked_num = cJSON_GetObjectItemCaseSensitive(jlinked, "num");
    mobj->num_linked = cJSON_GetNumberValue(jlinked_num);
    cJSON *jabspos;
    cJSON *jrelpos;
    jabspos = cJSON_GetObjectItemCaseSensitive(jlinked, "absolute position");
    Mobj_Link_Init(mobj);
    jrelpos = cJSON_GetObjectItemCaseSensitive(jlinked, "relative position");
    if (mobj->num_linked > 0) {
        Filesystem_readJSON_Array(jrelpos, mobj->relpos_linked);
        Filesystem_readJSON_Array(jabspos, mobj->abspos_linked);
        cJSON *j2change = cJSON_GetObjectItemCaseSensitive(jmobj, "2change");
        cJSON *j2change_num = cJSON_GetObjectItemCaseSensitive(j2change, "num");
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Tile_makeMvtCostarray(struct Tile *tile) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Tile_Free(struct Tile *tile) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (tile->name != NULL) {
        free(tile->name);
        tile->name = NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Tile_readJSON(void *input, const cJSON *const jtile) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Tile *tile = (struct Tile *)input;
    SDL_assert(jtile != NULL);
    cJSON *jname    = cJSON_GetObjectItemCaseSensitive(jtile, "Name");
    cJSON *jid      = cJSON_GetObjectItemCaseSensitive(jtile, "id");
    cJSON *jinside  = cJSON_GetObjectItemCaseSensitive(jtile, "inside");
    cJSON *jstats   = cJSON_GetObjectItemCaseSensitive(jtile, "Stats");
    cJSON *jmvtcost = cJSON_GetObjectItemCaseSensitive(jtile, "MvtCost");
    tile->id = cJSON_GetNumberValue(jid);
    char *temp_str = cJSON_GetStringValue(jname);
    if (temp_str != NULL) {
        tile->name = malloc(strlen(temp_str) + 1);
        strncpy(tile->name, temp_str, strlen(temp_str) + 1);
    }
    Filesystem_readJSON_Tilestats(jstats, &(tile->stats));
    tile->inside = cJSON_IsTrue(jinside);
    Filesystem_readJSON_mvtcost(jmvtcost, &(tile->cost_struct));
    Tile_makeMvtCostarray(tile);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Tile_writeJSON(const void *input, cJSON *jtile) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Tile *tile = (struct Tile *)input;
    SDL_assert(jtile != NULL);
    cJSON *jtilestats   = cJSON_CreateObject();
    cJSON *jcost        = cJSON_CreateObject();
    cJSON *jname        = cJSON_CreateString(tile->name);
    cJSON *jid          = cJSON_CreateNumber(tile->id);
    Filesystem_writeJSON_mvtcost(jcost, &(tile->cost_struct));
    Filesystem_writeJSON_Tilestats(jtilestats, &(tile->stats));
    cJSON_AddItemToObject(jtile, "Name",   jname);
    cJSON_AddItemToObject(jtile, "id",      jid);
    cJSON_AddBoolToObject(jtile, "inside",  tile->inside);
    cJSON_AddItemToObject(jtile, "Stats",   jtilestats);
    cJSON_AddItemToObject(jtile, "MvtCost", jcost);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Breakable_readJSON(void *input, cJSON *jbreakable) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Breakable *breakable = (struct Breakable *) input;
    cJSON *jhp      = cJSON_GetObjectItemCaseSensitive(jbreakable, "hp");
    cJSON *jdef     = cJSON_GetObjectItemCaseSensitive(jbreakable, "def");
    cJSON *jres     = cJSON_GetObjectItemCaseSensitive(jbreakable, "res");
    cJSON *jmobj    = cJSON_GetObjectItemCaseSensitive(jbreakable, "Map Object");
    breakable->hp   = cJSON_GetNumberValue(jhp);
    breakable->def  = cJSON_GetNumberValue(jdef);
    breakable->res  = cJSON_GetNumberValue(jres);
    // Mobj_Link_readJSON(breakable->link, jmobj);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Breakable_writeJSON(const void *input, cJSON *jbreakable) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Breakable *breakable = (struct Breakable *) input;
    SDL_assert(jbreakable != NULL);
    cJSON *jhp  = cJSON_CreateNumber(breakable->hp);
    cJSON *jdef = cJSON_CreateNumber(breakable->def);
    cJSON *jres = cJSON_CreateNumber(breakable->res);
    cJSON_AddItemToObject(jbreakable, "hp",  jhp);
    cJSON_AddItemToObject(jbreakable, "def", jdef);
    cJSON_AddItemToObject(jbreakable, "res", jres);
    cJSON *jmobj = cJSON_CreateObject();
    // Mobj_Link_writeJSON(breakable->link, jmobj);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Door_readJSON(void *input, cJSON *jdoor) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Door *door = (struct Door *) input;
    cJSON *jchapter = cJSON_GetObjectItemCaseSensitive(jdoor, "chapter");
    cJSON *jevent   = cJSON_GetObjectItemCaseSensitive(jdoor, "event");
    cJSON *jscene   = cJSON_GetObjectItemCaseSensitive(jdoor, "scene");
    cJSON *jmobj    = cJSON_GetObjectItemCaseSensitive(jdoor, "Map Object");
    door->chapter_open  = cJSON_GetNumberValue(jchapter);
    door->event         = cJSON_GetNumberValue(jchapter);
    door->scene         = cJSON_GetNumberValue(jscene);
    // Mobj_Link_readJSON(door->link, jmobj);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Door_writeJSON(const void *input, cJSON *jdoor) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Chest_readJSON(void *input, cJSON *jchest) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Chest *chest = (struct Chest *) input;
    cJSON *jgold = cJSON_GetObjectItemCaseSensitive(jchest, "gold");
    chest->gold  = cJSON_GetNumberValue(jgold);
    cJSON *jitem = cJSON_GetObjectItemCaseSensitive(jchest, "item");
    chest->item  = cJSON_GetNumberValue(jitem);
    cJSON *jmobj = cJSON_GetObjectItemCaseSensitive(jchest, "Map Object");
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Chest_writeJSON(const void *input, cJSON *jchest) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Chest *chest = (struct Chest *) input;
    SDL_assert(jchest != NULL);
    cJSON *jgold = cJSON_CreateNumber(chest->gold);
    cJSON *jitem = cJSON_CreateNumber(chest->item);
    cJSON_AddItemToObject(jchest, "gold", jgold);
    cJSON_AddItemToObject(jchest, "item", jitem);
    cJSON *jmobj = cJSON_CreateObject();
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
