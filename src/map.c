#include "map.h"
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
//#endif /* STB_SPRINTF_IMPLEMENTATION */

struct Map Map_default = {
    .json_element = JSON_MAP,
    .srcrect = {0, 0, DEFAULT_TILESIZE, DEFAULT_TILESIZE},
    .destrect = {0, 0, DEFAULT_TILESIZE, DEFAULT_TILESIZE},

    .renderer = NULL,
    .frames = 10,
    .speed = 50,
    .tilesize = {0, 0},

    .items_num = NULL,
    .arrivals_num = 0,

    .textures = NULL,

    .overlays = {NULL, NULL, NULL},
    .dangers = {NULL, NULL},

    .overlay_move = NULL,
    .overlay_attack = NULL,
    .overlay_heal = NULL,
    .overlay_danger = NULL,
    .show_overlay = false,
    .show_danger = false,
    .show_grid = false,
    .mode_overlay = 0,
    .mode_danger = 0,

    .tiles_num = 0,
    .row_len = 0,
    .col_len = 0,

    .tilemap = NULL,
    .tiles = NULL,
    .tilesindex = NULL,

    .boundsmin = {0, 0},
    .boundsmax = {255, 255},
    .offset_px = {DEFAULT_TILEMAP_XOFFSET, DEFAULT_TILEMAP_YOFFSET},
    .num_friendlies = 0,
    .num_enemies = 0,
    .turn = 0, // Automatic loss if turn 255.
    .chapter = 0,

    .arrivals = NULL,
    .loaded_arrivals = NULL,
    .arrival_equipments = NULL,

    .starting_positions = NULL,
    .armies_onfield = NULL,
    .num_armies_onfield = 0,
    .units_onfield = NULL,
    .num_units_onfield = 0,
    .friendlies_onfield = NULL,
    .num_friendlies_onfield = 0,
    .enemies_onfield = NULL,
    .num_enemies_onfield = 0,
    .unitmap = NULL,
    .unitmap = NULL,
    .essentials = NULL,
    // .boss;
    // .bossdied;
    // .seized; // maybe unecessary if turn system.
};


extern void Map_Breakable_onBroken(struct Map * in_map, tnecs_world_t * in_world, tnecs_entity_t in_breakable_ent) {

}

extern void Map_Door_onOpen(struct Map * in_map, tnecs_world_t * in_world, tnecs_entity_t in_door_ent) {

}

extern void Map_Chest_onOpen(struct Map * in_map, tnecs_world_t * in_world, tnecs_entity_t in_chest_ent) {

}

extern void Map_globalRange(struct Map * in_map, tnecs_world_t * in_world, uint8_t in_alignment) {
    SDL_Log("Map_globalRange");

    tnecs_entity_t * unit_entities = NULL;
    uint8_t num_unit_entities = 0;
    if (in_map->globalRange != NULL) {
        arrfree(in_map->globalRange);
    }
    arrsetlen(in_map->globalRange, in_map->row_len * in_map->col_len);
    for (int_tile_t row = 0; row < in_map->row_len; row++) {
        for (int_tile_t col = 0; col < in_map->col_len; col++) {
            in_map->globalRange[row * in_map->col_len + col] = 0;
        }
    }
    linalg_matrix_print_int16_t(in_map->globalRange, in_map->row_len, in_map->col_len);

    switch (in_alignment) {
        case ALIGNMENT_FRIENDLY:
            unit_entities = in_map->friendlies_onfield;
            num_unit_entities = in_map->num_friendlies_onfield;
            break;
        case ALIGNMENT_ENEMY:
            unit_entities = in_map->enemies_onfield;
            num_unit_entities = in_map->num_enemies_onfield;
            break;
    }
    int_tile_t * costmapp = NULL;
    int_tile_t * movemapp = NULL;
    arrsetlen(movemapp, in_map->row_len * in_map->col_len);
    int_tile_t * attackmapp = NULL;
    arrsetlen(attackmapp, in_map->row_len * in_map->col_len);

    SDL_Log("in_map->num_enemies_onfield %d", in_map->num_enemies_onfield);
    SDL_Log("num_unit_entities %d", num_unit_entities);
    SDL_Log("in_map->num_friendlies_onfield %d", in_map->num_friendlies_onfield);
    for (uint8_t i = 0; i < num_unit_entities; i++) {
        struct Unit * temp_unit = TNECS_GET_COMPONENT(in_world, unit_entities[i], Unit);
        SDL_Log("unit name: %s", temp_unit->name);
        struct Position * temp_pos = TNECS_GET_COMPONENT(in_world, unit_entities[i], Position);
        struct Unit_stats temp_effective_stats = Unit_effectiveStats(temp_unit);
        int_tile_t * range = Unit_computeRange(temp_unit);
        uint8_t move = temp_effective_stats.move;
        struct Point start = temp_pos->tilemap_pos;
        costmapp = Map_Costmap_Movement_Compute(in_map, in_world, unit_entities[i]);
        movemapp = Pathfinding_Map_Move(costmapp, in_map->row_len, in_map->col_len, start, move, POINTS_MATRIX);
        attackmapp = Pathfinding_Map_Attack(movemapp, in_map->row_len, in_map->col_len, move, range, POINTS_MATRIX, MOVETILE_INCLUDE);
        in_map->globalRange = linalg_plusint16_t(in_map->globalRange, attackmapp, in_map->row_len, in_map->col_len, 1);
        // in_map->globalRange = linalg_plusint16_t(in_map->globalRange, movemapp, in_map->row_len, in_map->col_len, 1);
        arrfree(costmapp);
        arrfree(movemapp);
        arrfree(attackmapp);
    }
    linalg_matrix_print_int16_t(in_map->globalRange, in_map->row_len, in_map->col_len);
}

