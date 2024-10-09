
#include "map/path.h"

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

void Map_Stacked_Dangermap_Compute(struct Map *map, i32 *dangermap) {
    SDL_assert(map->stacked_dangermap != NULL);
    int size = map->row_len * map->col_len;
    i32 *temp_map = matrix_ssmaller(dangermap, DANGERMAP_UNIT_DIVISOR, size);
    map->stacked_dangermap = matrix_and_noM(map->stacked_dangermap,
                                            dangermap, temp_map,
                                            map->row_len * map->col_len);
    map->stacked_dangermap = matrix_and_noM(map->stacked_dangermap,
                                            map->stacked_dangermap,
                                            map->movemap,
                                            map->row_len * map->col_len);
    SDL_free(temp_map);
    map->shading_changed = true;
}

void Map_Stacked_Dangermap_Reset(struct Map *map) {
    int len = map->row_len * map->col_len;
    if (map->stacked_dangermap != NULL) {
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

    return (map->movemap);
}

i32 *Map_Movemap_Compute(struct Map *map, tnecs_world *world, tnecs_entity unit_ent) {
    SDL_assert(map          != NULL);
    map->world = world;
    SDL_assert(map->world   != NULL);

    Map_Costmap_Movement_Compute(map, unit_ent);
    struct Unit     *unit   = TNECS_GET_COMPONENT(map->world, unit_ent, Unit);
    struct Position *pos    = TNECS_GET_COMPONENT(map->world, unit_ent, Position);
    i32              move   = Unit_getStats(unit).move;
    struct Point     start  = pos->tilemap_pos;
    return (_Map_Movemap_Compute(map, start, move));
}

// TODO: get rid of this useless interface
i32 *_Map_tomap_Compute(i32 *tomap, i32 *movemap, tnecs_entity *occupymap,
                        u8 row_len, u8 col_len, i32 move,
                        struct Range *range, u8 mode_movetile) {
    Pathfinding_Attackto_noM(tomap, movemap, occupymap,
                             row_len, col_len,
                             (i32 *)range, mode_movetile);
    return (tomap);
}

i32 *Map_Healtolist_Compute(struct Map   *map) {
    SDL_assert(map->healtomap  != NULL);
    SDL_assert(map->healtolist != NULL);
    map->healtolist = matrix2list_noM(map->healtomap, map->healtolist,
                                      map->row_len, map->col_len);
    return (map->healtolist);
}

// i32 *Map_Healtomap_Compute_wLoadout(struct Map *map, tnecs_world *world, tnecs_entity ent,
//                                     b32 move, Loadout *loadout) {
//     struct Unit     *unit = TNECS_GET_COMPONENT(world, ent, Unit);

//     /* Save starting equipment */
//     i32 start_equipped[UNIT_ARMS_NUM];
//     Unit_Equipped_Export(unit, start_equipped);

//     /* Compute healmap with input loadout */
//     Unit_Loadout_Import(unit, loadout);
//     i32 *out = Map_Healtomap_Compute(map, world, ent, move, true);

//     /* Restore starting equipment */
//     Unit_Equipped_Import(unit, start_equipped);

//     return (out);
// }


// i32 *Map_Attacktolist_Compute(struct Map *map) {
//     SDL_assert(map->attacktolist    != NULL);
//     SDL_assert(map->attacktomap     != NULL);
//     map->attacktolist = matrix2list_noM(map->attacktomap, map->attacktolist,
//                                         map->row_len, map->col_len);
//     return (map->attacktolist);
// }

i32 *Map_Attackfromlist_Compute(struct Map *map) {
    map->attackfromlist = matrix2list_noM(map->attackfrommap, map->attackfromlist,
                                          map->row_len, map->col_len);
    return (map->attackfromlist);
}

// i32 *Map_Maptomap_Compute_wLoadout(struct Map *map, tnecs_world *world, tnecs_entity ent,
//                                       b32 move, Loadout *loadout) {
//     struct Unit     *unit = TNECS_GET_COMPONENT(world, ent, Unit);

//     /* Save starting equipment */
//     i32 start_equipped[UNIT_ARMS_NUM];
//     Unit_Equipped_Export(unit, start_equipped);

//     /* Compute healmap with input loadout */
//     Unit_Loadout_Import(unit, loadout);
//     i32 *out = Map_Maptomap_Compute(map, world, ent, move, true);

//     /* Restore starting equipment */
//     Unit_Equipped_Import(unit, start_equipped);

//     return (out);
// }

i32 *Map_Mapto(  struct Map *map, tnecs_entity unit_ent, MapTo mapto) {
    SDL_assert(map          != NULL);
    SDL_assert(map->world   != NULL);

    Map_Costmap_Movement_Compute(map, unit_ent);
    struct Unit     *unit = TNECS_GET_COMPONENT(map->world, unit_ent, Unit);
    struct Position *pos  = TNECS_GET_COMPONENT(map->world, unit_ent, Position);
    SDL_assert(unit != NULL);
    SDL_assert(pos  != NULL);

    struct Point     start      = pos->tilemap_pos;
    i32 move_stat  = mapto.move ? Unit_getStats(unit).move : 0;

    Range *range = NULL;
    if (mapto.eq_type == LOADOUT_EQUIPPED) {
        range = Unit_Range_Equipped(unit, mapto.archetype);
    } else if (mapto.eq_type == LOADOUT_EQUIPMENT) {
        range = Unit_Range_Equipment(unit, mapto.archetype);
    } else if (mapto.eq_type == LOADOUT_LOADOUT) {
        /* Save starting equipment */
        i32 start_equipped[UNIT_ARMS_NUM];
        Unit_Equipped_Export(unit, start_equipped);

        /* Compute healmap with input loadout */
        Unit_Equipped_Import(unit, mapto._loadout);
        range = Unit_Range_Equipped(unit, mapto.archetype);

        /* Restore starting equipment */
        Unit_Equipped_Import(unit, start_equipped);
    }
    SDL_assert(range != NULL);

    // Enable occupymap only to check when unit actually MOVES
    tnecs_entity *input_occupymap = (mapto.move == true) ? map->unitmap : NULL;

    i32 **tomap  = NULL;
    i32 **tolist = NULL;
    if (mapto.archetype == ITEM_ARCHETYPE_WEAPON) {
        tomap   = &map->attacktomap;
        tolist  = &map->attacktolist;
    } else if (mapto.archetype == ITEM_ARCHETYPE_STAFF) {
        tomap   = &map->healtomap;
        tolist  = &map->healtolist;
    }
    SDL_assert(tomap    != NULL);
    SDL_assert(tolist   != NULL);

    /* Compute new attacktomap */
    *tomap = _Map_tomap_Compute(*tomap, map->movemap,
                                input_occupymap, map->row_len, map->col_len,
                                move_stat, range, mapto.mode_movetile);
    // matrix_print(map->attacktomap, map->row_len, map->col_len);

    i32* out = NULL;
    if (mapto.output_type == ARRAY_MATRIX) {
        out     = *tomap;
    } else if (mapto.output_type == ARRAY_MATRIX)  {
        *tolist = matrix2list_noM(*tomap, map->attacktolist,
                                  map->row_len, map->col_len);
        out     = *tolist;
    }
    SDL_assert(out != NULL);

    map->update = true;
    return (out);
}


// i32 *Map_Healtomap_Compute(struct Map *map, tnecs_world *world, tnecs_entity unit_ent,
//                            b32 move, b32 equipped) {
//     SDL_assert(map          != NULL);
//     map->world = world;
//     SDL_assert(map->world   != NULL);

//     Map_Costmap_Movement_Compute(map, unit_ent);
//     struct Unit     *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
//     struct Position *pos  = TNECS_GET_COMPONENT(world, unit_ent, Position);
//     struct Point start = pos->tilemap_pos;
//     i32 move_stat = move ? Unit_getStats(unit).move : 0;
//     _Map_Movemap_Compute(map, start, move_stat);
//     struct Range *range = Unit_Range_Equipped(unit, ITEM_ARCHETYPE_STAFF);

//     map->update = true;
//     map->healtomap = _Map_tomap_Compute(map->healtomap, map->movemap,
//                                         map->unitmap,
//                                         map->row_len, map->col_len,
//                                         move_stat, range, MOVETILE_INCLUDE);
//     // matrix_print(map->healtomap, map->row_len, map->col_len);
//     return (map->healtomap);
// }

// i32 *Map_Maptomap_Compute(struct Map *map, tnecs_world *world,
//                              tnecs_entity unit_ent, b32 move, b32 equipped) {
//     SDL_assert(map          != NULL);
//     // map->world = world;
//     SDL_assert(map->world   != NULL);

//     /* Compute Movemap */
//     Map_Costmap_Movement_Compute(map, unit_ent);
//     struct Unit     *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
//     struct Position *pos  = TNECS_GET_COMPONENT(world, unit_ent, Position);
//     struct Point     start      = pos->tilemap_pos;
//     i32              move_stat  = move ? Unit_getStats(unit).move : 0;

//     _Map_Movemap_Compute(map, start, move_stat);

//     /* Compute Unit range */
//     struct Range *range = Unit_Range_Equipped(unit, ITEM_ARCHETYPE_WEAPON);

//     /* Enable occupymap */
//     tnecs_entity *input_occupymap = NULL;
//     if (move == true) {
//         // Enable occupymap only to check when unit actually MOVES
//         input_occupymap = map->unitmap;
//     }

//     /* Compute new attacktomap */
//     map->attacktomap = _Map_tomap_Compute(map->attacktomap, map->movemap,
//                                           input_occupymap,
//                                           map->row_len, map->col_len,
//                                           move_stat, range, MOVETILE_INCLUDE);
//     map->update = true;
//     // matrix_print(map->attacktomap, map->row_len, map->col_len);
//     return (map->attacktomap);
// }

i32 *Map_Attackfrommap_Compute(struct Map *map, tnecs_world *world, tnecs_entity agg,
                               tnecs_entity dft, b32 move, b32 equipped) {
    SDL_assert(map          != NULL);
    map->world = world;
    SDL_assert(map->world   != NULL);

    Map_Costmap_Movement_Compute(map, agg);
    // matrix_print(map->costmap, map->row_len, map->col_len);

    struct Unit *agg_unit       = TNECS_GET_COMPONENT(world, agg, Unit);
    /* Get dft position */
    struct Position *agg_pos    = TNECS_GET_COMPONENT(world, agg, Position);
    struct Position *dft_pos    = TNECS_GET_COMPONENT(world, dft, Position);
    /* Get agg range */
    struct Range *range = Unit_Range_Equipped(agg_unit, ITEM_ARCHETYPE_WEAPON);

    /* Compute movemap */
    i32 move_stat = move ? Unit_getStats(agg_unit).move : 0;
    _Map_Movemap_Compute(map, agg_pos->tilemap_pos, move_stat);
    // matrix_print(map->movemap, map->row_len, map->col_len);

    Pathfinding_Attackfrom_noM(map->attackfrommap, map->movemap, map->row_len,
                               map->col_len, dft_pos->tilemap_pos, (i32 *)range);

    return (map->attackfrommap);
}

i32 *Map_Danger_Compute(struct Map *map, tnecs_world *world, tnecs_entity unit_ent) {
    SDL_assert(map          != NULL);
    map->world = world;
    SDL_assert(map->world   != NULL);

    Map_Costmap_Movement_Compute(map, unit_ent);
    struct Position *position   = TNECS_GET_COMPONENT(world, unit_ent, Position);
    struct Unit *unit           = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    SDL_assert(position != NULL);
    SDL_assert(unit != NULL);
    i32 move = Unit_getStats(unit).move;
    struct Point start = position->tilemap_pos;
    _Map_Movemap_Compute(map, start, move);
    struct Range *range = Unit_Range_Equipment(unit, ITEM_ARCHETYPE_WEAPON);

    map->attacktomap = _Map_tomap_Compute(map->attacktomap, map->movemap,
                                          map->unitmap,
                                          map->row_len, map->col_len,
                                          move, range, MOVETILE_INCLUDE);
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

/* Create costmap for unit. Block tiles if occupied by enemy.
*   costmap: how many points to traverse/move to tile.
*/
i32 *_Map_Costmap_Movement_Compute(struct Map *map, struct Unit *unit) {
    SDL_assert(map          != NULL);
    SDL_assert(map->world   != NULL);
    SDL_assert(map->unitmap != NULL);
    SDL_assert(map->costmap != NULL);
    SDL_assert(map->tilemap != NULL);
    SDL_assert(map->col_len > 0);
    SDL_assert(map->row_len > 0);
    SDL_assert(unit         != NULL);
    SDL_assert(unit->mvt_type > UNIT_MVT_START);

    /* Reset costmap */
    memset(map->costmap, 0, sizeof(*map->costmap) * map->col_len * map->row_len);

    /* - Compute cost of each tile - */
    int len = (map->col_len * map->row_len);
    SDL_assert(len > 0);
    for (int i = 0; i < len; ++i) {
#ifdef UNITS_IGNORE_TERRAIN

        /* - All units fly - */
        map->costmap[i] = COSTMAP_MIN;

#else /* !UNITS_IGNORE_TERRAIN */

        // /* - Compute cost from tile - */
        i32 tile_ind = map->tilemap[i] / TILE_DIVISOR;
        SDL_assert(tile_ind > 0);
        size_t tile_order = Map_Tile_Order(map, tile_ind);
        struct Tile *temp_tile = map->tiles + tile_order;
        map->costmap[i] = temp_tile->cost_array[unit->mvt_type];

#endif /* UNITS_IGNORE_TERRAIN */

        /* - Check if tile is occupied by enemy unit - */
        tnecs_entity ontile_unit_ent = map->unitmap[i];

        // Skip if tile unoccupied
        if (ontile_unit_ent <= TNECS_NULL) {
            continue;
        }

        struct Unit *ontile_unit = TNECS_GET_COMPONENT(map->world, ontile_unit_ent, Unit);
        SDL_assert(ontile_unit != NULL);

        u8 ontile_army = ontile_unit->army;
        SDL_assert((ontile_army < ARMY_END) && (ontile_army > ARMY_START));

        if (SotA_army2alignment(ontile_army) != SotA_army2alignment(unit->army)) {
            map->costmap[i] = COSTMAP_BLOCKED;
        }
    }
    return (map->costmap);
}

i32 *Map_Costmap_Movement_Compute(struct Map *map, tnecs_entity unit_ent) {
    SDL_assert(map          != NULL);
    SDL_assert(map->world   != NULL);
    SDL_assert(map->unitmap != NULL);
    SDL_assert(map->costmap != NULL);

    /* - Skip if previously computed - */
    if (map->costmap_ent == unit_ent) {
        return (map->costmap);
    }

    /* - Preliminaries - */
    map->costmap_ent = unit_ent;
    struct Unit *unit = TNECS_GET_COMPONENT(map->world, unit_ent, Unit);

    return (_Map_Costmap_Movement_Compute(map, unit));
}

void Map_globalRange(struct Map *map, tnecs_world *world, u8 alignment) {
    SDL_assert(map != NULL);
    map->world = world;
    SDL_assert(map->world != NULL);

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
        struct Unit     *temp_unit  = TNECS_GET_COMPONENT(map->world, unit_entities[i], Unit);
        struct Position *temp_pos   = TNECS_GET_COMPONENT(map->world, unit_entities[i], Position);
        struct Range *range = Unit_Range_Equipment(temp_unit, ITEM_ARCHETYPE_WEAPON);

        struct Unit_stats temp_effective_stats = Unit_effectiveStats(temp_unit);
        u8 move = temp_effective_stats.move;
        struct Point start = {temp_pos->tilemap_pos.x, temp_pos->tilemap_pos.y};
        Map_Costmap_Movement_Compute(map, unit_entities[i]);
        Pathfinding_Moveto_noM(map->movemap, map->costmap, map->row_len,
                               map->col_len, start, move);
        Pathfinding_Attackto_noM(map->attacktomap, map->movemap,
                                 map->unitmap,
                                 map->row_len, map->col_len,
                                 (i32 *)range, MOVETILE_INCLUDE);
        map->global_rangemap = matrix_plus_noM(map->global_rangemap, map->attacktomap,
                                               map->row_len * map->col_len);
    }
}

void Map_Danger_Perimeter_Compute(struct Map *map, i32 *danger) {
    if (map->edges_danger == NULL) {
        size_t bytesize = sizeof(struct Padding);
        map->edges_danger = calloc(map->row_len * map->col_len, bytesize);
    }
    Map_Perimeter(map->edges_danger, danger, map->row_len, map->col_len);
}

struct Padding *Map_PerimeterM(i32 *map, i32 row_len, i32 col_len) {
    size_t bytesize = sizeof(struct Padding);
    struct Padding *edges = calloc(row_len * col_len, bytesize);
    Map_Perimeter(edges, map, row_len, col_len);
    return (edges);
}

void Map_Perimeter(struct Padding *edges, i32 *map, i32 row_len, i32 col_len) {
    /* Find all perimeter edges of a tilemap */
    // Every tile has one Padding array defining the 4 edges.
    // Every edge that needs to be drawn is set to 1,
    // only for inside tile.
    SDL_assert(edges != NULL);
    i32 outside = 0;
    size_t bytesize = row_len * col_len * sizeof(*edges);
    memset(edges, 0, bytesize);

    /* Iterate over all tiles */
    /* Only set edge if one tile is in, and other outside */
    /* Only set edge to be drawn for inside tiles */
    for (i32 i = 0; i < row_len * col_len; i++) {
        i32 x = i % col_len;
        i32 y = i / col_len;
        for (i32 ii = 0; ii < SQUARE_NEIGHBOURS; ii++) {
            /* ii = 0: SOTA_PADDING_RIGHT   +x  */
            /* ii = 1: SOTA_PADDING_TOP     -y  */
            /* ii = 2: SOTA_PADDING_LEFT    -x  */
            /* ii = 3: SOTA_PADDING_BOTTOM  +y  */
            i32 plusx = q_cycle4_pzmz(ii);
            i32 plusy = q_cycle4_zmzp(ii);
            i32 nx = int_inbounds(x + plusx, 0, col_len - 1);
            i32 ny = int_inbounds(y + plusy, 0, row_len - 1);
            i32 neighbor = nx + col_len * ny;

            /* Skip if neighbor is the same as current tile */
            if (neighbor == i)
                continue;

            /* Skip if tile is outside */
            // Prevents setting perimeter tile from neighbor
            b32 i_outside = (map[i]         == outside);
            if (i_outside)
                continue;

            /* Skip if tiles are both inside, or both outside */
            b32 n_outside = (map[neighbor]  == outside);
            if (i_outside == n_outside)
                continue;

            /* Setting perimeter tile to inside tile only */
            i32 *pad_arr = (i32 *) & (edges[i]);
            pad_arr[ii]  = 1;
        }
    }
}