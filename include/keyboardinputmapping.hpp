#ifndef KEYBOARDINPUTMAPPING_HPP
#define KEYBOARDINPUTMAPPING_HPP

#include "ecs.hpp"
#include "components.hpp"
#include "game.hpp"

class KeyboardInputMapping {
    public:
        std::vector<SDL_Scancode> moveright{SDL_SCANCODE_D, SDL_SCANCODE_RIGHT};
        std::vector<SDL_Scancode> moveup{SDL_SCANCODE_W, SDL_SCANCODE_UP};
        std::vector<SDL_Scancode> movedown{SDL_SCANCODE_S, SDL_SCANCODE_DOWN};
        std::vector<SDL_Scancode> moveleft{SDL_SCANCODE_A, SDL_SCANCODE_LEFT};
        std::vector<SDL_Scancode> accept{SDL_SCANCODE_SPACE};
        std::vector<SDL_Scancode> cancel{SDL_SCANCODE_BACKSPACE};
        std::vector<SDL_Scancode> pause{SDL_SCANCODE_RETURN};
};

#endif /* KEYBOARDINPUTMAPPING_HPP */