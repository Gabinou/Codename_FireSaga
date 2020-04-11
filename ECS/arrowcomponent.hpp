#ifndef ARROWCOMPONENT_HPP
#define ARROWCOMPONENT_HPP

#include "utilities.hpp"
#include "positioncomponent.hpp"

// I want this to not be able to be used for RNG abuse, because fuck RNG abuse.

class ArrowComponent {
    protected:
        SDL_Texture * texture;
        std::vector<SDL_Rect> srcrects = {{0, 0, 32, 32}, {32, 0, 32, 32}, {64, 0, 32, 32}}; // x,y,w,h
        std::vector<SDL_Rect> destrects = {{0, 0, 32, 32}};
        std::vector<std::vector<short int>> path;// this is a 2D path with x and y.
        std::vector<short int> pathx;
        // conaints only 1 or 0. 1 means move in x direction. 0 means move in y direction.
        // movement direction is based on computed distance between origin and cursor.
        std::vector <std::vector<short int>> open_tiles;
        // Entity * cursor;
        PositionComponent * positioncomponent;
        bool visible;
        bool updatable;
        short int move = 6; //temporary, waiting for units to be able to give their move units.
        short unsigned int * tilesize; // if no map, just use the pixel position as usual.
        short int * origin;
        Map * map = NULL; // no map-> position is not on a grid.

    public:
        ArrowComponent() = default;

        ArrowComponent(const char * in_path) {
            texture = loadTexture(Game::renderer, in_path);
        }

        // ArrowComponent(const char * in_path, Entity * in_cursor) : ArrowComponent(in_path) {
        //     visible = true;
        //     cursor = in_cursor;
        // }

        ArrowComponent(Map * in_map, const char * in_path) : ArrowComponent(in_path) {
            setMap(in_map);
        }

        void setMap(Map * in_map) {
            map = in_map;
            tilesize = map->getTilesize();
            destrects[0].w = tilesize[0];
            destrects[0].h = tilesize[1];
            // map->getTile(0, 0);
        }

        bool traversable(int * tile_pos) {
            return (true);
        }


        virtual void init() {
            // positioncomponent = &entity->getComponent<PositionComponent>();
            origin = positioncomponent->getPos();
        }

        virtual void update() {


            // short int * objectivepos = cursor->getComponent<PositionComponent>().getPos();


            // if (!cursor->getComponent<PositionComponent>().isUpdatable()) {
            //     if (path.size() < move) {
            //         path.push_back(std::vector<short int>());
            //         path[path.size()].push_back(objectivepos[0]);
            //         path[path.size()].push_back(objectivepos[1]);
            //     } else {

            //     }
            // }

        }

        virtual void draw() {
            // printf("Is visible? %d\n", visible == true);
            if (visible) {
                SDL_RenderCopy(Game::renderer, texture, &srcrects[0], &destrects[0]);
            }
        }
};

#endif /* ARROWCOMPONENT_HPP */