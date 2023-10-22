#ifndef MAP_H
#define MAP_H

#include "macros.h"
#include "enums.h"
#include "tnecs.h"
#include "map/tiles.h"
#include "map/ontile.h"
#include "map/find.h"
#include "map/path.h"
#include "index_shader.h"
#include "arrow.h"
#include "nmath.h"
#include "unit.h"
#include "position.h"
#include "sprite.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;

/* --- ENUMS --- */
#define ARROW_FILENAME ".."DIR_SEPARATOR"assets"DIR_SEPARATOR"GUI"DIR_SEPARATOR"arrow.png"

/* --- STRUCTS --- */
struct Map_condition {
    i8 condition_type;
    /* --- CRITERIA --- */
    /* -- WHO -- */
    i8     lvl;        /* -1 for anyone    */
    i8     sex;        /* -1 for anyone    */
    i16    army;       /* -1 for anyone    */
    i16    unit;       /* -1 for anyone    */
    i8     unit_class; /* -1 for anyone    */
    /* -- WHERE -- */
    i16    col_min;    /* -1 for anywhere  */
    i16    col_max;    /* -1 for anywhere  */
    i16    row_min;    /* -1 for anywhere  */
    i16    row_max;    /* -1 for anywhere  */
    /* -- WHAT -- */
    i16    equippedL;  /* -1 for anything  */
    i16    equippedR;  /* -1 for anything  */
    /* -- WHEN --  */
    i16    min;        /* -1 for anytime   */
    i16    max;        /* -1 for anytime   */

    /* --- GENERAL PURPOSE COUNTER --- */
    i16 counter;

    /* --- REWARDS --- */
    i16    gold;
    i16    item;
    i16    scene;
    i16    event;
    i8     substate;
};

struct Map {
    u8      json_element;  /* JSON_ELEM_bOFFSET = 0 */
    char   *json_filename; /* JSON_ELEM_bOFFSET = 1 */

    /* --- BASICS --- */
    u8 turn; /* Automatic loss if turn 255. */
    i32 boss;
    u8 row_len;
    u8 col_len;
    i8 chapter;
    i32 tilesize[TWO_D];
    struct Arrow    *arrow;
    tnecs_world_t   *world;
    SDL_Renderer    *renderer;

    /* --- SHADERS --- */
    struct Tilemap_Shader *tilemap_shader;

    /* --- STACK_MAP --- */
    u8 stack_mode;

    /* - Actual stackmaps - */
    i32 *stacked_dangermap;         /* 2D dynamic array */
    i32 *stacked_global_dangermap;  /* 2D dynamic array */

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
    u8  frames;                /* number of map frames.    < 1 if not animated */
    u8  frames_shadow;         /* number of shadow frames. < 1 if not animated */
    u8  shadow_frame_pause;    /* number of frames to wait until change shadow animated frame */
    u8  tilemap_frame_pause;   /* number of frames to wait until change tilemap animated frame */
    u8  shadow_frame_counter;
    u8  tilemap_frame_counter;
    float shadow_frame_factor;  /* framerate * factor = pause */
    float tilemap_frame_factor; /* framerate * factor = pause */

    /* --- COSTMAP, MOVEMAP, ATTACKMAP... --- */
    i32 *temp;                  /* 2D dynamic array */
    i32 *costmap;               /* 2D dynamic array */
    i32 *movemap;               /* 2D dynamic array */
    i32 *tilemap;               /* 2D dynamic array [row * col_len + col] */
    i32 *healtomap;             /* 2D dynamic array */
    i32 *dangermap;             /* 2D dynamic array */
    i32 *healtolist;            /* 2D dynamic array */
    i32 *healfrommap;           /* 2D dynamic array */
    i32 *global_dangermap;      /* 2D dynamic array */
    /* attacktomap: Tiles where unit can attack, 2D map  */
    i32 *attacktomap;           /* 2D dynamic array */
    /* attacktolist: List of tiles where unit can attack */
    i32 *attacktolist;          /* 2D dynamic array */
    i32 *attackfrommap;         /* 2D dynamic array */
    i32 *global_rangemap;        /* 2D dynamic array */
    float *fcostmap;            /* 2D dynamic array */
    float *fmovemap;            /* 2D dynamic array */
    tnecs_entity_t *unitmap;    /* [row * col_len + col] */

    /* --- PALETTES --- */
    SDL_Palette *palette_default;
    u8 *palettemap;    // [row * col_len + col]
    i32 *temp_palette;  // [row * col_len + col]

    /* --- AGONY --- */
    struct Agony_timer *agony_stack;
    u8 num_agonizing;
    tnecs_entity_t costmap_ent;

    /* --- ARMIES --- */
    tnecs_entity_t *units_onfield;
    tnecs_entity_t *friendlies_onfield;
    tnecs_entity_t *enemies_onfield;
    i8 num_enemies;
    i8 num_friendlies;
    u8 *armies_onfield;
    u8 num_units_onfield;
    u8 num_armies_onfield;
    u8 num_enemies_onfield;
    u8 num_friendlies_onfield;
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
    tnecs_entity_t *doors_ent;  /* breakable doors are here */
    tnecs_entity_t *chests_ent;
    tnecs_entity_t *breakables_ent;
    u16 door_num;
    u16 chest_num;
    u16 breakable_num;

    /* --- MAP SWITCHES --- */
    bool update             : 1;
    bool seized             : 1; /* maybe unnecessary if turn system. */
    bool bossdied           : 1;
    bool show_move          : 1;
    bool show_heal          : 1;
    bool show_grid          : 1;
    bool show_icons         : 1;
    bool show_attack        : 1;
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

void Map_dArrays_Free(struct Map *map);
void Map_dArrays_Init(struct Map *map, const struct Settings *settings);

static void _Map_Tilemap_Shader_Init( struct Map *map);
static void _Map_Tilemap_Shader_Free( struct Map *map);
void Map_Tilemap_Surface_Init(struct Map *map);
void Map_Tilemap_Surface_Free(struct Map *map);
void Map_Tilemap_Texture_Init(struct Map *map);
void Map_Tilemap_Texture_Free(struct Map *map);

static void _Map_Tilesindex_Init(struct Map  *map);
static void _Map_Tilesindex_Free(struct Map  *map);

void Map_Tilesprites_Init(struct Map *map, size_t tiles_num);
void Map_Tilesprites_Free(struct Map *map);

/* --- I/O --- */
void Map_readJSON(       void *input, const cJSON *const jmap);
void Map_writeJSON(const void *input, cJSON             *jmap);

/* --- Map events / Triggers --- */
void Map_Turn_Increment(struct Map *map);

#endif /* MAP_H */
