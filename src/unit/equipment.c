
#include "unit/equipment.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/loadout.h"
#include "inventory.h"
#include "macros.h"
#include "item.h"
#include "weapon.h"
#include "globals.h"
#include "nmath.h"

tnecs_entity *Unit_Equipment(Unit *unit) {
    return (unit->equipment._arr);
}

i32 *Unit_canEquip_Arr(Unit *unit) {
    return (unit->can_equip.arr);
}

/* --- Items --- */
/* Private item atker at specific spot. Does no checks */
void _Unit_Item_Takeat(Unit         *unit,
                       tnecs_entity  entity,
                       i32           eq) {

    unit->equipment._arr[eq - ITEM1] = entity;
}

/* Take item at specific spot */
void Unit_Item_Takeat(Unit          *unit,
                      tnecs_entity   entity,
                      i32            eq) {
    SDL_assert(unit);
    SDL_assert(gl_weapons_dtab  != NULL);
    SDL_assert(gl_items_dtab    != NULL);

    if (entity <= TNECS_NULL) {
        return;
    }
    Inventory_item *item = IES_GET_C(gl_world, entity,  Inventory_item);
    SDL_assert(item != NULL);

    if (Weapon_ID_isValid(item->id)) {
        Weapon_Load(gl_weapons_dtab, item->id);
    } else if (Item_ID_isValid(item->id)) {
        Item_Load(gl_items_dtab, item->id);
    } else {
        return;
    }

    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    SDL_assert(Unit_InvItem_Entity(unit, eq) <= TNECS_NULL);

    _Unit_Item_Takeat(unit, entity, eq);
    unit->equipment.num++;
}

void Unit_Item_Take(Unit *unit, tnecs_entity entity) {
    SDL_assert(unit);
    SDL_assert(unit->equipment.num < SOTA_EQUIPMENT_SIZE);
    SDL_assert(entity > TNECS_NULL);

    for (i32 eq = ITEM1; eq <= ITEM6; eq++) {
        if (Unit_InvItem_Entity(unit, eq) == TNECS_NULL) {
            Unit_Item_Takeat(unit, entity, eq);
            break;
        }
    }
}

void Unit_Equipment_Drop(Unit *unit) {
    for (i32 eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; eq++) {
        Unit_Item_Drop(unit, eq);
    }
}

tnecs_entity Unit_Item_Drop(Unit *unit, i32 eq) {
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);

    tnecs_entity out = Unit_InvItem_Entity(unit, eq);
    _Unit_Item_Takeat(unit, TNECS_NULL, eq);
    if (unit->equipment.num > 0)
        unit->equipment.num--;
    else
        unit->equipment.num = 0;

    return (out);
}

void Unit_Item_Swap(Unit *unit, i32 i1, i32 i2) {
    SDL_assert(unit);

    if (i1 == i2)
        return;

    b32 i1_valid = (i1 >= ITEM1) && (i1 < SOTA_EQUIPMENT_SIZE);
    b32 i2_valid = (i2 >= ITEM1) && (i2 < SOTA_EQUIPMENT_SIZE);
    if (i1_valid && i2_valid) {
        Equipment_Swap(unit->equipment._arr, i1, i2);
    } else {
        SDL_Log("Invalid item swapping index %d %d", i1, i2);
        SDL_assert(false);
    }
}

void Unit_Item_Trade(Unit   *giver,  Unit *taker,
                     i32     eq_g,   i32   eq_t) {
    SDL_assert(giver);
    SDL_assert(taker);
    SDL_assert(eq_g >= ITEM1);
    SDL_assert(eq_g < SOTA_EQUIPMENT_SIZE);
    SDL_assert(eq_t >= ITEM1);
    SDL_assert(eq_t < SOTA_EQUIPMENT_SIZE);

    tnecs_entity giver_ent = Unit_Item_Drop(giver, eq_g);
    tnecs_entity taker_ent = Unit_Item_Drop(taker, eq_t);
    Unit_Item_Takeat(taker, giver_ent, eq_t);
    Unit_Item_Takeat(giver, taker_ent, eq_g);
}

