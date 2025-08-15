#ifndef TILE_H
#define TILE_H

#include "enums.h"
#include "types.h"
#include "structs.h"

#include "SDL.h"

#include "cJSON.h"

/* --- FORWARD DECLARATIONS --- */
struct cJSON;
struct Unit;

typedef struct Tile {
    struct jsonIO_Header jsonio_header;

    struct Tile_stats stats;
    struct Movement_cost cost;
    i32 id;
    b32 indoors; /* Can't mount indoors */
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

typedef struct Coliseum {
    /* Units can fight an enemy for money */
} Coliseum;
extern const struct Arena Arena_default;

typedef struct Village {
    /* Units can talk to villagers */

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
void Door_Init(struct Door *door);
void Door_Init_tnecs(void *voiddoor);

void Chest_Init(struct Chest *chest);
void Chest_Init_tnecs(void *voidchest);

void Breakable_Init(struct Breakable *breakable);
void Breakable_Init_tnecs(void *voidbreakable);

void Tile_Free(struct Tile *tile);
b32 Tile_Valid_ID(u8 id);

/* -- Mobj -- */
void Mobj_Link_Init_tnecs(void *mobj);
void Mobj_Link_Free_tnecs(void *mobj);
void Mobj_Link_Init(struct Mobj_Link *mobj);
void Mobj_Link_Free(struct Mobj_Link *mobj);

/* --- I/O --- */
/* -- Tile -- */
void Tile_readJSON(      void *input, const cJSON *j);
void Tile_writeJSON(     const void *input, cJSON *j);
void Tile_stats_readJSON(void *input, const cJSON *j);
s8   Tile_Name(struct Tile *tile);

/* -- Mobj -- */
void Door_readJSON(      void *input, const cJSON *j);
void Chest_readJSON(     void *input, const cJSON *j);
void Door_writeJSON(     const void *input, cJSON *j);
void Chest_writeJSON(    const void *input, cJSON *j);
void Breakable_readJSON( void *input, const cJSON *j);
void Mobj_Link_readJSON( void *input, const cJSON *j);
void Mobj_Link_writeJSON(const void *input, cJSON *j);
void Breakable_writeJSON(const void *input, cJSON *j);

/* --- Internals --- */
i32*    Tile_Cost_Array(const struct Tile *tile);
i32     Tile_Cost(  const   struct Tile *tile,
                    const   struct Unit *unit);

#endif /* TILE_H */
