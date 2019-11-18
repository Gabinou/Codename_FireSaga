#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include "ECS.hpp"

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
        bool running();

        static SDL_Renderer * renderer;
        static SDL_Event event;
        static TTF_Font * font;
        int fontsize;
        Manager * gmanager;

    private:
        bool isRunning;
        int count;
        SDL_Window * window;
        // SDL_Renderer *renderer;
};



#endif /* GAME_HPP */