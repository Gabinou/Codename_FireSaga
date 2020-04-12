#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

#include "map.hpp"
#include "filesystem.hpp"
#include "positioncomponent.hpp"
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"
#include "game.hpp"
#include "utilities.hpp"
#include "SDL2/SDL.h"

class SpriteComponent {

    protected:
        SDL_Rect srcrect = {0, 0, 32, 32}; //x,y,w,h
        SDL_Rect destrect = {0, 0, 32, 32}; //x,y,w,h
        SDL_Texture * texture;

        short int frames = 10, speed = 50;
        short int slideint = 0; // for slide_type = "geometric"
        float slidefactors[2] = {2, 1.025}; // for slide_type = "geometric"

        bool visible;
        bool animated = false;

        std::string asset_name;
        std::string ss_looping = "pingpong"; //ss: spritesheet
        std::string slidetype = "";
    public:
        SpriteComponent() = default;

        SpriteComponent(const char * in_asset_name) {
            visible = true;
            asset_name = in_asset_name;
            setTexture(in_asset_name);
        }

        SpriteComponent(const char * in_asset_name, int in_picsize[2]) : SpriteComponent(in_asset_name)  {
            srcrect.w = in_picsize[0];
            srcrect.h = in_picsize[1];
            destrect.w = in_picsize[0];
            destrect.h = in_picsize[1];
        }

        SpriteComponent(const char * in_asset_name, int inFrames, int inSpeed) : SpriteComponent(in_asset_name) {
            animated = true;
            frames = inFrames;
            speed = inSpeed;
        }

        SDL_Texture * getTexture() {
            return (texture);
        }

        float * getSlidefactors() {
            return (slidefactors);
        }

        void hide() {
            visible = false;
        }

        bool isAnimated() {
            return (animated);
        }

        bool isVisible() {
            return (visible);
        }

        std::string getSs_looping() {
            return (ss_looping);
        }

        short int getFrames() {
            return (frames);
        }

        short int getSpeed() {
            return (speed);
        }

        void show() {
            visible = true;
        }

        std::string getSlidetype() {
            return (slidetype);
        }

        void setSlidetype(std::string in_slidetype, short unsigned int * tilesize) {
            slidetype = in_slidetype;

            if (slidetype == "geometric") {
                setSrcrect(tilesize[0] * 2, tilesize[1] * 2); // Manually entered from cursor png size.
                setDestrect(tilesize[0] * 2, tilesize[1] * 2);
            }
        }

        void setTexture(const char * in_asset_name) {
            texture = loadTexture(Game::renderer, in_asset_name);
        }

        void setAnimation(int inFrames, int inSpeed) {
            frames = inFrames;
            speed = inSpeed;
        }

        short int getSlideint() {
            return (slideint);
        }

        void setSrcrect(SDL_Rect in_rect) {
            srcrect = in_rect;
        }

        void setSrcrect(int width, int height) {
            srcrect.w = width;
            srcrect.h = height;
        }

        void setDestrect(SDL_Rect in_rect) {
            destrect = in_rect;
        }

        void setDestrect(int width, int height) {
            destrect.w = width;
            destrect.h = height;
        }

        SDL_Rect getSrcrect() {
            return (srcrect);
        }

        SDL_Rect getDestrect() {
            return (destrect);
        }

        void draw() {
            if (visible) {
                SDL_RenderCopy(Game::renderer, texture, &srcrect, &destrect);
            }
        }
};

#endif /* SPRITECOMPONENT_HPP */