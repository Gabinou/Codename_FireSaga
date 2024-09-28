#ifndef MAP_TILES_H
#define MAP_TILES_H

#include "map/map.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;

/* --- Tiles --- */
void Map_Tiles_Load(  struct Map *map);
void Map_Tiles_Free(  struct Map *map);
u8   Map_Tile_Order(  struct Map *map, i32 tile);

void Map_Tilesets_Free(     struct Map *map);
void Map_Tilesets_Load(     struct Map *map);
void Map_Tileset_Stack_Add( struct Map *map);
void Map_Tileset_newPalette(struct Map *map, i32 tile, u8 palette);

/* --- Place objects on tilemap --- */
/* Override tile if Chest, Door or Breakable on it*/
void Map_Tilemap_Doors(     struct Map *map);
void Map_Tilemap_Chests(    struct Map *map);
void Map_Tilemap_MapObjects(struct Map *map);
void Map_Tilemap_Breakables(struct Map *map);

#endif /* MAP_TILES_H */
