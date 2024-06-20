
#include "unit/equipment.h"


/* --- Usability --- */
void Unit_Find_Usable(struct Unit *unit, u64 archetype) {
    /* -- Find usable weapons in eq_space --  */
    unit->num_usable = 0;
    for (int i = 0; i < DEFAULT_EQUIPMENT_SIZE; i++) {
        if (Unit_Eq_Usable(unit, archetype, i))
            unit->eq_usable[unit->num_usable++] = i;
    }
}

b32 Unit_All_Usable(struct Unit *unit) {
    /* -- Set all weapons to be usable --  */
    /* Use case: Dropping item  */
    unit->num_usable    = 0;
    b32 all_usable     = true;
    for (int i = 0; i < unit->num_equipment; i++) {
        unit->eq_usable[unit->num_usable] = i;
        if (!unit->eq_usable[unit->num_usable])
            all_usable = false;
        unit->num_usable++;
    }
    return (all_usable);
}

b32 Unit_Eq_Usable( struct Unit *unit, u64 archetype, int i) {
    SDL_assert(i >= 0);
    SDL_assert(i < DEFAULT_EQUIPMENT_SIZE);
    return (Unit_Item_Usable(unit, archetype, unit->_equipment[i].id));
}

b32 Unit_Item_Usable(struct Unit *unit, u64 archetype, int id) {
    b32 usable = false;
    do {
        /* -- If item, everything is usable --  */
        if (archetype == ITEM_ARCHETYPE_ITEM) {
            usable = true;
            break;
        }

        /* -- Looking for a weapon, found item --  */
        if (!Weapon_ID_isValid(id)) {
            usable = false;
            break;
        }

        // Are equipped weapons usable? No.
        // UNLESS TWO-handed and equipped only in one hand.

        /* -- Check if weapon type is in archetype --  */
        Weapon_Load(unit->weapons_dtab, id);
        struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, id);
        SDL_Log("archetype %#16x, %#16x", archetype, weapon->item->type);
        SDL_Log("%d", weapon->item->type & archetype);

        SDL_assert(weapon       != NULL);
        SDL_assert(weapon->item != NULL);
        SDL_assert(weapon->item->type > 0);

        if (flagsum_isIn(weapon->item->type, archetype))
            usable = true;

    } while (false);
    return (usable);
}

/* --- Items --- */
/* Private item atker at specific spot. Does no checks
 */
void _Unit_Item_Takeat(struct Unit *unit, struct Inventory_item item, size_t i) {
    unit->_equipment[i] = item;
}

/* Take item at specific spot
    - During gameplay, errors if taken at non-empty location
 */
void Unit_Item_Takeat(struct Unit *unit, struct Inventory_item item, size_t i) {
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab != NULL);
    if (item.id == ITEM_NULL) {
        return;
    }

    if (Weapon_ID_isValid(item.id)) {
        Weapon_Load(unit->weapons_dtab, item.id);
    } else {
        Item_Load(unit->items_dtab, item.id);
    }

    if ((i < 0) || (i >= DEFAULT_EQUIPMENT_SIZE)) {
        SDL_Log("Item i out of bounds");
        // TODO: many errors are asserts
        exit(ERROR_OutofBounds);
    }
    if (unit->_equipment[i].id != ITEM_NULL) {
        SDL_Log("Item taken at non-empty spot");
        exit(ERROR_OutofBounds);
    }
    _Unit_Item_Takeat(unit, item, i);
    unit->num_equipment++;
}

void Unit_Item_Take(struct Unit *unit, struct Inventory_item item) {
    SDL_assert(unit);
    if (unit->num_equipment >= DEFAULT_EQUIPMENT_SIZE) {
        SDL_Log("Unit Inventory full, should not be able to take item");
        exit(ERROR_OutofBounds);
    }

    if (item.id == ITEM_NULL) {
        SDL_Log("Unit should not be able to take NULL item");
        exit(ERROR_OutofBounds);
    }

    for (size_t i = 0; i < DEFAULT_EQUIPMENT_SIZE; i++) {
        if (unit->_equipment[i].id == ITEM_NULL) {
            Unit_Item_Takeat(unit, item, i);
            break;
        }
    }
}

void Unit_Equipment_Drop(struct Unit *unit) {
    for (int i = 0; i < DEFAULT_EQUIPMENT_SIZE; i++) {
        Unit_Item_Drop(unit, i);
    }
}

