
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
    Equipment_Swap(unit->_equipment, i1, i2);
}

// TODO move to equipment source
void Equipment_Swap(struct Inventory_item *_equipment, i16 i1, i16 i2) {
    if ((i1 < 0) || (i1 >= DEFAULT_EQUIPMENT_SIZE)) {
        SDL_Log("Item index1 out of bounds");
        exit(ERROR_OutofBounds);
    }
    if ((i2 < 0) || (i2 >= DEFAULT_EQUIPMENT_SIZE)) {
        SDL_Log("Item index2 out of bounds");
        exit(ERROR_OutofBounds);
    }
    struct Inventory_item buffer  = _equipment[i1];
    _equipment[i1]          = _equipment[i2];
    _equipment[i2]          = buffer;
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
    unit->equipped[weakhand]   = true;
    unit->isTwoHanding         = true;

    return (out);
}

void _Unit_Check_Equipped(struct Unit *unit, b32 hand) {
    if (!unit->equipped[hand])
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

b32 Unit_Equip(struct Unit *unit, b32 hand, int i) {
    Unit_Item_Swap(unit, hand, i);
    return (Unit_Equip_inHand(unit, hand));
}

b32 Unit_Equip_inHand(struct Unit *unit, b32 hand) {
    SDL_Log("hand %d, equipment id %ld", hand, unit->_equipment[hand].id);
    SDL_assert(unit != NULL);

    /* -- Error if try to equip NULL item -- */
    if (unit->_equipment[hand].id <= ITEM_NULL) {
        SDL_Log("No item in hand. Cannot equip.");
        return (unit->equipped[hand] = false);
    }
    if (!Weapon_ID_isValid(unit->_equipment[hand].id)) {
        SDL_Log("Invalid weapon. Cannot equip.");
        return (unit->equipped[hand] = false);
    }

    Weapon_Load(unit->weapons_dtab, unit->_equipment[hand].id);

    /* -- Error checking -- */
    if (!Unit_canEquip(unit, unit->_equipment[hand].id)) {
        SDL_Log("Cannot equip item.");
        return (unit->equipped[hand] = false);
    }

    if (!unit->hands[hand]) {
        SDL_Log("No hand to equip with.");
        return (unit->equipped[hand] = false);
    }

    unit->equipped[hand] = true;
    Unit_isdualWielding(unit);

    return (unit->equipped[hand]);
}

void Unit_Unequip(struct Unit *unit, b32 hand) {
    SDL_assert(unit);

    /* -- Unequip -- */
    unit->equipped[hand] = false;

    /* -- If twohanding, not anymore! -- */
    if (unit->isTwoHanding) {
        unit->_equipment[hand] = Inventory_item_default;
        unit->isTwoHanding    = false;
    }
    /* -- If dual wielding, not anymore! -- */
    unit->isDualWielding = false;

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
//     b32 out = false;
//     do {
//         struct Inventory_item *item = Unit_Item_Strong(unit, UNIT_HAND_WEAK);
//         if ((item->id == ITEM_NULL) || (item->id == ITEM_ID_BROKEN))
//             break;

//         u64 archetype = Item_Archetype(item->id);
//         /* Offhands count as weapon archetype, so need to check like this for wronghanding*/
//         if ((archetype == ITEM_ARCHETYPE_SHIELD) || Item_isOffhand(item->id))
//             break;
//         out = true;
//     } while (false);

//     return (out);
// }

/* Is a unit wielding a weapon in its hand? Note: Units can equip staves.
    -> Equipped + a weapon (not a staff, or offhand, or trinket...)
 */
b32 Unit_isWielding(struct Unit *unit, b32 hand) {
    if (!unit->equipped[hand])
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
b32 Unit_Loadout_twoHanding(int lh, int rh) {
    b32 lh_valid = (lh > -1) && (lh < DEFAULT_EQUIPMENT_SIZE);
    b32 rh_valid = (rh > -1) && (rh < DEFAULT_EQUIPMENT_SIZE);
    return ((lh_valid && rh_valid && (lh == rh)));
}

/* - Public: Chooses between tohanding and not - */
void Unit_Loadout_Swap(struct Unit *unit, int lh, int rh) {
    b32 lh_valid = (lh > -1) && (lh < DEFAULT_EQUIPMENT_SIZE);
    b32 rh_valid = (rh > -1) && (rh < DEFAULT_EQUIPMENT_SIZE);
    b32 twohands = Unit_Loadout_twoHanding(lh, rh);

    if (twohands) {
        _Unit_Loadout_Swap_Twohanding(unit, lh);
    } else if (lh_valid || rh_valid) {
        _Unit_Loadout_Swap(unit, lh, rh);
    } else {
        SDL_Log("Invalid input. Not swapping items %d %d", lh, rh);
    }
    // Unit_Equipment_Print(unit);

}

void _Unit_Loadout_Swap_Twohanding(struct Unit *unit, int i) {
    int stronghand  = Unit_Hand_Strong(unit);
    if (i != stronghand)
        Unit_Item_Swap(unit, stronghand, i);
    unit->temp = Unit_Equip_TwoHanding(unit);
    // TODO: Place in an empty inventory spot.
}

void _Unit_Loadout_Swap(struct Unit *unit, int lh, int rh) {
    SDL_assert(lh != rh); /* no twohanding here */
    b32 lh_valid = ((lh > -1) && (lh < DEFAULT_EQUIPMENT_SIZE));
    b32 rh_valid = ((rh > -1) && (rh < DEFAULT_EQUIPMENT_SIZE));

    /* Swapping */
    if (lh_valid && (lh != UNIT_HAND_LEFT)) {
        Unit_Item_Swap(unit, UNIT_HAND_LEFT, lh);
        /* item rh wanted switched UNIT_HAND_LEFT -> lh */
        if (rh == UNIT_HAND_LEFT)
            rh = lh;
    }

    if (rh_valid && (rh != UNIT_HAND_RIGHT))
        Unit_Item_Swap(unit, UNIT_HAND_RIGHT, rh);

    /* Equipping */
    if (lh_valid)
        Unit_Equip_inHand(unit, UNIT_HAND_LEFT);
    else
        Unit_Unequip(unit, UNIT_HAND_LEFT);

    if (rh_valid)
        Unit_Equip_inHand(unit, UNIT_HAND_RIGHT);
    else
        Unit_Unequip(unit, UNIT_HAND_RIGHT);
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
    if (!unit->equipped[hand]) {
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


