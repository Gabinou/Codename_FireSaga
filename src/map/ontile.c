
#include "map/ontile.h"

/* --- Entity finders --- */
bool entity_isIn(u64 *array, u64 to_find, size_t arr_len) {
    bool found = false;
    for (size_t i = 0; i < arr_len; i++) {
        if (array[i] == to_find) {
            found = true;
            break;
        }
    }
    return (found);
}

size_t *entity_where(u64 *array, u64 to_find, size_t arr_len) {
    size_t *found_list = DARR_INIT(found_list, size_t, arr_len);
    for (size_t i = 0; i < arr_len; i++) {
        if (array[i] == to_find) {
            DARR_PUT(found_list, i);
            break;
        }
    }
    DARR_LEN(found_list) = DARR_NUM(found_list);
    size_t newl = (DARR_NUM(found_list) < SOTA_MINLEN ? SOTA_MINLEN : DARR_NUM(found_list));
    found_list = DARR_REALLOC(found_list, newl);
    return (found_list);
}

void Map_startingPos_Add(struct Map *map, i32 col, i32 row) {
    struct Point pos = {col, row};
    DARR_PUT(map->start_pos, pos);
}

void Map_Unit_Put(struct Map *map, tnecs_world_t *world, u8 col, u8 row,
                  tnecs_entity_t entity) {
    SDL_Log("%lu", entity);
    SDL_assert(map->unitmap != NULL);
    SDL_assert((row < map->row_len) && (col < map->col_len));
    SDL_assert(entity);

    /* -- Preliminaries -- */
    tnecs_entity_t current = map->unitmap[row * map->col_len + col];
    if (current != TNECS_NULL) {
        SDL_Log("Unit already on map");
        exit(ERROR_Generic);
    }
    map->unitmap[row * map->col_len + col] = entity;
    DARR_PUT(map->units_onfield, entity);
    map->num_units_onfield++;

    /* -- Updating unit pos -- */
    struct Position *pos         = TNECS_GET_COMPONENT(world, entity, Position);
    struct Unit     *temp_unit   = TNECS_GET_COMPONENT(world, entity, Unit);
    struct Sprite   *temp_sprite = TNECS_GET_COMPONENT(world, entity, Sprite);
    pos->tilemap_pos.x = col;
    pos->tilemap_pos.y = row;

    if (temp_sprite != NULL)
        temp_sprite->visible = true;
    SDL_assert(temp_unit != NULL);

    /* -- Adding unit to army list -- */
    switch (SotA_army2alignment(temp_unit->army)) {
        case ALIGNMENT_FRIENDLY: {
            SDL_assert(Unit_ID_Valid(temp_unit->_id));
            SDL_assert(global_unitOrders != NULL);
            SDL_assert(dtab_get(global_unitOrders, temp_unit->_id) != NULL);
            u16 index = *(u16 *)dtab_get(global_unitOrders, temp_unit->_id);
            SDL_assert(index != 0);
            SDL_assert(index < DARR_NUM(global_unitNames));
            DARR_PUT(map->friendlies_onfield, entity);
            map->num_friendlies_onfield++;
            break;
        }
        case ALIGNMENT_ENEMY: {
            SDL_assert(Unit_ID_Valid(temp_unit->_id));
            SDL_assert(global_unitOrders != NULL);
            SDL_assert(dtab_get(global_unitOrders, temp_unit->_id) != NULL);
            u16 index = *(u16 *)dtab_get(global_unitOrders, temp_unit->_id);
            SDL_assert(index != 0);
            SDL_assert(index < DARR_NUM(global_unitNames));
            DARR_PUT(map->enemies_onfield, entity);
            map->num_enemies_onfield++;
            break;
        }
    }
    if (temp_unit->_id == 0)
        Map_addArmy(map, temp_unit->army);
}

