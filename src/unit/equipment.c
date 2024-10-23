
#include "unit/equipment.h"

/* --- Items --- */
/* Private item atker at specific spot. Does no checks */
void _Unit_Item_Takeat(struct Unit *unit, struct Inventory_item item, i32 eq) {
    unit->_equipment[eq - ITEM1] = item;
}

/* Take item at specific spot */
void Unit_Item_Takeat(struct Unit *unit, struct Inventory_item item, i32 eq) {
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab != NULL);
    if (item.id <= ITEM_NULL) {
        return;
    }

    if (Weapon_ID_isValid(item.id)) {
        Weapon_Load(unit->weapons_dtab, item.id);
    } else if (Item_ID_isValid(item.id)) {
        Item_Load(unit->items_dtab, item.id);
    } else {
        return;
    }

    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    SDL_assert(unit->_equipment[eq - ITEM1].id <= ITEM_NULL);

    _Unit_Item_Takeat(unit, item, eq);
    unit->num_equipment++;
}

void Unit_Item_Take(struct Unit *unit, struct Inventory_item item) {
    SDL_assert(unit);
    SDL_assert(unit->num_equipment < SOTA_EQUIPMENT_SIZE);
    SDL_assert(item.id > ITEM_NULL);

    for (i32 eq = ITEM1; eq <= ITEM6; eq++) {
        if (unit->_equipment[eq - ITEM1].id == ITEM_NULL) {
            Unit_Item_Takeat(unit, item, eq);
            break;
        }
    }
}

void Unit_Equipment_Drop(struct Unit *unit) {
    for (i32 eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; eq++) {
        Unit_Item_Drop(unit, eq);
    }
}

struct Inventory_item Unit_Item_Drop(struct Unit *unit, i32 eq) {
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);

    struct Inventory_item out       = unit->_equipment[eq - ITEM1];
    unit->_equipment[eq - ITEM1]    = Inventory_item_default;
    if (unit->num_equipment > 0)
        unit->num_equipment--;
    else
        unit->num_equipment = 0;

    return (out);
}

void Unit_Item_Swap(struct Unit *unit, i32 i1, i32 i2) {
    SDL_assert(unit);

    if (i1 == i2)
        return;

    b32 i1_valid = (i1 >= ITEM1) && (i1 < SOTA_EQUIPMENT_SIZE);
    b32 i2_valid = (i2 >= ITEM1) && (i2 < SOTA_EQUIPMENT_SIZE);
    if (i1_valid && i2_valid) {
        Equipment_Swap(unit->_equipment, i1, i2);
    } else {
        SDL_Log("Invalid item swapping index %d %d", i1, i2);
        SDL_assert(false);
    }
}

void Unit_Item_Trade(struct Unit   *giver,  struct Unit *taker,
                     i32 eq_g,     i32    eq_t) {
    SDL_assert(giver);
    SDL_assert(taker);
    SDL_assert(eq_g >= ITEM1);
    SDL_assert(eq_g < SOTA_EQUIPMENT_SIZE);
    SDL_assert(eq_t >= ITEM1);
    SDL_assert(eq_t < SOTA_EQUIPMENT_SIZE);

    struct Inventory_item buffer_giver = Unit_Item_Drop(giver, eq_g);
    struct Inventory_item buffer_taker = Unit_Item_Drop(taker, eq_t);
    Unit_Item_Takeat(taker, buffer_giver, eq_t);
    Unit_Item_Takeat(giver, buffer_taker, eq_g);
}

/* Checking equipped for errors broken items */
void _Unit_Check_Equipped(struct Unit *unit, i32 hand) {
    if (!Unit_isEquipped(unit, hand)) {
        // SDL_Log("Not equipped");
        return;
    }

    i32 id = Unit_Id_Equipped(unit, hand);

    if (id != ITEM_ID_BROKEN) {
        // SDL_Log("Not equipped");
        return;
    }

    if (Item_ID_isValid(id)) {
        // SDL_Log("Valid item");
        return;
    }

    // SDL_Log("Unit_Unequip");
    Unit_Unequip(unit, hand);
}

