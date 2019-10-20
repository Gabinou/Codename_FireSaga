
#include "gameobject.hpp"
#include "texturemanager.hpp"

gameobject::gameobject(const char * texturesheet, SDL_Renderer* ren) {
    
    renderer = ren;
    objTexture = texturemanager::loadtexture(texturesheet, ren);
    
}

void gameobject::update() {
    xpos = 0;
    ypos = 0;
    
    srcRect.h = 32;
    srcRect.w = 32;
    srcRect.x = 0;
    srcRect.y = 0;
    
    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = srcRect.w * 2;
    destRect.h = srcRect.h * 2;
}

void gameobject::render() {
    SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}