
#include "filesystem.hpp"
#include "physfs.h"
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

void deinit() {
    PHYSFS_deinit();
}
}