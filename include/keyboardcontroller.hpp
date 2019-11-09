#ifndef KEYBOARDCONTROL_HPP
#define KEYBOARDCONTROL_HPP

#include "ecs.hpp"
#include "components.hpp"
#include "game.hpp"
#include "keyboardinputmapping.hpp"

struct LastPressed {
    std::vector<std::vector<SDL_Scancode>> keys; 
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
    
    void check_pressed(std::vector<std::vector<SDL_Scancode>>pressed){
        // printf("is empty: %d\n", pressed.empty());
        if ((lastpressed.keys == pressed) && (!pressed.empty())) {
            lastpressed.activeframes++;
        } else {
            lastpressed.keys = pressed;
            lastpressed.activeframes = 0;
        }
    }
    
    void update() override {
        const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
        std::vector<std::vector<SDL_Scancode>> current_pressed{}; 
        printf("Up active frames: %d\n", lastpressed.activeframes);
        if(Game::event.type == SDL_KEYDOWN || Game::event.type == SDL_KEYUP){
            if (is_pressed(keyboard_state_array, moveup) && !is_pressed(keyboard_state_array, movedown)){
                positioncomponent->addPos(Vector2D(0, -1));
                current_pressed.push_back(moveup);
            } else if  (!is_pressed(keyboard_state_array, moveup) && is_pressed(keyboard_state_array, movedown)){
                positioncomponent->addPos(Vector2D(0, 1));
                current_pressed.push_back(movedown);
            } 
            if (!is_pressed(keyboard_state_array, moveright) && is_pressed(keyboard_state_array, moveleft)){
                positioncomponent->addPos(Vector2D(-1, 0));
                current_pressed.push_back(moveleft);
            } else if (is_pressed(keyboard_state_array, moveright) && !is_pressed(keyboard_state_array, moveleft)){
                positioncomponent->addPos(Vector2D(1, 0));
                current_pressed.push_back(moveright);
            }
            // printf("is empty: %d\n", current_pressed.empty());
            check_pressed(current_pressed);
            positioncomponent->setUpdatable(false);
        }
    }
    
};




#endif /* KEYBOARDCONTROL_HPP */