/* Checking equipped for errors broken items */
void _Unit_Check_Equipped(Unit *unit, i32 hand) {
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

void Unit_Check_Equipped(Unit *unit) {
    /* Checks if equipped item is BORKED,
    ** de-equips if so */
    for (i32 hand = UNIT_HAND_LEFT;
         hand <= unit->arms.num;
         hand++
        ) {
        if (Unit_hasHand(unit, hand)) {
            _Unit_Check_Equipped(unit, hand);
        }
    }
}

void Unit_Equip(Unit *unit, i32 hand, i32 eq) {
    /* Note: Does NOT check if item can be equipped */
    SDL_assert(unit);
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(hand < (MAX_ARMS_NUM  + UNIT_HAND_LEFT));
    SDL_assert(hand < (UNIT_ARMS_NUM + UNIT_HAND_LEFT));

    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);

    i32 id = Unit_Id_Equipment(unit, eq);

    if (id <= ITEM_NULL) {
        /* Can't equip broken empty equipment. */
        /* Everything else can be equipped. */
        return;
    }

    i32 *equipped = Unit_Equipped_Array(unit);
    equipped[hand - UNIT_HAND_LEFT] = eq;
}

void Unit_Unequip_All(Unit *unit) {
    SDL_assert(unit);
    i32 *equipped   = Unit_Equipped_Array(unit);
    i64 bytesize    = unit->arms.num * sizeof(*equipped);
    memset(equipped, 0, bytesize);
}

void Unit_Unequip(Unit *unit, i32 hand) {
    SDL_assert(unit);
    SDL_assert(hand >= 0);
    SDL_assert(hand <= MAX_ARMS_NUM);

    /* -- Unequip -- */
    i32 *equipped = Unit_Equipped_Array(unit);
    equipped[hand - UNIT_HAND_LEFT] = ITEM_UNEQUIPPED;

    if (unit->arms.num == UNIT_ARMS_NUM) {
        /* -- If twohanding, not anymore! -- */
        SDL_assert(Unit_istwoHanding(unit) == false);

        /* -- If dual wielding, not anymore! -- */
        SDL_assert(Unit_isdualWielding(unit) == false);
    }
    /* TODO: Tetrabrachios */
}

b32 Unit_canEquip_AnyHand(Unit *unit, canEquip can_equip) {
    SDL_assert(unit != NULL);
    /* Check that unit has hands to equip with */
    b32 found = false;
    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms.num; hand++) {
        if (Unit_hasHand(unit, hand)) {
            found = true;
            break;
        }
    }

    if (!found) {
        return (false);
    }

    /* Check that unit can equip item in hand */
    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms.num; hand++) {
        // SDL_Log("hand %d", hand);
        /* Skip hands that unit doesn't have */
        if (!Unit_hasHand(unit, hand)) {
            continue;
        }
        can_equip.hand = hand;

        /* AnyHand can equip: true */
        if (Unit_canEquip(unit, can_equip)) {
            return (true);
        }
    }

    return (false);
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

    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms.num; hand++) {
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

    unit->can_equip.num = 0;
    for (i32 eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; eq++) {
        for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms.num; hand++) {
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
                unit->can_equip.arr[unit->can_equip.num++] = eq;
                break;
            }
            can_equip.hand  = input_hand;
        }
    }

    /* Restore starting equipment */
    Unit_Equipped_Import(unit, start_equipped);
}

/* Check if canEquip, with current loadout */
b32 _Unit_canEquip(Unit *unit, canEquip can_equip) {
    SDL_assert(unit != NULL);

    SDL_assert(can_equip._eq >= ITEM_UNEQUIPPED);
    SDL_assert(can_equip._eq <= SOTA_EQUIPMENT_SIZE);
    /* --- Can't equip non-equipment item ---  */
    if (can_equip._eq == ITEM_UNEQUIPPED) {
        // SDL_Log("Can't equip nothing \n");
        return (false);
    }

    /* --- Can't equip if unit doesn't have hand ---  */
    if (!Unit_hasHand(unit, can_equip.hand)) {
        // SDL_Log("No hand \n");
        return (false);
    }

    i32 eq = can_equip._eq;
    i32 id = Unit_Id_Equipment(unit, eq);

    /* --- Can't equip non-existant item ---  */
    if (id <= ITEM_NULL) {
        // SDL_Log("ITEM_NULL\n");
        return (false);
    }

    /* --- Can't equip non-existant item ---  */
    if (!Unit_canEquip_Type(unit, id)) {
        // SDL_Log("!Unit_canEquip_Type\n");
        return (false);
    }

    /* --- Item is not part of equippable types ---  */
    if (!Unit_canEquip_Archetype(unit, id, can_equip.archetype)) {
        // SDL_Log("!Unit_canEquip_Archetype\n");
        return (false);
    }

    /* --- Unit is not part of item users ---  */
    if (!Unit_canEquip_Users(unit, id)) {
        // SDL_Log("!Unit_canEquip_Users\n");
        return (false);
    }

    /* --- Unit can't equip weapon in one hand --- */
    if (!Unit_canEquip_OneHand(unit, eq, can_equip.hand, can_equip.two_hands_mode)) {
        // SDL_Log("!Unit_canEquip_OneHand\n");
        return (false);
    }

    /* --- Unit can't equip weapon in two hands --- */
    if (!Unit_canEquip_TwoHand(unit, eq, can_equip.hand, can_equip.two_hands_mode)) {
        // SDL_Log("!Unit_canEquip_TwoHand\n");
        return (false);
    }

    // SDL_Log("canEquip\n");
    return (true);
}

