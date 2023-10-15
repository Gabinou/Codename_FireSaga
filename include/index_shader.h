#ifndef INDEX_SHADER_H
#define INDEX_SHADER_H

#include "SDL2/SDL.h"
#include "structs.h"
#include "utilities.h"
#include "enums.h"
#include "types.h"
#include "nmath.h"
#include "map/render.h"
#include "cJSON.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;

/* --- pixels --- */
extern uf8 *pixels2list_noM(uf8 *matrix, uf8 *list, size_t row_len, size_t col_len);
extern uf8 *pixels2list(uf8 *matrix, size_t row_len, size_t col_len);
extern uf8 *pixels_and_noM(uf8 *out, uf8 *matrix1, uf8 *matrix2, size_t arr_len);
extern uf8 *pixels_and(uf8 *matrix1, uf8 *matrix2, size_t arr_len);


/* --- SHADING NOISE --- */
extern int *matrix_circ_noise(int *m, int ox, int oy, size_t w, size_t h,  size_t rl, size_t cl);
extern int *matrix_rect_noise(int *m, int ox, int oy, size_t d, size_t rl, size_t cl);

/* --- TILEMAP_SHADER --- */
struct Tilemap_Shader {
    uf8   *to;                      /* [from] is a palette_table                            */
    uf8  **shadowtile_pixels_lists; /* [shadow_tile_order] -> tilesize*tilesize dynarray    */
    uf8    *inTilemap_indices;      /* [shadow_tile_order] -> [shadow_tile_index]           */
    uf8    *inTilemap_orders;       /* [shadow_tile_index] -> [shadow_tile_order]           */
    size_t *shadowtile_pixels_num;  /* [shadow_tile_order]                                  */
    i32   **shadow_tilemaps;        /* [frame][row * col_len + col] -> [shadow_tile_order]  */
    struct Map *map;
    uf8 frames;
    uf8 shadowtile_num;
};
extern struct Tilemap_Shader Tilemap_Shader_default;

/* -- Constructor/Destructor -- */
extern void Tilemap_Shader_Free(       struct Tilemap_Shader *s);
extern void Tilemap_Shader_Alloc(      struct Tilemap_Shader *s, size_t tilenum);
extern void Tilemap_Shader_Shadow_Free(struct Tilemap_Shader *s);

/* -- Loading -- */
extern void Tilemap_Shader_Load_JSON(struct Tilemap_Shader *s, const cJSON *const jm,
                                     const cJSON *const st);
extern void Tilemap_Shader_Load_Tileset_JSON(struct Tilemap_Shader *s, const cJSON *const st);
extern void Tilemap_Shader_Load_Tilemap_JSON(struct Tilemap_Shader *s, const cJSON *const jmap);
extern void Tilemap_Shader_Load_Tileset_pixels(struct Tilemap_Shader *s, const char *f,
                                               size_t tnum, i32 tilesize[TWO_D]);

/* -- Shading -- */
extern SDL_Surface *Tilemap_Shade_Surface(struct Tilemap_Shader *ts, SDL_Surface *su,
                                          uf8 f, const struct Settings *st, struct Camera *cam);

/* --- INDEX_SHADER --- */
/* Shade specific regions of tiles using a mask.                         */
/* Modifies the index value of the pixels, without changing the palette. */
struct Index_Shader {
    uf8 *to;            /* [from] is a palette_table */
    uf8 *pixels_list;   /* POINTS_LIST: 2D -> X: [2*i], Y: [2*i+1], POINTS_MATRIX */
    uf8 *shaded_pixels;
    size_t pixels_num;  /* POINTS_LIST: 2D -> X: [2*i], Y: [2*i+1], POINTS_MATRIX */
};
extern struct Index_Shader Index_Shader_default;

/* -- Constructor/Destructor -- */
extern void Index_Shader_Free(struct Index_Shader *is);

/* -- Loading -- */
extern void Index_Shader_Load(struct Index_Shader *is, SDL_Surface *s, SDL_Rect *r);

/* -- Shading -- */
extern void Index_Shade_Rect(  uf8 *to, SDL_Surface *unlocked_surface, SDL_Rect *r);
extern void Index_Shade_Pixels(uf8 *to, SDL_Surface *us, uf8 *pl, size_t pn, size_t x, size_t y);
extern SDL_Surface *Index_Shade_Surface(struct Index_Shader *is, SDL_Surface *s, SDL_Rect *r);

#endif /* INDEX_SHADER_H */
