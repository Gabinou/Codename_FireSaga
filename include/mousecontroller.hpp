#ifndef MOUSECONTROLLER_HPP
#define MOUSECONTROLLER_HPP

#include "game.hpp"
#include "map.hpp"
#include "utilities.hpp"
#include <vector>

class MouseController {
private:
    MouseInputMap inputmap;
    std::vector<unsigned char> held_button;
    double time_button = 0.;
    Point pixel_pos;
    Point tilemap_pos;

public:

    void mouseEvent(SDL_MouseButtonEvent in_event) {
        switch (in_event.type) {
            case SDL_MOUSEBUTTONDOWN:
                addHeld(in_event.button);
                break;

            case SDL_MOUSEBUTTONUP:
                removeHeld(in_event.button);
                break;
        }
    }

    void removeHeld(unsigned char in_button) {
        // for (short int i = 0; held_button.size(); i++) {
        //     if (held_button[i] == in_button) {
        //         held_button.re
        //     }
        // }
    }

    void addHeld(unsigned char in_button) {
        held_button.push_back(in_button);
    }

    MouseInputMap getInputMap() {
        return (inputmap);
    }

    void setInputMap(MouseInputMap in_inputmap) {
        inputmap = in_inputmap;
    }

    double getHeldbutton() {
        return (time_button);
    }

    void check_button(std::vector<unsigned char> in_pressed, double dt) {
        if ((held_button == in_pressed) && (!in_pressed.empty())) {
            time_button += dt;
        } else {
            held_button = in_pressed;
            time_button = 0.;
        }
    }

};

#endif /* MOUSECONTROLLER_HPP */