void Unit_Check_Equipped(struct Unit *unit) {
    /* Checks if equipped weapon is BORKED, de-equips if so */
    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
        if (Unit_hasHand(unit, hand)) {
            _Unit_Check_Equipped(unit, hand);
        }
    }
}

// Does NOT check if item can be equipped
void Unit_Equip(Unit *unit, i32 hand, i32 eq) {
    SDL_assert(unit);
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(hand < (MAX_ARMS_NUM  + UNIT_HAND_LEFT));
    SDL_assert(hand < (UNIT_ARMS_NUM + UNIT_HAND_LEFT));

    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);

    i32 id = Unit_Id_Equipment(unit, eq);
    SDL_assert(id > ITEM_NULL);

    unit->_equipped[hand - UNIT_HAND_LEFT] = eq;
}

void Unit_Unequip_All(Unit *unit) {
    SDL_assert(unit);
    i64 bytesize = unit->arms_num * sizeof(*unit->_equipped);
    memset(unit->_equipped, 0, bytesize);
}

void Unit_Unequip(struct Unit *unit, i32 hand) {
    SDL_assert(unit);
    SDL_assert(hand >= 0);
    SDL_assert(hand <= MAX_ARMS_NUM);

    /* -- Unequip -- */
    unit->_equipped[hand - UNIT_HAND_LEFT] = ITEM_UNEQUIPPED;

    if (unit->arms_num == UNIT_ARMS_NUM) {
        /* -- If twohanding, not anymore! -- */
        SDL_assert(Unit_istwoHanding(unit) == false);

        /* -- If dual wielding, not anymore! -- */
        unit->isDualWielding    = false;
    }
    // TODO dualwielding, towhanding for Tetrabrachios
}

b32 Unit_canEquip_AnyHand(Unit *unit, canEquip can_equip) {
    SDL_assert(unit != NULL);
    /* Check that unit has hands to equip with */
    b32 found = false;
    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
        if (Unit_hasHand(unit, hand)) {
            found = true;
            break;
        }
    }

    if (!found) {
        return (false);
    }

    /* Check that unit can equip item in hand */
    for (i32 hand = UNIT_HAND_LEFT; hand < unit->arms_num; hand++) {
        /* Skip hands that unit doesn't have */
        if (!Unit_hasHand(unit, hand)) {
            continue;
        }
        can_equip.hand = hand;
        if (!Unit_canEquip(unit, can_equip)) {
            return (false);
        }
    }

    return (true);
}

/* -- Usable -- */
// - Weapon:    CanEquip
// - Staff:     CanEquip
// - Item:      Yes

/* -- canEquip -- */
/* Can unit equip weapon input item? */
// Yes if:
//    - Weapon type is in list of unit usable weapons
//    - Unit can put weapon in the hand
//          - Weapon might need one hand/left/right hand
//          - Weapon might need to be twohanded
//    - IF applicable: unit is in list of users
//  Note: Depends on current equipment in other hands
void Unit_canEquip_Equipment(Unit *unit, canEquip can_equip) {
    /* Save starting equipment */
    i32 start_equipped[MAX_ARMS_NUM];
    Unit_Equipped_Export(unit, start_equipped);

    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
        int eq = can_equip._loadout[hand - ITEM1];
        if (!eq_valid(eq) ) {
            continue;
        }

        i32 id = Unit_Id_Equipment(unit, eq);
        if (!Weapon_ID_isValid(id)) {
            continue;
        }

        Unit_Equip(unit, hand, eq);
    }

    unit->num_canEquip = 0;
    for (i32 eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; eq++) {
        for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
            // SDL_Log("eq, hand %d %d", eq, hand);
            /* Skip if hand is not the input */
            /* If input is NULL, check for any hand */
            if ((can_equip.hand != UNIT_HAND_NULL) && (can_equip.hand != hand)) {
                continue;
            }

            canEquip_Eq(&can_equip, eq);

            /* Set can_equip.hand to durrent hand, and save original */
            i32 input_hand  = can_equip.hand;
            can_equip.hand  = hand;

            if (_Unit_canEquip(unit, can_equip)) {
                // No need to check other hands if added to canEquip.
                unit->eq_canEquip[unit->num_canEquip++] = eq;
                break;
            }
            can_equip.hand  = input_hand;
        }
    }

    /* Restore starting equipment */
    Unit_Equipped_Import(unit, start_equipped);
}

