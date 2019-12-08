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
        std::vector<std::string> held_move;
        std::vector<std::string> held_button;
        unsigned int frames_move = 0;
        unsigned int frames_button = 0;
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

        int getHeldmove() {
            return (frames_move);
        }

        void check_move(std::vector<std::string>pressed_move) {
            if ((held_move == pressed_move) && (!pressed_move.empty())) {
                frames_move++;
            } else {
                held_move = pressed_move;
                frames_move = 0;
            }
        }


        void check_button(std::vector<std::string>pressed_button) {
            if ((held_button == pressed_button) && (!pressed_button.empty())) {
                frames_button++;
            } else {
                held_button = pressed_button;
                frames_button = 0;
            }
        }

        void update() override {
            Sint16 mainxaxis = SDL_GameControllerGetAxis(controller, inputmap.mainxaxis[0]);
            Sint16 mainyaxis = SDL_GameControllerGetAxis(controller, inputmap.mainyaxis[0]);
            Sint16 secondxaxis = SDL_GameControllerGetAxis(controller, inputmap.secondxaxis[0]);
            Sint16 secondyaxis = SDL_GameControllerGetAxis(controller, inputmap.secondyaxis[0]);
            std::vector<std::string> pressed_move{};
            std::vector<std::string> pressed_button{};

            if (mainxaxis > joystick_dead_zone) {
                positioncomponent->addPos(Vector2D(1, 0));
                pressed_move.push_back("right");
            } else if (mainxaxis < -joystick_dead_zone) {
                positioncomponent->addPos(Vector2D(-1, 0));
                pressed_move.push_back("left");
            }

            if (mainyaxis > joystick_dead_zone) {
                positioncomponent->addPos(Vector2D(0, 1));
                pressed_move.push_back("up");
            } else if (mainyaxis < -joystick_dead_zone)  {
                positioncomponent->addPos(Vector2D(0, -1));
                pressed_move.push_back("down");
            }

            check_move(pressed_move);
            check_move(pressed_button);
        }
};



#endif /* GAMEPADCONTROLLER_HPP */