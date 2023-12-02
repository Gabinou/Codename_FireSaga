
#include "map/path.h"

void Map_Bounds_Compute(struct Map *map) {
    SDL_Log("%d %d", map->col_len, map->row_len);
    SDL_assert((map->col_len != 0) || (map->row_len != 0));
    map->boundsmin.x = 0;
    map->boundsmax.x = map->col_len - 1;
    map->boundsmin.y = 0;
    map->boundsmax.y = map->row_len - 1;
}

void Map_Global_Danger_Reset(struct Map *map) {
    memset(map->global_dangermap, 0, sizeof(*map->global_dangermap) * map->row_len * map->col_len);
    map->shading_changed = true;
}

void Map_Global_Danger_Add(struct Map *map, i32 *danger) {
    map->global_dangermap = matrix_plus_noM(map->global_dangermap, danger,
                                            map->row_len * map->col_len);
    map->shading_changed = true;
}

void Map_Global_Danger_Sub(struct Map *map, i32 *danger) {
    map->global_dangermap = matrix_sub_noM(map->global_dangermap, danger,
                                           map->row_len * map->col_len);
    map->shading_changed = true;
}

void Map_Danger_Reset(struct Map *map) {
    memset(map->dangermap, 0, sizeof(*map->dangermap) * map->row_len * map->col_len);
    map->shading_changed = true;
}

void Map_Danger_Add(struct Map *map, i32 *danger) {
    map->dangermap = matrix_plus_noM(map->dangermap, danger, map->row_len * map->col_len);
    map->shading_changed = true;
}

void Map_Danger_Sub(struct Map *map, i32 *danger) {
    map->dangermap = matrix_sub_noM(map->dangermap, danger, map->row_len * map->col_len);
    map->shading_changed = true;
}

void Map_Stacked_Dangermap_Compute(struct Map *map) {
    /* assumes movemap, attacktomap, dangermap are computed */

    int size = map->row_len * map->col_len;
    i32 *temp_map = matrix_ssmaller(map->dangermap, DANGERMAP_UNIT_DIVISOR, size);
    map->stacked_dangermap = matrix_and_noM(map->stacked_dangermap, map->dangermap, temp_map,
                                            map->row_len * map->col_len);
    map->stacked_dangermap = matrix_and_noM(map->stacked_dangermap, map->stacked_dangermap,
                                            map->movemap, map->row_len * map->col_len);

    SDL_free(temp_map);
    map->shading_changed = true;
}

void Map_Stacked_Dangermap_Reset(struct Map *map) {
    int len = map->row_len * map->col_len;
    if (map->stacked_dangermap != NULL) {
        SDL_Log("Map_Stackmap_Reset SETdanger");
        int size = sizeof(*map->stacked_dangermap);
        map->stacked_dangermap = memset(map->stacked_dangermap, 0, size * len);
    }

    map->shading_changed = true;
}


void Map_Stacked_Global_Dangermap_Reset(struct Map *map) {
    int len = map->row_len * map->col_len;

    if (map->stacked_global_dangermap != NULL) {
        int size = sizeof(*map->stacked_global_dangermap);
        map->stacked_global_dangermap = memset(map->stacked_global_dangermap, 0, size * len);
    }

    map->shading_changed = true;
}

float *_Map_fMovemap_Compute(struct Map *map, struct Pointf start, float move) {
    // map->fmovemap = pathfinding_Map_Moveto_noM_float(map->fmovemap, map->fcostmap,
    // map->row_len, map->col_len, start, move);
    return (map->fmovemap);
}

float *Map_fMovemap_Compute(struct Map *map, tnecs_world *world, tnecs_entity unit_ent) {
    Map_fCostmap_Movement_Compute(map, world, unit_ent);
    struct Unit *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    struct Position *pos = TNECS_GET_COMPONENT(world, unit_ent, Position);
    i32 move = Unit_getStats(unit).move;
    struct Pointf start;
    start.x = (float)pos->tilemap_pos.x;
    start.y = (float)pos->tilemap_pos.y;
    return (_Map_fMovemap_Compute(map, start, move));
}

i32 *_Map_Movemap_Compute(struct Map *map, struct Point start_in, i32 move) {
    struct Point start = {start_in.x, start_in.y};
    Pathfinding_Moveto_noM(map->movemap, map->costmap,
                           map->row_len, map->col_len, start, move);
    // matrix_print(map->movemap, map->row_len, map->col_len);

    return (map->movemap);
}

i32 *Map_Movemap_Compute(struct Map *map, tnecs_world *world, tnecs_entity unit_ent) {
    Map_Costmap_Movement_Compute(map, world, unit_ent);
    struct Unit     *unit   = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    struct Position *pos    = TNECS_GET_COMPONENT(world, unit_ent, Position);
    i32              move   = Unit_getStats(unit).move;
    struct Point     start  = pos->tilemap_pos;
    return (_Map_Movemap_Compute(map, start, move));
}

