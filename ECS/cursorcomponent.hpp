#ifndef CURSORCOMPONENT_HPP
#define CURSORCOMPONENT_HPP

#include "shared.hpp"
#include "spritecomponent.hpp"
#include "keyboardcontroller.hpp"
#include "SDL2/SDL.h"

class CursorComponent : public SpriteComponent{
    
    private:
        KeyboardController *keyboardcontroller;
        float slidefactors[2] = {2, 1.025};
        int slide_int = 0;
        
    public:
        using SpriteComponent::SpriteComponent;
        
        void init() override {
            SpriteComponent::init();
            keyboardcontroller = &entity->getComponent<KeyboardController>();
            SpriteComponent::setSrcrect(64, 64);
            destrect.w = srcrect.w;
            destrect.h = srcrect.h;
        }
        
        void update() override {
            SpriteComponent::update();
            LastPressed lastpressed = keyboardcontroller->getLastPressed();
            objectivepos.x -= srcrect.w/4;
            objectivepos.y -= srcrect.h/4;
            if (lastpressed.pressed_frames>25){
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
            destrect.x = slidepos.x;
            destrect.y = slidepos.y;              

        }
        
};

#endif /* CURSORCOMPONENT_HPP */