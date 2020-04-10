#ifndef GAMEPADCONTROLLER_HPP
#define GAMEPADCONTROLLER_HPP

#include "ECS.hpp"
#include "game.hpp"
#include "map.hpp"
#include "utilities.hpp"
#include <vector>

class GamepadController : public Component {
    private:
        PositionComponent * positioncomponent;
        Game * game;
        Map * map;
        SDL_GameController * controller = NULL;
        GamepadInputMap inputmap;
        int joystick_dead_zone = 8000;
        std::vector<short unsigned int> held_move;
        std::vector<std::vector<SDL_GameControllerButton>> held_button;
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

        void setMap(Map * in_map) {
            map = in_map;
        }

        bool isPressed(std::vector<SDL_GameControllerButton> in_map) {
            for (int i = 0; i < in_map.size(); ++i) {
                if (SDL_GameControllerGetButton(controller, in_map[i])) {
                    return (true);
                }
            }

            return (false);
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

        void check_move(std::vector<short unsigned int> pressed_move) {
            if ((held_move == pressed_move) && (!pressed_move.empty())) {
                frames_move++;
            } else {
                held_move = pressed_move;
                frames_move = 0;
            }
        }

        void check_button(std::vector<std::vector<SDL_GameControllerButton>> pressed_button) {
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
            std::vector<short unsigned int> pressed_move{};
            std::vector<std::vector<SDL_GameControllerButton>> pressed_button{};

            if ((mainxaxis > joystick_dead_zone) || (secondxaxis > joystick_dead_zone)) {
                positioncomponent->addPos(1, 0);
                pressed_move.push_back(GAME::BUTTON::RIGHT);
            } else if ((mainxaxis < -joystick_dead_zone) || (secondxaxis < -joystick_dead_zone)) {
                positioncomponent->addPos(-1, 0);
                pressed_move.push_back(GAME::BUTTON::LEFT);
            }

            if ((mainyaxis > joystick_dead_zone) || (secondyaxis > joystick_dead_zone)) {
                positioncomponent->addPos(0, 1);
                pressed_move.push_back(GAME::BUTTON::UP);
            } else if ((mainyaxis < -joystick_dead_zone) || (secondyaxis < -joystick_dead_zone))  {
                positioncomponent->addPos(0, -1);
                pressed_move.push_back(GAME::BUTTON::DOWN);
            }

            if (isPressed(inputmap.moveright)) {
                positioncomponent->addPos(1, 0);
                pressed_move.push_back(GAME::BUTTON::RIGHT);
            } else if (isPressed(inputmap.moveleft)) {
                positioncomponent->addPos(-1, 0);
                pressed_move.push_back(GAME::BUTTON::LEFT);
            }

            if (isPressed(inputmap.moveup)) {
                positioncomponent->addPos(0, -1);
                pressed_move.push_back(GAME::BUTTON::UP);
            } else if (isPressed(inputmap.movedown)) {
                positioncomponent->addPos(0, 1);
                pressed_move.push_back(GAME::BUTTON::DOWN);
            }

            if (isPressed(inputmap.accept)) {
                pressed_button.push_back(inputmap.accept);
                short unsigned int toset = -1;
                // Entity * setter;
                // Entity * ontile = map->getTile(positioncomponent->getPos()[0], positioncomponent->getPos()[1]);

                if ((game->getState() == GAME::STATE::MAP) && (frames_button == 1)) {
                    SDL_Log("cursor Position, %d %d \n", positioncomponent->getPos()[0], positioncomponent->getPos()[1]);

                    // if (ontile) {
                    //     toset = GAME::STATE::UNITMOVE;
                    //     setter = ontile;
                    // } else {
                    //     toset = GAME::STATE::OPTIONS;
                    //     setter = entity;
                    // }
                } else if ((game->getState() == GAME::STATE::UNITMOVE) && (frames_button == 1)) {
                    // toset = GAME::STATE::UNITMENU;
                    // setter = entity;
                }

                // if (toset != -1) {game->setState(*setter, toset); }
            }

            if (isPressed(inputmap.cancel)) {
                pressed_button.push_back(inputmap.cancel);

                if ((game->getState() == GAME::STATE::UNITMENU) ||
                        (game->getState() == GAME::STATE::OPTIONS) ||
                        (game->getState() == GAME::STATE::UNITMOVE)) {
                    game->setState(*entity, GAME::STATE::MAP);
                }
            }

            check_move(pressed_move);
            check_button(pressed_button);
        }
};



#endif /* GAMEPADCONTROLLER_HPP */