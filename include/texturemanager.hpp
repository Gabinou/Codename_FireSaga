#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP
// #pragma once

#include "shared.hpp"
#include "game.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"



class texturemanager{
    
public:
    static SDL_Texture* loadtexture(const char * filename);
    static void draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
    
};


#endif /* TEXTUREMANAGER_HPP */