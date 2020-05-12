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
public:

    void removeHeld(unsigned char in_button) {
        std::vector<unsigned char> temp = held_button;

        for (short int i = 0; i < held_button.size(); i++) {
            if (held_button[i] == in_button) {
                temp.erase(temp.begin() + i + temp.size() - held_button.size());
            }
        }

        held_button = temp;
        time_button = 0.;
    }

    void addHeld(unsigned char in_button) {
        if (!cppisin(in_button, held_button)) {
            held_button.push_back(in_button);
        }

        time_button = 0.;
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

    void check_button(double dt) {
        if (!held_button.empty()) {
            time_button += dt;
        } else {
            time_button = 0.;
        }
    }

    bool isPressed(std::vector<unsigned char> in_map) {
        bool out = false;

        for (short int i = 0; i < in_map.size(); i++) {
            if (cppisin(in_map[i], held_button)) {
                out = true;
            }
        }

        return (out);
    }

};

#endif /* MOUSECONTROLLER_HPP */