b32 _Unit_canEquip(Unit *unit, canEquip can_equip) {
    SDL_assert(unit != NULL);

    SDL_assert(can_equip._eq >= ITEM_UNEQUIPPED);
    SDL_assert(can_equip._eq <= SOTA_EQUIPMENT_SIZE);

    if (can_equip._eq == ITEM_UNEQUIPPED) {
        // SDL_Log("Can't equip nothing \n");
        return (false);
    }

    if (!Unit_hasHand(unit, can_equip.hand)) {
        // SDL_Log("No hand \n");
        return (false);
    }

    i32 eq = can_equip._eq;
    i32 id = Unit_Id_Equipment(unit, eq);

    if (id <= ITEM_NULL) {
        // SDL_Log("ITEM_NULL\n");
        return (false);
    }

    if (!Weapon_ID_isValid(id)) {
        // SDL_Log("!Weapon_ID_isValid\n");
        return (false);
    }

    if (!Unit_canEquip_Type(unit, id)) {
        // SDL_Log("!Unit_canEquip_Type\n");
        return (false);
    }

    if (!Unit_canEquip_Archetype(unit, id, can_equip.archetype)) {
        // SDL_Log("!Unit_canEquip_Archetype\n");
        return (false);
    }

    if (!Unit_canEquip_Users(unit, id)) {
        // SDL_Log("!Unit_canEquip_Users\n");
        return (false);
    }

    if (!Unit_canEquip_OneHand(unit, eq, can_equip.hand, can_equip.two_hands_mode)) {
        // SDL_Log("!Unit_canEquip_OneHand\n");
        return (false);
    }

    if (!Unit_canEquip_TwoHand(unit, eq, can_equip.hand, can_equip.two_hands_mode)) {
        // SDL_Log("!Unit_canEquip_TwoHand\n");
        return (false);
    }

    return (true);
}

b32 Unit_canEquip(Unit *unit, canEquip can_equip) {
    SDL_assert(unit != NULL);
    /* Save starting equipment */
    i32 start_equipped[MAX_ARMS_NUM];
    Unit_Equipped_Export(unit, start_equipped);

    /* Equip loadout */
    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
        int eq = can_equip._loadout[hand - ITEM1];
        if (!eq_valid(eq) ) {
            continue;
        }

        i32 id = Unit_Id_Equipment(unit, eq);
        if (!Weapon_ID_isValid(id)) {
            continue;
        }

        Unit_Equip(unit, hand, eq);
    }
    /* Check if can equip */
    b32 can = _Unit_canEquip(unit, can_equip);

    /* Restore starting equipment */
    Unit_Equipped_Import(unit, start_equipped);

    return (can);
}

b32 Unit_canEquip_Archetype(Unit *unit, i32 id, i64 archetype) {
    SDL_assert(unit                 != NULL);
    SDL_assert(unit->weapons_dtab   != NULL);

    if (archetype == ITEM_ARCHETYPE_NULL) {
        return (true);
    }

    if (archetype == ITEM_ARCHETYPE_ITEM) {
        return (true);
    }

    Weapon_Load(unit->weapons_dtab, id);
    struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, id);
    SDL_assert(wpn != NULL);

    if (!flagsum_isIn(wpn->item->type, archetype)) {
        return (false);
    }

    return (true);
}

