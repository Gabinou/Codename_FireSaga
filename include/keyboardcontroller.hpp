#ifndef KEYBOARDCONTROLLER_HPP
#define KEYBOARDCONTROLLER_HPP

#include "map.hpp"
#include "utilities.hpp"

class KeyboardController {
private:
    std::vector<std::vector<SDL_Scancode>> held_button;
    std::vector<std::vector<SDL_Scancode>> held_move;
    double time_button = 0.;
    double time_move = 0.;
    short unsigned int * tilesize;
public:
    KeyboardController() = default;

    void setTilesize(short unsigned int * in_tilesize) {
        tilesize = in_tilesize;
    }

    short unsigned int * getTilesize() {
        return (tilesize);
    }

    double getHeldmove() {
        return (time_move);
    }

    double getHeldbutton() {
        return (time_button);
    }

    void check_move(std::vector<std::vector<SDL_Scancode>>in_pressed, double dt) {
        if ((held_move == in_pressed) && (!in_pressed.empty())) {
            time_move += dt;
        } else {
            held_move = in_pressed;
            time_move = 0.;
        }
    }

    void check_button(std::vector<std::vector<SDL_Scancode>>in_pressed, double dt) {
        if ((held_button == in_pressed) && (!in_pressed.empty())) {
            time_button += dt;
        } else {
            held_button = in_pressed;
            time_button = 0.;
        }
    }

    bool is_pressed(const Uint8 * state_array, std::vector<SDL_Scancode> to_find) {
        for (auto it = std::begin(to_find); it != std::end(to_find); ++it) {
            if (state_array[*it]) {
                return (true);
            }
        }

        return (false);
    }
};

#endif /* KEYBOARDCONTROLLER_HPP */