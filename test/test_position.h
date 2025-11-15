
#include "nourstest.h"
#include "position.h"

void test_position() {
    struct Position position = Position_default;
    // struct Point pos = {0};
    // struct Point *ptrpos = NULL;
    position.onTilemap = false;

    _Position_Bounds_Set(&position, 0, 100, 0, 100);
    _Position_Pos_Set(&position, 1, 1);

    nourstest_true(position.pixel_pos.x == 1);
    nourstest_true(position.pixel_pos.y == 1);
    _Position_Pos_Add(&position, 0, 2);

    nourstest_true(position.pixel_pos.x == 1);
    nourstest_true(position.pixel_pos.y == 3);
    _Position_Bounds_Set(&position, 0, 10, 0, 10);
    _Position_Pos_Set(&position, 10, 10);

    nourstest_true(position.pixel_pos.x == 10);
    nourstest_true(position.pixel_pos.y == 10);
    _Position_Pos_Add(&position, 0, 10);

    nourstest_true(position.pixel_pos.x == 10);
    nourstest_true(position.pixel_pos.y == 10);
    _Position_Pos_Add(&position, 10, 0);

    nourstest_true(position.pixel_pos.x == 10);
    nourstest_true(position.pixel_pos.y == 10);
    _Position_Pos_Add(&position, 1, 1);

    nourstest_true(position.pixel_pos.x == 10);
    nourstest_true(position.pixel_pos.y == 10);
    _Position_Pos_Set(&position, 0, 0);

    nourstest_true(position.pixel_pos.x == 0);
    nourstest_true(position.pixel_pos.y == 0);
    _Position_Pos_Add(&position, 0, -1);

    nourstest_true(position.pixel_pos.x == 0);
    nourstest_true(position.pixel_pos.y == 0);
    _Position_Pos_Add(&position, 0, -1);

    nourstest_true(position.pixel_pos.x == 0);
    nourstest_true(position.pixel_pos.y == 0);
    _Position_Pos_Add(&position, -1, 0);

    nourstest_true(position.pixel_pos.x == 0);
    nourstest_true(position.pixel_pos.y == 0);
    _Position_Pos_Add(&position, -1, -1);

    nourstest_true(position.pixel_pos.x == 0);
    nourstest_true(position.pixel_pos.y == 0);

    // struct Point tiles_pos      = {0};
    // struct Position position2   = {0};
    // uint_fast16_t tilesize      = 32;
    // position2.scale[0]          = tilesize;
    // position2.scale[1]          = tilesize;
    // position2.onTilemap         = false;
}
