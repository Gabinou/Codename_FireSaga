
#include "map/path.h"
#include "map/tiles.h"
#include "map/map.h"
#include "tile.h"
#include "position.h"
#include "utilities.h"
#include "pathfinding.h"
#include "nmath.h"
#include "unit/equipment.h"
#include "unit/unit.h"
#include "unit/anim.h"
#include "unit/boss.h"
#include "unit/range.h"
#include "unit/stats.h"
#include "unit/mount.h"
#include "unit/flags.h"
#include "unit/status.h"
#include "unit/loadout.h"
#include "unit/equipment.h"

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

/* Note: effective_move should have been multiplied by cost_multiplier */
i32 *_Map_Movemap_Compute(struct Map *map, struct Point start_in, i32 effective_move) {
    struct Point start = {start_in.x, start_in.y};
    Pathfinding_Moveto_noM(map->movemap, map->costmap,
                           map->row_len, map->col_len, start, effective_move);

    return (map->movemap);
}

i32 *Map_Movemap_Compute(struct Map *map, tnecs_entity unit_ent) {
    SDL_assert(map          != NULL);
    SDL_assert(map->world   != NULL);

    Map_Costmap_Movement_Compute(map, unit_ent);
    struct Unit     *unit   = IES_GET_COMPONENT(map->world, unit_ent, Unit);
    struct Position *pos    = IES_GET_COMPONENT(map->world, unit_ent, Position);
    i32    effective_move   = Unit_effectiveStats(unit).move * map->cost_multiplier;
    struct Point     start  = pos->tilemap_pos;
    return (_Map_Movemap_Compute(map, start, effective_move));
}

i32 *Map_Act_To(  struct Map *map, MapAct mapto) {
    SDL_assert(map              != NULL);
    SDL_assert(map->world       != NULL);
    SDL_assert(mapto.aggressor  > TNECS_NULL);

    struct Unit     *unit = IES_GET_COMPONENT(map->world, mapto.aggressor, Unit);
    struct Position *pos  = IES_GET_COMPONENT(map->world, mapto.aggressor, Position);
    SDL_assert(unit != NULL);
    SDL_assert(pos  != NULL);

    if (mapto.move) {
        Map_Costmap_Movement_Compute(map, mapto.aggressor);
        Map_Movemap_Compute(map, mapto.aggressor);
    } else {
        // only put start in move_matrix
        /* -- Wipe move_matrix -- */
        for (size_t i = 0; i < map->row_len * map->col_len; i++)
            map->movemap[i] = MOVEMAP_BLOCKED;

        i32 current_i   = pos->tilemap_pos.y * map->col_len + pos->tilemap_pos.x;
        map->movemap[current_i] = 1;
    }

    struct Point     start      = pos->tilemap_pos;
    i32 move_stat  = mapto.move ? Unit_effectiveStats(unit).move : 0;

    Range range = Range_default;
    if (mapto.eq_type == LOADOUT_EQUIPPED) {
        // SDL_Log("LOADOUT_EQUIPPED");
        Unit_Range_Equipped(unit, mapto.archetype, &range);
    } else if (mapto.eq_type == LOADOUT_EQUIPMENT) {
        // SDL_Log("LOADOUT_EQUIPMENT");
        Unit_Range_Equipment(unit, mapto.archetype, &range);
    } else if (mapto.eq_type == LOADOUT_EQ) {
        // SDL_Log("LOADOUT_EQ");
        Unit_Range_Eq(unit, mapto._eq, mapto.archetype, &range);
    } else if (mapto.eq_type == LOADOUT_LOADOUT) {
        // SDL_Log("LOADOUT_LOADOUT");
        /* Save starting equipment */
        i32 start_equipped[UNIT_ARMS_NUM];
        Unit_Equipped_Export(unit, start_equipped);

        /* Compute healmap/attackmap with input loadout */
        Unit_Equipped_Import(unit, mapto._loadout);
        Unit_Range_Equipped(unit, mapto.archetype, &range);

        /* Restore starting equipment */
        Unit_Equipped_Import(unit, start_equipped);
    }
    // SDL_Log("range %d %d", range->min, range->max);

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
    /* Bug: healtolist: Illuminate tiles depending on staff TARGET.*/
    /* Bug: healtolist: don't include start tile in attacktomap, but include its NEIGHBORS.
    .*/

    PathfindingAct actto    = PathfindingAct_default;
    actto.movemap           = map->movemap;
    actto.acttomap          = *tomap;
    actto.occupymap         = input_occupymap;
    actto.row_len           = map->row_len;
    actto.col_len           = map->col_len;
    actto.self              = mapto.aggressor;
    actto.range             = range;
    actto.mode_movetile     = mapto.mode_movetile;

    Pathfinding_Attackto_noM(actto);

    // printf("Map_Act_to occupymap\n");
    // if (input_occupymap != NULL)
    //     entity_print(input_occupymap, map->row_len, map->col_len);
    // printf("Map_Act_to ATK\n");
    // matrix_print(map->attacktomap, map->row_len, map->col_len);

    i32* out = NULL;
    if (mapto.output_type == ARRAY_MATRIX) {
        out     = *tomap;
    } else if (mapto.output_type == ARRAY_LIST)  {
        *tolist = matrix2list_noM(*tomap, *tolist,
                                  map->row_len, map->col_len);
        out     = *tolist;
    }
    SDL_assert(out != NULL);

    map->update = true;
    return (out);
}

