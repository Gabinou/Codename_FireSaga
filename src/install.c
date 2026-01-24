
#include "physfs.h"
#include "names/game.h"

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

int main(int argc, char *argv[]) {
    printf("Installing " STRINGIFY(GAME_NAME));
    /* -- 1- Read org, app name from central location -- */
    
    /* -- 2- get prefdir -- */
    // const char * PHYSFS_getPrefDir  (   const char *    org,
    //     const char *    app 
    // )

    /* -- 3- Copy exe to prefdir -- */
    /* -- 4- Copy data.bsa to prefdir -- */
    /* -- 5- make saves dir -- */

    return (0);
}
