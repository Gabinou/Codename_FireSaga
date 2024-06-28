
#include "map/ontile.h"

/* --- Entity finders --- */
void Map_startingPos_Add(struct Map *map, i32 col, i32 row) {
    struct Point pos = {col, row};

    if (Point_isIn(pos, map->start_pos, DARR_NUM(map->start_pos))) {
        SDL_LogError(SOTA_LOG_SYSTEM, "Start position %d %d not unique", col, row);
        exit(ERROR_Generic);
    }
    map->start_posmap[row * map->col_len + col] = 1;
    DARR_PUT(map->start_pos, pos);
}

void _Map_Unit_Put(struct Map *map, u8 col, u8 row, tnecs_entity entity) {

    /* -- Updating unit pos -- */
    if (entity > TNECS_NULL) {
        size_t index = row * map->col_len + col;
        struct Position *pos = TNECS_GET_COMPONENT(map->world, entity, Position);
        pos->tilemap_pos.x   = col;
        pos->tilemap_pos.y   = row;
        pos->pixel_pos.x     = pos->tilemap_pos.x * pos->scale[0];
        pos->pixel_pos.y     = pos->tilemap_pos.y * pos->scale[1];
        SDL_Log("_Map_Unit_Put %lu %d %d", entity, pos->tilemap_pos.x, pos->tilemap_pos.y);
        map->unitmap[index]  = entity;
    }
}

void Map_Unit_Put(struct Map *map, u8 col, u8 row, tnecs_entity entity) {
    SDL_Log("%lu", entity);
    SDL_assert(map->world != NULL);
    SDL_assert(map->unitmap != NULL);
    SDL_assert((row < map->row_len) && (col < map->col_len));
    SDL_assert(entity);

    /* -- Preliminaries -- */
    tnecs_entity current = map->unitmap[row * map->col_len + col];
    _Map_Unit_Put(map, col, row, entity);
    DARR_PUT(map->units_onfield, entity);

    /* -- Adding MapHPBar -- */
    if (!TNECS_ENTITY_HASCOMPONENT(map->world, entity, MapHPBar)) {
        TNECS_ADD_COMPONENT(map->world, entity, MapHPBar);
        struct MapHPBar *map_hp_bar = TNECS_GET_COMPONENT(map->world, entity, MapHPBar);
        *map_hp_bar = MapHPBar_default;
        map_hp_bar->unit_ent = entity;
        map_hp_bar->len = map->tilesize[0];
    }

    /* -- Updating unit pos -- */
    struct Unit     *temp_unit   = TNECS_GET_COMPONENT(map->world, entity, Unit);
    struct Sprite   *temp_sprite = TNECS_GET_COMPONENT(map->world, entity, Sprite);

    if (temp_sprite != NULL)
        temp_sprite->visible = true;
    SDL_assert(temp_unit != NULL);

    /* -- Adding unit to army list -- */
    switch (army_alignment[temp_unit->army]) {
        case ALIGNMENT_FRIENDLY: {
            SDL_assert(Unit_ID_Valid(temp_unit->_id));
            SDL_assert(global_unitOrders != NULL);
            SDL_assert(dtab_get(global_unitOrders, temp_unit->_id) != NULL);
            u16 order = *(u16 *)dtab_get(global_unitOrders, temp_unit->_id);
            SDL_assert(order > 0);
            SDL_assert(order < UNIT_NUM);
            DARR_PUT(map->friendlies_onfield, entity);
            break;
        }
        case ALIGNMENT_ENEMY: {
            SDL_assert(Unit_ID_Valid(temp_unit->_id));
            SDL_assert(global_unitOrders != NULL);
            SDL_assert(dtab_get(global_unitOrders, temp_unit->_id) != NULL);
            u16 order = *(u16 *)dtab_get(global_unitOrders, temp_unit->_id);
            SDL_assert(order > 0);
            SDL_assert(order < UNIT_NUM);
            DARR_PUT(map->enemies_onfield, entity);
            break;
        }
    }
    SDL_assert(temp_unit->_id > UNIT_ID_NULL);
    SDL_assert(temp_unit->_id < UNIT_ID_END);

    Map_addArmy(map, temp_unit->army);
}

void Map_Unit_Swap(struct Map *map, u8 old_col, u8 old_row, u8 new_col, u8 new_row) {
    /* New position of unit should always be empty */
    size_t old_i = old_row * map->col_len + old_col;
    size_t new_i = new_row * map->col_len + new_col;
    tnecs_entity unit_old = map->unitmap[old_i];
    tnecs_entity unit_new = map->unitmap[new_i];

    _Map_Unit_Put(map, new_col, new_row, unit_old);
    _Map_Unit_Put(map, old_col, old_row, unit_new);
}

void Map_Unit_Move(struct Map *map, u8 col, u8 row, u8 new_col, u8 new_row) {
    SDL_Log(__func__);
    /* Note: Does NOT check if [new_x, new_y] is empty. */
    SDL_assert(map->unitmap != NULL);
    SDL_assert(col      < map->col_len);
    SDL_assert(row      < map->row_len);
    SDL_assert(new_col  < map->col_len);
    SDL_assert(new_row  < map->row_len);

    /* New position of unit should always be empty */
    size_t old_i = row      * map->col_len + col;
    size_t new_i = new_row  * map->col_len + new_col;

    /* -- Move unit on unitmap -- */
    tnecs_entity entity = map->unitmap[old_i];
    SDL_assert(map->unitmap[old_i] > TNECS_NULL);

    _Map_Unit_Put(map, new_col, new_row, map->unitmap[old_i]);

    SDL_assert(map->unitmap[new_i] > TNECS_NULL);
    SDL_assert(map->unitmap[new_i] == entity);
    struct Position *pos = TNECS_GET_COMPONENT(map->world, entity, Position);
    SDL_assert(pos->tilemap_pos.x == new_col);
    SDL_assert(pos->tilemap_pos.y == new_row);
    map->unitmap[old_i] = TNECS_NULL;
}