// IF equipment can be two-handed, CAN the unit equip it?
// TODO: Tetrabrachios twohanding?
b32 Unit_canEquip_TwoHand(Unit *unit, i32 eq, i32 hand, i32 mode) {
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    SDL_assert(unit                 != NULL);
    SDL_assert(unit->weapons_dtab   != NULL);

    i32 id = Unit_Id_Equipment(unit, eq);
    if (id <= ITEM_NULL) {
        return (false);
    }
    if (!Weapon_ID_isValid(id)) {
        return (false);
    }

    struct Weapon *wpn  = DTAB_GET(unit->weapons_dtab, id);

    /* Failure: Trying to onehand a twohand only weapon */
    b32 two_hand_only   = Weapon_TwoHand_Only(wpn);

    b32 other_hand      = UNIT_OTHER_HAND(hand);
    i32 eq_other        = Unit_Eq_Equipped(unit, other_hand);
    // Two-hand only weapon can't be equipped if:
    //      - Other hand equipped different wpn.
    b32 eq_diff         = (eq_other != eq);
    b32 strict          = (mode == TWO_HAND_EQ_MODE_STRICT);
    b32 eq_in_bound     = (eq_other >= ITEM1) && (eq_other <= SOTA_EQUIPMENT_SIZE);
    b32 two_hand_cant   = two_hand_only && (eq_in_bound && eq_diff);
    // SDL_Log("eq_other %d", eq_other);
    // SDL_Log("eq_diff, eq_in_bound, two_hand_only %d %d %d", eq_diff, eq_in_bound, two_hand_only);

    if (strict && two_hand_cant) {
        return (false);
    }

    // Weapon could be:
    //  - Two hand only weapon equipped in other hand
    //  - Any hand weapon equipped in one or two hand

    return (true);
}

// IF equipment can be one-handed, CAN the unit equip it?
b32 Unit_canEquip_OneHand(Unit *unit, i32 eq, i32 hand, i32 mode) {
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    SDL_assert(unit                 != NULL);
    SDL_assert(unit->weapons_dtab   != NULL);

    i32 id = Unit_Id_Equipment(unit, eq);
    if (id <= ITEM_NULL)
        return (false);
    if (!Weapon_ID_isValid(id))
        return (false);

    struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, id);
    SDL_assert(wpn != NULL);

    // left hand wpn in right hand
    if ((wpn->handedness == WEAPON_HAND_LEFT) && (hand == UNIT_HAND_RIGHT))
        return (false);

    // right hand wpn in left hand
    if ((wpn->handedness == WEAPON_HAND_RIGHT) && (hand == UNIT_HAND_LEFT))
        return (false);

    /* Failure: Trying to twohand a onehand only wpn */
    b32 other_hand      = UNIT_OTHER_HAND(hand);
    i32 eq_other        = Unit_Eq_Equipped(unit, other_hand);

    // One-hand only wpn can't be equipped if:
    //      - Other hand equipped sane wpn.
    b32 eq_same         = (eq_other == eq);
    b32 eq_in_bound     = (eq_other >= ITEM1) && (eq_other <= SOTA_EQUIPMENT_SIZE);

    b32 one_hand_only   = Weapon_OneHand_Only(wpn);
    b32 one_hand_cant   = one_hand_only && (eq_in_bound && eq_same);

    if (one_hand_cant) {
        // SDL_Log("hand, other hand, %d %d", hand, other_hand);
        return (false);
    }

    b32 strict = (mode == TWO_HAND_EQ_MODE_STRICT);
    // SDL_Log("mode %d %d", mode, TWO_HAND_EQ_MODE_STRICT);
    /* Cannot onehand magic weapons/staves */
    if (Item_hasType(wpn->item, ITEM_TYPE_STAFF)) {
        b32 one_hand_skill = Unit_canStaff_oneHand(unit);
        if (strict && !eq_same && !one_hand_skill) {
            // SDL_Log("Cannot onehand staves %d %d %d", strict, eq_same, one_hand_skill);
            return (false);
        }
    } else if (
            Item_hasType(wpn->item, ITEM_TYPE_ELEMENTAL) ||
            Item_hasType(wpn->item, ITEM_TYPE_ANGELIC)   ||
            Item_hasType(wpn->item, ITEM_TYPE_DEMONIC)
    ) {
        b32 one_hand_skill = Unit_canMagic_oneHand(unit);
        if (strict && !eq_same && !one_hand_skill) {
            // SDL_Log("Cannot onehand magic weapons");
            return (false);
        }
    }

    // Weapon could be:
    //  - one hand only weapon equipped NOT equipped in other hand
    //  - Any hand weapon equipped in one or two hand

    return (true);
}

