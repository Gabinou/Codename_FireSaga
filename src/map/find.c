
#include "map/find.h"
#include "map/map.h"
#include "map/path.h"
#include "position.h"
#include "weapon.h"
#include "pathfinding.h"
#include "utilities.h"
#include "unit/equipment.h"
#include "nmath.h"
#include "unit/unit.h"
#include "unit/loadout.h"
#include "unit/equipment.h"
#include "unit/anim.h"
#include "unit/status.h"
#include "unit/boss.h"
#include "unit/flags.h"
#include "unit/range.h"
#include "unit/stats.h"
#include "unit/mount.h"

/*-- Map Usable -- */

/* Find if a weapon/staff usable by unit has an enemy in range */
void Map_canEquip(struct Map *map, tnecs_entity unit_ent, canEquip can_equip) {
    SDL_assert(map          != NULL);
    SDL_assert(map->world   != NULL);
    SDL_assert((can_equip.archetype == ITEM_ARCHETYPE_WEAPON) ||
               (can_equip.archetype == ITEM_ARCHETYPE_STAFF));

    Unit     *unit = IES_GET_COMPONENT(map->world, unit_ent, Unit);
    SDL_assert(unit != NULL);
    Position *pos  = IES_GET_COMPONENT(map->world, unit_ent, Position);
    SDL_assert(pos != NULL);
    _Map_Costmap_Movement_Compute(map, unit);

    /* Compute movemap */
    struct Point start = pos->tilemap_pos;
    Unit_stats eff_s = Unit_effectiveStats(unit);
    i32 move_stat       = can_equip.move ? eff_s.move : 0;
    i32 effective_move  = move_stat * map->cost_multiplier; // TODO make utility
    _Map_Movemap_Compute(map, start, effective_move);

    // printf("MOVE\n");
    // matrix_print(map->movemap, map->row_len, map->col_len);

    /* Alloc defendants */
    tnecs_entity *defendants  = DARR_INIT(defendants, tnecs_entity, 4);

    /* Make canEquip for Map_canEquip_Range */
    canEquip range_can_equip    = can_equip;
    range_can_equip.eq_type     = LOADOUT_EQ;

    unit->can_equip.num  = 0;
    for (int eq = ITEM1; eq < SOTA_EQUIPMENT_SIZE; eq++) {
        /* Skip if weapon is not usable */

        // SDL_Log("Map_canEquip eq %d \n", eq);
        canEquip_Eq(&can_equip, eq);

        /* -- Skip if eq doesn't match -- */
        if ((can_equip.eq_type == LOADOUT_EQ) && (eq != can_equip._eq)) {
            continue;
        }

        if (!Unit_canEquip_AnyHand(unit, can_equip)) {
            // SDL_Log("!Unit_canEquip_AnyHand");
            continue;
        }

        range_can_equip._eq         = eq;

        if (!Map_canEquip_Range(map, unit_ent, defendants, range_can_equip)) {
            // SDL_Log("!Map_canEquip_Range");
            continue;
        }

        unit->can_equip.arr[unit->can_equip.num++] = eq;
    }

    DARR_FREE(defendants);
}

