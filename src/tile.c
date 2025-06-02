#include "tile.h"
#include "cJSON.h"
#include "jsonio.h"
#include "names.h"

const struct Mobj_Link Mobj_Link_default = {0};

const struct Chest Chest_default = {
    .gold = 5000,
};

const struct Breakable Breakable_default = {
    .hp  = 50,
    .def = 10,
    .res = 10,
};

const struct Door Door_default = {0};

const struct Tile Tile_default = {
    .jsonio_header.json_element   = JSON_TILE,
};

s8 Tile_Name(struct Tile *tile) {
    return (global_tilenames[tile->id]);
}

void Mobj_Link_Init_tnecs(void *voidmobj) {
    Mobj_Link_Init(voidmobj);
}
void Mobj_Link_Free_tnecs(void *voidmobj) {
    Mobj_Link_Free(voidmobj);
}

void Mobj_Link_Init(struct Mobj_Link *mobj) {
    Mobj_Link_Free(mobj);
    if (mobj->num_linked > 0) {
        mobj->relpos_linked = SDL_calloc(mobj->num_linked * TWO_D, sizeof(*mobj->relpos_linked));
        mobj->abspos_linked = SDL_calloc(mobj->num_linked * TWO_D, sizeof(*mobj->abspos_linked));
    }
}

void Mobj_Link_Free(struct Mobj_Link *mobj) {
    if (mobj->relpos_linked != NULL)
        SDL_free(mobj->relpos_linked);
    if (mobj->abspos_linked != NULL)
        SDL_free(mobj->abspos_linked);
}

void Mobj_Link_writeJSON(const void *_input, cJSON *jmobj) {
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
    // cJSON *j2change = cJSON_CreateObject();
    // cJSON * jnum2change = cJSON_CreateNumber(_mobj->num2change);
    jrelpos = cJSON_CreateArray();
    jabspos = cJSON_CreateArray();
}

void Tile_stats_readJSON(void *input, const struct cJSON      *_jstats) {
    struct Tile_stats *stats = input;
    SDL_assert(_jstats != NULL);
    struct cJSON *jdodge = cJSON_GetObjectItem(_jstats, "Dodge");
    struct cJSON *jPprot = cJSON_GetObjectItem(_jstats, "Pprot");
    struct cJSON *jMprot = cJSON_GetObjectItem(_jstats, "Mprot");
    struct cJSON *jHeal  = cJSON_GetObjectItem(_jstats, "Heal");
    if (jdodge)
        stats->dodge     = cJSON_GetNumberValue(jdodge);
    if (jPprot)
        stats->Pprot     = cJSON_GetNumberValue(jPprot);
    if (jMprot)
        stats->Mprot     = cJSON_GetNumberValue(jMprot);
    if (jHeal)
        stats->heal      = cJSON_GetNumberValue(jHeal);
}

void Mobj_Link_readJSON(void *input, const cJSON *_jmobj) {
    struct Mobj_Link *mobj = (struct Mobj_Link *)input;
    cJSON *jlinked      = cJSON_GetObjectItemCaseSensitive(_jmobj, "linked");
    cJSON *jlinked_num  = cJSON_GetObjectItemCaseSensitive(jlinked, "num");
    mobj->num_linked    = cJSON_GetNumberValue(jlinked_num);
    cJSON *jabspos, *jrelpos;
    jabspos = cJSON_GetObjectItemCaseSensitive(jlinked, "absolute position");
    Mobj_Link_Init(mobj);
    jrelpos = cJSON_GetObjectItemCaseSensitive(jlinked, "relative position");
    if (mobj->num_linked > 0) {
        Array_readJSON(jrelpos, mobj->relpos_linked);
        Array_readJSON(jabspos, mobj->abspos_linked);
        // cJSON *j2change     = cJSON_GetObjectItemCaseSensitive(_jmobj,   "2change");
        // cJSON *j2change_num = cJSON_GetObjectItemCaseSensitive(j2change, "num");
    }
}

i32* Tile_Cost_Array(struct Tile *tile) {
    /* -1 cause 0 is NULL index */
    return ((i32*)&tile->cost - 1);
}
void Tile_Free(struct Tile *tile) {
    s8_free(&tile->jsonio_header.json_filename);
}

void Tile_readJSON(void *input, const cJSON *_jtile) {
    struct Tile *tile = (struct Tile *)input;
    SDL_assert(_jtile != NULL);
    cJSON *jid      = cJSON_GetObjectItemCaseSensitive(_jtile, "id");
    cJSON *jindoors  = cJSON_GetObjectItemCaseSensitive(_jtile, "indoors");
    cJSON *jstats   = cJSON_GetObjectItemCaseSensitive(_jtile, "Stats");
    cJSON *jmvtcost = cJSON_GetObjectItemCaseSensitive(_jtile, "MvtCost");
    tile->id = cJSON_GetNumberValue(jid);
    Tile_stats_readJSON(&(tile->stats), jstats);
    tile->indoors = cJSON_IsTrue(jindoors);
    Movement_cost_readJSON(&(tile->cost), jmvtcost);
}

void Tile_writeJSON(const void *_input, cJSON *jtile) {
    struct Tile *_tile = (struct Tile *)_input;
    SDL_assert(jtile != NULL);
    cJSON *jtilestats   = cJSON_CreateObject();
    cJSON *jcost        = cJSON_CreateObject();
    cJSON *jid          = cJSON_CreateNumber(_tile->id);
    Movement_cost_writeJSON(&(_tile->cost), jcost);
    Tile_stats_writeJSON(&(_tile->stats), jtilestats);
    s8 name = global_tilenames[_tile->id];
    if (s8valid(name)) {
        cJSON *jname = cJSON_CreateString(name.data);
        cJSON_AddItemToObject(jtile, "Name",   jname);
    }

    cJSON_AddItemToObject(jtile, "id",      jid);
    cJSON_AddBoolToObject(jtile, "indoors",  _tile->indoors);
    cJSON_AddItemToObject(jtile, "Stats",   jtilestats);
    cJSON_AddItemToObject(jtile, "MvtCost", jcost);
}

