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

    Point pixel_pos = {1, 1};
    Point tilemap_pos = {1, 1};
    Point offset = {1, 1};
    Point pixel_boundsmin = {0, 100};
    Point pixel_boundsmax = {0, 100};
    Point tilemap_boundsmin = {0, 100};
    Point tilemap_boundsmax = {0, 100};

    Point * position = nullptr;
    Point * boundsmin = nullptr;
    Point * boundsmax = nullptr;
    short int bounds[4]; //xmin, xmax, ymin, ymax

    bool newPos(short int newx, short int newy);
    void replaceInbounds();
    void whichSpace();
public:
    Position();
    Position(short int in_x, short int in_y);
    Position(short int * position);
    Position(short int in_x, short int in_y, short int xmin, short int xmax, short int ymin, short int ymax);
    Position(short int in_x, short int in_y, short int in_bounds[4]);
    Position(short int in_x, short int in_y, std::vector<short int> in_bounds);

    void setBounds(Point in_boundsmin, Point in_boundsmax);
    void setBounds(short int xmin, short int xmax, short int ymin, short int ymax);
    void setBounds(short int in_bounds[4]);
    void setBounds(std::vector<short int> in_bounds);
    short int * getBounds();
    short int * getTilemapBounds();
    short int * getPixelBounds();

    void setOffset(short int in_offset[2]);
    void setOffset(Point in_offset);

    void setOffset(short int xoffset, short int yoffset);
    Point getOffset();

    void setUpdatable(bool in_updatable);
    bool isUpdatable();

    bool isPeriodic();
    void setPeriodic(bool in_periodic);

    bool isonTilemap();
    void setonTilemap(bool in_tilemap);

    bool setTilemapPos(Point in_pos);
    bool setTilemapPos(short int in_x, short int in_y);
    bool setPixelPos(Point in_pos);
    bool setPixelPos(short int in_x, short int in_y);
    bool setPos(Point * in_pos);
    bool setPos(Point in_pos);
    bool setPos(short int * in_pos);
    bool setPos(short int in_x, short int in_y);
    bool addPos(short int move_x, short int move_y);
    Point getPos();
    Point getTilemapPos();
    Point getPixelPos();
};

#endif /* POSITION_HPP */