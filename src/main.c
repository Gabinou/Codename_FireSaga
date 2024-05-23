/*--------------------------------------------------------------------
* |           Average Bear Games                                     |
* |        _         _                    Available on....           |
* |       ( \_.---._/ )                                              |
* |        \  .   .  /       ,''',                                   |
* |         |  (@)  |       (*****)                                  |
* |         \   ⋏   /        \   /                                   |
* |          '-._.-'         |   |                                   |
* |        Made by Gabriel Taillon                                   |
* --------------------------------------------------------------------*/
/* Imperial Eagle Bearer */
/* Turn-based strategy Role Playing Game (RPG)
*  Strongly inspired by Fire Emblem and Tear Ring Saga
*  Source code is free to own, modify and compile
*  Game is free to play only if you can compile it
*  Please do not distribute compiled executables
*/

#include "game/game.h"

// hot reload TODO TODO:
// 1. Check if .so file changed every x frames? y seconds?.
// 2. If changed,
//  1. Load so object:
//      void * so_handle = SDL_LoadObject(const char *sofile);
//  2. Load function from so object:
//      void *SDL_LoadFunction(void *so_handle, const char *name);

struct VTable {
    void (*game_pre_init)(int argc, char *argv[]);
    void (*game_init)(struct Game *sota, int argc, char *argv[]);
    void (*game_step)(struct Game *sota);
    void (*game_free)(struct Game *sota);
    void (*game_post_free)(void);
};

#define SOTA_DLL "/home/gabinours/firesaga/build/libsota_dll.so"
#define RELOAD_FRAMES 60

void VTable_Load(struct VTable *vtable) {
    void *so_handle         = SDL_LoadObject(SOTA_DLL);
    SDL_assert(so_handle != NULL);
    vtable->game_pre_init   = SDL_LoadFunction(so_handle, "Game_Pre_Init");
    vtable->game_init       = SDL_LoadFunction(so_handle, "Game_Init");
    vtable->game_step       = SDL_LoadFunction(so_handle, "Game_Step");
    vtable->game_free       = SDL_LoadFunction(so_handle, "Game_Free");
    vtable->game_post_free  = SDL_LoadFunction(so_handle, "Game_Post_Free");
    SDL_assert(vtable->game_pre_init    != NULL);
    SDL_assert(vtable->game_init        != NULL);
    SDL_assert(vtable->game_step        != NULL);
    SDL_assert(vtable->game_free        != NULL);
    SDL_assert(vtable->game_post_free   != NULL);
    SDL_UnloadObject(so_handle);
}

extern struct Game Game_default;

int main(int argc, char *argv[]) {
    /* -- atexit -> Quit SDL -- */
    atexit(SDL_Quit);

    /* -- Initialize vtable with first loaded functions -- */
    struct VTable vtable = {};
    VTable_Load(&vtable);

    /* -- Startup -- */
    vtable.game_pre_init(argc, argv);

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Game *sota = SDL_malloc(sizeof(struct Game));
    vtable.game_init(sota, argc, argv);

    /* -- Master loop -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Starting main game loop\n");
    while (sota->isrunning) {
        VTable_Load(&vtable);
        vtable.game_step(sota);
    }

    /* -- Cleaning & Quitting -- */
    vtable.game_free(sota);
    vtable.game_post_free();
    SDL_Log("SotA quit.\n");
    return (NO_ERROR);
}

/* 六月四日天安門广场大屠杀 */
