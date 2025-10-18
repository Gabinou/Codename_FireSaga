
#include "unit/unit.h"
#include "unit/range.h"
#include "unit/flags.h"
#include "unit/loadout.h"
#include "unit/equipment.h"
#include "nmath.h"
#include "enums.h"
#include "macros.h"
#include "position.h"
#include "item.h"
#include "weapon.h"
#include "globals.h"

b32 _Range_Archetype_Match(i32 id, i64 archetype) {
    if (!Item_ID_isValid(id)) {
        return (false);
    }

    if (archetype == ITEM_ARCHETYPE_NULL) {
        return (false);
    }

    if (archetype == ITEM_ARCHETYPE_ANY) {
        return (true);
    }

    if (!flagsum_isIn(Item_ID2Type(id), archetype)) {
        return (false);
    }

    return (true);
}


void Unit_Range_Id( Unit    *unit,
                    int id, i64 archetype,
                    Range   *range) {
    // struct Range *range = &unit->computed_stats.range_equipment;
    SDL_assert(range != NULL);
    *range = Range_default;

    if (id <= ITEM_NULL) {
        // SDL_Log("ITEM_NULL");
        return;
    }

    if (!Item_ID_isValid(id)) {
        // SDL_Log("!Item_ID_isValid");
        return;
    }

    if (!_Range_Archetype_Match(id, archetype)) {
        // SDL_Log("!_Range_Archetype_Match");
        return;
    }

    // TODO: differentiate range for USE and range for ATTACK
    if (Weapon_ID_isValid(id)) {
        const Weapon *wpn  = _Weapon_Get(id);
        SDL_assert(wpn);
        Ranges_Combine(range, Weapon_Range(wpn));
    } else {
        const Item *item  = _Item_Get(id);
        Ranges_Combine(range, Item_Range(item));
    }
}

void Unit_Range_Eq( Unit *unit, i32 eq,
                    i64 archetype, Range *range) {
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
        // struct Range *range = &unit->computed_stats.range_equipment;
        SDL_assert(range != NULL);
        *range = Range_default;
        return;
    }

    Unit_Range_Id(  unit, Unit_Id_Equipment(unit, eq),
                    archetype, range);
}

