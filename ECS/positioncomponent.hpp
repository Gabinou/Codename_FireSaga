#ifndef POSITIONCOMPONENT_HPP
#define POSITIONCOMPONENT_HPP

#include "ECS.hpp"
#include "components.hpp"
#include "vector2D.hpp"
#include "game.hpp"
#include "tilesize.hpp"

class PositionComponent : public Component {
    private:
        Vector2D pixelposition;
        Vector2D tileposition;
        bool updatable = true;
    public:
        Vector2D position;
        
        PositionComponent(){
            position.x = 0;
            position.y = 0;
        }
        
        void setUpdatable(bool up_in){
            updatable = up_in;
        }
        
        PositionComponent(short unsigned int x, short unsigned int y){
            position.x = x;
            position.y = y;
        }

        void setPos(short unsigned int x, short unsigned int y) {
            printf("%d\n", updatable);
            if (updatable) {
                position.x = x;
                position.y = y;
            }
        }
        
        Vector2D getPos() {
            return(position);
        }
        
        void update() override {
        }
};


#endif /* POSITIONCOMPONENT_HPP */