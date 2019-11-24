#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include "vector2D.hpp"
#include "shared.hpp"

struct InputMap {
    std::vector<SDL_Scancode> moveright{SDL_SCANCODE_RIGHT, SDL_SCANCODE_D, SDL_SCANCODE_L};
    std::vector<SDL_Scancode> moveup{SDL_SCANCODE_W, SDL_SCANCODE_UP, SDL_SCANCODE_I};
    std::vector<SDL_Scancode> movedown{SDL_SCANCODE_S, SDL_SCANCODE_DOWN, SDL_SCANCODE_K};
    std::vector<SDL_Scancode> moveleft{SDL_SCANCODE_A, SDL_SCANCODE_LEFT, SDL_SCANCODE_J};
    std::vector<SDL_Scancode> accept{SDL_SCANCODE_SPACE};
    std::vector<SDL_Scancode> cancel{SDL_SCANCODE_BACKSPACE};
    std::vector<SDL_Scancode> stats{};
    std::vector<SDL_Scancode> menu_right{};
    std::vector<SDL_Scancode> menu_left{};
    std::vector<SDL_Scancode> minimap{};
    std::vector<SDL_Scancode> faster{}; // Toggle or Hold?
    std::vector<SDL_Scancode> pause{SDL_SCANCODE_RETURN};
};


class Game {

    public:
        Game();
        ~Game();

        void init(const char * title, int xpos, int ypos, int width, int height, bool fullscreen);

        void handleEvents();
        void update();
        void render();
        void clean();

        void setFontsize(int in_fontsize);

        int getFontsize();

        InputMap getInputMap();

        void setContext(std::string in_context);
        void setContext(const char * in_context);

        std::string getContext();


        bool running();

        static SDL_Renderer * renderer;
        static SDL_Event event;
        static TTF_Font * font;

    private:

        InputMap inputMap;
        int fontsize;
        bool isRunning;
        int count;
        SDL_Window * window;
        std::string context;
};



#endif /* GAME_HPP */