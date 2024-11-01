#ifndef MAP_RENDER_H
#define MAP_RENDER_H

#include "map/map.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;

void Map_Units_Hide(struct Map *map);

/* --- Palettes --- */
void Map_Palettemap_Reset(  struct Map *map);
void Map_Palettemap_addMap( struct Map *map, i32 *pmap, u8 palette);
void Map_Palettemap_addList(struct Map *map, i32 *list, u8 palette);
void Map_Palettemap_Autoset(struct Map *map, u16 flagsum, tnecs_entity self);
void Map_Palettemap_SetwMap(struct Map *map, u8 *pm);

void Map_swappedTextures_All(struct Map *map);

/* --- Utils --- */
void Map_Renderer_Set(struct Map *map, SDL_Renderer *renderer);
void Map_Frame_Pauses(struct Map *map,  struct Settings *settings);

/* -- Compute visible map bounds -- */
void Map_Visible_Tiles(struct Map *map,  struct Settings *s, struct Camera *c);
void Map_Visible_Bounds(u8 *min, u8 *max, size_t row_len, size_t col_len,
                        i32 tilesize[TWO_D],  struct Point *res,
                        struct Camera *camera);

/* --- Map stitching --- */
/* -- Surface stitching: for shading -- */
SDL_Surface *Map_Tilemap_Surface_Stitch(struct Map *map);

/* -- Texture stitching: if no shading -- */
SDL_Texture *Map_Tilemap_Texture_Stitch(struct Map *map, struct SDL_Texture *rt);

/* --- Rendering --- */
void Map_Draw(     struct Map    *map,  struct Settings    *s,
                   struct Camera *c,         struct SDL_Texture *rt);
void Map_Update(   struct Map    *map,  struct Settings    *s,
                   struct Camera *c,         struct SDL_Texture *rt);
void Map_Grid_Draw(struct Map    *map,  struct Settings    *s, struct Camera *c);

void _Map_Perimeter_Draw(struct Map *map, struct Settings *s, struct Camera *c,
                         i32 *inside, SDL_Color color, struct Padding *p);
void Map_Perimeter_Draw_Danger(struct Map *map, struct Settings *s, struct Camera *c);
void Map_Perimeter_Draw_Aura(struct Map     *map,    struct Settings *settings,
                             struct Camera  *camera, struct Point pos,
                             struct Range    range,  int colori);

b32 Map_Tilemap_newFrame(  struct Map *map);
b32 Map_Shadowmap_newFrame(struct Map *map);

#endif /* MAP_RENDER_H */
