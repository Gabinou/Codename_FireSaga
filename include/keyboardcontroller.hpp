#ifndef KEYBOARDCONTROL_HPP
#define KEYBOARDCONTROL_HPP

#include "map.hpp"
#include "utilities.hpp"

class KeyboardController {
private:
    std::vector<std::vector<SDL_Scancode>> held_button;
    std::vector<std::vector<SDL_Scancode>> held_move;
    unsigned int frames_button = 0;
    unsigned int frames_move = 0;
    short unsigned int * tilesize;
public:
    KeyboardController() = default;

    void setTilesize(short unsigned int * in_tilesize) {
        tilesize = in_tilesize;
    }

    short unsigned int * getTilesize() {
        return (tilesize);
    }

    unsigned int getHeldmove() {
        return (frames_move);
    }

    unsigned int getHeldbutton() {
        return (frames_button);
    }

    void check_move(std::vector<std::vector<SDL_Scancode>>in_pressed) {
        if ((held_move == in_pressed) && (!in_pressed.empty())) {
            frames_move++;
        } else {
            held_move = in_pressed;
            frames_move = 0;
        }
    }

    void check_button(std::vector<std::vector<SDL_Scancode>>in_pressed) {
        if ((held_button == in_pressed) && (!in_pressed.empty())) {
            frames_button++;
        } else {
            held_button = in_pressed;
            frames_button = 0;
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



#endif /* KEYBOARDCONTROL_HPP */