#ifndef TILE_H
#define TILE_H

#include "enums.h"
#include "types.h"
#include "structs.h"
#include "nmath.h"
#include "filesystem.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

/* --- FORWARD DECLARATIONS --- */
struct cJSON;

typedef struct Tile {
    uf8 json_element; /* JSON_ELEM_bOFFSET = 0 */
    char *name;
    i32 id;
    struct Tile_stats stats;
    float cost_array[UNIT_MVT_END];
    struct fMovement_cost cost_struct;
    bool inside: 1;
} Tile;
extern struct Tile Tile_default;

/* --- Map objects --- */
/*  Many less map objects than tiles. -> store with arrays  */
/*  Mobjs are entities:                                     */
/*  -> Position is just another component                   */

typedef struct Mobj_Link {
    i32 *relpos_linked;
    i32 *abspos_linked;
    uf8 num_linked;
} Mobj_Link;
extern struct Mobj_Link Mobj_Link_default;

typedef struct Arena {

} Arena;
extern struct Arena Arena_default;

typedef struct Village {

} Village;
extern struct Village Village_default;

typedef struct Chest {
    i32 tile;
    uf16 item;
    u32 gold;
} Chest;
extern struct Chest Chest_default;

/*  Breakable is overriden by door/chest        */
/*  -> for tile shown, linking behavior, etc.   */
typedef struct Breakable {
    /*--- LINKING ---*/
    /*  linked breakables share stats and break at the same time    */
    /*--- PROPERTIES ---*/
    /*  Door/chests can be breakable -> add breakable entity        */
    i32 tile;
    i32 tile_after;
    uf8 hp;
    uf8 def;
    uf8 res;
} Breakable;
extern struct Breakable Breakable_default;

typedef struct Door {
    /*--- LINKING ---*/
    /* linked doors open at the same time */
    i32 tile;
    uf8 chapter_open;
    uf8 chapter_close;
    u32 event;
    u32 scene;
} Door;
extern struct Door Door_default;

/* --- Constructors/Destructors --- */
/* -- Tile -- */
extern void Tile_Free(struct Tile *tile);

/* -- Mobj -- */
extern void Mobj_Link_Init(struct Mobj_Link *mobj);
extern void Mobj_Link_Free(struct Mobj_Link *mobj);

/* --- I/O --- */
/* -- Tile -- */
extern void Tile_readJSON(       void *input, const  cJSON *const jtile);
extern void Tile_writeJSON(const void *input,        cJSON *jtile);

/* -- Mobj -- */
extern void Door_writeJSON(     const void *input,       cJSON *j);
extern void Chest_writeJSON(    const void *input,       cJSON *j);
extern void Mobj_Link_readJSON(       void *input, const cJSON *j);
extern void Mobj_Link_writeJSON(const void *input,       cJSON *j);
extern void Breakable_writeJSON(const void *input,       cJSON *j);

/* --- Internals --- */
extern void Tile_makeMvtCostarray(struct Tile *tile);

#endif /* TILE_H */
