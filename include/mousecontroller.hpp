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

    void setPixelPos(short int in_x, short int in_y) {
        pixel_pos.x = in_x;
        pixel_pos.y = in_y;
    }    

    void setPixelPos(Point in_point) {
        pixel_pos.x = in_point.x;
        pixel_pos.y = in_point.y;
    }

    void setTilemapPos(short int in_x, short int in_y) {
        tilemap_pos.x = in_x;
        tilemap_pos.y = in_y;
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

};

#endif /* MOUSECONTROLLER_HPP */