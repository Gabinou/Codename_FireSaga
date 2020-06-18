#ifndef KEYBOARDCONTROLLER_HPP
#define KEYBOARDCONTROLLER_HPP

#include "map.hpp"
#include "utilities.hpp"

class KeyboardController {
private:
    std::vector<std::vector<SDL_Scancode>> held_button;
    std::vector<std::vector<SDL_Scancode>> held_move;
    int64_t time_button = 0.;
    int64_t time_move = 0.;
    int16_t * tilesize;
public:
    KeyboardController() = default;

    void setTilesize(int16_t * in_tilesize) {
        tilesize = in_tilesize;
    }

    int16_t * getTilesize() {
        return (tilesize);
    }

    int64_t getHeldmove() {
        return (time_move);
    }

    int64_t getHeldbutton() {
        return (time_button);
    }

    void check_move(std::vector<std::vector<SDL_Scancode>>in_pressed, int64_t dt) {
        if ((held_move == in_pressed) && (!in_pressed.empty())) {
            time_move += dt;
        } else {
            held_move = in_pressed;
            time_move = 0.;
        }
    }

    void check_button(std::vector<std::vector<SDL_Scancode>>in_pressed, int64_t dt) {
        if ((held_button == in_pressed) && (!in_pressed.empty())) {
            time_button += dt;
        } else {
            held_button = in_pressed;
            time_button = 0.;
        }
    }

    bool is_pressed(const Uint8 * state_array, std::vector<SDL_Scancode> to_find) {
        bool out = false;

        for (auto it = std::begin(to_find); it != std::end(to_find); ++it) {
            if (state_array[*it]) {
                out = true;
            }
        }

        return (out);
    }
};

#endif /* KEYBOARDCONTROLLER_HPP */