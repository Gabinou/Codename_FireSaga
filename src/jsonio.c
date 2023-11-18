
#include "jsonio.h"

json_func json_read_funcs [JSON_END] = {
    /* JSON_NULL        */ NULL,
    /* JSON_ITEM        */ Item_readJSON,
    /* JSON_TILE        */ Tile_readJSON,
    /* JSON_UNIT        */ Unit_readJSON,
    /* JSON_WEAPON      */ Weapon_readJSON,
    /* JSON_MAP         */ Map_readJSON,
    /* JSON_SCENE       */ Scene_readJSON,
    /* JSON_CONVOY      */ Convoy_readJSON,
    /* JSON_NARRATIVE   */ NULL,
    /* JSON_CAMP        */ Camp_readJSON,
    /* JSON_SPRITESHEET */ Sprite_readJSON,
};

json_func json_write_funcs[JSON_END] = {
    /* JSON_NULL        */ NULL,
    /* JSON_ITEM        */ Item_writeJSON,
    /* JSON_TILE        */ Tile_writeJSON,
    /* JSON_UNIT        */ Unit_writeJSON,
    /* JSON_WEAPON      */ Weapon_writeJSON,
    /* JSON_MAP         */ Map_writeJSON,
    /* JSON_SCENE       */ NULL,
    /* JSON_CONVOY      */ Convoy_writeJSON,
    /* JSON_NARRATIVE   */ NULL,
    /* JSON_CAMP        */ Camp_writeJSON,
    /* JSON_SPRITESHEET */ NULL,
};

struct cJSON *jsonio_parseJSON(s8 filename) {

