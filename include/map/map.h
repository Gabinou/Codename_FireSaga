#ifndef MAP_H
#define MAP_H

#include "enums.h"
#include "structs.h"
#include "tnecs.h"
#include "nstr.h"
#include "SDL.h"
#include "cJSON.h"

/* --- GLOSSARY --- */
// Traversing and occupying tiles
//  - Units can  *move through*   *traversable* tiles
//  - Units can *occupy/move to*  *unoccupied*  tiles
//      - Note: costmap is movement cost to *traverse* to tile

/* --- FORWARD DECLARATIONS --- */
struct Game;
struct Unit;
struct Map_condition;
struct cJSON;
struct Arrow;

/* --- ENUMS --- */
#define ARROW_FILENAME ARROW_FILENAME_
#define ARROW_FILENAME_ PATH_JOIN("..", "assets", "GUI", "arrow.png")

typedef struct NewMap {
    /* Input parameters to map constructors*/
    i32 tilesize[TWO_D];

    i32 row_len; /* [tiles] */
    i32 col_len; /* [tiles] */

    tnecs_world *world;

    SDL_Renderer    *renderer;

    i32 stack_mode;
} NewMap;
extern const NewMap NewMap_default;


typedef struct Map_Perimiter {

} Map_Perimiter;

typedef struct Map_Arrays {

} Map_Arrays;

typedef struct Map_Flags {
    b32 win;
    b32 loss;
    b32 update;
    b32 seized; /* maybe unnecessary if turn system. */
    b32 show_icons;
    b32 show_danger;
    b32 show_overlay;
    b32 camera_moved;
    b32 visible_changed;
    b32 shading_changed;
    b32 show_globalRange;
} Map_Flags;

typedef struct Map {
    struct jsonIO_Header jsonio_header;

    // TODO: remove. should be in savefile
    s8 party_filename;

    /* --- PERIMETER --- */
    i32 perimiter_danger_color;
    i32 perimiter_aura_color;

    /* --- BASICS --- */
    i32 turn; /* Automatic loss if turn 255. */
    i32 reinf_loaded;

    /* Map size */
    i32 row_len; /* [tiles] */
    i32 col_len; /* [tiles] */
    i32 chapter;
    i32 tilesize[TWO_D]; /* [pixels] */

    Arrow           *arrow;
    tnecs_world     *world;
    SDL_Renderer    *renderer;
    u8 num_agonizing;

    /* --- SHADERS --- */
    struct Tilemap_Shader *tilemap_shader;

    /* --- STACK_MAP --- */
    i32 stack_mode;

    /* - Actual stackmaps - */
    i32 *stacked_dangermap;         /* 2D dynamic array */
    i32 *stacked_global_dangermap;  /* 2D dynamic array */

    /* - Perimeter - */
    struct Padding *edges_danger; /* Todo change to general purpose */

    /* --- ICONS --- */
    struct SDL_Texture *stack_move;
    struct SDL_Texture *stack_attack;
    struct SDL_Texture *stack_danger;

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

    /* Rational costs for map */
    i32 cost_multiplier;
    /* No need for denominator map: just one cost_multiplier
        1. Define all fractional effective costs on map
        2. Compute lowest common multiple (LCM) of effective costs denominators.
            - That's the cost_multiplier
        3. Costmap:
            1. Multiply all effective costs until denominator is LCM/cost_multiplier.
            2. Costmap values are the numerators.
        4. Effective unit move is: unit move stats * cost_multiplier.
            1.  Compute moveable tiles in usual way i.e.
                all tiles with effective cumulative cost < effective unit move
       */
    //  Example:
    // 1. I want a map with 5/6 effective cost and 5/4 effective costs
    //     - Denominators: 4 and 6
    // 2. cost_multiplier/LCM = 12
    // 3.
    //     1. 5/6 = 10/12 -> cost in costmap = 10
    //     2. 5/4 = 15/12 -> cost in costmap = 15
    // 4. Move stat = 4 -> effective move = 4 * 12 = 48

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
    tnecs_entity *unitmap;      /* [row * col_len + col], occupymap */
    tnecs_entity *occupymap;    /* [row * col_len + col], friendlies only, enemies only... */
    tnecs_entity costmap_ent; /* costmap computed for*/

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

    i32 *army_onfield;
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

    /* --- REINFORCEMENT --- */
    u8 *items_num;
    struct Reinforcement   *reinforcements;  /* pointer to 1D dynamic array */
    struct Inventory_item **reinf_equipments;

    /* --- MAP ENTITIES --- */
    tnecs_entity *doors_ent;  /* breakable doors are here */
    tnecs_entity *chests_ent;
    tnecs_entity *breakables_ent;

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

    struct Camera camera;

    struct Map_Flags flags;
} Map;
extern const struct Map Map_default;

/* --- Constructor/Destructors --- */

struct Map *Map_New(NewMap new_map);

void Map_Members_Alloc(Map *map);

void Map_Texture_Alloc(         Map *map);
void Map_Tilemap_Shader_Init(   Map *map);

/* -- SDL_free -- */
void Map_Free(Map *map);

void Map_Members_Free(Map *map);

void Map_Unitmap_Free(          Map *map);
void Map_Tilemap_Shader_Free(   Map *map);
void Map_Reinforcements_Free(   Map *map);

/* --- Tilemap --- */
void Map_Tilemap_Surface_Init(struct Map *map);
void Map_Tilemap_Surface_Free(struct Map *map);
void Map_Tilemap_Texture_Init(struct Map *map);
void Map_Tilemap_Texture_Free(struct Map *map);

/* --- Setters --- */
void Map_Size_Set(Map *map, i32 col, i32 row);
void Map_Tilesize_Set(Map *map, i32 w, i32 h);

/* --- I/O --- */
void Map_readJSON(  void *input, const cJSON *jmap);
void Map_writeJSON(const  void *input, cJSON *jmap);
void Map_RowCol_readJSON(s8 filename, i32 rowcol[TWO_D]);

/* --- Map events / Triggers --- */
void Map_Turn_Increment(struct Map *map);

i32 Map_Army_Next(struct Map *map);

/* --- Music --- */
void Map_Music_Load(struct Map *map);

/* --- Boss --- */
b32 Map_Boss_Alive(struct Map *map, i16 army);

/* --- Tile --- */
struct Tile *Map_Tile_Get(struct Map *map, i32 x, i32 y);

/* --- Bonus --- */
/* -- Apply -- */
void Map_Aura_Apply(struct Map *map, struct Aura aura, tnecs_entity *entities,
                    tnecs_entity source_ent, u16 item, u16 skill, b32 active, b32 instant);
void Map_Bonus_Support_Apply( struct Map *map);

void Map_Bonus_Standard_Apply(     struct Map *map, i32 army);
void Map_Bonus_Standard_Apply_Army(struct Map *map, i32 army);
void Map_Bonus_Standard_Apply_Unit(struct Map *map, tnecs_entity ent, tnecs_entity *entities);

/* -- Remove -- */
void Map_Bonus_Remove_Instant(      struct Map *map, i32 army);
void Map_Bonus_Remove_Persistent(   struct Map *map, i32 army);
void Map_Bonus_Remove_onMove_Unit(  struct Map *map, tnecs_entity ent);
void Map_Bonus_Remove_Turn_End_Unit(struct Map *map, tnecs_entity ent);

/* -- Entities -- */
tnecs_entity *Map_Get_onField(struct Map *map, i32 army);

/* -- Flags -- */
b32 Map_isWon(      const Map *map);
b32 Map_isLost(     const Map *map);
b32 Map_isUpdate(   const Map *map);

#endif /* MAP_H */