b32 Map_canEquip_Range(struct Map *map, tnecs_entity unit_ent,
                       tnecs_entity *defendants, struct canEquip can_equip) {
    /* NOTES:
        2- assumes entities are tracked on unitmap
    */
    SDL_assert(map              != NULL);
    SDL_assert(defendants       != NULL);
    SDL_assert(map->world       != NULL);
    SDL_assert(map->movemap     != NULL);
    SDL_assert(map->unitmap     != NULL);
    SDL_assert(map->attacktomap != NULL);

    Unit     *unit = IES_GET_COMPONENT(map->world, unit_ent, Unit);

    /* Compute range */
    struct Range range = Range_default;
    Unit_Range_Eq(unit, can_equip._eq, can_equip.archetype, &range);
    // SDL_Log("range %d %d", range->min, range->max);

    /* Compute attacktolist to check if any enemy in it */
    /* --- Compute list (attacktolist, healtolist) of unit in range --- */
    MapAct map_to       = MapAct_default;

    map_to.move         = can_equip.move;
    map_to.archetype    = can_equip.archetype;
    map_to.eq_type      = can_equip.eq_type;
    map_to._eq          = can_equip._eq;
    map_to.output_type  = ARRAY_LIST;
    map_to.aggressor    = unit_ent;

    Map_Act_To(map, map_to);

    // printf("MOVE\n");
    // matrix_print(map->movemap, map->row_len, map->col_len);
    // printf("ATK\n");
    // matrix_print(map->attacktomap, map->row_len, map->col_len);

    /* Find all Defendants/Patients in list */
    MapFind mapfind     = MapFind_default;

    mapfind.found       = defendants;
    mapfind.seeker      = unit_ent;
    mapfind.fastquit    = true;
    mapfind.eq_type     = can_equip.eq_type;
    // Note _eq used for LOADOUT_IEQ only
    mapfind._eq         = can_equip._eq;

    if (can_equip.archetype == ITEM_ARCHETYPE_WEAPON) {
        mapfind.list        = map->attacktolist;
        defendants          = Map_Find_Defendants(map, mapfind);
    } else if (can_equip.archetype == ITEM_ARCHETYPE_STAFF) {
        /* --- Compute healtolist --- */
        mapfind.list        = map->healtolist;
        defendants          = Map_Find_Patients(map, mapfind);
    }

    // printf("DARR_NUM(defendants) %d\n", DARR_NUM(defendants));
    b32 iscan_equip = (DARR_NUM(defendants) > 0);

    DARR_NUM(defendants) = 0;
    return (iscan_equip);
}


