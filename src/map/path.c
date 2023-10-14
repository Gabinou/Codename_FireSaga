
#include "map/path.h"

void Map_Bounds_Compute(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("%d %d", map->col_len, map->row_len);
    SDL_assert((map->col_len != 0) || (map->row_len != 0));
    map->boundsmin.x = 0;
    map->boundsmax.x = map->col_len - 1;
    map->boundsmin.y = 0;
    map->boundsmax.y = map->row_len - 1;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Global_Danger_Reset(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    memset(map->global_dangermap, 0, sizeof(*map->global_dangermap) * map->row_len * map->col_len);
    map->shading_changed = true;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Global_Danger_Add(struct Map *map, i32 *danger) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    map->global_dangermap = linalg_plus_noM_int32_t(map->global_dangermap, danger,
                                                    map->row_len * map->col_len);
    map->shading_changed = true;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Global_Danger_Sub(struct Map *map, i32 *danger) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    map->global_dangermap = linalg_sub_noM_int32_t(map->global_dangermap, danger,
                                                   map->row_len * map->col_len);
    map->shading_changed = true;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Danger_Reset(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    memset(map->dangermap, 0, sizeof(*map->dangermap) * map->row_len * map->col_len);
    map->shading_changed = true;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Danger_Add(struct Map *map, i32 *danger) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    map->dangermap = linalg_plus_noM_int32_t(map->dangermap, danger, map->row_len * map->col_len);
    map->shading_changed = true;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Danger_Sub(struct Map *map, i32 *danger) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    map->dangermap = linalg_sub_noM_int32_t(map->dangermap, danger, map->row_len * map->col_len);
    map->shading_changed = true;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Stacked_Dangermap_Compute(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* assumes movemap, attacktomap, dangermap are computed */

    int size = map->row_len * map->col_len;
    i32 *temp_map = linalg_ssmaller_int32_t(map->dangermap, DANGERMAP_UNIT_DIVISOR, size);
    map->stacked_dangermap = linalg_and_noM_int32_t(map->stacked_dangermap, map->dangermap, temp_map,
                                                    map->row_len * map->col_len);
    map->stacked_dangermap = linalg_and_noM_int32_t(map->stacked_dangermap, map->stacked_dangermap,
                                                    map->movemap, map->row_len * map->col_len);

    free(temp_map);
    map->shading_changed = true;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Stacked_Dangermap_Reset(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    int len = map->row_len * map->col_len;
    if (map->stacked_dangermap != NULL) {
        SOTA_Log_Debug("Map_Stackmap_Reset SETdanger");
        int size = sizeof(*map->stacked_dangermap);
        map->stacked_dangermap = memset(map->stacked_dangermap, 0, size * len);
    }

    map->shading_changed = true;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}


void Map_Stacked_Global_Dangermap_Reset(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    int len = map->row_len * map->col_len;

    if (map->stacked_global_dangermap != NULL) {
        SOTA_Log_Debug("Map_Stackmap_Reset SETglobal_danger");
        int size = sizeof(*map->stacked_global_dangermap);
        map->stacked_global_dangermap = memset(map->stacked_global_dangermap, 0, size * len);
    }

    map->shading_changed = true;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

float *_Map_fMovemap_Compute(struct Map *map, struct nmath_point_float start, float move) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    map->fmovemap = pathfinding_Map_Moveto_noM_float(map->fmovemap, map->fcostmap,
                                                     map->row_len, map->col_len, start, move);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (map->fmovemap);
}

float *Map_fMovemap_Compute(struct Map *map, tnecs_world_t *world, tnecs_entity_t unit_ent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Map_fCostmap_Movement_Compute(map, world, unit_ent);
    struct Unit *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    struct Position *pos = TNECS_GET_COMPONENT(world, unit_ent, Position);
    i32 move = Unit_getStats(unit).move;
    struct nmath_point_float start;
    start.x = (float)pos->tilemap_pos.x;
    start.y = (float)pos->tilemap_pos.y;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (_Map_fMovemap_Compute(map, start, move));
}

i32 *_Map_Movemap_Compute(struct Map *map, struct Point start_in, i32 move) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Point start = {start_in.x, start_in.y};
    Map_Pathfinding_Moveto_noM(map->movemap, map->costmap,
                               map->row_len, map->col_len, start, move);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (map->movemap);
}

i32 *Map_Movemap_Compute(struct Map *map, tnecs_world_t *world, tnecs_entity_t unit_ent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Map_Costmap_Movement_Compute(map, world, unit_ent);
    struct Unit     *unit   = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    struct Position *pos    = TNECS_GET_COMPONENT(world, unit_ent, Position);
    struct Unit_stats temp_effective_stats = Unit_effectiveStats(unit);
    i32 move = Unit_getStats(unit).move;
    struct Point start = pos->tilemap_pos;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (_Map_Movemap_Compute(map, start, move));
}

i32 *Taxicab_Circle(i32 *matrix, i32 x, i32 y, size_t row_len, size_t col_len,
                    struct Range *range) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    i32 subrangey_min, subrangey_max;
    for (i32 rangex = 0; rangex <= range->max; rangex++) {
        subrangey_min = (rangex > range->min) ? 0 : (range->min - rangex);
        subrangey_max = (rangex > range->max) ? 0 : (range->max - rangex);
        for (i32 rangey = subrangey_min; rangey <= subrangey_max; rangey++) {
            for (int8_t sq_neighbor = 0; sq_neighbor < NMATH_SQUARE_NEIGHBOURS; sq_neighbor++) {
                i32 tempx = nmath_inbounds_int32_t(x + q_cycle4_pmmp(sq_neighbor) * rangex, 0, col_len - 1);
                i32 tempy = nmath_inbounds_int32_t(y + q_cycle4_ppmm(sq_neighbor) * rangey, 0, row_len - 1);
                matrix[tempx * col_len + tempy] = 1;
            }
        }
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (matrix);
}

i32 *Taxicab_Circle_List(i32 *darr_list, i32 *matrix, i32 x, i32 y,
                         size_t row_len, size_t col_len, struct Range *range) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // Taxicabs can't move diagonal so manhattan distance: abs(x1-x2) + abs(y1-ys2)
    // Manhattan (distance) used to trace 'circles' on square tilemap
    // Returns: List points at distance [range_min, range_max] dist from [x, y]
    matrix = Taxicab_Circle(matrix, x, y, row_len, col_len, range);
    darr_list = linalg_matrix2list_noM_int32_t(matrix, darr_list, row_len, col_len);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (darr_list);
}

i32 *_Map_tomap_Compute(i32 *tomap, i32 *movemap, uf8 row_len, uf8 col_len,
                        i32 move, struct Range *range, uf8 mode_movetile) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tomap = pathfinding_Map_Attackto_noM_int32_t(tomap, movemap, row_len, col_len, move, (uf8 *)range,
                                                 mode_movetile);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (tomap);
}

i32 *Map_Healtolist_Compute(struct Map   *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(map->healtomap  != NULL);
    SDL_assert(map->healtolist != NULL);
    map->healtolist = linalg_matrix2list_noM_int32_t(map->healtomap, map->healtolist,
                                                     map->row_len, map->col_len);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (map->healtolist);
}

i32 *Map_Healtomap_Compute_wLoadout(struct Map *map, tnecs_world_t *world, tnecs_entity_t ent,
                                    bool move, int lh, int rh) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit     *unit = TNECS_GET_COMPONENT(world, ent, Unit);
    Unit_Loadout_Swap(unit, lh, rh);
    i32 *out = Map_Healtomap_Compute(map, world, ent, move, true);
    Unit_Loadout_Swap_Reverse(unit, lh, rh);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

i32 *Map_Healtomap_Compute(struct Map *map, tnecs_world_t *world, tnecs_entity_t unit_ent,
                           bool move, bool equipped) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Map_Costmap_Movement_Compute(map, world, unit_ent);
    struct Unit     *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    struct Position *pos  = TNECS_GET_COMPONENT(world, unit_ent, Position);
    struct Point start = pos->tilemap_pos;
    i32 move_stat = move ? Unit_getStats(unit).move : 0;
    _Map_Movemap_Compute(map, start, move_stat);
    struct Range range = {.min = UINT8_MAX, .max = 0 };
    _Unit_Range_Combine(unit, &range, equipped, ITEM_ARCHETYPE_STAFF);
    // SOTA_Log_Debug("range %d %d", range.min, range.max);

    map->update = true;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    map->healtomap = _Map_tomap_Compute(map->healtomap, map->movemap, map->row_len, map->col_len,
                                        move_stat, &range, NMATH_MOVETILE_INCLUDE);
    // linalg_matrix_print_int(map->healtomap, map->row_len, map->col_len);
    return (map->healtomap);
}