/* Combines range of all weapons in equipment assuming NO LOADOUT */
// - Combined range may no reflect actual loadout range
//      - Ex: will combine range of two two-hand only weapons
void Unit_Range_Equipment(  Unit    *unit, i64 archetype,
                            Range   *range) {
    // struct Range *range = &unit->computed_stats.range_equipment;
    *range              = Range_default;

    for (int eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; eq++) {

        /* Skip if no item */
        i32 id = Unit_Id_Equipment(unit, eq);
        if (!Item_ID_isValid(id)) {
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
        if (!_Range_Archetype_Match(id, archetype)) {
            // SDL_Log("!!_Range_Archetype_Match");
            return;
        }

        // TODO: differentiate range for USE and range for ATTACK
        if (Weapon_ID_isValid(id)) {
            const Weapon *wpn  = _Weapon_Get(id);
            SDL_assert(wpn);
            Ranges_Combine(range, Weapon_Range(wpn));
        } else {
            const Item *item  = _Item_Get(id);
            Ranges_Combine(range, Item_Range(item));
        }
    }
    return;
}

// Combine ranges of items in current loadout
// Item was previously equipped, no need to check if CAN equip
void Unit_Range_Equipped(   Unit    *unit, i64 archetype,
                            Range   *range) {
    // struct Range *range = &unit->computed_stats.range_loadout;
    *range = Range_default;

    for (int hand = UNIT_HAND_LEFT; hand <= unit->arms.num; hand++) {
        int id = Unit_Id_Equipped(unit, hand);

        /* Skip if no item */

        /* Combine ranges */
        if (!_Range_Archetype_Match(id, archetype)) {
            // SDL_Log("!!_Range_Archetype_Match");
            continue;
        }

        // TODO: differentiate range for USE and range for ATTACK
        if (Weapon_ID_isValid(id)) {
            const Weapon *wpn  = _Weapon_Get(id);
            SDL_assert(wpn);
            Ranges_Combine(range, Weapon_Range(wpn));
        } else {
            const Item *item  = _Item_Get(id);
            Ranges_Combine(range, Item_Range(item));
        }
    }
}

/* Is defendant in range of aggressor equipped? */
b32 Unit_inRange_Equipped(struct Unit       *agg,
                          struct Position    *agg_pos,
                          struct Position    *dft_pos,
                          i64                 archetype) {
    struct Range range = Range_default;
    Unit_Range_Equipped(agg, archetype, &range);
    int distance = Point_Distance(  agg_pos->tilemap_pos,
                                    dft_pos->tilemap_pos);
    /* TODO: use inrange_dist */
    return ((distance >= range.min) && (distance <= range.max));
}

b32 Range_Valid(struct Range range) {
    return ((range.min >= SOTA_MIN_RANGE) &&
            (range.min <= SOTA_MAX_RANGE) &&
            (range.max >= SOTA_MIN_RANGE) &&
            (range.max <= SOTA_MAX_RANGE) &&
            (range.max >= range.min)
           );
}

b32 Ranges_Gap(struct Range r1, struct Range r2) {
    /* Only a gap if:
    **  - r2.max at 1 distance or more to r1.min
    **  - r1.max at 1 distance or more to r2.min */
    b32 gap  = (r1.max < (r2.min  - 1)) || (r1.min > (r2.max  + 1));
    return (gap);
}

/* "Public", in place range combiner with checking */
void Ranges_Combine(Range *r1, Range r2) {
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
Range _Ranges_Combine(Range r1, Range r2) {
    Range out = Range_default;
    out.max = r1.max > r2.max ? r1.max : r2.max; /* Best max range is biggest  */
    out.min = r1.min < r2.min ? r1.min : r2.min; /* Best min range is smallest */
    return (out);
}

Range _Ranges_Combinearr(Range *range, i32 num) {
    Range out = Range_default;
    for (int i = 0; i < num; i++) {
        out = _Ranges_Combine(out, range[i]);
    }
    return (out);
}

/* --- Rangemap --- */
int Unit_Rangemap_Get(Unit *unit) {
    if (unit == NULL)
        return (0);
    i32 user_rangemap   = Unit_User_Rangemap(unit);
    i32 rangemap        = Unit_Rangemap(unit);
    int out = user_rangemap > RANGEMAP_NULL ? user_rangemap : rangemap;
    return (out);
}
int  Unit_Rangemap(Unit *unit) {
    if (unit == NULL)
        return (0);
    return (unit->render.rangemap);
}
int  Unit_User_Rangemap(Unit *unit) {
    if (unit == NULL)
        return (0);
    return (unit->render.user_rangemap);
}
void Unit_Rangemap_set(Unit *unit, int rangemap) {
    if (unit == NULL)
        return;
    unit->render.rangemap = rangemap;
}
void Unit_User_Rangemap_set(Unit *unit, int rangemap) {
    if (unit == NULL)
        return;
    unit->render.user_rangemap = rangemap;
}

void Unit_RangeMap_Act_Toggle(struct Unit *unit) {
    SDL_assert((unit->render.rangemap > RANGEMAP_NULL) && (unit->render.rangemap < RANGEMAP_NUM));

    /* Set user_rangemap to default */
    if (Unit_User_Rangemap(unit) == RANGEMAP_NULL)
        Unit_User_Rangemap_set(unit, Unit_Rangemap(unit));

    /* Toggle only if hasStaff or canAttack with equipment*/
    b32 toggle = false;
    toggle |= Unit_canAttack_Eq(unit) && (Unit_User_Rangemap(unit) == RANGEMAP_HEALMAP);
    toggle |= Unit_canStaff_Eq(unit)  && (Unit_User_Rangemap(unit) == RANGEMAP_ATTACKMAP);

    /* user_rangemap not set previously, reverse rangemap */
    // RANGEMAP_NUM - RANGEMAP_ATTACKMAP == RANGEMAP_HEALMAP and vice versa!
    //      3       -         2          ==        1
    //      3       -         1          ==        2
    if (toggle)
        Unit_User_Rangemap_set(unit, RANGEMAP_NUM - Unit_User_Rangemap(unit));

}

void Unit_Rangemap_default(struct Unit *unit) {
    // Compute default rangemap priority

    /* Sota default for class (healer staff) */
    i32 class = Unit_Class(unit);
    if ((class == UNIT_CLASS_PRIEST) ||
        (class == UNIT_CLASS_BISHOP) ||
        (class == UNIT_CLASS_CLERIC) ||
        (class == UNIT_CLASS_ORACLE)) {
        Unit_Rangemap_set(unit, RANGEMAP_HEALMAP);
    } else {
        Unit_Rangemap_set(unit, RANGEMAP_ATTACKMAP);
    }
}

b32 inRange(Range r1, Range r2) {
    /* InRange if ranges overlap */
    b32 r2_max_in = (r2.max >= r1.min) && (r2.max <= r1.max);
    b32 r2_min_in = (r2.min >= r1.min) && (r2.min <= r1.max);

    b32 r1_max_in = (r1.max >= r2.min) && (r1.max <= r2.max);
    b32 r1_min_in = (r1.min >= r2.min) && (r1.min <= r2.max);

    return (r1_max_in || r1_min_in || r2_max_in || r2_min_in);
}

b32 inRange_Dist(Range r1, i32 dist) {
    /* Distance is inrange if dist ∈ [r1.min, r1.max]  */
    b32 out = (dist >= r1.min) && (dist <= r1.max);
    return (out);
}
