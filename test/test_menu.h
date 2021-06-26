#include "minctest.h"
#include "menu.h"

void test_menu() {
    SDL_Log("Testing menu");

    struct Menu * test_menu = (struct Menu *)malloc(sizeof(struct Menu));
    *test_menu = Menu_default;

}