i32 *Map_Attacktolist_Compute(struct Map   *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    map->attacktolist = linalg_matrix2list_noM_int32_t(map->attacktomap, map->attacktolist,
                                                       map->row_len, map->col_len);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (map->attacktolist);
}

i32 *Map_Attacktomap_Compute_wLoadout(struct Map *map, tnecs_world_t *world, tnecs_entity_t ent,
                                      bool move, int lh, int rh) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit     *unit = TNECS_GET_COMPONENT(world, ent, Unit);
    Unit_Loadout_Swap(unit, lh, rh);
    i32 *out = Map_Attacktomap_Compute(map, world, ent, move, true);
    Unit_Loadout_Swap_Reverse(unit, lh, rh);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

i32 *Map_Attacktomap_Compute(struct Map *map, tnecs_world_t *world, tnecs_entity_t unit_ent,
                             bool move, bool equipped) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Map_Costmap_Movement_Compute(map, world, unit_ent);
    struct Unit     *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    struct Position *pos  = TNECS_GET_COMPONENT(world, unit_ent, Position);
    struct Point start = pos->tilemap_pos;
    i32 move_stat = move ? Unit_getStats(unit).move : 0;
    _Map_Movemap_Compute(map, start, move_stat);
    struct Range range = {.min = UINT8_MAX, .max = 0 };
    _Unit_Range_Combine(unit, &range, equipped, ITEM_ARCHETYPE_WEAPON);
    map->update = true;
    map->attacktomap = _Map_tomap_Compute(map->attacktomap, map->movemap, map->row_len, map->col_len,
                                          move_stat, &range, NMATH_MOVETILE_INCLUDE);
    // linalg_matrix_print_int(map->attacktomap, map->row_len, map->col_len);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (map->attacktomap);
}

