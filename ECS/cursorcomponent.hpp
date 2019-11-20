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
        unsigned int bounds[4] = {0, 255, 0, 255}; //xmin,xmax,ymin,ymax

    public:
        using SpriteComponent::SpriteComponent; //inherits all constructors

        void setBounds(unsigned int xmin, unsigned int xmax, unsigned int ymin, unsigned int ymax) {
            bounds[0] = xmin;
            bounds[1] = xmax;
            bounds[2] = ymin;
            bounds[3] = ymax;
        }

        void setBounds(unsigned int in_bounds[4]) {
            bounds[0] = in_bounds[0];
            bounds[1] = in_bounds[1];
            bounds[2] = in_bounds[2];
            bounds[3] = in_bounds[3];
        }

        void setBounds(std::vector<unsigned int> in_bounds) {
            bounds[0] = in_bounds[0];
            bounds[1] = in_bounds[1];
            bounds[2] = in_bounds[2];
            bounds[3] = in_bounds[3];
        }

        void init() override {
            printf("init\n");
            SpriteComponent::init();
            keyboardcontroller = &entity->getComponent<KeyboardController>();
            keyboardcontroller->setTilesize(tilesize);
            SpriteComponent::setSrcrect(64, 64); //Manually entered from cursor png size.
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

            // printf("after: %d %d\n", objectivepos.x, objectivepos.y);
            // printf("after: %d %d\n", slidepos.x, slidepos.y);
            destrect.x = slidepos.x;
            destrect.y = slidepos.y;

            // getchar();
        }

};

#endif /* CURSORCOMPONENT_HPP */