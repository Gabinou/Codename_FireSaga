
#include "unit/equipment.h"


/* --- Usability --- */
// TODO: Unit_Find_Usable -> Unit_CanEquip_Archetype_Equipment
void Unit_Find_Usable(struct Unit *unit, u64 archetype) {
    /* -- Find usable weapons in eq_space --  */
    unit->num_usable = 0;
    for (i32 eq = 0; eq < SOTA_EQUIPMENT_SIZE; eq++) {
        if (Unit_Eq_Usable(unit, archetype, eq))
            unit->eq_usable[unit->num_usable++] = eq;
    }
}

// TODO: Unit_canEquip_Archetype_Eq
/* Can unit equip item at eq of archetype */
b32 Unit_Eq_Usable( struct Unit *unit, u64 archetype, i32 eq) {
    SDL_assert(eq >= 0);
    SDL_assert(eq < SOTA_EQUIPMENT_SIZE);
    return (Unit_Item_Usable(unit, archetype, unit->_equipment[eq].id));
}

// TODO: Unit_Item_Usable -> Unit_canEquip_Archetype
b32 Unit_Item_Usable(struct Unit *unit, u64 archetype, i32 id) {
    b32 usable = false;
    /* Only one return is good. */
    do {
        /* -- If item, everything is usable --  */
        if (archetype == ITEM_ARCHETYPE_ITEM) {
            usable = true;
            // SDL_Log("If item, everything is usable");
            break;
        }

        /* -- Looking for a weapon, found item --  */
        if (!Weapon_ID_isValid(id)) {
            usable = false;
            // SDL_Log("Looking for a weapon, found item");
            break;
        }

        // Are equipped weapons usable? No.
        // UNLESS TWO-handed and equipped only in one hand.

        /* -- Check if weapon type is in archetype --  */
        Weapon_Load(unit->weapons_dtab, id);
        struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, id);

        SDL_assert(weapon       != NULL);
        SDL_assert(weapon->item != NULL);
        SDL_assert(weapon->item->type > 0);

        if (!flagsum_isIn(weapon->item->type, archetype)) {
            // SDL_Log("Not in usable archetypes");

            break;
        }

        /* -- Check if weapons can be twohanded -- */
        i32 id_left     = Unit_Id_Equipped(unit, UNIT_HAND_LEFT);
        i32 id_right    = Unit_Id_Equipped(unit, UNIT_HAND_RIGHT);
        // if ((id_left == id) || (id_right == id)) {
        //     /* Item is already equipped in one hand */
        //     if (!Unit_canTwoHand(unit, eq)) {

        //     }
        // }

        usable = true;

    } while (false);
    return (usable);
}
/* Order in _equipment of equipped weapon */
i32 Unit_Eq_Equipped(Unit *unit, i32 hand) {
    SDL_assert((hand == UNIT_HAND_LEFT) || (hand == UNIT_HAND_RIGHT));
    return (unit->_equipped[hand]);
}

