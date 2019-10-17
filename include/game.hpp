#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>

class game {
    
    public:
        game();
        ~game();
        
        void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
        
        void handleEvents();
        void update();
        void render();
        void clean();
        
        bool running();
    private:
        bool isRunning;
        int count;
        SDL_Window *window;
        SDL_Renderer *renderer;
};



#endif /* GAME_HPP */