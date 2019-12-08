#ifndef ARROWCOMPONENT_HPP
#define ARROWCOMPONENT_HPP

#include "ECS.hpp"
#include "shared.hpp"
#include "positioncomponent.hpp"


class ArrowComponent : public Component {

    protected:
        SDL_Texture * texture;
        std::vector<SDL_Rect> srcrects = {{0, 0, 32, 32}, {32, 0, 32, 32}, {64, 0, 32, 32}}; // x,y,w,h
        std::vector<SDL_Rect> destrects = {{0, 0, 32, 32}};
        std::vector < std::vector << int>> path;
        bool visible;
        Map * map = NULL; // no map-> position is not on a grid.
        int * tilesize; // if no map, just use the pixel position as usual.
        Entity * cursor;
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

        virtual void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();

        }

        virtual void update() override {
            int * objectivepos = setting_entity.getComponent<PositionComponent>().getPos();
        }

        virtual void draw() override {
            // printf("Is visible? %d\n", visible == true);
            if (visible) {
                SDL_RenderCopy(Game::renderer, texture, &srcrect, &destrect);
            }
        }
};

#endif /* ARROWCOMPONENT_HPP */