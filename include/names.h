#ifndef NAMES_H
#define NAMES_H

#include <math.h>
#include "nstr.h"
#include "platform.h"
#include "unit.h"
#include "types.h"
#include "enums.h"
#include "macros.h"
#include "debug.h"
#include "SDL.h"

/* -- Names -- */
// TODO migrate names to s8
// TODO make names not global
extern s8 sexNames[UNIT_SEX_NUM];
extern s8 statNames[UNIT_STAT_NUM];
extern s8 armyNames[ARMY_NUM];
extern s8 classNames[UNIT_CLASS_NUM];
extern s8 sceneTimes[SCENE_TIME_NUM];
extern s8 unitStatuses[UNIT_STATUS_END];
extern s8 campjobNames[CAMPJOB_END];
extern s8 mapFilenames[CHAPTER_END];
extern s8 support_types[SUPPORT_TYPE_NUM];
extern s8 gameStatenames[GAME_STATE_END];
extern s8 menuOptionnames[MENU_OPTION_END];
extern s8 jsonElementnames[JSON_END];
extern s8 global_unitNames[UNIT_NUM];  /* global_unitNames[order] = unit_name */
extern s8 global_itemNames[ITEM_NUM];
extern s8 global_tilenames[TILE_ID_MAX];
extern s8 gamesubStatenames[GAME_SUBSTATE_END];

/* --- Order at run-time --- */
/* order at comp-time: use ITEM_ORDER_##X */
extern struct dtab   *global_itemOrders; /* DTAB_GET(global_unitOrders, id) = order */
extern struct dtab   *global_unitOrders; /* DTAB_GET(global_unitOrders, id) = order */

extern u16           *class_equippables;

/* -- Name makers -- */
void Names_sceneTimes();
void Names_campjobNames();
void Names_mapFilenames();
void Menu_MakeOptionnames();
void Names_gameStatenames();
void Names_jsonElementnames();
void Names_gamesubStatenames();

/* - units - */
void   Names_armyNames();
void   Names_unitNames();
void   Names_statNames();
void   Names_classNames();
void   Names_unitStatuses();
void   Names_class_equippables();

/* - supports - */
void Names_supportTypes();

/* - items - */
void Names_itemNames();
void Names_tileNames();

/* --- X macros generators --- */
void Names_wpnType_Free( s8 *types);
s8 *Names_wpnType(       u16 typecode);
s8 *Names_unitTypes(     u16 typecode);
s8 *Names_skillNames(    u64 skillscode);
s8 *Names_wpnEffects(    u64 effect);
s8 *Names_unitstateNames(u32 statecode);

/* -- API -- */
void Names_Free();
void Names_Load_All();
void Names_Print(s8 filename);
void Names_Print_All(const char *filename);

#endif /* NAMES_H */
