
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
    bounds[0] = xmin;
    bounds[1] = xmax;
    bounds[2] = ymin;
    bounds[3] = ymax;

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

void Position::setUpdatable(bool in_updatable) {
    updatable = in_updatable;
}

bool Position::isUpdatable() {
    return (updatable);
}

void Position::setPos(short int * in_pos) {
    setPos(in_pos[0], in_pos[1]);
}

void Position::setPos(short int in_x, short int in_y) {
    if (updatable) {
        if ((in_x >= bounds[0]) && (in_x <= bounds[1])) {
            position[0] = in_x;
        }

        if ((in_y >= bounds[2]) && (in_y <= bounds[3])) {
            position[1] = in_y;
        }
    }
}

void Position::addPos(short int move_x, short int move_y) {
    setPos((move_x + position[0]), (move_y + position[1]));
}

short int * Position::getPos() {
    return (position);
}
