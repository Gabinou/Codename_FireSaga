
#include "jsonio.h"

json_func json_read_funcs [JSON_END] = {
    /* JSON_NULL        */  NULL,
    /* JSON_ITEM        */  Item_readJSON,
    /* JSON_TILE        */  Tile_readJSON,
    /* JSON_UNIT        */  Unit_readJSON,
    /* JSON_WEAPON      */  Weapon_readJSON,
    /* JSON_MAP         */  Map_readJSON,
    /* JSON_SCENE       */  Scene_readJSON,
    /* JSON_CONVOY      */  Convoy_readJSON,
    /* JSON_NARRATIVE   */  NULL,
    /* JSON_CAMP        */  Camp_readJSON,
    /* JSON_SPRITE      */  Sprite_readJSON,
    /* JSON_AI          */  AI_readJSON,
    /* JSON_PARTY       */  Party_readJSON,
};

json_func json_write_funcs[JSON_END] = {
    /* JSON_NULL        */  NULL,
    /* JSON_ITEM        */  Item_writeJSON,
    /* JSON_TILE        */  Tile_writeJSON,
    /* JSON_UNIT        */  Unit_writeJSON,
    /* JSON_WEAPON      */  Weapon_writeJSON,
    /* JSON_MAP         */  Map_writeJSON,
    /* JSON_SCENE       */  NULL,
    /* JSON_CONVOY      */  Convoy_writeJSON,
    /* JSON_NARRATIVE   */  NULL,
    /* JSON_CAMP        */  Camp_writeJSON,
    /* JSON_SPRITESHEET */  NULL,
    /* JSON_AI          */  AI_writeJSON,
    /* JSON_PARTY       */  NULL,
};

struct cJSON *jsonio_parseJSON(s8 filename) {
    /* Error if file doesn't exist */
    if (!PHYSFS_exists(filename.data)) {
        SDL_Log("File %s does not exist", filename.data);
        exit(ERROR_CannotOpenFile);
    }

    /* Error if file can't be read */
    PHYSFS_file *fp = PHYSFS_openRead(filename.data);
    if (fp == NULL) {
        SDL_Log("%s " STRINGIZE(__LINE__), __func__);
        SDL_Log("Could not read JSON file");
        exit(ERROR_CannotOpenFile);
    }

    /* Read the file's bytes */
    uint32_t filelen = PHYSFS_fileLength(fp);
    char filebuffer[filelen];
    PHYSFS_readBytes(fp, filebuffer, filelen);
    PHYSFS_close(fp);

    /* Actually parse the JSON */
    struct cJSON *jfile = cJSON_ParseWithLength(filebuffer, filelen);

    return (jfile);
}

void jsonio_readJSON(s8 filename, void *struct_ptr) {
    SDL_Log("Reading JSON: '%s'", filename.data);

    /* Make mutable filename */
    char *string = filename.data;
    s8 filename_mut = s8_mut(filename.data);
    SDL_assert(filename_mut.num == filename.num);

    /* Parse the json file */
    struct cJSON *jfile = jsonio_parseJSON(filename);
    if (jfile == NULL) {
        SDL_Log("Could not parse JSON file '%s'", filename.data);
        exit(ERROR_JSONElementNotSet);
    }

    /* Get the json element id */
    u8 *byte_ptr    = (u8 *)struct_ptr;
    u8 jelem_id     = *(byte_ptr + JSON_ELEM_bOFFSET);

    s8 elem_name = jsonElementnames[jelem_id];
    SDL_Log("Reading JSON element %d %s", jelem_id, elem_name.data);
    if (jelem_id >= JSON_END) {
        SDL_Log("JSON element not set");
        exit(ERROR_JSONElementNotSet);
    }

    /* Get the json element */
    struct cJSON *jelement = cJSON_GetObjectItem(jfile, elem_name.data);
    if (jelement == NULL) {
        SDL_Log("JSON element %s does not exist in %s", elem_name.data, filename.data);
        exit(ERROR_JSONElementNotSet);
    }

    /* Actually read the json file */
    SDL_assert(json_read_funcs[jelem_id] != NULL);
    json_read_funcs[jelem_id](struct_ptr, jelement);

    /* Set json_filename in struct to input filename */
    byte_ptr            = (u8 *)struct_ptr;
    s8 *json_filename   = (s8 *)(byte_ptr + JSON_FILENAME_bOFFSET);

    if (json_filename->data == NULL)
        *json_filename = filename_mut;

    /* Clean the jfile */
    if (jfile != NULL)
        cJSON_Delete(jfile);
}