void Unit_Equipped_Import(Unit *unit, int *equipped) {
    size_t bytesize = UNIT_HANDS_NUM * sizeof(*equipped);
    memcpy(unit->_equipped, equipped, bytesize);
}

void Unit_Equipped_Export(Unit *unit, int *equipped) {
    size_t bytesize = UNIT_HANDS_NUM * sizeof(*equipped);
    memcpy(equipped, unit->_equipped, bytesize);
}

void Unit_Equipment_Import(struct Unit *unit, struct Inventory_item *equipment) {
    Equipment_Copy(unit->_equipment, equipment, DEFAULT_EQUIPMENT_SIZE);
}

void Unit_Equipment_Export(struct Unit *unit, struct Inventory_item *equipment) {
    Equipment_Copy(equipment, unit->_equipment, DEFAULT_EQUIPMENT_SIZE);
}

struct Inventory_item Unit_Item_Drop(struct Unit *unit, i16 i) {
    if ((i < 0) || (i >= DEFAULT_EQUIPMENT_SIZE)) {
        SDL_Log("Item index out of bounds");
        exit(ERROR_OutofBounds);
    }
    struct Inventory_item out  = unit->_equipment[i];
    unit->_equipment[i]        = Inventory_item_default;
    if (unit->num_equipment > 0)
        unit->num_equipment--;
    else
        unit->num_equipment = 0;

    return (out);
}

void Unit_Item_Swap(struct Unit *unit, i16 i1, i16 i2) {
    SDL_assert(unit);

    if (i1 == i2)
        return;

    b32 i1_valid = (i1 >= 0) && (i1 < DEFAULT_EQUIPMENT_SIZE);
    b32 i2_valid = (i2 >= 0) && (i2 < DEFAULT_EQUIPMENT_SIZE);
    if (i1_valid && i2_valid) {
        Equipment_Swap(unit->_equipment, i1, i2);
    } else {
        SDL_Log("Invalid item swapping index %d %d", i1, i2);
        SDL_assert(false);
    }
}

void Unit_Item_Trade(struct Unit *giver, struct Unit *taker, i16 ig, i16 it) {
    SDL_assert(giver);
    SDL_assert(taker);
    if ((it < 0) || (it >= DEFAULT_EQUIPMENT_SIZE)) {
        SDL_Log("Taker Item index out of bounds");
        exit(ERROR_OutofBounds);
    }
    if ((ig < 0) || (ig >= DEFAULT_EQUIPMENT_SIZE)) {
        SDL_Log("Giver Item index out of bounds");
        exit(ERROR_OutofBounds);
    }
    struct Inventory_item buffer_giver = Unit_Item_Drop(giver, ig);
    struct Inventory_item buffer_taker = Unit_Item_Drop(taker, it);
    Unit_Item_Takeat(taker, buffer_giver, it);
    Unit_Item_Takeat(giver, buffer_taker, ig);
}

struct Inventory_item Unit_Equip_TwoHanding(struct Unit *unit) {
    SDL_assert(unit != NULL);
    /* Weapon is in stronghand, equipping to weakhand */
    int stronghand = Unit_Hand_Strong(unit);
    b32 weakhand  = 1 - stronghand;
    if (!unit->hands[weakhand] || !unit->hands[stronghand]) {
        SDL_Log("Can't equip a weapon in two hands without two hands");
        exit(ERROR_Generic);
    }

    /* -- Checks -- */
    SDL_assert(unit->_equipment[stronghand].id > ITEM_NULL);
    Weapon_Load(unit->weapons_dtab, unit->_equipment[stronghand].id);

    /* -- Copy item in hand to other hand -- */
    struct Inventory_item out  = unit->_equipment[weakhand];
    unit->_equipment[weakhand] = unit->_equipment[stronghand];

    /* -- Set flags -- */
    unit->_equipped[weakhand]   = unit->_equipped[stronghand];
    unit->isTwoHanding          = true;

    return (out);
}

void _Unit_Check_Equipped(struct Unit *unit, b32 hand) {
    if (!Unit_isEquipped(unit, hand))
        return;

    if (unit->_equipment[hand].id <= ITEM_NULL)
        return;

    if (unit->_equipment[hand].id != ITEM_ID_BROKEN)
        return;

    /* Broken item in hand, unequipping */
    Unit_Unequip(unit, hand);
}

