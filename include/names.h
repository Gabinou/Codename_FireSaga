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
#include "SDL2/SDL.h"

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
extern uf16          *class_equippables;
extern char         **armyNames;

/* -- Name makers -- */
extern void Names_sceneTimes();
extern void Names_campjobNames();
extern void Names_mapFilenames();
extern void Menu_MakeOptionnames();
extern void Names_gameStatenames();
extern void Names_jsonElementnames();
extern void Names_gamesubStatenames();

/* - units - */
extern void   Names_sexNames();
extern void   Names_armyNames();
extern void   Names_unitNames();
extern void   Names_statNames();
extern void   Names_classNames();
extern void   Names_unitStates();
extern void   Names_class_equippables();
extern char **Names_unitstateNames(u32 statecode);

/* - supports - */
extern void Names_supportTypes();

/* - items - */
extern void Names_itemNames();
extern void Names_tileNames();

/* --- X macros generators --- */
extern void Names_wpnType_Free(char **types);
extern char **Names_wpnType(   uf16 typecode);
extern char **Names_unitTypes( uf16 typecode);
extern char **Names_skillNames(u64  skillscode);
extern char **Names_wpnEffects(u64  effect);

/* -- API -- */
extern void Names_Free();
extern void Names_Load_All();
extern void Names_Print_All(const char *filename);

#endif /* NAMES_H */
