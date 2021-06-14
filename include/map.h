#ifndef MAP_H
#define MAP_H

#undef M_PI
#include <stdio.h>
#include <math.h>
#include "enums.h"
#include "q_math.h"
#include "narrative.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include "utilities.h"
#include "platform.h"
#include "stb_ds.h"
#include "filesystem.h"
#include "tile.h"
#include "flecs.h"
#include "tnecs.h"
#include "unit.h"
#include "linalg.h"
#include "jsonio.h"
#include "position.h"
#include "pathfinding.h"
#include "sprite.h"

// Pointer: // 32 bits

typedef struct Map {
    // Unaligned size 236 bytes
    // Aligned size 204 bytes
    uint8_t json_element;
    bool show_overlay;
    bool show_danger;
    bool show_grid;
    bool show_globalRange;
    int8_t mode_overlay;
    int8_t mode_danger;
    int8_t mode_globalRange;

    struct Point boundsmin; //  Maybe useless with row_len and offset?

    struct Point boundsmax; //  Maybe useless with row_len and offset?

    struct Point offset_px;

    SDL_Rect srcrect; // 128 bits

    SDL_Rect destrect; // 128 bits

    SDL_Renderer * renderer; // 32 bits
    uint16_t frames;
    uint16_t speed;

    uint16_t tilesize[2]; // 32 bits
    struct SDL_Textures_phash * textures; // 32 bits

    SDL_Texture * overlays[3]; // 96 bits
    bool bossdied;
    bool seized; // maybe unnecessary if turn system.
    uint8_t row_len;
    uint8_t col_len;

    SDL_Texture * dangers[2]; // 64 bits

    int_tile_t * overlay_move; // 2D dynamic array
    int_tile_t * overlay_attack; // 2D dynamic array

    int_tile_t * overlay_heal; // 2D dynamic array
    int_tile_t * overlay_danger; // 2D dynamic array

    int_tile_t * globalRange; // 2D dynamic array
    int_tile_t * tilesindex;

    struct Agony_timer * agony_stack;
    uint8_t num_agonizing;

    int8_t num_friendlies;
    int8_t num_enemies;
    int8_t turn; // Automatic loss if turn 255.
    int8_t chapter;
    uint8_t tiles_num;
    uint8_t starting_positions_num;
    uint8_t num_armies_onfield;

    struct Tiles_hash * tiles;
    uint8_t num_friendlies_onfield;
    uint8_t num_enemies_onfield;
    uint8_t num_units_onfield;

    int_tile_t * tilemap; // pointer to 2D dynamic array.
    ecs_entity_t * unitmap; // pointer to 2D dynamic array -> [i*leni + j]
    tnecs_entity_t * tnecs_unitmap; // pointer to 2D dynamic array -> [i*leni + j]

    struct Map_arrival * arrivals; // pointer to 1D dynamic array.
    struct Map_arrival * loaded_arrivals; // pointer to 1D dynamic array.

    struct Inventory_item * arrival_equipments; // pointer to 2D dynamic array.
    uint8_t arrivals_num;
    uint8_t loaded_arrivals_num;
    uint16_t boss;

    uint8_t * items_num;
    uint8_t * armies_onfield;

    ecs_entity_t * units_onfield;
    tnecs_entity_t * tnecs_units_onfield;
    ecs_entity_t * friendlies_onfield;
    tnecs_entity_t * tnecs_friendlies_onfield;

    struct Point * units_positions_list; // same order as unit_onfield
    uint16_t * essentials;

    ecs_entity_t * enemies_onfield;
    tnecs_entity_t * tnecs_enemies_onfield;
    struct Point * starting_positions;

} Map;
extern struct Map Map_default;

extern void Map_writeJSON(struct Map * in_map, cJSON * in_jmap);
extern void Map_readJSON(struct Map * in_map, cJSON * in_jmap);
extern void Map_Bounds_Compute(struct Map * in_map);
extern void Map_startingPos_Add(struct Map * in_map, struct Point in_position);

extern void Map_overlayTextures_load(struct Map * in_map);
extern void Map_Overlays_Clear(struct Map * in_map);
extern void Map_Overlays_Init(struct Map * in_map);
extern void Map_Overlays_Set(struct Map * in_map, const uint8_t in_mode, int16_t * in_overlay);

