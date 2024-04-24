#ifndef MAP_H
#define MAP_H

#include "macros.h"
#include "enums.h"
#include "tnecs.h"
#include "map/tiles.h"
#include "map/ontile.h"
#include "map/conditions.h"
#include "map/find.h"
#include "map/path.h"
#include "index_shader.h"
#include "arrow.h"
#include "reinforcement.h"
#include "nmath.h"
#include "unit/unit.h"
#include "position.h"
#include "sprite.h"
#include "music.h"
#include "settings.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_mixer.h"

/* --- GLOSSARY --- */
// Traversing and occupying tiles
//  - Units can  *move through*   *traversable* tiles
//  - Units can *occupy/move to*  *unoccupied*  tiles
//      - Note: costmap is movement cost to *traverse* to tile

/* --- FORWARD DECLARATIONS --- */
struct Game;
struct Unit;
struct Map_condition;

/* --- ENUMS --- */
#define ARROW_FILENAME ARROW_FILENAME_
#define ARROW_FILENAME_ PATH_JOIN("..", "assets", "GUI", "arrow.png")

struct Map {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

    // TODO: remove. should be in savefile
    s8 party_filename;

    /* --- BASICS --- */
    u8 perimiter_danger_color;
    u8 perimiter_aura_color;

    /* --- BASICS --- */
    u8 turn; /* Automatic loss if turn 255. */
    u8 reinf_loaded;
    u8 row_len;
    u8 col_len;
    i32 chapter;
    i32 tilesize[TWO_D];
    struct Arrow    *arrow;
    tnecs_world     *world;
    SDL_Renderer    *renderer;

    /* --- SHADERS --- */
    struct Tilemap_Shader *tilemap_shader;

    /* --- STACK_MAP --- */
    u8 stack_mode;

    /* - Actual stackmaps - */
    i32 *stacked_dangermap;         /* 2D dynamic array */
    i32 *stacked_global_dangermap;  /* 2D dynamic array */

    /* - Perimeter - */
    struct Padding *edges_danger;

    /* --- ICONS --- */
    struct SDL_Texture *stack_move;
    struct SDL_Texture *stack_attack;
    struct SDL_Texture *stack_danger;

    struct Point boundsmin; /* Maybe useless with row_len and offset? */
    struct Point boundsmax; /* Maybe useless with row_len and offset? */

    struct Point visiblemin;
    struct Point visiblemax;

    /* --- FRAMES --- */
    i32  speed;
    u8  frames;                /* map frames num.    < 1 no anim */
    u8  frames_shadow;         /* shadow frames num. < 1 no anim */
    u8  shadow_frame_pause;    /* framenum wait to change shadow frame */
    u8  tilemap_frame_pause;   /* framenum wait to change anim frame */
    u8  shadow_frame_counter;
    u8  tilemap_frame_counter;
    float shadow_frame_factor;  /* framerate * factor = pause */
    float tilemap_frame_factor; /* framerate * factor = pause */

    /* --- costmap, MOVEMAP, ATTACKMAP... --- */
    i32 *temp;                  /* 2D dynamic array */
    i32 *costmap;               /* 2D dynamic array */
    i32 *movemap;               /* 2D dynamic array */
    i32 *start_posmap;          /* 2D dynamic array */
    // Tile index = map->tilemap[i] / TILE_DIVISOR;
    i32 *tilemap;               /* 2D dynamic array [row * col_len + col] */
    i32 *healtomap;             /* 2D dynamic array */

    i32 *dangermap;             /* 2D dynamic array */
    i32 *healtolist;            /* 2D dynamic array */
    i32 *healfrommap;           /* 2D dynamic array */
    i32 *global_dangermap;      /* 2D dynamic array */
    // Pointer to global_dangermap OR dangermap
    i32 *rendered_dangermap;    /* 2D dynamic array */

    /* attacktomap: Tiles where unit can attack enemy, 2D map  */
    i32 *attacktomap;           /* 2D dynamic array */
    /* attacktolist: List of tiles where unit can attack enemy */
    i32 *attacktolist;          /* 2D dynamic array */
    /* attackfrommap: Tiles where unit can attack enemy from*/
    i32 *attackfrommap;         /* 2D dynamic array */
    i32 *attackfromlist;        /* 2D dynamic array */
    i32 *global_rangemap;       /* 2D dynamic array */
    float *fcostmap;            /* 2D dynamic array */
    float *fmovemap;            /* 2D dynamic array */
    tnecs_entity *unitmap;      /* [row * col_len + col], occupymap */
    tnecs_entity *occupymap;    /* [row * col_len + col], friendlies only, enemies only... */

    /* --- PALETTES --- */
    SDL_Palette *palette_default;
    u8 *palettemap;     /* [row * col_len + col] */
    i32 *temp_palette;  /* [row * col_len + col] */