i32 *Map_Act_From(struct Map *map, MapAct map_from) {
    SDL_assert(map                  != NULL);
    SDL_assert(map->world           != NULL);
    SDL_assert(map_from.aggressor   > TNECS_NULL);
    SDL_assert(map_from.defendant   > TNECS_NULL);

    Map_Costmap_Movement_Compute(map, map_from.aggressor);
    // matrix_print(map->costmap, map->row_len, map->col_len);

    struct Unit *agg_unit       = IES_GET_COMPONENT(map->world, map_from.aggressor, Unit);
    /* Get dft position */
    struct Position *agg_pos    = IES_GET_COMPONENT(map->world, map_from.aggressor, Position);
    struct Position *dft_pos    = IES_GET_COMPONENT(map->world, map_from.defendant, Position);
    /* Get agg range */
    struct Range range = Range_default;
    Unit_Range_Equipped(agg_unit, ITEM_ARCHETYPE_WEAPON, &range);

    /* Compute movemap */
    i32 move_stat       = map_from.move ? Unit_effectiveStats(agg_unit).move : 0;
    i32 effective_move  = move_stat * map->cost_multiplier;

    _Map_Movemap_Compute(map, agg_pos->tilemap_pos, effective_move);
    // matrix_print(map->movemap, map->row_len, map->col_len);


    i32 **tomap  = NULL;
    i32 **tolist = NULL;
    if (map_from.archetype == ITEM_ARCHETYPE_WEAPON) {
        tomap   = &map->attackfrommap;
        tolist  = &map->attackfrommap;
    } else if (map_from.archetype == ITEM_ARCHETYPE_STAFF) {
        tomap   = &map->healfrommap;
        tolist  = &map->healfrommap;
    }

    tnecs_entity *input_occupymap = (map_from.move == true) ? map->unitmap : NULL;

    /* Compute new attacktomap */

    PathfindingAct actto    = PathfindingAct_default;
    actto.movemap           = map->movemap;
    actto.acttomap          = *tomap;
    actto.occupymap         = input_occupymap;
    actto.row_len           = map->row_len;
    actto.col_len           = map->col_len;
    actto.self              = map_from.aggressor;
    actto.range             = range;
    actto.mode_movetile     = map_from.mode_movetile;

    Pathfinding_Attackto_noM(actto);

    i32* out = NULL;
    if (map_from.output_type == ARRAY_MATRIX) {
        out     = *tomap;
    } else if (map_from.output_type == ARRAY_LIST)  {
        *tolist = matrix2list_noM(*tomap, *tolist,
                                  map->row_len, map->col_len);
        out     = *tolist;
    }
    SDL_assert(out != NULL);

    map->update = true;
    return (out);
}

