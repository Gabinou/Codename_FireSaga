#ifndef POSITIONCOMPONENT_HPP
#define POSITIONCOMPONENT_HPP

#include "ECS.hpp"
#include "components.hpp"
#include "vector2D.hpp"

class PositionComponent : public Component {
    public:
        Vector2D position;
        
        PositionComponent(){
            position.x = 0;
            position.y = 0;
        }
        
        PositionComponent(short unsigned int x, short unsigned int y){
            position.x = x;
            position.y = y;
        }

        void setPos(short unsigned int x, short unsigned int y) {
            position.x = x;
            position.y = y;
        }
        
        Vector2D getPos() {
            return(position);
        }
        
        void update() override {
        }
};


#endif /* POSITIONCOMPONENT_HPP */