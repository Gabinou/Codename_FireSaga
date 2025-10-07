
#include "nmath.h"
#include "weapon.h"
#include "globals.h"
#include "position.h"
#include "utilities.h"
#include "pathfinding.h"

#include "map/map.h"
#include "map/find.h"
#include "map/path.h"

#include "unit/unit.h"
#include "unit/anim.h"
#include "unit/boss.h"
#include "unit/flags.h"
#include "unit/range.h"
#include "unit/stats.h"
#include "unit/mount.h"
#include "unit/status.h"
#include "unit/loadout.h"
#include "unit/equipment.h"

/*-- Map Usable -- */

/* Find if a weapon/staff usable by agg has an enemy in range */
struct Unit_Equippable Map_canEquip(struct Map  *map,
                                    tnecs_E      agg_ent,
                                    canEquip     can_equip) {
    SDL_assert(map          != NULL);
    SDL_assert(gl_world   != NULL);
    SDL_assert((can_equip.archetype == ITEM_ARCHETYPE_WEAPON) ||
               (can_equip.archetype == ITEM_ARCHETYPE_STAFF));

    Unit     *agg   = IES_GET_C(gl_world, agg_ent, Unit);
    SDL_assert(agg != NULL);
    Position *pos   = IES_GET_C(gl_world, agg_ent, Position);
    SDL_assert(pos != NULL);
    _Map_Costmap_Movement_Compute(map, agg);

    /* Compute movemap */
    struct Point start  = pos->tilemap_pos;
    Unit_stats eff_s    = Unit_effectiveStats(agg);
    i32 move_stat       = can_equip.move ? eff_s.move : 0;
    i32 effective_move  = Map_Cost_Effective(map, move_stat);
    // printf("effective_move %d\n", effective_move);
    // SDL_Log("effective_move %d\n", effective_move);

    _Map_Movemap_Compute(map, start, effective_move);

    // printf("MOVE\n");
    // matrix_print(map->darrs.movemap, Map_row_len(map), Map_col_len(map));

    /* Alloc defendants */
    tnecs_E *dfts  = DARR_INIT(dfts, tnecs_E, 4);

    /* Make canEquip for Map_canEquip_Range */
    canEquip range_can_equip    = can_equip;
    range_can_equip.eq_type     = LOADOUT_EQ;

    struct Unit_Equippable equippable = Unit_Equippable_default;
    for (int eq = ITEM1; eq < SOTA_EQUIPMENT_SIZE; eq++) {
        /* Skip if weapon is not usable */

        // SDL_Log("Map_canEquip eq %d \n", eq);
        canEquip_Eq(&can_equip, eq);

        /* -- Skip if eq doesn't match -- */
        if ((can_equip.eq_type == LOADOUT_EQ) &&
            (eq != can_equip._eq)
           ) {
            continue;
        }

        if (!Unit_canEquip_AnyHand(agg, can_equip)) {
            /* SDL_Log("!Unit_canEquip_AnyHand"); */
            continue;
        }

        range_can_equip._eq         = eq;

        /* -- Skip if eq doesn't match -- */
        if (!Map_canEquip_Range(map, agg_ent, dfts,
                                range_can_equip)) {
            /* SDL_Log("!Map_canEquip_Range"); */
            continue;
        }

        equippable.arr[equippable.num++] = eq;
    }

    DARR_FREE(dfts);

    return (equippable);
}

