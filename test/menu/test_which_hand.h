
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
    Game *sota                  = Game_New(settings);
    SDL_assert(gl_world != NULL);

    /* -- dtab load -- */
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab,   struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab,     struct Item);

    Item_Load(ITEM_ID_GLADIUS);
    Unit Silou  = Unit_default;
    Item item   = Item_default;

    /* -- WHM creation -- */
    struct WhichHandMenu whm = WhichHandMenu_default;
    struct n9Patch n9patch = n9Patch_default;
    WhichHandMenu_Load(&whm, renderer, &n9patch);
    SDL_Texture *render_target = NULL;
    whm.unit = &Silou;

    /* --- Silou hands render --- */
    Item_Handedness_Set(&item, WEAPON_HAND_ANY);

    /* -- Right handed -- */
    Unit_Handedness_set(&Silou, UNIT_HAND_RIGHTIE);

    /* - RH only - */
    Unit_Hand_Set(&Silou, UNIT_HAND_LEFT,   0);
    Unit_Hand_Set(&Silou, UNIT_HAND_RIGHT,  1);

    _WhichHandMenu_Elements(&whm, &n9patch, &Silou, &item);
    WhichHandMenu_Update(&whm, &n9patch, render_target, renderer);

    Filesystem_Texture_Dump(PATH_JOIN("menu_which_hand",
                                      "WHM_Rightie_RH.png"),
                            renderer,
                            whm.texture,
                            SDL_PIXELFORMAT_ARGB8888,
                            render_target);


    /* - LH only - */
    Unit_Hand_Set(&Silou, UNIT_HAND_LEFT,   1);
    Unit_Hand_Set(&Silou, UNIT_HAND_RIGHT,  0);

    _WhichHandMenu_Elements(&whm, &n9patch, &Silou, &item);
    WhichHandMenu_Update(&whm, &n9patch, render_target, renderer);

    Filesystem_Texture_Dump(PATH_JOIN("menu_which_hand",
                                      "WHM_Rightie_LH.png"),
                            renderer,
                            whm.texture,
                            SDL_PIXELFORMAT_ARGB8888,
                            render_target);


    /* - 2H - */
    Unit_Hand_Set(&Silou, UNIT_HAND_LEFT,   1);
    Unit_Hand_Set(&Silou, UNIT_HAND_RIGHT,  1);

    _WhichHandMenu_Elements(&whm, &n9patch, &Silou, &item);
    WhichHandMenu_Update(&whm, &n9patch, render_target, renderer);

    Filesystem_Texture_Dump(PATH_JOIN("menu_which_hand",
                                      "WHM_Rightie_AH.png"),
                            renderer,
                            whm.texture,
                            SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Left handed -- */
    Unit_Handedness_set(&Silou, UNIT_HAND_LEFTIE);

    /* - RH only - */
    Unit_Hand_Set(&Silou, UNIT_HAND_LEFT,   0);
    Unit_Hand_Set(&Silou, UNIT_HAND_RIGHT,  1);

    _WhichHandMenu_Elements(&whm, &n9patch, &Silou, &item);
    WhichHandMenu_Update(&whm, &n9patch, render_target, renderer);

    Filesystem_Texture_Dump(PATH_JOIN("menu_which_hand",
                                      "WHM_Leftie_RH.png"),
                            renderer,
                            whm.texture,
                            SDL_PIXELFORMAT_ARGB8888,
                            render_target);


    /* - LH only - */
    Unit_Hand_Set(&Silou, UNIT_HAND_LEFT,   1);
    Unit_Hand_Set(&Silou, UNIT_HAND_RIGHT,  0);

    _WhichHandMenu_Elements(&whm, &n9patch, &Silou, &item);
    WhichHandMenu_Update(&whm, &n9patch, render_target, renderer);

    Filesystem_Texture_Dump(PATH_JOIN("menu_which_hand",
                                      "WHM_Leftie_LH.png"),
                            renderer,
                            whm.texture,
                            SDL_PIXELFORMAT_ARGB8888,
                            render_target);


    /* - 2H - */
    Unit_Hand_Set(&Silou, UNIT_HAND_LEFT,   1);
    Unit_Hand_Set(&Silou, UNIT_HAND_RIGHT,  1);

    _WhichHandMenu_Elements(&whm, &n9patch, &Silou, &item);
    WhichHandMenu_Update(&whm, &n9patch, render_target, renderer);

    Filesystem_Texture_Dump(PATH_JOIN("menu_which_hand",
                                      "WHM_Leftie_AH.png"),
                            renderer,
                            whm.texture,
                            SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Ambidextrous -- */
    Unit_Handedness_set(&Silou, UNIT_HAND_AMBIDEXTROUS);

    /* - RH only - */
    Unit_Hand_Set(&Silou, UNIT_HAND_LEFT,   0);
    Unit_Hand_Set(&Silou, UNIT_HAND_RIGHT,  1);

    _WhichHandMenu_Elements(&whm, &n9patch, &Silou, &item);
    WhichHandMenu_Update(&whm, &n9patch, render_target, renderer);

    Filesystem_Texture_Dump(PATH_JOIN("menu_which_hand",
                                      "WHM_Ambidex_RH.png"),
                            renderer,
                            whm.texture,
                            SDL_PIXELFORMAT_ARGB8888,
                            render_target);


    /* - LH only - */
    Unit_Hand_Set(&Silou, UNIT_HAND_LEFT,   1);
    Unit_Hand_Set(&Silou, UNIT_HAND_RIGHT,  0);

    _WhichHandMenu_Elements(&whm, &n9patch, &Silou, &item);
    WhichHandMenu_Update(&whm, &n9patch, render_target, renderer);

    Filesystem_Texture_Dump(PATH_JOIN("menu_which_hand",
                                      "WHM_Ambidex_LH.png"),
                            renderer,
                            whm.texture,
                            SDL_PIXELFORMAT_ARGB8888,
                            render_target);


    /* - 2H - */
    Unit_Hand_Set(&Silou, UNIT_HAND_LEFT,   1);
    Unit_Hand_Set(&Silou, UNIT_HAND_RIGHT,  1);

    _WhichHandMenu_Elements(&whm, &n9patch, &Silou, &item);
    WhichHandMenu_Update(&whm, &n9patch, render_target, renderer);

    Filesystem_Texture_Dump(PATH_JOIN("menu_which_hand",
                                      "WHM_Ambidex_AH.png"),
                            renderer,
                            whm.texture,
                            SDL_PIXELFORMAT_ARGB8888,
                            render_target);


    /* --- Item handedness render --- */
    Unit_Handedness_set(&Silou, UNIT_HAND_AMBIDEXTROUS);

    /* - 1H only - */
    Item_Handedness_Set(&item, WEAPON_HAND_ONE);
    _WhichHandMenu_Elements(&whm, &n9patch, &Silou, &item);
    WhichHandMenu_Update(&whm, &n9patch, render_target, renderer);

    Filesystem_Texture_Dump(PATH_JOIN("menu_which_hand",
                                      "WHM_Item_1H.png"),
                            renderer,
                            whm.texture,
                            SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* - 2H only - */
    Item_Handedness_Set(&item, WEAPON_HAND_TWO);
    _WhichHandMenu_Elements(&whm, &n9patch, &Silou, &item);
    WhichHandMenu_Update(&whm, &n9patch, render_target, renderer);

    Filesystem_Texture_Dump(PATH_JOIN("menu_which_hand",
                                      "WHM_Item_2H.png"),
                            renderer,
                            whm.texture,
                            SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* - RH only - */
    Item_Handedness_Set(&item, WEAPON_HAND_RIGHT);
    _WhichHandMenu_Elements(&whm, &n9patch, &Silou, &item);
    WhichHandMenu_Update(&whm, &n9patch, render_target, renderer);

    Filesystem_Texture_Dump(PATH_JOIN("menu_which_hand",
                                      "WHM_Item_RH.png"),
                            renderer,
                            whm.texture,
                            SDL_PIXELFORMAT_ARGB8888,
                            render_target);
    /* - LH only - */
    Item_Handedness_Set(&item, WEAPON_HAND_LEFT);
    _WhichHandMenu_Elements(&whm, &n9patch, &Silou, &item);
    WhichHandMenu_Update(&whm, &n9patch, render_target, renderer);

    Filesystem_Texture_Dump(PATH_JOIN("menu_which_hand",
                                      "WHM_Item_LH.png"),
                            renderer,
                            whm.texture,
                            SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* --- Free --- */
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    SDL_DestroyRenderer(renderer);
}
