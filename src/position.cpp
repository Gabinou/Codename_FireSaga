
#include "position.hpp"

Position::Position() {
    whichSpace();
    setPos(0, 0);
}

Position::Position(int16_t in_x, int16_t in_y) {
    whichSpace();
    setPos(in_x, in_y);
}

Position::Position(int16_t * position) {
    whichSpace();
    setPos(position[0], position[1]);
}

Position::Position(int16_t in_x, int16_t in_y,
                   int16_t xmin, int16_t xmax,
                   int16_t ymin, int16_t ymax)
    : Position::Position(in_x, in_y) {
    whichSpace();
    setBounds(xmin, xmax, ymin, ymax);
}

Position::Position(int16_t in_x, int16_t in_y,
                   int16_t in_bounds[4])
    : Position::Position(in_x, in_y) {
    whichSpace();
    setBounds(in_bounds);
}

Position::Position(int16_t in_x, int16_t in_y,
                   std::vector<int16_t> in_bounds)
    : Position::Position(in_x, in_y) {
    whichSpace();
    setBounds(in_bounds);
}

void Position::replaceInbounds() {
    if (position != nullptr) {
        if (position->x < boundsmin->x) {
            position->x = boundsmin->x;
        }

        if (position->x > boundsmax->x) {
            position->x = boundsmax->x;
        }

        if (position->y < boundsmin->y) {
            position->y = boundsmin->y;
        }

        if (position->y > boundsmax->y) {
            position->y = boundsmax->y;
        }
    }
}

void Position::setBounds(Point in_boundsmin, Point in_boundsmax) {
    setBounds(in_boundsmin.x, in_boundsmax.x, in_boundsmin.y, in_boundsmax.x);
}

void Position::setBounds(int16_t xmin, int16_t xmax, int16_t ymin, int16_t ymax) {
    boundsmin->x = xmin;
    boundsmax->x = xmax;
    boundsmin->y = ymin;
    boundsmax->y = ymax;
    replaceInbounds();
}

void Position::setBounds(int16_t in_bounds[4]) {
    setBounds(in_bounds[0], in_bounds[1], in_bounds[2], in_bounds[3]);
}

void Position::setBounds(std::vector<int16_t> in_bounds) {
    setBounds(in_bounds[0], in_bounds[1], in_bounds[2], in_bounds[3]);
}

void Position::setOffset(Point in_offset) {
    offset.x = in_offset.x;
    offset.y = in_offset.y;
}

void Position::setOffset(int16_t xoffset, int16_t yoffset) {
    offset.x = xoffset;
    offset.y = yoffset;
}

void Position::setOffset(int16_t in_offset[2]) {
    offset.x = in_offset[0];
    offset.y = in_offset[1];
}

Point Position::tilemap2pixel(Point in_point) {
    return (tilemap2pixel(in_point.x, in_point.y));
}

Point Position::tilemap2pixel(int16_t tilemap_x, int16_t tilemap_y) {
    Point pixel_pos;
    pixel_pos.x = scale[0] * (tilemap_x + offset.x);
    pixel_pos.y = scale[1] * (tilemap_y + offset.y);
    return (pixel_pos);
}

Point Position::pixel2tilemap(Point in_point) {
    return (pixel2tilemap(in_point.x, in_point.y));
}

Point Position::pixel2tilemap(int16_t pixel_x, int16_t pixel_y) {
    Point tile_pos;
    tile_pos.x = (int)(pixel_x / scale[0]) - offset.x;
    tile_pos.y = (int)(pixel_y / scale[1]) - offset.y;
    return (tile_pos);
}

int16_t * Position::getTilemapBounds() {
    bounds[0] = tilemap_boundsmin.x;
    bounds[1] = tilemap_boundsmax.x;
    bounds[2] = tilemap_boundsmin.y;
    bounds[3] = tilemap_boundsmax.y;
    return (bounds);
}

int16_t * Position::getPixelBounds() {
    bounds[0] = pixel_boundsmin.x;
    bounds[1] = pixel_boundsmax.x;
    bounds[2] = pixel_boundsmin.y;
    bounds[3] = pixel_boundsmax.y;
    return (bounds);
}

int16_t * Position::getBounds() {
    bounds[0] = boundsmin->x;
    bounds[1] = boundsmax->x;
    bounds[2] = boundsmin->y;
    bounds[3] = boundsmax->y;
    return (bounds);
}

