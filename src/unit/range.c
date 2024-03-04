
#include "unit/range.h"

/* -- Loadout Range -- */
/* Compute range of loadout:
*  - Range of weapon in dominant hand
*   - UNLESS dual-wielding. Ranges combine, BUT only add stats if in range.
*/
struct Range *Unit_Range_Loadout(struct Unit   *unit) {

    struct Range *range = &unit->computed_stats.range_loadout;
    range->min = UINT8_MAX;
    range->max = 0;
    Unit_isdualWielding(unit);

    bool stronghand = Unit_Hand_Strong(unit);
    do {
        /* If dual wielding, range_loadout is combined. */
        if (unit->isDualWielding) {
            SDL_Log("IS dual wielding,");
            _Unit_Range_Combine(unit, range, true, ITEM_ARCHETYPE_WEAPON);
            break;
        }

        /* If not dual wielding, compute range of weapon in stronghand. */
        if (!unit->equipped[stronghand]) {
            SDL_Log("If not dual wielding,");
            break;
        }

        if (!Unit_Eq_Usable(unit, ITEM_ARCHETYPE_WEAPON, stronghand)) {
            SDL_Log("If not usable,");
            break;
        }

        Weapon_Load(unit->weapons_dtab, unit->_equipment[stronghand].id);
        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, unit->_equipment[stronghand].id);
        if (wpn == NULL) {
            SDL_Log("wpn null,");
            break;
        }
        Ranges_Combine(range, wpn->stats.range);
    } while (false);

    return (range);
}

struct Range *Unit_Range_Item(struct Unit   *unit, int i) {
    struct Range *range = &unit->computed_stats.range_combined;
    range->min = UINT8_MAX;
    range->max = 0;

    do {
        /* If dual wielding, range_loadout is combined. */
        Weapon_Load(unit->weapons_dtab, unit->_equipment[i].id);
        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, unit->_equipment[i].id);
        if (wpn == NULL)
            break;

        Ranges_Combine(range, wpn->stats.range);
    } while (false);

    return (range);
}

/* -- Range Combiners -- */
/* Compute range of equipment or equipped by combining ranges
*   Usage:
*       - Dangermap
*       - Menu Option checking
*/

struct Range *_Unit_Range_Combine( struct Unit   *unit, struct Range *range,
                                   bool equipped, int archetype) {
    /* - Finds range of ANYTHING - */
    int num = equipped ? UNIT_HANDS_NUM : DEFAULT_EQUIPMENT_SIZE;
    bool stronghand = Unit_Hand_Strong(unit);
    for (int i = 0; i < num; i++) {
        /* Skip if no item */
        if (unit->_equipment[i].id == ITEM_NULL)
            continue;

        /* Equipped only: Skip if unequipped */
        if (equipped && !unit->equipped[i])
            continue;

        /* Skip if unusable */
        if (!Unit_Eq_Usable(unit, archetype, i))
            continue;

        /* Skip if wrong archetype*/
        if ((archetype == ITEM_ARCHETYPE_STAFF)  && !Item_isStaff(unit->_equipment[i].id))
            continue;

        /* Skip if weakhand for staff */
        if ((archetype == ITEM_ARCHETYPE_STAFF)  && (i != stronghand) && (equipped))
            continue;

        if ((archetype == ITEM_ARCHETYPE_WEAPON) && !Item_isWeapon(unit->_equipment[i].id))
            continue;

        /* Combine ranges */
        Weapon_Load(unit->weapons_dtab, unit->_equipment[i].id);
        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, unit->_equipment[i].id);
        Ranges_Combine(range, wpn->stats.range);
    }

    return (range);
}

struct Range *Unit_Range_Combine_Equipment(struct Unit *unit) {
    /* Compute range of equipment, using unit rangemap */
    /* Used to find if any */
    return (Unit_Range_Combine(unit, false));
}

struct Range *Unit_Range_Combine(struct Unit *unit, bool equipped) {
    /* Compute range using unit rangemap */
    int archetype = ITEM_ARCHETYPE_WEAPON;
    int rangemap = unit->user_rangemap > RANGEMAP_NULL ? unit->user_rangemap : unit->rangemap;

    if (rangemap == RANGEMAP_HEALMAP) {
        archetype = ITEM_ARCHETYPE_STAFF;
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        archetype = ITEM_ARCHETYPE_WEAPON;
    }

    struct Range *range = &unit->computed_stats.range_combined;
    range->min = UINT8_MAX;
    range->max = 0;
    return (_Unit_Range_Combine(unit, range, equipped, archetype));
}

struct Range *Unit_Range_Combine_Staves(struct Unit *unit, bool equipped) {
    /* - Finds range only for same weapon type as DECIDED BY INPUT - */

    struct Range *range = &unit->computed_stats.range_combined;
    range->min = UINT8_MAX;
    range->max = 0;
    _Unit_Range_Combine(unit, range, equipped, ITEM_ARCHETYPE_STAFF);

    return (range);
}

struct Range *Unit_Range_Combine_Weapons(struct Unit *unit, bool equipped) {
    /* - Finds range only for same weapon type as DECIDED BY INPUT - */

    struct Range *range = &unit->computed_stats.range_combined;
    range->min = UINT8_MAX;
    range->max = 0;
    _Unit_Range_Combine(unit, range, equipped, ITEM_ARCHETYPE_WEAPON);

    return (range);
}

bool Range_Valid(struct Range range) {
    return ((range.max > 0) && (range.min > 0) && (range.min <= SOTA_MAX_RANGE)
            && (range.max <= SOTA_MAX_RANGE));
}

void Ranges_Combine(struct Range *r1, struct Range r2) {
    // Gap example:    1  2  3  4  5
    // r1: [1,1]      |-|             (gap with    r2)
    // r1: [1,2]      |----|          (no gap with r2)
    // r2: [3,5]            |-------|
    bool r1_valid = Range_Valid(*r1);
    bool r2_valid = Range_Valid(r2);

    bool gap  = (r1->max < (r2.min  - 1)) || (r1->min > (r2.max  + 1));

    if (gap && r1_valid && r2_valid) {
        SDL_Log("Gap in combined ranges. Should never happen.");
        exit(ERROR_OutofBounds);
    }

    r1->max = r1->max > r2.max ? r1->max : r2.max; /* Best max range is biggest  */
    r1->min = r1->min < r2.min ? r1->min : r2.min; /* Best min range is smallest */
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
    bool toggle = false;
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
    bool stronghand = Unit_Hand_Strong(unit);
    if (unit->equipped[stronghand]) {
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
    bool weakhand = 1 - stronghand;
    if (unit->equipped[weakhand] && unit->hands[weakhand]) {
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
