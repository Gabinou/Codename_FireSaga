#ifndef NAMES_H
#define NAMES_H

#include <math.h>
#include "platform.h"
#include "unit.h"
#include "types.h"
#include "enums.h"
#include "globals.h"
#include "macros.h"
#include "debug.h"
#include "hashes.h"
#include "SDL.h"

/* -- Names -- */
extern char         **mapFilenames;
extern char         **sceneTimes;
extern char         **unitStates;
extern char         **jsonElementnames;
extern char         **campjobNames;
extern char         **menuOptionnames;
extern char         **gameStatenames;
extern char         **gamesubStatenames;
extern char         **global_unitNames;     /* global_unitNames[order] = unit_name      */
extern struct dtab   *global_unitOrders;    /* DTAB_GET(global_unitOrders, id) = order  */
extern char         **global_itemNames;
extern struct dtab   *global_itemOrders;    /* DTAB_GET(global_unitOrders, id) = order  */
extern char         **statNames;
extern char         **sexNames;
extern char         **classNames;
extern char         **global_tilenames;
extern i32           *tilesID_arr;
extern i32           *global_tilesID;
extern char         **support_types;
extern u16          *class_equippables;
extern char         **armyNames;

/* -- Name makers -- */
void Names_sceneTimes();
void Names_campjobNames();
void Names_mapFilenames();
void Menu_MakeOptionnames();
void Names_gameStatenames();
void Names_jsonElementnames();
void Names_gamesubStatenames();

/* - units - */
void   Names_sexNames();
void   Names_armyNames();
void   Names_unitNames();
void   Names_statNames();
void   Names_classNames();
void   Names_unitStates();
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
char **Names_skillNames(u64  skillscode);
char **Names_wpnEffects(u64  effect);

/* -- API -- */
void Names_Free();
void Names_Load_All();
void Names_Print_All(const char *filename);

#endif /* NAMES_H */
