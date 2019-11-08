#ifndef KEYBOARDCONTROL_HPP
#define KEYBOARDCONTROL_HPP

#include "ecs.hpp"
#include "components.hpp"
#include "game.hpp"

// template <typename T> bool in_vector(in_vector std::vector<T>, T to_find){
    // for (auto it = std::begin(in_vector); it != std::end(in_vector); ++it) { 
        // if(it==to_find){
            // return(true);
        // } 
    // }
    // return(false);    
// }

bool is_pressed(const Uint8 *state_array, std::vector<SDL_Scancode> to_find){
    for (auto it = std::begin(to_find); it != std::end(to_find); ++it) { 
        if(state_array[*it]){
            return(true);
        } 
    }
    return(false);    
}

class KeyboardInputMapping {
    public:
    
        std::vector<SDL_Scancode> moveright{SDL_SCANCODE_D, SDL_SCANCODE_RIGHT};
        std::vector<SDL_Scancode> moveup{SDL_SCANCODE_W, SDL_SCANCODE_UP};
        std::vector<SDL_Scancode> movedown{SDL_SCANCODE_S, SDL_SCANCODE_DOWN};
        std::vector<SDL_Scancode> moveleft{SDL_SCANCODE_A, SDL_SCANCODE_LEFT};
        std::vector<SDL_Scancode> accept{SDL_SCANCODE_SPACE};
        std::vector<SDL_Scancode> cancel{SDL_SCANCODE_BACKSPACE};
        std::vector<SDL_Scancode> pause{SDL_SCANCODE_RETURN};

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
        if(Game::event.type == SDL_KEYDOWN || Game::event.type == SDL_KEYUP){
            printf("Is space pressed: %d\n", is_pressed(keyboard_state_array, accept));
            if (keyboard_state_array[SDL_SCANCODE_UP] && !(keyboard_state_array[SDL_SCANCODE_DOWN])){
                positioncomponent->addPos(Vector2D(0, -1));
            } else if  (!keyboard_state_array[SDL_SCANCODE_UP] && keyboard_state_array[SDL_SCANCODE_DOWN]){
                positioncomponent->addPos(Vector2D(0, 1));
            }
            if (!keyboard_state_array[SDL_SCANCODE_RIGHT] && keyboard_state_array[SDL_SCANCODE_LEFT]){
                positioncomponent->addPos(Vector2D(-1, 0));
            } else if (keyboard_state_array[SDL_SCANCODE_RIGHT] && !keyboard_state_array[SDL_SCANCODE_LEFT]){
                positioncomponent->addPos(Vector2D(1, 0));
            }
            positioncomponent->setUpdatable(false);
        }
    }
    
};




#endif /* KEYBOARDCONTROL_HPP */