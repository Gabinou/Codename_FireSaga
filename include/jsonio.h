#ifndef JSONIO_H
#define JSONIO_H

#include "cJSON.h"
#include "tile.h"
#include "weapon.h"
#include "item.h"
#include "macros.h"
#include "convoy.h"
#include "physfs.h"
#include "unit/unit.h"
#include "map/map.h"
#include "unit/party.h"
#include "camp.h"
// #include "sprite.h"
#include "utilities.h"
#include "supports.h"
#include "names.h"
#include "SDL2/SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct Door;
struct Chest;
struct Scene;
struct Mobj_Link;
struct Breakable;
struct Conditions;

/* --- TYPEDEFS --- */
typedef void (*json_func)(void *, cJSON *);

/* --- READERS & WRITERS --- */
extern json_func json_read_funcs [JSON_END];
extern json_func json_write_funcs[JSON_END];

/* --- AI definitions --- */
void AI_readJSON( void *ai, cJSON *jai);
void AI_writeJSON(void *ai, cJSON *jai);

/* --- API --- */
void          jsonio_readJSON( s8 f, void *ptr);
void          jsonio_writeJSON(s8 f, void *ptr, bool a);
struct cJSON *jsonio_parseJSON(s8 f);

/* --- UTILITIES --- */
/* -- Read -- */
void Array_readJSON(  struct cJSON *j, i32 *arr);
void jsonio_Read_2DArray(struct cJSON *j, i32 *, u8 rl, u8 cl);

void RNseed_readJSON( struct cJSON *_j);

void Shop_readJSON(        char *f, struct Shop         *s);
void Palette_readJSON(     char *f, struct SDL_Palette  *p);
void Promotion_readJSON(   char *f, struct Promotion    *p);
void PaletteTable_readJSON(char *f, u8                  *p);

// void Door_readJSON(             struct cJSON *j, struct Door             *d);
// void Chest_readJSON(            struct cJSON *j, struct Chest            *c);
void Inventory_item_readJSON(   struct cJSON *j, struct Inventory_item   *i);
void fMovement_cost_readJSON(   struct cJSON *j, struct fMovement_cost   *c);
void Weapon_stats_readJSON(     struct cJSON *j, struct Weapon_stats     *s);
void Point_readJSON(            struct cJSON *j, struct Point            *p);
void Tile_stats_readJSON(       struct cJSON *j, struct Tile_stats       *s);
void Unit_stats_readJSON(       struct cJSON *j, struct Unit_stats       *s);
void MObj_Link_readJSON(        struct cJSON *j, struct Mobj_Link        *b);
void Item_stats_readJSON(       struct cJSON *j, struct Item_stats       *s);

/* - Narrative - */
void Scene_readJSON(void *input, struct cJSON *_jnarr);
// Note: Scenes don't get written, no writing
bool Conditions_Read(struct Conditions *conditions, cJSON *jconditions);
bool Condition_Read(u32 *bitfield, size_t bits, cJSON *jcondition);

/* -- Write -- */
void RNseed_writeJSON(   struct cJSON *j);

void Array_writeJSON(    struct cJSON *j, i32 *arr, size_t l);
void jsonio_Write_2DArray(  struct cJSON *j, i32 *arr, u8 rl, u8 cl);

void item_writeJSON(     struct cJSON *j, struct Inventory_item *i);
void mvtcost_writeJSON(  struct cJSON *j, struct fMovement_cost *c);
void Wpnstats_writeJSON( struct cJSON *j, struct Weapon_stats   *s);
void Itemstats_writeJSON(struct cJSON *j, struct Item_stats     *t);
void Unitstats_writeJSON(struct cJSON *j, struct Unit_stats     *t);
void Tilestats_writeJSON(struct cJSON *j, struct Tile_stats     *t);

/* -- Print -- */
void jsonio_Print(PHYSFS_file *fp, struct cJSON *json);

/* -- Comparisons -- */
b32  jsonio_Equal(char *f1, char *f2, b32 case_sensitive);

#endif /* Filesystem_H */
