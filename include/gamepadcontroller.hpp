#ifndef GAMEPADCONTROLLER_HPP
#define GAMEPADCONTROLLER_HPP

#include "ECS.hpp"
#include "game.hpp"
#include "map.hpp"

class GamepadController : public Component {

    private:
        PositionComponent * positioncomponent;
        Game * game;
        Map * map;
        SDL_GameController * controller = NULL;
        GamepadInputMap inputmap;
        int joystick_dead_zone = 8000;
    public:
        GamepadController() = default;

        GamepadController(Game * in_game) {
            setGame(in_game);
        }

        GamepadController(Game * in_game, Map * in_map) : GamepadController(in_game) {
            map = in_map;
        }

        void setGame(Game * in_game) {
            game = in_game;
        }

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

            positioncomponent = &entity->getComponent<PositionComponent>();
            inputmap = game->getGamepadInputMap();
            Manager & manager = entity->getManager();

        }

        void update() override {
            Sint16 mainxaxis = SDL_GameControllerGetAxis(controller, inputmap.mainxaxis[0]);
            Sint16 mainyaxis = SDL_GameControllerGetAxis(controller, inputmap.mainyaxis[0]);
            Sint16 secondxaxis = SDL_GameControllerGetAxis(controller, inputmap.secondxaxis[0]);
            Sint16 secondyaxis = SDL_GameControllerGetAxis(controller, inputmap.secondyaxis[0]);
            // printf("Controller axis val: %d\n", mainxaxis);

            if (mainxaxis > joystick_dead_zone) {
                printf("updatexmainxaxis");
                positioncomponent->addPos(Vector2D(1, 0));
            } else if (mainxaxis < -joystick_dead_zone) {
                positioncomponent->addPos(Vector2D(-1, 0));
            }

            if (mainyaxis > joystick_dead_zone) {
                positioncomponent->addPos(Vector2D(0, 1));
            } else if (mainyaxis < -joystick_dead_zone)  {
                positioncomponent->addPos(Vector2D(0, -1));
            }

            positioncomponent->setUpdatable(false);
        }
};



#endif /* GAMEPADCONTROLLER_HPP */