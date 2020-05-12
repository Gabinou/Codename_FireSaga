
#include "position.hpp"

Position::Position() {
    whichPos();
    setPos(0, 0);
}

Position::Position(short int in_x, short int in_y) {
    whichPos();
    setPos(in_x, in_y);
}

Position::Position(short int * position) {
    whichPos();
    setPos(position[0], position[1]);
}

Position::Position(short int in_x, short int in_y,
                   short int xmin, short int xmax,
                   short int ymin, short int ymax)
    : Position::Position(in_x, in_y) {
    whichPos();
    setBounds(xmin, xmax, ymin, ymax);
}

Position::Position(short int in_x, short int in_y,
                   short int in_bounds[4])
    : Position::Position(in_x, in_y) {
    whichPos();
    setBounds(in_bounds);
}

Position::Position(short int in_x, short int in_y,
                   std::vector<short int> in_bounds)
    : Position::Position(in_x, in_y) {
    whichPos();
    setBounds(in_bounds);
}

void Position::replaceInbounds() {
    if (position != nullptr) {
        if (position->x <= boundsmin.x) {
            position->x = boundsmin.x - offset.x;
        }

        if (position->x >= boundsmax.x) {
            position->x = boundsmax.x - offset.x;
        }

        if (position->y <= boundsmin.y) {
            position->y = boundsmin.y - offset.y;
        }

        if (position->y >= boundsmax.y) {
            position->y = boundsmax.y - offset.y;
        }
    }
}

void Position::setBounds(Point in_boundsmin, Point in_boundsmax) {
    setBounds(in_boundsmin.x, in_boundsmax.x, in_boundsmin.y, in_boundsmax.x);
}

void Position::setBounds(short int xmin, short int xmax, short int ymin, short int ymax) {
    boundsmin.x = xmin + offset.x;
    boundsmax.x = xmax + offset.x;
    boundsmin.y = ymin + offset.y;
    boundsmax.y = ymax + offset.y;
    replaceInbounds();
}

void Position::setBounds(short int in_bounds[4]) {
    setBounds(in_bounds[0], in_bounds[1], in_bounds[2], in_bounds[3]);
}

void Position::setBounds(std::vector<short int> in_bounds) {
    setBounds(in_bounds[0], in_bounds[1], in_bounds[2], in_bounds[3]);
}

void Position::setOffset(Point in_offset) {
    offset.x = in_offset.x;
    offset.y = in_offset.y;
}

void Position::setOffset(short int xoffset, short int yoffset) {
    offset.x = xoffset;
    offset.y = yoffset;
}

void Position::setOffset(short int in_offset[2]) {
    offset.x = in_offset[0];
    offset.y = in_offset[1];
}

Point Position::getBoundsmin() {
    return (boundsmin);
}

Point Position::getBoundsmax() {
    return (boundsmax);
}

short int * Position::getBounds() {
    bounds[0] = boundsmin.x;
    bounds[1] = boundsmax.x;
    bounds[2] = boundsmin.y;
    bounds[3] = boundsmax.y;
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
    whichPos();
}

void Position::setUpdatable(bool in_updatable) {
    updatable = in_updatable;
}

bool Position::isUpdatable() {
    return (updatable);
}

bool Position::setPos(Point * in_pos) {
    return (setPos(in_pos->x, in_pos->y));
}

bool Position::setPos(Point in_pos) {
    return (setPos(in_pos.x, in_pos.y));
}

bool Position::setPos(short int * in_pos) {
    return (setPos(in_pos[0], in_pos[1]));
}

bool Position::setPos(short int in_x, short int in_y) {
    short int newx = in_x + offset.x;
    short int newy = in_y + offset.y;
    return (newPos(newx, newy));
}

bool Position::addPos(short int move_x, short int move_y) {
    short int newx = move_x + position->x;
    short int newy = move_y + position->y;
    return (newPos(newx, newy));
}

void Position::whichPos() {
    if (onTilemap) {
        position = &tilemap_pos;
    } else {
        position = &pixel_pos;
    }
}

bool Position::newPos(short int newx, short int newy) {
    bool moved = false;

    if (updatable) {
        if (periodic) {

            while (newx < boundsmin.x) {
                newx += boundsmax.x - boundsmin.x + 1;
            }

            while (newx > boundsmax.x) {
                newx -= boundsmax.x - boundsmin.x + 1;
            }

            while (newy < boundsmin.y) {
                newy += boundsmax.y - boundsmin.y + 1;
            }

            while (newy > boundsmin.y) {
                newy -= boundsmax.y - boundsmin.y + 1;
            }

            // newx = (newx - 1) % (boundsmax.x - boundsmin.x + 1) + boundsmin.x; // don't know why these don't work.
            // newy = (newy - 1) % (boundsmax.y - boundsmin.y + 1) + boundsmin.y;
            // Python % is modulo. C/C++ % is remainder.
        }


        if ((newx > boundsmin.x) && (newx < boundsmax.x) && (position->x != newx)) {
            position->x = newx;
            moved = true;
        } else {
            if (newx <= boundsmin.x) {
                position->x = boundsmin.x;
            }

            if (newx >= boundsmax.x) {
                position->x = boundsmax.x;
            }
        }

        if ((newy > boundsmin.y) && (newy < boundsmax.y) && (position->x != newy)) {
            position->y = newy;
            moved = true;
        } else {
            if (newy <= boundsmin.y) {
                position->y = boundsmin.y;
            }

            if (newy >= boundsmin.y) {
                position->y = boundsmax.y;
            }
        }
    }

    return (moved);
}


Point Position::getPos() {
    return (*position);
}

Point Position::getTilemapPos() {
    return (tilemap_pos);
}

Point Position::getPixelPos() {
    return (pixel_pos);
}
