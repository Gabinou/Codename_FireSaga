#ifndef ARROWCOMPONENT_HPP
#define ARROWCOMPONENT_HPP

#include "ECS.hpp"
#include "shared.hpp"
#include "positioncomponent.hpp"


class ArrowComponent : public Component {
// I want this to not be able to be used for RNG abuse, because fuck RNG abuse.
    protected:
        SDL_Texture * texture;
        std::vector<SDL_Rect> srcrects = {{0, 0, 32, 32}, {32, 0, 32, 32}, {64, 0, 32, 32}}; // x,y,w,h
        std::vector<SDL_Rect> destrects = {{0, 0, 32, 32}};
        std::vector < std::vector << int>> path;// this is a 2D path with x and y.
        std::vector <int> pathx;
        // conaints only 1 or 0. 1 means move in x direction. 0 means move in y direction.
        // movement direction is based on computed distance between origin and cursor. 
        std::vector < std::vector << int>> open_tiles;
        bool visible;
        int move = 6; //temporary, waiting for units to be able to give their move units.
        Map * map = NULL; // no map-> position is not on a grid.
        int * tilesize; // if no map, just use the pixel position as usual.
        int * origin;
    public:
        ArrowComponent() = default;

        ArrowComponent(const char * in_path, Entity * in_cursor) {
            visible = true;
            texture = loadTexture(in_path);
            cursor = in_cursor;
        }

        ArrowComponent(Map * in_map, const char * in_path) : ArrowComponent(in_path) {
            setMap(in_map);
        }

        void setMap(Map * in_map) {
            map = in_map;
            tilesize = map->getTilesize();
            destrect.w = tilesize[0];
            destrect.h = tilesize[1];
            map->getTile(0, 0); // Works here.
        }

        bool traversable(int* tile_pos){
            return(true);
        }


        virtual void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
            origin = setting_entity.getComponent<PositionComponent>().getPos();
        }

        void make_path(int * cursor_pos){
            path.clear();
            int delta_x = origin[0] - cursor_pos[0];
            int sign_x = sgn(delta_x);
            int delta_y = origin[1] - cursor_pos[1];
            int sign_y = sgn(delta_y);
            int distance = abs(delta_x) + abs(delta_y);


        }

        virtual void update() override {
            int * objectivepos = setting_entity.getComponent<PositionComponent>().getPos();


            if (!setting_entity.getComponent<PositionComponent>().isUpdatable()) {
                if (path.size() < move) {
                    path.push_back(std::vector<int>());
                    path[path.size()].push_back(objectivepos[0]);
                    path[path.size()].push_back(objectivepos[1]);
                } else {
                    make_path(setting_entity.getComponent<PositionComponent>().getPos());
                }
            }

            manager.getEntities()[0]->getComponent<PositionComponent>()

        }

        virtual void draw() override {
            // printf("Is visible? %d\n", visible == true);
            if (visible) {
                SDL_RenderCopy(Game::renderer, texture, &srcrect, &destrect);
            }
        }
};

#endif /* ARROWCOMPONENT_HPP */