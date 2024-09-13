
#include "unit/range.h"

/* -- Loadout Range -- */
/* Compute range of current loadout:
*/
struct Range *Unit_Range(struct Unit *unit, canEquip can) {
    struct Range *range = &unit->computed_stats.range_loadout;
    *range = Range_default;

    return (range);
}

// What info do I need to compute range?
// Loadout Weapon id/eq
// Archetype
// 

// struct Range *Unit_Range_Loadout(struct Unit *unit) {

//     struct Range *range = &unit->computed_stats.range_loadout;
//     *range = Range_default;
    // Unit_isdualWielding(unit);

    // b32 stronghand = Unit_Hand_Strong(unit);
    // i32 id_lh = Unit_Id_Equipped(unit, UNIT_HAND_LEFT);
    // i32 id_fh = Unit_Id_Equipped(unit, UNIT_HAND_LEFT);
    // do {
    //     /* If dual wielding, range_loadout is combined. */
    //     if (unit->isDualWielding) {
    //         _Unit_Range_Combine(unit, range, true, ITEM_ARCHETYPE_WEAPON);
    //         // SDL_Log("unit->isDualWielding");
    //         break;
    //     }

    //     /* If not dual wielding, compute range of weapon in stronghand. */
    //     if (!Unit_isEquipped(unit, stronghand)) {
    //         // SDL_Log("stronghand not equipped");
    //         break;
    //     }

    //     canEquip can_equip  = canEquip_default;
    //     can_equip.eq        = stronghand;
    //     can_equip.lh        = -1;
    //     can_equip.rh        = -1;
    //     can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    //     can_equip.hand      = stronghand;

    //     if (!Unit_canEquip(unit, can_equip)) {
    //         // SDL_Log("Not usable");
    //         break;
    //     }

    //     Weapon_Load(unit->weapons_dtab, unit->_equipment[stronghand].id);
    //     struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, unit->_equipment[stronghand].id);
    //     if (wpn == NULL) {
    //         // SDL_Log("WPN null");
    //         break;
    //     }
    //     Ranges_Combine(range, wpn->stats.range);
    // } while (false);

//     return (range);
// }

struct Range *Unit_Range_Id(struct Unit *unit, int id) {
    struct Range *range = &unit->computed_stats.range_combined;
    SDL_assert(range != NULL);
    *range = Range_default;

    if (id == ITEM_NULL) {
        // SDL_Log("ITEM_NULL");
        return (range);
    }

    if (!Weapon_ID_isValid(id)) {
        // SDL_Log("!Weapon_ID_isValid");
        return (range);
    }

    canEquip can_equip  = canEquip_default;
    can_equip.hand      = Unit_Hand_Strong(unit);
    can_equip.id        = id;
    if (!Unit_canEquip(unit, can_equip)) {
        // SDL_Log("!Unit_canEquip");
        return (range);
    }

    Weapon_Load(unit->weapons_dtab, id);
    struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, id);
    SDL_assert(wpn != NULL);

    Ranges_Combine(range, wpn->stats.range);

    return (range);
}

struct Range *Unit_Range_Eq(struct Unit *unit, int eq) {
    SDL_assert(eq > 0);
    SDL_assert(eq < SOTA_EQUIPMENT_SIZE);

    return (Unit_Range_Eq(unit, Unit_Id_Equipment(unit, eq)));
}

/* -- Range Combiners -- */
/* Compute range of equipment or equipped by combining ranges
*   Usage:
*       - Dangermap
*       - Menu Option checking
*/

struct Range *_Unit_Range_Combine( struct Unit   *unit, struct Range *range,
                                   b32 equipped, int archetype) {
    /* - Finds range of ANYTHING - */
    b32 stronghand = Unit_Hand_Strong(unit);
    for (int eq = 0; eq < SOTA_EQUIPMENT_SIZE; eq++) {

        /* Skip if no item */
        if (unit->_equipment[eq].id == ITEM_NULL) {
            // SDL_Log("Skip if no item");
            continue;
        }

        /* Equipped only: Skip if unequipped */
        if (equipped && !Unit_isEquipped(unit, eq)) {
            // SDL_Log("Equipped only: Skip if unequipped");
            continue;
        }
        canEquip can_equip  = canEquip_default;
        can_equip.eq        = eq;
        can_equip.lh        = -1;
        can_equip.rh        = -1;
        can_equip.archetype = archetype;
        can_equip.hand      = stronghand;

        /* Skip if unusable */
        if (!Unit_canEquip(unit, can_equip)) {
            // SDL_Log("Skip if unusable");
            continue;
        }

        /* Combine ranges */
        Weapon_Load(unit->weapons_dtab, unit->_equipment[eq].id);
        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, unit->_equipment[eq].id);
        Ranges_Combine(range, wpn->stats.range);
    }
    return (range);
}

