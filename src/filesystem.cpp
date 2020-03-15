
#include "filesystem.hpp"
#include "game.hpp"


#if defined(_WIN32)
#include <windows.h>
int mkdir(char* path, int mode)
{
    WCHAR utf16_path[DEFAULT::MAXPATH];
    MultiByteToWideChar(CP_UTF8, 0, path, -1, utf16_path, DEFAULT::MAXPATH);
    return CreateDirectoryW(utf16_path, NULL);
}
#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__HAIKU__)
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <dirent.h>
#endif


namespace FILESYSTEM {

void log(void* userdata, int category, SDL_LogPriority priority, const char* message) {
    FILE * logf = fopen(LOGFILE, "a");
    fprintf(logf, message);
    fprintf(logf, "\n");
    fclose(logf);
    printf(message);
    printf("\n");
}

int init(char *argvZero, char* baseDir, char *assetsPath) {
    char output[DEFAULT::MAXPATH];
    int mkdirResult;
    const char * pathSep = PHYSFS_getDirSeparator();
    char saveDir[DEFAULT::MAXPATH] = "save";

    PHYSFS_init(argvZero);
    PHYSFS_permitSymbolicLinks(1);

    /* Determine the OS user directory */
    if (baseDir && strlen(baseDir) > 0)
    {
        strcpy(output, baseDir);

        /* We later append to this path and assume it ends in a slash */
        if (strcmp(std::string(1, output[strlen(output) - 1]).c_str(), pathSep) != 0)
        {
            strcat(output, pathSep);
        }
    }

    /* Mount our base user directory */
    PHYSFS_mount(output, NULL, 1);
    PHYSFS_setWriteDir(output);
    SDL_Log("Base directory: %s\n", output);

    /* Create save directory */
    strcpy(saveDir, output);
    strcat(saveDir, "saves");
    strcat(saveDir, PHYSFS_getDirSeparator());
    // mkdir(saveDir, 0777);
    SDL_Log("Save directory: %s\n", saveDir);

    /* Mount the stock content last */
    if (assetsPath) {
        strcpy(output, assetsPath);
        strcat(output, "\\assets.binou");
    } else {
        strcpy(output, PHYSFS_getBaseDir());
        strcat(output, "\\assets.binou");
    }
    SDL_Log("Path to assets: %s\n", output);
    if (!PHYSFS_mount(output, NULL, 1))
    {
        SDL_Log("Missing assets.binou\n");
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing assets.binou", "Missing assets.binou", NULL);
        return 0;
    }

    strcpy(output, baseDir);
    strcat(output, "\\gamecontrollerdb.txt");
    SDL_Log("Path to gamecontrollerdb: %s\n", output);
    // if (SDL_GameControllerAddMappingsFromFile(output) < 0)
    // {
    //     SDL_Log("gamecontrollerdb.txt not found!\n");
    // }
    return 1;
}

void loadFileToMemory(const char *name, unsigned char **mem, size_t *len, bool addnull) {
    PHYSFS_File *physfs_file = PHYSFS_openRead(name);
    if (physfs_file == NULL)
    {
        return;
    }
    PHYSFS_uint32 length = PHYSFS_fileLength(physfs_file);
    if (len != NULL)
    {
        *len = length;
    }
    if (addnull)
    {
        *mem = (unsigned char *) malloc(length + 1);
        (*mem)[length] = 0;
    }
    else
    {
        *mem = (unsigned char*) malloc(length);
    }
    PHYSFS_readBytes(physfs_file, *mem, length);
    PHYSFS_close(physfs_file);
}


void deinit() {
    PHYSFS_deinit();
}

void freeMemory(unsigned char **mem)
{
    free(*mem);
    *mem = NULL;
}
}

SDL_Surface * ZIP_loadSurface(const char *filename, bool noBlend = true, bool noAlpha = false)
{
    //Temporary storage for the image that's loaded
    SDL_Surface* loadedImage = NULL;
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    unsigned char *data;
    unsigned int width, height;

    unsigned char *fileIn = NULL;
    size_t length = 0;
    FILESYSTEM::loadFileToMemory(filename, &fileIn, &length);
    if (noAlpha) {
        lodepng_decode24(&data, &width, &height, fileIn, length);
    } else {
        lodepng_decode32(&data, &width, &height, fileIn, length);
    }
    FILESYSTEM::freeMemory(&fileIn);

    loadedImage = SDL_CreateRGBSurfaceFrom(
        data,
        width,
        height,
        noAlpha ? 24 : 32,
        width * (noAlpha ? 3 : 4),
        0x000000FF,
        0x0000FF00,
        0x00FF0000,
        noAlpha ? 0x00000000 : 0xFF000000
    );

    if (loadedImage != NULL) {
        optimizedImage = SDL_ConvertSurfaceFormat(
            loadedImage,
            SDL_PIXELFORMAT_ABGR8888,
            0
        );
        SDL_FreeSurface(loadedImage);
        free(data);
        if (noBlend) {
            SDL_SetSurfaceBlendMode(optimizedImage, SDL_BLENDMODE_BLEND);
        }
        return optimizedImage;
    } else {
        SDL_Log("Image not found: %s\n", filename);
        return NULL;
    }
}

SDL_Texture * loadTexture(SDL_Renderer * in_renderer, const char * filename) {
    SDL_Surface * tempsurface;
    if (PHYSFS_openRead(filename) != NULL) {
        SDL_Log("ZIP_loadSurface works?");
        tempsurface = ZIP_loadSurface(filename); // How fast is this?
    } else {
        SDL_Log("NOT ZIP_loadSurface");
        tempsurface = IMG_Load(filename); // Not that fast.
    }
    if (tempsurface == NULL) {
        SDL_Log("loadTexture. IMG_Load: %s\n", IMG_GetError());
    }
    SDL_Texture * texture = SDL_CreateTextureFromSurface(in_renderer, tempsurface); // THIS FUNCTION CRASHES.
    if (texture == NULL) {
       SDL_Log("CreateTextureFromSurface failed: %s\n", SDL_GetError()); 
    }
    return (texture);
}

SDL_Texture * textToTexture(SDL_Renderer * in_renderer, std::string textureText, SDL_Color textColor, TTF_Font * in_font) {
    SDL_Surface * textsurface = TTF_RenderText_Blended(in_font, textureText.c_str(), textColor); // Blended is better than Solid
    // TTF_RenderText_Shaded shades the whole destrect background.
    
    SDL_Texture * texture;

    if (textsurface == NULL) {
        SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        //Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface(in_renderer, textsurface);

        if (texture == NULL) {
            SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(textsurface); //Get rid of old surface
    }
    return (texture);
}

void writeText(SDL_Renderer * in_renderer, int in_fontsize, int in_position[2], float in_sizefactor[2], std::string in_text, SDL_Color in_color, TTF_Font * in_font) {
    std::string text = "FPS";
    SDL_Texture * texture = textToTexture(in_renderer, text, in_color, in_font);
    SDL_Rect srcrect, destrect; // background always first?
    srcrect.x = srcrect.y = 0;
    destrect.x = in_position[0];
    destrect.y = in_position[1];
    srcrect.h = in_fontsize;
    srcrect.w = text.length() * in_fontsize;
    destrect.h = (int)in_fontsize * in_sizefactor[0];
    destrect.w = (int)text.length() * in_fontsize * in_sizefactor[1];
    SDL_RenderCopy(in_renderer, texture, &srcrect, &destrect);
    SDL_RenderPresent(in_renderer);
}
