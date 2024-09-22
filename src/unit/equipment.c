
#include "unit/equipment.h"

/* - canEquips setters - */
void canEquip_Eq(canEquip *can_equip, enum_eq eq) {
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    can_equip->to_eq = eq + ITEM_EQUIPPED_DIFF;
}

void canEquip_Loadout(canEquip *can_equip, unit_hand hand, enum_eq eq) {
    SDL_assert(hand >= 0);
    SDL_assert(hand < MAX_ARMS_NUM);
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    can_equip->loadout[hand] = eq + ITEM_EQUIPPED_DIFF;
}

void canEquip_Loadout_None(canEquip *can_equip, unit_hand hand) {
    SDL_assert(hand >= 0);
    SDL_assert(hand < MAX_ARMS_NUM);
    can_equip->loadout[hand] = ITEM_UNEQUIPPED;
}

/* --- Items --- */
/* Private item atker at specific spot. Does no checks */
void _Unit_Item_Takeat(struct Unit *unit, struct Inventory_item item, enum_eq eq) {
    unit->_equipment[eq] = item;
}

/* Take item at specific spot
    - During gameplay, errors if taken at non-empty location
 */
void Unit_Item_Takeat(struct Unit *unit, struct Inventory_item item, enum_eq eq) {
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

    if ((eq < 0) || (eq >= SOTA_EQUIPMENT_SIZE)) {
        SDL_Log("Item i out of bounds");
        // TODO: many errors are asserts
        exit(ERROR_OutofBounds);
    }
    if (unit->_equipment[eq].id != ITEM_NULL) {
        SDL_Log("Item taken at non-empty spot");
        exit(ERROR_OutofBounds);
    }
    _Unit_Item_Takeat(unit, item, eq);
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

    for (enum_eq eq = 0; eq < SOTA_EQUIPMENT_SIZE; eq++) {
        if (unit->_equipment[eq].id == ITEM_NULL) {
            Unit_Item_Takeat(unit, item, eq);
            break;
        }
    }
}

void Unit_Equipment_Drop(struct Unit *unit) {
    for (enum_eq eq = 0; eq < SOTA_EQUIPMENT_SIZE; eq++) {
        Unit_Item_Drop(unit, eq);
    }
}

struct Inventory_item Unit_Item_Drop(struct Unit *unit, enum_eq eq) {
    if ((eq < 0) || (eq >= SOTA_EQUIPMENT_SIZE)) {
        SDL_Log("Item index out of bounds");
        exit(ERROR_OutofBounds);
    }
    struct Inventory_item out  = unit->_equipment[eq];
    unit->_equipment[eq]        = Inventory_item_default;
    if (unit->num_equipment > 0)
        unit->num_equipment--;
    else
        unit->num_equipment = 0;

    return (out);
}