void Map_startingPos_Add(struct Map * in_map, struct Point in_position) {
    SDL_Log("Map_startingPos_Add");
    arrput(in_map->starting_positions, in_position);
    in_map->starting_positions_num++;
}

void Map_Tiles_Load(struct Map * in_map) {
    SDL_Log("Map_Tiles_Load");
    // SDL_Log("%d", in_map->tiles_num);
    int16_t index;
    struct Tile * temp_tile = NULL;
    char filename[DEFAULT_BUFFER_SIZE];
    for (int16_t i = 0; i < in_map->tiles_num; i++) {
        if (in_map->tilesindex[i] > DEFAULT_TILE_MAX) {
            index = in_map->tilesindex[i] / DEFAULT_TILE_DIVISOR;
        } else {
            index = in_map->tilesindex[i];
        }
        SDL_Log("index %d", index);
        temp_tile = (struct Tile *)malloc(sizeof(struct Tile));
        *temp_tile = Tile_default;
        strcpy(filename, "");
        strcat(filename, "tiles");
        strcat(filename, PHYSFS_SEPARATOR);
        strcat(filename, hmget(tilenames, index));
        strcat(filename, ".json");
        readJSON(filename, temp_tile);
        Tile_makeMvtCostarray(temp_tile);

        hmput(in_map->tiles, index, temp_tile);
    }
    // struct Tile temp = hmget(in_map->tiles, TILE_PLAIN);
    // SDL_Log("name %s", temp->name);
    // SDL_Log("cost struct %d", hmget(in_map->tiles, TILE_PLAIN)->cost_struct.mages);
    // SDL_Log("cost array %d", hmget(in_map->tiles, TILE_PLAIN)->cost_array[UNIT_MVT_MAGES]);
    // SDL_Log("name %s", hmget(in_map->tiles, TILE_FOREST)->name);
    // SDL_Log("cost struct %d", hmget(in_map->tiles, TILE_FOREST)->cost_struct.mages);
    // SDL_Log("cost array %d", hmget(in_map->tiles, TILE_FOREST)->cost_array[UNIT_MVT_MAGES]);
    // SDL_Log("name %s", hmget(in_map->tiles, TILE_HILL)->name);
    // SDL_Log("cost struct %d", hmget(in_map->tiles, TILE_HILL)->cost_struct.mages);
    // SDL_Log("cost array %d", hmget(in_map->tiles, TILE_HILL)->cost_array[UNIT_MVT_MAGES]);
}

