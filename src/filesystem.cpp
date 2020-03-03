
#include "filesystem.hpp"
#include "physfs.h"
#include "LodePNG.h"
#include "shared.hpp"
#include <SDL2/SDL.h>
#include <string>
#include <cstring>
#include <stdlib.h>

namespace FILESYSTEM {
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
    printf("Base directory: %s\n", output);

    /* Create save directory */
    strcpy(saveDir, output);
    strcat(saveDir, "saves");
    strcat(saveDir, PHYSFS_getDirSeparator());
    // mkdir(saveDir, 0777);
    printf("Save directory: %s\n", saveDir);

    /* Mount the stock content last */
    if (assetsPath) {
        strcpy(output, assetsPath);
    } else {
        strcpy(output, PHYSFS_getBaseDir());
        strcat(output, "assets.binou");
    }
    if (!PHYSFS_mount(output, NULL, 1))
    {
        printf("Missing assets.binou");
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Missing assets.binou", "Missing assets.binou", NULL);
        return 0;
    }

    // strcpy(output, PHYSFS_getBaseDir());
    // strcpy(output, "gamecontrollerdb.txt");
    // if (SDL_GameControllerAddMappingsFromFile(output) < 0)
    // {
    //     printf("gamecontrollerdb.txt not found!\n");
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

SDL_Surface* LoadImage(const char *filename, bool noBlend = true, bool noAlpha = false)
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
    if (noAlpha)
    {
        lodepng_decode24(&data, &width, &height, fileIn, length);
    }
    else
    {
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

    if (loadedImage != NULL)
    {
        optimizedImage = SDL_ConvertSurfaceFormat(
            loadedImage,
            SDL_PIXELFORMAT_ABGR8888, // FIXME: Format? -flibit
            0
        );
        SDL_FreeSurface( loadedImage );
        free(data);
        if (noBlend)
        {
            SDL_SetSurfaceBlendMode(optimizedImage, SDL_BLENDMODE_BLEND);
        }
        return optimizedImage;
    }
    else
    {
        fprintf(stderr,"Image not found: %s\n", filename);
        SDL_assert(0 && "Image not found! See stderr.");
        return NULL;
    }
}