/* Find if a current equipment has enemy in range */
b32 Map_canEquip_Range(Map              *map,
                       tnecs_E           unit_ent,
                       tnecs_E          *defendants,
                       struct canEquip   can_equip) {
    /* NOTE: assumes Es are tracked on unitmap */
    SDL_assert(map                      != NULL);
    SDL_assert(defendants               != NULL);
    SDL_assert(gl_world                 != NULL);
    SDL_assert(map->darrs.movemap       != NULL);
    SDL_assert(map->darrs.unitmap       != NULL);
    SDL_assert(map->darrs.attacktomap   != NULL);

    Unit *unit = IES_GET_C(gl_world, unit_ent, Unit);

    /* Compute range */
    struct Range range = Range_default;
    Unit_Range_Eq(  unit,                    can_equip._eq,
                    can_equip.archetype,    &range);

    /* --- Compute list of units in range --- */
    MapAct map_to       = MapAct_default;

    map_to.move         = can_equip.move;
    map_to.archetype    = can_equip.archetype;
    map_to.eq_type      = can_equip.eq_type;
    map_to._eq          = can_equip._eq;
    map_to.output_type  = ARRAY_LIST;
    map_to.aggressor    = unit_ent;

    /* printf("MOVE BEFORE Map_Act_To\n"); */
    /* matrix_print(map->darrs.movemap, Map_row_len(map), Map_col_len(map)); */

    Map_Act_To(map, map_to);

    /* printf("MOVE AFTER Map_Act_To\n"); */
    /* matrix_print(map->darrs.movemap, Map_row_len(map), Map_col_len(map)); */
    /* printf("ATK\n"); */
    /* matrix_print(map->darrs.attacktomap, Map_row_len(map), Map_col_len(map)); */
    /* printf("HEAL\n"); */
    /* matrix_print(map->darrs.healtomap, Map_row_len(map), Map_col_len(map)); */
    /* printf("UNIT\n"); */
    /* entity_print(map->darrs.unitmap, Map_row_len(map), Map_col_len(map)); */

    /* Find all Defendants/Patients in list */
    MapFind mapfind     = MapFind_default;

    mapfind.found       = defendants;
    mapfind.seeker      = unit_ent;
    mapfind.fastquit    = true;
    mapfind.eq_type     = can_equip.eq_type;
    // Note _eq used for LOADOUT_IEQ only
    mapfind._eq         = can_equip._eq;

    if (can_equip.archetype == ITEM_ARCHETYPE_WEAPON) {
        mapfind.list        = map->darrs.attacktolist;
        defendants          = Map_Find_Defendants(map, mapfind);
    } else if (can_equip.archetype == ITEM_ARCHETYPE_STAFF) {
        /* --- Compute healtolist --- */
        mapfind.list        = map->darrs.healtolist;
        defendants          = Map_Find_Patients(map, mapfind);
    }

    b32 iscan_equip = (DARR_NUM(defendants) > 0);

    DARR_NUM(defendants) = 0;
    return (iscan_equip);
}

tnecs_E *Map_Find_Defendants(  struct Map *map,
                               MapFind mapfind) {
    /* Find all defendants on attacktolist according to alignment */
    i32             *attacktolist   = mapfind.list;
    tnecs_E    *defendants     = mapfind.found;
    tnecs_E     aggressor      = mapfind.seeker;
    // b32              fastquit       = mapfind.fastquit;

    /* Note: attacktolist should have been created with same eq_type and _eq before */
    SDL_assert(aggressor > TNECS_NULL);
    SDL_assert(defendants   != NULL);
    SDL_assert(attacktolist != NULL);
    for (size_t i = 0; i < DARR_NUM(attacktolist) / TWO_D; i++) {
        size_t x_at = attacktolist[TWO_D * i];
        size_t y_at = attacktolist[TWO_D * i + 1];

        /* - Checking for units on x_at, y_at - */
        size_t index = y_at * Map_col_len(map) + x_at;
        tnecs_E unitontile = map->darrs.unitmap[index];

        // TODO: Make this an assert?
        if (unitontile <= TNECS_NULL) {
            // SDL_Log("No unit on tile");
            continue;
        }

        /* - Found unit, checking alignment - */
        // SDL_Log("Found unit on %lu %lu ", x_at, y_at);
        struct Unit *agg    = IES_GET_C(gl_world, aggressor, Unit);
        struct Unit *unit   = IES_GET_C(gl_world, unitontile, Unit);
        u8 align_t          = SotA_army2alignment(Unit_Army(unit));
        u8 align_a          = SotA_army2alignment(Unit_Army(agg));

        if (align_a == align_t) {
            // SDL_Log("Same alignment");
            continue;
        }
        // SDL_Log("Adding");
        DARR_PUT(defendants, unitontile);
    }
    return (defendants);
}

tnecs_E *Map_Find_Breakables(struct Map *map, i32 *attacktolist,
                             tnecs_E *attackable, b32 fastquit) {
    /* Find all breakables on attacktolist */

    SDL_assert(attackable != NULL);
    SDL_assert(attacktolist != NULL);
    for (size_t i = 0; i < DARR_NUM(attacktolist) / TWO_D; i++) {
        size_t x_at = attacktolist[TWO_D * i];
        size_t y_at = attacktolist[TWO_D * i + 1];
        /* - Checking for breakable on x_at, y_at - */
        for (size_t j = 0; j < DARR_NUM(map->Es.breakables); j++) {
            struct Position *pos;
            pos = IES_GET_C(gl_world, map->Es.breakables[j], Position);
            SDL_assert(pos != NULL);
            size_t x_br = pos->tilemap_pos.x;
            size_t y_br = pos->tilemap_pos.y;
            /* - breakable on x_br, y_br - */
            if ((x_at == x_br) && (y_at == y_br))
                DARR_PUT(attackable, map->Es.breakables[i]);
        }
    }
    return (attackable);
}

