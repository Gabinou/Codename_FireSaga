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
#include "hashes.h"
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
extern char         **jsonElementnames;
extern char         **global_unitNames;  /* global_unitNames[order] = unit_name */
extern char         **global_itemNames;
extern char         **global_tilenames;
extern char         **gamesubStatenames;

// TODO: change into simple array + simple function?
//      -> simpler #include
extern struct dtab   *global_itemOrders; /* DTAB_GET(global_unitOrders, id) = order */
extern struct dtab   *global_unitOrders; /* DTAB_GET(global_unitOrders, id) = order */

extern i32           *tilesID_arr;
extern i32           *global_tilesID;
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
char **Names_unitstateNames(u32 statecode);

/* - supports - */
void Names_supportTypes();

/* - items - */
void Names_itemNames();
void Names_tileNames();

/* --- X macros generators --- */
void Names_wpnType_Free(char **types);
char **Names_wpnType(   u16 typecode);
char **Names_unitTypes( u16 typecode);
char **Names_skillNames(u64 skillscode);
char **Names_wpnEffects(u64 effect);

/* -- API -- */
void Names_Free();
void Names_Load_All();
void Names_Print(s8 filename);
void Names_Print_All(const char *filename);

#endif /* NAMES_H */
