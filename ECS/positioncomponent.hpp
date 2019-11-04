#ifndef POSITIONCOMPONENT_HPP
#define POSITIONCOMPONENT_HPP

#include "ECS.hpp"
#include "components.hpp"
#include "vector2D.hpp"
#include "game.hpp"
#include "tilesize.hpp"

class PositionComponent : public Component, Tilesize {
    private:
        Vector2D pixelposition;
        Vector2D tileposition;
    public:
        Vector2D position;
        
        PositionComponent(){
            position.x = 0;
            position.y = 0;
        }
        
        PositionComponent(short unsigned int x, short unsigned int y){
            static int * currenttilesize = (getTilesize());
            printf("currenttilesize: %d \n", currenttilesize[0]);
            position.x = x * currenttilesize[0];
            position.y = y * currenttilesize[1];
        }

        void setPos(short unsigned int x, short unsigned int y) {
            static int * currenttilesize = (getTilesize());
            position.x = x * currenttilesize[0];
            position.y = y * currenttilesize[1];
        }
        
        Vector2D getPos() {
            return(position);
        }
        
        void update() override {
        }
};


#endif /* POSITIONCOMPONENT_HPP */