/* Check if canEquip, with can_equip._loadout equipped */
b32 Unit_canEquip(Unit *unit, canEquip can_equip) {
    SDL_assert(unit != NULL);

    /* Save starting equipment */
    i32 start_equipped[MAX_ARMS_NUM];
    Unit_Equipped_Export(unit, start_equipped);

    /* Equip loadout */
    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms.num; hand++) {
        int eq = can_equip._loadout[hand - ITEM1];
        if (!eq_valid(eq) ) {
            continue;
        }

        i32 id = Unit_Id_Equipment(unit, eq);

        // Note: All items can be equipped
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
    SDL_assert(gl_weapons_dtab   != NULL);

    if (archetype == ITEM_ARCHETYPE_NULL) {
        return (true);
    }

    if (archetype == ITEM_ARCHETYPE_ITEM) {
        return (true);
    }

    Weapon_Load(gl_weapons_dtab, id);
    const Weapon *wpn = DTAB_GET_CONST(gl_weapons_dtab, id);
    SDL_assert(wpn != NULL);

    if (!flagsum_isIn(wpn->item.type.top, archetype)) {
        return (false);
    }

    return (true);
}

/* IF equipment can be two-handed, CAN the unit equip it? */
/* TODO: Tetrabrachios ? */
b32 Unit_canEquip_TwoHand(Unit *unit, i32 eq, i32 hand, i32 mode) {
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    SDL_assert(unit                 != NULL);
    SDL_assert(gl_weapons_dtab   != NULL);

    i32 id = Unit_Id_Equipment(unit, eq);
    if (id <= ITEM_NULL) {
        return (false);
    }
    if (!Weapon_ID_isValid(id)) {
        return (false);
    }

    const Weapon *wpn  = DTAB_GET_CONST(gl_weapons_dtab, id);

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
    /* Weapon CAN be equipped in two hands. Could be:
    **  - 2H only weapon equipped in other hand (strict)
    **  - 2H only weapon NOT equipped in other hand (!strict)
    **  - AH weapon equipped in one or two hand
    */

    return (true);
}

