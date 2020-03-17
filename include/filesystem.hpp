
#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <stdio.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include "physfs.h"
#include "LodePNG.h"
#include "utilities.hpp"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include "game.hpp"

#define ASSET_LIB "assets.binou"
#define LOGFILE "log.txt"

namespace FILESYSTEM {
int init(char * argvZero, char * baseDir, char * assetsPath);
void loadFileToMemory(const char * name, unsigned char ** mem,
                      size_t * len, bool addnull = false);
void log(void * userdata, int category, SDL_LogPriority priority, const char * message);
}
char * getlineFS();
extern SDL_Texture * loadTexture(SDL_Renderer * in_renderer, const char * filename);
extern SDL_Texture * textToTexture(SDL_Renderer * in_renderer, std::string textureText, SDL_Color textColor, TTF_Font * in_font);
extern void writeText(int in_fontsize, int in_position[2], float in_sizefactor[2], std::string in_text, SDL_Color in_color, TTF_Font * in_font, SDL_Renderer * in_renderer);

#endif /* FILESYSTEM_HPP */