#ifndef POSITIONCOMPONENT_HPP
#define POSITIONCOMPONENT_HPP

#include "ECS.hpp"
#include "components.hpp"

class PositionComponent : public Component {
    private:
        int xpos;
        int ypos;
    public:
        
        PositionComponent(){
            xpos = 0;
            ypos = 0;
        }
        
        PositionComponent(int x, int y){
            xpos = x;
            ypos = y;
        }

        void setPos(int x, int y) {
            xpos = x;
            ypos = y;
        }
        
        void init() override {
            xpos = 0;
            ypos = 0;
        }
        
        void update() override {
            xpos++;
            ypos++;
        }
        int x() {return(xpos);}
        void x(int x) {xpos = x;}
        void y(int y) {ypos = y;}
        int y() {return(ypos);}
};


#endif /* POSITIONCOMPONENT_HPP */