// TODO: get rid of this useless interface
i32 *_Map_tomap_Compute(i32 *tomap, i32 *movemap, u8 row_len, u8 col_len,
                        i32 move, struct Range *range, u8 mode_movetile) {
    Pathfinding_Attackto_noM(tomap, movemap, row_len, col_len, (u8 *)range,
                             mode_movetile);
    return (tomap);
}

i32 *Map_Healtolist_Compute(struct Map   *map) {
    SDL_assert(map->healtomap  != NULL);
    SDL_assert(map->healtolist != NULL);
    map->healtolist = matrix2list_noM(map->healtomap, map->healtolist,
                                      map->row_len, map->col_len);
    return (map->healtolist);
}

i32 *Map_Healtomap_Compute_wLoadout(struct Map *map, tnecs_world *world, tnecs_entity ent,
                                    bool move, int lh, int rh) {
    struct Unit     *unit = TNECS_GET_COMPONENT(world, ent, Unit);
    Unit_Loadout_Swap(unit, lh, rh);
    i32 *out = Map_Healtomap_Compute(map, world, ent, move, true);
    Unit_Loadout_Swap_Reverse(unit, lh, rh);

    return (out);
}

i32 *Map_Healtomap_Compute(struct Map *map, tnecs_world *world, tnecs_entity unit_ent,
                           bool move, bool equipped) {
    Map_Costmap_Movement_Compute(map, world, unit_ent);
    struct Unit     *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    struct Position *pos  = TNECS_GET_COMPONENT(world, unit_ent, Position);
    struct Point start = pos->tilemap_pos;
    i32 move_stat = move ? Unit_getStats(unit).move : 0;
    _Map_Movemap_Compute(map, start, move_stat);
    struct Range range = {.min = UINT8_MAX, .max = 0 };
    _Unit_Range_Combine(unit, &range, equipped, ITEM_ARCHETYPE_STAFF);
    // SDL_Log("range %d %d", range.min, range.max);

    map->update = true;
    map->healtomap = _Map_tomap_Compute(map->healtomap, map->movemap, map->row_len, map->col_len,
                                        move_stat, &range, MOVETILE_INCLUDE);
    // matrix_print(map->healtomap, map->row_len, map->col_len);
    return (map->healtomap);
}

i32 *Map_Attacktolist_Compute(struct Map *map) {
    map->attacktolist = matrix2list_noM(map->attacktomap, map->attacktolist,
                                        map->row_len, map->col_len);
    return (map->attacktolist);
}

i32 *Map_Attackfromlist_Compute(struct Map *map) {
    map->attackfromlist = matrix2list_noM(map->attackfrommap, map->attackfromlist,
                                          map->row_len, map->col_len);
    return (map->attackfromlist);
}

i32 *Map_Attacktomap_Compute_wLoadout(struct Map *map, tnecs_world *world, tnecs_entity ent,
                                      bool move, int lh, int rh) {
    struct Unit     *unit = TNECS_GET_COMPONENT(world, ent, Unit);
    Unit_Loadout_Swap(unit, lh, rh);
    i32 *out = Map_Attacktomap_Compute(map, world, ent, move, true);
    Unit_Loadout_Swap_Reverse(unit, lh, rh);

    return (out);
}

i32 *Map_Attacktomap_Compute(struct Map *map, tnecs_world *world,
                             tnecs_entity unit_ent, bool move, bool equipped) {
    Map_Costmap_Movement_Compute(map, world, unit_ent);
    struct Unit     *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    struct Position *pos  = TNECS_GET_COMPONENT(world, unit_ent, Position);
    struct Point start = pos->tilemap_pos;
    i32 move_stat = move ? Unit_getStats(unit).move : 0;
    _Map_Movemap_Compute(map, start, move_stat);
    struct Range range = {.min = UINT8_MAX, .max = 0 };
    _Unit_Range_Combine(unit, &range, equipped, ITEM_ARCHETYPE_WEAPON);
    map->update = true;
    map->attacktomap = _Map_tomap_Compute(map->attacktomap, map->movemap, map->row_len,
                                          map->col_len, move_stat, &range, MOVETILE_INCLUDE);
    // matrix_print(map->attacktomap, map->row_len, map->col_len);
    return (map->attacktomap);
}

