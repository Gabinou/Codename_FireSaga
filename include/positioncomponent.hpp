#ifndef POSITIONCOMPONENT_HPP
#define POSITIONCOMPONENT_HPP

#include "game.hpp"
#include "map.hpp"
#include "utilities.hpp"
#include "SDL2/SDL.h"

class PositionComponent {
    private:
        bool updatable = true;
        short int position[2] = {0};
        short int bounds[4] = {0, 255, 0, 255}; //xmin, xmax, ymin, ymax

    public:
        PositionComponent() {
            setPos(0, 0);
        }

        PositionComponent(short int in_x, short int in_y) {
            setPos(in_x, in_y);
        }

        PositionComponent(short int * position) {
            setPos(position[0], position[1]);
        }

        PositionComponent(short int in_x, short int in_y,
                          short int xmin, short int xmax,
                          short int ymin, short int ymax)
            : PositionComponent(in_x, in_y) {
            setBounds(xmin, xmax, ymin, ymax);
        }

        PositionComponent(short int in_x, short int in_y,
                          short int in_bounds[4])
            : PositionComponent(in_x, in_y) {
            setBounds(in_bounds);
        }

        PositionComponent(short int in_x, short int in_y,
                          std::vector<short int> in_bounds)
            : PositionComponent(in_x, in_y) {
            setBounds(in_bounds);
        }

        void setBounds(short int xmin, short int xmax, short int ymin, short int ymax) {
            bounds[0] = xmin;
            bounds[1] = xmax;
            bounds[2] = ymin;
            bounds[3] = ymax;
        }

        void setBounds(short int in_bounds[4]) {
            bounds[0] = in_bounds[0];
            bounds[1] = in_bounds[1];
            bounds[2] = in_bounds[2];
            bounds[3] = in_bounds[3];
        }

        void setBounds(std::vector<short int> in_bounds) {
            bounds[0] = in_bounds[0];
            bounds[1] = in_bounds[1];
            bounds[2] = in_bounds[2];
            bounds[3] = in_bounds[3];
        }

        short int * getBounds() {
            return (bounds);
        }

        void setUpdatable(bool in_updatable) {
            updatable = in_updatable;
        }
        bool isUpdatable() {
            return (updatable);
        }

        void setPos(short int * in_pos) {
            setPos(in_pos[0], in_pos[1]);
        }

        void setPos(short int in_x, short int in_y) {
            if (updatable) {
                if ((in_x >= bounds[0]) && (in_x <= bounds[1])) {
                    position[0] = in_x;
                }

                if ((in_y >= bounds[2]) && (in_y <= bounds[3])) {
                    position[1] = in_y;
                }
            }
        }

        void addPos(short int move_x, short int move_y) {
            setPos((move_x + position[0]), (move_y + position[1]));
        }

        short int * getPos() {
            return (position);
        }
};

#endif /* POSITIONCOMPONENT_HPP */