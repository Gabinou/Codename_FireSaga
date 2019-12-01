#ifndef KEYBOARDCONTROL_HPP
#define KEYBOARDCONTROL_HPP

#include "ECS.hpp"
#include "game.hpp"
#include "map.hpp"
#include "textcomponent.hpp"

class KeyboardController : public Component {

    private:
        PositionComponent * positioncomponent;
        int * tilesize;
        Game * game;
        Map * map;
    public:
       
};



#endif /* KEYBOARDCONTROL_HPP */