void Map_readJSON(struct Map * in_map, cJSON * in_jmap) {
    SDL_Log("Map_readJSON");
    SDL_assert(in_jmap != NULL);
    cJSON * jchapter = cJSON_GetObjectItem(in_jmap, "chapter");
    in_map->chapter = cJSON_GetNumberValue(jchapter);

    cJSON * jmap_size = cJSON_GetObjectItem(in_jmap, "Map size");
    cJSON * jrow_len = cJSON_GetObjectItem(jmap_size, "row_len");
    cJSON * jcol_len = cJSON_GetObjectItem(jmap_size, "col_len");
    in_map->row_len = cJSON_GetNumberValue(jrow_len);
    in_map->col_len = cJSON_GetNumberValue(jcol_len);
    SDL_Log("%d %d ", in_map->row_len, in_map->col_len);

    cJSON * jarrivals = cJSON_GetObjectItem(in_jmap, "Arrivals");
    cJSON * jstartingpositions = cJSON_GetObjectItem(in_jmap, "StartingPositions");
    cJSON * jtiles = cJSON_GetObjectItem(in_jmap, "tiles");
    cJSON * jtile = cJSON_GetObjectItem(jtiles, "tile");

    // Read starting positions
    cJSON * jcol;
    cJSON * jrow;
    struct Point temp_pos;
    if (in_map->starting_positions != NULL) {
        arrfree(in_map->starting_positions);
    }
    in_map->starting_positions_num = 0;
    cJSON * jstartingposition = cJSON_GetObjectItem(jstartingpositions, "Position");
    while (jstartingposition != NULL) {
        jcol = cJSON_GetObjectItem(jstartingposition, "col");
        jrow = cJSON_GetObjectItem(jstartingposition, "row");
        if ((jrow != NULL) && (jcol != NULL)) {
            temp_pos.x = cJSON_GetNumberValue(jcol);
            temp_pos.y = cJSON_GetNumberValue(jrow);
            Map_startingPos_Add(in_map, temp_pos);
        }
        jstartingposition = jstartingposition->next;
    }

    // Read tiles
    cJSON * jid;
    cJSON * jtilename;
    if (in_map->tilesindex != NULL) {
        arrfree(in_map->tilesindex);
    }
    in_map->tiles_num = 0;
    while (jtile != NULL) {
        jid = cJSON_GetObjectItem(jtile, "id");
        jtilename = cJSON_GetObjectItem(jtile, "name");
        arrput(in_map->tilesindex, cJSON_GetNumberValue(jid) * DEFAULT_TILE_DIVISOR);
        // SDL_Log("Read Tilesindex %d", in_map->tilesindex[in_map->tiles_num]);
        in_map->tiles_num++;
        jtile = jtile->next;
    }
    Map_Tiles_Load(in_map);

    //read arrivals
    cJSON * jarrival = cJSON_GetObjectItem(jarrivals, "Arrival");
    cJSON * jequipment;
    cJSON * jitem;
    struct Map_arrival temp_arrival;
    if (in_map->arrivals != NULL) {
        arrfree(in_map->arrivals);
    }
    if (in_map->arrival_equipments != NULL) {
        arrfree(in_map->arrival_equipments);
    }
    if (in_map->items_num != NULL) {
        arrfree(in_map->items_num);
    }

    struct Inventory_item temp_item;
    struct Inventory_item * temp_arrival_equipment = NULL;
    uint8_t arrival_num = 0;
    uint8_t item_num;
    while (jarrival != NULL) {
        temp_arrival = Map_arrival_default;
        readJSON_arrival(jarrival, &temp_arrival);
        arrput(in_map->arrivals, temp_arrival);
        arraddn(in_map->arrival_equipments, DEFAULT_EQUIPMENT_SIZE);
        jequipment = cJSON_GetObjectItem(jarrival, "Equipment");
        jitem = cJSON_GetObjectItem(jequipment, "Item");
        item_num = 0;
        while (jitem != NULL) {
            temp_item = Inventory_item_default;
            readJSON_item(jitem, &temp_item);
            (in_map->arrival_equipments)[arrival_num * DEFAULT_EQUIPMENT_SIZE + item_num] = temp_item;
            jitem = jitem->next;
            item_num++;
        }
        arrput(in_map->items_num, item_num);
        temp_arrival_equipment = NULL;
        jarrival = jarrival->next;
        arrival_num++;
    }
    in_map->arrivals_num = arrival_num;

    cJSON * jbounds = cJSON_GetObjectItem(in_jmap, "Bounds");
    cJSON * jrow_min = cJSON_GetObjectItem(jbounds, "row_min");
    cJSON * jrow_max = cJSON_GetObjectItem(jbounds, "row_max");
    cJSON * jcol_min = cJSON_GetObjectItem(jbounds, "col_min");
    cJSON * jcol_max = cJSON_GetObjectItem(jbounds, "col_max");
    in_map->boundsmin.x = cJSON_GetNumberValue(jrow_min);
    in_map->boundsmax.x = cJSON_GetNumberValue(jrow_max);
    in_map->boundsmin.y = cJSON_GetNumberValue(jcol_min);
    in_map->boundsmax.y = cJSON_GetNumberValue(jcol_max);
    cJSON * joffset = cJSON_GetObjectItem(in_jmap, "Offset_px");
    cJSON * joffset_x = cJSON_GetObjectItem(joffset, "offset_x");
    cJSON * joffset_y = cJSON_GetObjectItem(joffset, "offset_y");
    in_map->offset_px.x = cJSON_GetNumberValue(joffset_x);
    in_map->offset_px.y = cJSON_GetNumberValue(joffset_y);
    cJSON * jtilemap = cJSON_GetObjectItem(in_jmap, "Tilemap");
    in_map->tilemap = readJSON_2DArray(jtilemap, in_map->tilemap, in_map->row_len, in_map->col_len);
    Map_Bounds_Compute(in_map);
}

