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
        if (Game::event.type == SDL_KEYDOWN){
            std::string currentcombination;
            static int * currenttilesize = (getTilesize());
            if (std::find(this->moveright.begin(), this->moveright.end(), Game::event.key.keysym.sym) != this->moveright.end()){
                // positioncomponent->position.add(Vector2D(currenttilesize[0],0));
                currentcombination += "right";
            }
            if (std::find(this->moveleft.begin(), this->moveleft.end(), Game::event.key.keysym.sym) != this->moveleft.end()){
                // positioncomponent->position.add(Vector2D(-currenttilesize[0],0));
                currentcombination += "left";
            }
            if (std::find(this->movedown.begin(), this->movedown.end(), Game::event.key.keysym.sym) != this->movedown.end()){
                // positioncomponent->position.add(Vector2D(0,currenttilesize[1]));
                currentcombination += "down";
            } 
            if (std::find(this->moveup.begin(), this->moveup.end(), Game::event.key.keysym.sym) != this->moveup.end()){
                // positioncomponent->position.add(Vector2D(0,-currenttilesize[1]));
                currentcombination += "up";
            }
            if (currentcombination.compare(this->lastpressed.combination) == 0) {
                lastpressed.activeframes++;
            } else {
                lastpressed.combination = currentcombination;
                lastpressed.activeframes = 0;
            }
            if ((lastpressed.combination.compare("right") == 0) && (lastpressed.activeframes == 0)) {
                positioncomponent->position.add(Vector2D(currenttilesize[0],0));
            }
            if ((lastpressed.combination.compare("left") == 0) && (lastpressed.activeframes == 0)) {
                positioncomponent->position.add(Vector2D(-currenttilesize[0],0));
            }
            if ((lastpressed.combination.compare("up") == 0) && (lastpressed.activeframes == 0)) {
                positioncomponent->position.add(Vector2D(0, -currenttilesize[1]));
            }
            if ((lastpressed.combination.compare("down") == 0) && (lastpressed.activeframes == 0)) {
                positioncomponent->position.add(Vector2D(0, currenttilesize[1]));
            }
        }
        if (Game::event.type == SDL_KEYUP){ 
            lastpressed.combination = "";
            lastpressed.activeframes = 0;
        }
    }
    
};




#endif /* KEYBOARDCONTROL_HPP */