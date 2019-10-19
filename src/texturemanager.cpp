#include "texturemanager.hpp"

SDL_Texture* texturemanager::loadtexture(const char * texture, SDL_Renderer ren) {
    SDL_Surface* tempsurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempsurface);
    SDL_FreeSurface(tempssurface);
    return(tex);
}
