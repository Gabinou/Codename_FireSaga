#ifndef KEYBOARDCONTROL_HPP
#define KEYBOARDCONTROL_HPP

#include "ECS.hpp"
#include "components.hpp"
#include "game.hpp"
#include "keyboardinputmapping.hpp"

struct LastPressed {
    std::vector<std::vector<SDL_Scancode>> keys; 
    unsigned int pressed_frames = 0; 
};

class KeyboardController : public Component, Tilesize, KeyboardInputMapping {
    
    private:
        LastPressed lastpressed;

    public:
        PositionComponent *positioncomponent;
        
        void init() override {
            positioncomponent = &entity->getComponent<PositionComponent>();
        }
        
        LastPressed getLastPressed(){
            return(lastpressed);
        }
        
        void check_pressed(std::vector<std::vector<SDL_Scancode>>pressed){
            // printf("is empty: %d\n", pressed.empty());
            if ((lastpressed.keys == pressed) && (!pressed.empty())) {
                lastpressed.pressed_frames++;
            } else {
                lastpressed.keys = pressed;
                lastpressed.pressed_frames = 0;
            }
        }
        
        void update() override {
            const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
            std::vector<std::vector<SDL_Scancode>> current_pressed{}; 
            printf("Is pressed? %d\n", is_pressed(keyboard_state_array, movedown));
            // printf("repeats? %d\n", Game::event.key.repeat);
            // printf("updates %d %d\n", (Game::event.type == SDL_KEYDOWN), (Game::event.type == SDL_KEYUP));
            // if(Game::event.type == SDL_KEYDOWN || Game::event.type == SDL_KEYUP){
                // // printf("Ispressed: %d %d \n", is_pressed(keyboard_state_array, moveright), is_pressed(keyboard_state_array, moveleft));
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
                    // printf("Right is pressed %d \n", lastpressed.pressed_frames);
                }
                check_pressed(current_pressed);
                positioncomponent->setUpdatable(false);
            // }
        }
    
};




#endif /* KEYBOARDCONTROL_HPP */