void Unit_Check_Equipped(struct Unit *unit) {
    /* Checks if equipped weapon is BORKED, de-equips if so */
    _Unit_Check_Equipped(unit, UNIT_HAND_LEFT);
    _Unit_Check_Equipped(unit, UNIT_HAND_RIGHT);

}

void Unit_Equip(struct Unit *unit, b32 hand, int i) {
    SDL_assert(unit);
    SDL_assert(i >= 0);
    SDL_assert(i < DEFAULT_EQUIPMENT_SIZE);

    unit->_equipped[hand] = i;
}

void Unit_Unequip(struct Unit *unit, b32 hand) {
    SDL_assert(unit);
    /* -- Unequip -- */
    unit->_equipped[hand]   = -1;

    /* -- If twohanding, not anymore! -- */
    unit->isTwoHanding      = false;

    /* -- If dual wielding, not anymore! -- */
    unit->isDualWielding    = false;
}

/* -- Can Equip -- */
/* Can unit equip weapon input item? */
b32 Unit_canEquip( struct Unit *unit, i16 id) {
    if (id <= ITEM_NULL) {
        return (false);
    }

    b32 type    = Unit_canEquip_Type(unit, id);
    b32 left    = Unit_canEquip_Hand(unit, id, UNIT_HAND_LEFT);
    b32 right   = Unit_canEquip_Hand(unit, id, UNIT_HAND_RIGHT);
    // SDL_Log("Unit_canEquip_Hand %d %d %d", type, right, left);
    return (type && (left || right));
}

/* Can unit equip weapon currently in hand? */
b32 Unit_canEquip_inHand( struct Unit *unit, b32 hand) {
    SDL_assert(unit->weapons_dtab != NULL);
    i16 hand_id = unit->_equipment[hand].id;
    return (Unit_canEquip_Hand(unit, hand_id, hand));
}

/* Can unit equip arbitrary weapon in its hand? */
b32 Unit_canEquip_Hand( struct Unit *unit, i16 id, b32 hand) {
    if (id <= ITEM_NULL) {
        return (false);
    }
    if (!Weapon_ID_isValid(id)) {
        return (false);
    }

    SDL_assert(unit->weapons_dtab != NULL);
    struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, id);

    b32 eq_any = (weapon->handedness == WEAPON_HAND_ANY);
    // 2O: two-hand weapon in other hand
    b32 eq_2O  = (weapon->handedness == WEAPON_HAND_TWO);
    // 1A: one-hand weapon in any hand
    b32 eq_1A  = (weapon->handedness == WEAPON_HAND_ONE);
    // TODO: Right hand or left hand

    if (eq_2O || eq_any  || eq_1A) {
        return (true);
    }

    return (false);
}

/* Find all equippable types, put them in equippables array */
u8 Unit_Equippables(struct Unit *unit, u8 *equippables) {
    u8 type = 1, equippable_num = 0;
    uint64_t wpntypecode = 1;
    memset(equippables, 0, ITEM_TYPE_EXP_END * sizeof(*equippables));
    while ((wpntypecode < ITEM_TYPE_END) & (type < ITEM_TYPE_EXP_END)) {
        if ((unit->equippable & wpntypecode) > 0)
            equippables[equippable_num++] = type;
        type++;
        wpntypecode *= 2;
    }
    return (equippable_num);
}

/* Can unit equip arbitrary weapon with a certain type? */
b32 Unit_canEquip_Type( struct Unit *unit, i16 id) {
    /* Unequippable if ITEM_NULL */
    if (id <= ITEM_NULL) {
        return (false);
    }

    if (!Weapon_ID_isValid(id)) {
        return (false);
    }

    SDL_assert(unit->weapons_dtab != NULL);
    Weapon_Load(unit->weapons_dtab, id);
    struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, id);
    u16 wpntypecode      = weapon->item->type;
    SDL_assert(wpntypecode);

    /* Is unit among weapon's users? */
    if ((weapon->item->users == NULL) || (DARR_NUM(weapon->item->users) == 0)) {
        return ((unit->equippable & wpntypecode) > 0);
    }

    /* Is weapon's type equippable by unit? */
    b32 found = false;
    for (u8 i = 0; i < DARR_NUM(weapon->item->users); i++) {
        found = (weapon->item->users[i] == unit->_id);
        if (found) {
            return ((unit->equippable & wpntypecode) > 0);
        }
    }

    return (false);
}

/* Is a rightie using a weapon in its left hand? */
// b32 Unit_iswrongHanding(struct Unit *unit) {
//  Check if archetype in weakhand is NOT ITEM_ARCHETYPE_WEAKHAND
//     return (out);
// }

