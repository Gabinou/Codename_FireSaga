#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP
#include "shared.hpp"
#include "game.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

class TextureManager {

    public:
        static SDL_Texture * loadTexture(const char * filename);
        static SDL_Texture * loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font * in_font = Game::font);
        static void draw(SDL_Texture * texture, SDL_Rect srcrect, SDL_Rect destrect);
};

#endif /* TEXTUREMANAGER_HPP */