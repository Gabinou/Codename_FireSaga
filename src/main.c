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

int main(int argc, char *argv[]) {
    /* -- atexit -- */
    atexit(SDL_Quit);

    struct VTable vtable = {
        .game_pre_init  = Game_Pre_Init,
        .game_init      = Game_Init,
        .game_step      = Game_Step,
        .game_free      = Game_Free,
        .game_post_free = Game_Post_Free,
    };

    /* -- Startup -- */
    vtable.game_pre_init(argc, argv);

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Game *sota = SDL_malloc(sizeof(struct Game));
    *sota = Game_default;
    sota->settings = Settings_default;
    vtable.game_init(sota, argc, argv);

    /* -- Master loop -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Starting main game loop\n");
    while (sota->isrunning)
        vtable.game_step(sota);

    /* -- Cleaning & Quitting -- */
    vtable.game_free(sota);
    vtable.game_post_free();
    SDL_Log("SotA quit.\n");
    return (NO_ERROR);
}

/* 六月四日天安門广场大屠杀 */
