#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP
#include "shared.hpp"
#include "game.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

class TextureManager{
    
public:
    static SDL_Texture* LoadTexture(const char * filename);
    static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);    
};

#endif /* TEXTUREMANAGER_HPP */