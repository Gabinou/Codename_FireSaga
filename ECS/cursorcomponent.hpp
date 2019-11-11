#ifndef CURSORCOMPONENT_HPP
#define CURSORCOMPONENT_HPP

#include "shared.hpp"
#include "spritecomponent.hpp"
#include "keyboardcontroller.hpp"
#include "SDL2/SDL.h"

class CursorComponent : public SpriteComponent{
    
    private:
        KeyboardController *keyboardcontroller;
        SDL_Rect spriteclips[9];
        
    public:
        using SpriteComponent::SpriteComponent;
        
        // CursorComponent
        
        void init() override {
            SpriteComponent::init();
            keyboardcontroller = &entity->getComponent<KeyboardController>();
            SpriteComponent::setSrcrect(64, 64);
        }
        
        void update() override {
            SpriteComponent::update();
            LastPressed lastpressed = keyboardcontroller->getLastPressed();
            float slidefactor = 2;

            if (lastpressed.pressed_frames>25){
                slidefactor = 1.025;
            }
            if (objectivepos.x != slidepos.x) {
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
            destrect.x -= srcrect.w/4;
            destrect.y -= srcrect.h/4;           
            destrect.w = srcrect.w;
            destrect.h = srcrect.h;
        }
        
};

#endif /* CURSORCOMPONENT_HPP */