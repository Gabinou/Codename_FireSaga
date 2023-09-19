#ifndef MAP_TILES_H
#define MAP_TILES_H

#include "map/map.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;

/* --- Tiles --- */
extern void Map_Tiles_Load(  struct Map *map);
extern void Map_Tiles_Free(  struct Map *map);
extern uf8  Map_Tile_Order(  struct Map *map, i32 tile);

extern void Map_Tilesize_Set(struct Map *map, i32 width, i32 height);

extern void Map_Tilesets_Free(     struct Map *map);
extern void Map_Tilesets_Load(     struct Map *map);
extern void Map_Tileset_Stack_Add( struct Map *map);
extern void Map_Tileset_newPalette(struct Map *map, i32 tile, uf8 palette);

/* --- Place objects on tilemap --- */
/* Override tile if Chest, Door or Breakable on it*/
extern void Map_Tilemap_Doors(     struct Map *map);
extern void Map_Tilemap_Chests(    struct Map *map);
extern void Map_Tilemap_MapObjects(struct Map *map);
extern void Map_Tilemap_Breakables(struct Map *map);

#endif /* MAP_TILES_H */
