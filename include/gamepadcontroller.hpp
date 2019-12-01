#ifndef GAMEPADCONTROLLER_HPP
#define GAMEPADCONTROLLER_HPP

#include "ECS.hpp"
#include "game.hpp"
#include "map.hpp"
#include "textcomponent.hpp"

class GamepadController : public Component {

    private:
        PositionComponent * positioncomponent;
        int * tilesize;
        Game * game;
        Map * map;
        SDL_GameController * controller = NULL;
        const int joystick_dead_zone = 8000;
    public:
        void init() override {
            for (int i = 0; i < SDL_NumJoysticks(); ++i) {
                if (SDL_IsGameController(i)) {
                    controller = SDL_GameControllerOpen(i);

                    if (controller) {
                        break;
                    } else {
                        fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
                    }
                }
            }
        }

        void update() override {
            Sint16 axisvalx = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
            Sint16 axisvaly = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
            printf("Controller axis val: %d\n", axisvalx);
        }
};



#endif /* GAMEPADCONTROLLER_HPP */