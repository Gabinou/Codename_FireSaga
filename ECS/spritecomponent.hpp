#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

#include "ECS.hpp"
#include "map.hpp"
#include "shared.hpp"
#include "positioncomponent.hpp"
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"
#include "game.hpp"
#include "shared.hpp"
#include "SDL2/SDL.h"

class SpriteComponent : public Component {

    protected:
        SDL_Texture * texture;
        PositionComponent * positioncomponent;
        KeyboardController * keyboardcontroller;
        GamepadController * gamepadcontroller;
        SDL_Rect srcrect = {0, 0, 32, 32}; // x,y,w,h
        SDL_Rect destrect = {0, 0, 32, 32};
        Map * map = NULL; // no map-> position is not on a grid.
        int objectivepos[2];
        int slidepos[2];
        int * tilesize; // if no map, just use the pixel position as usual.
        int frames = 10, speed = 50;
        bool visible;
        bool animated = false;
        std::string ss_looping = "pingpong"; //ss: spritesheet
        float slidefactors[2] = {2, 1.025}; // for slide_type = "geometric"
        int slideint = 0; // for slide_type = "geometric"
        std::string slidetype = "";
    public:
        SpriteComponent() = default;

        SpriteComponent(const char * in_path) {
            visible = true;
            setTexture(in_path);
        }

        SpriteComponent(const char * in_path, int in_picsize[2]) : SpriteComponent(in_path)  {
            srcrect.w = in_picsize[0];
            srcrect.h = in_picsize[1];
            destrect.w = in_picsize[0];
            destrect.h = in_picsize[1];
        }

        SpriteComponent(Map * in_map, const char * in_path, int in_picsize[2]) : SpriteComponent(in_path)  {
            setMap(in_map);
            srcrect.w = in_picsize[0];
            srcrect.h = in_picsize[1];
            destrect.w = in_picsize[0];
            destrect.h = in_picsize[1];
        }

        SpriteComponent(Map * in_map, const char * in_path) : SpriteComponent(in_path) {
            setMap(in_map);
        }

        SpriteComponent(Map * in_map, const char * in_path, int inFrames, int inSpeed) : SpriteComponent(in_map, in_path) {
            animated = true;
            frames = inFrames;
            speed = inSpeed;
        }

        SDL_Texture * getTexture() {
            return (texture);
        }

        int * getTilesize() {
            return (tilesize);
        }

        void hide() {
            visible = false;
        }

        void show() {
            visible = true;
        }

        void initSlide() {
            if (slidetype == "geometric") {
                setSrcrect(64, 64); // Manually entered from cursor png size.
                setDestrect(tilesize[0] * 2, tilesize[1] * 2);
                slidepos[0] = objectivepos[0] = (int)positioncomponent->getPos()[0] * tilesize[0] - destrect.w / 4;
                slidepos[1] = objectivepos[1] = (int)positioncomponent->getPos()[1] * tilesize[1] - destrect.h / 4;
            }
        }

        void setSlidetype(std::string in_slidetype) {
            slidetype = in_slidetype;
            initSlide();
        }
        void setSlidetype(const char * in_slidetype) {
            slidetype = std::string(in_slidetype);
            initSlide();
        }

        void setTexture(const char * in_path) {
            texture = loadTexture(in_path);
        }

        void setMap(Map * in_map) {
            map = in_map;
            tilesize = map->getTilesize();
            srcrect.w = tilesize[0];
            srcrect.h = tilesize[1];
            destrect.w = tilesize[0];
            destrect.h = tilesize[1];
            map->getTile(0, 0); // Works here.
        }

        Map * getMap() {
            return (map);
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
                slidepos[0] = (int)positioncomponent->getPos()[0];
                slidepos[1] = (int)positioncomponent->getPos()[1];
            } else {
                slidepos[0] = (int)positioncomponent->getPos()[0] * tilesize[0];
                slidepos[1] = (int)positioncomponent->getPos()[1] * tilesize[1];
            }

            if (entity->hasComponent<KeyboardController>()) {
                keyboardcontroller = &entity->getComponent<KeyboardController>();
                keyboardcontroller->setTilesize(tilesize);
            }

            if (entity->hasComponent<GamepadController>()) {
                gamepadcontroller = &entity->getComponent<GamepadController>();
            }

            if (entity->hasComponent<SpriteComponent>()) {
                gamepadcontroller = &entity->getComponent<GamepadController>();
            }

            initSlide();
        }

        virtual void update() override {
            int kb_held = 0;
            int gp_held = 0;

            if (animated) { //looping sprites.
                if (ss_looping == "pingpong") {
                    srcrect.x = srcrect.w * pingpong(static_cast<int>(SDL_GetTicks() / speed), frames, 0);
                } else if ((ss_looping == "linear") || (ss_looping == "direct")) {
                    srcrect.x = srcrect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
                } else if (ss_looping == "reverse") {
                    srcrect.x = srcrect.w * (frames - static_cast<int>((SDL_GetTicks() / speed) % frames));
                }
            } else {
                if (map == NULL) {//move on the pixelspace
                    slidepos[0] = (int)positioncomponent->getPos()[0];
                    slidepos[1] = (int)positioncomponent->getPos()[1];
                } else {//move on the map.
                    slidepos[0] = (int)positioncomponent->getPos()[0] * tilesize[0];
                    slidepos[1] = (int)positioncomponent->getPos()[1] * tilesize[1];
                }

            }

            if (entity->hasComponent<KeyboardController>()) {
                kb_held = keyboardcontroller->getHeldmove();
            }

            if (entity->hasComponent<GamepadController>()) {
                gp_held = gamepadcontroller->getHeldmove();
            }

            if (slidetype == "geometric") { //Cursor movement on the map.

                objectivepos[0] = (int)positioncomponent->getPos()[0] * (tilesize[0]) - destrect.w / 4;
                objectivepos[1] = (int)positioncomponent->getPos()[1] * (tilesize[1]) - destrect.h / 4;

                if ((gp_held > 25) || (kb_held > 25))  {
                    slideint = 1;
                }

                if (objectivepos[0] != slidepos[0]) {
                    slidepos[0] += geometricslide((objectivepos[0] - slidepos[0]), slidefactors[slideint]);
                }

                if (objectivepos[1] != slidepos[1]) {
                    slidepos[1] += geometricslide((objectivepos[1] - slidepos[1]), slidefactors[slideint]);
                }

                if ((objectivepos[0] == slidepos[0]) && (objectivepos[1] == slidepos[1])) {
                    positioncomponent->setUpdatable(true);
                    slideint = 0;
                } else {
                    positioncomponent->setUpdatable(false);
                }
            }

            if (slidetype == "vector") { //unit movement on the map.

            }

            destrect.x = slidepos[0];
            destrect.y = slidepos[1];
        }

        virtual void draw() override {
            // printf("Is visible? %d\n", visible == true);
            if (visible) {
                SDL_RenderCopy(Game::renderer, texture, &srcrect, &destrect);
            }
        }
};

#endif /* SPRITECOMPONENT_HPP */