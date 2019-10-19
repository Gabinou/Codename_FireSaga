#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP
// #pragma once

#include "shared.hpp"
#include "game.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"



class texturemanager{
    
public:
    static SDL_Texture* loadtexture(const char * filename, SDL_Renderer* ren);
    
};


#endif /* TEXTUREMANAGER_HPP */