tnecs_E *Map_Find_Patients( Map     *map,
                            MapFind  mapfind) {
    /* Find patients on healtolist with alignment
    **  1. Does not check range.
    **          Range used in Map_Act_to for healtolist
    **  2. attacktolist should have been created with
    **          same eq_type and _eq before
    **  3. Can only find patients for usable items.
    */
    // DESIGN:
    //  1. Should patients be unique?
    //  2. Should item usable on patient also be output? Nah
    // How does weapon select menu item finds usable weapons?

    i32     *healtolist = mapfind.list;
    tnecs_E *patients   = mapfind.found;
    tnecs_E  healer_E   = mapfind.seeker;
    // b32 fastquit                = mapfind.fastquit;

    Unit *healer = IES_GET_C(gl_world, healer_E, Unit);
    SDL_assert(healer          != NULL);
    SDL_assert(gl_weapons_dtab != NULL);

    for (i32 eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; eq++) {
        /* SDL_Log("Map_Find_Patients eq %d", eq); */

        /* -- Skip if eq doesn't match -- */
        if ((mapfind.eq_type == LOADOUT_EQ) &&
            (eq != mapfind._eq)) {
            continue;
        }

        /* -- Skip if its not a valid item -- */
        i32 id = Unit_Id_Equipment(healer, eq);
        if (id <= ITEM_NULL) {
            continue;
        }

        /* -- Skip if its not a staff, or a usable item -- */
        i32 is_staff    = Staff_ID_isValid(id);
        i32 is_usable   = Unit_canUse_ItemID(healer, id);

        if (!is_staff && !is_usable) {
            continue;
        }

        /* -- Check healtolist for valid patients for item -- */
        Item *item      = _Item_Get(id);
        mapfind.item    = item;
        _Map_Find_Patient(map, mapfind);
    }
    return (patients);
}

tnecs_E _Map_Find_Item_Patients(Map     *map,
                                MapFind  mapfind) {
    i32     *healtolist = mapfind.list;
    tnecs_E *patients   = mapfind.found;
    Item    *item       = mapfind.item;
    i32      col_len    = Map_col_len(map);

    for (size_t i = 0; i < DARR_NUM(healtolist) / 2; i++) {
        size_t x_at = healtolist[TWO_D * i];
        size_t y_at = healtolist[TWO_D * i + 1];
        size_t ind  = sota_2D_index(x_at, y_at, col_len);
        tnecs_E patient_E = map->darrs.unitmap[ind];

        /* -- Skip if no patient on tile -- */
        if (patient_E <= TNECS_NULL) {
            // SDL_Log("No patient on tile");
            continue;
        }

        Unit *patient = IES_GET_C(gl_world, patient_E, Unit);

        /* -- Skip if no item target does not match -- */
        b32 align_match = Unit_Target_Match(healer,
                                            patient,
                                            item->ids.target);

        if (!align_match) {
            // SDL_Log("Target does not have correct alignment");
            continue;
        }

        /* -- Considering Full game state, canUse? -- */
        i32 canUse_Full = item->flags.canUse_Full;
        item_CanUse_full_t canUse_f = Item_CanUse_Func(canUse_Full);

        if ((canUse_f != NULL) &&
            (!canUse_f(NULL, healer, patient, item))) {
            // SDL_Log("Item Full CanUse critera not fulfilled");
            continue;
        }

        DARR_PUT(patients, patient);
    }
}


tnecs_E Map_Find_Breakable_Ent(struct Map *map, i32 x, i32 y) {
    SDL_assert(map      != NULL);
    SDL_assert(gl_world != NULL);
    tnecs_E out = TNECS_NULL;
    for (size_t i = 0; i < DARR_NUM(map->Es.breakables); i++) {
        struct Position *pos;
        pos = IES_GET_C(gl_world, map->Es.breakables[i], Position);
        SDL_assert(pos != NULL);
        if ((x == pos->tilemap_pos.x) && (y == pos->tilemap_pos.y)) {
            out = map->Es.breakables[i];
            break;
        }
    }
    return (out);
}

