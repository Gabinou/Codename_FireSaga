#ifndef POSITION_HPP
#define POSITION_HPP

#include "game.hpp"
#include "utilities.hpp"
#include "SDL2/SDL.h"
#include <functional>

class Position {
private:
    bool updatable = true;
    bool onTilemap = true;
    bool periodic = false;
    short int position[2] = {0};
    short int bounds[4] = {0, 255, 0, 255}; //xmin, xmax, ymin, ymax
    short int offset[2] = {0, 0}; //xmin, xmax, ymin, ymax
    void newPos(short int newx, short int newy);
public:
    Position();
    Position(short int in_x, short int in_y);
    Position(short int * position);
    Position(short int in_x, short int in_y, short int xmin, short int xmax, short int ymin, short int ymax);
    Position(short int in_x, short int in_y, short int in_bounds[4]);
    Position(short int in_x, short int in_y, std::vector<short int> in_bounds);

    void setBounds(short int xmin, short int xmax, short int ymin, short int ymax);
    void setBounds(short int in_bounds[4]);
    void setBounds(std::vector<short int> in_bounds);
    short int * getBounds();

    void setOffset(short int in_offset[2]);
    void setOffset(short int xoffset, short int yoffset);
    short int * getOffset();

    void setUpdatable(bool in_updatable);
    bool isUpdatable();

    bool isPeriodic();
    void setPeriodic(bool in_periodic);

    bool isonTilemap();
    void setonTilemap(bool in_tilemap);

    void setPos(short int * in_pos);
    void setPos(short int in_x, short int in_y);
    void addPos(short int move_x, short int move_y);
    short int * getPos();
};

#endif /* POSITION_HPP */