/* Is unit among item possible users? */
b32 Unit_canEquip_Users(struct Unit *unit, i32 id) {
    SDL_assert(unit                 != NULL);
    SDL_assert(unit->weapons_dtab   != NULL);

    Weapon_Load(unit->weapons_dtab, id);
    struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, id);

    /* Can equip if no list of users */
    if (weapon->item->users == NULL) {
        return (true);
    }

    if (DARR_NUM(weapon->item->users) == 0) {
        return (true);
    }

    for (i32 u = 0; u < DARR_NUM(weapon->item->users); u++) {
        if (weapon->item->users[u] == unit->_id)
            return (true);
    }
    return (false);
}

/* Can unit equip arbitrary weapon with a certain type? */
b32 Unit_canEquip_Type(struct Unit *unit, i32 id) {
    /* Unequippable if ITEM_NULL */
    if (id <= ITEM_NULL) {
        return (false);
    }

    if (!Weapon_ID_isValid(id)) {
        return (false);
    }

    SDL_assert(unit->weapons_dtab != NULL);
    Weapon_Load(unit->weapons_dtab, id);
    struct Weapon *weapon   = DTAB_GET(unit->weapons_dtab, id);
    u16 wpntypecode         = weapon->item->type;
    SDL_assert(wpntypecode);

    /* Is weapon's type equippable by unit? */
    return ((unit->equippable & wpntypecode) > 0);
}

/* Find all CanEquip types, put them in equippables array */
// TODO better types
u8 Unit_canEquip_allTypes(struct Unit *unit, u8 *equippables) {
    u8 type = 1, equippable_num = 0;
    i64 wpntypecode = 1;
    memset(equippables, 0, ITEM_TYPE_EXP_END * sizeof(*equippables));
    while ((wpntypecode < ITEM_TYPE_END) & (type < ITEM_TYPE_EXP_END)) {
        if ((unit->equippable & wpntypecode) > 0)
            equippables[equippable_num++] = type;
        type++;
        wpntypecode *= 2;
    }
    return (equippable_num);
}

/* Is a unit wielding a weapon in its hand? Note: Units can equip staves.
    -> Equipped + a weapon (not a staff, or offhand, or trinket...)
 */
b32 Unit_isWielding(struct Unit *unit, i32 hand) {
    if (!Unit_isEquipped(unit, hand))
        return (false);

    i32 id = Unit_Id_Equipment(unit, hand);
    if (id <= ITEM_NULL)
        return (false);

    if (Item_Archetype(id) != ITEM_ARCHETYPE_WEAPON)
        return (false);

    return (true);
}