/* ID of equipped weapon */
i32 Unit_Id_Equipped(Unit *unit, i32 hand) {
    SDL_assert((hand == UNIT_HAND_LEFT) || (hand == UNIT_HAND_RIGHT));
    return (unit->_equipment[Unit_Eq_Equipped(unit, hand)].id);
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

    if ((i < 0) || (i >= SOTA_EQUIPMENT_SIZE)) {
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
    if (unit->num_equipment >= SOTA_EQUIPMENT_SIZE) {
        SDL_Log("Unit Inventory full, should not be able to take item");
        exit(ERROR_OutofBounds);
    }

    if (item.id == ITEM_NULL) {
        SDL_Log("Unit should not be able to take NULL item");
        exit(ERROR_OutofBounds);
    }

    for (size_t i = 0; i < SOTA_EQUIPMENT_SIZE; i++) {
        if (unit->_equipment[i].id == ITEM_NULL) {
            Unit_Item_Takeat(unit, item, i);
            break;
        }
    }
}

void Unit_Equipment_Drop(struct Unit *unit) {
    for (i32 i = 0; i < SOTA_EQUIPMENT_SIZE; i++) {
        Unit_Item_Drop(unit, i);
    }
}

void Unit_Equipped_Import(Unit *unit, i32 *equipped) {
    size_t bytesize = UNIT_HANDS_NUM * sizeof(*equipped);
    memcpy(unit->_equipped, equipped, bytesize);
}

void Unit_Equipped_Export(Unit *unit, i32 *equipped) {
    size_t bytesize = UNIT_HANDS_NUM * sizeof(*equipped);
    memcpy(equipped, unit->_equipped, bytesize);
}

void Unit_Equipment_Import(struct Unit *unit, struct Inventory_item *equipment) {
    Equipment_Copy(unit->_equipment, equipment, SOTA_EQUIPMENT_SIZE);
}

void Unit_Equipment_Export(struct Unit *unit, struct Inventory_item *equipment) {
    Equipment_Copy(equipment, unit->_equipment, SOTA_EQUIPMENT_SIZE);
}

struct Inventory_item Unit_Item_Drop(struct Unit *unit, i16 i) {
    if ((i < 0) || (i >= SOTA_EQUIPMENT_SIZE)) {
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

    b32 i1_valid = (i1 >= 0) && (i1 < SOTA_EQUIPMENT_SIZE);
    b32 i2_valid = (i2 >= 0) && (i2 < SOTA_EQUIPMENT_SIZE);
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
    if ((it < 0) || (it >= SOTA_EQUIPMENT_SIZE)) {
        SDL_Log("Taker Item index out of bounds");
        SDL_assert(false);
        exit(ERROR_OutofBounds);
    }
    if ((ig < 0) || (ig >= SOTA_EQUIPMENT_SIZE)) {
        SDL_Log("Giver Item index out of bounds");
        SDL_assert(false);
        exit(ERROR_OutofBounds);
    }
    struct Inventory_item buffer_giver = Unit_Item_Drop(giver, ig);
    struct Inventory_item buffer_taker = Unit_Item_Drop(taker, it);
    Unit_Item_Takeat(taker, buffer_giver, it);
    Unit_Item_Takeat(giver, buffer_taker, ig);
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

/// @return: 0 on equip, 1 on failed to equip
i32 Unit_Equip(struct Unit *unit, b32 hand, i32 eq) {
    SDL_assert(unit);
    SDL_assert(eq >= 0);
    SDL_assert(eq < SOTA_EQUIPMENT_SIZE);
    SDL_assert(unit->_equipment[eq].id > ITEM_NULL);
    SDL_assert(unit->weapons_dtab != NULL);

    Weapon_Load(unit->weapons_dtab, unit->_equipment[eq].id);

    /* Check if unit is trying to and can twohand weapon */
    // TODO-> change to Unit_canEquip once twohand polarity is reversed
    b32 other_hand = 1 - hand;
    if ((unit->_equipped[other_hand] == eq) && (!Unit_canTwoHand(unit, eq))) {
        SDL_Log("Cannot twohand");
        return (EXIT_FAILURE);
    }

    unit->_equipped[hand] = eq;
    return (EXIT_SUCCESS);
}

void Unit_Unequip(struct Unit *unit, b32 hand) {
    SDL_assert(unit);
    /* -- Unequip -- */
    unit->_equipped[hand]   = -1;

    /* -- If twohanding, not anymore! -- */
    SDL_assert(Unit_istwoHanding(unit) == false);

    /* -- If dual wielding, not anymore! -- */
    unit->isDualWielding    = false;
}

/* -- Can Equip -- */
// CanEquip vs usable:
// SAME CONCEPT.
// TODO: Remove usable stuff
// TODO: Remove usable stuff
/* Can unit equip weapon input item? */
// Yes if:
//    - Weapon type is in list of unit usable weapons
//    - Unit has space in one hand to put weapon
//          - Weapon might need left/right hand
//          - Weapon might need to be twohanded
//              - Unit can twohand the weapon
//    - IF applicable: unit is in list of users
// Does canEquip depend on equipment in current hand?
//  - Equipment can always be switched
//  - Twohands -> yes.
//      - First hand can always equip.
//      - Second hand needs to check if pointing to same "eq"
//      - Need "eq" in input, not id.
b32 Unit_canEquip(struct Unit *unit, i16 id) {
    if (id <= ITEM_NULL) {
        return (false);
    }

    b32 type    = Unit_canEquip_Type(unit, id);
    b32 left    = Unit_canEquip_Hand(unit, id, UNIT_HAND_LEFT);
    b32 right   = Unit_canEquip_Hand(unit, id, UNIT_HAND_RIGHT);
    b32 users   = Unit_canEquip_Users(unit, id);

    // SDL_Log("Unit_canEquip_Hand %d %d %d", type, right, left);
    return (users && type && (left || right));
}

/* -- Can -- */
// Polarity is INVERSED
// if weapon can be twohanded is PART of Unit_canEquip

b32 Unit_canTwoHand(Unit *unit, i32 i) {
    SDL_assert(unit                 != NULL);
    SDL_assert(unit->weapons_dtab   != NULL);

    struct Inventory_item inv_item = unit->_equipment[i];

    /* Cannot twohand non-weapon, pure items, or broken */
    if (!Weapon_ID_isValid(inv_item.id)) {
        return (false);
    }

    /* Cannot twohand unequippable item */
    if (!Unit_canEquip(unit, inv_item.id)) {
        return (false);
    }

    /* Cannot twohand magic weapons */
    struct Weapon *wpn  = DTAB_GET(unit->weapons_dtab, inv_item.id);
    SDL_assert(wpn->item->type > ITEM_TYPE_NULL);
    b32 is_elemental    = flagsum_isIn(wpn->item->type, ITEM_TYPE_ELEMENTAL);
    b32 is_angelic      = flagsum_isIn(wpn->item->type, ITEM_TYPE_ANGELIC);
    b32 is_demonic      = flagsum_isIn(wpn->item->type, ITEM_TYPE_DEMONIC);
    if (is_elemental || is_angelic || is_demonic) {
        return (false);
    }

    /* Cannot twohand weapon that can only be wielded in one hand */
    b32 any_hand = (wpn->handedness == WEAPON_HAND_TWO);
    b32 two_hand = (wpn->handedness == WEAPON_HAND_ANY);
    if (!(any_hand || two_hand)) {
        return (false);
    }

    return (true);
}

/* Can unit equip weapon currently in hand? */
b32 Unit_canEquip_inHand( struct Unit *unit, b32 hand) {
    SDL_assert(unit->weapons_dtab != NULL);
    i16 hand_id = unit->_equipment[hand].id;
    return (Unit_canEquip_Hand(unit, hand_id, hand));
}

/* Can unit equip input weapon in its hand? */
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

    if (eq_2O || eq_any || eq_1A) {
        return (true);
    }

    return (false);
}

/* Is item among possible users? */
b32 Unit_canEquip_Users(struct Unit *unit, i16 id) {
    /* Is unit among weapon's users? */
    SDL_assert(unit->weapons_dtab != NULL);
    Weapon_Load(unit->weapons_dtab, id);
    struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, id);

    /* Can equip if no list of users */
    if (weapon->item->users == NULL) {
        return (true);
    }

    u16 wpntypecode      = weapon->item->type;
    SDL_assert(wpntypecode);
    b32 found = false;
    for (i32 u = 0; u < DARR_NUM(weapon->item->users); u++) {
        found = (weapon->item->users[u] == unit->_id);
        if (found)
            break;
    }
    return (found);
}

/* Can unit equip arbitrary weapon with a certain type? */
b32 Unit_canEquip_Type(struct Unit *unit, i16 id) {
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

/* Find all CanEquip types, put them in equippables array */
u8 Unit_canEquip_Types(struct Unit *unit, u8 *equippables) {
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
    // TODO: test this function

    /* To be dual wielding unit must: */
    // - Wield a weapon in weakhand
    // - Wield a weapon in stronghand
    b32 left            = Unit_isWielding(unit, UNIT_HAND_LEFT);
    b32 right           = Unit_isWielding(unit, UNIT_HAND_RIGHT);

    i32 eq_L = unit->_equipped[UNIT_HAND_LEFT];
    i32 eq_R = unit->_equipped[UNIT_HAND_RIGHT];
    b32 left_canWeakhand    = left  ? Weapon_canWeakhand(eq_L) : true;
    b32 right_canWeakhand   = right ? Weapon_canWeakhand(eq_R) : true;
    unit->isDualWielding    = (left_canWeakhand && right_canWeakhand) && !Unit_istwoHanding(unit);
    return (unit->isDualWielding);
}


/* --- Loadout Manipulation --- */
/* - Does that unit wield a weapon with two hands? - */
b32 Unit_istwoHanding(Unit *unit) {
    b32 lvalid  =   unit->_equipped[UNIT_HAND_LEFT] >= 0;
    lvalid      &=  unit->_equipped[UNIT_HAND_LEFT] < SOTA_EQUIPMENT_SIZE;
    b32 rvalid  =   unit->_equipped[UNIT_HAND_RIGHT] >= 0;
    rvalid      &=  unit->_equipped[UNIT_HAND_RIGHT] < SOTA_EQUIPMENT_SIZE;
    b32 equal   =   unit->_equipped[UNIT_HAND_LEFT] == unit->_equipped[UNIT_HAND_RIGHT];
    return (lvalid && rvalid && equal);
}

/* -- Deplete: decrease durability -- */
void _Unit_Item_Deplete(struct Unit *unit, i32 i, u64 archetype) {
    /* Only unit function that calls Inventory_item_Deplete */

    /* Skip if NULL. Not an error, unit can have empty hand. */
    if (unit->_equipment[i].id == ITEM_NULL) {
        // SDL_Log("ITEM_NULL");
        return;
    }

    /* Skip if item's archetype to deplete does not match input. */
    if (!(flagsum_isIn(unit->_equipment[i].id, archetype))) {
        // SDL_Log("Archetype mismatch");
        return;
    }

    // SDL_Log("Depleting");
    struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, unit->_equipment[i].id);
    struct Item   *item   = weapon->item;
    Inventory_item_Deplete(&unit->_equipment[i], item->stats.uses);
}

void _Unit_Equipped_Deplete(struct Unit *unit, b32 hand, u64 archetype) {
    if (!Unit_isEquipped(unit, hand)) {
        return;
    }

    _Unit_Item_Deplete(unit, unit->_equipped[hand], archetype);
}

void Unit_Item_Deplete(struct Unit *unit, i32 i) {
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
    if (!Unit_istwoHanding(unit))
        _Unit_Equipped_Deplete(unit, UNIT_HAND_RIGHT, ITEM_ARCHETYPE_WEAPON);
}

void Unit_Equipped_Shields_Deplete(struct Unit *unit) {
    /* Upon getting hit, use shields */
    _Unit_Equipped_Deplete(unit, UNIT_HAND_LEFT, ITEM_ARCHETYPE_SHIELD);
    if (!Unit_istwoHanding(unit))
        _Unit_Equipped_Deplete(unit, UNIT_HAND_RIGHT, ITEM_ARCHETYPE_SHIELD);
}

b32 Unit_isEquipped(Unit *unit, b32 hand) {
    b32 min_bound = (unit->_equipped[hand] >= 0);
    b32 max_bound = (unit->_equipped[hand] < SOTA_EQUIPMENT_SIZE);
    return (min_bound && max_bound);
}

i32 Unit_Equipped(Unit *unit, b32 hand) {
    /* Return order of equipped item in unit _equipment*/
    return (unit->_equipped[hand]);
}

Inventory_item *Unit_Item_Equipped(Unit *unit, b32 hand) {
    if (unit->_equipped[hand] < 0 || unit->_equipped[hand] > SOTA_EQUIPMENT_SIZE)
        return (NULL);
    else
        return (&unit->_equipment[unit->_equipped[hand]]);
}

Inventory_item *Unit_InvItem(Unit *unit, i32 i) {
    return (&unit->_equipment[i]);
}

/* -- Getters -- */
Weapon *Unit_Equipped_Weapon(Unit *unit, b32 hand) {
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);

    /* Skipped if not a weapon */
    if (!unit->_equipped[hand])
        return (NULL);

    return (Unit_Weapon(unit, hand));
}