void jsonio_writeJSON(s8 filename, void *struct_ptr, b32 append) {
    SDL_Log("%s:", filename.data);

    /* Parse the json file */
    PHYSFS_file *fp     = NULL;
    struct cJSON *json  = cJSON_CreateObject();

    /* Get the json element id */
    u8 *byte_ptr    = (u8 *)struct_ptr;
    u8 jelem_id     = *(byte_ptr + JSON_ELEM_bOFFSET);

    s8 elem_name = jsonElementnames[jelem_id];
    SDL_Log("Writing JSON element %s", elem_name.data);
    if (jelem_id >= JSON_END) {
        SDL_Log("JSON element not set");
        exit(ERROR_JSONElementNotSet);
    }

    /* Create the json element */
    struct cJSON *jelement = cJSON_CreateObject();

    /* Open the file */
    if (append)
        fp = PHYSFS_openAppend(filename.data);
    else
        fp = PHYSFS_openWrite(filename.data);

    if (!fp) {
        SDL_Log("Could not open %s for writing\n", filename.data);
        exit(ERROR_CannotOpenFile);
    }

    /* Write to the json element */
    SDL_assert(json_write_funcs[jelem_id] != NULL);
    json_write_funcs[jelem_id](struct_ptr, jelement);

    /* Actually write to the file */
    cJSON_AddItemToObject(json, elem_name.data, jelement);
    jsonio_Print(fp, json);

    /* Clean the file */
    PHYSFS_close(fp);
    if (json != NULL)
        cJSON_Delete(json);
}

/* --- UTILITIES --- */
/* -- Read -- */
// TODO: rewrite with input cjson
void Shop_readJSON(char *filename, struct Shop *shop) {
    SDL_Log("%s", filename);
    struct cJSON *jfile = jsonio_parseJSON(s8_var(filename));
    SDL_assert(jfile != NULL);
    struct cJSON *jshop = cJSON_GetObjectItem(jfile, "Shop");
    if (jshop == NULL) {
        SDL_Log("No Shop element in shop json");
        exit(ERROR_JSONParsingFailed);
    }
    struct cJSON *jitems = cJSON_GetObjectItem(jshop, "Items");
    if (jitems == NULL) {
        SDL_Log("No Items array in shop json");
        exit(ERROR_JSONParsingFailed);
    }
    size_t items_num = cJSON_GetArraySize(jitems);
    shop->qty = calloc(items_num, sizeof(*shop->qty));
    shop->items = calloc(items_num, sizeof(*shop->items));
    for (int i = 0; i < items_num; i++) {
        struct cJSON *jitem = cJSON_GetArrayItem(jitems, i);
        shop->qty[i]        = cJSON_GetNumberValue(jitem);
        // shop->items[i]      = Hashes_itemName2ID(jitem->string);
    }
    struct cJSON *jshopkeeper = cJSON_GetObjectItem(jshop, "Shopkeeper");
    // shop->shopkeeper = Hashes_shopkeeperName2ID(cJSON_GetStringValue(jshopkeeper));
    if (jfile != NULL)
        cJSON_Delete(jfile);
}

void Promotion_readJSON(char *filename, struct Promotion *promotion) {
    SDL_Log("%s", filename);
    struct cJSON *jfile = jsonio_parseJSON(s8_var(filename));
    SDL_assert(jfile != NULL);
    struct cJSON *jpromotion = cJSON_GetObjectItem(jfile, "Promotion");
    if (jpromotion == NULL) {
        SDL_Log("No Promotion element in promotion json");
        exit(ERROR_JSONParsingFailed);
    }
    struct cJSON *jstats = cJSON_GetObjectItem(jpromotion, "Stats");
    if (jstats == NULL) {
        SDL_Log("No Stats element in promotion json");
        exit(ERROR_JSONParsingFailed);
    }
    Unit_stats_readJSON(&promotion->bonus, jstats);
    struct cJSON *jskill = cJSON_GetObjectItem(jpromotion, "Skill");
    if (jskill != NULL) {
        struct cJSON *jname = cJSON_GetObjectItem(jskill, "Name");
        if (jname == NULL) {
            SDL_Log("No Name element in skills element of promotion json");
            exit(ERROR_JSONParsingFailed);
        }
        // promotion->skill = Hashes_skillName2ID(cJSON_GetStringValue(jname));

        struct cJSON *jlvl = cJSON_GetObjectItem(jskill, "level");
        if (jlvl != NULL)
            promotion->level = cJSON_GetNumberValue(jlvl);
    }

    if (jfile != NULL)
        cJSON_Delete(jfile);

}

