#ifndef CURSORCOMPONENT_HPP
#define CURSORCOMPONENT_HPP

#include "ECS.hpp"
#include "components.hpp"
#include "map.hpp"
#include "vector2D.hpp"
#include "shared.hpp"
#include "spritecomponent.hpp"
#include "keyboardcontroller.hpp"
#include "texturemanager.hpp"
#include "SDL2/SDL.h"

class CursorComponent : public SpriteComponent{
    
    private:
        KeyboardController *keyboardcontroller;

    public:
        // CursorComponent() = default;
        
        using SpriteComponent::SpriteComponent;
        
        
        void init() {
            SpriteComponent::init();
            keyboardcontroller = &entity->getComponent<KeyboardController>();
        }
        
        void update() override {
            SpriteComponent::update();
            // objectivepos.x = (int)positioncomponent->getPos().x * currenttilesize[0];
            // objectivepos.y = (int)positioncomponent->getPos().y * currenttilesize[1]; 
            keyboardcontroller->getLastPressed();      
            if (objectivepos.x != SpriteComponent::slidepos.x) {
                SpriteComponent::slidepos.x += geometricslide((objectivepos.x - SpriteComponent::slidepos.x));
            }
            // if (objectivepos.y != slidepos.y) {
                // slidepos.y += geometricslide((objectivepos.y - SpriteComponent::slidepos.y));
            // }
            // if ((objectivepos.x == slidepos.x) && (objectivepos.y == slidepos.y)) {
                // positioncomponent->setUpdatable(true);
            // }
            destrect.x = SpriteComponent::slidepos.x;
            destrect.y = SpriteComponent::slidepos.y;
        }
        
};

#endif /* CURSORCOMPONENT_HPP */