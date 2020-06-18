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
    float scale[2] = {0.0f, 0.0f};
    int16_t bounds[4]; //xmin, xmax, ymin, ymax

    bool newPos(int16_t newx, int16_t newy);
    void replaceInbounds();
    void whichSpace();
public:
    Position();
    Position(int16_t in_x, int16_t in_y);
    Position(int16_t * position);
    Position(int16_t in_x, int16_t in_y, int16_t xmin, int16_t xmax, int16_t ymin, int16_t ymax);
    Position(int16_t in_x, int16_t in_y, int16_t in_bounds[4]);
    Position(int16_t in_x, int16_t in_y, std::vector<int16_t> in_bounds);

    void setBounds(Point in_boundsmin, Point in_boundsmax);
    void setBounds(int16_t xmin, int16_t xmax, int16_t ymin, int16_t ymax);
    void setBounds(int16_t in_bounds[4]);
    void setBounds(std::vector<int16_t> in_bounds);
    int16_t * getBounds();
    Point getBoundsmin();
    Point getBoundsmax();
    int16_t * getTilemapBounds();
    int16_t * getPixelBounds();

    void setOffset(int16_t in_offset[2]);
    void setOffset(Point in_offset);

    void setOffset(int16_t xoffset, int16_t yoffset);
    Point getOffset();

    float * getScale();
    void setScale(float in_x, float in_y);

    void setUpdatable(bool in_updatable);
    bool isUpdatable();

    bool isPeriodic();
    void setPeriodic(bool in_periodic);

    bool isonTilemap();
    void setonTilemap(bool in_tilemap);

    bool setTilemapPos(Point in_pos);
    bool setTilemapPos(int16_t in_x, int16_t in_y);
    bool setPixelPos(Point in_pos);
    bool setPixelPos(int16_t in_x, int16_t in_y);
    bool setPos(Point * in_pos);
    bool setPos(Point in_pos);
    bool setPos(int16_t * in_pos);
    bool setPos(int16_t in_x, int16_t in_y);
    bool addPos(int16_t move_x, int16_t move_y);
    Point getPos();
    Point getTilemapPos();
    Point getPixelPos();

    Point tilemap2pixel(Point in_point);
    Point tilemap2pixel(int16_t tilemap_x, int16_t tilemap_y);
    Point pixel2tilemap(Point in_point);
    Point pixel2tilemap(int16_t pixel_x, int16_t pixel_y);
};

#endif /* POSITION_HPP */