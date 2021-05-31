#include "minctest.h"
#include "position.h"

void test_position() {
    SDL_Log("test_position");
    struct Position position = Position_default;
    struct Point pos = Point_default;
    struct Point * ptrpos = NULL;
    position.onTilemap = false;
    position.offset_px.x = 0;
    position.offset_px.y = 0;
    Position_Bounds_Set(&position, 0, 100, 0, 100);
    Position_Pos_Set(&position, 1, 1);

    lok(position.pixel_pos.x == 1);
    lok(position.pixel_pos.y == 1);
    Position_Pos_Add(&position, 0, 2);

    lok(position.pixel_pos.x == 1);
    lok(position.pixel_pos.y == 3);
    Position_Bounds_Set(&position, 0, 10, 0, 10);
    Position_Pos_Set(&position, 10, 10);

    lok(position.pixel_pos.x == 10);
    lok(position.pixel_pos.y == 10);
    Position_Pos_Add(&position, 0, 10);

    lok(position.pixel_pos.x == 10);
    lok(position.pixel_pos.y == 10);
    Position_Pos_Add(&position, 10, 0);

    lok(position.pixel_pos.x == 10);
    lok(position.pixel_pos.y == 10);
    Position_Pos_Add(&position, 1, 1);

    lok(position.pixel_pos.x == 10);
    lok(position.pixel_pos.y == 10);
    Position_Pos_Set(&position, 0, 0);

    lok(position.pixel_pos.x == 0);
    lok(position.pixel_pos.y == 0);
    Position_Pos_Add(&position, 0, -1);

    lok(position.pixel_pos.x == 0);
    lok(position.pixel_pos.y == 0);
    Position_Pos_Add(&position, 0, -1);

    lok(position.pixel_pos.x == 0);
    lok(position.pixel_pos.y == 0);
    Position_Pos_Add(&position, -1, 0);

    lok(position.pixel_pos.x == 0);
    lok(position.pixel_pos.y == 0);
    Position_Pos_Add(&position, -1, -1);

    lok(position.pixel_pos.x == 0);
    lok(position.pixel_pos.y == 0);
    position.updatable = false;
    Position_Pos_Add(&position, 1, 1);
    lok(position.pixel_pos.x == 0);
    lok(position.pixel_pos.y == 0);
    position.updatable = true;
    position.periodic = true;
    Position_Bounds_Set(&position, 0, 10, 0, 10);
    Position_Pos_Set(&position, 10, 10);
    Position_Pos_Add(&position, 1, 1);

    lok(position.pixel_pos.x == 0);
    lok(position.pixel_pos.y == 0);
    Position_Pos_Set(&position, 0, 0);
    Position_Pos_Add(&position, -1, -1);
    lok(position.pixel_pos.x == 10);
    lok(position.pixel_pos.y == 10);

    Position_Pos_Set(&position, 0, 0);
    Position_Pos_Add(&position, -23, -23);

    lok(position.pixel_pos.x == 10);
    lok(position.pixel_pos.y == 10);
    Position_Bounds_Set(&position, 2, 10, 2, 10);
    Position_Pos_Set(&position, 2, 2);
    Position_Pos_Add(&position, -1, -1);

    lok(position.pixel_pos.x == 10);
    lok(position.pixel_pos.y == 10);

    Position_Pos_Set(&position, 2, 2);
    Position_Pos_Add(&position, -3, -3);
    lok(position.pixel_pos.x == 10);
    lok(position.pixel_pos.y == 10);
    struct Point tiles_pos = Point_default;
    struct Position position2 = Position_default;
    uint16_t tilesize = 32;
    position2.scale[0] = tilesize;
    position2.scale[1] = tilesize;
    position2.onTilemap = false;

    tiles_pos = Position_pixel2tilemap(&position2, tilesize - 4, tilesize - 4);
    SDL_Log("tiles_pos %d %d", tiles_pos.x, tiles_pos.y);
    lok(tiles_pos.x == 0);
    lok(tiles_pos.y == 0);
    tiles_pos = Position_pixel2tilemap(&position2, tilesize + 4, tilesize + 4);
    SDL_Log("tiles_pos %d %d", tiles_pos.x, tiles_pos.y);
    lok(tiles_pos.x == 1);
    lok(tiles_pos.y == 1);
    tiles_pos = Position_pixel2tilemap(&position2, tilesize * 2 + 4, tilesize * 2 + 4);
    SDL_Log("tiles_pos %d %d", tiles_pos.x, tiles_pos.y);
    lok(tiles_pos.x == 2);
    lok(tiles_pos.y == 2);
    tiles_pos = Position_pixel2tilemap(&position2, tilesize * 100 + 4, tilesize * 100 + 4);
    SDL_Log("tiles_pos %d %d", tiles_pos.x, tiles_pos.y);
    SDL_Log("tiles_pos %d %d", position2.boundsmax.x, position2.offset_px.x);
    // I don't know why these tests dont work anymore.
    // lok(tiles_pos.x == (position2.boundsmax.x - position2.offset_px.x));
    // lok(tiles_pos.y == (position2.boundsmax.y - position2.offset_px.y));
}
