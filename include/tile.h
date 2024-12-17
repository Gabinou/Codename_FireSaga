#ifndef TILE_H
#define TILE_H

#include "enums.h"
#include "types.h"
#include "structs.h"
#include "SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct cJSON;
typedef struct cJSON cJSON;

typedef struct Tile {
    s8  json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8  json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */
    s8  name;
    i32 id;
    struct Tile_stats stats;
    struct Movement_cost cost;
    b32 inside;
} Tile;
extern const struct Tile Tile_default;

/* --- Map objects --- */
/*  Many less map objects than tiles. -> store with arrays  */
/*  Mobjs are entities:                                     */
/*  -> Position is just another component                   */

typedef struct Mobj_Link {
    i32 *relpos_linked;
    i32 *abspos_linked;
    u8 num_linked;
} Mobj_Link;
extern const struct Mobj_Link Mobj_Link_default;

typedef struct Arena {

} Arena;
extern const struct Arena Arena_default;

typedef struct Village {

} Village;
extern const struct Village Village_default;

typedef struct Chest {
    i32 tile;
    u16 item;
    u32 gold;
} Chest;
extern const struct Chest Chest_default;

/*  Breakable is overriden by door/chest        */
/*  -> for tile shown, linking behavior, etc.   */
typedef struct Breakable {
    /*--- LINKING ---*/
    /*  linked breakables share stats and break at the same time    */
    /*--- PROPERTIES ---*/
    /*  Door/chests can be breakable -> add breakable entity        */
    i32 tile;
    i32 tile_after;
    u8 hp;
    u8 def;
    u8 res;
} Breakable;
extern const struct Breakable Breakable_default;

typedef struct Door {
    /*--- LINKING ---*/
    /* linked doors open at the same time */
    i32 tile;
    u8 chapter_open;
    u8 chapter_close;
    u32 event;
    u32 scene;
} Door;
extern const struct Door Door_default;

/* --- Constructors/Destructors --- */
/* -- Tile -- */
void Tile_Free(struct Tile *tile);
b32 Tile_Valid_ID(u8 id);

/* -- Mobj -- */
void Mobj_Link_Init(struct Mobj_Link *mobj);
void Mobj_Link_Free(struct Mobj_Link *mobj);

/* --- I/O --- */
/* -- Tile -- */
void Tile_readJSON(      void *input, cJSON *j);
void Tile_writeJSON(     void *input, cJSON *j);
void Tile_stats_readJSON(void *input, cJSON *j);

/* -- Mobj -- */
void Door_readJSON(      void *input, cJSON *j);
void Chest_readJSON(     void *input, cJSON *j);
void Door_writeJSON(     void *input, cJSON *j);
void Chest_writeJSON(    void *input, cJSON *j);
void Breakable_readJSON( void *input, cJSON *j);
void Mobj_Link_readJSON( void *input, cJSON *j);
void Mobj_Link_writeJSON(void *input, cJSON *j);
void Breakable_writeJSON(void *input, cJSON *j);

/* --- Internals --- */
i32* Tile_Cost_Array(struct Tile *tile);

#endif /* TILE_H */