void Palette_readJSON(char *filename, struct SDL_Palette *palette) {
    SDL_Log("%s", filename);
    struct cJSON *jfile = jsonio_parseJSON(s8_var(filename));
    SDL_assert(jfile != NULL);
    struct cJSON *jpalette = cJSON_GetObjectItem(jfile, "Palette");
    if (jpalette == NULL) {
        SDL_Log("No Palette element in palette json");
        exit(ERROR_JSONParsingFailed);
    }
    struct cJSON *jrgbs = cJSON_GetObjectItem(jpalette, "rgb");
    if (jrgbs == NULL) {
        SDL_Log("No rgb array in palette json");
        exit(ERROR_JSONParsingFailed);
    }

    size_t colors_num = cJSON_GetArraySize(jrgbs);
    if (colors_num != palette->ncolors) {
        SDL_Log("Invalid number of colors in Palette");
        exit(ERROR_JSONParsingFailed);
    }

    for (int i = 0; i < colors_num; i++) {
        struct cJSON *jrgb   = cJSON_GetArrayItem(jrgbs, i);
        palette->colors[i].r = cJSON_GetNumberValue(cJSON_GetArrayItem(jrgb, 0));
        palette->colors[i].g = cJSON_GetNumberValue(cJSON_GetArrayItem(jrgb, 1));
        palette->colors[i].b = cJSON_GetNumberValue(cJSON_GetArrayItem(jrgb, 2));
        palette->colors[i].a = 0;
    }
    if (jfile != NULL)
        cJSON_Delete(jfile);

}

void PaletteTable_readJSON(char *filename, u8 *table) {
    SDL_Log("%s", filename);
    struct cJSON *jfile = jsonio_parseJSON(s8_var(filename));
    SDL_assert(jfile != NULL);
    struct cJSON *jpalette_table = cJSON_GetObjectItem(jfile, "Palette Table");
    if (jpalette_table == NULL) {
        SDL_Log("No 'Palette Table' element in palette table json");
        exit(ERROR_JSONParsingFailed);
    }

    struct cJSON *jfrom_tos = cJSON_GetObjectItem(jpalette_table, "from_to");
    if (jfrom_tos == NULL) {
        SDL_Log("No from_to array in palette table json");
        exit(ERROR_JSONParsingFailed);
    }

    size_t colors_num = cJSON_GetArraySize(jfrom_tos);
    // TODO: Check number of colors
    // if (colors_num != PALETTE_NES_COLOR_NUM) {
    //     SDL_Log("Invalid number of colors in Palette Table");
    //     exit(ERROR_JSONParsingFailed);
    // }

    for (int i = 0; i < colors_num; i++) {
        struct cJSON *jfrom_to =  cJSON_GetArrayItem(jfrom_tos, i);
        table[i] = cJSON_GetNumberValue(jfrom_to);
    }
    if (jfile != NULL)
        cJSON_Delete(jfile);
}

void Point_writeJSON(void *input, struct cJSON *jpos) {
    struct Point *pos = input;

    struct cJSON *jnumx = cJSON_CreateNumber(pos->x);
    struct cJSON *jnumy = cJSON_CreateNumber(pos->y);
    cJSON_AddItemToObject(jpos, "col", jnumx);
    cJSON_AddItemToObject(jpos, "row", jnumy);
}

void Point_readJSON(void *input, struct cJSON *_jpos) {
    struct Point *pos = input;

    SDL_assert(_jpos != NULL);
    SDL_assert(pos != NULL);
    if (cJSON_IsArray(_jpos)) {
        pos->x = cJSON_GetNumberValue(cJSON_GetArrayItem(_jpos, 0));
        pos->y = cJSON_GetNumberValue(cJSON_GetArrayItem(_jpos, 1));
    } else {
        struct cJSON *jcol = cJSON_GetObjectItem(_jpos, "col");
        struct cJSON *jrow = cJSON_GetObjectItem(_jpos, "row");
        if (jcol != NULL)
            pos->x = cJSON_GetNumberValue(jcol);
        if (jrow != NULL)
            pos->y = cJSON_GetNumberValue(jrow);
    }
}

void fMovement_cost_readJSON(void *input, struct cJSON *_jcost) {
    struct fMovement_cost *cost = input;
    SDL_assert(cost != NULL);
    struct cJSON *jmages       = cJSON_GetObjectItem(_jcost, "mages");
    struct cJSON *jfliers      = cJSON_GetObjectItem(_jcost, "fliers");
    struct cJSON *jarmors      = cJSON_GetObjectItem(_jcost, "armors");
    struct cJSON *jpirates     = cJSON_GetObjectItem(_jcost, "pirates");
    struct cJSON *jbandits     = cJSON_GetObjectItem(_jcost, "bandits");
    struct cJSON *jfoot_slow   = cJSON_GetObjectItem(_jcost, "foot_slow");
    struct cJSON *jfoot_fast   = cJSON_GetObjectItem(_jcost, "foot_fast");
    struct cJSON *jriders_slow = cJSON_GetObjectItem(_jcost, "riders_slow");
    struct cJSON *jriders_fast = cJSON_GetObjectItem(_jcost, "riders_fast");
    cost->mages       = cJSON_GetNumberValue(jmages);
    cost->fliers      = cJSON_GetNumberValue(jfliers);
    cost->armors      = cJSON_GetNumberValue(jarmors);
    cost->pirates     = cJSON_GetNumberValue(jpirates);
    cost->bandits     = cJSON_GetNumberValue(jbandits);
    cost->foot_slow   = cJSON_GetNumberValue(jfoot_slow);
    cost->foot_fast   = cJSON_GetNumberValue(jfoot_fast);
    cost->riders_slow = cJSON_GetNumberValue(jriders_slow);
    cost->riders_fast = cJSON_GetNumberValue(jriders_fast);
}

