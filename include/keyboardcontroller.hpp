#ifndef KEYBOARDCONTROL_HPP
#define KEYBOARDCONTROL_HPP

#include "ecs.hpp"
#include "components.hpp"
#include "game.hpp"
#include "keyboardinputmapping.hpp"

struct LastPressed {
    std::vector<SDL_Scancode> key; 
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
    
    void check_pressed(std::vector<SDL_Scancode> pressed){
        if (lastpressed.key == pressed) {
            lastpressed.activeframes++;
        } else {
            lastpressed.key = pressed;
            lastpressed.activeframes = 0;
        } 
    }
    
    void update() override {
        const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
        printf("Up active frames: %d\n", lastpressed.activeframes);
        if(Game::event.type == SDL_KEYDOWN || Game::event.type == SDL_KEYUP){
            if (is_pressed(keyboard_state_array, moveup) && !is_pressed(keyboard_state_array, movedown)){
                positioncomponent->addPos(Vector2D(0, -1));
                check_pressed(moveup);
            } else if  (!is_pressed(keyboard_state_array, moveup) && is_pressed(keyboard_state_array, movedown)){
                positioncomponent->addPos(Vector2D(0, 1));
                check_pressed(movedown);
            } 
            if (!is_pressed(keyboard_state_array, moveright) && is_pressed(keyboard_state_array, moveleft)){
                positioncomponent->addPos(Vector2D(-1, 0));
                check_pressed(moveleft);
            } else if (is_pressed(keyboard_state_array, moveright) && !is_pressed(keyboard_state_array, moveleft)){
                positioncomponent->addPos(Vector2D(1, 0));
                check_pressed(moveright);
            } else {
                lastpressed.activeframes=0;
            }
            positioncomponent->setUpdatable(false);
        }
    }
    
};




#endif /* KEYBOARDCONTROL_HPP */