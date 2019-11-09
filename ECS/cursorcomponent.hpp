#ifndef CURSORCOMPONENT_HPP
#define CURSORCOMPONENT_HPP

#include "shared.hpp"
#include "spritecomponent.hpp"
#include "keyboardcontroller.hpp"
#include "SDL2/SDL.h"

class CursorComponent : public SpriteComponent{
    
    private:
        KeyboardController *keyboardcontroller;

    public:
        using SpriteComponent::SpriteComponent;
        
        
        void init() {
            SpriteComponent::init();
            keyboardcontroller = &entity->getComponent<KeyboardController>();
        }
        
        void update() override {
            SpriteComponent::update();
            LastPressed lastpressed = keyboardcontroller->getLastPressed();
            float slidefactor = 2;
            if (lastpressed.pressed_frames>30){
                // printf("Up active frames: %d\n", lastpressed.pressed_frames);
                printf("Going fast\n");
                slidefactor = 1.05;
            }
            if (objectivepos.x != slidepos.x) {
                // printf("geo_factor: %d", geo_factor);
                slidepos.x += geometricslide((objectivepos.x - slidepos.x), slidefactor);
            }
            if (objectivepos.y != slidepos.y) {
                slidepos.y += geometricslide((objectivepos.y - slidepos.y), slidefactor);
            }
            if ((objectivepos.x == slidepos.x) && (objectivepos.y == slidepos.y)) {
                positioncomponent->setUpdatable(true);
            }
            destrect.x = slidepos.x;
            destrect.y = slidepos.y;
        }
        
};

#endif /* CURSORCOMPONENT_HPP */