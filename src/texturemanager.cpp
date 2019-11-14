#include "texturemanager.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

SDL_Texture * TextureManager::loadTexture(const char * filename) {
    SDL_Surface * tempsurface = IMG_Load(filename);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(Game::renderer, tempsurface);
    SDL_FreeSurface(tempsurface);
    return (texture);
}

SDL_Texture * TextureManager::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font * in_font) {
    SDL_Surface * textsurface = TTF_RenderText_Solid(in_font, textureText.c_str(), textColor);
    SDL_Texture * texture;

    if (textsurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        //Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface(Game::renderer, textsurface);

        if (texture == NULL) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }

        //Get rid of old surface
        SDL_FreeSurface(textsurface);
    }

    return (texture);
}


void TextureManager::draw(SDL_Texture * texture, SDL_Rect srcrect, SDL_Rect destrect) {
    SDL_RenderCopy(Game::renderer, texture, &srcrect, &destrect);
}

