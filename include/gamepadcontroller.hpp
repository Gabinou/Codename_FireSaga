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
    std::vector<int> controller_indices; // joystick indices that are supported by the game controller interface
    short int joystick_deadzone = 8000;
    std::vector<short unsigned int> held_move;
    std::vector<std::vector<SDL_GameControllerButton>> held_button;
    double time_button = 0.;
    double time_move = 0.;
public:
    GamepadController() {
    }

    short int getDeadzone() {
        return (joystick_deadzone);
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

    void setController(int in_index) {
        controller = SDL_GameControllerOpen(controller_indices[in_index]);

    }

    void removeController(int in_joystick) {
        for (int i = 0; i < controller_indices.size(); i++) {
            if (controller_indices[i] == in_joystick) {
                SDL_Log("Removing controller %d at indice %d", controller_indices[i], i);
                controller_indices.erase(controller_indices.begin() + i);
                controller = NULL;
                break;
            }
        }

        if (controller_indices.size() > 0) {
            setController(0);
        }
    }

    void addController(int in_joystick) {
        SDL_Log("Adding controller %d", in_joystick);

        if (SDL_IsGameController(in_joystick)) {
            controller_indices.push_back(in_joystick);

            if (!controller) {
                controller = SDL_GameControllerOpen(in_joystick);

                if (controller) {
                    SDL_Log("Could not add gamecontroller %i: %s\n", in_joystick, SDL_GetError());
                }
            } else {
                SDL_Log("Controller already exists.");
            }
        } else {
            SDL_Log("in_joystick %d is not supported by the game controller interface ", in_joystick);
        }
    }

    SDL_GameController * getController() {
        return (controller);
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