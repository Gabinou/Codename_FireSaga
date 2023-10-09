
#include "map/ontile.h"

void Map_startingPos_Add(struct Map *map, i32 col, i32 row) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Point pos = {col, row};
    DARR_PUT(map->start_pos, pos);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Unit_Put(struct Map *map, tnecs_world_t *world, uf8 col, uf8 row,
                  tnecs_entity_t entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("%lu", entity);
    SDL_assert(map->unitmap != NULL);
    SDL_assert((row < map->row_len) && (col < map->col_len));
    SDL_assert(entity);

    /* -- Preliminaries -- */
    tnecs_entity_t current = map->unitmap[row * map->col_len + col];
    if (current != TNECS_NULL) {
        SOTA_Log_Debug("Unit already on map");
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
            SOTA_Log_Debug("id  UNIT_ID_CORSAIR %d %d", temp_unit->_id, UNIT_ID_CORSAIR);
            SDL_assert(global_unitOrders != NULL);
            SDL_assert(dtab_get(global_unitOrders, temp_unit->_id) != NULL);
            SOTA_Log_Debug("id %d", temp_unit->_id);
            uf16 index = *(uf16 *)dtab_get(global_unitOrders, temp_unit->_id);
            SOTA_Log_Debug("int %d", index);
            SDL_assert(index != 0);
            SDL_assert(index < DARR_NUM(global_unitNames));
            SOTA_Log_Debug("Putting friendly: %s", global_unitNames[index]);
            DARR_PUT(map->friendlies_onfield, entity);
            map->num_friendlies_onfield++;
            break;
        }
        case ALIGNMENT_ENEMY: {
            SDL_assert(Unit_ID_Valid(temp_unit->_id));
            SOTA_Log_Debug("id  UNIT_ID_CORSAIR %d %d", temp_unit->_id, UNIT_ID_CORSAIR);
            SDL_assert(global_unitOrders != NULL);
            SDL_assert(dtab_get(global_unitOrders, temp_unit->_id) != NULL);
            SOTA_Log_Debug("id %d", temp_unit->_id);
            uf16 index = *(uf16 *)dtab_get(global_unitOrders, temp_unit->_id);
            SOTA_Log_Debug("int %d", index);
            SDL_assert(index != 0);
            SDL_assert(index < DARR_NUM(global_unitNames));
            SOTA_Log_Debug("Putting enemy : %s", global_unitNames[index]);
            DARR_PUT(map->enemies_onfield, entity);
            map->num_enemies_onfield++;
            break;
        }
    }
    if (temp_unit->_id == 0)
        Map_addArmy(map, temp_unit->army);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Unit_Move(struct Map *map, uf8 col, uf8 row, uf8 new_col, uf8 new_row) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

tnecs_entity_t *Map_Units_Get(struct Map *map, tnecs_world_t *world, const uf8 army) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tnecs_entity_t *unit_ents = NULL;
    unit_ents = DARR_INIT(unit_ents, tnecs_entity_t, 16);
    tnecs_entity_t current_unit_ent;
    struct Unit *current_unit;
    for (uf8 i = 0; i < map->num_units_onfield; i++) {
        current_unit_ent = map->units_onfield[i];
        current_unit = TNECS_GET_COMPONENT(world, current_unit_ent, Unit);
        if (current_unit->army == army)
            DARR_PUT(unit_ents, current_unit_ent);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit_ents);
}

tnecs_entity_t Map_Unit_Get(struct Map *map, uf8 col, uf8 row) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(map->unitmap != NULL);
    SDL_assert(col < map->col_len);
    SDL_assert(row < map->row_len);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (map->unitmap[row * map->col_len + col]);
}

void Map_Breakable_onBroken(struct Map *map, tnecs_world_t *world, tnecs_entity_t breakable) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Door_onOpen(struct Map *map, tnecs_world_t *world, tnecs_entity_t door) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Chest_onOpen(struct Map *map, tnecs_world_t *world, tnecs_entity_t chest) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_addArmy(struct Map *map, const uf8 army) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    bool found = false;
    for (uf8 i = 0; i < map->num_armies_onfield; i++)
        found |= (map->armies_onfield[i] == army);

    if (!found) {
        DARR_PUT(map->armies_onfield, army);
        map->num_armies_onfield++;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Unit_Remove(struct Map *map, const tnecs_entity_t entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    size_t *found = NULL;
    if (linalg_isIn_uint64_t(map->friendlies_onfield, entity, map->num_friendlies_onfield)) {
        found = linalg_where_uint64_t(map->friendlies_onfield, entity, map->num_friendlies_onfield);
        DARR_DEL(map->friendlies_onfield, found[0]);
        map->num_friendlies_onfield--;
        free(found);
    }
    if (linalg_isIn_uint64_t(map->enemies_onfield, entity, map->num_enemies_onfield)) {
        found = linalg_where_uint64_t(map->enemies_onfield, entity, map->num_enemies_onfield);
        DARR_DEL(map->enemies_onfield, found[0]);
        map->num_enemies_onfield--;
        free(found);
    }
    if (linalg_isIn_uint64_t(map->units_onfield, entity, map->num_units_onfield)) {
        found = linalg_where_uint64_t(map->units_onfield, entity, map->num_units_onfield);
        DARR_DEL(map->units_onfield, found[0]);
        map->num_units_onfield--;
        free(found);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Unit_Remove_fromPos(struct Map *map, uf8 col, uf8 row) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(map->unitmap != NULL);
    tnecs_entity_t ontile_ent = map->unitmap[row * map->col_len + col];
    map->unitmap[row * map->col_len + col] = 0;
    Map_Unit_Remove(map, ontile_ent);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Unit_Remove_fromEntity(struct Map *map, tnecs_world_t *world, tnecs_entity_t entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
