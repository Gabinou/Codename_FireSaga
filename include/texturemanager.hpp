#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP
#include "shared.hpp"
#include "game.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

class TextureManager{
    
public:
    static SDL_Texture* loadTexture(const char * filename);
    static void draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest);    
};

#endif /* TEXTUREMANAGER_HPP */