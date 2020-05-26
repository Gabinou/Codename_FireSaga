#ifndef GAMEPADCONTROLLER_HPP
#define GAMEPADCONTROLLER_HPP

#include "game.hpp"
#include "map.hpp"
#include "utilities.hpp"

class GamepadController {
private:
    SDL_GameController * controller = NULL;
    GamepadInputMap inputmap;
    std::vector<int> controller_indices; // joystick indices that are supported by the game controller interface
    // used controller should always be zero.
    short int joystick_deadzone = 8000;
    std::vector<short unsigned int> held_move;
    std::vector<std::vector<SDL_GameControllerButton>> held_button;
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

    bool isPressed(std::vector<SDL_GameControllerButton> in_map) {
        bool out = false;

        for (int i = 0; i < in_map.size(); ++i) {
            if (SDL_GameControllerGetButton(controller, in_map[i])) {
                out = true;
            }
        }

        return (out);
    }

    void setController(int in_index) {
        if (controller_indices.size() > 0) {
            controller = SDL_GameControllerOpen(controller_indices[in_index]);
        }
    }

    void removeController() {
        SDL_Log("Removing controller");
        std::vector<int> temp = controller_indices;

        for (int i = 0; i < controller_indices.size(); i++) {
            if (!SDL_IsGameController(controller_indices[i])) {
                temp.erase(temp.begin() + i - controller_indices.size() + temp.size());
            }
        }

        controller_indices = temp;
        // SDL_Log("controller_indices size: %d", controller_indices.size());
        controller = NULL;
        setController(0);
    }

    void addController(int in_joystick) {
        SDL_Log("Adding controller %d", in_joystick);

        if (SDL_IsGameController(in_joystick)) {
            controller_indices.push_back(in_joystick);
            // SDL_Log("Added controller %d", controller_indices[controller_indices.size() - 1]);

            if (!controller) {
                controller = SDL_GameControllerOpen(in_joystick);

                if (!controller) {
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

};

#endif /* GAMEPADCONTROLLER_HPP */