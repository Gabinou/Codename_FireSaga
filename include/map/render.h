#ifndef MAP_RENDER_H
#define MAP_RENDER_H

#include "map/map.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;

/* --- Palettes --- */
extern uf8  Map_Palette_Order(           struct Map *map, uf8 palette);
extern void Map_Palettemap_Reset(        struct Map *map);
extern void Map_Palettemap_addMap(       struct Map *map, uf8 *pm);
extern void Map_Palettemap_addList(      struct Map *map, i32 *list, uf8 palette);
extern void Map_Palettemap_Autoset(      struct Map *map, uf16 flagsum);
extern void Map_Palettemap_SetwMap(      struct Map *map, uf8 *pm);
extern void Map_Palettemap_addMap_scalar(struct Map *map, i32 *pm, uf8 palette);

extern void Map_swappedTextures_All(struct Map *map);

/* --- Utils --- */
extern void Map_Renderer_Set(struct Map *map, SDL_Renderer *renderer);
extern void Map_Frame_Pauses(struct Map *map, const struct Settings *settings);

/* -- Compute visible map bounds -- */
extern void Map_Visible_Tiles(struct Map *map, const struct Settings *s, struct Camera *c);
extern void Map_Visible_Bounds(uf8 *min, uf8 *max, size_t row_len, size_t col_len,
                               i32 tilesize[TWO_D], const struct Point *res,
                               struct Camera *camera);

/* --- Map stitching --- */
/* -- Surface stitching: for shading -- */
extern SDL_Surface *Map_Tilemap_Surface_Stitch(struct Map *map);

/* -- Texture stitching: if no shading -- */
extern SDL_Texture *Map_Tilemap_Texture_Stitch(struct Map *map, struct SDL_Texture *rt);

/* --- Rendering --- */

extern void Map_Draw(     struct Map    *map, const struct Settings    *s,
                          struct Camera *c,         struct SDL_Texture *rt);
extern void Map_Update(   struct Map    *map, const struct Settings    *s,
                          struct Camera *c,         struct SDL_Texture *rt);
extern void Map_Grid_Draw(struct Map    *map, const struct Settings    *s, struct Camera *c);

extern bool Map_Tilemap_newFrame(  struct Map *map);
extern bool Map_Shadowmap_newFrame(struct Map *map);

#endif /* MAP_RENDER_H */