void Map_writeJSON(struct Map * in_map, cJSON * in_jmap) {
    SDL_Log("Map_writeJSON");
    SDL_assert(in_jmap != NULL);
    cJSON * jchapter = cJSON_CreateNumber(in_map->chapter);
    cJSON_AddItemToObject(in_jmap, "chapter", jchapter);
    cJSON * jstartingpositions = cJSON_CreateObject();
    cJSON * jmap_size = cJSON_CreateObject();
    cJSON * jrow_len = cJSON_CreateNumber(in_map->row_len);
    cJSON * jcol_len = cJSON_CreateNumber(in_map->col_len);
    cJSON_AddItemToObject(jmap_size, "row_len", jrow_len);
    cJSON_AddItemToObject(jmap_size, "col_len", jcol_len);
    cJSON_AddItemToObject(in_jmap, "Map size", jmap_size);
    // Writing Starting Positions
    cJSON_AddItemToObject(in_jmap, "StartingPositions", jstartingpositions);
    cJSON * jstartingpos;
    cJSON * jstartingposx;
    cJSON * jstartingposy;
    SDL_assert(in_map->starting_positions != NULL);
    for (int16_t i = 0; i < in_map->starting_positions_num; i++) {
        jstartingpos = cJSON_CreateObject();
        cJSON_AddItemToObject(jstartingpositions, "Position", jstartingpos);
        jstartingposx = cJSON_CreateNumber(in_map->starting_positions[i].x);
        jstartingposy = cJSON_CreateNumber(in_map->starting_positions[i].y);
        cJSON_AddItemToObject(jstartingpos, "col", jstartingposx);
        cJSON_AddItemToObject(jstartingpos, "row", jstartingposy);
    }

    // Writing Tiles
    cJSON * jtiles = cJSON_CreateObject();
    cJSON_AddItemToObject(in_jmap, "tiles", jtiles);
    cJSON * jtile;
    cJSON * jtilename;
    cJSON * jtileid;
    uint64_t temp_tile_id;
    for (size_t i = 0; i < in_map->tiles_num; i++) {
        jtile = cJSON_CreateObject();
        if (in_map->tilesindex[i] >= TILE_END) {
            temp_tile_id = in_map->tilesindex[i] / DEFAULT_TILE_DIVISOR;
            jtilename = cJSON_CreateString(hmget(tilenames, temp_tile_id));
            jtileid = cJSON_CreateNumber(temp_tile_id);
        } else {
            jtilename = cJSON_CreateString(hmget(tilenames, in_map->tilesindex[i]));
            jtileid = cJSON_CreateNumber(in_map->tilesindex[i]);
        }
        cJSON_AddItemToObject(jtile, "id", jtileid);
        cJSON_AddItemToObject(jtile, "name", jtilename);
        cJSON_AddItemToObject(jtiles, "tile", jtile);
    }
    // Writing Arrivals
    struct Inventory_item * temp_item;
    SDL_assert((in_map->arrival_equipments != NULL) & (in_map->arrivals != NULL));
    cJSON * jarrival;
    cJSON * jarrivaleq;
    cJSON * jarrivals = cJSON_CreateObject();
    cJSON_AddItemToObject(in_jmap, "Arrivals", jarrivals);
    for (uint8_t arrival = 0; arrival < in_map->arrivals_num; arrival++) {
        jarrival = cJSON_CreateObject();
        jarrivaleq = cJSON_CreateObject();
        writeJSON_arrival(jarrival, &(in_map->arrivals)[arrival]);
        for (uint8_t item_num = 0; item_num < in_map->items_num[arrival]; item_num ++) {
            temp_item = &(in_map->arrival_equipments)[arrival * DEFAULT_EQUIPMENT_SIZE + item_num];
            if (hmget(itemNames, temp_item->id)[0] != NAME_ITEM_INVALID[0]) {
                writeJSON_item(jarrivaleq, temp_item);
            }
        }
        cJSON_AddItemToObject(jarrival, "Equipment", jarrivaleq);
        cJSON_AddItemToObject(jarrivals, "Arrival", jarrival);
    }

    cJSON * jbounds = cJSON_CreateObject();
    cJSON * jrow_min = cJSON_CreateNumber(in_map->boundsmin.x);
    cJSON * jrow_max = cJSON_CreateNumber(in_map->boundsmax.x);
    cJSON * jcol_min = cJSON_CreateNumber(in_map->boundsmin.y);
    cJSON * jcol_max = cJSON_CreateNumber(in_map->boundsmax.y);
    cJSON_AddItemToObject(jbounds, "row_min", jrow_min);
    cJSON_AddItemToObject(jbounds, "row_max", jrow_max);
    cJSON_AddItemToObject(jbounds, "col_min", jcol_min);
    cJSON_AddItemToObject(jbounds, "col_max", jcol_max);
    cJSON_AddItemToObject(in_jmap, "Bounds", jbounds);
    cJSON * joffset = cJSON_CreateObject();
    cJSON * joffset_x = cJSON_CreateNumber(in_map->offset_px.x);
    cJSON * joffset_y = cJSON_CreateNumber(in_map->offset_px.y);
    cJSON_AddItemToObject(joffset, "offset_x", joffset_x);
    cJSON_AddItemToObject(joffset, "offset_y", joffset_y);
    cJSON_AddItemToObject(in_jmap, "Offset_px", joffset);
    cJSON * jtilemap = cJSON_CreateObject();
    writeJSON_2DArray(jtilemap, in_map->tilemap, in_map->row_len, in_map->col_len);
    cJSON_AddItemToObject(in_jmap, "Tilemap", jtilemap);
}

void Map_Unitmap_Clear(struct Map * in_map, tnecs_world_t * in_world) {
    tnecs_entity_t temp_unit_ent;
    for (uint16_t row = 0; row < in_map->row_len; row++) {// This loop cache friendly.
        for (uint16_t col = 0; col < in_map->col_len; col++) {
            temp_unit_ent = in_map->unitmap[row * in_map->col_len + col];
            if (temp_unit_ent != 0) {
                tnecs_entity_destroy(in_world, temp_unit_ent);
            }
        }
    }
    arrfree(in_map->unitmap);
}

void Map_addArmy(struct Map * in_map, const uint8_t in_army) {
    bool found = false;
    for (uint8_t i = 0; i < in_map->num_armies_onfield; i++) {
        if (in_map->armies_onfield[i] == in_army) {
            found = true;
        }
    }
    if (!found) {
        arrput(in_map->armies_onfield, in_army);
        in_map->num_armies_onfield++;
    }
}

