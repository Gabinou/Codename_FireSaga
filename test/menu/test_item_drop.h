#include "nourstest.h"
#include "menu/item_drop.h"

void test_menu_item_drop() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    /* -- Preliminaries -- */
    sota_mkdir("menu_item_drop");

    /* -- Weapon dtab -- */
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);
    struct dtab *items_dtab   = DTAB_INIT(items_dtab,   struct Item);

    /* -- Create n9patch -- */
    struct n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;

    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    /* -- Create LoadoutSelectMenu -- */
    struct ItemDropMenu *idm = ItemDropMenu_Alloc();
    ItemDropMenu_Load(idm, renderer, &n9patch);

    /* - loading fonts - */
    idm->pixelnours     = PixelFont_Alloc();
    idm->pixelnours_big = PixelFont_Alloc();
    PixelFont_Load(idm->pixelnours,     renderer, PATH_JOIN("..", "assets", "Fonts", "pixelnours.png"));
    PixelFont_Load(idm->pixelnours_big, renderer, PATH_JOIN("..", "assets", "Fonts",
                                                            "pixelnours_Big.png"));
    SDL_assert(idm->pixelnours);
    SDL_assert(idm->pixelnours_big);

    /* -- Create Unit -- */
    struct Unit Silou = Unit_default;
    Unit_Init(&Silou);
    idm->unit = &Silou;

    /* - title - */
    Silou.weapons_dtab = weapons_dtab;
    Silou.items_dtab   = items_dtab;
    SDL_assert(Silou.num_equipment == 0);
    jsonio_readJSON(PATH_JOIN("units", "Silou_test.json"), &Silou);
    SDL_assert(Silou.num_equipment == 4);

    /* - Unit equip - */
    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id   = ITEM_ID_FLEURET;
    in_wpn.used = 0;
    Weapon_Load(weapons_dtab, in_wpn.id);

    /* -- Long weapon names -- */
    Silou.handedness = UNIT_HAND_LEFTIE;
    Silou._equipment[0].used = 1;
    Silou._equipment[1].used = 0;
    Silou._equipment[2].used = 0;
    Silou._equipment[0].id   = ITEM_ID_RETRACTABLE_WRISTBLADE;
    Silou._equipment[1].id   = ITEM_ID_REPEATABLE_CROSSBOW;
    Silou._equipment[2].id   = ITEM_ID_HONJOU_MASAMUNE;
    Silou._equipment[3].id   = ITEM_ID_SILVERLIGHT_SPEAR;
    Weapon_Load(weapons_dtab, Silou._equipment[0].id);
    Weapon_Load(weapons_dtab, Silou._equipment[1].id);
    Weapon_Load(weapons_dtab, Silou._equipment[2].id);
    Weapon_Load(weapons_dtab, Silou._equipment[3].id);
    idm->unit->eq_usable[0] = 0;
    idm->unit->eq_usable[1] = 1;
    idm->unit->eq_usable[2] = 2;
    idm->unit->eq_usable[3] = 3;
    idm->unit->num_usable   = 4;

    idm->item_todrop = 0;
    ItemDropMenu_Update(idm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_item_drop", "ItemDropMenu_0.png"), renderer,
                            idm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    idm->item_todrop = 1;
    ItemDropMenu_Update(idm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_item_drop", "ItemDropMenu_1.png"), renderer,
                            idm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    idm->item_todrop = 2;
    ItemDropMenu_Update(idm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_item_drop", "ItemDropMenu_2.png"), renderer,
                            idm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    idm->item_todrop = 3;
    ItemDropMenu_Update(idm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_item_drop", "ItemDropMenu_3.png"), renderer,
                            idm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- FREE -- */
    n9Patch_Free(&n9patch);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    PixelFont_Free(idm->pixelnours, true);
    PixelFont_Free(idm->pixelnours_big, true);
    ItemDropMenu_Free(idm);
    Weapons_All_Free(weapons_dtab);
    DTAB_FREE(items_dtab);
    DTAB_FREE(weapons_dtab);
}