struct Range *Unit_Range_Combine_Equipment(struct Unit *unit) {
    /* Compute range of equipment, using unit rangemap */
    /* Used to find if any */
    return (Unit_Range_Combine(unit, false));
}

struct Range *Unit_Range_Combine(struct Unit *unit, b32 equipped) {
    /* Compute range using unit rangemap */

    /* Decide if user rangemap is used */
    int rangemap = unit->user_rangemap > RANGEMAP_NULL ? unit->user_rangemap : unit->rangemap;

    /* Get weapon archetype from rangemap */
    int archetype = ITEM_ARCHETYPE_WEAPON;
    if (rangemap == RANGEMAP_HEALMAP) {
        archetype = ITEM_ARCHETYPE_STAFF;
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        archetype = ITEM_ARCHETYPE_WEAPON;
    }

    /* Compute range usign archetype */
    struct Range *range = &unit->computed_stats.range_combined;
    range->min = UINT8_MAX;
    range->max = 0;
    return (_Unit_Range_Combine(unit, range, equipped, archetype));
}

struct Range *Unit_Range_Combine_Staves(struct Unit *unit, b32 equipped) {
    /* - Finds range only for same weapon type as DECIDED BY INPUT - */

    struct Range *range = &unit->computed_stats.range_combined;
    range->min = UINT8_MAX;
    range->max = 0;
    _Unit_Range_Combine(unit, range, equipped, ITEM_ARCHETYPE_STAFF);

    return (range);
}

struct Range *Unit_Range_Combine_Weapons(struct Unit *unit, b32 equipped) {
    /* - Finds range only for same weapon type as DECIDED BY INPUT - */

    struct Range *range = &unit->computed_stats.range_combined;
    range->min = Range_default.min;
    range->max = Range_default.max;
    _Unit_Range_Combine(unit, range, equipped, ITEM_ARCHETYPE_WEAPON);

    return (range);
}

b32 Range_Valid(struct Range range) {
    return ((range.min >= SOTA_MIN_RANGE) && (range.min <= SOTA_MAX_RANGE) &&
            (range.max >= SOTA_MIN_RANGE) && (range.max <= SOTA_MAX_RANGE));
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
    SDL_assert(any_invalid || !Ranges_Gap(*r1, r2));

    *r1 = _Ranges_Combine(*r1, r2);
    SDL_assert(Range_Valid(*r1));
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

void Unit_Rangemap_Toggle(struct Unit *unit) {
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

void Unit_Rangemap_Equipment(struct Unit *unit) {
    /* 1- Weapon equipped in strong hand */
    b32 stronghand = Unit_Hand_Strong(unit);
    if (Unit_isEquipped(unit, stronghand) && unit->hands[stronghand]) {
        Weapon_Load(unit->weapons_dtab, unit->_equipment[stronghand].id);
        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, unit->_equipment[stronghand].id);
        u16 type = wpn->item->type;

        if (flagsum_isIn(type, ITEM_TYPE_STAFF) || flagsum_isIn(type, ITEM_TYPE_ITEM)) {
            unit->rangemap = RANGEMAP_HEALMAP;
        } else {
            unit->rangemap = RANGEMAP_ATTACKMAP;
        }
        return;
    }

    /* 2- Weapon equipped in weak hand */
    b32 weakhand = 1 - stronghand;
    if (Unit_isEquipped(unit, weakhand) && unit->hands[weakhand]) {
        Weapon_Load(unit->weapons_dtab, unit->_equipment[weakhand].id);
        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, unit->_equipment[weakhand].id);
        u16 type = wpn->item->type;

        if (flagsum_isIn(type, ITEM_TYPE_STAFF) || flagsum_isIn(type, ITEM_TYPE_ITEM)) {
            unit->rangemap = RANGEMAP_HEALMAP;
        } else {
            unit->rangemap = RANGEMAP_ATTACKMAP;
        }

        return;
    }
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

b32 Unit_inRange_Loadout(Unit *agg, Position *agg_pos, Position *dft_pos) {
    struct Range *range = Unit_Range_Loadout(agg);
    int distance    =  abs(agg_pos->tilemap_pos.x - dft_pos->tilemap_pos.x);
    distance        += abs(agg_pos->tilemap_pos.y - dft_pos->tilemap_pos.y);
    return ((distance >= range->min) && (distance <= range->max));
}
