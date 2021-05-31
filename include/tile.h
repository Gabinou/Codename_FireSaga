#ifndef TILE_H
#define TILE_H

#include "types.h"
#include "narrative.h"
#include "cJSON.h"
#include "utilities.h"
#include "enums.h"
#include "physfs.h"
#include "flecs.h"
#include "filesystem.h"

typedef struct Tile {
    // Unaligned size 36 bytes
    // Aligned size 32 bytes
    uint8_t json_element;
    bool inside;
    char * name;
    int_tile_t id;
    struct Tile_stats stats;
    uint8_t cost_array[UNIT_MVT_END];
    struct Movement_cost cost_struct;
    struct Breakable * breakable;
    struct Door * door;
    struct Chest * chest;
} Tile;
extern struct Tile Tile_default;

typedef struct TileModule {
    ECS_DECLARE_COMPONENT(Tile);
} TileModule;

void TileModuleImport(ecs_world_t * in_world);
#define TileModuleImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, Tile);



struct Tiles_hash {
    int_tile_t key;
    struct Tile * value;
};

struct Tiles_list {
    struct Point * pos_relative;
    int_tile_t * tiles_index;
    uint8_t num;
};

typedef struct Chest {
    int_item_t item;
    uint32_t gold;
    struct Tiles_list * toChange_onOpen;
} Chest;
extern struct Chest Chest_default;

typedef struct ChestModule {
    ECS_DECLARE_COMPONENT(Chest);
} ChestModule;

void ChestModuleImport(ecs_world_t * in_world);
#define ChestModuleImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, Chest);


typedef struct Breakable {
    // makes position passable OR impassable
    // according to position component + relative
    // Door/chests can be breakable -> add breakable to tile
    struct Tiles_list * toChange_onBreak;
    uint8_t hp;
    uint8_t def;
    uint8_t res;
} Breakable;
extern struct Breakable Breakable_default;

typedef struct BreakableModule {
    ECS_DECLARE_COMPONENT(Breakable);
} BreakableModule;

void BreakableModuleImport(ecs_world_t * in_world);
#define BreakableModuleImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, Breakable);

typedef struct Door {
    struct Tiles_list * toChange_onOpen;
    int_chapter_t chapter_open;
    int_event_t event_onOpen;
    int_scene_t scene_onOpen;
} Door;
extern struct Door Door_default;

typedef struct DoorModule {
    ECS_DECLARE_COMPONENT(Door);
} DoorModule;

void DoorModuleImport(ecs_world_t * in_world);
#define DoorModuleImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, Door);


extern void Tile_writeJSON(struct Tile * in_tile, cJSON * in_jtile);
extern void Tile_readJSON(struct Tile * in_tile, cJSON * in_jtile);
extern void Tile_makeMvtCostarray(struct Tile * in_tile);
extern void Breakable_writeJSON(struct Breakable * in_breakable, cJSON * in_jbreakable);
extern void Door_writeJSON(struct Door * in_door, cJSON * in_jdoor);
extern void Chest_writeJSON(struct Chest * in_chest, cJSON * in_jchest);
extern void Tiles_list_writeJSON(struct Tiles_list * in_tiles_list, cJSON * in_jtiles_list);

#endif /* TILE_H */
