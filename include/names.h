#ifndef NAMES_H
#define NAMES_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "nstr.h"
#include "unit/unit.h"
#include "supports.h"

/* -- Names -- */
// TODO migrate names to s8
// TODO make names not global
extern const s8 sexNames[UNIT_SEX_NUM];

extern s8   ai_names[AI_NUM];
extern u64  ai_hashes[AI_NUM];
extern s8 scenes[SCENE_NUM];
extern s8 sceneTimes[SCENE_TIME_NUM];
extern s8 gl_unit_names[UNIT_NUM];  /* [unit_order] */

extern int global_unitTitlesId[UNIT_TITLE_NUM];  /* [unit_id] */
extern s8 global_unitTitles[UNIT_NUM];  /* [title_id] */

extern u64 unitHashes[UNIT_NUM];  /* [order]*/
extern u64 unitIDs[UNIT_NUM];  /* [order]*/
extern u64 unitIDs[UNIT_NUM];  /* [order]*/

extern s8 statNames[UNIT_STAT_MALLOC];
extern s8 armyNames[ARMY_MALLOC];
extern s8 classNames[UNIT_CLASS_NUM];
extern s8 unitStatuses[UNIT_STATUS_NUM];
extern s8 campjobNames[CAMPJOB_END];
extern s8 mapFilenames[CHAPTER_MALLOC];
extern s8 support_types[SUPPORT_TYPE_NUM];
extern s8 gameStatenames[GAME_STATE_END];
extern s8 menuOptionnames[MENU_OPTION_END];
extern s8 jsonElementnames[JSON_END];
extern s8 global_itemNames[ITEM_NUM];
extern s8 global_tilenames[TILE_ID_MAX];
extern s8 gamesubStatenames[GAME_SUBSTATE_END];
extern s8 mount_names[MOUNT_NUM];

/* --- Order at run-time --- */
/* order at comp-time: use ITEM_ORDER_##X */
extern struct dtab   *global_itemOrders; /* DTAB_GET(gl_unit_order, id) = order */
extern struct dtab   *gl_unit_order; /* DTAB_GET(gl_unit_order, id) = order */
extern u64           *class_equippables;

/* -- Name makers -- */
void Names_Mounts(void);
void Names_scenes(void);
void Names_sceneTimes(void);
void Names_campjobNames(void);
void Names_mapFilenames(void);
void Menu_MakeOptionnames(void);
void Names_gameStatenames(void);
void Names_jsonElementnames(void);
void Names_gamesubStatenames(void);

/* - units - */
void Names_armyNames(void);
void Names_unitNames(void);
void Names_statNames(void);
void Names_classNames(void);
void Names_unitStatuses(void);
void Names_class_equippables(void);

/* - supports - */
void Names_supportTypes(void);

/* - items - */
void Names_itemNames(void);
void Names_tileNames(void);

/* --- X macros generators --- */
void Names_wpnType_Free(s8 *types);

s8 *Names_wpnType(       u16 typecode);
s8 *Names_unitTypes(     u16 typecode);
s8 *Names_skillNames(    u64 skillscode);
s8 *Names_wpnEffects(    u64 effect);
s8 *Names_unitstateNames(u32 statecode);

/* -- API -- */
void Names_Free(void);
void Names_Load_All(void);
void Names_Print(s8 filename);
void Names_Print_All(char *filename);

u64 sota_hash_djb2(s8 str);
s8  Item_Name(i32 id);
int Unit_Name2Order(s8 name);
int Unit_Name2ID(s8 name);

s8  AI_filename(i32 ai_id);
i32 AI_Name2ID(s8 name);

#endif /* NAMES_H */