extern void Map_Danger_Load(struct Map * in_map);
extern void Map_Danger_Add(struct Map * in_map, int16_t * in_danger);
extern void Map_Danger_Sub(struct Map * in_map, int16_t * in_danger);

extern void Map_Tiles_Set(struct Map * in_map, int16_t ** to_load, size_t len);
extern void Map_Tiles_Load(struct Map * in_map);
extern void Map_tileTextures_Load(struct Map * in_map);
extern void Map_Tilesize_Set(struct Map * in_map, int16_t width, int16_t height);

extern void Map_Unit_Move(struct Map * in_map, const uint8_t x, const uint8_t y, const uint8_t new_x, const uint8_t new_y);
extern void Map_Unit_Move_tnecs(struct Map * in_map, const uint8_t x, const uint8_t y, const uint8_t new_x, const uint8_t new_y);
extern void Map_Unit_Put(struct Map * in_map, ecs_world_t * in_world, const uint8_t col, const uint8_t row, ecs_entity_t in_unit_id);
extern void Map_Unit_Put_tnecs(struct Map * in_map, tnecs_world_t * in_world, const uint8_t col, const uint8_t row, tnecs_entity_t in_unit_id);
extern void Map_Unit_Remove_fromPos(struct Map * in_map, const uint8_t col, const uint8_t row);
extern void Map_Unit_Remove_fromEntity(struct Map * in_map, ecs_world_t * in_world, const ecs_entity_t in_entity);
extern void Map_Unit_Remove_fromEntity_tnecs(struct Map * in_map, tnecs_world_t * in_world, const tnecs_entity_t in_entity);
extern void Map_Unit_Remove(struct Map * in_map, const ecs_entity_t in_entity);
extern void Map_Unit_Remove_tnecs(struct Map * in_map, const tnecs_entity_t in_entity);

extern ecs_entity_t Map_Unit_Get(struct Map * in_map, const uint8_t col, const uint8_t row);
extern tnecs_entity_t Map_Unit_Get_tnecs(struct Map * in_map, const uint8_t col, const uint8_t row);
extern ecs_entity_t * Map_Unit_Gets(struct Map * in_map, ecs_world_t * in_world, const uint8_t in_army);
extern tnecs_entity_t * Map_Unit_Gets_tnecs(struct Map * in_map, tnecs_world_t * in_world, const uint8_t in_army);

extern void Map_Unitmap_Clear(struct Map * in_map, ecs_world_t * in_world);
extern void Map_Unitmap_Clear_tnecs(struct Map * in_map, tnecs_world_t * in_world);

extern void Map_globalRange(struct Map * in_map, ecs_world_t * in_world, uint8_t in_alignment);
extern void Map_globalRange_tnecs(struct Map * in_map, tnecs_world_t * in_world, uint8_t in_alignment);

extern void Map_Renderer_Set(struct Map * in_map, SDL_Renderer * in_renderer);
int_tile_t * Map_Costmap_Movement_Compute(struct Map * in_map, ecs_world_t * in_world, ecs_entity_t in_unit_ent);
int_tile_t * Map_Costmap_Movement_Compute_tnecs(struct Map * in_map, tnecs_world_t * in_world, tnecs_entity_t in_unit_ent);
int_tile_t * Map_Costmap_PushPull_Compute(struct Map * in_map, ecs_world_t * in_world, ecs_entity_t in_unit_ent);
int_tile_t * Map_Costmap_PushPull_Compute_tnecs(struct Map * in_map, tnecs_world_t * in_world, tnecs_entity_t in_unit_ent);

extern void Map_Breakable_onBroken(struct Map * in_map, ecs_world_t * in_world, ecs_entity_t in_breakable_ent);
extern void Map_Breakable_onBroken_tnecs(struct Map * in_map, tnecs_world_t * in_world, tnecs_entity_t in_breakable_ent);
extern void Map_Door_onOpen(struct Map * in_map, ecs_world_t * in_world, ecs_entity_t in_door_ent);
extern void Map_Door_onOpen_tnecs(struct Map * in_map, tnecs_world_t * in_world, tnecs_entity_t in_door_ent);
extern void Map_Chest_onOpen(struct Map * in_map, ecs_world_t * in_world, ecs_entity_t in_chest_ent);
extern void Map_Chest_onOpen_tnecs(struct Map * in_map, tnecs_world_t * in_world, tnecs_entity_t in_chest_ent);

extern void Map_draw(struct Map * in_map);

#endif /* MAP_H */