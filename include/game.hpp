#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include "vector2D.hpp"
#include "shared.hpp"

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
        bool running();

        static SDL_Renderer * renderer;
        static SDL_Event event;
        static TTF_Font * font;

    private:
        int fontsize;
        bool isRunning;
        int count;
        SDL_Window * window;
        std::string context;

        // SDL_Renderer *renderer;
};



#endif /* GAME_HPP */