void Array_readJSON(struct cJSON *_jarray, i32 *array) {
    SDL_assert(array != NULL);
    struct cJSON *jnum;
    size_t i = 0;
    cJSON_ArrayForEach(jnum, _jarray) {
        array[i++] = (i32)cJSON_GetNumberValue(jnum);
    }
}

void Array2D_readJSON(struct cJSON *_jarr, i32 *arr2D,
                      u8 row_len, u8 col_len) {
    /* caller deals with memory */
    SDL_assert(_jarr != NULL);
    SDL_assert(cJSON_IsArray(_jarr));
    SDL_assert(cJSON_GetArraySize(_jarr) == row_len);
    for (u8 row = 0; row < cJSON_GetArraySize(_jarr); row++) {
        struct cJSON *jrow  = cJSON_GetArrayItem(_jarr, row);
        SDL_assert(cJSON_GetArraySize(jrow) == col_len);
        for (u8 col = 0; col < cJSON_GetArraySize(jrow); col++) {
            struct cJSON *jnum  = cJSON_GetArrayItem(jrow, col);
            arr2D[(row * col_len + col)] = (i32)cJSON_GetNumberValue(jnum);
        }
    }
}

/* -- Write -- */
void Array_writeJSON(struct cJSON *_jarr, i32 *arr, size_t num) {
    SDL_Log("%zu", num);
    SDL_assert(arr != NULL);
    for (u8 i = 0; i < num; i++) {
        struct cJSON *jnum = cJSON_CreateNumber(arr[i]);
        cJSON_AddItemToArray(_jarr, jnum);
    }
}

void Array2D_writeJSON(struct cJSON *arr, i32 *arr2D, u8 row_len, u8 col_len) {
    SDL_assert(arr != NULL);
    struct cJSON *jrow, *jnum;
    char rowname[DEFAULT_BUFFER_SIZE / 8];
    for (u8 row = 0; row < row_len; row++) {
        jrow = cJSON_CreateArray();
        for (u8 col = 0; col < col_len; col++) {
            jnum = cJSON_CreateNumber(arr2D[(row * col_len + col)]);
            cJSON_AddItemToArray(jrow, jnum);
        }
        stbsp_sprintf(rowname, "row %d", row);
        cJSON_AddItemToObject(arr, rowname, jrow);
    }
}

void fMovement_cost_writeJSON(void *input, struct cJSON *_jcost) {
    struct fMovement_cost *_cost = input;
    SDL_assert(_jcost != NULL);
    struct cJSON *jmages       = cJSON_CreateNumber(_cost->mages);
    struct cJSON *jfliers      = cJSON_CreateNumber(_cost->fliers);
    struct cJSON *jarmors      = cJSON_CreateNumber(_cost->armors);
    struct cJSON *jpirates     = cJSON_CreateNumber(_cost->pirates);
    struct cJSON *jbandits     = cJSON_CreateNumber(_cost->bandits);
    struct cJSON *jfoot_slow   = cJSON_CreateNumber(_cost->foot_slow);
    struct cJSON *jfoot_fast   = cJSON_CreateNumber(_cost->foot_fast);
    struct cJSON *jriders_slow = cJSON_CreateNumber(_cost->riders_slow);
    struct cJSON *jriders_fast = cJSON_CreateNumber(_cost->riders_fast);
    cJSON_AddItemToObject(_jcost, "mages",       jmages);
    cJSON_AddItemToObject(_jcost, "fliers",      jfliers);
    cJSON_AddItemToObject(_jcost, "armors",      jarmors);
    cJSON_AddItemToObject(_jcost, "pirates",     jpirates);
    cJSON_AddItemToObject(_jcost, "bandits",     jbandits);
    cJSON_AddItemToObject(_jcost, "foot_slow",   jfoot_slow);
    cJSON_AddItemToObject(_jcost, "foot_fast",   jfoot_fast);
    cJSON_AddItemToObject(_jcost, "riders_slow", jriders_slow);
    cJSON_AddItemToObject(_jcost, "riders_fast", jriders_fast);
}

