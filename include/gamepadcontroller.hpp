#ifndef GAMEPADCONTROLLER_HPP
#define GAMEPADCONTROLLER_HPP

#include "game.hpp"
#include "map.hpp"
#include "utilities.hpp"
#include <vector>

class GamepadController {
private:
    SDL_GameController * controller = NULL;
    GamepadInputMap inputmap;
    int joystick_dead_zone = 8000;
    std::vector<short unsigned int> held_move;
    std::vector<std::vector<SDL_GameControllerButton>> held_button;
    double time_button = 0.;
    double time_move = 0.;
public:
    GamepadController() {
        init();
    }

    int getDeadzone() {
        return (joystick_dead_zone);
    }

    GamepadInputMap getInputMap() {
        return (inputmap);
    }

    void setInputMap(GamepadInputMap in_inputmap) {
        inputmap = in_inputmap;
    }

    double getHeldmove() {
        return (time_move);
    }

    double getHeldbutton() {
        return (time_button);
    }

    bool isPressed(std::vector<SDL_GameControllerButton> in_map) {
        for (int i = 0; i < in_map.size(); ++i) {
            if (SDL_GameControllerGetButton(controller, in_map[i])) {
                return (true);
            }
        }

        return (false);
    }

    SDL_GameController * getController() {
        return (controller);
    }

    void init()  {
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

    void check_move(std::vector<short unsigned int> in_pressed, double dt) {
        if ((held_move == in_pressed) && (!in_pressed.empty())) {
            time_move += dt;
        } else {
            held_move = in_pressed;
            time_move = 0.;
        }
    }

    void check_button(std::vector<std::vector<SDL_GameControllerButton>> in_pressed, double dt) {
        if ((held_button == in_pressed) && (!in_pressed.empty())) {
            time_button += dt;
        } else {
            held_button = in_pressed;
            time_button = 0.;
        }
    }
};

#endif /* GAMEPADCONTROLLER_HPP */