void Map_Unit_Remove(struct Map * in_map, const tnecs_entity_t in_entity) {
    size_t * found = NULL;
    if (array_isIn_uint64_t(in_map->friendlies_onfield, in_entity, in_map->num_friendlies_onfield)) {
        found = array_where_uint64_t(in_map->friendlies_onfield, in_entity, in_map->num_friendlies_onfield);
        arrdel(in_map->friendlies_onfield, found[0]);
        in_map->num_friendlies_onfield--;
        arrfree(found);
    }
    if (array_isIn_uint64_t(in_map->enemies_onfield, in_entity, in_map->num_enemies_onfield)) {
        found = array_where_uint64_t(in_map->enemies_onfield, in_entity, in_map->num_enemies_onfield);
        arrdel(in_map->enemies_onfield, found[0]);
        in_map->num_enemies_onfield--;
        arrfree(found);
    }
    if (array_isIn_uint64_t(in_map->units_onfield, in_entity, in_map->num_units_onfield)) {
        found = array_where_uint64_t(in_map->units_onfield, in_entity, in_map->num_units_onfield);
        arrdel(in_map->units_onfield, found[0]);
        in_map->num_units_onfield--;
        arrfree(found);
    }
}

void Map_Unit_Remove_fromPos(struct Map * in_map, const uint8_t col, const uint8_t row) {
    SDL_Log("Map_Unit_Remove_fromPos");
    SDL_assert(in_map->unitmap != NULL);
    tnecs_entity_t ontile_ent = in_map->unitmap[row * in_map->col_len + col];
    in_map->unitmap[row * in_map->col_len + col] = 0;
    Map_Unit_Remove(in_map, ontile_ent);
}

void Map_Unit_Remove_fromEntity(struct Map * in_map, tnecs_world_t * in_world, const tnecs_entity_t in_entity) {
    SDL_Log("Map_Unit_Remove_fromEntity");
    // assumes there are no doubles in the _onfield arrays
    SDL_assert(in_map->unitmap != NULL);
    struct Position * temp_position = TNECS_GET_COMPONENT(in_world, in_entity, Position);
    tnecs_entity_t ontile_ent = 0;
    if (temp_position->onTilemap) {
        ontile_ent = in_map->unitmap[temp_position->tilemap_pos.y * in_map->col_len + temp_position->tilemap_pos.x];
    }
    SDL_assert(ontile_ent != 0);
    if (ontile_ent == in_entity) {
        in_map->unitmap[temp_position->tilemap_pos.y * in_map->col_len + temp_position->tilemap_pos.x] = 0;
    }
    Map_Unit_Remove(in_map, in_entity);
}

void Map_Unit_Put(struct Map * in_map, tnecs_world_t * in_world, const uint8_t col, const uint8_t row, tnecs_entity_t in_entity) {
    SDL_Log("Map_Unit_Put");
    SDL_Log("in_entity %d", in_entity);
    SDL_assert(in_map->unitmap != NULL);
    SDL_assert((row < in_map->row_len) && (col < in_map->col_len));

    size_t typeflag_id = tnecs_typeflagid(in_world, in_world->entity_typeflags[in_entity]);
    SDL_assert(in_world->entities_bytype[typeflag_id][(in_world->num_entities_bytype[typeflag_id] - 1)] == in_entity);
    in_map->unitmap[row * in_map->col_len + col] = in_entity;
    arrput(in_map->units_onfield, in_entity);
    in_map->num_units_onfield++;
    struct Unit * temp_unit = TNECS_GET_COMPONENT(in_world, in_entity, Unit);


    SDL_assert(in_world->entities_bytype[typeflag_id][(in_world->num_entities_bytype[typeflag_id] - 1)] == in_entity);
    SDL_assert(temp_unit != NULL);

    switch (utilities_army2alignment(temp_unit->army)) {
        case ALIGNMENT_FRIENDLY:
            SDL_Log("Putting friendly: %s", temp_unit->name);
            arrput(in_map->friendlies_onfield, in_entity);
            in_map->num_friendlies_onfield++;
            break;
        case ALIGNMENT_ENEMY:
            SDL_Log("Putting enemy : %s", temp_unit->name);
            arrput(in_map->enemies_onfield, in_entity);
            in_map->num_enemies_onfield++;
            break;
    }
    if (temp_unit->_id == 0) {
        Map_addArmy(in_map, temp_unit->army);
    }
}

tnecs_entity_t * Map_Unit_Gets(struct Map * in_map, tnecs_world_t * in_world, const uint8_t in_army) {
    tnecs_entity_t * unit_ents = NULL;
    tnecs_entity_t current_unit_ent;
    struct Unit * current_unit;
    for (uint8_t i = 0; i < in_map->num_units_onfield; i++) {
        current_unit_ent = in_map->units_onfield[i];
        current_unit = TNECS_GET_COMPONENT(in_world, current_unit_ent, Unit);
        if (current_unit->army == in_army) {
            arrput(unit_ents, current_unit_ent);
        }
    }
    return (unit_ents);
}

tnecs_entity_t Map_Unit_Get(struct Map * in_map, const uint8_t col, const uint8_t row) {
    SDL_Log("Map_Unit_Get");
    tnecs_entity_t unit_ent = 0;
    SDL_assert(in_map->unitmap != NULL);
    SDL_assert((col < in_map->col_len) && (row < in_map->row_len));
    unit_ent = in_map->unitmap[row * in_map->col_len + col];
    return (unit_ent);
}