tnecs_E Map_Find_Door_Ent(struct Map *map, i32 x, i32 y) {
    SDL_assert(map          != NULL);
    SDL_assert(gl_world   != NULL);
    tnecs_E out = TNECS_NULL;
    for (size_t i = 0; i < DARR_NUM(map->Es.doors); i++) {
        struct Position *pos = IES_GET_C(gl_world, map->Es.doors[i], Position);
        SDL_assert(pos != NULL);
        if ((x == pos->tilemap_pos.x) && (y == pos->tilemap_pos.y)) {
            out = map->Es.doors[i];
            break;
        }
    }
    return (out);
}

tnecs_E Map_Find_Chest_Ent(struct Map *map, i32 x, i32 y) {
    SDL_assert(map          != NULL);
    SDL_assert(gl_world   != NULL);
    tnecs_E out = TNECS_NULL;
    for (size_t i = 0; i < DARR_NUM(map->Es.chests); i++) {
        struct Position *pos = IES_GET_C(gl_world, map->Es.chests[i], Position);
        SDL_assert(pos != NULL);
        if ((x == pos->tilemap_pos.x) && (y == pos->tilemap_pos.y)) {
            out = map->Es.chests[i];
            break;
        }
    }
    return (out);
}

tnecs_E *Map_Find_Spectators(struct Map *map, tnecs_E *spectators, i32 x, i32 y) {

    /* -- Find spectator on neighbour tiles (to dance) -- */
    for (i32 i = 0; i < SQUARE_NEIGHBOURS; i++) {
        int x_at = int_inbounds(x + q_cycle4_mzpz(i), 0, Map_col_len(map));
        int y_at = int_inbounds(y + q_cycle4_zmzp(i), 0, Map_row_len(map));

        /* Skip if tile is same as current tile */
        if ((x_at == x) && (y_at == y))
            continue;

        tnecs_E spectator = map->darrs.unitmap[y_at * Map_col_len(map) + x_at];
        if (spectator == TNECS_NULL)
            continue;

        struct Unit *unit = IES_GET_C(gl_world, spectator, Unit);
        SDL_assert(unit);
        if (Unit_isWaiting(unit))
            DARR_PUT(spectators, spectator);
    }
    return (spectators);
}

tnecs_E *Map_Find_Auditors(struct Map *map, tnecs_E *auditors, i32 x, i32 y) {
    /* -- Find auditors on neighbour tiles (to speak) -- */

    for (i32 i = 0; i < SQUARE_NEIGHBOURS; i++) {
        int x_at = int_inbounds(x + q_cycle4_mzpz(i), 0, Map_col_len(map));
        int y_at = int_inbounds(y + q_cycle4_zmzp(i), 0, Map_row_len(map));

        /* Skip if tile is same as current tile */
        if ((x_at == x) && (y_at == y))
            continue;

        tnecs_E auditor_ent = map->darrs.unitmap[y_at * Map_col_len(map) + x_at];
        if (auditor_ent <= TNECS_NULL)
            continue;

        struct Unit *unit = IES_GET_C(gl_world, auditor_ent, Unit);
        SDL_assert(unit);
        if (Unit_isTalkable(unit))
            DARR_PUT(auditors, auditor_ent);
    }
    return (auditors);
}

tnecs_E *Map_Find_Traders(struct Map *map, tnecs_E *passives, i32 x, i32 y) {
    /* -- Find traders on neighbours (to trade with) -- */
    for (i32 i = 0; i < SQUARE_NEIGHBOURS; i++) {
        int x_at = int_inbounds(x + q_cycle4_mzpz(i), 0, Map_col_len(map));
        int y_at = int_inbounds(y + q_cycle4_zmzp(i), 0, Map_row_len(map));

        /* Skip if tile is same as current tile */
        if ((x_at == x) && (y_at == y))
            continue;

        tnecs_E passive = map->darrs.unitmap[y_at * Map_col_len(map) + x_at];
        if (passive <= TNECS_NULL)
            continue;

        struct Unit *unit = IES_GET_C(gl_world, passive, Unit);
        if (SotA_isPC(Unit_Army(unit)))
            DARR_PUT(passives, passive);
    }
    return (passives);
}

