#include "texturemanager.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

SDL_Texture* texturemanager::loadtexture(const char * texture, SDL_Renderer * ren) {
    SDL_Surface* tempsurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempsurface);
    SDL_FreeSurface(tempsurface);
    return(tex);
}
