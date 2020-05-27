#include "minctest.h"
#include "position.hpp"

void test_position() {
    SDL_Log("test_position");
    Position position;
    Point pos;

    position.setonTilemap(false);
    position.setOffset(0, 0);
    position.setBounds(0, 100, 0, 100);
    position.setPos(1, 1);
    pos = position.getPos();
    lok(pos.x == 1);
    lok(pos.y == 1);

    position.addPos(0, 2);
    pos = position.getPos();
    lok(pos.x == 1);
    lok(pos.y == 3);

    position.setBounds(0, 10, 0, 10);
    position.setPos(10, 10);
    pos = position.getPos();
    lok(pos.x == 10);
    lok(pos.y == 10);
    position.addPos(0, 1);
    pos = position.getPos();
    lok(pos.x == 10);
    lok(pos.y == 10);
    position.addPos(1, 0);
    pos = position.getPos();
    lok(pos.x == 10);
    lok(pos.y == 10);
    position.addPos(1, 1);
    pos = position.getPos();
    lok(pos.x == 10);
    lok(pos.y == 10);

    position.setPos(0, 0);
    pos = position.getPos();
    lok(pos.x == 0);
    lok(pos.y == 0);
    position.addPos(0, -1);
    pos = position.getPos();
    lok(pos.x == 0);
    lok(pos.y == 0);
    position.addPos(0, -1);
    pos = position.getPos();
    lok(pos.x == 0);
    lok(pos.y == 0);
    position.addPos(-1, 0);
    pos = position.getPos();
    lok(pos.x == 0);
    lok(pos.y == 0);
    position.addPos(-1, -1);
    pos = position.getPos();
    lok(pos.x == 0);
    lok(pos.y == 0);

    position.setUpdatable(false);
    position.addPos(1, 1);
    lok(pos.x == 0);
    lok(pos.y == 0);

    position.setUpdatable(true);
    position.setPeriodic(true);
    position.setBounds(0, 10, 0, 10);
    position.setPos(10, 10);
    position.addPos(1, 1);
    pos = position.getPos();
    lok(pos.x == 0);
    lok(pos.y == 0);
    position.setPos(0, 0);
    position.addPos(-1, -1);
    pos = position.getPos();
    lok(pos.x == 10);
    lok(pos.y == 10);
    position.setPos(0, 0);
    position.addPos(-23, -23);
    pos = position.getPos();
    lok(pos.x == 10);
    lok(pos.y == 10);
    position.setBounds(2, 10, 2, 10);
    position.setPos(2, 2);
    position.addPos(-1, -1);
    pos = position.getPos();
    lok(pos.x == 10);
    lok(pos.y == 10);
    position.setPos(2, 2);
    position.addPos(-2, -2);
    pos = position.getPos();
    lok(pos.x == 9);
    lok(pos.y == 9);

    Point tiles_pos;
    Position position2;
    short int tilesize = 32;
    position2.setScale(tilesize, tilesize);
    tiles_pos = position2.pixel2tilemap(tilesize - 4, tilesize - 4);
    lok(tiles_pos.x == 0);
    lok(tiles_pos.y == 0);

    tiles_pos = position2.pixel2tilemap(tilesize + 4, tilesize + 4);
    lok(tiles_pos.x == 0);
    lok(tiles_pos.y == 0);

    tiles_pos = position2.pixel2tilemap(tilesize * 2 + 4, tilesize * 2 + 4);
    lok(tiles_pos.x == 1);
    lok(tiles_pos.y == 1);

    tiles_pos = position2.pixel2tilemap(tilesize * 100 + 4, tilesize * 100 + 4);
    lok(tiles_pos.x == (position2.getBounds()[1] - position2.getOffset().x));
    lok(tiles_pos.y == (position2.getBounds()[3] - position2.getOffset().y));


}