tnecs_E *Map_Find_Victims(struct Map *map, tnecs_E *victims_ent,
                          i32 x, i32 y, tnecs_E savior_ent) {
    /* Find victims on neighbours (to rescue) */
    // TODO: map edges!

    for (i32 i = 0; i < SQUARE_NEIGHBOURS; i++) {
        int x_at = int_inbounds(x + q_cycle4_mzpz(i), 0, Map_col_len(map));
        int y_at = int_inbounds(y + q_cycle4_zmzp(i), 0, Map_row_len(map));

        /* Skip if tile is same as current tile */
        if ((x_at == x) && (y_at == y))
            continue;

        tnecs_E victim_ent = map->darrs.unitmap[y_at * Map_col_len(map) + x_at];
        if (victim_ent <= TNECS_NULL)
            continue;

        struct Unit *victim = IES_GET_C(gl_world, victim_ent, Unit);
        struct Unit *savior = IES_GET_C(gl_world, savior_ent, Unit);
        if (Unit_canCarry(savior, victim) && SotA_isPC(Unit_Army(victim)))
            DARR_PUT(victims_ent, victim_ent);
    }
    return (victims_ent);
}

tnecs_E *Map_Find_Doors(struct Map *map, tnecs_E *openable, i32 x, i32 y) {
    /* -- Check if unit is next to a door -- */
    for (size_t i = 0; i < DARR_NUM(map->Es.doors); i++) {
        struct Position *pos = IES_GET_C(gl_world, map->Es.doors[i], Position);
        size_t x_at          = pos->tilemap_pos.x;
        size_t y_at          = pos->tilemap_pos.y;
        b32 door = (((x + 1) == x_at)    && (y == y_at));
        door     |= (((x - 1) == x_at)    && (y == y_at));
        door     |= (((y + 1) == y_at)    && (x == x_at));
        door     |= (((y - 1) == y_at)    && (x == x_at));

        if (door)
            DARR_PUT(openable, map->Es.doors[i]);
        // TODO: check if key in inventory to put in openable
    }
    return (openable);
}

tnecs_E *Map_Find_Chests(struct Map *map, tnecs_E *openable, i32 x, i32 y) {
    // Find Chests on current position and neighbours
    for (size_t i = 0; i < DARR_NUM(map->Es.chests); i++) {
        struct Position *pos = IES_GET_C(gl_world, map->Es.chests[i], Position);
        size_t x_at          = pos->tilemap_pos.x;
        size_t y_at          = pos->tilemap_pos.y;
        b32 chest = (((x + 1) == x_at)    && (y == y_at));
        chest     |= (((x - 1) == x_at)    && (y == y_at));
        chest     |= (((y + 1) == y_at)    && (x == x_at));
        chest     |= (((y - 1) == y_at)    && (x == x_at));

        if (chest)
            DARR_PUT(openable, map->Es.chests[i]);
        // TODO: check if key in inventory to put in openable
    }
    return (openable);
}

/* - Closest - */
tnecs_E Map_Find_Enemy_Closest(struct Map *map, i32 x, i32 y) {
    int num             = DARR_NUM(map->units.onfield.enemies);
    i32 dist, min_dist  = INT32_MAX;
    tnecs_E out    = TNECS_NULL;

    for (size_t i = 0; i < num; i++) {
        tnecs_E enemy = map->units.onfield.enemies[i];
        SDL_assert(enemy != TNECS_NULL);
        struct Position *pos = IES_GET_C(gl_world, enemy, Position);
        SDL_assert(pos != NULL);
        dist = _Pathfinding_Manhattan(x, y, pos->tilemap_pos.x, pos->tilemap_pos.y);
        if (dist < min_dist) {
            min_dist    = dist;
            out         = enemy;
        }
    }
    return (out);
}

tnecs_E Map_Find_Friendly_Closest(struct Map *map, i32 x, i32 y) {
    int num             = DARR_NUM(map->units.onfield.friendlies);
    i32 dist, min_dist  = INT32_MAX;
    tnecs_E out    = TNECS_NULL;

    for (size_t i = 0; i < num; i++) {
        tnecs_E friendly = map->units.onfield.friendlies[i];
        SDL_assert(friendly != TNECS_NULL);
        struct Position *pos = IES_GET_C(gl_world, friendly, Position);
        SDL_assert(pos != NULL);
        dist = _Pathfinding_Manhattan(x, y, pos->tilemap_pos.x, pos->tilemap_pos.y);
        if (dist < min_dist) {
            min_dist    = dist;
            out         = friendly;
        }
    }
    return (out);
}

tnecs_E Map_Find_Unit_Closest(struct Map *map, u8 army, i32 x, i32 y) {
    tnecs_E out    = TNECS_NULL;

    return (out);
}