/* IF equipment can be one-handed, CAN the unit equip it? */
b32 Unit_canEquip_OneHand(Unit *unit, i32 eq, i32 hand, i32 mode) {
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    SDL_assert(unit             != NULL);
    SDL_assert(gl_weapons_dtab  != NULL);

    i32 id = Unit_Id_Equipment(unit, eq);
    if (id <= ITEM_NULL)
        return (false);
    if (!Weapon_ID_isValid(id))
        return (false);

    const Weapon *wpn = DTAB_GET_CONST(gl_weapons_dtab, id);
    SDL_assert(wpn != NULL);

    /* -- left hand wpn in right hand -- */
    if ((Weapon_Handedness(wpn) == WEAPON_HAND_LEFT) && (hand == UNIT_HAND_RIGHT))
        return (false);

    /* -- right hand wpn in left hand -- */
    if ((Weapon_Handedness(wpn) == WEAPON_HAND_RIGHT) && (hand == UNIT_HAND_LEFT))
        return (false);

    /* -- Failure: Trying to twohand a onehand only wpn -- */
    b32 other_hand      = UNIT_OTHER_HAND(hand);
    i32 eq_other        = Unit_Eq_Equipped(unit, other_hand);

    /* One-hand only wpn can't be equipped if:
    **      - Other hand equipped same wpn. */
    b32 eq_same         = (eq_other == eq);
    b32 eq_in_bound     = (eq_other >= ITEM1) && (eq_other <= SOTA_EQUIPMENT_SIZE);

    b32 one_hand_only   = Weapon_OneHand_Only(wpn);
    b32 one_hand_cant   = one_hand_only && (eq_in_bound && eq_same);

    if (one_hand_cant) {
        /* SDL_Log("one_hand_cant"); */
        /* SDL_Log("%d %d %d", one_hand_only, eq_in_bound, eq_same); */
        /* SDL_Log("hand, other hand, %d %d", hand, other_hand); */
        return (false);
    }

    b32 strict = (mode == TWO_HAND_EQ_MODE_STRICT);
    /* SDL_Log("mode %d %d", mode, TWO_HAND_EQ_MODE_STRICT); */
    /* Cannot onehand magic weapons/staves */
    if (Item_hasType(&wpn->item, ITEM_TYPE_STAFF)) {
        b32 one_hand_skill = Unit_canStaff_oneHand(unit);
        if (strict && !eq_same && !one_hand_skill) {
            /* SDL_Log("Cannot onehand staves %d %d %d", strict, eq_same, one_hand_skill); */
            return (false);
        }
    } else if (
            Item_hasType(&wpn->item, ITEM_TYPE_ELEMENTAL) ||
            Item_hasType(&wpn->item, ITEM_TYPE_ANGELIC)   ||
            Item_hasType(&wpn->item, ITEM_TYPE_DEMONIC)
    ) {
        b32 one_hand_skill = Unit_canMagic_oneHand(unit);
        if (strict && !eq_same && !one_hand_skill) {
            /* SDL_Log("Cannot onehand magic weapons"); */
            return (false);
        }
    }
    /* Weapon CAN be equipped in one hand. Could be:
    **  - 1H weapon NOT currently equipped in other hand
    **  - AH weapon equipped in one or two hand
    */
    return (true);
}

/* Is unit among item possible users? */
b32 Unit_canEquip_Users(Unit *unit, i32 id) {
    SDL_assert(unit                 != NULL);
    SDL_assert(gl_weapons_dtab   != NULL);

    Weapon_Load(gl_weapons_dtab, id);
    const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, id);

    /* Can equip if no list of users */
    if (weapon->item.users.id == NULL) {
        return (true);
    }

    if (DARR_NUM(weapon->item.users.id) == 0) {
        return (true);
    }

    for (i32 u = 0; u < DARR_NUM(weapon->item.users.id); u++) {
        if (weapon->item.users.id[u] == Unit_id(unit))
            return (true);
    }
    return (false);
}

/* Can unit equip arbitrary weapon with a certain type? */
b32 Unit_canEquip_Type(Unit *unit, i32 id) {
    /* Unequippable if ITEM_NULL */
    if (id <= ITEM_NULL) {
        return (false);
    }

    if (!Weapon_ID_isValid(id)) {
        return (false);
    }

    SDL_assert(gl_weapons_dtab != NULL);
    Weapon_Load(gl_weapons_dtab, id);
    const Weapon *weapon   = DTAB_GET_CONST(gl_weapons_dtab, id);
    u16 wpntypecode         = weapon->item.type.top;
    SDL_assert(wpntypecode);

    /* Is weapon's type equippable by unit? */
    return ((unit->flags.equippable & wpntypecode) > 0);
}

/* Find all CanEquip types, put them in equippables array */
// TODO better types
u8 Unit_canEquip_allTypes(Unit *unit, u8 *equippables) {
    u8 type = 1, equippable_num = 0;
    i64 wpntypecode = 1;
    memset(equippables, 0, ITEM_TYPE_EXP_END * sizeof(*equippables));
    while ((wpntypecode < ITEM_TYPE_END) & (type < ITEM_TYPE_EXP_END)) {
        if ((unit->flags.equippable & wpntypecode) > 0)
            equippables[equippable_num++] = type;
        type++;
        wpntypecode *= 2;
    }
    return (equippable_num);
}

/* Is a unit wielding a weapon in its hand? Note: Units can equip staves.
    -> Equipped + a weapon (not a staff, or offhand, or trinket...)
 */
