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
int Unit_Id_Strong(struct Unit *u, int i);

/* -- Item spaces -- */
// TODO: REMOVE ________________________________________________________________
/* Equipment, items                         ->    side space */
/* Menus, popups are top hand/strong hand   ->  strong space */
struct Inventory_item *Unit_Item_Side(  struct Unit *u, int i); /*   side space */
struct Inventory_item *Unit_Item_Strong(struct Unit *u, int i); /* strong space */
void Unit_Set_Item_Side(struct Unit *u, int i);                 /*   side space */
int Unit_Hand_Strong(struct Unit *u);
int SotA_Hand_Strong(i8 handedness);
int Unit_Hand_Strong2Side(struct Unit *unit, int i);
int Unit_Hand_Side2Strong(struct Unit *unit, int i);
// TODO: REMOVE ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int Unit_Get_Equipped(struct Unit *unit, b32 hand);
struct Weapon   *Unit_Get_Equipped_Weapon(  struct Unit *unit, b32 hand);

// TODO: remove
int Unit_Equipped_Strong(struct Unit *u, int i); /* strong space */

/* --- Items --- */
/* -- Deplete: decrease durability -- */
void _Unit_Item_Deplete(           struct Unit *u, int  i, u64 a);
void _Unit_Equipped_Deplete(       struct Unit *u, b32 h, u64 a);

void Unit_Item_Deplete(            struct Unit *u, int i);
void Unit_Equipped_Staff_Deplete(  struct Unit *u, b32 h);
void Unit_Equipped_Weapons_Deplete(struct Unit *u);
void Unit_Equipped_Shields_Deplete(struct Unit *u);

/* -- Trading -- */
void  Unit_Item_Take(  struct Unit *u, struct Inventory_item item);
void  Unit_Item_Swap(  struct Unit *u, i16 ind1, i16 ind2);
void  Unit_Item_Trade( struct Unit *g, struct Unit *t, i16 giver_i, i16 taker_i);
void  Unit_Item_Takeat(struct Unit *u, struct Inventory_item i, size_t j);
void _Unit_Item_Takeat(struct Unit *u, struct Inventory_item i, size_t j);

/* -- Dropping -- */
struct Inventory_item Unit_Item_Drop(struct Unit *u, i16 ind1);
void Unit_Equipment_Drop(struct Unit *u);
void Unit_Equipment_Import(struct Unit *u, struct Inventory_item *equipment);
void Unit_Equipment_Export(struct Unit *u, struct Inventory_item *equipment);

/* --- Equipping --- */
b32                   Unit_Equip(           struct Unit *u, b32 h, int i);
void                  Unit_Unequip(         struct Unit *u, b32 h);
b32                   Unit_Equip_inHand(    struct Unit *u, b32 h);
struct Inventory_item Unit_Equip_TwoHanding(struct Unit *u);

/* --- Checking Equipment: de-equip if broken --- */
void _Unit_Check_Equipped(struct Unit *u, b32 hand);
void  Unit_Check_Equipped(struct Unit *u);

/* --- Debug --- */
b32 Unit_Equipment_Full( struct Unit *u);
void Unit_Equipment_Print(struct Unit *u);

/* --- Usability --- */
b32  Unit_Eq_Usable(  struct Unit *u, u64 a, int i);
b32  Unit_All_Usable( struct Unit *u);
void Unit_Find_Usable(struct Unit *u, u64 a);
b32  Unit_Item_Usable(struct Unit *u, u64 a, int i);

/* -- Can Equip -- */
b32 Unit_canEquip(       struct Unit *u, i16 id);
u8  Unit_Equippables(    struct Unit *u, u8 *eq);
b32 Unit_canEquip_Type(  struct Unit *u, i16 id);
b32 Unit_canEquip_Hand(  struct Unit *u, i16 id, b32 h);
b32 Unit_canEquip_inHand(struct Unit *u, b32 h);

// TODO: move to src/equipment.c
void Equipment_Swap(struct Inventory_item *_equipment, i16 i1, i16 i2);

#endif /* UNIT_EQUIPMENT_H */
