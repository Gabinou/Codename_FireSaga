#include "texturemanager.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

SDL_Texture* TextureManager::LoadTexture(const char * texture) {
    SDL_Surface* tempsurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempsurface);
    SDL_FreeSurface(tempsurface);
    return(tex);
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest){
    
    SDL_RenderCopy(Game::renderer, tex, &src, &dest);
    
}
    