i32 *Map_Danger_Compute(struct Map *map, tnecs_world_t *world, tnecs_entity_t unit_ent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Map_Costmap_Movement_Compute(map, world, unit_ent);
    struct Position *position = TNECS_GET_COMPONENT(world, unit_ent, Position);
    struct Unit *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    SDL_assert(position != NULL);
    SDL_assert(unit != NULL);
    i32 move = Unit_getStats(unit).move;
    struct Point start = position->tilemap_pos;
    _Map_Movemap_Compute(map, start, move);
    struct Range *range = Unit_Range_Combine_Equipment(unit);

    map->attacktomap = _Map_tomap_Compute(map->attacktomap, map->movemap, map->row_len,
                                          map->col_len, move, range, NMATH_MOVETILE_INCLUDE);
    memset(map->temp, 0, sizeof(*map->temp)*map->row_len * map->col_len);
    map->temp = linalg_plus_noM_int32_t(map->temp, map->attacktomap, map->row_len * map->col_len);
    // linalg_matrix_print_int(map->temp, map->row_len, map->col_len);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (map->temp);
}

i32 *Map_Costmap_PushPull_Compute(struct Map *map, tnecs_world_t *world,
                                  tnecs_entity_t unit_ent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit_ent != 0);
    SDL_assert(map->costmap != NULL);
    memset(map->costmap, 0, sizeof(*map->costmap) * map->col_len * map->row_len);
    struct Unit *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    struct Tile *temp_tile;
    i32 tile_ind = 0;
    if8 unit_movetype = unit->mvt_type;
    uf8 army = unit->army;
    uf8 ontile_army;
    tnecs_entity_t ontile_unit_ent;
    SDL_assert(unit_movetype > UNIT_MVT_START);
    for (uf8 i = 0; i < map->row_len * map->col_len; i++) {
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
            map->costmap[i] = NMATH_COSTMAP_BLOCKED;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (map->costmap);
}

float *Map_fCostmap_Movement_Compute(struct Map *map, tnecs_world_t *world,
                                     tnecs_entity_t unit_ent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(map->unitmap != NULL);
    SDL_assert(map->costmap != NULL);
    SDL_assert(((unit_ent > 0) && (unit_ent < UNIT_ID_NPC_END)));

    /* Preliminaries*/
    memset(map->fcostmap, 0, sizeof(*map->fcostmap) * map->col_len * map->row_len);
    struct Unit *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    struct Tile *temp_tile;
    i32 tile_ind = 0;
    if8 unit_movetype = unit->mvt_type;
    uf8 army = unit->army;

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
        tnecs_entity_t ontile_unit_ent = map->unitmap[i];
        if ((ontile_unit_ent <= UNIT_ID_START) || (ontile_unit_ent >= UNIT_ID_NPC_END))
            continue;

        struct Unit *ontile_unit = TNECS_GET_COMPONENT(world, ontile_unit_ent, Unit);
        SDL_assert(ontile_unit != NULL);
        uf8 ontile_army = ontile_unit->army;
        SDL_assert((ontile_army < ARMY_END) && (ontile_army > ARMY_START));

        if (SotA_army2alignment(ontile_army) != SotA_army2alignment(army))
            map->fcostmap[i] = NMATH_COSTMAP_fBLOCKED;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (map->fcostmap);
}