void Map_Unit_Move(struct Map * in_map, const uint8_t col, const uint8_t row, const uint8_t new_col, const uint8_t new_row) {
    // Does NOT check if [new_x, new_y] is empty.
    SDL_Log("Map_Unit_Move %d %d %d %d", col, row, new_col, new_row);
    SDL_assert(in_map->unitmap != NULL);
    SDL_assert((col < in_map->col_len) && (row < in_map->row_len) && (new_col < in_map->col_len) && (new_row < in_map->row_len));
    in_map->unitmap[new_row * in_map->col_len + new_col] = in_map->unitmap[row * in_map->col_len + col];
    in_map->unitmap[row * in_map->col_len + col] = 0;

}

void Map_removeMapArrival(struct Map * in_map, const uint8_t index) {
    arrput(in_map->loaded_arrivals, in_map->arrivals[index]);
    arrdel(in_map->arrivals, index);
}

int_tile_t * Map_Costmap_PushPull_Compute(struct Map * in_map, tnecs_world_t * in_world, tnecs_entity_t in_unit_ent) {
    SDL_Log("Map_Costmap_PushPull_Compute");
    int_tile_t * costmap = NULL;
    arrsetlen(costmap, in_map->row_len * in_map->col_len);
    SDL_assert(in_unit_ent != 0);
    struct Unit * in_unit_ptr = TNECS_GET_COMPONENT(in_world, in_unit_ent, Unit);
    struct Tile * temp_tile;
    int16_t tile_ind = 0;
    int8_t in_unit_movetype = in_unit_ptr->mvt_type;
    uint8_t in_army = in_unit_ptr->army;
    uint8_t ontile_army;
    tnecs_entity_t ontile_unit_ent;
    SDL_assert(in_unit_movetype > UNIT_MVT_START);
    for (uint8_t row = 0; row < in_map->row_len; row++) {
        for (uint8_t col = 0; col < in_map->col_len; col++) {
            tile_ind = in_map->tilemap[row * in_map->col_len + col] / DEFAULT_TILE_DIVISOR;
            ontile_unit_ent = in_map->unitmap[row * in_map->col_len + col];
            temp_tile = hmget(in_map->tiles, tile_ind);
            costmap[row * in_map->col_len + col] = temp_tile->cost_array[in_unit_movetype];
            if (ontile_unit_ent != 0) {
                struct Unit * ontile_unit_ptr = TNECS_GET_COMPONENT(in_world, ontile_unit_ent, Unit);
                if (ontile_unit_ptr != NULL) {
                    costmap[row * in_map->col_len + col] = COSTMAP_BLOCKED;
                }
            }
        }
    }
    return (costmap);
}

int_tile_t * Map_Costmap_Movement_Compute(struct Map * in_map, tnecs_world_t * in_world, tnecs_entity_t in_unit_ent) {
    SDL_Log("Map_Costmap_Movement_Compute");
    int_tile_t * costmap = NULL;
    arrsetlen(costmap, in_map->row_len * in_map->col_len);
    SDL_assert(in_unit_ent != 0);
    struct Unit * in_unit_ptr = TNECS_GET_COMPONENT(in_world, in_unit_ent, Unit);
    struct Tile * temp_tile;
    int16_t tile_ind = 0;
    int8_t in_unit_movetype = in_unit_ptr->mvt_type;
    uint8_t in_army = in_unit_ptr->army;
    uint8_t ontile_army;
    tnecs_entity_t ontile_unit_ent;

    SDL_Log("in_unit_movetype: %d", in_unit_movetype);
    SDL_assert(in_unit_movetype > UNIT_MVT_START);
    for (uint8_t row = 0; row < in_map->row_len; row++) {
        for (uint8_t col = 0; col < in_map->col_len; col++) {
            tile_ind = in_map->tilemap[row * in_map->col_len + col] / DEFAULT_TILE_DIVISOR;
            ontile_unit_ent = in_map->unitmap[row * in_map->col_len + col];
            temp_tile = hmget(in_map->tiles, tile_ind);
            costmap[row * in_map->col_len + col] = temp_tile->cost_array[in_unit_movetype];
            if (ontile_unit_ent != 0) {
                struct Unit * ontile_unit_ptr = TNECS_GET_COMPONENT(in_world, ontile_unit_ent, Unit);
                SDL_assert(ontile_unit_ptr != NULL);
                ontile_army = ontile_unit_ptr->army;
                SDL_assert((ontile_army < ARMY_END) && (ontile_army > ARMY_START));
                if (utilities_army2alignment(ontile_army) != utilities_army2alignment(in_army)) {
                    costmap[row * in_map->col_len + col] = COSTMAP_BLOCKED;
                }
            }
        }
    }
    return (costmap);
}

void Map_Tilesize_Set(struct Map * in_map, int16_t width, int16_t height) {
    in_map->tilesize[0] = width;
    in_map->tilesize[1] = height;
    in_map->srcrect.x = 0;
    in_map->srcrect.y = 0;
    in_map->srcrect.w = width;
    in_map->srcrect.h = height;
    in_map->destrect.x = 0;
    in_map->destrect.y = 0;
    in_map->destrect.w = width;
    in_map->destrect.h = height;
}


void Map_unloadTiles(struct Map * in_map, int16_t * to_unload, size_t len) {
    for (uint16_t i = 0; i < len; i++) {
        hmdel(in_map->tiles, to_unload[i]);
    }
}

