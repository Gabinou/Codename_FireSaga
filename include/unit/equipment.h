#ifndef UNIT_EQUIPMENT_H
#define UNIT_EQUIPMENT_H
/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** Unit equipment management:
**  - Equipping, trading, dropping, depleting...
**
** --- Glossary ---
**  - Loadout:   weapons arr that **could** be equipped
**  - Equipped:  weapons arr **currently** equipped
**  - *eq* is between [ITEM1, SOTA_EQUIPMENT_SIZE + ITEM1]
**      -
*/


#include "types.h"
#include "enums.h"
#include "tnecs.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Range;
struct Weapon;

/* --- Inventory Items --- */
Inventory_item *Unit_InvItem(       Unit *u, i32 eq);
Inventory_item *Unit_Item_Equipped( Unit *u, i32 hand);
tnecs_E         Unit_InvItem_Entity(Unit *u, i32 eq);

/* -- Deplete: decrease durability -- */
void _Unit_Item_Deplete(            Unit *u, i32 eq, i64 a);
void _Unit_Equipped_Deplete(        Unit *u, i32 h,  i64 a);

void Unit_Item_Deplete(             Unit *u, i32 eq);
void Unit_Equipped_Staff_Deplete(   Unit *u, i32 h);
void Unit_Equipped_Weapons_Deplete( Unit *u);
void Unit_Equipped_Shields_Deplete( Unit *u);

/* -- Trading -- */
void  Unit_Item_Take(  Unit *u, tnecs_E item);
void  Unit_Item_Trade( Unit *g, Unit *t, i32 giver_i, i32 taker_i);
void  Unit_Item_Takeat(Unit *u, tnecs_E i, i32 j);
void _Unit_Item_Takeat(Unit *u, tnecs_E i, i32 j);

/* -- Swapping -- */
void Unit_Item_Swap(  Unit *u, i32 ind1, i32 ind2);

/* -- Dropping -- */
/* Drop item at input index */
tnecs_E Unit_Item_Drop(Unit *u, i32 i);

/* Drop all items in equipment */
void Unit_Equipment_Drop(Unit *u);

/* -- Getters -- */
const struct Weapon *Unit_Weapon(   Unit *u, i32 eq);
const struct Item   *Unit_Eq_Item(  Unit *u, i32 eq);
/* Get pointer to equipped item.
**  If item or nothing equipped, NULL output */
const struct Weapon *Unit_Equipped_Weapon(Unit *u, i32 hand);

/* ID of equipped weapon */
i32  Unit_Id_Equipped( Unit *u, i32 hand);
i32  Unit_Id_Equipment(Unit *u, i32 eq);
void Unit_Id_Equipped_Set( Unit *u, i32 hand, i32 eq);
i32* Unit_Equipped_Array(const Unit *const unit);

/* Order in _equipment of equipped weapon */
i32 Unit_Eq_Equipped(   const Unit *const unit, i32 h);
i32 Unit_Order_canEquip(const struct Unit_Equippable *const equippable, i32 eq);

/* --- Equipping --- */
void    Unit_Equip(     Unit *u, i32 h, i32 eq);
void    Unit_Unequip(   Unit *u, i32 h);
void    Unit_Unequip_All(Unit *u);
b32     Unit_isEquipped(Unit *u, i32 h);
b32     Unit_anyWeapon_Equipped(Unit *u, i32 h);

/* - Base canEquips - */
// TODO: tetrabrachios can_equip?
b32  Unit_canEquip_Type(    Unit *u,    i32 id);
b32  Unit_canEquip_Range(   i32 id, struct Range *r, i32 mode);
b32  Unit_canEquip_Users(   Unit *u,    i32 id);
b32  Unit_canEquip_OneHand( Unit *u,    i32 eq, i32 h, i32 mode);
b32  Unit_canEquip_TwoHand( Unit *u,    i32 eq, i32 h, i32 mode);

b32 Unit_canEquip_Archetype(i32 id, i64 archetype);

u8  Unit_canEquip_allTypes( Unit *u,    u8 *eq);

/* - Combined canEquips - */
b32  _Unit_canEquip(          Unit *u, canEquip e);
b32   Unit_canEquip(          Unit *u, canEquip e);
b32   Unit_canEquip_AnyHand(  Unit *u, canEquip e);
struct Unit_Equippable Unit_canEquip_Equipment(Unit *u, canEquip e);

/* --- Checking Equipment: de-equip if broken --- */
void _Unit_Check_Equipped(Unit *u, i32 hand);
void  Unit_Check_Equipped(Unit *u);

/* --- Unit state --- */
b32 Unit_istwoHanding(  Unit *u);
b32 Unit_isdualWielding(Unit *u);
b32 Unit_iswrongHanding(Unit *u);

/* --- Debug --- */
i32  Unit_Equipment_Num(    Unit *u);
b32  Unit_Equipment_isFull( Unit *u);
void Unit_Equipment_Print(  Unit *u);

tnecs_E *Unit_Equipment(Unit *u);

/* -- Use -- */
void Unit_Staff_Use(Unit *unit, Unit *patient);

#endif /* UNIT_EQUIPMENT_H */
