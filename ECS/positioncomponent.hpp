#ifndef POSITIONCOMPONENT_HPP
#define POSITIONCOMPONENT_HPP

#include "ECS.hpp"
#include "vector2D.hpp"
#include "game.hpp"
#include "map.hpp"
#include "shared.hpp"
#include "SDL2/SDL.h"

class PositionComponent : public Component {
    private:
        bool updatable = true;
        Vector2D position;
        int bounds[4] = {0, 255, 0, 255}; //xmin, xmax, ymin, ymax
        Map * map = nullptr;
        // It seems that on first init of the component, entity is still a nullptr. Tryeing something.
    public:
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
            map->setTile(position.x, position.y, entity);
        }

        PositionComponent() {
            setPos(0, 0);
        }

        void setUpdatable(bool in_updatable) {
            updatable = in_updatable;
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

        void setPos(int in_x, int in_y) {
            if (updatable) {

                if ((in_x >= bounds[0]) && (in_x <= bounds[1])) {
                    position.x = in_x;

                }

                if ((in_y >= bounds[2]) && (in_y <= bounds[3])) {
                    position.y = in_y;
                }

                if (map != nullptr) {
                    printf("set new tile");
                    map->setTile(position.x, position.y, entity);
                }
            }


        }

        void addPos(Vector2D move) {
            setPos((move.x + position.x), (move.y + position.y));
        }

        Vector2D getPos() {
            return (position);
        }

};


#endif /* POSITIONCOMPONENT_HPP */