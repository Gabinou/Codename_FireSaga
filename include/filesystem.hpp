
#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <stdio.h>
#include "physfs.h"
#include "LodePNG.h"
#include "shared.hpp"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include <SDL2/SDL.h>
#include <string>
#include <cstring>
#include <stdlib.h>

namespace FILESYSTEM {
void loadFileToMemory(const char * name, unsigned char ** mem,
                      size_t * len, bool addnull = false);
}

extern SDL_Texture * loadTexture(SDL_Renderer * renderer, const char * filename);
extern SDL_Texture * textToTexture(SDL_Renderer * renderer, std::string textureText, SDL_Color textColor, TTF_Font * in_font);

#endif /* FILESYSTEM_HPP */