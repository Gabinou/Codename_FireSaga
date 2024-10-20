
#include "unit/range.h"
#include "enums.h"

b32 _Range_Archetype_Match(struct Weapon *wpn, i64 archetype) {
    SDL_assert(wpn != NULL);
    if (archetype == ITEM_ARCHETYPE_NULL) {
        return (false);
    }

    if (archetype == ITEM_ARCHETYPE_ITEM) {
        return (false);
    }

    if (!flagsum_isIn(wpn->item->type, archetype)) {
        return (false);
    }

    return (true);
}


struct Range *Unit_Range_Id(struct Unit *unit, int id, i64 archetype) {
    struct Range *range = &unit->computed_stats.range_equipment;
    SDL_assert(range != NULL);
    *range = Range_default;

    if (id <= ITEM_NULL) {
        // SDL_Log("ITEM_NULL");
        return (range);
    }

    if (!Weapon_ID_isValid(id)) {
        // SDL_Log("!Weapon_ID_isValid");
        return (range);
    }

    // Weapon_Load(unit->weapons_dtab, id);
    struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, id);
    SDL_assert(wpn != NULL);

    if (!_Range_Archetype_Match(wpn, archetype)) {
        // SDL_Log("!_Range_Archetype_Match");
        return (range);
    }

    Ranges_Combine(range, wpn->stats.range);

    return (range);
}

struct Range *Unit_Range_Eq(struct Unit *unit, i32 eq, i64 archetype) {
    SDL_assert(unit != NULL);
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);

    canEquip can_equip          = canEquip_default;
    can_equip.hand              = Unit_Hand_Strong(unit);
    can_equip.archetype         = archetype;
    can_equip.two_hands_mode    = TWO_HAND_EQ_MODE_LOOSE;
    canEquip_Eq(&can_equip, eq);
    canEquip_Loadout_None(&can_equip, UNIT_HAND_LEFT);
    canEquip_Loadout_None(&can_equip, UNIT_HAND_RIGHT);

    if (!Unit_canEquip_AnyHand(unit, can_equip)) {
        // SDL_Log("!Unit_canEquip_AnyHand");
        struct Range *range = &unit->computed_stats.range_equipment;
        SDL_assert(range != NULL);
        *range = Range_default;
        return (range);
    }

    return (Unit_Range_Id(unit, Unit_Id_Equipment(unit, eq), archetype));
}

/* Combines range of all weapons in equipment assuming NO LOADOUT */
// - Combined range may no reflect actual loadout range
//      - Ex: will combine range of two two-hand only weapons
struct Range *Unit_Range_Equipment(Unit *unit, i64 archetype) {
    struct Range *range = &unit->computed_stats.range_equipment;
    *range              = Range_default;

    for (int eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; eq++) {

        /* Skip if no item */
        i32 id = Unit_Id_Equipment(unit, eq);
        if (!Weapon_ID_isValid(id)) {
            // SDL_Log("!Weapon_ID_isValid\n");
            continue;
        }

        /* Skip if can't equip */
        canEquip can_equip          = canEquip_default;
        can_equip.hand              = Unit_Hand_Strong(unit);
        can_equip.archetype         = archetype;
        can_equip.two_hands_mode    = TWO_HAND_EQ_MODE_LOOSE;
        canEquip_Eq(&can_equip, eq);
        canEquip_Loadout_None(&can_equip, UNIT_HAND_LEFT);
        canEquip_Loadout_None(&can_equip, UNIT_HAND_RIGHT);

        if (!Unit_canEquip(unit, can_equip)) {
            // SDL_Log("Skip if unusable");
            continue;
        }

        /* Combine ranges */
        Weapon_Load(unit->weapons_dtab, id);
        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, id);
        SDL_assert(wpn != NULL);

        if (!_Range_Archetype_Match(wpn, archetype)) {
            // SDL_Log("!!_Range_Archetype_Match");
            return (range);
        }

        Ranges_Combine(range, wpn->stats.range);
    }
    return (range);
}

// Combine ranges of items in current loadout
// Item was previously equipped, no need to check if CAN equip
struct Range *Unit_Range_Equipped(Unit *unit, i64 archetype) {
    struct Range *range = &unit->computed_stats.range_loadout;
    *range = Range_default;

    for (int hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
        int id = Unit_Id_Equipped(unit, hand);

        /* Skip if no item */
        if (!Weapon_ID_isValid(id)) {
            // SDL_Log("!Weapon_ID_isValid\n");
            continue;
        }

        /* Combine ranges */
        Weapon_Load(unit->weapons_dtab, id);
        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, id);
        SDL_assert(wpn != NULL);

