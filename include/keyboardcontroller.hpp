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

class KeyboardController : public Component, Tilesize, KeyboardInputMapping {
    
    private:
        unsigned int tilesize[2] = {32, 32};
    
    public:
        PositionComponent *positioncomponent;
        
    void init() override {
        positioncomponent = &entity->getComponent<PositionComponent>();
    }
    
    void update() override {
        if (Game::event.type == SDL_KEYDOWN){
            
            if (std::find(this->moveright.begin(), this->moveright.end(), Game::event.key.keysym.sym) != this->moveright.end()){
                positioncomponent->position.add(Vector2D(1,0));
            } else if (std::find(this->moveleft.begin(), this->moveleft.end(), Game::event.key.keysym.sym) != this->moveleft.end()){
                positioncomponent->position.add(Vector2D(-1,0));
            } else if (std::find(this->movedown.begin(), this->movedown.end(), Game::event.key.keysym.sym) != this->movedown.end()){
                positioncomponent->position.add(Vector2D(0,1));
            } else if (std::find(this->moveup.begin(), this->moveup.end(), Game::event.key.keysym.sym) != this->moveup.end()){
                positioncomponent->position.add(Vector2D(0,-1));
            }
                 
        }
    }
    
};




#endif /* KEYBOARDCONTROL_HPP */