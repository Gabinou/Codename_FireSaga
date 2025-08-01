
#include "map/ontile.h"
#include "names.h"
#include "sprite.h"
#include "log.h"
#include "position.h"
#include "nmath.h"
#include "bars/map_hp.h"
#include "unit/flags.h"

/* --- Entity finders --- */
void Map_startingPos_Add(struct Map *map, i32 col, i32 row) {
    struct Point pos = {col, row};

    if (Point_isIn(pos, map->start_pos.arr, DARR_NUM(map->start_pos.arr))) {
        SDL_LogWarn(SOTA_LOG_SYSTEM, "Start position %d %d not unique", col, row);
        return;
    }
    map->start_pos.map[row * Map_col_len(map) + col] = 1;
    DARR_PUT(map->start_pos.arr, pos);
}

void _Map_Unit_Put(struct Map *map, u8 col, u8 row, tnecs_entity entity) {
    SDL_assert(map          != NULL);
    SDL_assert(map->darrs.unitmap != NULL);
    /* -- Updating unit pos -- */
    if (entity <= TNECS_NULL) {
        return;
    }
    size_t index        = row * Map_col_len(map) + col;
    struct Position *pos = IES_GET_COMPONENT(map->world, entity, Position);
    pos->tilemap_pos.x   = col;
    pos->tilemap_pos.y   = row;
    pos->pixel_pos.x     = pos->tilemap_pos.x * pos->scale[0];
    pos->pixel_pos.y     = pos->tilemap_pos.y * pos->scale[1];
    map->darrs.unitmap[index]  = entity;
}

void Map_Unit_Put(struct Map *map, u8 col, u8 row, tnecs_entity entity) {
    SDL_assert(map->world != NULL);
    SDL_assert(map->darrs.unitmap != NULL);
    SDL_assert((row < Map_row_len(map)) && (col < Map_col_len(map)));
    SDL_assert(entity);

    /* -- Preliminaries -- */
    // tnecs_entity current = map->darrs.unitmap[row * Map_col_len(map) + col];
    _Map_Unit_Put(map, col, row, entity);
    DARR_PUT(map->units.onfield.arr, entity);

    /* -- Adding MapHPBar -- */
    if (!IES_ENTITY_HASCOMPONENT(map->world, entity, MapHPBar)) {
        TNECS_ADD_COMPONENT(map->world, entity, MapHPBar_ID);
        struct MapHPBar *map_hp_bar = IES_GET_COMPONENT(map->world, entity, MapHPBar);
        *map_hp_bar = MapHPBar_default;
    }
    struct MapHPBar *map_hp_bar = IES_GET_COMPONENT(map->world, entity, MapHPBar);
    map_hp_bar->unit_ent    = entity;
    const Point *tilesize = Map_Tilesize(map);

    map_hp_bar->len         = tilesize->x;
    map_hp_bar->update      = true;
    map_hp_bar->visible     = true;

    /* -- Updating unit pos -- */
    struct Unit     *temp_unit   = IES_GET_COMPONENT(map->world, entity, Unit);
    struct Sprite   *temp_sprite = IES_GET_COMPONENT(map->world, entity, Sprite);

    if (temp_sprite != NULL)
        temp_sprite->visible = true;
    SDL_assert(temp_unit != NULL);

    /* -- Adding unit to army list -- */
    switch (army_alignment[Unit_Army(temp_unit)]) {
        case ALIGNMENT_FRIENDLY: {
            SDL_assert(Unit_ID_Valid(Unit_id(temp_unit)));
            SDL_assert(gl_unit_order != NULL);
            SDL_assert(dtab_get(gl_unit_order, Unit_id(temp_unit)) != NULL);
            u64 order = *(u64 *)dtab_get(gl_unit_order, Unit_id(temp_unit));
            SDL_assert(order > 0);
            SDL_assert(order < UNIT_NUM);
            DARR_PUT(map->units.onfield.friendlies, entity);
            break;
        }
        case ALIGNMENT_ENEMY: {
            SDL_assert(Unit_ID_Valid(Unit_id(temp_unit)));
            SDL_assert(gl_unit_order != NULL);
            SDL_assert(dtab_get(gl_unit_order, Unit_id(temp_unit)) != NULL);
            u64 order = *(u64 *)dtab_get(gl_unit_order, Unit_id(temp_unit));
            SDL_assert(order > 0);
            SDL_assert(order < UNIT_NUM);
            DARR_PUT(map->units.onfield.enemies, entity);
            break;
        }
    }
    SDL_assert(Unit_id(temp_unit) > UNIT_ID_NULL);
    SDL_assert(Unit_id(temp_unit) < UNIT_ID_END);

    Map_addArmy(map, Unit_Army(temp_unit));
}