i32 *Map_Costmap_Movement_Compute(struct Map *map, tnecs_world_t *world,
                                  tnecs_entity_t unit_ent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(map->unitmap != NULL);
    SDL_assert(map->costmap != NULL);

    /* - Skip if previously computed - */
    if (map->costmap_ent == unit_ent) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (map->costmap);
    }

    /* - Preliminaries - */
    map->costmap_ent = unit_ent;
    memset(map->costmap, 0, sizeof(*map->costmap) * map->col_len * map->row_len);
    struct Unit *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    struct Tile *temp_tile;
    i32 tile_ind = 0;
    if8 unit_movetype = unit->mvt_type;
    uf8 army = unit->army;

    /* - Compute cost of each tile - */
    SDL_assert(unit_movetype > UNIT_MVT_START);
    for (size_t i = 0; i < (map->col_len * map->row_len); i++) {
        #ifndef UNITS_IGNORE_TERRAIN
        /* - Compute cost from tile - */
        tile_ind = map->tilemap[i] / TILE_DIVISOR;
        SDL_assert(tile_ind > 0);
        size_t tile_order = Map_Tile_Order(map, tile_ind);
        temp_tile = map->tiles + tile_order;
        map->costmap[i] = temp_tile->cost_array[unit_movetype];

        /* - Check if tile is blocked from opposing army - */
        tnecs_entity_t ontile_unit_ent = map->unitmap[i];
        if ((ontile_unit_ent <= UNIT_ID_START) || (ontile_unit_ent >= UNIT_ID_NPC_END))
            continue;

        struct Unit *ontile_unit = TNECS_GET_COMPONENT(world, ontile_unit_ent, Unit);
        SDL_assert(ontile_unit != NULL);
        uf8 ontile_army = ontile_unit->army;
        SDL_assert((ontile_army < ARMY_END) && (ontile_army > ARMY_START));

        if (SotA_army2alignment(ontile_army) != SotA_army2alignment(army))
            map->costmap[i] = NMATH_COSTMAP_BLOCKED;

        #else /* UNITS_DONOTIGNORE_TERRAIN */
        map->costmap[i] = NMATH_COSTMAP_MOVEABLEMIN;
        #endif /* UNITS_IGNORE_TERRAIN */
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (map->costmap);
}


