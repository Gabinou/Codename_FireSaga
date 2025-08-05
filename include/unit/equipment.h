#ifndef UNIT_EQUIPMENT_H
#define UNIT_EQUIPMENT_H

#include "types.h"
#include "enums.h"
#include "tnecs.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Weapon;
struct Item;

/* --- Glossary ---
** - Loadout:   array of weapons that COULD be equipped
** - Equipped:  array of weapons currently equipped
** - struct Loadout: array of weapons indices
**      - Could be to represent Loadout, OR Equipped. */

/* --- Items --- */
Inventory_item *Unit_InvItem(       Unit *u, i32 eq);
Inventory_item *Unit_Item_Equipped( Unit *u, i32 hand);
tnecs_entity    Unit_InvItem_Entity(Unit *u, i32 eq);

/* -- Deplete: decrease durability -- */
void _Unit_Item_Deplete(            Unit *u, i32 eq, i64 a);
void _Unit_Equipped_Deplete(        Unit *u, i32 h,  i64 a);

void Unit_Item_Deplete(             Unit *u, i32 eq);
void Unit_Equipped_Staff_Deplete(   Unit *u, i32 h);
void Unit_Equipped_Weapons_Deplete( Unit *u);
void Unit_Equipped_Shields_Deplete( Unit *u);

/* -- Trading -- */
void  Unit_Item_Take(  Unit *u, tnecs_entity item);
void  Unit_Item_Trade( Unit *g, Unit *t, i32 giver_i, i32 taker_i);
void  Unit_Item_Takeat(Unit *u, tnecs_entity i, i32 j);
void _Unit_Item_Takeat(Unit *u, tnecs_entity i, i32 j);

/* -- Swapping -- */
void Unit_Item_Swap(  Unit *u, i32 ind1, i32 ind2);

/* -- Dropping -- */
tnecs_entity Unit_Item_Drop(Unit *u, i32 ind1);
void Unit_Equipment_Drop(Unit *u);

/* -- Getters -- */
const struct Weapon *Unit_Weapon(         Unit *u, i32 eq);
const struct Item   *Unit_Get_Item(       Unit *u, i32 eq);
const struct Weapon *Unit_Equipped_Weapon(Unit *u, i32 hand);

/* ID of equipped weapon */
i32  Unit_Id_Equipped( Unit *u, i32 hand);
i32  Unit_Id_Equipment(Unit *u, i32 eq);
void Unit_Id_Equipped_Set( Unit *u, i32 hand, i32 eq);
i32* Unit_Equipped_Array(const Unit *const unit);

/* Order in _equipment of equipped weapon */
i32 Unit_Eq_Equipped(   const Unit *const unit, i32 h);
i32 Unit_Order_canEquip(const Unit *const unit, i32 eq);

/* --- Equipping --- */
void    Unit_Equip(     Unit *u, i32 h, i32 eq);
void    Unit_Unequip(   Unit *u, i32 h);
void    Unit_Unequip_All(Unit *u);
b32     Unit_isEquipped(Unit *u, i32 h);
b32     Unit_anyWeapon_Equipped(Unit *u, i32 h);

/* - Base canEquips - */
// TODO: tetrabrachios canequip?
// only if playable!
b32  Unit_canEquip_Type(    Unit *u,    i32 id);
b32  Unit_canEquip_Users(   Unit *u,    i32 id);
b32  Unit_canEquip_OneHand( Unit *u,    i32 eq, i32 h, i32 mode);
b32  Unit_canEquip_TwoHand( Unit *u,    i32 eq, i32 h, i32 mode);

b32 Unit_canEquip_Archetype(Unit *u,    i32 id, i64 archetype);

u8  Unit_canEquip_allTypes( Unit *u,    u8 *eq);

/* - Combined canEquips - */
b32  _Unit_canEquip(          Unit *u, canEquip e);
b32   Unit_canEquip(          Unit *u, canEquip e);
b32   Unit_canEquip_AnyHand(  Unit *u, canEquip e);
void  Unit_canEquip_Equipment(Unit *u, canEquip e);

/* --- Checking Equipment: de-equip if broken --- */
void _Unit_Check_Equipped(Unit *u, i32 hand);
void  Unit_Check_Equipped(Unit *u);

/* --- Unit state --- */
b32 Unit_istwoHanding(  Unit *u);
b32 Unit_isdualWielding(Unit *u);
b32 Unit_iswrongHanding(Unit *u);

/* --- Debug --- */
b32  Unit_Equipment_Full( Unit *u);
void Unit_Equipment_Print(Unit *u);

tnecs_entity *Unit_Equipment(Unit *u);
i32 *Unit_canEquip_Arr(Unit *u);

/* -- Use -- */
void Unit_Staff_Use(Unit *unit, Unit *patient);

#endif /* UNIT_EQUIPMENT_H */
