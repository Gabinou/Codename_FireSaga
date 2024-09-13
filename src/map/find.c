
#include "map/find.h"

/*-- Map Usable -- */

/* Find if a weapon/staff usable by unit has an enemy in range */
void Map_canEquip(struct Map *map, tnecs_entity unit_ent,
                  b32 move, int archetype) {
    SDL_assert(map          != NULL);
    SDL_assert(map->world   != NULL);
    SDL_assert((archetype == ITEM_ARCHETYPE_WEAPON) ||
               (archetype == ITEM_ARCHETYPE_STAFF));

    Unit     *unit = TNECS_GET_COMPONENT(map->world, unit_ent, Unit);
    Position *pos  = TNECS_GET_COMPONENT(map->world, unit_ent, Position);
    _Map_Costmap_Movement_Compute(map, unit);

    /* Compute movemap */
    struct Point start = pos->tilemap_pos;
    i32 move_stat = move ? Unit_getStats(unit).move : 0;
    _Map_Movemap_Compute(map, start, move_stat);

    // printf("MOVE\n");
    // matrix_print(map->movemap, map->row_len, map->col_len);

    /* Alloc defendants */
    tnecs_entity *defendants  = DARR_INIT(defendants, tnecs_entity, 4);

    unit->num_canEquip = 0;
    canEquip can_equip  = canEquip_default;
    // Map_canEquip point is to find ALL possible equippables
    can_equip.lh        = -1;
    can_equip.rh        = -1;
    can_equip.archetype = archetype;
    for (int eq = 0; eq < SOTA_EQUIPMENT_SIZE; eq++) {
        /* Skip if weapon is not usable */
        SDL_Log("eq %d \n", eq);
        can_equip.eq        = eq;

        can_equip.hand      = UNIT_HAND_LEFT;
        b32 can_equip_left  = Unit_canEquip(unit, can_equip);
        can_equip.hand      = UNIT_HAND_RIGHT;
        b32 can_equip_right = Unit_canEquip(unit, can_equip);
        if (!can_equip_right && !can_equip_right) {
            continue;
        }

        /* Compute range */
        struct Range *range = Unit_Range_Eq(unit, eq);

        /* Compute attacktolist to check if any enemy in it */
        Pathfinding_Attackto_noM(map->attacktomap, map->movemap,
                                 map->unitmap,
                                 map->row_len, map->col_len,
                                 (i32 *)range, MOVETILE_IGNORE);
        // printf("ATK\n");
        // matrix_print(map->attacktomap, map->row_len, map->col_len);

        Map_Attacktolist_Compute(map);

        /* Find all Defendants in list */
        if (archetype == ITEM_ARCHETYPE_WEAPON) {
            defendants = Map_Find_Defendants(map, map->attacktolist, defendants, unit_ent, false);
        } else if (archetype == ITEM_ARCHETYPE_STAFF) {
            defendants = Map_Find_Patients(map, map->attacktolist, defendants, unit_ent, eq, false);
        }

        printf("DARR_NUM(defendants) %d\n", DARR_NUM(defendants));
        if (DARR_NUM(defendants) > 0) {
            unit->eq_canEquip[unit->num_canEquip++] = eq;
        }
        DARR_NUM(defendants) = 0;
    }
    DARR_FREE(defendants);
}

tnecs_entity *Map_Find_Defendants(struct Map *map, i32 *attacktolist,
                                  tnecs_entity *defendants, tnecs_entity aggressor,
                                  b32 fastquit) {
    /* Find all defendants on attacktolist according to alignment */

    /* Note: Assumes attacktolist was created before with matrix2list_noM */
    SDL_assert(aggressor > TNECS_NULL);
    SDL_assert(defendants != NULL);
    SDL_assert(attacktolist);
    for (size_t i = 0; i < DARR_NUM(attacktolist) / TWO_D; i++) {
        size_t x_at = attacktolist[TWO_D * i];
        size_t y_at = attacktolist[TWO_D * i + 1];

        /* - Checking for units on x_at, y_at - */
        size_t index = y_at * map->col_len + x_at;
        tnecs_entity unitontile = map->unitmap[index];
        if (unitontile <= TNECS_NULL)
            continue;

        /* - Found unit, checking alignment - */
        // SDL_Log("Found unit on %lu %lu ", x_at, y_at);
        struct Unit *agg    = TNECS_GET_COMPONENT(map->world, aggressor, Unit);
        struct Unit *unit   = TNECS_GET_COMPONENT(map->world, unitontile, Unit);
        u8 align_t          = SotA_army2alignment(unit->army);
        u8 align_a          = SotA_army2alignment(agg->army);

        if (align_a != align_t)
            DARR_PUT(defendants, unitontile);
    }
    return (defendants);
}

