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
Inventory_item *Unit_InvItem(       Unit *u, enum_item eq);
Inventory_item *Unit_Item_Equipped( Unit *u, unit_hand hand);

/* -- Deplete: decrease durability -- */
void _Unit_Item_Deplete(           Unit *u, enum_item eq, item_archetype a);
void _Unit_Equipped_Deplete(       Unit *u, unit_hand h,  item_archetype a);

void Unit_Item_Deplete(            Unit *u, enum_item eq);
void Unit_Equipped_Staff_Deplete(  Unit *u, unit_hand h);
void Unit_Equipped_Weapons_Deplete(Unit *u);
void Unit_Equipped_Shields_Deplete(Unit *u);

/* -- Trading -- */
void  Unit_Item_Take(  Unit *u, Inventory_item item);
void  Unit_Item_Trade( Unit *g, Unit *t, enum_item giver_i, enum_item taker_i);
void  Unit_Item_Takeat(Unit *u, Inventory_item i, enum_item j);
void _Unit_Item_Takeat(Unit *u, Inventory_item i, enum_item j);

/* -- Swapping -- */
void Unit_Item_Swap(  Unit *u, enum_item ind1, enum_item ind2);

/* -- Dropping -- */
Inventory_item Unit_Item_Drop(Unit *u, enum_item ind1);
void Unit_Equipment_Drop(Unit *u);
/* TODO: Remove. No swapping -> no need to export equipment*/

/* -- Importing/Exporting -- */
void Unit_Equipment_Import(Unit *u, Inventory_item *equipment);
void Unit_Equipment_Export(Unit *u, Inventory_item *equipment);

void Unit_Equipped_Import(Unit *u, enum_equipped *equipped);
void Unit_Equipped_Export(Unit *u, enum_equipped *equipped);

/* -- Getters -- */
Weapon *Unit_Weapon(         Unit *u, enum_item eq);
Item   *Unit_Get_Item(       Unit *u, enum_item eq);
Weapon *Unit_Equipped_Weapon(Unit *u, unit_hand hand);

/* ID of equipped weapon */
i32 Unit_Id_Equipped( Unit *u, unit_hand hand);
i32 Unit_Id_Equipment(Unit *u, enum_item eq);

/* Order in _equipment of equipped weapon */
i32 Unit_Eq_Equipped(Unit *u, unit_hand hand);

/* --- Equipping --- */
void    Unit_Equip(     Unit *u, unit_hand h, enum_item eq);
void    Unit_Unequip(   Unit *u, unit_hand h);
b32     Unit_isEquipped(Unit *u, unit_hand h);

/* - canEquips setters - */
void canEquip_Eq(           canEquip *e, enum_item eq);
void canEquip_Loadout(      canEquip *e, unit_hand h, enum_item eq);
void canEquip_Loadout_None( canEquip *e, unit_hand h);

/* - Base canEquips - */
// TODO: tetrabrachios canequip?
// only if playable!
b32  Unit_canEquip_Type(    Unit *u,    item_id id);
b32  Unit_canEquip_Users(   Unit *u,    item_id id);
b32  Unit_canEquip_OneHand( Unit *u,    enum_item eq, unit_hand h);
b32  Unit_canEquip_TwoHand( Unit *u,    enum_item eq, unit_hand h, i32 mode);

b32 Unit_canEquip_Archetype(Unit *u,    enum_item eq, item_archetype archetype);

u8  Unit_canEquip_allTypes( Unit *u,    u8 *eq);

/* - Combined canEquips - */
b32  _Unit_canEquip(          Unit *u, canEquip e);
b32   Unit_canEquip(          Unit *u, canEquip e);
b32   Unit_canEquip_AnyHand(  Unit *u, canEquip e);
void  Unit_canEquip_Equipment(Unit *u, canEquip e);

/* --- Checking Equipment: de-equip if broken --- */
void _Unit_Check_Equipped(Unit *u, unit_hand hand);
void  Unit_Check_Equipped(Unit *u);

/* --- Unit state --- */
b32 Unit_istwoHanding(  Unit *u);
b32 Unit_isdualWielding(Unit *u);
b32 Unit_iswrongHanding(Unit *u);

/* --- Debug --- */
b32  Unit_Equipment_Full( Unit *u);
void Unit_Equipment_Print(Unit *u);

/* -- Use -- */
void Unit_Staff_Use(Unit *unit, Unit *patient);

#endif /* UNIT_EQUIPMENT_H */
