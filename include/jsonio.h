#ifndef JSONIO_H
#define JSONIO_H

#include "structs.h"
#include "physfs.h"
#include "cJSON.h"
#include "SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct Scene;
struct Mobj_Link;
struct Breakable;
struct Conditions;
struct cJSON;
typedef struct cJSON cJSON;

/* --- ENUMS --- */
enum JSON_bOFFSET {
    // bOFFSET: offset in bytes
    // HOWTO:
    //  1. char * byte_ptr = (char *)struct_ptr
    //  2. <type> * <var> = (<type> *)(byte_ptr + bOFFSET_<var>)
    JSON_FILENAME_bOFFSET   = BYTE_OFFSET(jsonIO_Header, json_filename),
    JSON_ELEM_bOFFSET       = BYTE_OFFSET(jsonIO_Header, json_element), /* for u8 in json read/writable structs  */
};

/* --- TYPEDEFS --- */
typedef void (*json_func)(void *, cJSON *);

/* --- READERS & WRITERS --- */
extern const json_func json_read_funcs [JSON_END];
extern const json_func json_write_funcs[JSON_END];

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
void Movement_cost_readJSON(    void *input, struct cJSON *j);
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
void Array2D_writeJSON(struct cJSON *j, i32 *a2d, u8 rl, u8 cl);

void Range_writeJSON(           void *input, struct cJSON *j);
void Point_writeJSON(           void *input, struct cJSON *j);
void Item_stats_writeJSON(      void *input, struct cJSON *j);
void Unit_stats_writeJSON(      void *input, struct cJSON *j);
void Tile_stats_writeJSON(      void *input, struct cJSON *j);
void Weapon_stats_writeJSON(    void *input, struct cJSON *j);
void Movement_cost_writeJSON(   void *input, struct cJSON *j);
void Inventory_item_writeJSON(  void *input, struct cJSON *j);
void Computed_Stats_writeJSON(  void *input, struct cJSON *j);

/* -- Print -- */
void jsonio_Print(PHYSFS_file *fp, struct cJSON *json);

/* -- Comparisons -- */
b32  jsonio_Equal(char *f1, char *f2, b32 case_sensitive);

#endif /* Filesystem_H */