void Tile_stats_writeJSON(void *input, struct cJSON *jstats) {
    struct Tile_stats *stats = input;
    SDL_assert(jstats != NULL);
    struct cJSON *jdodge = cJSON_CreateNumber(stats->dodge);
    struct cJSON *jPprot = cJSON_CreateNumber(stats->Pprot);
    struct cJSON *jMprot = cJSON_CreateNumber(stats->Mprot);
    struct cJSON *jheal  = cJSON_CreateNumber(stats->heal);
    cJSON_AddItemToObject(jstats, "Dodge", jdodge);
    cJSON_AddItemToObject(jstats, "Pprot", jPprot);
    cJSON_AddItemToObject(jstats, "Mprot", jMprot);
    cJSON_AddItemToObject(jstats, "Heal",  jheal);
}

void Unit_stats_writeJSON(void *input, struct cJSON *jstats) {
    struct Unit_stats *stats = input;
    SDL_assert(jstats != NULL);
    struct cJSON *php   = cJSON_CreateNumber(stats->hp);
    struct cJSON *pstr  = cJSON_CreateNumber(stats->str);
    struct cJSON *pmag  = cJSON_CreateNumber(stats->mag);
    struct cJSON *pagi  = cJSON_CreateNumber(stats->agi);
    struct cJSON *pdex  = cJSON_CreateNumber(stats->dex);
    struct cJSON *pluck = cJSON_CreateNumber(stats->luck);
    struct cJSON *pfth  = cJSON_CreateNumber(stats->fth);
    struct cJSON *pdef  = cJSON_CreateNumber(stats->def);
    struct cJSON *pres  = cJSON_CreateNumber(stats->res);
    struct cJSON *pcon  = cJSON_CreateNumber(stats->con);
    struct cJSON *pmove = cJSON_CreateNumber(stats->move);
    struct cJSON *pprof = cJSON_CreateNumber(stats->prof);
    cJSON_AddItemToObject(jstats, "hp",   php);
    cJSON_AddItemToObject(jstats, "str",  pstr);
    cJSON_AddItemToObject(jstats, "mag",  pmag);
    cJSON_AddItemToObject(jstats, "agi",  pagi);
    cJSON_AddItemToObject(jstats, "dex",  pdex);
    cJSON_AddItemToObject(jstats, "luck", pluck);
    cJSON_AddItemToObject(jstats, "fth",  pfth);
    cJSON_AddItemToObject(jstats, "def",  pdef);
    cJSON_AddItemToObject(jstats, "res",  pres);
    cJSON_AddItemToObject(jstats, "con",  pcon);
    cJSON_AddItemToObject(jstats, "move", pmove);
    cJSON_AddItemToObject(jstats, "prof", pprof);
}

void Item_stats_writeJSON(void *input, struct cJSON *jstats) {
    struct Item_stats *stats = input;
    SDL_assert(jstats != NULL);
    struct cJSON *jAP    = cJSON_CreateNumber(stats->AP);
    struct cJSON *juses  = cJSON_CreateNumber(stats->uses);
    struct cJSON *jprice = cJSON_CreateNumber(stats->price);
    cJSON_AddItemToObject(jstats, "uses", juses);
    cJSON_AddItemToObject(jstats, "price", jprice);
    cJSON_AddItemToObject(jstats, "AP", jAP);
}

void Weapon_stats_writeJSON(void *input, struct cJSON *jstats) {
    struct Weapon_stats *stats = input;
    SDL_assert(jstats != NULL);
    struct cJSON *pprot   = cJSON_CreateArray();
    struct cJSON *prange  = cJSON_CreateArray();
    struct cJSON *pattack = cJSON_CreateArray();
    cJSON_AddItemToArray(pprot,   cJSON_CreateNumber(stats->protection[DAMAGE_TYPE_PHYSICAL]));
    cJSON_AddItemToArray(pprot,   cJSON_CreateNumber(stats->protection[DAMAGE_TYPE_MAGICAL]));
    cJSON_AddItemToArray(prange,  cJSON_CreateNumber(stats->range.min));
    cJSON_AddItemToArray(prange,  cJSON_CreateNumber(stats->range.max));
    cJSON_AddItemToArray(pattack, cJSON_CreateNumber(stats->attack[DAMAGE_TYPE_PHYSICAL]));
    cJSON_AddItemToArray(pattack, cJSON_CreateNumber(stats->attack[DAMAGE_TYPE_MAGICAL]));
    cJSON_AddItemToArray(pattack, cJSON_CreateNumber(stats->attack[DAMAGE_TYPE_TRUE]));
    struct cJSON *phit   = cJSON_CreateNumber(stats->hit);
    struct cJSON *pdodge = cJSON_CreateNumber(stats->dodge);
    struct cJSON *pcrit  = cJSON_CreateNumber(stats->crit);
    struct cJSON *pfavor = cJSON_CreateNumber(stats->favor);
    struct cJSON *pwgt   = cJSON_CreateNumber(stats->wgt);
    struct cJSON *pprof  = cJSON_CreateNumber(stats->prof);
    // struct cJSON * pminhand = cJSON_CreateNumber(stats->hand);
    // struct cJSON * pismagic = cJSON_CreateNumber(stats->ismagic);
    cJSON_AddItemToObject(jstats, "Attack",     pattack);
    cJSON_AddItemToObject(jstats, "Protection", pprot);
    cJSON_AddItemToObject(jstats, "hit",        phit);
    cJSON_AddItemToObject(jstats, "dodge",      pdodge);
    cJSON_AddItemToObject(jstats, "crit",       pcrit);
    cJSON_AddItemToObject(jstats, "favor",      pfavor);
    cJSON_AddItemToObject(jstats, "wgt",        pwgt);
    cJSON_AddItemToObject(jstats, "prof",       pprof);
    cJSON_AddItemToObject(jstats, "Range",      prange);
    // cJSON_AddItemToObject(jstats, "hand", pminhand);
    // cJSON_AddItemToObject(jstats, "dmg_type", pdmg_type);
}

