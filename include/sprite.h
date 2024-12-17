#ifndef SPRITE_H
#define SPRITE_H

#include "enums.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Index_Shader;
struct Position;
struct cJSON;
typedef struct cJSON cJSON;

/* --- Spritesheet --- */
/* -- ants -- */
#define MAP_SPRITE_FRAME_NUM 4

enum SPRITESHEET {
    SPRITESHEET_COL_LEN       =  8,
    DEFAULT_TILESPRITE_BUFFER = 16,
};

enum MAP_UNIT_TILESIZE {
    MAP_UNIT_TILESIZE_X = 2,
    MAP_UNIT_TILESIZE_Y = 2,
};

enum MAP_UNIT_LOOPS {
    MAP_UNIT_LOOP_IDLE      =  0,
    MAP_UNIT_LOOP_TAUNT     =  1,
    MAP_UNIT_LOOP_ITEM      =  2,
    MAP_UNIT_LOOP_AGONY     =  3,
    MAP_UNIT_LOOP_ATTACKR   =  4,
    MAP_UNIT_LOOP_ATTACKT   =  5,
    MAP_UNIT_LOOP_ATTACKL   =  6,
    MAP_UNIT_LOOP_ATTACKB   =  7,
    MAP_UNIT_LOOP_MOVER     =  8,
    MAP_UNIT_LOOP_MOVET     =  9,
    MAP_UNIT_LOOP_MOVEL     = 10,
    MAP_UNIT_LOOP_MOVEB     = 11,
    MAP_UNIT_LOOP_STANCE    = 12,
    MAP_UNIT_LOOP_NUM       = 13,
};

extern const int map_unit_offsets[MAP_UNIT_LOOP_NUM][TWO_D];

struct Spritesheet {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

    /* metadata for 8 column spritesheet */
    SDL_Surface *surface;           /* pixels with NES color palette        */
    SDL_Surface *surface_shaded;    /* shaded pixels to be shown on screen  */
    SDL_Palette *palette;

    i16  *loops_pos;    /* [loop] flattened index for 8 cols, starting top left */
    i8   *frames;       /* [loop], number of frames */
    i16 **speeds;       /* [loop][frame], ms */
    i8   *loop_modes;   /* [loop] */
    i32  *loops;        /* [loop] */
    i8    loop_num;     /* mostly for debugging, checking spritesheet size */
    i8    current_loop;
    i8    current_frame;
    i8    frame_i;
};
extern const struct Spritesheet Spritesheet_default;

/* --- Constructor/Destructors --- */
void Spritesheet_Free(struct Spritesheet *ss);

/* --- Animation --- */
void Spritesheet_Loop_Set(struct Spritesheet *ss, int l, SDL_RendererFlip f);

/* --- I/O --- */
void Spritesheet_readJSON(void *ss,  cJSON *jss);

/* --- Sprite --- */
/* -- ants -- */
enum SPRITE {
    DEFAULT_SPRITE_SRC_W = 32,
    DEFAULT_SPRITE_SRC_H = 32,
};

typedef struct Sprite {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

    SDL_Rect srcrect; /* x,y,w,h */
    SDL_Rect dstrect; /* x,y,w,h */
    SDL_Rect srcrect_shadow; /* x,y,w,h -> where sprite gets shaded */
    u16 tilesize[TWO_D];
    SDL_RendererFlip flip;

    SDL_Texture *texture;  /* pixels actually shown on screen */

    /* Index_Shaders all Sprites need */
    struct Index_Shader *shader_darken;
    struct Index_Shader *shader_lighten;
    struct Point scale;

    /* Index_Shaders specific to this sprite */
    struct Index_Shader *shader_any;

    /* Each sprite can have ONE spritesheets -> one timer per entity */
    struct Spritesheet *spritesheet;

    s8 asset_name;

    b32 map_unit : 1;
    b32 visible  : 1;
} Sprite;
extern const struct Sprite Sprite_default;

/* --- dstrect_funcs array --- */
typedef void (*dstrect_func_t)(struct Sprite *, struct Point *, struct Camera *);
extern dstrect_func_t dstrect_funcs[TWO_D][2];

/* --- Constructor/Destructors --- */
void Sprite_Free(struct Sprite *s);

/* --- Initialization --- */
void Sprite_Load(               struct Sprite *s,  char *n, SDL_Renderer *r);
void Sprite_Tilesize_Set(       struct Sprite *s, u16 *t);
void Sprite_Map_Unit_Load(      struct Sprite *s, struct Unit *u, SDL_Renderer *r);
void Sprite_defaultShaders_Load(struct Sprite *s);

/* -- rect computation -- */
void Cursor_Rects_Init(      struct Sprite *s);
void Sprite_Rects_Init(      struct Sprite *s);
void Cursor_Dstrect_Relative(struct Sprite *s, struct Point *, struct Camera *);
void Cursor_Dstrect_Absolute(struct Sprite *s, struct Point *, struct Camera *);
void Sprite_Dstrect_Relative(struct Sprite *s, struct Point *, struct Camera *);
void Sprite_Dstrect_Absolute(struct Sprite *s, struct Point *, struct Camera *);

/* --- Shading --- */
void Sprite_Shade(       struct Sprite *s, SDL_Renderer *r, struct Index_Shader *is);
void Sprite_Unveil(      struct Sprite *s, SDL_Renderer *r);
void Sprite_Darken(      struct Sprite *s, SDL_Renderer *r);
void Sprite_Lighten(     struct Sprite *s, SDL_Renderer *r);
void Sprite_Palette_Swap(struct Sprite *s, SDL_Palette *p, SDL_Renderer *r);

/* --- Animation --- */
void Sprite_Draw(             struct Sprite *s, SDL_Renderer *r);
void Sprite_Animation_Loop(   struct Sprite *s);
void Sprite_Animation_Restart(struct Sprite *s, int loop);

/* --- I/O --- */
void Sprite_readJSON(void *s,  cJSON *js);

#endif /* SPRITE_H */
