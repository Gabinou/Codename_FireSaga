#ifndef POSITIONCOMPONENT_HPP
#define POSITIONCOMPONENT_HPP

#include "ECS.hpp"
#include "game.hpp"
#include "map.hpp"
#include "shared.hpp"
#include "SDL2/SDL.h"

class PositionComponent : public Component {
        // On first init of the component, entity is still a nullptr.
    private:
        bool updatable = true;
        int position[2] = {0};
        int bounds[4] = {0, 255, 0, 255}; //xmin, xmax, ymin, ymax
        Map * map = nullptr;

    public:
        PositionComponent() {
            setPos(0, 0);
        }

        PositionComponent(int in_x, int in_y) {
            setPos(in_x, in_y);
        }

        PositionComponent(int in_x, int in_y,
                          int xmin, int xmax,
                          int ymin, int ymax)
            : PositionComponent(in_x, in_y) {
            setBounds(xmin, xmax, ymin, ymax);
        }

        PositionComponent(int in_x, int in_y,
                          int in_bounds[4])
            : PositionComponent(in_x, in_y) {
            setBounds(in_bounds);
        }

        PositionComponent(int in_x, int in_y,
                          std::vector<int> in_bounds)
            : PositionComponent(in_x, in_y) {
            setBounds(in_bounds);
        }

        void setBounds(int xmin, int xmax, int ymin, int ymax) {
            bounds[0] = xmin;
            bounds[1] = xmax;
            bounds[2] = ymin;
            bounds[3] = ymax;
        }

        void setBounds(int in_bounds[4]) {
            bounds[0] = in_bounds[0];
            bounds[1] = in_bounds[1];
            bounds[2] = in_bounds[2];
            bounds[3] = in_bounds[3];
        }

        void setBounds(std::vector<int> in_bounds) {
            bounds[0] = in_bounds[0];
            bounds[1] = in_bounds[1];
            bounds[2] = in_bounds[2];
            bounds[3] = in_bounds[3];
        }

        int * getBounds() {
            return (bounds);
        }

        Map * getMap() {
            return (map);
        }

        void setMap(Map * in_map) {
            map = in_map;
            map->setTile(position[0], position[1], entity);
        }

        void setUpdatable(bool in_updatable) {
            updatable = in_updatable;
        }
        bool isUpdatable() {
            return (updatable);
        }

        void setPos(int in_x, int in_y) {
            if (updatable) {

                if ((in_x >= bounds[0]) && (in_x <= bounds[1])) {
                    position[0] = in_x;
                }

                if ((in_y >= bounds[2]) && (in_y <= bounds[3])) {
                    position[1] = in_y;
                }

                if (map != nullptr) {
                    map->setTile(position[0], position[1], entity);
                }
            }
        }

        void addPos(int move_x, int move_y) {
            setPos((move_x + position[0]), (move_y + position[1]));
        }

        int * getPos() {
            return (position);
        }
};

#endif /* POSITIONCOMPONENT_HPP */