i32 *Map_Attackfrommap_Compute(struct Map *map, tnecs_world *world, tnecs_entity agg,
                               tnecs_entity dft, bool move, bool equipped) {
    Map_Costmap_Movement_Compute(map, world, agg);
    // SDL_Log("COSTMAP");
    // matrix_print(map->costmap, map->row_len, map->col_len);

    struct Unit *agg_unit = TNECS_GET_COMPONENT(world, agg, Unit);
    /* Get dft position */
    struct Position *agg_pos = TNECS_GET_COMPONENT(world, agg, Position);
    struct Position *dft_pos = TNECS_GET_COMPONENT(world, dft, Position);
    /* Get agg range */
    struct Range range = {.min = UINT8_MAX, .max = 0};
    _Unit_Range_Combine(agg_unit, &range, equipped, ITEM_ARCHETYPE_WEAPON);

    /* Compute movemap */
    i32 move_stat = move ? Unit_getStats(agg_unit).move : 0;
    _Map_Movemap_Compute(map, agg_pos->tilemap_pos, move_stat);
    // SDL_Log("MOVEMAP");
    // matrix_print(map->movemap, map->row_len, map->col_len);

    Pathfinding_Attackfrom_noM(map->attackfrommap, map->movemap, map->row_len,
                               map->col_len, dft_pos->tilemap_pos, (u8 *)&range);

    return (map->attackfrommap);
}


i32 *Map_Danger_Compute(struct Map *map, tnecs_world *world, tnecs_entity unit_ent) {
    Map_Costmap_Movement_Compute(map, world, unit_ent);
    struct Position *position   = TNECS_GET_COMPONENT(world, unit_ent, Position);
    struct Unit *unit           = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    SDL_assert(position != NULL);
    SDL_assert(unit != NULL);
    i32 move = Unit_getStats(unit).move;
    struct Point start = position->tilemap_pos;
    _Map_Movemap_Compute(map, start, move);
    struct Range *range = Unit_Range_Combine_Equipment(unit);

    map->attacktomap = _Map_tomap_Compute(map->attacktomap, map->movemap, map->row_len,
                                          map->col_len, move, range, MOVETILE_INCLUDE);
    memset(map->temp, 0, sizeof(*map->temp)*map->row_len * map->col_len);
    map->temp = matrix_plus_noM(map->temp, map->attacktomap, map->row_len * map->col_len);
    // matrix_print(map->temp, map->row_len, map->col_len);
    return (map->temp);
}

i32 *Map_Costmap_PushPull_Compute(struct Map *map, tnecs_world *world,
                                  tnecs_entity unit_ent) {
    SDL_assert(unit_ent != 0);
    SDL_assert(map->costmap != NULL);
    memset(map->costmap, 0, sizeof(*map->costmap) * map->col_len * map->row_len);
    struct Unit *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    struct Tile *temp_tile;
    i32 tile_ind = 0;
    i8 unit_movetype = unit->mvt_type;
    u8 army = unit->army;
    u8 ontile_army;
    tnecs_entity ontile_unit_ent;
    SDL_assert(unit_movetype > UNIT_MVT_START);
    for (u8 i = 0; i < map->row_len * map->col_len; i++) {
        tile_ind = map->tilemap[i] / TILE_DIVISOR;
        ontile_unit_ent = map->unitmap[i];
        SDL_assert(tile_ind > 0);
        size_t tile_order = Map_Tile_Order(map, tile_ind);
        temp_tile = map->tiles + tile_order;
        map->costmap[i] = temp_tile->cost_array[unit_movetype];
        if (ontile_unit_ent <= TNECS_NULL)
            continue;
        struct Unit *ontile_unit = TNECS_GET_COMPONENT(world, ontile_unit_ent, Unit);
        if (ontile_unit != NULL)
            map->costmap[i] = COSTMAP_BLOCKED;
    }
    return (map->costmap);
}

float *Map_fCostmap_Movement_Compute(struct Map *map, tnecs_world *world,
                                     tnecs_entity unit_ent) {
    SDL_assert(map->unitmap != NULL);
    SDL_assert(map->costmap != NULL);
    SDL_assert(((unit_ent > 0) && (unit_ent < UNIT_ID_NPC_END)));

    /* Preliminaries*/
    memset(map->fcostmap, 0, sizeof(*map->fcostmap) * map->col_len * map->row_len);
    struct Unit *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    struct Tile *temp_tile;
    i32 tile_ind = 0;
    i8 unit_movetype = unit->mvt_type;
    u8 army = unit->army;

    /* Compute cost of each tile*/
    SDL_assert(unit_movetype > UNIT_MVT_START);
    for (size_t i = 0; i < (map->col_len * map->row_len); i++) {
        /* - Compute cost from tile - */
        tile_ind = map->tilemap[i] / TILE_DIVISOR;
        SDL_assert(tile_ind > 0);
        size_t tile_order = Map_Tile_Order(map, tile_ind);
        temp_tile = map->tiles + tile_order;
        map->fcostmap[i] = temp_tile->cost_array[unit_movetype];

        /* - Check if tile is blocked from opposing army - */
        tnecs_entity ontile_unit_ent = map->unitmap[i];
        if ((ontile_unit_ent <= UNIT_ID_START) || (ontile_unit_ent >= UNIT_ID_NPC_END))
            continue;

        struct Unit *ontile_unit = TNECS_GET_COMPONENT(world, ontile_unit_ent, Unit);
        SDL_assert(ontile_unit != NULL);
        u8 ontile_army = ontile_unit->army;
        SDL_assert((ontile_army < ARMY_END) && (ontile_army > ARMY_START));

        if (SotA_army2alignment(ontile_army) != SotA_army2alignment(army))
            map->fcostmap[i] = costmap_fBLOCKED;
    }
    return (map->fcostmap);
}

