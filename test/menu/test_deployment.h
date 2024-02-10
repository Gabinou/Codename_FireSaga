#include "nourstest.h"
#include "platform.h"
#include "menu/deployment.h"
#include "unit/unit.h"
#include "RNG.h"

void test_menu_deployment() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    /* --- Preliminaries --- */
    sota_mkdir("menu_deployment");

}