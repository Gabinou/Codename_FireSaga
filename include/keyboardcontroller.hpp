#ifndef KEYBOARDCONTROL_HPP
#define KEYBOARDCONTROL_HPP

#include "ecs.hpp"
#include "components.hpp"
#include "game.hpp"

class KeyboardInputMapping {
    public:
        std::vector<SDL_Keycode> moveright{SDLK_d, SDLK_RIGHT} ;
        std::vector<SDL_Keycode> moveup{SDLK_w, SDLK_UP} ;
        std::vector<SDL_Keycode> movedown{SDLK_s, SDLK_DOWN} ;
        std::vector<SDL_Keycode> moveleft{SDLK_a, SDLK_LEFT} ;
};
struct LastPressed {
    std::string combination; 
    unsigned int activeframes = 0; 
};
class KeyboardController : public Component, Tilesize, KeyboardInputMapping {
    
    private:
        LastPressed lastpressed;

    public:
        PositionComponent *positioncomponent;
        
    void init() override {
        positioncomponent = &entity->getComponent<PositionComponent>();
        
        
    }

    void update() override {
        const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
        std::string currentcombination;
        if(Game::event.type == SDL_KEYDOWN || Game::event.type == SDL_KEYUP){
            if (keyboard_state_array[SDL_SCANCODE_UP] && !(keyboard_state_array[SDL_SCANCODE_DOWN])){
                positioncomponent->addPos(Vector2D(0, -1));
                // positioncomponent->position.add(Vector2D(0,-1));
            } else if  (!keyboard_state_array[SDL_SCANCODE_UP] && keyboard_state_array[SDL_SCANCODE_DOWN]){
                // positioncomponent->position.add(Vector2D(0,1));
                positioncomponent->addPos(Vector2D(0, 1));
            }
            
            if (!keyboard_state_array[SDL_SCANCODE_RIGHT] && keyboard_state_array[SDL_SCANCODE_LEFT]){
                positioncomponent->addPos(Vector2D(-1, 0));
                // positioncomponent->position.add(Vector2D(-1,0));
            } else if (keyboard_state_array[SDL_SCANCODE_RIGHT] && !keyboard_state_array[SDL_SCANCODE_LEFT]){
                // positioncomponent->position.add(Vector2D(1,0));
                positioncomponent->addPos(Vector2D(1, 0));
            }
            positioncomponent->setUpdatable(false);
        }
    }
    
};




#endif /* KEYBOARDCONTROL_HPP */