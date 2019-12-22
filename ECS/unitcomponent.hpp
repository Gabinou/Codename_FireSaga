#ifndef UNITCOMPONENT_HPP
#define UNITCOMPONENT_HPP

#include "ECS.hpp"
#include "game.hpp"
#include "shared.hpp"
#include "SDL2/SDL.h"

class UnitComponent : public Component {
    private:
        bool updatable = true;
        int position[2] = {0};
        int bounds[4] = {0, 255, 0, 255}; //xmin, xmax, ymin, ymax
        Map * map = nullptr;
        // On first init of the component, entity is still a nullptr.

};


#endif /* UNITCOMPONENT_HPP */