i32 *Map_Danger_Compute(struct Map *map, tnecs_entity unit_ent) {
    SDL_assert(map          != NULL);
    SDL_assert(map->world   != NULL);

    Map_Costmap_Movement_Compute(map, unit_ent);
    struct Position *position   = IES_GET_COMPONENT(map->world, unit_ent, Position);
    struct Unit *unit           = IES_GET_COMPONENT(map->world, unit_ent, Unit);
    SDL_assert(position != NULL);
    SDL_assert(unit     != NULL);
    i32 effective_move = Unit_effectiveStats(unit).move * map->cost_multiplier;
    struct Point start = position->tilemap_pos;
    _Map_Movemap_Compute(map, start, effective_move);
    struct Range range = Range_default;
    Unit_Range_Equipment(unit, ITEM_ARCHETYPE_WEAPON, &range);

    PathfindingAct actto    = PathfindingAct_default;
    actto.movemap           = map->movemap;
    actto.acttomap          = map->attacktomap;
    actto.occupymap         = map->unitmap;
    actto.row_len           = map->row_len;
    actto.col_len           = map->col_len;
    actto.self              = unit_ent;
    actto.range             = range;
    actto.mode_movetile     = MOVETILE_INCLUDE;

    Pathfinding_Attackto_noM(actto);

    // memset(map->temp, 0, sizeof(*map->temp)*map->row_len * map->col_len);
    map->temp = matrix_plus_noM(map->temp, map->attacktomap, map->row_len * map->col_len);
    // matrix_print(map->temp, map->row_len, map->col_len);
    return (map->temp);
}

i32 *Map_Costmap_PushPull_Compute(struct Map *map, tnecs_entity unit_ent) {
    SDL_assert(unit_ent != 0);
    SDL_assert(map          != NULL);
    SDL_assert(map->costmap != NULL);
    SDL_assert(map->world   != NULL);
    memset(map->costmap, 0, sizeof(*map->costmap) * map->col_len * map->row_len);
    struct Unit *unit = IES_GET_COMPONENT(map->world, unit_ent, Unit);
    struct Tile *temp_tile;
    i32 tile_ind = 0;
    i8 unit_movetype = Unit_Movement(unit);
    u8 army = Unit_Army(unit);
    u8 ontile_army;
    tnecs_entity ontile_unit_ent;
    SDL_assert(unit_movetype > UNIT_MVT_START);
    for (u8 i = 0; i < map->row_len * map->col_len; i++) {
        tile_ind = map->tilemap[i] / TILE_DIVISOR;
        ontile_unit_ent = map->unitmap[i];
        SDL_assert(tile_ind > 0);
        size_t tile_order = Map_Tile_Order(map, tile_ind);
        temp_tile = map->tiles + tile_order;
        i32* cost_array = Tile_Cost_Array(temp_tile);
        map->costmap[i] = cost_array[unit_movetype] * map->cost_multiplier;
        if (ontile_unit_ent <= TNECS_NULL)
            continue;
        struct Unit *ontile_unit = IES_GET_COMPONENT(map->world, ontile_unit_ent, Unit);
        if (ontile_unit != NULL)
            map->costmap[i] = COSTMAP_BLOCKED;
    }
    return (map->costmap);
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
    SDL_assert(Unit_Movement(unit) > UNIT_MVT_START);

    /* Reset costmap */
    memset(map->costmap, 0, sizeof(*map->costmap) * map->col_len * map->row_len);

    /* - Compute cost of each tile - */
    int len = (map->col_len * map->row_len);
    SDL_assert(len > 0);
    for (int i = 0; i < len; ++i) {
#ifdef UNITS_IGNORE_TERRAIN

        /* - All units fly - */
        map->costmap[i] = COSTMAP_MIN * map->cost_multiplier;

#else /* !UNITS_IGNORE_TERRAIN */

        // /* - Compute cost from tile - */
        i32 tile_ind = map->tilemap[i] / TILE_DIVISOR;
        SDL_assert(tile_ind > 0);
        size_t tile_order = Map_Tile_Order(map, tile_ind);
        struct Tile *temp_tile = map->tiles + tile_order;
        i32* cost_array = Tile_Cost_Array(temp_tile);
        map->costmap[i] = cost_array[Unit_Movement(unit)] * map->cost_multiplier;

#endif /* UNITS_IGNORE_TERRAIN */

        /* - Check if tile is occupied by enemy unit - */
        tnecs_entity ontile_unit_ent = map->unitmap[i];

        // Skip if tile unoccupied
        if (ontile_unit_ent <= TNECS_NULL) {
            continue;
        }

        struct Unit *ontile_unit = IES_GET_COMPONENT(map->world, ontile_unit_ent, Unit);
        SDL_assert(ontile_unit != NULL);

        u8 ontile_army = Unit_Army(ontile_unit);
        SDL_assert((ontile_army < ARMY_END) && (ontile_army > ARMY_START));

        if (SotA_army2alignment(ontile_army) != SotA_army2alignment(Unit_Army(unit))) {
            map->costmap[i] = COSTMAP_BLOCKED;
        }
    }
    return (map->costmap);
}
b32 Map_Costmap_Skip(Map *map, tnecs_entity unit_ent) {
    /* - Skip if previously computed - */
    return (map->costmap_ent == unit_ent);
}

