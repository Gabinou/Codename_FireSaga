#ifndef MOUSECONTROLLER_HPP
#define MOUSECONTROLLER_HPP

#include "structs.hpp"

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