tnecs_entity *Map_Unit_Gets(struct Map *map, u8 army) {
    tnecs_entity *unit_ents = NULL;
    unit_ents = DARR_INIT(unit_ents, tnecs_entity, 16);
    tnecs_entity current_unit_ent;
    struct Unit *current_unit;
    for (u8 i = 0; i < DARR_NUM(map->units_onfield); i++) {
        current_unit_ent = map->units_onfield[i];
        current_unit = TNECS_GET_COMPONENT(map->world, current_unit_ent, Unit);
        if (current_unit->army == army)
            DARR_PUT(unit_ents, current_unit_ent);
    }
    return (unit_ents);
}

tnecs_entity Map_Unit_Get_Boss(struct Map *map, u8 army) {
    tnecs_entity out = TNECS_NULL;

    for (int i = 0; i < DARR_NUM(map->units_onfield); i++) {
        tnecs_entity ent = map->units_onfield[i];
        struct Unit *unit = TNECS_GET_COMPONENT(map->world, ent, Unit);
        struct Unit *boss = TNECS_GET_COMPONENT(map->world, ent, Boss);
        if ((unit->army == army) && (boss != NULL)) {
            out = ent;
            break;
        }
    }
    return (out);
}


tnecs_entity Map_Unit_Get(struct Map *map, u8 col, u8 row) {
    SDL_assert(map->unitmap != NULL);
    SDL_assert(col < map->col_len);
    SDL_assert(row < map->row_len);
    return (map->unitmap[row * map->col_len + col]);
}

void Map_Breakable_onBroken(struct Map *map, tnecs_entity breakable) {
}

void Map_Door_onOpen(struct Map *map, tnecs_entity door) {
}

void Map_Chest_onOpen(struct Map *map, tnecs_entity chest) {
}

void Map_addArmy(struct Map *map, u8 army) {
    /* -- Don't add army if found -- */
    for (u8 i = 0; i < DARR_NUM(map->army_onfield); i++)
        if (map->army_onfield[i] == army)
            return;

    /* -- Army priority: smaller army go first -- */
    int insert = 0;
    /* If DARR_NUM(map->army_onfield) is 0, insert is 0 */
    for (int i = 0; i < DARR_NUM(map->army_onfield); i++) {
        if (army > map->army_onfield[i]) {
            insert = i;
            break;
        }
    }

    DARR_INSERT(map->army_onfield, army, insert);
}

// size_t *entity_where(u64 *array, u64 to_find, size_t arr_len) {
//     size_t *found_list = DARR_INIT(found_list, size_t, arr_len);
//     for (size_t i = 0; i < arr_len; i++) {
//         if (array[i] == to_find) {
//             DARR_PUT(found_list, i);
//             break;
//         }
//     }
//     DARR_LEN(found_list) = DARR_NUM(found_list);
//     size_t newl = (DARR_NUM(found_list) < SOTA_MINLEN ? SOTA_MINLEN : DARR_NUM(found_list));
//     found_list = DARR_REALLOC(found_list, newl);
//     return (found_list);
// }

int entity_isIn(u64 *array, u64 to_find, size_t arr_len) {
    int found = -1;
    for (size_t i = 0; i < arr_len; i++) {
        if (array[i] == to_find)
            return (i);
    }
    return (found);
}

void _Map_Unit_Remove_List(struct Map *map,  tnecs_entity entity) {
    int found = entity_isIn(map->friendlies_onfield, entity, DARR_NUM(map->friendlies_onfield));
    if (found > -1)
        DARR_DEL(map->friendlies_onfield, found);

    found = entity_isIn(map->enemies_onfield, entity, DARR_NUM(map->enemies_onfield));
    if (found > -1)
        DARR_DEL(map->enemies_onfield, found);

    found = entity_isIn(map->units_onfield, entity, DARR_NUM(map->units_onfield));
    if (found > -1)
        DARR_DEL(map->units_onfield, found);
}

void _Map_Unit_Remove_Map(struct Map *map, u8 col, u8 row) {
    SDL_assert(map->unitmap != NULL);
    map->unitmap[row * map->col_len + col] = 0;
}

void Map_Unit_Remove(struct Map *map, tnecs_entity entity) {
    SDL_assert(entity > TNECS_NULL);
    SDL_assert(map->world   != NULL);
    SDL_assert(map->unitmap != NULL);

    /* --- Check that entity is really on map --- */
    struct Position *pos = TNECS_GET_COMPONENT(map->world, entity, Position);
    SDL_assert(pos->onTilemap);

    SDL_Log("pos->tilemap_pos %d %d", pos->tilemap_pos.x, pos->tilemap_pos.y);
    int index = pos->tilemap_pos.y * map->col_len + pos->tilemap_pos.x;
    tnecs_entity ontile_ent = map->unitmap[index];
    SDL_assert(ontile_ent > TNECS_NULL);
    SDL_Log("%s %d %d", __func__, ontile_ent, map->unitmap[index]);
    SDL_assert(ontile_ent == entity);
    struct Sprite *sprite = TNECS_GET_COMPONENT(map->world, entity, Sprite);
    if (sprite != NULL)
        sprite->visible = false;

    if (TNECS_ENTITY_HASCOMPONENT(map->world, entity, MapHPBar))
        TNECS_REMOVE_COMPONENTS(map->world, entity, MapHPBar);

    /* --- Check that entity is really on map --- */
    map->unitmap[index] = 0;
    _Map_Unit_Remove_List(map, entity);
}
