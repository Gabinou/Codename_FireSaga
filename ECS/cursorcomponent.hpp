#ifndef CURSORCOMPONENT_HPP
#define CURSORCOMPONENT_HPP

#include "shared.hpp"
#include "spritecomponent.hpp"
#include "keyboardcontroller.hpp"
#include "SDL2/SDL.h"

class CursorComponent : public SpriteComponent {

    private:
        KeyboardController * keyboardcontroller;
        float slidefactors[2] = {2, 1.025};
        int slide_int = 0;

    public:
        using SpriteComponent::SpriteComponent; //inherits all constructors


        void init() override {
            printf("init\n");
            SpriteComponent::init();
            keyboardcontroller = &entity->getComponent<KeyboardController>();
            SpriteComponent::setSrcrect(64, 64);
            SpriteComponent::setDestrect(tilesize[0] * 2, tilesize[1] * 2);

            // printf("%d %d\n", srcrect.w, srcrect.h);
            // printf("%d %d\n", (int)positioncomponent->getPos().x, (int)positioncomponent->getPos().y);
            slidepos.x = objectivepos.x = (int)positioncomponent->getPos().x * tilesize[0];
            slidepos.y = objectivepos.y = (int)positioncomponent->getPos().y * tilesize[1];

            // printf("%d %d\n", objectivepos.x, objectivepos.y);
            // printf("%d %d\n", slidepos.x, slidepos.y);
            objectivepos.x -= srcrect.w / 2;
            objectivepos.y -= srcrect.h / 2;

            // getchar();
        }

        void update() override {
            SpriteComponent::update();
            LastPressed lastpressed = keyboardcontroller->getLastPressed();
            // printf("%d %d\n", objectivepos.x, objectivepos.y);
            // printf("%d %d\n", slidepos.x, slidepos.y);


            if (lastpressed.pressed_frames > 25) {
                slide_int = 1;
            }

            if (objectivepos.x != slidepos.x) {
                slidepos.x += geometricslide((objectivepos.x - slidepos.x), slidefactors[slide_int]);
            }

            if (objectivepos.y != slidepos.y) {
                slidepos.y += geometricslide((objectivepos.y - slidepos.y), slidefactors[slide_int]);
            }

            if ((objectivepos.x == slidepos.x) && (objectivepos.y == slidepos.y)) {
                positioncomponent->setUpdatable(true);
                slide_int = 0;
            }

            // printf("%d %d\n", objectivepos.x, objectivepos.y);
            // printf("%d %d\n", slidepos.x, slidepos.y);
            destrect.x = slidepos.x;
            destrect.y = slidepos.y;

            // getchar();
        }

};

#endif /* CURSORCOMPONENT_HPP */