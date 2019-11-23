#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

#include "ECS.hpp"
#include "map.hpp"
#include "vector2D.hpp"
#include "shared.hpp"
#include "positioncomponent.hpp"
#include "game.hpp"
#include "shared.hpp"
#include "SDL2/SDL.h"


class SpriteComponent : public Component {

    protected:
        SDL_Texture * texture;
        PositionComponent * positioncomponent;
        SDL_Rect srcrect = {0, 0, 32, 32}; // x,y,w,h
        SDL_Rect destrect = {0, 0, 32, 32};
        Map * map = NULL; // no map-> position is not on a grid.
        Vector2D objectivepos;
        Vector2D slidepos;
        int * tilesize; // if no map, just use the pixel position as usual.
        bool animated = false;
        int frames = 10;
        int speed = 50;
        std::string ss_looping = "pingpong"; //ss: spritesheet

    public:
        SpriteComponent() = default;

        SpriteComponent(const char * in_path) {
            setTexture(in_path);
        }

        int * getTilesize() {
            return (tilesize);
        }

        SpriteComponent(const char * in_path, int in_picsize[2]) : SpriteComponent(in_path)  {
            srcrect.w = in_picsize[0];
            srcrect.h = in_picsize[1];
            destrect.w = in_picsize[0];
            destrect.h = in_picsize[1];
        }

        SpriteComponent(Map * inmap, const char * in_path) : SpriteComponent(in_path) {
            setMap(inmap);
        }

        SpriteComponent(Map * inmap, const char * in_path, int inFrames, int inSpeed) : SpriteComponent(inmap, in_path) {
            animated = true;
            frames = inFrames;
            speed = inSpeed;
        }

        SpriteComponent(Map * inmap, const char * in_path, int inFrames, int inSpeed, std::string in_looping) : SpriteComponent(inmap, in_path, inFrames, inSpeed) {
            ss_looping = in_looping;
        }

        SDL_Texture * getTexture() {
            return (texture);
        }

        void setTexture(const char * in_path) {
            texture = loadTexture(in_path);
            // texture = TextureManager::loadFromFile(in_path);
        }

        void setMap(Map * inmap) {
            map = inmap;
            tilesize = map->getTilesize();
            srcrect.w = tilesize[0];
            srcrect.h = tilesize[1];
            destrect.w = tilesize[0];
            destrect.h = tilesize[1];
        }

        void setSrcrect(int width, int height) {
            srcrect.w = width;
            srcrect.h = height;
        }

        void setDestrect(int width, int height) {
            destrect.w = width;
            destrect.h = height;
        }

        virtual void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();

            if (map == NULL) {
                slidepos.x = (int)positioncomponent->getPos().x;
                slidepos.y = (int)positioncomponent->getPos().y;
            } else {
                slidepos.x = (int)positioncomponent->getPos().x * tilesize[0];
                slidepos.y = (int)positioncomponent->getPos().y * tilesize[1];
            }

            destrect.x = slidepos.x;
            destrect.y = slidepos.y;
        }

        virtual void update() override {

            if (animated) {
                if (ss_looping == "pingpong") {
                    srcrect.x = srcrect.w * pingpong(static_cast<int>(SDL_GetTicks() / speed), frames, 0);
                } else if ((ss_looping == "linear") || (ss_looping == "direct")) {
                    srcrect.x = srcrect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
                } else if (ss_looping == "reverse") {
                    srcrect.x = srcrect.w * (frames - static_cast<int>((SDL_GetTicks() / speed) % frames));
                }
            }

            destrect.x = slidepos.x;
            destrect.y = slidepos.y;
        }

        virtual void draw() override {
            // TextureManager::draw(texture, srcrect, destrect);
            SDL_RenderCopy(Game::renderer, texture, &srcrect, &destrect);
        }
};

#endif /* SPRITECOMPONENT_HPP */