Point Position::getOffset() {
    return (offset);
}

bool Position::isPeriodic() {
    return (periodic);
}

void Position::setPeriodic(bool in_periodic) {
    periodic = in_periodic;
}

bool Position::isonTilemap() {
    return (onTilemap);
}

void Position::setonTilemap(bool in_tilemap) {
    onTilemap = in_tilemap;
    whichSpace();
}

void Position::setUpdatable(bool in_updatable) {
    updatable = in_updatable;
}

bool Position::isUpdatable() {
    return (updatable);
}

bool Position::setTilemapPos(Point in_pos) {
    return (setTilemapPos(in_pos.x, in_pos.y));
}

bool Position::setTilemapPos(int16_t in_x, int16_t in_y) {
    bool out;
    position = &tilemap_pos;
    out = setPos(in_x, in_y);
    whichSpace();
    return (out);
}

bool Position::setPixelPos(Point in_pos) {
    return (setPixelPos(in_pos.x, in_pos.y));
}

bool Position::setPixelPos(int16_t in_x, int16_t in_y) {
    bool out;
    position = &pixel_pos;
    out = setPos(in_x, in_y);
    whichSpace();
    return (out);
}

bool Position::setPos(Point * in_pos) {
    return (setPos(in_pos->x, in_pos->y));
}

bool Position::setPos(Point in_pos) {
    return (setPos(in_pos.x, in_pos.y));
}

bool Position::setPos(int16_t * in_pos) {
    return (setPos(in_pos[0], in_pos[1]));
}

bool Position::setPos(int16_t in_x, int16_t in_y) {
    int16_t newx = in_x + offset.x;
    int16_t newy = in_y + offset.y;
    return (newPos(newx, newy));
}

bool Position::addPos(int16_t move_x, int16_t move_y) {
    int16_t newx = move_x + position->x;
    int16_t newy = move_y + position->y;
    return (newPos(newx, newy));
}

float * Position::getScale() {
    return (scale);
}

void Position::setScale(float in_x, float in_y) {
    scale[0] = in_x;
    scale[1] = in_y;
}

void Position::whichSpace() {
    if (onTilemap) {
        position = &tilemap_pos;
        boundsmin = &tilemap_boundsmin;
        boundsmax = &tilemap_boundsmax;
    } else {
        position = &pixel_pos;
        boundsmin = &pixel_boundsmin;
        boundsmax = &pixel_boundsmax;
    }
}

bool Position::newPos(int16_t newx, int16_t newy) {
    bool moved = false;

    if (updatable) {
        if (periodic) {
            // SDL_Log("Periodic.");

            while (newx < boundsmin->x) {
                newx += boundsmax->x - boundsmin->x + 1;
            }

            while (newx > boundsmax->x) {
                newx -= boundsmax->x - boundsmin->x + 1;
            }

            while (newy < boundsmin->y) {
                newy += boundsmax->y - boundsmin->y + 1;
            }

            while (newy > boundsmax->y) {
                newy -= boundsmax->y - boundsmin->y + 1;
            }

            // newx = (newx - 1) % (boundsmax->x - boundsmin->x + 1) + boundsmin->x; // don't know why these don't work.
            // newy = (newy - 1) % (boundsmax->y - boundsmin->y + 1) + boundsmin->y;
            // Python % is modulo. C/C++ % is remainder.
        }

        if ((newx > boundsmin->x) && (newx < boundsmax->x) && (position->x != newx)) {
            position->x = newx;
            moved = true;
        } else {
            if (newx <= boundsmin->x) {
                position->x = boundsmin->x;
            }

            if (newx >= boundsmax->x) {
                position->x = boundsmax->x;
            }
        }

        if ((newy > boundsmin->y) && (newy < boundsmax->y) && (position->y != newy)) {
            position->y = newy;
            moved = true;
        } else {
            if (newy <= boundsmin->y) {
                position->y = boundsmin->y;
            }

            if (newy >= boundsmax->y) {
                position->y = boundsmax->y;
            }
        }
    }

    return (moved);
}


Point Position::getPos() {
    return (*position);
}

Point Position::getBoundsmin() {
    return (*boundsmin);
}

Point Position::getBoundsmax() {
    return (*boundsmax);
}

Point Position::getTilemapPos() {
    return (tilemap_pos);
}

Point Position::getPixelPos() {
    return (pixel_pos);
}
