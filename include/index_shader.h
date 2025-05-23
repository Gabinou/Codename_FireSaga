#ifndef INDEX_SHADER_H
#define INDEX_SHADER_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "structs.h"
#include "cJSON.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;
struct cJSON;

/* --- pixels --- */
u8 *pixels2list(    u8 *matrix,           size_t row_len, size_t col_len);
u8 *pixels2list_noM(u8 *matrix, u8 *list, size_t row_len, size_t col_len);

u8 *pixels_and(             u8 *matrix1, u8 *matrix2, size_t arr_len);
u8 *pixels_and_noM(u8 *out, u8 *matrix1, u8 *matrix2, size_t arr_len);

/* --- SHADING NOISE --- */
int *matrix_circ_noise(int *m, int ox, int oy, size_t w, size_t h,  size_t rl, size_t cl);
int *matrix_rect_noise(int *m, int ox, int oy, size_t d, size_t rl, size_t cl);

/* --- TILEMAP_SHADER --- */
struct Tilemap_Shader {
    u8   *to;                      /* [from] is a palette_table                            */
    u8  **shadowtile_pixels_lists; /* [shadow_tile_order] -> tilesize*tilesize dynarray    */
    u8    *inTilemap_indices;      /* [shadow_tile_order] -> [shadow_tile_index]           */
    u8    *inTilemap_orders;       /* [shadow_tile_index] -> [shadow_tile_order]           */
    size_t *shadowtile_pixels_num;  /* [shadow_tile_order]                                  */
    i32   **shadow_tilemaps;        /* [frame][row * col_len + col] -> [shadow_tile_order]  */
    struct Map *map;
    u8 frames;
    u8 shadowtile_num;
};
extern const struct Tilemap_Shader Tilemap_Shader_default;

/* -- ructor/Destructor -- */
void Tilemap_Shader_Free(       struct Tilemap_Shader *s);
void Tilemap_Shader_Alloc(      struct Tilemap_Shader *s, size_t tilenum);

/* -- Loading -- */
void Tilemap_Shader_Load_JSON(struct Tilemap_Shader *s, const cJSON *jm, const cJSON *st);
void Tilemap_Shader_Load_Tileset_JSON(struct Tilemap_Shader *s, const cJSON *st);
void Tilemap_Shader_Load_Tilemap_JSON(struct Tilemap_Shader *s, const cJSON *jmap);
void Tilemap_Shader_Load_Tileset_pixels(struct Tilemap_Shader *s,  char *f,
                                        size_t tnum, i32 tilesize[TWO_D]);

/* -- Shading -- */
SDL_Surface *Tilemap_Shade_Surface(struct Tilemap_Shader *ts, SDL_Surface *su,
                                   u8 f,  struct Settings *st);

/* --- INDEX_SHADER --- */
/* Shade specific regions of tiles using a mask.                         */
/* Modifies the index value of the pixels, without changing the palette. */
struct Index_Shader {
    u8 *to;            /* [from] is a palette_table */
    u8 *pixels_list;   /* ARRAY_LIST: 2D -> X: [2*i], Y: [2*i+1], ARRAY_MATRIX */
    u8 *shaded_pixels;
    size_t pixels_num;  /* ARRAY_LIST: 2D -> X: [2*i], Y: [2*i+1], ARRAY_MATRIX */
};
extern const struct Index_Shader Index_Shader_default;

/* -- ructor/Destructor -- */
void Index_Shader_Free(struct Index_Shader *is);

/* -- Loading -- */
void Index_Shader_Load(struct Index_Shader *is, SDL_Surface *s, SDL_Rect *r);

/* -- Shading -- */
SDL_Surface *Index_Shade_Surface(struct Index_Shader *is, SDL_Surface *s, SDL_Rect *r);

#endif /* INDEX_SHADER_H */
