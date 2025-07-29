
#include "nourstest.h"
#include "menu/which_hand.h"

void test_menu_which_hand(void) {
    /* --- Preliminaries --- */
    sota_mkdir("menu_which_hand");

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    /* -- Game -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Settings settings    = Settings_default;
    settings.window             = SDL_WINDOW_HIDDEN;
    sota                        = Game_New(settings);
    SDL_assert(gl_world != NULL);

    /* -- dtab load -- */
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab,   struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab,     struct Item);

    Weapon_Load(gl_weapons_dtab, ITEM_ID_FLEURET);
    Unit Silou  = Unit_default;
    Item item   = Item_default;

    /* -- WHM creation -- */
    struct WhichHandMenu whm = WhichHandMenu_default;
    struct n9Patch n9patch = n9Patch_default;
    WhichHandMenu_Load(&whm, renderer, &n9patch);
    SDL_Texture *render_target = NULL;

    /* --- Silou hands render --- */
    Item_Handedness_Set(&item, WEAPON_HAND_ANY);

    /* -- RH only -- */
    Unit_Hand_Set(&Silou, UNIT_HAND_LEFT,   0);
    Unit_Hand_Set(&Silou, UNIT_HAND_RIGHT,  1);

    _WhichHandMenu_Elements(&whm, &Silou, &item);

    WhichHandMenu_Update(&whm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_which_hand", "WHM_NoLH.png"),
                            renderer,
                            whm.texture,
                            SDL_PIXELFORMAT_ARGB8888,
                            render_target);


    /* -- LH only -- */
    Unit_Hand_Set(&Silou, UNIT_HAND_LEFT,   1);
    Unit_Hand_Set(&Silou, UNIT_HAND_RIGHT,  0);

    /* -- 2H -- */
    Unit_Hand_Set(&Silou, UNIT_HAND_LEFT,   1);
    Unit_Hand_Set(&Silou, UNIT_HAND_RIGHT,  1);

    /* --- Item handedness render --- */
    Item_Handedness_Set(&item, WEAPON_HAND_ANY);

    /* --- Free --- */
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    SDL_DestroyRenderer(renderer);
}
