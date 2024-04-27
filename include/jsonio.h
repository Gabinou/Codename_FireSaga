#ifndef JSONIO_H
#define JSONIO_H

#include "cJSON.h"
#include "tile.h"
#include "weapon.h"
#include "structs.h"
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
void          jsonio_writeJSON(s8 f, void *ptr, b32 a);
struct cJSON *jsonio_parseJSON(s8 f);

/* --- UTILITIES --- */
/* -- Read -- */
void Array_readJSON(  struct cJSON *j, i32 *arr);
void Array2D_readJSON(struct cJSON *j, i32 *, u8 rl, u8 cl);

void RNseed_readJSON(struct cJSON *_j);

void PaletteTable_readJSON(char *filename, u8 *palette_table);

// TODO: change filename to input cJSON struct.
void Shop_readJSON(        char *filename, struct Shop         *s);
void Palette_readJSON(     char *filename, struct SDL_Palette  *p);
void Promotion_readJSON(   char *filename, struct Promotion    *p);

void Range_readJSON(            void *input, struct cJSON *j);
void Point_readJSON(            void *input, struct cJSON *j);
void Unit_stats_readJSON(       void *input, struct cJSON *j);
void Item_stats_readJSON(       void *input, struct cJSON *j);
void Weapon_stats_readJSON(     void *input, struct cJSON *j);
void fMovement_cost_readJSON(   void *input, struct cJSON *j);
void Inventory_item_readJSON(   void *input, struct cJSON *j);
void Computed_Stats_readJSON(   void *input, struct cJSON *j);

/* - Narrative - */
void Scene_readJSON(void *input, struct cJSON *_jnarr);
// Note: Scenes don't get written, no writing
b32 Conditions_Read(struct Conditions *conditions, cJSON *jconditions);
b32 Condition_Read(u32 *bitfield, size_t bits, cJSON *jcondition);

/* -- Write -- */
void RNseed_writeJSON(struct cJSON *j);

void Array_writeJSON(  struct cJSON *j, i32 *arr, size_t l);
void Array2D_writeJSON(struct cJSON *j, i32 *arr, u8 rl, u8 cl);

void Range_writeJSON(           void *input, struct cJSON *j);
void Point_writeJSON(           void *input, struct cJSON *j);
void Item_stats_writeJSON(      void *input, struct cJSON *j);
void Unit_stats_writeJSON(      void *input, struct cJSON *j);
void Tile_stats_writeJSON(      void *input, struct cJSON *j);
void Weapon_stats_writeJSON(    void *input, struct cJSON *j);
void Inventory_item_writeJSON(  void *input, struct cJSON *j);
void fMovement_cost_writeJSON(  void *input, struct cJSON *j);
void Computed_Stats_writeJSON(  void *input, struct cJSON *j);

/* -- Print -- */
void jsonio_Print(PHYSFS_file *fp, struct cJSON *json);

/* -- Comparisons -- */
b32  jsonio_Equal(char *f1, char *f2, b32 case_sensitive);

#endif /* Filesystem_H */