void Map_Unit_Swap(struct Map *map, u8 old_col, u8 old_row, u8 new_col, u8 new_row) {
    /* New position of unit should always be empty */
    size_t old_i = old_row * Map_col_len(map) + old_col;
    size_t new_i = new_row * Map_col_len(map) + new_col;
    tnecs_entity unit_old = map->darrs.unitmap[old_i];
    tnecs_entity unit_new = map->darrs.unitmap[new_i];

    _Map_Unit_Put(map, new_col, new_row, unit_old);
    _Map_Unit_Put(map, old_col, old_row, unit_new);
}

void Map_Unit_Move(struct Map *map, u8 col, u8 row, u8 new_col, u8 new_row) {
    /* Note: Does NOT check if [new_x, new_y] is empty. */
    SDL_assert(map->darrs.unitmap != NULL);
    SDL_assert(col      < Map_col_len(map));
    SDL_assert(row      < Map_row_len(map));
    SDL_assert(new_col  < Map_col_len(map));
    SDL_assert(new_row  < Map_row_len(map));

    /* New position of unit should always be empty */
    size_t old_i = row      * Map_col_len(map) + col;
    size_t new_i = new_row  * Map_col_len(map) + new_col;

    /* -- Move unit on unitmap -- */
    tnecs_entity entity = map->darrs.unitmap[old_i];
    SDL_assert(map->darrs.unitmap[old_i] > TNECS_NULL);

    _Map_Unit_Put(map, new_col, new_row, map->darrs.unitmap[old_i]);

    SDL_assert(map->darrs.unitmap[new_i] > TNECS_NULL);
    SDL_assert(map->darrs.unitmap[new_i] == entity);
    struct Position *pos = IES_GET_COMPONENT(map->world, entity, Position);
    SDL_assert(pos->tilemap_pos.x == new_col);
    SDL_assert(pos->tilemap_pos.y == new_row);
    map->darrs.unitmap[old_i] = TNECS_NULL;
}

tnecs_entity *Map_Unit_Gets(struct Map *map, u8 army) {
    tnecs_entity *unit_ents = NULL;
    unit_ents = DARR_INIT(unit_ents, tnecs_entity, 16);
    tnecs_entity current_unit_ent;
    struct Unit *current_unit;
    for (u8 i = 0; i < DARR_NUM(map->units.onfield.arr); i++) {
        current_unit_ent = map->units.onfield.arr[i];
        current_unit = IES_GET_COMPONENT(map->world, current_unit_ent, Unit);
        if (Unit_id(current_unit) == army)
            DARR_PUT(unit_ents, current_unit_ent);
    }
    return (unit_ents);
}

tnecs_entity Map_Unit_Get_Boss(struct Map *map, u8 army) {
    tnecs_entity out = TNECS_NULL;

    for (int i = 0; i < DARR_NUM(map->units.onfield.arr); i++) {
        tnecs_entity ent = map->units.onfield.arr[i];
        struct Unit *unit = IES_GET_COMPONENT(map->world, ent, Unit);
        struct Unit *boss = IES_GET_COMPONENT(map->world, ent, Boss);
        if ((Unit_Army(unit) == army) && (boss != NULL)) {
            out = ent;
            break;
        }
    }
    return (out);
}


tnecs_entity Map_Unit_Get(struct Map *map, u8 col, u8 row) {
    SDL_assert(map->darrs.unitmap != NULL);
    SDL_assert(col < Map_col_len(map));
    SDL_assert(row < Map_row_len(map));
    return (map->darrs.unitmap[row * Map_col_len(map) + col]);
}

