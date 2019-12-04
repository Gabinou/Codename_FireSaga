#ifndef SLIDECOMPONENT_HPP
#define SLIDECOMPONENT_HPP

#include "shared.hpp"
#include "spritecomponent.hpp"
#include "keyboardcontroller.hpp"
#include "gamepadcontroller.hpp"
#include "SDL2/SDL.h"

class SlideComponent : public SpriteComponent {

    private:
        KeyboardController * keyboardcontroller;
        GamepadController * gamepadcontroller;
        float slidefactors[2] = {2, 1.025}; // for slide_type = "geometric"
        int slideint = 0; // for slide_type = "geometric"
        std::string slidetype = "geometric";

        //

    public:

        void setSlideType(std::string in_slidetype) {
            slidetype = in_slidetype;
        }

        void init() override {
            SpriteComponent::init();

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

            // SpriteComponent::setSrcrect(64, 64); // Manually entered from cursor png size.
            // SpriteComponent::setDestrect(tilesize[0] * 2, tilesize[1] * 2);

            slidepos.x = objectivepos.x = (int)positioncomponent->getPos().x * tilesize[0] - destrect.w / 4;
            slidepos.y = objectivepos.y = (int)positioncomponent->getPos().y * tilesize[1] - destrect.h / 4;
        }

        void update() override {
            SpriteComponent::update();
            int kb_held = 0;
            int gp_held = 0;

            if (entity->hasComponent<KeyboardController>()) {
                kb_held = keyboardcontroller->getHeldframes();
            }

            if (entity->hasComponent<GamepadController>()) {
                gp_held = gamepadcontroller->getHeldframes();
            }

            objectivepos.x = (int)positioncomponent->getPos().x * (tilesize[0]) - destrect.w / 4;
            objectivepos.y = (int)positioncomponent->getPos().y * (tilesize[1]) - destrect.h / 4;
            // printf("before: %d %d\n", objectivepos.x, objectivepos.y);

            if (slidetype == "geometric") {
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

};

#endif /* SLIDECOMPONENT_HPP */