/* Is a unit wielding a weapon in its hand? Note: Units can equip staves.
    -> Equipped + a weapon (not a staff, or offhand, or trinket...)
 */
b32 Unit_isWielding(struct Unit *unit, b32 hand) {
    if (!Unit_isEquipped(unit, hand))
        return (false);

    u16 id = unit->_equipment[hand].id;
    if (id <= ITEM_NULL)
        return (false);

    if (Item_Archetype(id) != ITEM_ARCHETYPE_WEAPON)
        return (false);

    return (true);
}

/* If a unit dual wielding? i.e. wielding a weapon in both hands */
b32 Unit_isdualWielding(struct Unit *unit) {
    SDL_assert(unit);

    // Unit_Equipment_Print(unit);

    b32 left   = Unit_isWielding(unit, UNIT_HAND_LEFT);
    b32 right  = Unit_isWielding(unit, UNIT_HAND_RIGHT);

    return (unit->isDualWielding = left && right && !unit->isTwoHanding);
}


/* --- Loadout Manipulation --- */
/* - Does that loadout wields a weapon with two hands? - */
b32 Unit_istwoHanding(  struct Unit *u) {
    // TODO
    return (false);
}

/* -- Deplete: decrease durability -- */
void _Unit_Item_Deplete(struct Unit *unit, int i, u64 archetype) {
    /* Only unit function that calls Inventory_item_Deplete */

    /* Skip if NULL. Not an error, unit can have empty hand. */
    if (unit->_equipment[i].id == ITEM_NULL) {
        return;
    }

    /* Skip if item's archetype to deplete does not match input. */
    if (!(Item_Archetype(unit->_equipment[i].id) != archetype)) {
        return;
    }

    struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, unit->_equipment[i].id);
    struct Item   *item   = weapon->item;
    Inventory_item_Deplete(&unit->_equipment[UNIT_HAND_LEFT], item->stats.uses);
}

void _Unit_Equipped_Deplete(struct Unit *unit, b32 hand, u64 archetype) {
    if (!Unit_isEquipped(unit, hand)) {
        return;
    }

    _Unit_Item_Deplete(unit, hand, archetype);
}

void Unit_Item_Deplete(struct Unit *unit, int i) {
    /* Upon use, decrease item durability */
    _Unit_Item_Deplete(unit, i, ITEM_ARCHETYPE_NULL);
}

void Unit_Equipped_Staff_Deplete(struct Unit *unit, b32 hand) {
    /* Upon healing, decrease staff durability */
    _Unit_Equipped_Deplete(unit, hand, ITEM_ARCHETYPE_STAFF);
}

void Unit_Equipped_Weapons_Deplete(struct Unit *unit) {
    /* Upon getting hit, decrease shields durability */
    _Unit_Equipped_Deplete(unit, UNIT_HAND_LEFT, ITEM_ARCHETYPE_WEAPON);
    if (!unit->isTwoHanding)
        _Unit_Equipped_Deplete(unit, UNIT_HAND_RIGHT, ITEM_ARCHETYPE_WEAPON);
}

void Unit_Equipped_Shields_Deplete(struct Unit *unit) {
    /* Upon getting hit, use shields */
    _Unit_Equipped_Deplete(unit, UNIT_HAND_LEFT, ITEM_ARCHETYPE_SHIELD);
    if (!unit->isTwoHanding)
        _Unit_Equipped_Deplete(unit, UNIT_HAND_RIGHT, ITEM_ARCHETYPE_SHIELD);
}

b32 Unit_isEquipped(Unit *unit, b32 hand) {
    b32 min_bound = (unit->_equipped[hand] >= 0);
    b32 max_bound = (unit->_equipped[hand] < DEFAULT_EQUIPMENT_SIZE);
    return (min_bound && max_bound);
}

int Unit_Equipped(Unit *unit, b32 hand) {
    /* Return order of equipped item in unit _equipment*/
    return (unit->_equipped[hand]);
}

Inventory_item *Unit_Item_Equipped(Unit *unit, b32 hand) {
    if (unit->_equipped[hand] < 0 || unit->_equipped[hand] > DEFAULT_EQUIPMENT_SIZE)
        return (NULL);
    else
        return (&unit->_equipment[unit->_equipped[hand]]);
}

Inventory_item *Unit_Item(Unit *unit, int i) {
    return (&unit->_equipment[i]);
}
