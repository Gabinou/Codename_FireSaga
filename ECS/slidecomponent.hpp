#ifndef SLIDECOMPONENT_HPP
#define SLIDECOMPONENT_HPP

#include "shared.hpp"
#include "spritecomponent.hpp"
#include "keyboardcontroller.hpp"
#include "SDL2/SDL.h"

class SlideComponent : public SpriteComponent {

    private:
        KeyboardController * keyboardcontroller;
        float slidefactors[2] = {2, 1.025}; // for slide_type = "geometric"
        int slideint = 0; // for slide_type = "geometric"
        int slidespeed = 10;

        std::string slidetype = "geometric";

    public:
        using SpriteComponent::SpriteComponent; //inherits all constructors
        using SpriteComponent::SpriteComponent; //inherits all constructors

        void setSlideType(std::string in_slidetype) {
            slidetype = in_slidetype;
        }

        void init() override {
            printf("init\n");
            SpriteComponent::init();
            keyboardcontroller = &entity->getComponent<KeyboardController>();
            keyboardcontroller->setTilesize(tilesize);
            SpriteComponent::setSrcrect(64, 64); // Manually entered from cursor png size.
            SpriteComponent::setDestrect(tilesize[0] * 2, tilesize[1] * 2);

            slidepos.x = objectivepos.x = (int)positioncomponent->getPos().x * tilesize[0] - destrect.w / 4;
            slidepos.y = objectivepos.y = (int)positioncomponent->getPos().y * tilesize[1] - destrect.h / 4;
        }

        void update() override {
            SpriteComponent::update();
            LastPressed lastpressed = keyboardcontroller->getLastPressed();
            objectivepos.x = (int)positioncomponent->getPos().x * (tilesize[0]) - destrect.w / 4;
            objectivepos.y = (int)positioncomponent->getPos().y * (tilesize[1]) - destrect.h / 4;
            // printf("before: %d %d\n", slidepos.x, slidepos.y);

            if (slidetype == "geometric") {
                if (lastpressed.pressed_frames > 25) {
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
                }
            }

            // printf("after: %d %d\n", objectivepos.x, objectivepos.y);
            // printf("after: %d %d\n", slidepos.x, slidepos.y);
            destrect.x = slidepos.x;
            destrect.y = slidepos.y;

            // getchar();
        }

};

#endif /* SLIDECOMPONENT_HPP */