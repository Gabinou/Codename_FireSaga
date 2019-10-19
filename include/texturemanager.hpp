#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include "game.hpp"



class texturemanager{
    
public:
    static SDL_Texture* loadtexture(const char * filename, SDL_Renderer* ren);
    
}


#endif /* TEXTUREMANAGER_HPP */