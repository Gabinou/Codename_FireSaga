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
*  Source code is SDL_free to own, modify and compile
*  Game is SDL_free to play only if you can compile it
*  Please do not distribute compiled executables
*/

#include "game/game.h"
#include "SDL2/SDL.h"
#include <dlfcn.h>

// hot reload TODO TODO:
// 1. Check if .so file changed every x frames? y seconds?.
// 2. If changed,
//  1. Load so object:
//      void * so_handle = SDL_LoadObject(const char *sofile);
//  2. Load function from so object:
//      void *SDL_LoadFunction(void *so_handle, const char *name);

struct VTable {
    void (*IES_Init)(int argc, char *argv[]);
    void (*game_init)(struct Game *sota, int argc, char *argv[]);
    void (*game_step)(struct Game *sota);
    void (*game_free)(struct Game *sota);
    void (*game_post_free)(void);
};

#define RELOAD_FRAMES 60
static const char *sota_dll = "libsota_dll.so";

void *so_handle = NULL;

void VTable_Load(struct VTable *vtable) {
    if (so_handle != NULL)
        SDL_UnloadObject(so_handle);

    so_handle = SDL_LoadObject(sota_dll);
    // so_handle = dlopen(sota_dll, RTLD_NOW);

    if (so_handle == NULL) {
        printf("HOTRELOAD: could not load %s: %s\n", sota_dll, dlerror());
        exit(1);
    }
    vtable->IES_Init   = SDL_LoadFunction(so_handle, "IES_Init");
    vtable->game_init       = SDL_LoadFunction(so_handle, "Game_Init");
    vtable->game_step       = SDL_LoadFunction(so_handle, "Game_Step");
    vtable->game_free       = SDL_LoadFunction(so_handle, "Game_Free");
    vtable->game_post_free  = SDL_LoadFunction(so_handle, "Game_Post_Free");
    if (vtable->IES_Init == NULL) {
        printf("HOTRELOAD: could not load %s: %s\n", "IES_Init", dlerror());
        exit(1);
    }

    if (vtable->game_init == NULL) {
        printf("HOTRELOAD: could not load %s: %s\n", "Game_Init", dlerror());
        exit(1);
    }

    if (vtable->game_step == NULL) {
        printf("HOTRELOAD: could not load %s: %s\n", "Game_Step", dlerror());
        exit(1);
    }

    if (vtable->game_free == NULL) {
        printf("HOTRELOAD: could not load %s: %s\n", "Game_Free", dlerror());
        exit(1);
    }

    if (vtable->game_post_free == NULL) {
        printf("HOTRELOAD: could not load %s: %s\n", "Game_Post_Free", dlerror());
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    /* -- atexit -> Quit SDL -- */
    atexit(SDL_Quit);

    /* -- Initialize vtable with first loaded functions -- */
    struct VTable vtable = {0};
    VTable_Load(&vtable);

    /* -- Startup -- */
    vtable.IES_Init(argc, argv);

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Game *sota = SDL_malloc(sizeof(struct Game));
    vtable.Game *sota = Game_New(settings);

    /* -- Master loop -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Starting main game loop\n");
    u64 i = 1;
    while (sota->isrunning) {
        // if (i++ % 120 == 0) {
        VTable_Load(&vtable);
        // }
        vtable.game_step(sota);
    }

    /* -- Cleaning & Quitting -- */
    vtable.game_free(sota);
    vtable.game_post_free();
    SDL_Log("SotA quit.\n");
    return (NO_ERROR);
}

/* 六月四日天安門广场大屠杀 */