void Inventory_item_readJSON(void *input, struct cJSON *_jitem) {
    struct Inventory_item *item = input;
    SDL_assert(_jitem != NULL);
    struct cJSON *jid       = cJSON_GetObjectItem(_jitem, "id");
    struct cJSON *jused     = cJSON_GetObjectItem(_jitem, "used");
    struct cJSON *jinfusion = cJSON_GetObjectItem(_jitem, "infusion");
    item->id       = cJSON_GetNumberValue(jid);
    item->used     = cJSON_GetNumberValue(jused);
    item->infusion = cJSON_GetNumberValue(jinfusion);
}

void Inventory_item_writeJSON(void *input, struct cJSON *jitem) {
    struct Inventory_item *item = input;
    SDL_assert(jitem != NULL);
    char buffer[DEFAULT_BUFFER_SIZE];
    cJSON *jid       = cJSON_CreateNumber(item->id);
    cJSON *jused     = cJSON_CreateNumber(item->used);
    cJSON *jinfusion = cJSON_CreateNumber(item->infusion);
    cJSON_AddItemToObject(jitem, "id",       jid);
    cJSON_AddItemToObject(jitem, "used",     jused);
    cJSON_AddItemToObject(jitem, "infusion", jinfusion);
}

void Item_stats_readJSON(void *input, struct cJSON *_jstats) {
    SDL_assert(cJSON_IsObject(_jstats));
    struct Item_stats *stats = input;
    SDL_assert(_jstats != NULL);
    struct cJSON *jAP    = cJSON_GetObjectItem(_jstats, "AP");
    struct cJSON *juses  = cJSON_GetObjectItem(_jstats, "uses");
    struct cJSON *jprice = cJSON_GetObjectItem(_jstats, "price");
    stats->AP    = cJSON_GetNumberValue(jAP);
    stats->uses  = cJSON_GetNumberValue(juses);
    stats->price = cJSON_GetNumberValue(jprice);
}

void Range_writeJSON(void *input, struct cJSON *jrange) {
    SDL_assert(cJSON_IsArray(jrange));
    struct Range *range = input;
    struct cJSON *jmin      = cJSON_CreateNumber(range->min);
    struct cJSON *jmax      = cJSON_CreateNumber(range->max);
    cJSON_AddItemToArray(jrange, jmin);
    cJSON_AddItemToArray(jrange, jmax);
}

void Range_readJSON(void *input, struct cJSON *jrange) {
    SDL_assert(cJSON_IsArray(jrange));
    struct Range *range = input;
    SDL_assert(jrange != NULL);

    range->min = cJSON_GetNumberValue(cJSON_GetArrayItem(jrange, RANGE_MIN_INDEX));
    range->max = cJSON_GetNumberValue(cJSON_GetArrayItem(jrange, RANGE_MAX_INDEX));
}

