#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

#include "ECS.hpp"
#include "map.hpp"
#include "vector2D.hpp"
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
        Vector2D objectivepos;
        Vector2D slidepos;
        int * tilesize; // if no map, just use the pixel position as usual.
        int frames = 10;
        int speed = 50;
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
                slidepos.x = objectivepos.x = (int)positioncomponent->getPos().x * tilesize[0] - destrect.w / 4;
                slidepos.y = objectivepos.y = (int)positioncomponent->getPos().y * tilesize[1] - destrect.h / 4;
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
                slidepos.x = (int)positioncomponent->getPos().x;
                slidepos.y = (int)positioncomponent->getPos().y;
            } else {
                slidepos.x = (int)positioncomponent->getPos().x * tilesize[0];
                slidepos.y = (int)positioncomponent->getPos().y * tilesize[1];
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

            if (animated) {
                if (ss_looping == "pingpong") {
                    srcrect.x = srcrect.w * pingpong(static_cast<int>(SDL_GetTicks() / speed), frames, 0);
                } else if ((ss_looping == "linear") || (ss_looping == "direct")) {
                    srcrect.x = srcrect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
                } else if (ss_looping == "reverse") {
                    srcrect.x = srcrect.w * (frames - static_cast<int>((SDL_GetTicks() / speed) % frames));
                }
            } else {
                if (map == NULL) {
                    slidepos.x = (int)positioncomponent->getPos().x;
                    slidepos.y = (int)positioncomponent->getPos().y;
                } else {
                    slidepos.x = (int)positioncomponent->getPos().x * tilesize[0];
                    slidepos.y = (int)positioncomponent->getPos().y * tilesize[1];
                }

            }

            if (entity->hasComponent<KeyboardController>()) {
                kb_held = keyboardcontroller->getHeldframes();
            }

            if (entity->hasComponent<GamepadController>()) {
                gp_held = gamepadcontroller->getHeldframes();
            }


            // printf("before: %d %d\n", objectivepos.x, objectivepos.y);

            if (slidetype == "geometric") {

                objectivepos.x = (int)positioncomponent->getPos().x * (tilesize[0]) - destrect.w / 4;
                objectivepos.y = (int)positioncomponent->getPos().y * (tilesize[1]) - destrect.h / 4;

                // intended to animate the cursor.
                if ((gp_held > 25) || (kb_held > 25))  {
                    slideint = 1;
                }

                if (objectivepos.x != slidepos.x) {
                    slidepos.x += geometricslide((objectivepos.x - slidepos.x), slidefactors[slideint]);
                }

                if (objectivepos.y != slidepos.y) {
                    slidepos.y += geometricslide((objectivepos.y - slidepos.y), slidefactors[slideint]);
                }

                if ((objectivepos.x == slidepos.x) && (objectivepos.y == slidepos.y)) {
                    positioncomponent->setUpdatable(true);
                    slideint = 0;
                } else {
                    positioncomponent->setUpdatable(false);
                }
            }

            if (slidetype == "vector") {
                //intended to animate units moving on the map.
            }

            destrect.x = slidepos.x;
            destrect.y = slidepos.y;




        }

        virtual void draw() override {
            // printf("Is visible? %d\n", visible == true);
            if (visible) {
                SDL_RenderCopy(Game::renderer, texture, &srcrect, &destrect);
            }
        }
};

#endif /* SPRITECOMPONENT_HPP */