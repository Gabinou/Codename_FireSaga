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
    unsigned int frames_move = 0;
    unsigned int frames_button = 0;
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

    unsigned int getHeldmove() {
        return (frames_move);
    }

    unsigned int getHeldbutton() {
        return (frames_button);
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

};



#endif /* GAMEPADCONTROLLER_HPP */