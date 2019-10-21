#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include "ECS.hpp"

class PositionComponent : public Component {
    private:
        int xpos;
        int ypos;
    public:
        int x(){return(xpos);};
        int y(){return(ypos);};
        
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
    
};


#endif /* COMPONENTS_HPP */