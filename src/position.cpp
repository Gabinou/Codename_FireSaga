
#include "position.hpp"

Position::Position() {
    setPos(0, 0);
}

Position::Position(short int in_x, short int in_y) {
    setPos(in_x, in_y);
}

Position::Position(short int * position) {
    setPos(position[0], position[1]);
}

Position::Position(short int in_x, short int in_y,
                   short int xmin, short int xmax,
                   short int ymin, short int ymax)
    : Position::Position(in_x, in_y) {
    setBounds(xmin, xmax, ymin, ymax);
}

Position::Position(short int in_x, short int in_y,
                   short int in_bounds[4])
    : Position::Position(in_x, in_y) {
    setBounds(in_bounds);
}

Position::Position(short int in_x, short int in_y,
                   std::vector<short int> in_bounds)
    : Position::Position(in_x, in_y) {
    setBounds(in_bounds);
}

void Position::setBounds(short int xmin, short int xmax, short int ymin, short int ymax) {
    bounds[0] = xmin + offset[0];
    bounds[1] = xmax + offset[0];
    bounds[2] = ymin + offset[1];
    bounds[3] = ymax + offset[1];

    if (position[0] < bounds[0]) {
        position[0] = bounds[0];
    }

    if (position[0] > bounds[1]) {
        position[0] = bounds[1];
    }

    if (position[1] < bounds[2]) {
        position[1] = bounds[2];
    }

    if (position[1] > bounds[3]) {
        position[1] = bounds[3];
    }

}

void Position::setOffset(short int xoffset, short int yoffset) {
    offset[0] = xoffset;
    offset[1] = yoffset;
}

void Position::setOffset(short int in_offset[2]) {
    offset[0] = in_offset[0];
    offset[1] = in_offset[1];
}

void Position::setBounds(short int in_bounds[4]) {
    bounds[0] = in_bounds[0];
    bounds[1] = in_bounds[1];
    bounds[2] = in_bounds[2];
    bounds[3] = in_bounds[3];
}

void Position::setBounds(std::vector<short int> in_bounds) {
    bounds[0] = in_bounds[0];
    bounds[1] = in_bounds[1];
    bounds[2] = in_bounds[2];
    bounds[3] = in_bounds[3];
}

short int * Position::getBounds() {
    return (bounds);
}

short int * Position::getOffset() {
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
}

void Position::setUpdatable(bool in_updatable) {
    updatable = in_updatable;
}

bool Position::isUpdatable() {
    return (updatable);
}

bool Position::setPos(short int * in_pos) {
    return (setPos(in_pos[0], in_pos[1]));
}

bool Position::setPos(short int in_x, short int in_y) {
    short int newx = in_x + offset[0];
    short int newy = in_y + offset[1];
    return (newPos(newx, newy));
}

bool Position::addPos(short int move_x, short int move_y) {
    short int newx = move_x + position[0];
    short int newy = move_y + position[1];
    return (newPos(newx, newy));
}

bool Position::newPos(short int newx, short int newy) {
    bool moved = false;

    if (updatable) {

        if (periodic) {

            while (newx <= bounds[0]) {
                newx += bounds[1] - bounds[0] + 1;
            }

            while (newy <= bounds[2]) {
                newy += bounds[3] - bounds[2] + 1;
            }

            newx = (newx - 1) % (bounds[1] - bounds[0] + 1) + bounds[0];
            newy = (newy - 1) % (bounds[3] - bounds[2] + 1) + bounds[2];
            // Python % is modulo. C/C++ % is remainder.
        }

        if ((newx > bounds[0]) && (newx < bounds[1]) && (position[0] != newx)) {
            position[0] = newx;
            moved = true;
        } else {
            if (newx <= bounds[0]) {
                position[0] = bounds[0];
            }

            if (newx >= bounds[1]) {
                position[0] = bounds[1];
            }
        }

        if ((newy > bounds[2]) && (newy < bounds[3]) && (position[1] != newy)) {
            position[1] = newy;
            moved = true;
        } else {
            if (newy <= bounds[2]) {
                position[1] = bounds[2];
            }

            if (newy >= bounds[3]) {
                position[1] = bounds[3];
            }
        }
    }

    return (moved);
}


short int * Position::getPos() {
    return (position);
}