void Map_Renderer_Set(struct Map * in_map, SDL_Renderer * in_renderer) {
    SDL_Log("Setting Map renderer");
    SDL_assert(in_renderer);
    in_map->renderer = in_renderer;
}

void Map_tileTextures_Load(struct Map * in_map) {
    SDL_Log("Map_tileTextures_Load");
    SDL_assert(in_map->tilesindex != NULL);
    uint16_t tile_ind;
    char texturename[DEFAULT_BUFFER_SIZE] = "";
    char numbuff[DEFAULT_BUFFER_SIZE];
    SDL_Texture * temp_texture;
    struct Tile * temp_tile;
    for (size_t i = 0; i < in_map->tiles_num; i++) {
        // temp_texture = NULL;
        temp_tile = NULL;
        if (in_map->tilesindex[i] > DEFAULT_TILE_MAX) {
            tile_ind = in_map->tilesindex[i] / DEFAULT_TILE_DIVISOR;
        } else {
            tile_ind = in_map->tilesindex[i];
        }
        SDL_Log("tile_ind %d", tile_ind);
        strcat(texturename, ".."DIR_SEPARATOR"assets"DIR_SEPARATOR"Tiles"DIR_SEPARATOR);
        temp_tile = (hmget(in_map->tiles, tile_ind));
        SDL_Log("temp_tile id %d", temp_tile->id);
        SDL_Log("temp_tile name %s", temp_tile->name);
        strcat(texturename, temp_tile->name);
        strcat(texturename, "_");
        stbsp_sprintf(numbuff, "%03d", in_map->tilesindex[i]);
        strcat(texturename, numbuff);
        SDL_Log("numbuff: %s", numbuff);
        strcat(texturename, ".png");
        hmput(in_map->textures, (in_map->tilesindex[i]), loadTexture(in_map->renderer, texturename, false));
        memset(&texturename, 0, DEFAULT_BUFFER_SIZE);
    }
}

void Map_overlayTextures_load(struct Map * in_map) {
    SDL_Log("Map_loadOverlays");
    in_map->overlays[(uint8_t)log2(MAP_OVERLAY_MOVE) - 1] = loadTexture(in_map->renderer, "tile_overlay_move.png", true);
    in_map->overlays[(uint8_t)log2(MAP_OVERLAY_ATTACK) - 1] = loadTexture(in_map->renderer, "tile_overlay_attack.png", true);
    in_map->overlays[(uint8_t)log2(MAP_OVERLAY_HEAL) - 1] = loadTexture(in_map->renderer, "tile_overlay_heal.png", true);
}

void Map_Danger_Load(struct Map * in_map) {
    SDL_Log("Loading Map dangerzone");
    in_map->dangers[0] = loadTexture(in_map->renderer, ".."DIR_SEPARATOR"assets"DIR_SEPARATOR"Tiles"DIR_SEPARATOR"tile_overlay_danger.png", false);
}

void Map_Danger_Add(struct Map * in_map, int16_t * in_danger) {
    in_map->overlay_danger = linalg_plusint16_t(in_map->overlay_danger, in_danger, in_map->row_len, in_map->col_len, 1);
}

void Map_Danger_Sub(struct Map * in_map, int16_t * in_danger) {
    in_map->overlay_danger = linalg_plusint16_t(in_map->overlay_danger, in_danger, in_map->row_len, in_map->col_len, -1);
}

void Map_Bounds_Compute(struct Map * in_map) {
    SDL_Log("Map_Bounds_Compute");
    SDL_Log("%d %d", in_map->col_len, in_map->row_len);
    SDL_assert((in_map->col_len != 0) | (in_map->row_len != 0));
    // in_map->boundsmin.x = in_map->offset_px.x;
    in_map->boundsmin.x = 0;
    in_map->boundsmax.x = in_map->col_len - 1;
    // in_map->boundsmin.y = in_map->offset_px.y;
    in_map->boundsmin.y = 0;
    in_map->boundsmax.y = in_map->row_len - 1;
    SDL_Log("%d %d %d %d", in_map->boundsmin.x, in_map->boundsmax.x, in_map->boundsmin.y, in_map->boundsmax.y);
}

void Map_Overlays_Set(struct Map * in_map, const uint8_t in_mode, int16_t * in_overlay) {
    // SDL_Log("Map_Overlays_Set");
    if ((in_mode & MAP_OVERLAY_HEAL) > 0) {
        if (in_map->overlay_heal != NULL) {
            arrfree(in_map->overlay_heal);
        }
        arrsetlen(in_map->overlay_heal, in_map->row_len * in_map->col_len);
        for (uint16_t row = 0; row < in_map->row_len; row++) {// This loop cache friendly.
            for (uint16_t col = 0; col < in_map->col_len; col++) {
                in_map->overlay_heal[row * in_map->col_len + col] = in_overlay[row * in_map->col_len + col];
            }
        }
    }

    if ((in_mode & MAP_OVERLAY_ATTACK) > 0) {
        if (in_map->overlay_attack != NULL) {
            arrfree(in_map->overlay_attack);
        }
        arrsetlen(in_map->overlay_attack, in_map->row_len * in_map->col_len);
        for (uint16_t row = 0; row < in_map->row_len; row++) {// This loop cache friendly.
            for (uint16_t col = 0; col < in_map->col_len; col++) {
                in_map->overlay_attack[row * in_map->col_len + col] = in_overlay[row * in_map->col_len + col];
            }
        }
    }

    if ((in_mode & MAP_OVERLAY_MOVE) > 0) {
        if (in_map->overlay_move != NULL) {
            arrfree(in_map->overlay_move);
        }
        arrsetlen(in_map->overlay_move, in_map->row_len * in_map->col_len);
        for (int_tile_t row = 0; row < in_map->row_len; row++) {// This loop cache friendly.
            for (int_tile_t col = 0; col < in_map->col_len; col++) {
                in_map->overlay_move[row * in_map->col_len + col] = in_overlay[row * in_map->col_len + col];
            }
        }
    }
}