void Computed_Stats_writeJSON(void *input, struct cJSON *jstats) {
    struct Computed_Stats *stats = input;

    struct cJSON *jarr      = cJSON_CreateArray();
    struct cJSON *jnum      = cJSON_CreateNumber(stats->attack[0]);
    cJSON_AddItemToArray(jarr, jnum);
    jnum      = cJSON_CreateNumber(stats->attack[1]);
    cJSON_AddItemToArray(jarr, jnum);
    jnum      = cJSON_CreateNumber(stats->attack[2]);
    cJSON_AddItemToArray(jarr, jnum);
    cJSON_AddItemToObject(jstats, "Attack", jarr);

    jarr      = cJSON_CreateArray();
    jnum      = cJSON_CreateNumber(stats->protection[0]);
    cJSON_AddItemToArray(jarr, jnum);
    jnum      = cJSON_CreateNumber(stats->protection[1]);
    cJSON_AddItemToArray(jarr, jnum);
    cJSON_AddItemToObject(jstats, "Protection", jarr);


    struct cJSON *jrange_loadout  = cJSON_CreateArray();
    Range_writeJSON(&stats->range_loadout, jrange_loadout);
    cJSON_AddItemToObject(jstats, "Range_loadout", jrange_loadout);
    struct cJSON *jrange_combined  = cJSON_CreateArray();
    Range_writeJSON(&stats->range_combined, jrange_combined);
    cJSON_AddItemToObject(jstats, "Range_combined", jrange_combined);

    struct cJSON *jhit      = cJSON_CreateNumber(stats->hit);
    struct cJSON *jdodge    = cJSON_CreateNumber(stats->dodge);
    struct cJSON *jcrit     = cJSON_CreateNumber(stats->crit);
    struct cJSON *jfavor    = cJSON_CreateNumber(stats->favor);
    struct cJSON *jmove     = cJSON_CreateNumber(stats->move);
    struct cJSON *jspeed    = cJSON_CreateNumber(stats->speed);
    struct cJSON *jagony    = cJSON_CreateNumber(stats->agony);
    cJSON_AddItemToObject(jstats, "hit", jhit);
    cJSON_AddItemToObject(jstats, "dodge", jdodge);
    cJSON_AddItemToObject(jstats, "crit", jcrit);
    cJSON_AddItemToObject(jstats, "favor", jfavor);
    cJSON_AddItemToObject(jstats, "move", jmove);
    cJSON_AddItemToObject(jstats, "speed", jspeed);
    cJSON_AddItemToObject(jstats, "agony", jagony);
}

void Computed_Stats_readJSON(void *input, struct cJSON *jstats) {
    struct Computed_Stats *stats = input;
    SDL_assert(jstats != NULL);
    struct cJSON *jnum;
    size_t i;
    struct cJSON *jattack = cJSON_GetObjectItem(jstats, "Attack");
    i = 0;
    cJSON_ArrayForEach(jnum, jattack) {
        stats->attack[i++] = (u8)cJSON_GetNumberValue(jnum);
    }

    struct cJSON *jprot = cJSON_GetObjectItem(jstats, "Protection");
    i = 0;
    cJSON_ArrayForEach(jnum, jprot) {
        stats->protection[i++] = (u8)cJSON_GetNumberValue(jnum);
    }

    struct cJSON *jrange = cJSON_GetObjectItem(jstats, "Range_loadout");
    if (jrange != NULL)
        Range_readJSON(&stats->range_loadout, jrange);

    jrange = cJSON_GetObjectItem(jstats, "Range_combined");
    if (jrange != NULL)
        Range_readJSON(&stats->range_combined, jrange);

    // SDL_assert(stats->range.max >= stats->range.min);
    struct cJSON *jhit      = cJSON_GetObjectItem(jstats, "hit");
    struct cJSON *jdodge    = cJSON_GetObjectItem(jstats, "dodge");
    struct cJSON *jcrit     = cJSON_GetObjectItem(jstats, "crit");
    struct cJSON *jfavor    = cJSON_GetObjectItem(jstats, "favor");
    struct cJSON *jmove     = cJSON_GetObjectItem(jstats, "move");
    struct cJSON *jspeed    = cJSON_GetObjectItem(jstats, "speed");
    struct cJSON *jagony    = cJSON_GetObjectItem(jstats, "agony");

    if (jhit != NULL)
        stats->hit   = cJSON_GetNumberValue(jhit);
    if (jdodge != NULL)
        stats->dodge = cJSON_GetNumberValue(jdodge);
    if (jcrit != NULL)
        stats->crit  = cJSON_GetNumberValue(jcrit);
    if (jfavor != NULL)
        stats->favor = cJSON_GetNumberValue(jfavor);
    if (jmove != NULL)
        stats->move = cJSON_GetNumberValue(jmove);
    if (jspeed != NULL)
        stats->favor = cJSON_GetNumberValue(jspeed);
    if (jagony != NULL)
        stats->favor = cJSON_GetNumberValue(jagony);
}