void Map_globalRange(struct Map *map, tnecs_world_t *world, uf8 alignment) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tnecs_entity_t *unit_entities = NULL;
    uf8 num_unit_entities = 0;
    SDL_assert(map->global_rangemap != NULL);
    memset(map->global_rangemap, 0, sizeof(*map->global_rangemap) * map->row_len * map->col_len);

    /* Get enemies depending on alignment */
    switch (alignment) {
        case ALIGNMENT_FRIENDLY:
            unit_entities       = map->friendlies_onfield;
            num_unit_entities   = map->num_friendlies_onfield;
            break;
        case ALIGNMENT_ENEMY:
            unit_entities       = map->enemies_onfield;
            num_unit_entities   = map->num_enemies_onfield;
            break;
    }
    /* TODO: Only recompute if enemy died, turn changed */

    /* Add all enemies attackmap into global_rangemap */
    for (int i = 0; i < num_unit_entities; i++) {
        struct Unit     *temp_unit  = TNECS_GET_COMPONENT(world, unit_entities[i], Unit);
        struct Position *temp_pos   = TNECS_GET_COMPONENT(world, unit_entities[i], Position);
        struct Range    *range      = Unit_Range_Combine_Equipment(temp_unit);
        struct Unit_stats temp_effective_stats = Unit_effectiveStats(temp_unit);
        uf8 move = temp_effective_stats.move;
        struct Point start = {temp_pos->tilemap_pos.x, temp_pos->tilemap_pos.y};
        Map_Costmap_Movement_Compute(map, world, unit_entities[i]);
        Map_Pathfinding_Moveto_noM(map->movemap, map->costmap, map->row_len,
                                   map->col_len, start, move);
        map->attacktomap = pathfinding_Map_Attackto_noM_int32_t(map->attacktomap, map->movemap,
                                                                map->row_len, map->col_len, move, (uf8 *)range, NMATH_MOVETILE_INCLUDE);
        map->global_rangemap = linalg_plus_noM_int32_t(map->global_rangemap, map->attacktomap,
                                                       map->row_len * map->col_len);
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Pushing and pulling --- */

/* TODO: Convert from nmath to SOTA */
// int * came_from2path_map(int * path_map, int * came_from, size_t row_len, size_t col_len, int x_start, int y_start, int x_end, int y_end) {
//     struct Point current = {x_end, y_end};
//     for (size_t i = 0; i < NMATH_ITERATIONS_LIMIT; i++) {
//         path_map[current.y * col_len + current.x] = 1;
//         if ((current.x == x_start) && (current.y == y_start)) {
//             break;
//         }
//         switch (came_from[current.y * col_len + current.x]) {
//             case NMATH_DIRECTION_UP:
//                 current.y -= 1;
//                 break;
//             case NMATH_DIRECTION_DOWN:
//                 current.y += 1;
//                 break;
//             case NMATH_DIRECTION_LEFT:
//                 current.x += 1;
//                 break;
//             case NMATH_DIRECTION_RIGHT:
//                 current.x -= 1;
//                 break;
//         }
//     }
//     path_map[y_end * col_len + x_end] = 1;
//     return (path_map);
// }

// int * came_from2path_list(int * path_list, int * came_from, size_t row_len, size_t col_len, int x_start, int y_start, int x_end, int y_end) {
//     struct Point current = {x_end, y_end};
//     DARR_NUM(path_list) = 0;
//     for (size_t i = 0; i < NMATH_ITERATIONS_LIMIT; i++) {
//         DARR_PUT(path_list, current.x);
//         DARR_PUT(path_list, current.y);
//         if ((current.x == x_start) && (current.y == y_start)) {
//             break;
//         }
//         switch (came_from[current.y * col_len + current.x]) {
//             case NMATH_DIRECTION_UP:
//                 current.y -= 1;
//                 break;
//             case NMATH_DIRECTION_DOWN:
//                 current.y += 1;
//                 break;
//             case NMATH_DIRECTION_LEFT:
//                 current.x += 1;
//                 break;
//             case NMATH_DIRECTION_RIGHT:
//                 current.x -= 1;
//                 break;
//         }
//     }
//     return (path_list);
// }

// struct nmath_sq_neighbors  pathfinding_Direction_Pushto(i32  * attackfrommap, size_t row_len, size_t col_len, u8 range[2], struct Point target) {
//     struct nmath_sq_neighbors  Pushto = {0, 0, 0, 0};
//     struct Point neighbor;
//     for (i32 distance = range[0]; distance <= range[1]; distance++) {
//         for (i32  i = 0; i < NMATH_SQUARE_NEIGHBOURS; i++) {
//             neighbor.x = nmath_inbounds((target.x + distance * q_cycle4_pzmz(i)), 0, col_len - 1);
//             neighbor.y = nmath_inbounds((target.y + distance * q_cycle4_zpzm(i)), 0, row_len - 1);
//             if (attackfrommap[neighbor.y * col_len + neighbor.x] >= NMATH_ATTACKFROM_MOVEABLEMIN) {
//                 if (neighbor.x > target.x) {
//                     Pushto.left = true;
//                 } else if (neighbor.y > target.y) {
//                     Pushto.top = true;
//                 } else if (neighbor.x < target.x) {
//                     Pushto.right = true;
//                 } else if (neighbor.y < target.y) {
//                     Pushto.bottom = true;
//                 }
//             }
//         }
//     }
//     return (Pushto);
// }

// struct nmath_sq_neighbors  pathfinding_Direction_Pullto(i32  * attackfrommap, size_t row_len, size_t col_len, u8 range[2], struct Point target) {
//     struct nmath_sq_neighbors  Pullto = {0, 0, 0, 0};
//     struct Point neighbor;
//     i32 * Pullto_ptr = (i32 *)&Pullto;
//     assert(range[0] < range[1]);
//     for (i32  distance = range[0]; distance <= range[1]; distance++) {
//         for (i32  i = 0; i < NMATH_SQUARE_NEIGHBOURS; i++) {
//             neighbor.x = nmath_inbounds((target.x + distance * q_cycle4_pzmz(i)), 0, col_len - 1);
//             neighbor.y = nmath_inbounds((target.y + distance * q_cycle4_zmzp(i)), 0, row_len - 1);
//             if ((attackfrommap[neighbor.y * col_len + neighbor.x] > 0) && (neighbor.x != 0) && (neighbor.x != (col_len - 1)) && (neighbor.y != 0) && (neighbor.x != (row_len - 1))) {
//                 *(Pullto_ptr + i) = true;
//             }
//         }
//     }
//     return (Pullto);
// }


// struct nmath_sq_neighbors  pathfinding_Direction_Block(i32  * costmap_pushpull, size_t row_len, size_t col_len, struct Point start) {
//     struct nmath_sq_neighbors  distance_block = {0, 0, 0, 0};
//     i32 * distance_ptr = (i32 *)&distance_block;
//     struct Point neighbor = {0, 0};
//     i32  distance = 0;
//     while ((distance_block.top == 0) || (distance_block.bottom == 0) || (distance_block.left == 0) || (distance_block.right == 0)) {
//         distance++;
//         for (i32  i = 0; i < NMATH_SQUARE_NEIGHBOURS; i++) {
//             neighbor.x = nmath_inbounds((start.x + q_cycle4_pzmz(i) * distance), 0, col_len - 1);
//             neighbor.y = nmath_inbounds((start.y + q_cycle4_zpzm(i) * distance), 0, row_len - 1);
//             if ((costmap_pushpull[neighbor.y * col_len + neighbor.x] == 0) && (*(distance_ptr + i) == 0)) {
//                 *(distance_ptr + i) = distance;
//             }
//             if ((neighbor.x == 0) && (distance_block.left == 0)) {
//                 distance_block.left = start.x;
//             } else if ((neighbor.y == 0) && (distance_block.top == 0)) {
//                 distance_block.top = start.y;
//             } else if ((neighbor.x == (col_len - 1)) && (distance_block.right == 0)) {
//                 distance_block.right = (col_len - 1) - start.x;
//             } else if ((neighbor.y == (row_len - 1)) && (distance_block.bottom == 0)) {
//                 distance_block.bottom = (row_len - 1) - start.y;
//             }
//         }
//     }
//     return (distance_block);
// }

// i32 * pathfinding_Map_PushPullto_noM(i32 * pushpulltomap, struct nmath_sq_neighbors  direction_block, struct nmath_sq_neighbors  pushpullto, size_t row_len, size_t col_len, struct Point start) {\
//     i32 temp_distance;
//     struct Point pushpullto_tile;
//     i32 * block_ptr = (i32 *)&direction_block;
//     i32 * pushpullto_ptr = (i32 *)&pushpullto;
//     for (size_t row = 0; row < row_len; row++) {
//         for (size_t col = 0; col < col_len; col++) {
//             pushpulltomap[(row * col_len + col)] = NMATH_PUSHPULLMAP_BLOCKED;
//         }
//     }
//     pushpulltomap[start.y * col_len + start.x] = 0;
//     for (i32  sq_neighbor = 0; sq_neighbor < NMATH_SQUARE_NEIGHBOURS; sq_neighbor++) {
//         temp_distance = *(block_ptr + sq_neighbor);
//         if (*(pushpullto_ptr + sq_neighbor) >= NMATH_PUSHPULLMAP_MINDIST) {
//         for (i32  distance = 1; distance < temp_distance; distance++) {
//             pushpullto_tile.x = nmath_inbounds((distance * q_cycle4_pzmz(sq_neighbor)) + start.x, 0, col_len - 1);
//             pushpullto_tile.y = nmath_inbounds((distance * q_cycle4_zmzp(sq_neighbor)) + start.y, 0, row_len - 1);
//             pushpulltomap[pushpullto_tile.y * col_len + pushpullto_tile.x] = distance;
//             }
//         }
//     }
//     return (pushpulltomap);
// }

// i32 * pathfinding_Map_PushPullto(struct nmath_sq_neighbors  direction_block, struct nmath_sq_neighbors  pushpullto, size_t row_len, size_t col_len, struct Point start, u8 mode_output) {
//     i32 * pushpulltomap = NULL;
//     i32 temp_distance;
//     struct Point pushpullto_tile;
//     i32 * block_ptr = (i32 *)&direction_block;
//     i32 * pushpullto_ptr = (i32 *)&pushpullto;
//     switch (mode_output) {
//         case (NMATH_POINTS_MODE_LIST):
//             pushpulltomap = DARR_INIT(pushpulltomap, i32, row_len * col_len * NMATH_TWO_D);
//             break;
//         case (NMATH_POINTS_MODE_MATRIX):
//             pushpulltomap = calloc(row_len * col_len, sizeof(*pushpulltomap));
//             for (size_t row = 0; row < row_len; row++) {
//                 for (size_t col = 0; col < col_len; col++) {
//                     pushpulltomap[(row * col_len + col)] = NMATH_PUSHPULLMAP_BLOCKED;
//                 }
//             }
//             break;
//     }
//     pushpulltomap[start.y * col_len + start.x] = 0;
//     for (i32  sq_neighbor = 0; sq_neighbor < NMATH_SQUARE_NEIGHBOURS; sq_neighbor++) {
//         temp_distance = *(block_ptr + sq_neighbor);
//         if (*(pushpullto_ptr + sq_neighbor) >= NMATH_PUSHPULLMAP_MINDIST) {
//             for (i32  distance = 1; distance < temp_distance; distance++) {
//                 pushpullto_tile.x = nmath_inbounds((distance * q_cycle4_pzmz(sq_neighbor)) + start.x, 0, col_len - 1);
//                 pushpullto_tile.y = nmath_inbounds((distance * q_cycle4_zmzp(sq_neighbor)) + start.y, 0, row_len - 1);
//                 switch (mode_output) {
//                     case NMATH_POINTS_MODE_LIST:
//                         DARR_PUT(pushpulltomap, pushpullto_tile.x);
//                         DARR_PUT(pushpulltomap, pushpullto_tile.y);
//                         break;
//                     case NMATH_POINTS_MODE_MATRIX:
//                         pushpulltomap[pushpullto_tile.y * col_len + pushpullto_tile.x] = distance;
//                         break;
//                 }
//             }
//         }
//     }
//     return (pushpulltomap);
// }

/* --- AStar --- */
i32 *Map_Pathfinding_CameFrom_List(i32 *path, i32 *came_from, size_t col_len,
                                   struct Point start, struct Point end) {
    struct Point current = end;
    struct Point move;
    DARR_NUM(path) = 0;
    for (size_t i = 0; i < NMATH_ITERATIONS_LIMIT; i++) {
        DARR_PUT(path, current.x);
        DARR_PUT(path, current.y);
        if ((current.x == start.x) && (current.y == start.y))
            break;
        move = Ternary_Moved(came_from[current.y * col_len + current.x]);
        current.x -= move.x;
        current.y -= move.y;
    }
    return (path);
}

i32 Map_Pathfinding_Manhattan(struct Point start, struct Point end) {
    /* Does not include endpoints */
    i32 distance = labs(start.x - end.x) + labs(start.y - end.y);
    return (distance);
}

i32 *Map_Pathfinding_Astar(i32 *path_list, i32 *costmap, size_t row_len,
                           size_t col_len, struct Point start, struct Point end) {
    /* Assumes square grid, path_list is a DARR */
    /* [1]: http://www.redblobgames.com/pathfinding/a-star/introduction.html */
    /* Checks */
    assert((start.x != end.x) || (start.y != end.y));
    assert(costmap[start.y * col_len + start.x] >= NMATH_MOVEMAP_MOVEABLEMIN);
    assert(costmap[end.y   * col_len + end.x]   >= NMATH_MOVEMAP_MOVEABLEMIN);

    /* Resetting path_list */
    DARR_NUM(path_list) = 0;

    /* Alloc variables */
    i32 *cost       = calloc(row_len * col_len, sizeof(*cost));
    i32 *came_from  = calloc(row_len * col_len, sizeof(*came_from));
    struct Nodeq current = {.x = start.x, .y = start.y, .cost = 0};
    struct Nodeq neighbor;

    /* Frontier points queue, by priority. */
    /* Lowest priority (movcost + distance) is top priority, at low index. */
    struct Nodeq *frontier_queue = DARR_INIT(frontier_queue, struct Nodeq, row_len * col_len);
    DARR_PUT(frontier_queue, current);

    /* Run algorithm until no points in frontier */
    while (DARR_NUM(frontier_queue) > 0) {
        current = DARR_POP(frontier_queue);

        /* End loop when end point is reached */
        if ((current.x == end.x) && (current.y == end.y))
            break;

        /* Visit all square neighbors */
        for (size_t sq_neighbor = 0; sq_neighbor < NMATH_SQUARE_NEIGHBOURS; sq_neighbor++) {
            /* Get next neighbor movement cost */
            neighbor.x = nmath_inbounds_int32_t(q_cycle4_mzpz(sq_neighbor) + current.x, 0, col_len - 1);
            neighbor.y = nmath_inbounds_int32_t(q_cycle4_zmzp(sq_neighbor) + current.y, 0, row_len - 1);
            neighbor.cost = current.cost + costmap[neighbor.y * col_len + neighbor.x];

            /* Compute conditions for adding neighbor to frontier */
            /* Skip neighbor if: blocked */
            bool blocked      = (costmap[neighbor.y * col_len + neighbor.x]  < NMATH_MOVEMAP_MOVEABLEMIN);
            if (blocked)
                continue;

            /* Skip neighbor if:  already visited AND higher cost */
            bool higher_cost  = (neighbor.cost >= cost[neighbor.y * col_len + neighbor.x]);
            bool visited      = (cost[   neighbor.y * col_len + neighbor.x] > 0);
            if (visited && higher_cost)
                continue;

            /* distance is heuristic for closeness to goal */
            size_t distance = linalg_distance_manhattan_int32_t(end.x, end.y, neighbor.x, neighbor.y);
            cost[neighbor.y * col_len + neighbor.x] = neighbor.cost;

            /* Djikstra algo only has cost in this step */
            neighbor.priority = neighbor.cost + distance;

            /* Find index to insert neighbor into priority queue. */
            size_t index = DARR_NUM(frontier_queue);
            while ((index > 0) && (neighbor.priority > frontier_queue[index - 1].priority))
                index--;

            DARR_INSERT(frontier_queue, neighbor, index);
            struct Point move = {neighbor.x - current.x, neighbor.y - current.y};

            /* Update came_from, to build better path */
            came_from[neighbor.y * col_len + neighbor.x] = Ternary_Direction(move);
        }
    }
    path_list = Map_Pathfinding_CameFrom_List(path_list, came_from, col_len, start, end);
    free(cost);
    free(came_from);
    DARR_FREE(frontier_queue);
    return (path_list);
}

i32 *Map_Pathfinding_Moveto(i32 *cost_matrix, size_t row_len, size_t col_len,
                            struct Point start, i32 move, int mode_output) {
    /* -- Setup output move_matrix -- */
    i32 *move_matrix = NULL;
    switch (mode_output) {
        case (NMATH_POINTS_MODE_LIST):
            move_matrix = DARR_INIT(move_matrix, i32, row_len * col_len * NMATH_TWO_D);
            break;
        case (NMATH_POINTS_MODE_MATRIX):
            move_matrix = calloc(row_len * col_len, sizeof(*move_matrix));
            for (size_t row = 0; row < row_len; row++) {
                for (size_t col = 0; col < col_len; col++) {
                    move_matrix[(row * col_len + col)] = NMATH_MOVEMAP_BLOCKED;
                }
            }
            break;
    }

    /* -- Setup variables -- */
    size_t init_size = row_len * col_len / 4;
    struct Node *open     = DARR_INIT(open,   struct Node, init_size);
    struct Node *closed   = DARR_INIT(closed, struct Node, init_size);
    struct Node current   = {start.x, start.y, 0}, neighbor;
    DARR_PUT(open, current);
    bool found;

    /* -- Loop over open nodes -- */
    while (DARR_NUM(open) > 0) {
        /* -- Get current open node from open list -- */
        current = DARR_POP(open);
        DARR_PUT(closed, current);

        /* -- Compute cost to current tile -- */
        i32 current_i   = current.y * col_len + current.x;
        i32 move_i      = move_matrix[current_i];
        switch (mode_output) {
            case NMATH_POINTS_MODE_MATRIX:
                if ((move_i == NMATH_MOVEMAP_BLOCKED) || (move_i > (current.distance + 1))) {
                    move_matrix[current_i] = current.distance + 1;
                }
                break;
            case NMATH_POINTS_MODE_LIST:
                size_t pnum = DARR_NUM(move_matrix) / NMATH_TWO_D;
                found = linalg_list_isIn_2D_int32_t(move_matrix, pnum, current.x,
                                                    current.y);
                if (!found) {
                    DARR_PUT(move_matrix, current.x);
                    DARR_PUT(move_matrix, current.y);
                }
                break;
        }

        /* -- Move to four square neighbor tiles -- */
        for (size_t i = 0; i < NMATH_SQUARE_NEIGHBOURS; i++) {
            /* - Get square neighbor - */
            neighbor.x = nmath_inbounds_int32_t(current.x + q_cycle4_mzpz(i), 0, col_len - 1);
            neighbor.y = nmath_inbounds_int32_t(current.y + q_cycle4_zmzp(i), 0, row_len - 1);

            /* - Get total cost to neighbor - */
            int current_n       = neighbor.y * col_len + neighbor.x;
            neighbor.distance   = current.distance + cost_matrix[current_n];

            /* - Skip if neighbor is out of reach - */
            if ((neighbor.distance > move) || (cost_matrix[current_n] == 0))
                continue;

            /* - Find if neighbor was already visited -> in closed - */
            int neighbor_inclosed = -1;
            for (i32 k = 0; k < DARR_NUM(closed); k++) {
                if ((neighbor.x == closed[k].x) && (neighbor.y == closed[k].y)) {
                    neighbor_inclosed = k;
                    break;
                }
            }

            /* - Add neighbor to open list, if not visited before - */
            if (neighbor_inclosed < 0) {
                DARR_PUT(open, neighbor);
                continue;
            }

            /* - Add neighbor to open list, if closer than before - */
            if (neighbor.distance < closed[neighbor_inclosed].distance) {
                DARR_DEL(closed, neighbor_inclosed);
                DARR_PUT(open,   neighbor);
            }

        }
    }
    DARR_FREE(open);
    DARR_FREE(closed);
    return (move_matrix);
}

void Map_Pathfinding_Moveto_noM(i32 *move_matrix, i32 *cost_matrix, size_t row_len,
                                size_t col_len, struct Point start, i32 move) {
    /* -- Wipe move_matrix -- */
    for (size_t row = 0; row < row_len; row++) {
        for (size_t col = 0; col < col_len; col++) {
            move_matrix[(row * col_len + col)] = NMATH_MOVEMAP_BLOCKED;
        }
    }

    /* -- Setup variables -- */
    size_t init_size    = row_len * col_len * 2;
    struct Node *open   = DARR_INIT(open,   struct Node, init_size);
    struct Node *closed = DARR_INIT(closed, struct Node, init_size);
    struct Node current = {start.x, start.y, 0}, neighbor;
    DARR_PUT(open, current);
    bool neighbor_inclosed;

    /* -- Loop over open nodes -- */
    while (DARR_NUM(open) > 0) {
        /* -- Get current open node from open list -- */
        current = DARR_POP(open);
        DARR_PUT(closed, current);

        /* -- Compute cost to current tile -- */
        i32 current_i   = current.y * col_len + current.x;
        i32 move_i      = move_matrix[current_i];
        if (( move_i == NMATH_MOVEMAP_BLOCKED) || (move_i > (current.distance + 1))) {
            move_matrix[current_i] = current.distance + 1;
        }

        /* -- Move to four square neighbor tiles -- */
        for (size_t i = 0; i < NMATH_SQUARE_NEIGHBOURS; i++) {
            /* - Get square neighbor - */
            neighbor.x = nmath_inbounds_int32_t(current.x + q_cycle4_mzpz(i), 0, col_len - 1);
            neighbor.y = nmath_inbounds_int32_t(current.y + q_cycle4_zmzp(i), 0, row_len - 1);

            /* - Get total cost to neighbor - */
            int current_n       = neighbor.y * col_len + neighbor.x;
            neighbor.distance   = current.distance + cost_matrix[current_n];

            /* - Skip if neighbor is out of reach - */
            if ((neighbor.distance > move) || (cost_matrix[current_n] == 0))
                continue;

            /* - Find if neighbor was already visited -> in closed - */
            int neighbor_inclosed = -1;
            for (i32 k = 0; k < DARR_NUM(closed); k++) {
                if ((neighbor.x == closed[k].x) && (neighbor.y == closed[k].y)) {
                    neighbor_inclosed = k;
                    break;
                }
            }

            /* - Add neighbor to open list, if not visited before - */
            if (neighbor_inclosed < 0) {
                DARR_PUT(open, neighbor);
                continue;
            }

            /* - Add neighbor to open list, if closer than before - */
            if (neighbor.distance < closed[neighbor_inclosed].distance) {
                DARR_DEL(closed, neighbor_inclosed);
                DARR_PUT(open,   neighbor);
            }
        }
    }
}