Weapon *Unit_Weapon(Unit *unit, i32 eq) {
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);

    /* Skipped if not a weapon */
    i16 id = unit->_equipment[eq].id;
    if (!Weapon_ID_isValid(id))
        return (NULL);

    /* Load and return weapon */
    Weapon_Load(unit->weapons_dtab, id);
    Weapon *weapon = DTAB_GET(unit->weapons_dtab, id);
    SDL_assert(weapon != NULL);
    return (weapon);
}

Item *Unit_Get_Item(Unit *unit, i32 eq) {
    SDL_assert(unit);
    SDL_assert(unit->items_dtab);

    /* Skipped if not an item */
    i16 id = unit->_equipment[eq].id;
    if (Weapon_ID_isValid(id))
        return (NULL);

    /* Load and return item */
    Item_Load(unit->items_dtab, id);
    struct Item *item = DTAB_GET(unit->items_dtab, id);
    SDL_assert(item != NULL);
    return (item);
}

/* -- Use -- */
void Unit_Staff_Use(Unit *healer, Unit *patient) {
    /* Should not be called if staves not properly equipped:
    *       - Staff in strong hand
    *       - Staff in weakhand IF not onehand skill
    */

    /* Get equipped weapon id */
    i32 stronghand  = Unit_Hand_Strong(healer);
    i32 weakhand    = 1 - stronghand;
    struct Inventory_item *weakhand_inv   = Unit_Item_Equipped(healer, weakhand);
    struct Inventory_item *stronghand_inv = Unit_Item_Equipped(healer, stronghand);
    SDL_assert(stronghand_inv != NULL);
    SDL_assert(Weapon_isStaff(stronghand_inv->id));

    /* TODO: Check if healer has the staff in onehand skill */
    b32 has_skill = false;
    if (!has_skill) {
        /* Staves should be equipped in both hands */
        SDL_assert((weakhand_inv != NULL) && (weakhand_inv->id == stronghand_inv->id));
    }

    /* Get staff weapon */
    struct Weapon *staff = DTAB_GET(healer->weapons_dtab, stronghand_inv->id);
    SDL_assert(flagsum_isIn(staff->item->type, ITEM_TYPE_STAFF));
    SDL_assert(staff->item->active != NULL);

    /* Use staff active */
    staff->item->active(staff->item, healer, patient);

    /* Deplete staff */
    Unit_Equipped_Staff_Deplete(healer, stronghand);
}
