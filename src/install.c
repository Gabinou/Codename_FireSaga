
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "physfs.h"

#include "nstr.h"
#include "names/game.h"
#include "names/zip_archive.h"

// void cmd_post_cpy_bsa(Target *target) {
//     /* --- Copy archive to install dir --- */

//     /* -- reading assets archive name -- */
//     /* Note: Can't comptime this include AFAIK */
// #define REGISTER_ENUM(x) char* archive = #x;
// #include "names/zip_archive.h"
// #undef REGISTER_ENUM

//     /* Command:
//     **  1. cp -u <archive> <BUILD_DIR>/<archive> */
//     /* TODO:
//     **  decide where archive BSA_DIR,
//     **  Then update command:
//     **  1. cp -u <archive> <BSA_DIR>/<archive>
//     **  2. ln -s <BSA_DIR> <BUILD_DIR>/<BSA_DIR> */
//     // Note: command_2. is only for development

//     char *commands = calloc(1, 256);
//     char *command_1_1 = "cp -u ";
//     strncat(commands, command_1_1, strlen(command_1_1));
//     strncat(commands, archive,     strlen(archive));
//     char *command_1_2 = " "BUILD_DIR"/";
//     strncat(commands, command_1_2, strlen(command_1_2));
//     strncat(commands, archive,     strlen(archive));
//     target->cmd_post = commands;
// }

// void cmd_post_install(Target *target) {
//     /* --- install game (for phony target) --- */

//     /* -- reading assets archive name -- */
//     /* Note: Can't comptime this include AFAIK */
// #define REGISTER_ENUM(x) char* archive = #x;
// #include "names/zip_archive.h"
// #undef REGISTER_ENUM
//     /* Commands:
//     **  1. Copy exe to install dir
//     **      cp -u build/<EXE_NAME> <INSTALL_DIR>/<EXE_NAME>
//     **  2. Copy archive to install dir
//     **      cp -u <archive> <INSTALL_DIR>/<ar>
//     **  3. Make saves dir
//     **      cp -u <archive> <INSTALL_DIR>/<ar>
//     */ 

//     char *commands = calloc(1, 256);
//     char *command_1 = "mkdir --parents " INSTALL_DIR " && cp -u ";
//     strncat(commands, command_1, strlen(command_1));
//     strncat(commands, archive,   strlen(archive));
//     strncat(commands, " ", 2);
//     char *command_2 = INSTALL_DIR "/";
//     strncat(commands, command_2, strlen(command_2));
//     strncat(commands, archive,   strlen(archive));
//     char *command_3 = " && cp -u build/" STRINGIFY(EXE_NAME) " " INSTALL_DIR "/" STRINGIFY(EXE_NAME);
//     strncat(commands, command_3, strlen(command_3));
//     target->cmd_pre = commands;
// }

#define STRINGIFY(x) #x
#define STRINGIZE(x) STRINGIFY(x)

void Searchpath(void) {
    /* -- Debug: printing search path -- */
    char **i;
    for (i = PHYSFS_getSearchPath(); *i != NULL; i++) {
        printf("[%s] is in the search path.\n", *i);
    }
}


int main(int argc, char *argv[]) {
    s8 saves_dir = {0};
    const char *app;
    const char *org;
    const char *sep;
    const char *prefdir;
    const char *writedir;
    /* -- 0- physfs init -- */
    if (PHYSFS_init(argv[0]) <= 0) {
        printf("Could not initialize PhysFS \n");
        exit(1);
    }
    PHYSFS_setSaneConfig(   STRINGIZE(GAME_COMPANY),   
                            STRINGIZE(GAME_TITLE_ABREV),
                            NULL,  0, 0);

    /* -- 1- Read org, app name from central location -- */
    app = STRINGIZE(GAME_TITLE_ABREV);
    org = STRINGIZE(GAME_COMPANY);
    printf("Installing %s \n", app);

    /* -- 2- get prefdir -- */
    prefdir = PHYSFS_getPrefDir(org, app);
    printf("prefdir '%s' \n", prefdir);
    if (NULL == PHYSFS_setWriteDir(prefdir)) {
        printf("Could not set write dir '%s' \n", prefdir);
    }
    /* -- 3- Copy exe to prefdir -- */

    /* -- 4- Copy data.bsa to prefdir -- */

    /* -- 5- make saves dir -- */
    if (0 == PHYSFS_mkdir("saves")) {
        printf("Could not mkdir '%s' \n", "saves");
    }

    return (0);
}
