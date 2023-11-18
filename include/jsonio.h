#ifndef JSONIO_H
#define JSONIO_H

#include "tile.h"
#include "weapon.h"
#include "item.h"
#include "macros.h"
#include "convoy.h"
#include "physfs.h"
#include "camp.h"
#include "sprite.h"
#include "unit.h"
#include "narrative.h"
#include "utilities.h"
#include "supports.h"
#include "names.h"
#include "SDL.h"

/* --- TYPEDEFS --- */
typedef void (*json_read_t)(       void *, const cJSON *const);
typedef void (*json_write_t)(const void *,       cJSON *);

/* --- READERS & WRITERS --- */
extern json_read_t  json_read_funcs [JSON_END];
extern json_write_t json_write_funcs[JSON_END];

/* --- API --- */
void          jsonio_readJSON( s8 f,       void *ptr);
void          jsonio_writeJSON(s8 f, const void *ptr, bool a);
struct cJSON *jsonio_parseJSON(s8 f);

/* --- UTILITIES --- */

/* --- JSON I/O --- */
// TODO: move to jsonIO
/* -- Read -- */
void jsonio_Read_Array(    const struct cJSON *const j, i32 *arr);
void jsonio_Read_2DArray(  const struct cJSON *const j, i32 *, u8 rl, u8 cl);

void jsonio_Read_Shop(        const char *f, struct Shop         *s);
void jsonio_Read_Palette(     const char *f, struct SDL_Palette  *p);
void jsonio_Read_Promotion(   const char *f, struct Promotion    *p);
void jsonio_Read_PaletteTable(const char *f, u8                 *p);

void jsonio_Read_RNseed(   const struct cJSON *const j);
void jsonio_Read_Door(     const struct cJSON *const j, struct Door  *d);
void jsonio_Read_Chest(    const struct cJSON *const j, struct Chest *c);
void jsonio_Read_Item(     const struct cJSON *const j, struct Inventory_item *i);
void jsonio_Read_mvtcost(  const struct cJSON *const j, struct fMovement_cost *c);
void jsonio_Read_Wpnstats( const struct cJSON *const j, struct Weapon_stats   *s);
void jsonio_Read_Position( const struct cJSON *const j, struct Point          *p);
void jsonio_Read_Tilestats(const struct cJSON *const j, struct Tile_stats     *s);
void jsonio_Read_Unitstats(const struct cJSON *const j, struct Unit_stats     *s);
void jsonio_Read_MObj_Link(const struct cJSON *const j, struct Mobj_Link      *b);
void jsonio_Read_Breakable(const struct cJSON *const j, struct Breakable      *b);
void jsonio_Read_Itemstats(const struct cJSON *const j, struct Item_stats     *s);
void jsonio_Read_Reinforce(const struct cJSON *const j, struct Reinforcement  *a);

/* -- Write -- */
void jsonio_Write_RNseed(   struct cJSON *j);

void jsonio_Write_Array(    struct cJSON *j, const i32 *arr, size_t l);
void jsonio_Write_2DArray(  struct cJSON *j, const i32 *arr, u8 rl, u8 cl);

void jsonio_Write_item(     struct cJSON *j, const struct Inventory_item *i);
void jsonio_Write_arrival(  struct cJSON *j, const struct Reinforcement  *a);
void jsonio_Write_mvtcost(  struct cJSON *j, const struct fMovement_cost *c);
void jsonio_Write_Wpnstats( struct cJSON *j, const struct Weapon_stats   *s);
void jsonio_Write_Itemstats(struct cJSON *j, const struct Item_stats     *t);
void jsonio_Write_Unitstats(struct cJSON *j, const struct Unit_stats     *t);
void jsonio_Write_Tilestats(struct cJSON *j, const struct Tile_stats     *t);

/* -- Print -- */
void jsonio_Print(PHYSFS_file *fp, const struct cJSON *json);

#endif /* Filesystem_H */
