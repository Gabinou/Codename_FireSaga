#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "game.hpp"


class gameobject {
    
    public:
        gameobject(const char* texturesheet, SDL_Renderer* ren, int x, int y);
        ~gameobject();
        
        void update();
        void render();
        
    private:
        short int xpos;
        short int ypos;
        
        SDL_Texture* objTexture;
        SDL_Rect srcRect, destRect;
        SDL_Renderer* renderer;
    

};


#endif /* GAMEOBJECT_HPP */