void Breakable_readJSON(void *input, const cJSON *_jbreakable) {
    struct Breakable *breakable = (struct Breakable *) input;
    cJSON *jhp      = cJSON_GetObjectItemCaseSensitive(_jbreakable, "hp");
    cJSON *jdef     = cJSON_GetObjectItemCaseSensitive(_jbreakable, "def");
    cJSON *jres     = cJSON_GetObjectItemCaseSensitive(_jbreakable, "res");
    // cJSON *jmobj    = cJSON_GetObjectItemCaseSensitive(_jbreakable, "Map Object");
    breakable->hp   = cJSON_GetNumberValue(jhp);
    breakable->def  = cJSON_GetNumberValue(jdef);
    breakable->res  = cJSON_GetNumberValue(jres);
    // Mobj_Link_readJSON(breakable->link, jmobj);
}

void Breakable_writeJSON(const void *_input, cJSON *jbreakable) {
    struct Breakable *_breakable = (struct Breakable *) _input;
    SDL_assert(jbreakable != NULL);
    cJSON *jhp  = cJSON_CreateNumber(_breakable->hp);
    cJSON *jdef = cJSON_CreateNumber(_breakable->def);
    cJSON *jres = cJSON_CreateNumber(_breakable->res);
    cJSON_AddItemToObject(jbreakable, "hp",  jhp);
    cJSON_AddItemToObject(jbreakable, "def", jdef);
    cJSON_AddItemToObject(jbreakable, "res", jres);
    // cJSON *jmobj = cJSON_CreateObject();
    // Mobj_Link_writeJSON(breakable->link, jmobj);
}

void Door_readJSON(void *input, const cJSON *jdoor) {
    struct Door *door = (struct Door *) input;

    cJSON *jtile            = cJSON_GetObjectItem(jdoor, "tile");
    cJSON *jchapter_open    = cJSON_GetObjectItem(jdoor, "chapter_open");
    cJSON *jchapter_close   = cJSON_GetObjectItem(jdoor, "chapter_close");
    cJSON *jevent           = cJSON_GetObjectItemCaseSensitive(jdoor, "event");
    cJSON *jscene           = cJSON_GetObjectItemCaseSensitive(jdoor, "scene");
    // cJSON *jmobj            = cJSON_GetObjectItemCaseSensitive(jdoor, "Map Object");

    // door->chapter_open  = cJSON_GetNumberValue(jchapter);
    // door->event         = cJSON_GetNumberValue(jchapter);
    // door->narrative.scene         = cJSON_GetNumberValue(jscene);
    // Mobj_Link_readJSON(door->link, jmobj);
    if (jchapter_open != NULL)
        door->chapter_open  = cJSON_GetNumberValue(jchapter_open);
    if (jchapter_close != NULL)
        door->chapter_close = cJSON_GetNumberValue(jchapter_close);
    if (jevent != NULL)
        door->event         = cJSON_GetNumberValue(jevent);
    if (jscene != NULL)
        door->scene         = cJSON_GetNumberValue(jscene);
    if (jtile != NULL)
        door->tile          = cJSON_GetNumberValue(jtile);
}

void Door_writeJSON(const void *input, cJSON *jdoor) {
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
    // cJSON *jmobj = cJSON_CreateObject();
    // Mobj_Link_writeJSON(door->link, jmobj);
}

void Chest_readJSON(void *input, const cJSON *jchest) {
    struct Chest *chest = (struct Chest *) input;
    SDL_assert(chest    != NULL);
    SDL_assert(jchest   != NULL);
    // TODO: Cleanup
    //     struct cJSON *jtile = cJSON_GetObjectItem(_jchest, "tile");

    //     if (jtile != NULL)
    //         chest->tile = cJSON_GetNumberValue(jtile);
    //         chest->gold = cJSON_GetNumberValue(jgold);
    //         char *name  = cJSON_GetStringValue(jitem);
    //         // if (name != NULL)
    //         // chest->item = Hashes_itemName2ID(name);
    //     }
    // }

    cJSON *jgold = cJSON_GetObjectItemCaseSensitive(jchest, "gold");
    cJSON *jitem = cJSON_GetObjectItemCaseSensitive(jchest, "item");
    // cJSON *jmobj = cJSON_GetObjectItemCaseSensitive(jchest, "Map Object");

    if (jgold != NULL)
        chest->gold  = cJSON_GetNumberValue(jgold);
    if (jitem != NULL)
        chest->item  = cJSON_GetNumberValue(jitem);
}

void Chest_writeJSON(const void *input, cJSON *jchest) {
    struct Chest *chest = (struct Chest *) input;
    SDL_assert(jchest != NULL);
    cJSON *jgold = cJSON_CreateNumber(chest->gold);
    cJSON *jitem = cJSON_CreateNumber(chest->item);
    cJSON_AddItemToObject(jchest, "gold", jgold);
    cJSON_AddItemToObject(jchest, "item", jitem);
    // cJSON *jmobj = cJSON_CreateObject();
}

b32 Tile_Valid_ID(u8 id) {
    return ((id < TILE_ID_MAX) && (id < TILE_END) && (id > TILE_START));
}
