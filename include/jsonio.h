#ifndef JSONIO_H
#define JSONIO_H
/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
**  jsonio: Read and write json data files
**      - Units
**      - Items
**      - Map
**      - Tiles
**      - Game saves
**
*/

#include "SDL.h"

#include "cJSON.h"
#include "physfs.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct cJSON;
struct Scene;
struct Mobj_Link;
struct Breakable;
struct Conditions;

/* --- ENUMS --- */
enum JSON_bOFFSET {
    // bOFFSET: offset in bytes
    // HOWTO:
    //  1. char * byte_ptr = (char *)struct_ptr
    //  2. <type> * <var> = (<type> *)(byte_ptr + bOFFSET_<var>)
    JSON_FILENAME_bOFFSET   = BYTE_OFFSET(jsonIO_Header, json_filename),
    JSON_ELEM_bOFFSET       = BYTE_OFFSET(jsonIO_Header, json_element),
};

/* --- TYPEDEFS --- */
typedef void (*json_rfunc)(void *, const cJSON *);
typedef void (*json_wfunc)(const void *, cJSON *);

/* --- READERS & WRITERS --- */
extern const json_rfunc json_rfuncs[JSON_END];
extern const json_wfunc json_wfuncs[JSON_END];

/* --- Macro reader utils --- */
#define JSONIO_READ_FLAG(var, jvar) if (jvar != NULL) {\
        var = cJSON_IsBool(jvar) ? \
              cJSON_IsTrue(jvar) : \
              cJSON_GetNumberValue(jvar);\
    }
#define JSONIO_READ_NUMBER(var, jvar) if (jvar != NULL) {\
        var = cJSON_GetNumberValue(jvar);\
    }

/* --- API --- */
void    jsonio_readJSON( s8 f, void *ptr);
void    jsonio_writeJSON(s8 f, const void *ptr, b32 a);
cJSON  *jsonio_parseJSON(s8 f);

/* --- UTILITIES --- */
/* -- Read -- */
void Array_readJSON(  cJSON *j, i32 *arr);
void Array2D_readJSON(cJSON *j, i32 *, u8 rl, u8 cl);

// TODO: PaletteTable struct to use jsonio_readjson
void PaletteTable_readJSON(     char *filename, u8 *palette_table);

// TODO: wrap SDL_palette with IES_palette to use jsonio_readjson
void Palette_readJSON(          char *filename, SDL_Palette *p);

void AI_readJSON(               void *input, const cJSON *j);
void Range_readJSON(            void *input, const cJSON *j);
void Scene_readJSON(            void *input, const cJSON *j);
void Point_readJSON(            void *input, const cJSON *j);
void InvItem_readJSON(          void *input, const cJSON *j);
void Unit_stats_readJSON(       void *input, const cJSON *j);
void Item_stats_readJSON(       void *input, const cJSON *j);
void Weapon_stats_readJSON(     void *input, const cJSON *j);
void Movement_cost_readJSON(    void *input, const cJSON *j);
void Computed_Stats_readJSON(   void *input, const cJSON *j);

/* -- Write -- */
// TODO: Wrap arrays into structs to use jsonio_readjson?
void Array_writeJSON(  cJSON *j, i32 *arr, size_t l);
void Array2D_writeJSON(cJSON *j, i32 *a2d, u8 rl, u8 cl);

void AI_writeJSON(              const void *input, cJSON *j);
void Range_writeJSON(           const void *input, cJSON *j);
void Point_writeJSON(           const void *input, cJSON *j);
void InvItem_writeJSON(         const void *input, cJSON *j);
void Item_stats_writeJSON(      const void *input, cJSON *j);
void Unit_stats_writeJSON(      const void *input, cJSON *j);
void Tile_stats_writeJSON(      const void *input, cJSON *j);
void Weapon_stats_writeJSON(    const void *input, cJSON *j);
void Movement_cost_writeJSON(   const void *input, cJSON *j);
void Computed_Stats_writeJSON(  const void *input, cJSON *j);

/* -- Print -- */
void jsonio_Print(PHYSFS_file *fp, cJSON *json);

/* -- Comparisons -- */
b32  jsonio_Equal(char *f1, char *f2, b32 case_sensitive);

#endif /* Filesystem_H */