tnecs_entity *Map_Find_Breakables(struct Map *map, i32 *attacktolist,
                                  tnecs_entity *attackable, b32 fastquit) {
    /* Find all breakables on attacktolist */

    // Note: Assumes attacktolist was created before with matrix2list_noM
    SDL_assert(attackable != NULL);
    SDL_assert(attacktolist != NULL);
    for (size_t i = 0; i < DARR_NUM(attacktolist) / TWO_D; i++) {
        size_t x_at = attacktolist[TWO_D * i];
        size_t y_at = attacktolist[TWO_D * i + 1];
        /* - Checking for breakable on x_at, y_at - */
        for (size_t j = 0; j < map->breakable_num; j++) {
            struct Position *pos;
            pos = TNECS_GET_COMPONENT(map->world, map->breakables_ent[j], Position);
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

tnecs_entity *Map_Find_Patients(struct Map *map, i32 *healtolist,
                                tnecs_entity *patients,
                                tnecs_entity healer_ent,
                                i32 eq, b32 fastquit) {
    /* Find all patients on healtolist according to alignment */
    /* Assumes healtolist was created before with matrix2list_noM */
    struct Unit *healer = TNECS_GET_COMPONENT(map->world, healer_ent, Unit);
    SDL_assert(healer               != NULL);
    SDL_assert(healer->weapons_dtab != NULL);

    /* TODO: full health people arent patients. */

    /* -- Getting staff -- */
    Inventory_item *item = Unit_InvItem(healer, eq);
    struct Weapon *staff = (struct Weapon *)DTAB_GET(healer->weapons_dtab, item->id);
    /* -- TODO: can only use staff in two hands -- */
    if (staff == NULL) {
        item = Unit_InvItem(healer, Unit_Hand_Weak(healer));
        SDL_assert(item != NULL);
        staff = (struct Weapon *)DTAB_GET(healer->weapons_dtab, item->id);
    }

    SDL_assert(staff        != NULL);
    SDL_assert(staff->item  != NULL);

    /* -- Check healtolist for valid patients -- */
    u8 align_healer = army_alignment[healer->army];
    for (size_t i = 0; i < DARR_NUM(healtolist) / 2; i++) {
        size_t x_at = healtolist[TWO_D * i];
        size_t y_at = healtolist[TWO_D * i + 1];
        tnecs_entity unitontile = map->unitmap[y_at * map->col_len + x_at];
        if (unitontile <= TNECS_NULL) {
            continue;
        }

        /* DESIGN QUESTION: Can healers heal themselves? */
        if (healer_ent == unitontile) {
            continue;
        }

        Unit *patient = TNECS_GET_COMPONENT(map->world, unitontile, Unit);
        Unit_stats p_eff_stats = Unit_effectiveStats(patient);

        u8 align_patient = army_alignment[patient->army];
        b32 add = false;
        switch (staff->item->target) {
            case ITEM_NO_TARGET:
                add = true;
                break;
            case ITEM_TARGET_FRIENDLY:
                add = (align_patient == align_healer) && (patient->current_hp < p_eff_stats.hp);
                break;
            case ITEM_TARGET_ENEMY:
                add = align_patient |= align_healer;
                break;
        }

        if (add)
            DARR_PUT(patients, unitontile);
    }
    return (patients);
}

tnecs_entity Map_Find_Breakable_Ent(struct Map *map, i32 x, i32 y) {
    tnecs_entity out = TNECS_NULL;
    for (size_t i = 0; i < map->breakable_num; i++) {
        struct Position *pos;
        pos = TNECS_GET_COMPONENT(map->world, map->breakables_ent[i], Position);
        if ((x == pos->tilemap_pos.x) && (y == pos->tilemap_pos.y)) {
            out = map->breakables_ent[i];
            break;
        }
    }
    return (out);
}

tnecs_entity Map_Find_Door_Ent(struct Map *map, i32 x, i32 y) {
    tnecs_entity out = TNECS_NULL;
    for (size_t i = 0; i < map->breakable_num; i++) {
        struct Position *pos = TNECS_GET_COMPONENT(map->world, map->doors_ent[i], Position);
        if ((x == pos->tilemap_pos.x) && (y == pos->tilemap_pos.y)) {
            out = map->doors_ent[i];
            break;
        }
    }
    return (out);
}

tnecs_entity Map_Find_Chest_Ent(struct Map *map, i32 x, i32 y) {
    tnecs_entity out = TNECS_NULL;
    for (size_t i = 0; i < map->breakable_num; i++) {
        struct Position *pos = TNECS_GET_COMPONENT(map->world, map->chests_ent[i], Position);
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

        struct Unit *unit = TNECS_GET_COMPONENT(map->world, spectator, Unit);
        SDL_assert(unit);
        if (unit->waits)
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

        struct Unit *unit = TNECS_GET_COMPONENT(map->world, auditor_ent, Unit);
        SDL_assert(unit);
        if (unit->talkable)
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

        struct Unit *unit = TNECS_GET_COMPONENT(map->world, passive, Unit);
        if (SotA_isPC(unit->army))
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

        struct Unit *victim = TNECS_GET_COMPONENT(map->world, victim_ent, Unit);
        struct Unit *savior = TNECS_GET_COMPONENT(map->world, savior_ent, Unit);
        if (Unit_canCarry(savior, victim) && SotA_isPC(victim->army))
            DARR_PUT(victims_ent, victim_ent);
    }
    return (victims_ent);
}

tnecs_entity *Map_Find_Doors(struct Map *map, tnecs_entity *openable, i32 x, i32 y) {
    /* -- Check if unit is next to a door -- */
    for (size_t i = 0; i < map->door_num; i++) {
        struct Position *pos = TNECS_GET_COMPONENT(map->world, map->doors_ent[i], Position);
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
    for (size_t i = 0; i < map->chest_num; i++) {
        struct Position *pos = TNECS_GET_COMPONENT(map->world, map->chests_ent[i], Position);
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
        struct Position *pos = TNECS_GET_COMPONENT(map->world, enemy, Position);
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
        struct Position *pos = TNECS_GET_COMPONENT(map->world, friendly, Position);
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