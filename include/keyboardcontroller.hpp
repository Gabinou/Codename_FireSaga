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
            
            if (std::find(this->moveright.begin(), this->moveright.end(), Game::event.key.keysym.sym) != this->moveright.end())
            {
                printf("moveright");
                positioncomponent->position.add(Vector2D(1,0));
            }
            switch (Game::event.key.keysym.sym){
                case SDLK_a:
                    printf("moveleft");
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