tnecs_entity *Map_Find_Defendants(struct Map *map, MapFind mapfind) {
    /* Find all defendants on attacktolist according to alignment */
    i32             *attacktolist   = mapfind.list;
    tnecs_entity    *defendants     = mapfind.found;
    tnecs_entity     aggressor      = mapfind.seeker;
    b32              fastquit       = mapfind.fastquit;

    /* Note: attacktolist should have been created with same eq_type and _eq before */
    SDL_assert(aggressor > TNECS_NULL);
    SDL_assert(defendants   != NULL);
    SDL_assert(attacktolist != NULL);
    for (size_t i = 0; i < DARR_NUM(attacktolist) / TWO_D; i++) {
        size_t x_at = attacktolist[TWO_D * i];
        size_t y_at = attacktolist[TWO_D * i + 1];

        /* - Checking for units on x_at, y_at - */
        size_t index = y_at * map->col_len + x_at;
        tnecs_entity unitontile = map->unitmap[index];

        // TODO: Make this an assert?
        if (unitontile <= TNECS_NULL) {
            // SDL_Log("No unit on tile");
            continue;
        }

        /* - Found unit, checking alignment - */
        // SDL_Log("Found unit on %lu %lu ", x_at, y_at);
        struct Unit *agg    = IES_GET_COMPONENT(map->world, aggressor, Unit);
        struct Unit *unit   = IES_GET_COMPONENT(map->world, unitontile, Unit);
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

tnecs_entity *Map_Find_Breakables(struct Map *map, i32 *attacktolist,
                                  tnecs_entity *attackable, b32 fastquit) {
    /* Find all breakables on attacktolist */

    SDL_assert(attackable != NULL);
    SDL_assert(attacktolist != NULL);
    for (size_t i = 0; i < DARR_NUM(attacktolist) / TWO_D; i++) {
        size_t x_at = attacktolist[TWO_D * i];
        size_t y_at = attacktolist[TWO_D * i + 1];
        /* - Checking for breakable on x_at, y_at - */
        for (size_t j = 0; j < DARR_NUM(map->breakables_ent); j++) {
            struct Position *pos;
            pos = IES_GET_COMPONENT(map->world, map->breakables_ent[j], Position);
            SDL_assert(pos != NULL);
            size_t x_br = pos->tilemap_pos.x;
            size_t y_br = pos->tilemap_pos.y;
            /* - breakable on x_br, y_br - */
            if ((x_at == x_br) && (y_at == y_br))
                DARR_PUT(attackable, map->breakables_ent[i]);
        }
    }
    return (attackable);
}

tnecs_entity *Map_Find_Patients(struct Map *map, MapFind mapfind) {
    /* Find all patients on healtolist according to alignment, staff */
    /* NOTE: Does not check range */
    i32 *healtolist             = mapfind.list;
    tnecs_entity *patients      = mapfind.found;
    tnecs_entity healer_ent     = mapfind.seeker;
    b32 fastquit                = mapfind.fastquit;

    /* Find all patients on healtolist according to alignment */
    /* Note: attacktolist should have been created with same eq_type and _eq before */
    struct Unit *healer = IES_GET_COMPONENT(map->world, healer_ent, Unit);
    SDL_assert(healer               != NULL);
    struct dtab *weapons_dtab = Unit_dtab_Weapons(healer);
    SDL_assert(weapons_dtab != NULL);

    /* TODO: full health people arent patients FOR HEALING STAVES */
    for (i32 eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; eq++) {
        // SDL_Log("Map_Find_Patients eq %d", eq);

        /* -- Skip if eq doesn't match -- */
        if ((mapfind.eq_type == LOADOUT_EQ) && (eq != mapfind._eq)) {
            continue;
        }

        /* -- Getting staff -- */
        i32 id = Unit_Id_Equipment(healer, eq);
        // SDL_Log("id %d", id);
        /* Skip if its not a valid item */

        if (id <= ITEM_NULL) {
            continue;
        }

        /* Skip if its not a staff */
        if (!Weapon_isStaff(id)) {
            continue;
        }

        Weapon_Load(weapons_dtab, id);
        struct Weapon *staff = (struct Weapon *)DTAB_GET(weapons_dtab, id);

        /* -- Check healtolist for valid patients -- */
        u8 align_healer = army_alignment[Unit_Army(healer)];
        for (size_t i = 0; i < DARR_NUM(healtolist) / 2; i++) {
            size_t x_at = healtolist[TWO_D * i];
            size_t y_at = healtolist[TWO_D * i + 1];
            tnecs_entity unitontile = map->unitmap[y_at * map->col_len + x_at];

            /* Note: No need to check range.
                     Should have been included in Map_Act_to call
                     that produced the healtolist */
            /* Skip if no unit on tile */
            if (unitontile <= TNECS_NULL) {
                continue;
            }

            /* Skip if self */
            /* DESIGN QUESTION: Can healers heal themselves? */
            if (healer_ent == unitontile) {
                continue;
            }

            Unit *patient = IES_GET_COMPONENT(map->world, unitontile, Unit);
            Unit_stats p_eff_stats = Unit_effectiveStats(patient);

            u8 align_patient = army_alignment[Unit_Army(patient)];
            b32 add = false;
            /* Staff patient alignment check */
            switch (staff->item->target) {
                case ITEM_NO_TARGET:
                    add = true;
                    break;
                case ITEM_TARGET_FRIENDLY:
                    i32 current_hp = Unit_Current_HP(patient);
                    add = (align_patient == align_healer) && (current_hp < p_eff_stats.hp);
                    break;
                case ITEM_TARGET_ENEMY:
                    add = align_patient |= align_healer;
                    break;
            }

            if (add)
                DARR_PUT(patients, unitontile);
        }
    }
    return (patients);
}

tnecs_entity Map_Find_Breakable_Ent(struct Map *map, i32 x, i32 y) {
    SDL_assert(map          != NULL);
    SDL_assert(map->world   != NULL);
    tnecs_entity out = TNECS_NULL;
    for (size_t i = 0; i < DARR_NUM(map->breakables_ent); i++) {
        struct Position *pos;
        pos = IES_GET_COMPONENT(map->world, map->breakables_ent[i], Position);
        SDL_assert(pos != NULL);
        if ((x == pos->tilemap_pos.x) && (y == pos->tilemap_pos.y)) {
            out = map->breakables_ent[i];
            break;
        }
    }
    return (out);
}

tnecs_entity Map_Find_Door_Ent(struct Map *map, i32 x, i32 y) {
    SDL_assert(map          != NULL);
    SDL_assert(map->world   != NULL);
    tnecs_entity out = TNECS_NULL;
    for (size_t i = 0; i < DARR_NUM(map->doors_ent); i++) {
        struct Position *pos = IES_GET_COMPONENT(map->world, map->doors_ent[i], Position);
        SDL_assert(pos != NULL);
        if ((x == pos->tilemap_pos.x) && (y == pos->tilemap_pos.y)) {
            out = map->doors_ent[i];
            break;
        }
    }
    return (out);
}

tnecs_entity Map_Find_Chest_Ent(struct Map *map, i32 x, i32 y) {
    SDL_assert(map          != NULL);
    SDL_assert(map->world   != NULL);
    tnecs_entity out = TNECS_NULL;
    for (size_t i = 0; i < DARR_NUM(map->chests_ent); i++) {
        struct Position *pos = IES_GET_COMPONENT(map->world, map->chests_ent[i], Position);
        SDL_assert(pos != NULL);
        if ((x == pos->tilemap_pos.x) && (y == pos->tilemap_pos.y)) {
            out = map->chests_ent[i];
            break;
        }
    }
    return (out);
}

tnecs_entity *Map_Find_Spectators(struct Map *map, tnecs_entity *spectators, i32 x, i32 y) {

    /* -- Find spectator on neighbour tiles (to dance) -- */
    for (i32 i = 0; i < SQUARE_NEIGHBOURS; i++) {
        int x_at = int_inbounds(x + q_cycle4_mzpz(i), 0, map->col_len);
        int y_at = int_inbounds(y + q_cycle4_zmzp(i), 0, map->row_len);

        /* Skip if tile is same as current tile */
        if ((x_at == x) && (y_at == y))
            continue;

        tnecs_entity spectator = map->unitmap[y_at * map->col_len + x_at];
        if (spectator == TNECS_NULL)
            continue;

        struct Unit *unit = IES_GET_COMPONENT(map->world, spectator, Unit);
        SDL_assert(unit);
        if (Unit_isWaiting(unit))
            DARR_PUT(spectators, spectator);
    }
    return (spectators);
}

tnecs_entity *Map_Find_Auditors(struct Map *map, tnecs_entity *auditors, i32 x, i32 y) {
    /* -- Find auditors on neighbour tiles (to speak) -- */

    for (i32 i = 0; i < SQUARE_NEIGHBOURS; i++) {
        int x_at = int_inbounds(x + q_cycle4_mzpz(i), 0, map->col_len);
        int y_at = int_inbounds(y + q_cycle4_zmzp(i), 0, map->row_len);

        /* Skip if tile is same as current tile */
        if ((x_at == x) && (y_at == y))
            continue;

        tnecs_entity auditor_ent = map->unitmap[y_at * map->col_len + x_at];
        if (auditor_ent <= TNECS_NULL)
            continue;

        struct Unit *unit = IES_GET_COMPONENT(map->world, auditor_ent, Unit);
        SDL_assert(unit);
        if (unit->flags.talkable)
            DARR_PUT(auditors, auditor_ent);
    }
    return (auditors);
}

tnecs_entity *Map_Find_Traders(struct Map *map, tnecs_entity *passives, i32 x, i32 y) {
    /* -- Find traders on neighbours (to trade with) -- */
    for (i32 i = 0; i < SQUARE_NEIGHBOURS; i++) {
        int x_at = int_inbounds(x + q_cycle4_mzpz(i), 0, map->col_len);
        int y_at = int_inbounds(y + q_cycle4_zmzp(i), 0, map->row_len);

        /* Skip if tile is same as current tile */
        if ((x_at == x) && (y_at == y))
            continue;

        tnecs_entity passive = map->unitmap[y_at * map->col_len + x_at];
        if (passive <= TNECS_NULL)
            continue;

        struct Unit *unit = IES_GET_COMPONENT(map->world, passive, Unit);
        if (SotA_isPC(Unit_Army(unit)))
            DARR_PUT(passives, passive);
    }
    return (passives);
}

tnecs_entity *Map_Find_Victims(struct Map *map, tnecs_entity *victims_ent,
                               i32 x, i32 y, tnecs_entity savior_ent) {
    /* Find victims on neighbours (to rescue) */
    // TODO: map edges!

    for (i32 i = 0; i < SQUARE_NEIGHBOURS; i++) {
        int x_at = int_inbounds(x + q_cycle4_mzpz(i), 0, map->col_len);
        int y_at = int_inbounds(y + q_cycle4_zmzp(i), 0, map->row_len);

        /* Skip if tile is same as current tile */
        if ((x_at == x) && (y_at == y))
            continue;

        tnecs_entity victim_ent = map->unitmap[y_at * map->col_len + x_at];
        if (victim_ent <= TNECS_NULL)
            continue;

        struct Unit *victim = IES_GET_COMPONENT(map->world, victim_ent, Unit);
        struct Unit *savior = IES_GET_COMPONENT(map->world, savior_ent, Unit);
        if (Unit_canCarry(savior, victim) && SotA_isPC(Unit_Army(victim)))
            DARR_PUT(victims_ent, victim_ent);
    }
    return (victims_ent);
}

tnecs_entity *Map_Find_Doors(struct Map *map, tnecs_entity *openable, i32 x, i32 y) {
    /* -- Check if unit is next to a door -- */
    for (size_t i = 0; i < DARR_NUM(map->doors_ent); i++) {
        struct Position *pos = IES_GET_COMPONENT(map->world, map->doors_ent[i], Position);
        size_t x_at          = pos->tilemap_pos.x;
        size_t y_at          = pos->tilemap_pos.y;
        b32 door = (((x + 1) == x_at)    && (y == y_at));
        door     |= (((x - 1) == x_at)    && (y == y_at));
        door     |= (((y + 1) == y_at)    && (x == x_at));
        door     |= (((y - 1) == y_at)    && (x == x_at));

        if (door)
            DARR_PUT(openable, map->doors_ent[i]);
        // TODO: check if key in inventory to put in openable
    }
    return (openable);
}

tnecs_entity *Map_Find_Chests(struct Map *map, tnecs_entity *openable, i32 x, i32 y) {
    // Find Chests on current position and neighbours
    for (size_t i = 0; i < DARR_NUM(map->chests_ent); i++) {
        struct Position *pos = IES_GET_COMPONENT(map->world, map->chests_ent[i], Position);
        size_t x_at          = pos->tilemap_pos.x;
        size_t y_at          = pos->tilemap_pos.y;
        b32 chest = (((x + 1) == x_at)    && (y == y_at));
        chest     |= (((x - 1) == x_at)    && (y == y_at));
        chest     |= (((y + 1) == y_at)    && (x == x_at));
        chest     |= (((y - 1) == y_at)    && (x == x_at));

        if (chest)
            DARR_PUT(openable, map->chests_ent[i]);
        // TODO: check if key in inventory to put in openable
    }
    return (openable);
}

/* - Closest - */
tnecs_entity Map_Find_Enemy_Closest(struct Map *map, i32 x, i32 y) {
    int num             = DARR_NUM(map->enemies_onfield);
    i32 dist, min_dist  = INT32_MAX;
    tnecs_entity out    = TNECS_NULL;

    for (size_t i = 0; i < num; i++) {
        tnecs_entity enemy = map->enemies_onfield[i];
        SDL_assert(enemy != TNECS_NULL);
        struct Position *pos = IES_GET_COMPONENT(map->world, enemy, Position);
        SDL_assert(pos != NULL);
        dist = _Pathfinding_Manhattan(x, y, pos->tilemap_pos.x, pos->tilemap_pos.y);
        if (dist < min_dist) {
            min_dist    = dist;
            out         = enemy;
        }
    }
    return (out);
}

tnecs_entity Map_Find_Friendly_Closest(struct Map *map, i32 x, i32 y) {
    int num             = DARR_NUM(map->friendlies_onfield);
    i32 dist, min_dist  = INT32_MAX;
    tnecs_entity out    = TNECS_NULL;

    for (size_t i = 0; i < num; i++) {
        tnecs_entity friendly = map->friendlies_onfield[i];
        SDL_assert(friendly != TNECS_NULL);
        struct Position *pos = IES_GET_COMPONENT(map->world, friendly, Position);
        SDL_assert(pos != NULL);
        dist = _Pathfinding_Manhattan(x, y, pos->tilemap_pos.x, pos->tilemap_pos.y);
        if (dist < min_dist) {
            min_dist    = dist;
            out         = friendly;
        }
    }
    return (out);
}

tnecs_entity Map_Find_Unit_Closest(struct Map *map, u8 army, i32 x, i32 y) {
    tnecs_entity out    = TNECS_NULL;

    return (out);
}