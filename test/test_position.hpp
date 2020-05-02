#include "minctest.h"
#include "position.hpp"

void test_position() {

    Position position;
    short int * pos;

    position.setPos(1, 1);
    pos = position.getPos();
    lok(pos[0] == 1);
    lok(pos[1] == 1);

    position.addPos(0, 2);
    pos = position.getPos();
    lok(pos[0] == 1);
    lok(pos[1] == 3);

    position.setBounds(0, 10, 0, 10);
    position.setPos(10, 10);
    pos = position.getPos();
    lok(pos[0] == 10);
    lok(pos[1] == 10);
    position.addPos(0, 1);
    pos = position.getPos();
    lok(pos[0] == 10);
    lok(pos[1] == 10);
    position.addPos(1, 0);
    pos = position.getPos();
    lok(pos[0] == 10);
    lok(pos[1] == 10);
    position.addPos(1, 1);
    pos = position.getPos();
    lok(pos[0] == 10);
    lok(pos[1] == 10);

    position.setPos(0, 0);
    pos = position.getPos();
    lok(pos[0] == 0);
    lok(pos[1] == 0);
    position.addPos(0, -1);
    pos = position.getPos();
    lok(pos[0] == 0);
    lok(pos[1] == 0);
    position.addPos(0, -1);
    pos = position.getPos();
    lok(pos[0] == 0);
    lok(pos[1] == 0);
    position.addPos(-1, 0);
    pos = position.getPos();
    lok(pos[0] == 0);
    lok(pos[1] == 0);
    position.addPos(-1, -1);
    pos = position.getPos();
    lok(pos[0] == 0);
    lok(pos[1] == 0);

    position.setUpdatable(false);
    position.addPos(1, 1);
    lok(pos[0] == 0);
    lok(pos[1] == 0);

    position.setUpdatable(true);
    position.setPeriodic(true);
    position.setBounds(0, 10, 0, 10);
    position.setPos(10, 10);
    position.addPos(1, 1);
    pos = position.getPos();
    lok(pos[0] == 0);
    lok(pos[1] == 0);
    position.setPos(0, 0);
    position.addPos(-1, -1);
    pos = position.getPos();
    lok(pos[0] == 10);
    lok(pos[1] == 10);
    position.setPos(0, 0);
    position.addPos(-23, -23);
    pos = position.getPos();
    lok(pos[0] == 10);
    lok(pos[1] == 10);
    position.setBounds(2, 10, 2, 10);
    position.setPos(2, 2);
    position.addPos(-1, -1);
    pos = position.getPos();
    lok(pos[0] == 10);
    lok(pos[1] == 10);
    position.setPos(2, 2);
    position.addPos(-2, -2);
    pos = position.getPos();
    lok(pos[0] == 9);
    lok(pos[1] == 9);
}