    /* Error if file doesn't exist */
    if (!PHYSFS_exists(filename.data)) {
        SDL_Log("File %s does not exist", filename.data);
        SDL_assert(false);
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
    const char *string = filename.data;
    SDL_Log("string '%s' %d", string, strlen(string));
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
    if (json_read_funcs[jelem_id] != NULL)
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

void jsonio_writeJSON(s8 filename, const void *struct_ptr, bool append) {
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
    if (json_write_funcs[jelem_id] != NULL)
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
void jsonio_Read_Shop(const char *filename, struct Shop *shop) {
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

void jsonio_Read_Promotion(const char *filename, struct Promotion *promotion) {
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
    jsonio_Read_Unitstats(jstats, &promotion->bonus);
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

void jsonio_Read_Palette(const char *filename, struct SDL_Palette *palette) {
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
    if (colors_num != PALETTE_NES_COLOR_NUM) {
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

void jsonio_Read_PaletteTable(const char *filename, u8   *table) {
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
    if (colors_num != PALETTE_NES_COLOR_NUM) {
        SDL_Log("Invalid number of colors in Palette Table");
        exit(ERROR_JSONParsingFailed);
    }

    for (int i = 0; i < colors_num; i++) {
        struct cJSON *jfrom_to =  cJSON_GetArrayItem(jfrom_tos, i);
        table[i] = cJSON_GetNumberValue(cJSON_GetArrayItem(jfrom_to, 1));
    }
    if (jfile != NULL)
        cJSON_Delete(jfile);

}

void jsonio_Read_MObj_Link(struct cJSON *j, struct Mobj_Link * b) {

}

void jsonio_Read_Breakable(struct cJSON *_jbreak, struct Breakable * breakable) {
    struct cJSON *jhp  = cJSON_GetObjectItem(_jbreak, "hp");
    struct cJSON *jdef = cJSON_GetObjectItem(_jbreak, "def");
    struct cJSON *jres = cJSON_GetObjectItem(_jbreak, "res");

    if (jres != NULL)
        breakable->res = cJSON_GetNumberValue(jres);
    if (jdef != NULL)
        breakable->def = cJSON_GetNumberValue(jdef);
    if (jhp != NULL)
        breakable->hp  = cJSON_GetNumberValue(jhp);
}

void jsonio_Read_Door(struct cJSON *_jdoor, struct Door * door) {
    struct cJSON *jtile          = cJSON_GetObjectItem(_jdoor, "tile");
    struct cJSON *jevent         = cJSON_GetObjectItem(_jdoor, "event");
    struct cJSON *jscene         = cJSON_GetObjectItem(_jdoor, "scene");
    struct cJSON *jchapter_open  = cJSON_GetObjectItem(_jdoor, "chapter_open");
    struct cJSON *jchapter_close = cJSON_GetObjectItem(_jdoor, "chapter_close");

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

void jsonio_Read_Chest(struct cJSON *_jchest, struct Chest * chest) {
    SDL_assert(_jchest != NULL);
    SDL_assert(chest != NULL);
    struct cJSON *jgold = cJSON_GetObjectItem(_jchest, "gold");
    struct cJSON *jitem = cJSON_GetObjectItem(_jchest, "gold");
    struct cJSON *jtile = cJSON_GetObjectItem(_jchest, "tile");

    if (jtile != NULL)
        chest->tile = cJSON_GetNumberValue(jtile);
    if (jgold != NULL)
        chest->gold = cJSON_GetNumberValue(jgold);
    if (jitem != NULL) {
        char *name  = cJSON_GetStringValue(jitem);
        // if (name != NULL)
        // chest->item = Hashes_itemName2ID(name);
    }
}

void jsonio_Read_Position(struct cJSON *_jpos, struct Point *pos) {
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

void jsonio_Read_mvtcost(struct cJSON          * _jcost,
                         struct fMovement_cost * cost) {
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

void jsonio_Read_Array(struct cJSON *_jarray, i32 * array) {
    SDL_assert(array != NULL);
    struct cJSON *jnum;
    size_t i = 0;
    cJSON_ArrayForEach(jnum, _jarray) {
        array[i++] = (i32)cJSON_GetNumberValue(jnum);
    }
}

void jsonio_Read_2DArray(struct cJSON *_jarr, i32 * arr2D,
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
void jsonio_Write_Array(struct cJSON *_jarr, const i32 * arr, size_t num) {
    SDL_Log("%zu", num);
    SDL_assert(arr != NULL);
    for (u8 i = 0; i < num; i++) {
        struct cJSON *jnum = cJSON_CreateNumber(arr[i]);
        cJSON_AddItemToArray(_jarr, jnum);
    }
}

void jsonio_Write_2DArray(struct cJSON *arr, const i32 * arr2D, u8 row_len,
                          u8 col_len) {
    SDL_Log("%d %d", row_len, col_len);
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

void jsonio_Write_mvtcost(struct cJSON *_jcost, struct fMovement_cost *_cost) {
    SDL_assert(jcost != NULL);
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

void jsonio_Read_Tilestats(struct cJSON      *_jstats,
                           struct Tile_stats *stats) {
    SDL_assert(jstats != NULL);
    struct cJSON *jdodge = cJSON_GetObjectItem(_jstats, "Dodge");
    struct cJSON *jPprot = cJSON_GetObjectItem(_jstats, "Pprot");
    struct cJSON *jMprot = cJSON_GetObjectItem(_jstats, "Mprot");
    struct cJSON *jHeal  = cJSON_GetObjectItem(_jstats, "Heal");
    stats->dodge         = cJSON_GetNumberValue(jdodge);
    stats->Pprot         = cJSON_GetNumberValue(jPprot);
    stats->Mprot         = cJSON_GetNumberValue(jMprot);
    stats->heal          = cJSON_GetNumberValue(jHeal);
}

void jsonio_Read_Reinforce(struct cJSON         *_jarrival,
                           struct Reinforcement *arrival) {
    SDL_assert(arrival  != NULL);
    SDL_assert(jarrival != NULL);
    struct cJSON *jid       = cJSON_GetObjectItem(_jarrival,  "id");
    struct cJSON *jarmy     = cJSON_GetObjectItem(_jarrival,  "army");
    struct cJSON *jturn     = cJSON_GetObjectItem(_jarrival,  "turn");
    struct cJSON *jlevelups = cJSON_GetObjectItem(_jarrival,  "levelups");
    struct cJSON *jposition = cJSON_GetObjectItem(_jarrival,  "Position");

    struct cJSON *jrow      = cJSON_GetObjectItem(jposition, "row");
    struct cJSON *jcol      = cJSON_GetObjectItem(jposition, "col");

    arrival->id         = cJSON_GetNumberValue(jid);
    arrival->army       = cJSON_GetNumberValue(jarmy);
    arrival->turn       = cJSON_GetNumberValue(jturn);
    arrival->levelups   = cJSON_GetNumberValue(jlevelups);
    arrival->position.x = cJSON_GetNumberValue(jrow);
    arrival->position.y = cJSON_GetNumberValue(jcol);
}

void jsonio_Write_arrival(      struct cJSON                  *jarrival,
                                const struct Reinforcement * arrival) {
    SDL_assert(jarrival != NULL);
    struct cJSON *jid       = cJSON_CreateNumber(arrival->id);
    struct cJSON *jrow      = cJSON_CreateNumber(arrival->position.x);
    struct cJSON *jcol      = cJSON_CreateNumber(arrival->position.y);
    struct cJSON *jturn     = cJSON_CreateNumber(arrival->turn);
    struct cJSON *jarmy     = cJSON_CreateNumber(arrival->army);
    struct cJSON *jlevelups = cJSON_CreateNumber(arrival->levelups);
    struct cJSON *jposition = cJSON_CreateObject();
    cJSON_AddItemToObject(jposition, "row",      jrow);
    cJSON_AddItemToObject(jposition, "col",      jcol);
    cJSON_AddItemToObject(jarrival,  "id",       jid);
    cJSON_AddItemToObject(jarrival,  "army",     jarmy);
    cJSON_AddItemToObject(jarrival,  "levelups", jlevelups);
    cJSON_AddItemToObject(jarrival,  "turn",     jturn);
    cJSON_AddItemToObject(jarrival,  "Position", jposition);
}

void jsonio_Write_Tilestats(      struct cJSON      * jstats,
                                  const struct Tile_stats * stats) {
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

void jsonio_Write_Unitstats(struct cJSON * jstats,
                            const struct Unit_stats * stats) {
    SDL_assert(jstats != NULL);
    struct cJSON *php   = cJSON_CreateNumber(stats->hp);
    struct cJSON *pstr  = cJSON_CreateNumber(stats->str);
    struct cJSON *pmag  = cJSON_CreateNumber(stats->mag);
    struct cJSON *pagi  = cJSON_CreateNumber(stats->agi);
    struct cJSON *pdex  = cJSON_CreateNumber(stats->dex);
    struct cJSON *pluck = cJSON_CreateNumber(stats->luck);
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
    cJSON_AddItemToObject(jstats, "def",  pdef);
    cJSON_AddItemToObject(jstats, "res",  pres);
    cJSON_AddItemToObject(jstats, "con",  pcon);
    cJSON_AddItemToObject(jstats, "move", pmove);
    cJSON_AddItemToObject(jstats, "prof", pprof);
}

void jsonio_Write_Itemstats(struct cJSON * jstats,
                            const struct Item_stats * stats) {
    SDL_assert(jstats != NULL);
    struct cJSON *jAP    = cJSON_CreateNumber(stats->AP);
    struct cJSON *juses  = cJSON_CreateNumber(stats->uses);
    struct cJSON *jprice = cJSON_CreateNumber(stats->price);
    cJSON_AddItemToObject(jstats, "uses", juses);
    cJSON_AddItemToObject(jstats, "price", jprice);
    cJSON_AddItemToObject(jstats, "AP", jAP);
}

void jsonio_Write_Wpnstats(      struct cJSON                *jstats,
                                 const struct Weapon_stats * stats) {
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

void jsonio_Read_Item(struct cJSON *_jitem,
                      struct Inventory_item * item) {
    SDL_assert(jitem != NULL);
    struct cJSON *jid       = cJSON_GetObjectItem(jitem, "id");
    struct cJSON *jused     = cJSON_GetObjectItem(jitem, "used");
    struct cJSON *jinfusion = cJSON_GetObjectItem(jitem, "infusion");
    item->id       = cJSON_GetNumberValue(jid);
    item->used     = cJSON_GetNumberValue(jused);
    item->infusion = cJSON_GetNumberValue(jinfusion);
}

void jsonio_Write_item(struct cJSON *jitem, const struct Inventory_item * item) {
    SDL_assert(jitem != NULL);
    char buffer[DEFAULT_BUFFER_SIZE];
    cJSON *jid       = cJSON_CreateNumber(item->id);
    cJSON *jused     = cJSON_CreateNumber(item->used);
    cJSON *jinfusion = cJSON_CreateNumber(item->infusion);
    cJSON_AddItemToObject(jitem, "id",       jid);
    cJSON_AddItemToObject(jitem, "used",     jused);
    cJSON_AddItemToObject(jitem, "infusion", jinfusion);
}

void jsonio_Read_Itemstats(struct cJSON *_jstats,
                           struct Item_stats * stats) {
    SDL_assert(jstats != NULL);
    struct cJSON *jAP    = cJSON_GetObjectItem(jstats, "AP");
    struct cJSON *juses  = cJSON_GetObjectItem(jstats, "uses");
    struct cJSON *jprice = cJSON_GetObjectItem(jstats, "price");
    stats->AP    = cJSON_GetNumberValue(jAP);
    stats->uses  = cJSON_GetNumberValue(juses);
    stats->price = cJSON_GetNumberValue(jprice);
}

void jsonio_Read_Wpnstats(const struct cJSON        *const    jstats,
                          struct Weapon_stats *  stats) {
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
    stats->range.min = (u8)cJSON_GetNumberValue(cJSON_GetArrayItem(jrange, RANGE_MIN));
    stats->range.max = (u8)cJSON_GetNumberValue(cJSON_GetArrayItem(jrange, RANGE_MAX));
    SDL_assert(stats->range.max >= stats->range.min);
    struct cJSON *jhit   = cJSON_GetObjectItem(jstats, "hit");
    struct cJSON *jwgt   = cJSON_GetObjectItem(jstats, "wgt");
    struct cJSON *jcrit  = cJSON_GetObjectItem(jstats, "crit");
    struct cJSON *jprof  = cJSON_GetObjectItem(jstats, "prof");
    struct cJSON *jhand  = cJSON_GetObjectItem(jstats, "hand");
    struct cJSON *jfavor = cJSON_GetObjectItem(jstats, "favor");
    struct cJSON *jdodge = cJSON_GetObjectItem(jstats, "dodge");
    stats->hit   = cJSON_GetNumberValue(jhit);
    stats->wgt   = cJSON_GetNumberValue(jwgt);
    stats->crit  = cJSON_GetNumberValue(jcrit);
    stats->prof  = cJSON_GetNumberValue(jprof);
    stats->favor = cJSON_GetNumberValue(jfavor);
    stats->dodge = cJSON_GetNumberValue(jdodge);
    // stats->hand = cJSON_GetNumberValue(jhand);
}

void jsonio_Read_Unitstats(struct cJSON      *    jstats,
                           struct Unit_stats * stats) {
    SDL_assert(stats != NULL);
    SDL_assert(jstats != NULL);
    struct cJSON *jhp   = cJSON_GetObjectItem(jstats, "hp");
    struct cJSON *jstr  = cJSON_GetObjectItem(jstats, "str");
    struct cJSON *jmag  = cJSON_GetObjectItem(jstats, "mag");
    struct cJSON *jagi  = cJSON_GetObjectItem(jstats, "agi");
    struct cJSON *jdex  = cJSON_GetObjectItem(jstats, "dex");
    struct cJSON *jfth  = cJSON_GetObjectItem(jstats, "fth");
    struct cJSON *jluck = cJSON_GetObjectItem(jstats, "luck");
    struct cJSON *jdef  = cJSON_GetObjectItem(jstats, "def");
    struct cJSON *jres  = cJSON_GetObjectItem(jstats, "res");
    struct cJSON *jcon  = cJSON_GetObjectItem(jstats, "con");
    struct cJSON *jmove = cJSON_GetObjectItem(jstats, "move");
    struct cJSON *jprof = cJSON_GetObjectItem(jstats, "prof");
    stats->hp   = cJSON_GetNumberValue(jhp);
    stats->str  = cJSON_GetNumberValue(jstr);
    stats->mag  = cJSON_GetNumberValue(jmag);
    stats->agi  = cJSON_GetNumberValue(jagi);
    stats->dex  = cJSON_GetNumberValue(jdex);
    stats->fth  = cJSON_GetNumberValue(jfth);
    stats->luck = cJSON_GetNumberValue(jluck);
    stats->def  = cJSON_GetNumberValue(jdef);
    stats->res  = cJSON_GetNumberValue(jres);
    stats->con  = cJSON_GetNumberValue(jcon);
    stats->move = cJSON_GetNumberValue(jmove);
    stats->prof = cJSON_GetNumberValue(jprof);
}

void jsonio_Print(PHYSFS_file *fp, struct cJSON *_json) {
    /* Write json element bytes to buffer */
    char *buffer = cJSON_Print(json);

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

