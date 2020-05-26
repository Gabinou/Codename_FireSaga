#ifndef MOUSECONTROLLER_HPP
#define MOUSECONTROLLER_HPP

#include "game.hpp"
#include "map.hpp"
#include "utilities.hpp"
#include <vector>

class MouseController {
private:
    MouseInputMap inputmap;
public:
    MouseInputMap getInputMap() {
        return (inputmap);
    }

    void setInputMap(MouseInputMap in_inputmap) {
        inputmap = in_inputmap;
    }
};

#endif /* MOUSECONTROLLER_HPP */