void Map_Costmap_Wipe(Map *map) {
    map->costmap_ent = TNECS_NULL;
}

i32 *Map_Costmap_Movement_Compute(struct Map *map, tnecs_entity unit_ent) {
    SDL_assert(map          != NULL);
    SDL_assert(map->world   != NULL);
    SDL_assert(map->unitmap != NULL);
    SDL_assert(map->costmap != NULL);

    if (Map_Costmap_Skip(map, unit_ent)) {
        return (map->costmap);
    }

    /* - Preliminaries - */
    map->costmap_ent = unit_ent;
    struct Unit *unit = IES_GET_COMPONENT(map->world, unit_ent, Unit);

    return (_Map_Costmap_Movement_Compute(map, unit));
}

void Map_globalRange(struct Map *map, u8 alignment) {
    SDL_assert(map          != NULL);
    SDL_assert(map->world   != NULL);

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
        struct Unit     *temp_unit  = IES_GET_COMPONENT(map->world, unit_entities[i], Unit);
        struct Position *temp_pos   = IES_GET_COMPONENT(map->world, unit_entities[i], Position);
        struct Range range = Range_default;
        Unit_Range_Equipment(temp_unit, ITEM_ARCHETYPE_WEAPON, &range);

        struct Unit_stats temp_effective_stats = Unit_effectiveStats(temp_unit);
        u8 move = temp_effective_stats.move;
        struct Point start = {temp_pos->tilemap_pos.x, temp_pos->tilemap_pos.y};
        Map_Costmap_Movement_Compute(map, unit_entities[i]);
        Pathfinding_Moveto_noM(map->movemap, map->costmap, map->row_len,
                               map->col_len, start, move);

        PathfindingAct actto    = PathfindingAct_default;
        actto.movemap           = map->movemap;
        actto.acttomap          = map->attacktomap;
        actto.occupymap         = map->unitmap;
        actto.row_len           = map->row_len;
        actto.col_len           = map->col_len;
        actto.self              = unit_entities[i];
        actto.range             = range;
        actto.mode_movetile     = MOVETILE_INCLUDE;

        Pathfinding_Attackto_noM(actto);
        map->global_rangemap = matrix_plus_noM(map->global_rangemap, map->attacktomap,
                                               map->row_len * map->col_len);
    }
}

void Map_Danger_Perimeter_Compute(struct Map *map, i32 *danger) {
    if (map->edges_danger == NULL) {
        size_t bytesize = sizeof(struct Padding);
        map->edges_danger = SDL_calloc(map->row_len * map->col_len, bytesize);
    }
    Map_Perimeter(map->edges_danger, danger, map->row_len, map->col_len);
}

struct Padding *Map_PerimeterM(i32 *map, i32 row_len, i32 col_len) {
    size_t bytesize = sizeof(struct Padding);
    struct Padding *edges = SDL_calloc(row_len * col_len, bytesize);
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