b32 Unit_isWielding(Unit *unit, i32 hand) {
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
b32 Unit_isdualWielding(Unit *unit) {
    SDL_assert(unit);
    // TODO: test this function

    /* To be dual wielding unit must: */
    // - Wield a weapon in weakhand
    // - Wield a weapon in stronghand
    b32 left            = Unit_isWielding(unit, UNIT_HAND_LEFT);
    b32 right           = Unit_isWielding(unit, UNIT_HAND_RIGHT);

    i32 eq_L = Unit_Eq_Equipped(unit, UNIT_HAND_LEFT);
    i32 eq_R = Unit_Eq_Equipped(unit, UNIT_HAND_RIGHT);
    b32 right_canWeakhand   = right ? Weapon_canWeakhand(eq_R) : false;
    b32 left_canWeakhand    = left  ? Weapon_canWeakhand(eq_L) : false;
    return ((left_canWeakhand && right_canWeakhand) && !Unit_istwoHanding(unit));
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
void _Unit_Item_Deplete(Unit *unit, i32 eq, i64 archetype) {
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
    SDL_assert(gl_weapons_dtab != NULL);
    Weapon *weapon = DTAB_GET(gl_weapons_dtab, id);
    Item   *item   = &weapon->item;
    SDL_assert(weapon != NULL);
    SDL_assert(item != NULL);
    if (!(flagsum_isIn(item->type.top, archetype))) {
        // SDL_Log("Archetype mismatch");
        return;
    }

    // SDL_Log("Depleting");
    tnecs_entity ent    = Unit_InvItem_Entity(unit, eq);
    Inventory_item *invitem = IES_GET_C(gl_world, ent,  Inventory_item);

    Inventory_item_Deplete(invitem, item);
}

void _Unit_Equipped_Deplete(Unit *unit, i32 hand, i64 archetype) {
    if (!Unit_isEquipped(unit, hand)) {
        return;
    }

    _Unit_Item_Deplete(unit, Unit_Eq_Equipped(unit, hand), archetype);
}

void Unit_Item_Deplete(Unit *unit, i32 eq) {
    /* Upon use, decrease item durability */
    _Unit_Item_Deplete(unit, eq, ITEM_ARCHETYPE_NULL);
}

void Unit_Equipped_Staff_Deplete(Unit *unit, i32 hand) {
    /* Upon healing, decrease staff durability */
    _Unit_Equipped_Deplete(unit, hand, ITEM_ARCHETYPE_STAFF);
}

// TODO: Tetrabrachios depletion?
void Unit_Equipped_Weapons_Deplete(Unit *unit) {
    // SDL_Log("Unit_Equipped_Weapons_Deplete");
    /* Upon getting a hit, decrease weapon durability */
    _Unit_Equipped_Deplete(unit, UNIT_HAND_LEFT, ITEM_ARCHETYPE_WEAPON);
    if (!Unit_istwoHanding(unit))
        _Unit_Equipped_Deplete(unit, UNIT_HAND_RIGHT, ITEM_ARCHETYPE_WEAPON);
}

// TODO: Tetrabrachios depletion?
void Unit_Equipped_Shields_Deplete(Unit *unit) {
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
    tnecs_entity ent = Unit_InvItem_Entity(unit, eq);
    Inventory_item *item  = IES_GET_C(gl_world, ent,  Inventory_item);

    return (item);
}

Inventory_item *Unit_InvItem(Unit *unit, i32 eq) {
    tnecs_entity ent = Unit_InvItem_Entity(unit, eq);
    Inventory_item *item  = IES_GET_C(gl_world, ent,  Inventory_item);
    return (item);
}

tnecs_entity Unit_InvItem_Entity(Unit *unit, i32 eq) {
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    return (unit->equipment._arr[eq - ITEM1]);
}

/* -- Getters -- */
const Weapon *Unit_Equipped_Weapon(Unit *unit, i32 hand) {
    SDL_assert(unit);
    SDL_assert(gl_weapons_dtab);

    /* Skipped if not a weapon */
    if (!Unit_isEquipped(unit, hand))
        return (NULL);

    return (Unit_Weapon(unit, hand));
}

const Weapon *Unit_Weapon(Unit *unit, i32 eq) {
    SDL_assert(unit);
    SDL_assert(gl_weapons_dtab);

    /* Skipped if not a weapon */
    tnecs_entity    ent     = Unit_InvItem_Entity(unit, eq);
    Inventory_item *item    = IES_GET_C(gl_world, ent,  Inventory_item);
    i32 id = item->id;
    if (!Weapon_ID_isValid(id))
        return (NULL);

    /* Load and return weapon */
    Weapon_Load(gl_weapons_dtab, id);
    const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, id);
    SDL_assert(weapon != NULL);
    return (weapon);
}

const Item *Unit_Get_Item(Unit *unit, i32 eq) {
    SDL_assert(unit);
    SDL_assert(gl_items_dtab);

    /* Skipped if not an item */
    tnecs_entity    ent     = Unit_InvItem_Entity(unit, eq);
    Inventory_item *invitem    = IES_GET_C(gl_world, ent,  Inventory_item);

    // i32 id = unit->equipment.arr[eq - ITEM1].id;
    i32 id = invitem->id;
    if (Weapon_ID_isValid(id))
        return (NULL);

    /* Load and return item */
    Item_Load(gl_items_dtab, id);
    const Item *item = DTAB_GET_CONST(gl_items_dtab, id);
    SDL_assert(item != NULL);
    return (item);
}

/* Order in eq in eq_canEquip equipped weapon
- eq_valid(order) is false if NOT in eq_canEquip
 */
i32 Unit_Order_canEquip(const Unit *const unit, i32 eq) {
    SDL_assert(unit != NULL);
    SDL_assert(eq_valid(eq));
    for (int i = 0; i < unit->can_equip.num; i++) {
        if (eq == unit->can_equip.arr[i]) {
            return (i);
        }
    }
    return (SOTA_EQUIPMENT_SIZE);
}

/* eq of _equipped weapon in hand */
i32 Unit_Eq_Equipped(const Unit *const unit, i32 hand) {
    SDL_assert(unit != NULL);
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(unit->arms.num > 0);
    SDL_assert(unit->arms.num <= MAX_ARMS_NUM);
    SDL_assert(hand < (UNIT_ARMS_NUM + UNIT_HAND_LEFT));
    SDL_assert(hand < (unit->arms.num + UNIT_HAND_LEFT));

    i32 *equipped = Unit_Equipped_Array(unit);
    return (equipped[hand - UNIT_HAND_LEFT]);
}

/* ID of equipment item */
i32 Unit_Id_Equipment(Unit *unit, i32 eq) {
    SDL_assert(gl_world != NULL);
    SDL_assert(unit     != NULL);
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    tnecs_entity    ent     = Unit_InvItem_Entity(unit, eq);

    if (TNECS_NULL == ent) {
        // SDL_Log("TNECS_NULL");
        return (ITEM_NULL);
    }
    Inventory_item *item    = IES_GET_C(gl_world, ent,  Inventory_item);
    if (NULL == item) {
        // SDL_Log("ITEM_NULL");
        return (ITEM_NULL);
    }
    return (item->id);
}

/* ID of equipped weapon */
i32 Unit_Id_Equipped(Unit *unit, i32 hand) {
    if (unit == NULL) {
        SDL_assert(false);
        return (0);
    }
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(hand <= unit->arms.num);

    if (!Unit_isEquipped(unit, hand)) {
        return (ITEM_NULL);
    }
    i32 eq = Unit_Eq_Equipped(unit, hand);
    tnecs_entity    ent     = Unit_InvItem_Entity(unit, eq);
    Inventory_item *item    = IES_GET_C(gl_world, ent,  Inventory_item);

    return (item->id);
}

i32* Unit_Equipped_Array(const Unit *const unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (NULL);
    }
    return ((i32*)&unit->equipment._equipped._loadout);
}

void Unit_Id_Equipped_Set( Unit *unit, i32 hand, i32 eq) {
    if (unit == NULL) {
        SDL_assert(false);
        return;
    }
    i32 *equipped = Unit_Equipped_Array(unit);
    equipped[hand - UNIT_HAND_LEFT] = eq;
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

    Inventory_item *weakhand_inv   = Unit_Item_Equipped(healer, weakhand);
    Inventory_item *stronghand_inv = Unit_Item_Equipped(healer, stronghand);
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
    SDL_assert(gl_weapons_dtab);
    const Weapon *staff = DTAB_GET_CONST(gl_weapons_dtab, stronghand_inv->id);
    SDL_assert(flagsum_isIn(staff->item.type.top, ITEM_TYPE_STAFF));
    SDL_assert(staff->item.effect.active != ITEM_EFFECT_NULL);

    /* Use staff active */
    use_function_t active_func = item_effect_funcs[staff->item.effect.active];
    active_func(&staff->item, healer, patient);

    /* Deplete staff */
    Unit_Equipped_Staff_Deplete(healer, stronghand);
}