        if (!_Range_Archetype_Match(wpn, archetype)) {
            // SDL_Log("!!_Range_Archetype_Match");
            continue;
        }

        Ranges_Combine(range, wpn->stats.range);
    }
    return (range);
}

b32 Unit_inRange_Loadout(struct Unit        *agg,
                         struct Position    *agg_pos,
                         struct Position    *dft_pos,
                         i64 archetype) {
    struct Range *range = Unit_Range_Equipped(agg, archetype);
    int distance    =  abs(agg_pos->tilemap_pos.x - dft_pos->tilemap_pos.x);
    distance        += abs(agg_pos->tilemap_pos.y - dft_pos->tilemap_pos.y);
    return ((distance >= range->min) && (distance <= range->max));
}

b32 Range_Valid(struct Range range) {
    return ((range.min >= SOTA_MIN_RANGE) && (range.min <= SOTA_MAX_RANGE) &&
            (range.max >= SOTA_MIN_RANGE) && (range.max <= SOTA_MAX_RANGE) &&
            (range.max >= range.min)
           );
}

b32 Ranges_Gap(struct Range r1, struct Range r2) {
    b32 gap  = (r1.max < (r2.min  - 1)) || (r1.min > (r2.max  + 1));
    return (gap);
}

/* "Public", in place range combiner with checking */
void Ranges_Combine(struct Range *r1, struct Range r2) {
    /* Combine ranges. Should never leave gaps*/
    // Gap example:    1  2  3  4  5
    // r1: [1,1]      |-|             (gap with    r2)
    // r1: [1,2]      |----|          (no gap with r2)
    // r2: [3,5]            |-------|
    // SDL_assert(Range_Valid(r2));

    /* Check for a gap in the ranges */
    /* If any range is the default_range, ignore the gaps in range. */
    b32 any_invalid;
    any_invalid  = (r1->min >= Range_default.min) && (r1->max <= Range_default.max);
    any_invalid  |= (r2.min >= Range_default.min) && (r2.max  <= Range_default.max);

    if (!any_invalid && Ranges_Gap(*r1, r2)) {
        *r1 = Range_default;
        // If there is any gaps, set output to invalid to detect during runtime
    } else {
        // There is no gap, combine ranges
        *r1 = _Ranges_Combine(*r1, r2);
        SDL_assert(Range_Valid(*r1));
    }

}

/* "Private", range combiner without checking */
struct Range _Ranges_Combine(struct Range r1, struct Range r2) {
    struct Range out;
    out.max = r1.max > r2.max ? r1.max : r2.max; /* Best max range is biggest  */
    out.min = r1.min < r2.min ? r1.min : r2.min; /* Best min range is smallest */
    return (out);
}

/* --- Rangemap --- */
int Unit_Rangemap_Get(struct Unit *unit) {
    int rangemap = unit->user_rangemap > RANGEMAP_NULL ? unit->user_rangemap : unit->rangemap;
    return (rangemap);
}

void Unit_RangeMap_Act_Toggle(struct Unit *unit) {
    SDL_assert((unit->rangemap > RANGEMAP_NULL) && (unit->rangemap < RANGEMAP_NUM));

    /* Set user_rangemap to default */
    if (unit->user_rangemap == RANGEMAP_NULL)
        unit->user_rangemap = unit->rangemap;

    /* Toggle only if hasStaff or canAttack with equipment*/
    b32 toggle = false;
    toggle |= Unit_canAttack_Eq(unit) && (unit->user_rangemap == RANGEMAP_HEALMAP);
    toggle |= Unit_canStaff_Eq(unit)  && (unit->user_rangemap == RANGEMAP_ATTACKMAP);

    /* user_rangemap not set previously, reverse rangemap */
    // RANGEMAP_NUM - RANGEMAP_ATTACKMAP == RANGEMAP_HEALMAP and vice versa!
    //      3       -         2          ==        1
    //      3       -         1          ==        2
    if (toggle)
        unit->user_rangemap = RANGEMAP_NUM - unit->user_rangemap;

}

void Unit_Rangemap_Default(struct Unit *unit) {
    int rangemap = unit->user_rangemap > RANGEMAP_NULL ? unit->user_rangemap : unit->rangemap;
    // Compute default rangemap priority

    /* Sota default for class (healer staff) */
    if ((unit->class == UNIT_CLASS_PRIEST) || (unit->class == UNIT_CLASS_BISHOP) ||
        (unit->class == UNIT_CLASS_CLERIC) || (unit->class == UNIT_CLASS_ORACLE)) {
        unit->rangemap = RANGEMAP_HEALMAP;
    } else {
        unit->rangemap = RANGEMAP_ATTACKMAP;
    }
}