i32 *Map_Costmap_Movement_Compute(struct Map *map, tnecs_world *world,
                                  tnecs_entity unit_ent) {
    SDL_assert(map->unitmap     != NULL);
    SDL_assert(map->costmap != NULL);

    /* - Skip if previously computed - */
    if (map->costmap_ent == unit_ent)
        return (map->costmap);

    /* - Preliminaries - */
    map->costmap_ent = unit_ent;
    memset(map->costmap, 0, sizeof(*map->costmap) * map->col_len * map->row_len);
    struct Unit *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    struct Tile *temp_tile;
    i32 tile_ind = 0;
    i8 unit_movetype = unit->mvt_type;
    u8 army = unit->army;

    /* - Compute cost of each tile - */
    SDL_assert(unit_movetype > UNIT_MVT_START);
    for (size_t i = 0; i < (map->col_len * map->row_len); i++) {
        #ifdef UNITS_IGNORE_TERRAIN
        /* - Everything flies - */
        map->costmap[i] = COSTMAP_MIN;

        #else /* not UNITS_IGNORE_TERRAIN */

        /* - Compute cost from tile - */
        tile_ind = map->tilemap[i] / TILE_DIVISOR;
        SDL_assert(tile_ind > 0);
        size_t tile_order = Map_Tile_Order(map, tile_ind);
        temp_tile = map->tiles + tile_order;
        map->costmap[i] = temp_tile->cost_array[unit_movetype];

        #endif /* UNITS_IGNORE_TERRAIN */

        /* - Check if tile is occupied by enemy unit - */
        tnecs_entity ontile_unit_ent = map->unitmap[i];
        if ((ontile_unit_ent <= UNIT_ID_START) || (ontile_unit_ent >= UNIT_ID_NPC_END))
            continue;

        struct Unit *ontile_unit = TNECS_GET_COMPONENT(world, ontile_unit_ent, Unit);
        SDL_assert(ontile_unit != NULL);
        u8 ontile_army = ontile_unit->army;
        SDL_assert((ontile_army < ARMY_END) && (ontile_army > ARMY_START));

        if (SotA_army2alignment(ontile_army) != SotA_army2alignment(army))
            map->costmap[i] = COSTMAP_BLOCKED;

    }
    return (map->costmap);
}


void Map_globalRange(struct Map *map, tnecs_world *world, u8 alignment) {
    tnecs_entity *unit_entities = NULL;
    u8 num_unit_entities = 0;
    SDL_assert(map->global_rangemap != NULL);
    memset(map->global_rangemap, 0, sizeof(*map->global_rangemap) * map->row_len * map->col_len);

    /* Get enemies depending on alignment */
    switch (alignment) {
        case ALIGNMENT_FRIENDLY:
            unit_entities       = map->friendlies_onfield;
            num_unit_entities   = DARR_NUM(map->friendlies_onfield);
            break;
        case ALIGNMENT_ENEMY:
            unit_entities       = map->enemies_onfield;
            num_unit_entities   = DARR_NUM(map->friendlies_onfield);
            break;
    }
    /* TODO: Only recompute if enemy died, turn changed */

    /* Add all enemies attackmap into global_rangemap */
    for (int i = 0; i < num_unit_entities; i++) {
        struct Unit     *temp_unit  = TNECS_GET_COMPONENT(world, unit_entities[i], Unit);
        struct Position *temp_pos   = TNECS_GET_COMPONENT(world, unit_entities[i], Position);
        struct Range    *range      = Unit_Range_Combine_Equipment(temp_unit);
        struct Unit_stats temp_effective_stats = Unit_effectiveStats(temp_unit);
        u8 move = temp_effective_stats.move;
        struct Point start = {temp_pos->tilemap_pos.x, temp_pos->tilemap_pos.y};
        Map_Costmap_Movement_Compute(map, world, unit_entities[i]);
        Pathfinding_Moveto_noM(map->movemap, map->costmap, map->row_len,
                               map->col_len, start, move);
        Pathfinding_Attackto_noM(map->attacktomap, map->movemap, map->row_len,
                                 map->col_len, (u8 *)range, MOVETILE_INCLUDE);
        map->global_rangemap = matrix_plus_noM(map->global_rangemap, map->attacktomap,
                                               map->row_len * map->col_len);
    }

}
