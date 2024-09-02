#ifndef UNIT_EQUIPMENT_H
#define UNIT_EQUIPMENT_H

#include "types.h"
#include "enums.h"
#include "item.h"
#include "weapon.h"
#include "unit/unit.h"
#include "structs.h"
#include "unit/unit.h"

/* --- Items --- */
Inventory_item *Unit_InvItem(       Unit *u, i32 i);
Inventory_item *Unit_Item_Equipped( Unit *u, b32 hand);

/* -- Deplete: decrease durability -- */
void _Unit_Item_Deplete(           Unit *u, i32  i, u64 a);
void _Unit_Equipped_Deplete(       Unit *u, b32 h, u64 a);

void Unit_Item_Deplete(            Unit *u, i32 i);
void Unit_Equipped_Staff_Deplete(  Unit *u, b32 h);
void Unit_Equipped_Weapons_Deplete(Unit *u);
void Unit_Equipped_Shields_Deplete(Unit *u);

/* -- Trading -- */
void  Unit_Item_Take(  Unit *u, Inventory_item item);
void  Unit_Item_Trade( Unit *g, Unit *t, i16 giver_i, i16 taker_i);
void  Unit_Item_Takeat(Unit *u, Inventory_item i, size_t j);
void _Unit_Item_Takeat(Unit *u, Inventory_item i, size_t j);

/* -- Swapping -- */
void Unit_Item_Swap(  Unit *u, i16 ind1, i16 ind2);

/* -- Dropping -- */
Inventory_item Unit_Item_Drop(Unit *u, i16 ind1);
void Unit_Equipment_Drop(Unit *u);
/* TODO: Remove. No swapping -> no need to export equipment*/

/* -- Importing/Exporting -- */
void Unit_Equipment_Import(Unit *u, Inventory_item *equipment);
void Unit_Equipment_Export(Unit *u, Inventory_item *equipment);

void Unit_Equipped_Import(Unit *u, i32 *equipped);
void Unit_Equipped_Export(Unit *u, i32 *equipped);

/* -- Getters -- */
Weapon *Unit_Weapon(         Unit *u, i32 eq);
Item   *Unit_Get_Item(       Unit *u, i32 eq);
Weapon *Unit_Equipped_Weapon(Unit *u, b32 hand);

/* ID of equipped weapon */
i32 Unit_Id_Equipped(Unit *u, i32 hand);
/* Order in _equipment of equipped weapon */
i32 Unit_Eq_Equipped(Unit *u, i32 hand);

/* --- Equipping --- */
i32     Unit_Equipped(  Unit *u, b32 hand);
b32     Unit_isEquipped(Unit *u, b32 hand);
i32     Unit_Equip(  Unit *u, b32 h, i32 i);
void    Unit_Unequip(Unit *u, b32 h);

/* -- Can Equip -- */
b32 Unit_canEquip(       Unit *u, i16 id);
u8  Unit_canEquip_Types(    Unit *u, u8 *eq);
b32 Unit_canEquip_Type(  Unit *u, i16 id);
b32 Unit_canEquip_Hand(  Unit *u, i16 id, b32 h);
b32 Unit_canEquip_inHand(Unit *u, b32 h);

/* --- Checking Equipment: de-equip if broken --- */
void _Unit_Check_Equipped(Unit *u, b32 hand);
void  Unit_Check_Equipped(Unit *u);

/* --- Usability --- */
b32  Unit_Eq_Usable(  Unit *u, u64 a, i32 i);
b32  Unit_All_Usable( Unit *u);
void Unit_Find_Usable(Unit *u, u64 a);
b32  Unit_Item_Usable(Unit *u, u64 a, i32 i);

/* --- Unit state --- */
b32 Unit_istwoHanding(  Unit *u);
b32 Unit_isdualWielding(Unit *u);
b32 Unit_iswrongHanding(Unit *u);

/* --- Debug --- */
b32  Unit_Equipment_Full( Unit *u);
void Unit_Equipment_Print(Unit *u);

/* -- Can -- */
b32 Unit_canTwoHand(Unit *u, i32 i);

/* -- Use -- */
void Unit_Staff_Use(Unit *unit, Unit *patient);

#endif /* UNIT_EQUIPMENT_H */
