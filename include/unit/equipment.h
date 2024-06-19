#ifndef UNIT_EQUIPMENT_H
#define UNIT_EQUIPMENT_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "structs.h"
#include "filesystem.h"
#include "weapon.h"
#include "supports.h"
#include "unit/mount.h"
#include "nmath.h"
#include "jsonio.h"
#include "equipment.h"
#include "RNG.h"
#include "physfs.h"
#include "equations.h"
#include "platform.h"
#include "utilities.h"
#include "combat.h"
#include "unit/anim.h"
#include "unit/status.h"
#include "unit/boss.h"
#include "unit/range.h"
#include "unit/stats.h"
#include "unit/mount.h"

/* --- Setters/Getters --- */
int Unit_Id_Strong(Unit *u, int i);

/* -- Item spaces -- */
// TODO: REMOVE ________________________________________________________________
/* Equipment, items                         ->    side space */
/* Menus, popups are top hand/strong hand   ->  strong space */
Inventory_item *Unit_Item_Side(  Unit *u, int i); /*   side space */
Inventory_item *Unit_Item_Strong(Unit *u, int i); /* strong space */
void Unit_Set_Item_Side(Unit *u, int i);                 /*   side space */
int Unit_Hand_Strong(Unit *u);
int SotA_Hand_Strong(i8 handedness);
int Unit_Hand_Strong2Side(Unit *unit, int i);
int Unit_Hand_Side2Strong(Unit *unit, int i);
int Unit_Equipped_Strong(Unit *u, int i); /* strong space */
void Unit_Set_Item_Side(struct Unit *u, int i);
// TODO: REMOVE ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// TODO: REMOVE ________________________________________________________________
Inventory_item *Unit_Item(Unit *u, int i);
// TODO: IMPLEMENT ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Inventory_item *Unit_Item(Unit *u, int i);

int Unit_Equipped(  Unit *unit, b32 hand);
b32 Unit_isEquipped(Unit *unit, b32 hand);
int Unit_Get_Equipped_ID(Unit *unit, b32 hand);
struct Weapon *Unit_Get_Equipped_Weapon(Unit *unit, b32 hand);

/* --- Items --- */
/* -- Deplete: decrease durability -- */
void _Unit_Item_Deplete(           Unit *u, int  i, u64 a);
void _Unit_Equipped_Deplete(       Unit *u, b32 h, u64 a);

void Unit_Item_Deplete(            Unit *u, int i);
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
void Equipment_Swap(Inventory_item *_equipment, i16 i1, i16 i2);

/* -- Dropping -- */
Inventory_item Unit_Item_Drop(Unit *u, i16 ind1);
void Unit_Equipment_Drop(Unit *u);
void Unit_Equipment_Import(Unit *u, Inventory_item *equipment);
void Unit_Equipment_Export(Unit *u, Inventory_item *equipment);

/* --- Equipping --- */
b32                   Unit_Equip(           Unit *u, b32 h, int i);
void                  Unit_Unequip(         Unit *u, b32 h);
b32                   Unit_Equip_inHand(    Unit *u, b32 h);
Inventory_item Unit_Equip_TwoHanding(Unit *u);

/* --- Checking Equipment: de-equip if broken --- */
void _Unit_Check_Equipped(Unit *u, b32 hand);
void  Unit_Check_Equipped(Unit *u);

/* --- Usability --- */
b32  Unit_Eq_Usable(  Unit *u, u64 a, int i);
b32  Unit_All_Usable( Unit *u);
void Unit_Find_Usable(Unit *u, u64 a);
b32  Unit_Item_Usable(Unit *u, u64 a, int i);

/* -- Can Equip -- */
b32 Unit_canEquip(       Unit *u, i16 id);
u8  Unit_Equippables(    Unit *u, u8 *eq);
b32 Unit_canEquip_Type(  Unit *u, i16 id);
b32 Unit_canEquip_Hand(  Unit *u, i16 id, b32 h);
b32 Unit_canEquip_inHand(Unit *u, b32 h);

/* --- Unit state --- */
b32 Unit_isdualWielding(struct Unit *u);
// WrongHanding: using shield or offhand in strong hand and vice vdrsa
b32 Unit_iswrongHanding(struct Unit *u);

/* --- Loadout Manipulation --- */
b32 Unit_Loadout_twoHanding(int lh, int rh);

/* - Public: Chooses between _Unit_Loadout_Swap_Twohanding and _Unit_Loadout_Swap - */
void Unit_Loadout_Swap(        struct Unit *u, int lh, int rh);

/* - Private: Only for not twohanding - */
void _Unit_Loadout_Swap(        struct Unit *u, int lh, int rh);

/* - Private: Only for twohanding - */
void _Unit_Loadout_Swap_Twohanding(        struct Unit *u, int i);


/* --- Debug --- */
b32  Unit_Equipment_Full( Unit *u);
void Unit_Equipment_Print(Unit *u);

#endif /* UNIT_EQUIPMENT_H */