void Map_Breakable_onBroken(struct Map *map, tnecs_entity breakable) {
}

void Map_Door_onOpen(struct Map *map, tnecs_entity door) {
}

void Map_Chest_onOpen(struct Map *map, tnecs_entity chest) {
}

void Map_addArmy(struct Map *map, u8 army) {
    /* -- Don't add army if found -- */
    for (u8 i = 0; i < DARR_NUM(map->armies.onfield); i++)
        if (map->armies.onfield[i] == army)
            return;

    /* -- Army priority: smaller army go first -- */
    int insert = 0;
    /* If DARR_NUM(map->armies.onfield) is 0, insert is 0 */
    for (int i = 0; i < DARR_NUM(map->armies.onfield); i++) {
        if (army > map->armies.onfield[i]) {
            insert = i;
            break;
        }
    }

    DARR_INSERT(map->armies.onfield, army, insert);
}

int entity_isIn(u64 *array, u64 to_find, size_t arr_len) {
    int found = -1;
    for (size_t i = 0; i < arr_len; i++) {
        if (array[i] == to_find)
            return (i);
    }
    return (found);
}

tnecs_entity _Map_Unit_Remove_List(struct Map *map,  tnecs_entity entity) {
    tnecs_entity out = TNECS_NULL;
    int found = entity_isIn(map->units.onfield.friendlies, entity,
                            DARR_NUM(map->units.onfield.friendlies));
    if (found > -1) {
        // tnecs_entity out = map->units.onfield.friendlies[found];
        DARR_DEL(map->units.onfield.friendlies, found);
    }

    found = entity_isIn(map->units.onfield.enemies, entity, DARR_NUM(map->units.onfield.enemies));
    if (found > -1) {
        // tnecs_entity out = map->units.onfield.enemies[found];
        DARR_DEL(map->units.onfield.enemies, found);
    }

    found = entity_isIn(map->units.onfield.arr, entity, DARR_NUM(map->units.onfield.arr));
    if (found > -1) {
        // tnecs_entity out = map->units.onfield.enemies[found];
        DARR_DEL(map->units.onfield.arr, found);
    }
    return (out);
}

tnecs_entity _Map_Unit_Remove_Map(struct Map *map, u8 col, u8 row) {
    SDL_assert(map->darrs.unitmap != NULL);
    tnecs_entity out = map->darrs.unitmap[row * Map_col_len(map) + col];
    map->darrs.unitmap[row * Map_col_len(map) + col] = TNECS_NULL;
    return (out);
}

void Map_Unit_Remove(struct Map *map, tnecs_entity entity) {
    SDL_assert(entity > TNECS_NULL);
    SDL_assert(map->world   != NULL);
    SDL_assert(map->darrs.unitmap != NULL);

    /* --- Check that entity is really on map --- */
    struct Position *pos = IES_GET_COMPONENT(map->world, entity, Position);
    SDL_assert(pos->onTilemap);

    // SDL_Log("pos->tilemap_pos %d %d", pos->tilemap_pos.x, pos->tilemap_pos.y);
    int index = pos->tilemap_pos.y * Map_col_len(map) + pos->tilemap_pos.x;
    tnecs_entity ontile_ent = map->darrs.unitmap[index];
    SDL_assert(ontile_ent > TNECS_NULL);
    // SDL_Log("%s %d %d", __func__, ontile_ent, map->darrs.unitmap[index]);
    SDL_assert(ontile_ent == entity);
    struct Sprite *sprite = IES_GET_COMPONENT(map->world, entity, Sprite);
    if (sprite != NULL)
        sprite->visible = false;

    if (IES_ENTITY_HASCOMPONENT(map->world, entity, MapHPBar))
        TNECS_REMOVE_COMPONENTS(map->world, entity, MapHPBar_ID);

    /* --- Check that entity is really on map --- */
    map->darrs.unitmap[index] = 0;
    _Map_Unit_Remove_List(map, entity);
}
