#ifndef JSONIO_H
#define JSONIO_H

#include "tile.h"
#include "weapon.h"
#include "item.h"
#include "macros.h"
#include "convoy.h"
#include "physfs.h"
#include "narrative.h"
#include "camp.h"
#include "sprite.h"
#include "unit.h"
#include "narrative.h"
#include "utilities.h"
#include "supports.h"
#include "names.h"
#include "SDL.h"

/* --- TYPEDEFS --- */
typedef void (*json_func)(void *, cJSON *);

/* --- READERS & WRITERS --- */
extern json_func json_read_funcs [JSON_END];
extern json_func json_write_funcs[JSON_END];

/* --- API --- */
void          jsonio_readJSON( s8 f, void *ptr);
void          jsonio_writeJSON(s8 f, void *ptr, bool a);
struct cJSON *jsonio_parseJSON(s8 f);

/* --- UTILITIES --- */
/* -- Read -- */
void jsonio_Read_Array(  struct cJSON *j, i32 *arr);
void jsonio_Read_2DArray(struct cJSON *j, i32 *, u8 rl, u8 cl);

void jsonio_Read_RNseed( struct cJSON *_j);

void jsonio_Read_Shop(        char *f, struct Shop         *s);
void jsonio_Read_Palette(     char *f, struct SDL_Palette  *p);
void jsonio_Read_Promotion(   char *f, struct Promotion    *p);
void jsonio_Read_PaletteTable(char *f, u8                 *p);

void jsonio_Read_Door(     struct cJSON *j, struct Door  *d);
void jsonio_Read_Chest(    struct cJSON *j, struct Chest *c);
void jsonio_Read_Item(     struct cJSON *j, struct Inventory_item *i);
void jsonio_Read_mvtcost(  struct cJSON *j, struct fMovement_cost *c);
void jsonio_Read_Wpnstats( struct cJSON *j, struct Weapon_stats   *s);
void jsonio_Read_Position( struct cJSON *j, struct Point          *p);
void jsonio_Read_Tilestats(struct cJSON *j, struct Tile_stats     *s);
void jsonio_Read_Unitstats(struct cJSON *j, struct Unit_stats     *s);
void jsonio_Read_MObj_Link(struct cJSON *j, struct Mobj_Link      *b);
void jsonio_Read_Breakable(struct cJSON *j, struct Breakable      *b);
void jsonio_Read_Itemstats(struct cJSON *j, struct Item_stats     *s);
void jsonio_Read_Reinforce(struct cJSON *j, struct Reinforcement  *a);

/* - Narrative - */
void Scene_readJSON(void *input, struct cJSON *_jnarr);
// Note: Scenes don't get written, no writing
bool Conditions_Read(struct Conditions *conditions, cJSON *jconditions);
bool Condition_Read(u32 *bitfield, size_t bits, cJSON *jcondition);

/* -- Write -- */
void jsonio_Write_RNseed(   struct cJSON *j);

void jsonio_Write_Array(    struct cJSON *j, i32 *arr, size_t l);
void jsonio_Write_2DArray(  struct cJSON *j, i32 *arr, u8 rl, u8 cl);

void jsonio_Write_item(     struct cJSON *j, struct Inventory_item *i);
void jsonio_Write_arrival(  struct cJSON *j, struct Reinforcement  *a);
void jsonio_Write_mvtcost(  struct cJSON *j, struct fMovement_cost *c);
void jsonio_Write_Wpnstats( struct cJSON *j, struct Weapon_stats   *s);
void jsonio_Write_Itemstats(struct cJSON *j, struct Item_stats     *t);
void jsonio_Write_Unitstats(struct cJSON *j, struct Unit_stats     *t);
void jsonio_Write_Tilestats(struct cJSON *j, struct Tile_stats     *t);

/* -- Print -- */
void jsonio_Print(PHYSFS_file *fp, struct cJSON *json);

#endif /* Filesystem_H */