    /* -- Map palette indices -- */
    u8 ipalette_base;
    u8 ipalette_red;
    u8 ipalette_blue;
    u8 ipalette_green;
    u8 ipalette_purple;
    u8 ipalette_darkred;
    u8 ipalette_shadow;
    u8 ipalette_enemy;

    /* --- AGONY --- */
    struct Agony_timer *agony_stack;
    u8 num_agonizing;
    tnecs_entity costmap_ent;

    /* --- ARMIES --- */
    int army_i; /* Current army in control */
    // TODO: Split by army
    tnecs_entity *units_onfield;
    tnecs_entity *friendlies_onfield;
    tnecs_entity *enemies_onfield;

    tnecs_entity *friendlies_agony;
    tnecs_entity *enemies_agony;

    tnecs_entity *friendlies_killed;
    tnecs_entity *enemies_killed;

    i8 *army_onfield;
    struct Point *units_positions_list;  /* same order as unit_onfield */
    struct Point *start_pos;

    /* --- TILES --- */
    struct Tile *tiles;  /* [tiles_order] */
    i32 *tiles_id;       /* [tiles_order] */
    i32 *tilesindex;     /* [tiles_order] */
    struct SDL_Surface    *tilemap_surface;
    struct SDL_Texture    *tilemap_overlay_texture;
    struct SDL_Texture    *tilemap_texture;
    struct SDL_Texture    *texture;
    struct SDL_Surface  ***tileset_surfaces; /* [palette_id][tiles_order] */
    struct SDL_Texture  ***tileset_textures; /* [palette_id][tiles_order] */

    /* -- Unique tiles -- */
    /* tileset_order == tile_order*/
    u16 **tilesprites_ind;     /* [tileset_order][tilesprite_order]    */
    u16  *tilesprites_num;     /* [tileset_order]                      */

    /* --- REINFORCEMENT --- */
    u8 *items_num;
    struct Reinforcement   *reinforcements;  /* pointer to 1D dynamic array */
    struct Inventory_item **reinf_equipments;

    /* --- MAP ENTITIES --- */
    tnecs_entity *doors_ent;  /* breakable doors are here */
    tnecs_entity *chests_ent;
    tnecs_entity *breakables_ent;
    u16 door_num;
    u16 chest_num;
    u16 breakable_num;

    /* --- MUSIC --- */
    i32 music_i_friendly;
    i32 music_i_enemy;
    Mix_Music *music_friendly;
    Mix_Music *music_enemy;

    /* --- CONDITIONS --- */
    struct Map_condition *death_enemy;
    struct Map_condition *death_friendly;
    struct Map_condition *turn_end;
    struct Map_condition *waits_friendly;

    /* --- MAP SWITCHES --- */
    bool win                : 1;
    bool loss               : 1;
    bool update             : 1;
    bool seized             : 1; /* maybe unnecessary if turn system. */
    // bool show_move          : 1;
    // bool show_heal          : 1;
    // bool show_grid          : 1;
    bool show_icons         : 1;
    // bool show_attack        : 1;
    bool show_danger        : 1;
    bool show_overlay       : 1;
    bool camera_moved       : 1;
    bool visible_changed    : 1;
    bool shading_changed    : 1;
    bool show_globalRange   : 1;
};
extern struct Map Map_default;

/* --- Constructor/Destructors --- */
void        Map_Free(      struct Map *map);
struct Map *Map_Init(      struct Map *map, i32 width, i32 height);
void        Map_Units_Free(struct Map *map);
void        Map_Units_Hide(struct Map *map);

void Map_Texture_Alloc(struct Map *map);

/* -- Dynamic arrays -- */
void Map_dArrays_Free(struct Map *map);
void Map_dArrays_Init(struct Map *map,  struct Settings *settings);

/* --- Tilemap --- */
void Map_Tilemap_Surface_Init(struct Map *map);
void Map_Tilemap_Surface_Free(struct Map *map);
void Map_Tilemap_Texture_Init(struct Map *map);
void Map_Tilemap_Texture_Free(struct Map *map);

/* --- Tilesprites --- */
void Map_Tilesprites_Init(struct Map *map, size_t tiles_num);
void Map_Tilesprites_Free(struct Map *map);

/* --- I/O --- */
void Map_readJSON(  void *input, cJSON *jmap);
void Map_writeJSON( void *input, cJSON *jmap);

/* --- Map events / Triggers --- */
void Map_Turn_Increment(struct Map *map);
i8 Map_Army_Next(struct Map *map);

/* --- Music --- */
void Map_Music_Load(struct Map *map);

/* --- Boss --- */
b32 Map_Boss_Alive(struct Map *map, i16 army);

/* --- Tile --- */
struct Tile *Map_Tile_Get(struct Map *map, i32 x, i32 y);

#endif /* MAP_H */
