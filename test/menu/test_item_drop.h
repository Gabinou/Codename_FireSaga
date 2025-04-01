
#include "nourstest.h"
#include "menu/item_drop.h"

void test_menu_item_drop() {
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
    PixelFont_Load(idm->pixelnours,     renderer, PATH_JOIN("..", "assets", "fonts", "pixelnours.png"));
    PixelFont_Load(idm->pixelnours_big, renderer, PATH_JOIN("..", "assets", "fonts",
                                                            "pixelnours_Big.png"));
    SDL_assert(idm->pixelnours);
    SDL_assert(idm->pixelnours_big);

    /* -- Create Unit -- */
    struct Unit Silou = Unit_default;
    Unit_Init(&Silou);
    idm->unit = &Silou;

    /* - title - */
    Silou.equipment.weapons_dtab = weapons_dtab;
    Silou.equipment.items_dtab   = items_dtab;
    SDL_assert(Silou.equipment.num == 0);
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Silou_test.json")), &Silou);
    SDL_assert(Silou.equipment.num == 4);

    /* - Unit equip - */
    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id   = ITEM_ID_FLEURET;
    in_wpn.used = 0;
    Weapon_Load(weapons_dtab, in_wpn.id);

    /* -- Long weapon names -- */
    Silou.handedness = UNIT_HAND_LEFTIE;
    Inventory_item *silou_eq = Unit_Equipment(&Silou);
    silou_eq[ITEM1 - ITEM1].used = 1;
    silou_eq[ITEM2 - ITEM1].used = 0;
    silou_eq[ITEM3 - ITEM1].used = 0;
    silou_eq[ITEM1 - ITEM1].id   = ITEM_ID_RETRACTABLE_WRISTBLADE;
    silou_eq[ITEM2 - ITEM1].id   = ITEM_ID_REPEATABLE_CROSSBOW;
    silou_eq[ITEM3 - ITEM1].id   = ITEM_ID_HONJOU_MASAMUNE;
    silou_eq[ITEM4 - ITEM1].id   = ITEM_ID_SILVERLIGHT_SPEAR;
    Weapon_Load(weapons_dtab, silou_eq[ITEM1 - ITEM1].id);
    Weapon_Load(weapons_dtab, silou_eq[ITEM2 - ITEM1].id);
    Weapon_Load(weapons_dtab, silou_eq[ITEM3 - ITEM1].id);
    Weapon_Load(weapons_dtab, silou_eq[ITEM4 - ITEM1].id);

    i32 *can_equip = Unit_canEquip_Arr(idm->unit);
    can_equip[0] = 0;
    can_equip[1] = 1;
    can_equip[2] = 2;
    can_equip[3] = 3;
    idm->unit->can_equip.num   = 4;

    idm->item_todrop = ITEM1;
    ItemDropMenu_Update(idm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_item_drop", "ItemDropMenu_0.png"), renderer,
                            idm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    idm->item_todrop = ITEM2;
    ItemDropMenu_Update(idm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_item_drop", "ItemDropMenu_1.png"), renderer,
                            idm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    idm->item_todrop = ITEM3;
    ItemDropMenu_Update(idm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_item_drop", "ItemDropMenu_2.png"), renderer,
                            idm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    idm->item_todrop = ITEM4;
    ItemDropMenu_Update(idm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_item_drop", "ItemDropMenu_3.png"), renderer,
                            idm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SDL_free -- */
    Unit_Free(&Silou);
    n9Patch_Free(&n9patch);
    SDL_FreeSurface(surface);
    PixelFont_Free(idm->pixelnours, true);
    PixelFont_Free(idm->pixelnours_big, true);
    ItemDropMenu_Free(idm);
    Weapons_All_Free(weapons_dtab);
    DTAB_FREE(items_dtab);
    DTAB_FREE(weapons_dtab);

    SDL_DestroyRenderer(renderer);
}