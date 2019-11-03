#include "texturemanager.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

SDL_Texture* TextureManager::loadTexture(const char * filename) {
    SDL_Surface* tempsurface = IMG_Load(filename);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tempsurface);
    SDL_FreeSurface(tempsurface);
    return(texture);
}

void TextureManager::draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest){
    
    SDL_RenderCopy(Game::renderer, texture, &src, &dest);
    
}
    
