#ifndef MAP_HPP
#define MAP_HPP

#include "game.hpp"

class map {
    
    public:
        map();
        ~map();
    
        void loadmap();
        void drawmap();
    
    private:
    
        SDL_rect src, dest;
        
        SDL_Texture* dirt;    
        SDL_Texture* grass;    
        SDL_Texture* water;    

        int map[20][25];


}


#endif /* MAP_HPP */