void Unit_Item_Swap(struct Unit *unit, enum_eq i1, enum_eq i2) {
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

void Unit_Item_Trade(struct Unit   *giver,  struct Unit *taker,
                     enum_eq ig,     enum_eq    it) {
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

/* Importing and exporting equipped for wloadout functions */
void Unit_Equipped_Import(Unit *unit, enum_equipped *equipped) {
    size_t bytesize = unit->arms_num * sizeof(*equipped);
    memcpy(unit->_equipped, equipped, bytesize);
}

void Unit_Equipped_Export(Unit *unit, enum_equipped *equipped) {
    size_t bytesize = unit->arms_num * sizeof(*equipped);
    memcpy(equipped, unit->_equipped, bytesize);
}

/* Importing and exporting equipment */
void Unit_Equipment_Import(struct Unit *unit, struct Inventory_item *equipment) {
    Equipment_Copy(unit->_equipment, equipment, SOTA_EQUIPMENT_SIZE);
}

void Unit_Equipment_Export(struct Unit *unit, struct Inventory_item *equipment) {
    Equipment_Copy(equipment, unit->_equipment, SOTA_EQUIPMENT_SIZE);
}

/* Checking equipped for errors broken items */
void _Unit_Check_Equipped(struct Unit *unit, unit_hand hand) {
    if (!Unit_isEquipped(unit, hand))
        return;

    item_id id = Unit_Id_Equipped(unit, hand);

    if (id != ITEM_ID_BROKEN)
        return;

    if (Item_ID_isValid(id))
        return;

    Unit_Unequip(unit, hand);
}

void Unit_Check_Equipped(struct Unit *unit) {
    /* Checks if equipped weapon is BORKED, de-equips if so */
    for (unit_hand hand = 0; hand < unit->arms_num; hand++) {
        if (unit->hands[hand]) {
            _Unit_Check_Equipped(unit, hand);
        }
    }
}

// Does NOT check if item can be equipped
void Unit_Equip(Unit *unit, unit_hand hand, enum_eq eq) {
    SDL_assert(unit);
    SDL_assert(hand >= 0);
    SDL_assert(hand < MAX_ARMS_NUM);
    SDL_assert(eq >= 0);
    SDL_assert(eq < SOTA_EQUIPMENT_SIZE);
    item_id id = Unit_Id_Equipment(unit, eq);
    SDL_assert(id > ITEM_NULL);

    unit->_equipped[hand] = eq + ITEM_EQUIPPED_DIFF;
}

void Unit_Unequip(struct Unit *unit, unit_hand hand) {
    SDL_assert(unit);
    SDL_assert(hand >= 0);
    SDL_assert(hand < MAX_ARMS_NUM);

    /* -- Unequip -- */
    unit->_equipped[hand] = ITEM_UNEQUIPPED;

    if (unit->arms_num == UNIT_ARMS_NUM) {
        /* -- If twohanding, not anymore! -- */
        SDL_assert(Unit_istwoHanding(unit) == false);

        /* -- If dual wielding, not anymore! -- */
        unit->isDualWielding    = false;
    }
    // TODO dualwielding, towhanding for Tetrabrachios
}

b32 Unit_canEquip_AnyHand(Unit *unit, canEquip can_equip) {
    /* Check that unit has hands to equip with */
    b32 found = false;
    for (unit_hand hand = 0; hand < unit->arms_num; hand++) {
        if (unit->hands[hand]) {
            found = true;
            break;
        }
    }

    if (!found) {
        return (false);
    }

    /* Check that unit can equip item in hand */
    for (unit_hand hand = 0; hand < unit->arms_num; hand++) {
        /* Skip hands that unit doesn't have */
        if (!unit->hands[hand]) {
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
    enum_equipped start_equipped[MAX_ARMS_NUM];
    Unit_Equipped_Export(unit, start_equipped);

    for (unit_hand hand = 0; hand < unit->arms_num; hand++) {
        unit->_equipped[hand] = can_equip.loadout[hand];
    }

    unit->num_canEquip = 0;
    for (enum_eq eq = 0; eq < SOTA_EQUIPMENT_SIZE; eq++) {
        for (unit_hand hand = 0; hand < unit->arms_num; hand++) {
            // canEquip hands
            canEquip_Eq(&can_equip, eq);
            can_equip.hand  = hand;
            if (_Unit_canEquip(unit, can_equip)) {
                unit->eq_canEquip[unit->num_canEquip++] = eq;
                continue;
            }
        }
    }

    /* Restore starting equipment */
    Unit_Equipped_Import(unit, start_equipped);
}

b32 _Unit_canEquip(Unit *unit, canEquip can_equip) {

    SDL_assert(can_equip.to_eq >= ITEM_UNEQUIPPED);
    SDL_assert(can_equip.to_eq <= SOTA_EQUIPMENT_SIZE);

    if (can_equip.to_eq == ITEM_UNEQUIPPED) {
        SDL_Log("Can't equip nothing \n");
        return (false);
    }

    if (!unit->hands[can_equip.hand]) {
        SDL_Log("No hand \n");
        return (false);
    }

    enum_equipped eq = can_equip.to_eq - ITEM_EQUIPPED_DIFF;

    item_id id = Unit_Id_Equipment(unit, eq);
    SDL_Log("eq, id %d %d\n", eq, id);

    if (id <= ITEM_NULL) {
        SDL_Log("ITEM_NULL\n");
        return (false);
    }

    if (!Weapon_ID_isValid(id)) {
        SDL_Log("!Weapon_ID_isValid\n");
        return (false);
    }

    if (!Unit_canEquip_Type(unit, id)) {
        SDL_Log("!Unit_canEquip_Type\n");
        return (false);
    }

    if (!Unit_canEquip_Archetype(unit, id, can_equip.archetype)) {
        SDL_Log("!Unit_canEquip_Archetype\n");
        return (false);
    }

    if (!Unit_canEquip_Users(unit, id)) {
        SDL_Log("!Unit_canEquip_Users\n");
        return (false);
    }

    if (!Unit_canEquip_OneHand(unit, eq, can_equip.hand)) {
        SDL_Log("!Unit_canEquip_OneHand\n");
        return (false);
    }

    if (!Unit_canEquip_TwoHand(unit, eq, can_equip.hand, can_equip.two_hands_mode)) {
        SDL_Log("!Unit_canEquip_TwoHand\n");
        return (false);
    }

    return (true);
}

b32 Unit_canEquip(Unit *unit, canEquip can_equip) {
    /* Save starting equipment */
    enum_equipped start_equipped[MAX_ARMS_NUM];
    Unit_Equipped_Export(unit, start_equipped);

    /* Equip loadout */
    for (unit_hand hand = 0; hand < unit->arms_num; hand++) {
        unit->_equipped[hand] = can_equip.loadout[hand];
    }
    /* Check if can equip */
    b32 can = _Unit_canEquip(unit, can_equip);

    /* Restore starting equipment */
    Unit_Equipped_Import(unit, start_equipped);

    return (can);
}

b32 Unit_canEquip_Archetype(Unit *unit, item_id id, item_archetype archetype) {
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
b32 Unit_canEquip_TwoHand(Unit *unit, enum_eq eq, unit_hand hand, i32 mode) {
    SDL_assert(eq >= 0);
    SDL_assert(eq < SOTA_EQUIPMENT_SIZE);
    SDL_assert(unit                 != NULL);
    SDL_assert(unit->weapons_dtab   != NULL);

    item_id id = Unit_Id_Equipment(unit, eq);
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
    enum_eq eq_other        = Unit_Eq_Equipped(unit, other_hand);
    // Two-hand only weapon can't be equipped if:
    //      - Other hand equipped different wpn.
    b32 eq_diff         = (eq_other != eq);
    b32 strict          = (mode == TWO_HAND_EQ_MODE_STRICT);
    b32 eq_in_bound     = (eq_other >= 0) && (eq_other < SOTA_EQUIPMENT_SIZE);
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
b32 Unit_canEquip_OneHand(Unit *unit, enum_eq eq, unit_hand hand) {
    SDL_assert(eq >= 0);
    SDL_assert(eq < SOTA_EQUIPMENT_SIZE);
    SDL_assert(unit                 != NULL);
    SDL_assert(unit->weapons_dtab   != NULL);

    item_id id = Unit_Id_Equipment(unit, eq);
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
    enum_eq eq_other        = Unit_Eq_Equipped(unit, other_hand);

    // One-hand only wpn can't be equipped if:
    //      - Other hand equipped different wpn.
    b32 eq_same         = (eq_other == eq);
    b32 eq_in_bound     = (eq_other >= 0) && (eq_other < SOTA_EQUIPMENT_SIZE);

    b32 one_hand_only   = Weapon_OneHand_Only(wpn);
    b32 one_hand_cant   = one_hand_only && (eq_in_bound && eq_same);
    SDL_Log("eq_other %d", eq_other);
    SDL_Log("eq_same, eq_in_bound, one_hand_only %d %d %d", eq_same, eq_in_bound, one_hand_only);

    if (one_hand_cant) {
        return (false);
    }
    SDL_Log("CAN");

    /* Cannot onehand magic weapons/staves */
    if (Item_hasType(wpn->item, ITEM_TYPE_STAFF)) {
        b32 one_hand_skill = Unit_canStaff_oneHand(unit);
        if (!eq_same && !one_hand_skill) {
            return (false);
        }
    } else if (
            Item_hasType(wpn->item, ITEM_TYPE_ELEMENTAL) ||
            Item_hasType(wpn->item, ITEM_TYPE_ANGELIC)   ||
            Item_hasType(wpn->item, ITEM_TYPE_DEMONIC)
    ) {
        b32 one_hand_skill = Unit_canMagic_oneHand(unit);
        if (!eq_same && !one_hand_skill) {
            return (false);
        }
    }

    // Weapon could be:
    //  - one hand only weapon equipped NOT equipped in other hand
    //  - Any hand weapon equipped in one or two hand

    return (true);
}



/* Is unit among item possible users? */
b32 Unit_canEquip_Users(struct Unit *unit, item_id id) {
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
b32 Unit_canEquip_Type(struct Unit *unit, item_id id) {
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
    item_type wpntypecode = 1;
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
b32 Unit_isWielding(struct Unit *unit, unit_hand hand) {
    if (!Unit_isEquipped(unit, hand))
        return (false);

    item_id id = Unit_Id_Equipment(unit, hand);
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

    enum_eq eq_L = Unit_Eq_Equipped(unit, UNIT_HAND_LEFT);
    enum_eq eq_R = Unit_Eq_Equipped(unit, UNIT_HAND_RIGHT);
    b32 left_canWeakhand    = left  ? Weapon_canWeakhand(eq_L) : true;
    b32 right_canWeakhand   = right ? Weapon_canWeakhand(eq_R) : true;
    unit->isDualWielding    = (left_canWeakhand && right_canWeakhand) && !Unit_istwoHanding(unit);
    return (unit->isDualWielding);
}


/* --- Loadout Manipulation --- */
/* - Does that unit wield a weapon with two hands? - */
// TODO: Tetrabrachios twohanding?
b32 Unit_istwoHanding(Unit *unit) {
    enum_eq eq_left     = Unit_Eq_Equipped(unit, UNIT_HAND_LEFT);
    if ((eq_left  < 0) || (eq_left  >= SOTA_EQUIPMENT_SIZE)) {
        return (false);
    }
    enum_eq eq_right    = Unit_Eq_Equipped(unit, UNIT_HAND_RIGHT);
    if ((eq_right  < 0) && (eq_right  >= SOTA_EQUIPMENT_SIZE)) {
        return (false);
    }
    return (eq_left == eq_right);
}

/* -- Deplete: decrease durability -- */
void _Unit_Item_Deplete(struct Unit *unit, enum_eq eq, item_archetype archetype) {
    /* Only unit function that calls Inventory_item_Deplete */
    item_id id = Unit_Id_Equipment(unit, eq);
    /* Skip if NULL. Not an error, unit can have empty hand. */
    if (id == ITEM_NULL) {
        // SDL_Log("ITEM_NULL");
        return;
    }

    /* Skip if item's archetype to deplete does not match input. */
    if (!(flagsum_isIn(id, archetype))) {
        // SDL_Log("Archetype mismatch");
        return;
    }

    // SDL_Log("Depleting");
    struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, id);
    struct Item   *item   = weapon->item;
    Inventory_item_Deplete(&unit->_equipment[eq], item->stats.uses);
}

void _Unit_Equipped_Deplete(Unit *unit, unit_hand hand, item_archetype archetype) {
    if (!Unit_isEquipped(unit, hand)) {
        return;
    }

    _Unit_Item_Deplete(unit, Unit_Eq_Equipped(unit, hand), archetype);
}

void Unit_Item_Deplete(struct Unit *unit, enum_eq eq) {
    /* Upon use, decrease item durability */
    _Unit_Item_Deplete(unit, eq, ITEM_ARCHETYPE_NULL);
}

void Unit_Equipped_Staff_Deplete(struct Unit *unit, unit_hand hand) {
    /* Upon healing, decrease staff durability */
    _Unit_Equipped_Deplete(unit, hand, ITEM_ARCHETYPE_STAFF);
}

// TODO: Tetrabrachios depletion?
void Unit_Equipped_Weapons_Deplete(struct Unit *unit) {
    /* Upon getting a hit, decrease weapon durability */
    _Unit_Equipped_Deplete(unit, UNIT_HAND_LEFT, ITEM_ARCHETYPE_WEAPON);
    if (!Unit_istwoHanding(unit))
        _Unit_Equipped_Deplete(unit, UNIT_HAND_RIGHT, ITEM_ARCHETYPE_WEAPON);
}

// TODO: Tetrabrachios depletion?
void Unit_Equipped_Shields_Deplete(struct Unit *unit) {
    /* Upon getting hit, decrease shields durability */
    _Unit_Equipped_Deplete(unit, UNIT_HAND_LEFT, ITEM_ARCHETYPE_SHIELD);
    if (!Unit_istwoHanding(unit))
        _Unit_Equipped_Deplete(unit, UNIT_HAND_RIGHT, ITEM_ARCHETYPE_SHIELD);
}

b32 Unit_isEquipped(Unit *unit, unit_hand hand) {
    b32 min_bound = (unit->_equipped[hand] > ITEM_UNEQUIPPED);
    b32 max_bound = (unit->_equipped[hand] <= SOTA_EQUIPMENT_SIZE);
    return (min_bound && max_bound);
}

Inventory_item *Unit_Item_Equipped(Unit *unit, unit_hand hand) {
    if (!Unit_isEquipped(unit, hand))
        return (NULL);

    enum_eq eq = Unit_Eq_Equipped(unit, hand);

    return (&unit->_equipment[eq]);
}

Inventory_item *Unit_InvItem(Unit *unit, enum_eq eq) {
    return (&unit->_equipment[eq]);
}

/* -- Getters -- */
Weapon *Unit_Equipped_Weapon(Unit *unit, unit_hand hand) {
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);

    /* Skipped if not a weapon */
    if (!Unit_isEquipped(unit, hand))
        return (NULL);

    return (Unit_Weapon(unit, hand));
}

Weapon *Unit_Weapon(Unit *unit, enum_eq eq) {
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);

    /* Skipped if not a weapon */
    item_id id = unit->_equipment[eq].id;
    if (!Weapon_ID_isValid(id))
        return (NULL);

    /* Load and return weapon */
    Weapon_Load(unit->weapons_dtab, id);
    Weapon *weapon = DTAB_GET(unit->weapons_dtab, id);
    SDL_assert(weapon != NULL);
    return (weapon);
}

Item *Unit_Get_Item(Unit *unit, enum_eq eq) {
    SDL_assert(unit);
    SDL_assert(unit->items_dtab);

    /* Skipped if not an item */
    item_id id = unit->_equipment[eq].id;
    if (Weapon_ID_isValid(id))
        return (NULL);

    /* Load and return item */
    Item_Load(unit->items_dtab, id);
    struct Item *item = DTAB_GET(unit->items_dtab, id);
    SDL_assert(item != NULL);
    return (item);
}

/* Order in _equipment of equipped weapon */
enum_eq Unit_Eq_Equipped(Unit *unit, enum_eq eq) {
    SDL_assert(unit != NULL);
    SDL_assert(eq >= 0);
    SDL_assert(eq < UNIT_ARMS_NUM);
    SDL_assert(eq < unit->arms_num);
    // ITEM_EQUIPPED_DIFF should only be used here _equipped only accessed here
    return (unit->_equipped[eq] - ITEM_EQUIPPED_DIFF);
}

/* ID of equipment item */
item_id Unit_Id_Equipment(Unit *unit, enum_eq eq) {
    SDL_assert(unit != NULL);
    SDL_assert(eq >= 0);
    SDL_assert(eq < SOTA_EQUIPMENT_SIZE);
    return (unit->_equipment[eq].id);
}

/* ID of equipped weapon */
item_id Unit_Id_Equipped(Unit *unit, unit_hand hand) {
    SDL_assert(unit != NULL);
    SDL_assert((hand == UNIT_HAND_LEFT) || (hand == UNIT_HAND_RIGHT));
    return (unit->_equipment[Unit_Eq_Equipped(unit, hand)].id);
}

/* -- Use -- */
void Unit_Staff_Use(Unit *healer, Unit *patient) {
    /* Should not be called if staves not properly equipped:
    *       - Staff in strong hand
    *       - Staff in weakhand IF not onehand skill
    */

    /* Get equipped weapon id */
    unit_hand stronghand  = Unit_Hand_Strong(healer);
    unit_hand weakhand    = 1 - stronghand;
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