/* If a unit dual wielding? i.e. wielding a weapon in both hands */
// TODO: Tetrabrachios dual wielding?
b32 Unit_isdualWielding(struct Unit *unit) {
    SDL_assert(unit);
    // TODO: test this function

    /* To be dual wielding unit must: */
    // - Wield a weapon in weakhand
    // - Wield a weapon in stronghand
    b32 left            = Unit_isWielding(unit, UNIT_HAND_LEFT);
    b32 right           = Unit_isWielding(unit, UNIT_HAND_RIGHT);

    i32 eq_L = Unit_Eq_Equipped(unit, UNIT_HAND_LEFT);
    i32 eq_R = Unit_Eq_Equipped(unit, UNIT_HAND_RIGHT);
    b32 left_canWeakhand    = left  ? Weapon_canWeakhand(eq_L) : true;
    b32 right_canWeakhand   = right ? Weapon_canWeakhand(eq_R) : true;
    unit->isDualWielding    = (left_canWeakhand && right_canWeakhand) && !Unit_istwoHanding(unit);
    return (unit->isDualWielding);
}


/* --- Loadout Manipulation --- */
/* - Does that unit wield a weapon with two hands? - */
// TODO: Tetrabrachios twohanding?
b32 Unit_istwoHanding(Unit *unit) {
    i32 eq_left     = Unit_Eq_Equipped(unit, UNIT_HAND_LEFT);
    i32 eq_right    = Unit_Eq_Equipped(unit, UNIT_HAND_RIGHT);
    return (_istwoHanding(eq_left, eq_right));
}

/* -- Deplete: decrease durability -- */
void _Unit_Item_Deplete(struct Unit *unit, i32 eq, i64 archetype) {
    /* Only unit function that calls Inventory_item_Deplete */
    SDL_assert(eq_valid(eq));
    i32 id = Unit_Id_Equipment(unit, eq);

    /* Skip if NULL. Not an error, unit can have empty hand. */
    if (id == ITEM_NULL) {
        // SDL_Log("ITEM_NULL");
        return;
    }

    if (!Weapon_ID_isValid(id)) {
        // SDL_Log("Invalid id");
        return;
    }


    /* Skip if item's archetype to deplete does not match input. */
    struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, id);
    struct Item   *item   = weapon->item;
    SDL_assert(weapon != NULL);
    SDL_assert(item != NULL);
    if (!(flagsum_isIn(item->type, archetype))) {
        // SDL_Log("Archetype mismatch");
        return;
    }

    // SDL_Log("Depleting");
    Inventory_item_Deplete(&unit->_equipment[eq - ITEM1], item->stats.uses);
}

void _Unit_Equipped_Deplete(Unit *unit, i32 hand, i64 archetype) {
    if (!Unit_isEquipped(unit, hand)) {
        return;
    }

    _Unit_Item_Deplete(unit, Unit_Eq_Equipped(unit, hand), archetype);
}

void Unit_Item_Deplete(struct Unit *unit, i32 eq) {
    /* Upon use, decrease item durability */
    _Unit_Item_Deplete(unit, eq, ITEM_ARCHETYPE_NULL);
}

void Unit_Equipped_Staff_Deplete(struct Unit *unit, i32 hand) {
    /* Upon healing, decrease staff durability */
    _Unit_Equipped_Deplete(unit, hand, ITEM_ARCHETYPE_STAFF);
}

// TODO: Tetrabrachios depletion?
void Unit_Equipped_Weapons_Deplete(struct Unit *unit) {
    // SDL_Log("Unit_Equipped_Weapons_Deplete");
    /* Upon getting a hit, decrease weapon durability */
    _Unit_Equipped_Deplete(unit, UNIT_HAND_LEFT, ITEM_ARCHETYPE_WEAPON);
    if (!Unit_istwoHanding(unit))
        _Unit_Equipped_Deplete(unit, UNIT_HAND_RIGHT, ITEM_ARCHETYPE_WEAPON);
}

// TODO: Tetrabrachios depletion?
void Unit_Equipped_Shields_Deplete(struct Unit *unit) {
    // SDL_Log("Unit_Equipped_Shields_Deplete");
    /* Upon getting hit, decrease shields durability */
    _Unit_Equipped_Deplete(unit, UNIT_HAND_LEFT, ITEM_ARCHETYPE_SHIELD);
    if (!Unit_istwoHanding(unit))
        _Unit_Equipped_Deplete(unit, UNIT_HAND_RIGHT, ITEM_ARCHETYPE_SHIELD);
}

