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
        Map * map = NULL; // no map-> position is not on a grid.
        entityx::ComponentHandle<Map> mapx;// no map-> position is not on a grid.
        SDL_Rect srcrect = {0, 0, 32, 32}; //x,y,w,h
        SDL_Rect destrect = {0, 0, 32, 32}; //x,y,w,h
        SDL_Texture * texture;

        short unsigned int * tilesize; // if no map, just use the pixel position as usual.
        short int frames = 10, speed = 50;
        short int slideint = 0; // for slide_type = "geometric"

        bool visible;
        bool animated = false;

        float slidefactors[2] = {2, 1.025}; // for slide_type = "geometric"

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

        SpriteComponent(Map * in_map, const char * in_asset_name) : SpriteComponent(in_asset_name) {
            setMap(in_map);
        }

        SpriteComponent(entityx::ComponentHandle<Map> in_mapx, const char * in_asset_name) : SpriteComponent(in_asset_name) {
            setMap(in_mapx);
        }

        SpriteComponent(Map * in_map, const char * in_asset_name, int in_picsize[2]) : SpriteComponent(in_asset_name, in_picsize)  {
            setMap(in_map);
        }

        SpriteComponent(Map * in_map, const char * in_asset_name, int inFrames, int inSpeed) : SpriteComponent(in_map, in_asset_name) {
            animated = true;
            frames = inFrames;
            speed = inSpeed;
        }

        SpriteComponent(entityx::ComponentHandle<Map> in_mapx, const char * in_asset_name, int inFrames, int inSpeed) : SpriteComponent(in_mapx, in_asset_name) {
            animated = true;
            frames = inFrames;
            speed = inSpeed;
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

        short unsigned int * getTilesize() {
            return (tilesize);
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

        void initSlide() {
            if (slidetype == "geometric") {
                setSrcrect(64, 64); // Manually entered from cursor png size.
                setDestrect(tilesize[0] * 2, tilesize[1] * 2);
            }
        }

        std::string getSlidetype() {
            return (slidetype);
        }

        void setSlidetype(std::string in_slidetype) {
            slidetype = in_slidetype;
            initSlide();
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

        void setMap(entityx::ComponentHandle<Map> in_mapx) {
            mapx = in_mapx;
            tilesize = mapx->getTilesize();
            srcrect.w = tilesize[0];
            srcrect.h = tilesize[1];
            destrect.w = tilesize[0];
            destrect.h = tilesize[1];
        }

        void setMap(Map * in_map) {
            map = in_map;
            tilesize = map->getTilesize();
            srcrect.w = tilesize[0];
            srcrect.h = tilesize[1];
            destrect.w = tilesize[0];
            destrect.h = tilesize[1];
            // map->getTile(0, 0);
        }

        Map * getMap() {
            return (map);
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

        void init() {
            initSlide();
        }

        void draw() {
            if (visible) {
                SDL_RenderCopy(Game::renderer, texture, &srcrect, &destrect);
            }
        }
};

#endif /* SPRITECOMPONENT_HPP */