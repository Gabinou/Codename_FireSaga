#ifndef KEYBOARDCONTROL_HPP
#define KEYBOARDCONTROL_HPP

#include "ecs.hpp"
#include "components.hpp"
#include "game.hpp"

class KeyboardController : public Component {
    
    public:
        PositionComponent *position;
        
    voic init() override {
        position = &entity->getComponent<PositionComponent>();
    }
    
    void update() override {
        // if (game::event.type == SDL_KEYDOWN){
            
        // }
        // if (game::event.type == SDL_KEYUP)
    }
    
};




#endif /* KEYBOARDCONTROL_HPP */