void Map_Overlays_Init(struct Map * in_map) {
    Map_Overlays_Clear(in_map);
    arrsetlen(in_map->overlay_attack, in_map->row_len * in_map->col_len);
    arrsetlen(in_map->overlay_move, in_map->row_len * in_map->col_len);
    arrsetlen(in_map->overlay_heal, in_map->row_len * in_map->col_len);
    arrsetlen(in_map->overlay_danger, in_map->row_len * in_map->col_len);
    SDL_Log("%d", in_map->row_len * in_map->col_len);

    for (uint8_t row = 0; row < in_map->row_len; row++) {
        for (uint8_t col = 0; col < in_map->col_len; col++) {
            in_map->overlay_heal[row * in_map->col_len + col] = 0;
            in_map->overlay_move[row * in_map->col_len + col] = 0;
            in_map->overlay_attack[row * in_map->col_len + col] = 0;
            in_map->overlay_danger[row * in_map->col_len + col] = 0;
        }
    }
}

void Map_Overlays_Clear(struct Map * in_map) {
    if (in_map->overlay_attack != NULL) {
        arrfree(in_map->overlay_attack);
    }
    if (in_map->overlay_move != NULL) {
        arrfree(in_map->overlay_move);
    }
    if (in_map->overlay_heal != NULL) {
        arrfree(in_map->overlay_heal);
    }
    if (in_map->overlay_danger != NULL) {
        arrfree(in_map->overlay_danger);
    }
}

void Map_draw(struct Map * in_map) {
    // SDL_Log("Map_draw %d", in_map->mode_overlay);
    int16_t tile_ind = 0;
    for (uint16_t row = 0; row < in_map->row_len; row++) {// This loop cache friendly.
        for (uint16_t col = 0; col < in_map->col_len; col++) {
            tile_ind = in_map->tilemap[(row * in_map->col_len + col)];
            // in_map->destrect.x = (col + in_map->offset.x) * in_map->tilesize[0];
            in_map->destrect.x = col * in_map->tilesize[0] + in_map->offset_px.x;
            // in_map->destrect.y = (row + in_map->offset.y) * in_map->tilesize[1];
            in_map->destrect.y = row * in_map->tilesize[1] + in_map->offset_px.y;
            SDL_RenderCopy(in_map->renderer, hmget(in_map->textures, tile_ind), &in_map->srcrect, &in_map->destrect);

            if (in_map->show_overlay) {
                if (((in_map->mode_overlay & MAP_OVERLAY_MOVE) > 0) && (in_map->overlays[0] != NULL)) {
                    if (in_map->overlay_move[row * in_map->col_len + col] > 0) {
                        SDL_RenderCopy(in_map->renderer, in_map->overlays[(uint8_t)log2(MAP_OVERLAY_MOVE) - 1], &in_map->srcrect, &in_map->destrect);
                    }
                }
                if (((in_map->mode_overlay & MAP_OVERLAY_ATTACK) > 0)  && (in_map->overlays[1] != NULL)) {
                    if (in_map->overlay_attack[row * in_map->col_len + col] == 1) {
                        SDL_RenderCopy(in_map->renderer, in_map->overlays[(int8_t)log2(MAP_OVERLAY_ATTACK) - 1], &in_map->srcrect, &in_map->destrect);
                    }
                }
                if (((in_map->mode_overlay & MAP_OVERLAY_HEAL) > 0) && (in_map->overlays[2] != NULL)) {
                    if (in_map->overlay_heal[row * in_map->col_len + col] == 1) {
                        SDL_RenderCopy(in_map->renderer, in_map->overlays[(int8_t)log2(MAP_OVERLAY_HEAL) - 1], &in_map->srcrect, &in_map->destrect);
                    }
                }
            }
            if (in_map->show_danger) {
                if (in_map->overlay_danger[row * in_map->col_len + col] > 0) {
                    SDL_RenderCopy(in_map->renderer, in_map->dangers[0], &in_map->srcrect, &in_map->destrect);
                }
            }
            if (in_map->show_globalRange) {
                if (in_map->globalRange[row * in_map->col_len + col] > 0) {
                    switch (in_map->mode_globalRange) {
                        case ALIGNMENT_ENEMY:
                            SDL_RenderCopy(in_map->renderer, in_map->dangers[0], &in_map->srcrect, &in_map->destrect);
                            break;
                        case ALIGNMENT_FRIENDLY:
                            SDL_RenderCopy(in_map->renderer, in_map->overlays[(uint8_t)log2(MAP_OVERLAY_MOVE) - 1], &in_map->srcrect, &in_map->destrect);
                            break;
                    }
                }
            }
        }
    }
}