b32 Unit_isEquipped(Unit *unit, i32 hand) {
    b32 min_bound = (Unit_Eq_Equipped(unit, hand) >= ITEM1);
    b32 max_bound = (Unit_Eq_Equipped(unit, hand) <= SOTA_EQUIPMENT_SIZE);
    return (min_bound && max_bound);
}

Inventory_item *Unit_Item_Equipped(Unit *unit, i32 hand) {
    if (!Unit_isEquipped(unit, hand))
        return (NULL);

    i32 eq = Unit_Eq_Equipped(unit, hand);

    return (&unit->_equipment[eq - ITEM1]);
}

Inventory_item *Unit_InvItem(Unit *unit, i32 eq) {
    return (&unit->_equipment[eq - ITEM1]);
}

/* -- Getters -- */
Weapon *Unit_Equipped_Weapon(Unit *unit, i32 hand) {
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);

    /* Skipped if not a weapon */
    if (!Unit_isEquipped(unit, hand))
        return (NULL);

    return (Unit_Weapon(unit, hand));
}

Weapon *Unit_Weapon(Unit *unit, i32 eq) {
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);

    /* Skipped if not a weapon */
    i32 id = unit->_equipment[eq - ITEM1].id;
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
    i32 id = unit->_equipment[eq - ITEM1].id;
    if (Weapon_ID_isValid(id))
        return (NULL);

    /* Load and return item */
    Item_Load(unit->items_dtab, id);
    struct Item *item = DTAB_GET(unit->items_dtab, id);
    SDL_assert(item != NULL);
    return (item);
}

/* Order in _equipment of equipped weapon */
i32 Unit_Eq_Equipped(const Unit *const unit, i32 hand) {
    SDL_assert(unit != NULL);
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(hand < (UNIT_ARMS_NUM + UNIT_HAND_LEFT));
    SDL_assert(hand < (unit->arms_num + UNIT_HAND_LEFT));
    return (unit->_equipped[hand - UNIT_HAND_LEFT]);
}

/* ID of equipment item */
i32 Unit_Id_Equipment(Unit *unit, i32 eq) {
    SDL_assert(unit != NULL);
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    return (unit->_equipment[eq - ITEM1].id);
}

/* ID of equipped weapon */
i32 Unit_Id_Equipped(Unit *unit, i32 hand) {
    SDL_assert(unit != NULL);
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(hand <= unit->arms_num);

    if (!Unit_isEquipped(unit, hand)) {
        return (ITEM_NULL);
    }
    i32 eq = Unit_Eq_Equipped(unit, hand);
    return (unit->_equipment[eq - ITEM1].id);
}

/* -- Use -- */
void Unit_Staff_Use(Unit *healer, Unit *patient) {
    /* Should not be called if staves not properly equipped:
    *       - Staff in strong hand
    *       - Staff in weakhand IF not onehand skill
    */

    /* Get equipped weapon id */
    i32 stronghand  = Unit_Hand_Strong(healer);
    i32 weakhand    = Unit_Hand_Weak(healer);

    i32 weakhand_is   = Unit_isEquipped(healer, weakhand);
    i32 stronghand_is = Unit_isEquipped(healer, stronghand);
    i32 weakhand_id   = Unit_Id_Equipped(healer, weakhand);
    i32 stronghand_id = Unit_Id_Equipped(healer, stronghand);

    struct Inventory_item *weakhand_inv   = Unit_Item_Equipped(healer, weakhand);
    struct Inventory_item *stronghand_inv = Unit_Item_Equipped(healer, stronghand);
    SDL_assert(weakhand_inv     != NULL);
    SDL_assert(stronghand_inv   != NULL);
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
