#ifndef KEYBOARDCONTROL_HPP
#define KEYBOARDCONTROL_HPP

#include "ecs.hpp"
#include "components.hpp"
#include "game.hpp"

class KeyboardController : public Component, Tilesize {
    
    private:
        unsigned int tilesize[2] = {32, 32};
    
    public:
        PositionComponent *positioncomponent;
        
    void init() override {
        positioncomponent = &entity->getComponent<PositionComponent>();
    }
    
    void update() override {
        if (Game::event.type == SDL_KEYDOWN){
            switch (Game::event.key.keysym.sym){
                case SDLK_d:
                
                    positioncomponent->position.add(Vector2D(1,0));
                    break;
                case SDLK_a:
                    positioncomponent->position.add(Vector2D(-1,0));
                    break;
                case SDLK_s:
                    positioncomponent->position.add(Vector2D(0,1));
                    break;                
                case SDLK_w:
                    positioncomponent->position.add(Vector2D(0,-1));
                    break;
                default:
                    break;
            }
        }
    }
    
};




#endif /* KEYBOARDCONTROL_HPP */