void Weapon_stats_readJSON(void *input, struct cJSON *jstats) {
    struct Weapon_stats *stats = input;
    SDL_assert(jstats != NULL);
    struct cJSON *jnum;
    size_t i;
    struct cJSON *jattack = cJSON_GetObjectItem(jstats, "Attack");
    i = 0;
    cJSON_ArrayForEach(jnum, jattack) {
        stats->attack[i++] = (u8)cJSON_GetNumberValue(jnum);
    }

    struct cJSON *jprot = cJSON_GetObjectItem(jstats, "Protection");
    i = 0;
    cJSON_ArrayForEach(jnum, jprot) {
        stats->protection[i++] = (u8)cJSON_GetNumberValue(jnum);
    }

    struct cJSON *jrange = cJSON_GetObjectItem(jstats, "Range");
    if (jrange != NULL)
        Range_readJSON(&stats->range, jrange);

    // SDL_assert(stats->range.max >= stats->range.min);
    struct cJSON *jhit   = cJSON_GetObjectItem(jstats, "hit");
    struct cJSON *jdodge = cJSON_GetObjectItem(jstats, "dodge");
    struct cJSON *jcrit  = cJSON_GetObjectItem(jstats, "crit");
    struct cJSON *jfavor = cJSON_GetObjectItem(jstats, "favor");
    struct cJSON *jwgt   = cJSON_GetObjectItem(jstats, "wgt");
    struct cJSON *jprof  = cJSON_GetObjectItem(jstats, "prof");

    if (jhit != NULL)
        stats->hit   = cJSON_GetNumberValue(jhit);
    if (jdodge != NULL)
        stats->dodge  = cJSON_GetNumberValue(jdodge);
    if (jcrit != NULL)
        stats->crit  = cJSON_GetNumberValue(jcrit);
    if (jfavor != NULL)
        stats->favor = cJSON_GetNumberValue(jfavor);
    if (jwgt != NULL)
        stats->wgt   = cJSON_GetNumberValue(jwgt);
    if (jprof != NULL)
        stats->prof  = cJSON_GetNumberValue(jprof);
}

void Unit_stats_readJSON(void *input, struct cJSON *jstats) {
    struct Unit_stats *stats = input;
    SDL_assert(stats != NULL);
    SDL_assert(jstats != NULL);
    struct cJSON *jhp   = cJSON_GetObjectItem(jstats, "hp");
    struct cJSON *jstr  = cJSON_GetObjectItem(jstats, "str");
    struct cJSON *jmag  = cJSON_GetObjectItem(jstats, "mag");
    struct cJSON *jagi  = cJSON_GetObjectItem(jstats, "agi");
    struct cJSON *jdex  = cJSON_GetObjectItem(jstats, "dex");
    struct cJSON *jluck = cJSON_GetObjectItem(jstats, "luck");
    struct cJSON *jfth  = cJSON_GetObjectItem(jstats, "fth");
    struct cJSON *jdef  = cJSON_GetObjectItem(jstats, "def");
    struct cJSON *jres  = cJSON_GetObjectItem(jstats, "res");
    struct cJSON *jcon  = cJSON_GetObjectItem(jstats, "con");
    struct cJSON *jmove = cJSON_GetObjectItem(jstats, "move");
    struct cJSON *jprof = cJSON_GetObjectItem(jstats, "prof");

    if (jhp != NULL)
        stats->hp   = cJSON_GetNumberValue(jhp);
    if (jstr != NULL)
        stats->str  = cJSON_GetNumberValue(jstr);
    if (jmag != NULL)
        stats->mag  = cJSON_GetNumberValue(jmag);
    if (jagi != NULL)
        stats->agi  = cJSON_GetNumberValue(jagi);
    if (jdex != NULL)
        stats->dex  = cJSON_GetNumberValue(jdex);
    if (jfth != NULL)
        stats->fth  = cJSON_GetNumberValue(jfth);
    if (jluck != NULL)
        stats->luck = cJSON_GetNumberValue(jluck);
    if (jdef != NULL)
        stats->def  = cJSON_GetNumberValue(jdef);
    if (jres != NULL)
        stats->res  = cJSON_GetNumberValue(jres);
    if (jcon != NULL)
        stats->con  = cJSON_GetNumberValue(jcon);
    if (jmove != NULL)
        stats->move = cJSON_GetNumberValue(jmove);
    if (jprof != NULL)
        stats->prof = cJSON_GetNumberValue(jprof);
}

void jsonio_Print(PHYSFS_file *fp, struct cJSON *_json) {
    SDL_Log("jsonio_Print");
    /* Write json element bytes to buffer */
    char *buffer = cJSON_Print(_json);

    /* Set up buffering for a PhysicsFS file handle. */
    size_t length = strlen(buffer);
    if (!PHYSFS_setBuffer(fp, length)) {
        SDL_Log("PHYSFS_setBuffer failed");
        exit(ERROR_CannotWriteFile);
    }

    /* Write bytes to file with Physfs */
    PHYSFS_writeBytes(fp, buffer, length);

    /* Free buffer */
    cJSON_free(buffer);
}

/* -- Comparisons -- */
b32  jsonio_Equal(char *filename1, char *filename2, b32 case_sensitive) {
    SDL_assert(filename1 != NULL);
    SDL_assert(filename2 != NULL);

    struct cJSON *jfile1 = jsonio_parseJSON(s8_var(filename1));
    struct cJSON *jfile2 = jsonio_parseJSON(s8_var(filename2));

    b32 out = cJSON_Compare(jfile1, jfile2, case_sensitive);

    /* Clean the jfile */
    if (jfile1 != NULL)
        cJSON_Delete(jfile1);
    if (jfile2 != NULL)
        cJSON_Delete(jfile2);

    return (out);
}