void Map_Unit_Move(struct Map *map, u8 col, u8 row, u8 new_col, u8 new_row) {
    // Note: Does NOT check if [new_x, new_y] is empty.
    SDL_assert(map->unitmap != NULL);
    SDL_assert(col      < map->col_len);
    SDL_assert(row      < map->row_len);
    SDL_assert(new_col  < map->col_len);
    SDL_assert(new_row  < map->row_len);

    /* -- Update unitmap -- */
    size_t old_i = row * map->col_len + col;
    size_t new_i = new_row * map->col_len + new_col;
    tnecs_entity_t unit = map->unitmap[old_i];
    map->unitmap[new_i] = unit;
    map->unitmap[old_i] = TNECS_NULL;

    /* -- Updating unit pos -- */
    struct Position *pos = TNECS_GET_COMPONENT(map->world, unit, Position);
    pos->tilemap_pos.x   = new_col;
    pos->tilemap_pos.y   = new_row;

}

tnecs_entity_t *Map_Units_Get(struct Map *map, tnecs_world_t *world, const u8 army) {
    tnecs_entity_t *unit_ents = NULL;
    unit_ents = DARR_INIT(unit_ents, tnecs_entity_t, 16);
    tnecs_entity_t current_unit_ent;
    struct Unit *current_unit;
    for (u8 i = 0; i < map->num_units_onfield; i++) {
        current_unit_ent = map->units_onfield[i];
        current_unit = TNECS_GET_COMPONENT(world, current_unit_ent, Unit);
        if (current_unit->army == army)
            DARR_PUT(unit_ents, current_unit_ent);
    }
    return (unit_ents);
}

tnecs_entity_t Map_Unit_Get(struct Map *map, u8 col, u8 row) {
    SDL_assert(map->unitmap != NULL);
    SDL_assert(col < map->col_len);
    SDL_assert(row < map->row_len);
    return (map->unitmap[row * map->col_len + col]);
}

void Map_Breakable_onBroken(struct Map *map, tnecs_world_t *world, tnecs_entity_t breakable) {
}

void Map_Door_onOpen(struct Map *map, tnecs_world_t *world, tnecs_entity_t door) {
}

void Map_Chest_onOpen(struct Map *map, tnecs_world_t *world, tnecs_entity_t chest) {
}

void Map_addArmy(struct Map *map, const u8 army) {
    bool found = false;
    for (u8 i = 0; i < map->num_armies_onfield; i++)
        found |= (map->armies_onfield[i] == army);

    if (!found) {
        DARR_PUT(map->armies_onfield, army);
        map->num_armies_onfield++;
    }
}

void Map_Unit_Remove(struct Map *map, const tnecs_entity_t entity) {
    size_t *found = NULL;
    if (entity_isIn(map->friendlies_onfield, entity, map->num_friendlies_onfield)) {
        found = entity_where(map->friendlies_onfield, entity, map->num_friendlies_onfield);
        DARR_DEL(map->friendlies_onfield, found[0]);
        map->num_friendlies_onfield--;
        free(found);
    }
    if (entity_isIn(map->enemies_onfield, entity, map->num_enemies_onfield)) {
        found = entity_where(map->enemies_onfield, entity, map->num_enemies_onfield);
        DARR_DEL(map->enemies_onfield, found[0]);
        map->num_enemies_onfield--;
        free(found);
    }
    if (entity_isIn(map->units_onfield, entity, map->num_units_onfield)) {
        found = entity_where(map->units_onfield, entity, map->num_units_onfield);
        DARR_DEL(map->units_onfield, found[0]);
        map->num_units_onfield--;
        free(found);
    }
}

void Map_Unit_Remove_fromPos(struct Map *map, u8 col, u8 row) {
    SDL_assert(map->unitmap != NULL);
    tnecs_entity_t ontile_ent = map->unitmap[row * map->col_len + col];
    map->unitmap[row * map->col_len + col] = 0;
    Map_Unit_Remove(map, ontile_ent);
}

void Map_Unit_Remove_fromEntity(struct Map *map, tnecs_world_t *world, tnecs_entity_t entity) {
    // assumes there are no doubles in the _onfield arrays
    SDL_assert(map->unitmap != NULL);
    struct Position *pos = TNECS_GET_COMPONENT(world, entity, Position);
    tnecs_entity_t ontile_ent = 0;
    if (pos->onTilemap)
        ontile_ent = map->unitmap[pos->tilemap_pos.y * map->col_len + pos->tilemap_pos.x];
    SDL_assert(ontile_ent != 0);
    if (ontile_ent == entity)
        map->unitmap[pos->tilemap_pos.y * map->col_len + pos->tilemap_pos.x] = 0;
    Map_Unit_Remove(map, entity);
}
