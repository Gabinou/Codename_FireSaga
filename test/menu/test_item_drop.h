
#include "tnecs.h"
#include "globals.h"
#include "nourstest.h"

#include "menu/item_drop.h"

#define TEST_SET_EQUIPMENT(world, ID, eq) \
    seteqentity  = IES_E_CREATE_wC(world, InvItem_ID);\
    seteqinvitem = IES_GET_C(world, seteqentity, InvItem);\
    seteqinvitem->id = ID;\
    silou_eq[eq] = seteqentity;

void test_menu_item_drop() {
    /* -- Preliminaries -- */
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;

#include "register/components.h"

    sota_mkdir("menu_item_drop");
    tnecs_E    seteqentity     = TNECS_NULL;
    InvItem *seteqinvitem    = NULL;

    /* -- Weapon dtab -- */
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab,   struct Item);

    /* -- Create n9patch -- */
    struct n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;

    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    /* -- Create LoadoutSelectMenu -- */
    struct ItemDropMenu *idm = ItemDropMenu_Alloc();
    ItemDropMenu_Load(idm, renderer, &n9patch);

    /* - loading fonts - */
    idm->pixelnours     = PixelFont_New();
    idm->pixelnours_big = PixelFont_New();
    PixelFont_Load(idm->pixelnours,     renderer, PATH_JOIN("..", "assets", "fonts", "pixelnours.png"));
    PixelFont_Load(idm->pixelnours_big, renderer, PATH_JOIN("..", "assets", "fonts",
                                                            "pixelnours_Big.png"));
    SDL_assert(idm->pixelnours);
    SDL_assert(idm->pixelnours_big);

    /* -- Create Unit -- */
    tnecs_E Silou_E = IES_E_CREATE_wC(world, Unit_ID);
    SDL_assert(Silou_E != TNECS_NULL);
    Unit *Silou = IES_GET_C(world, Silou_E, Unit);
    SDL_assert(Silou != NULL);

    Unit_Init(Silou);
    idm->unit_E = Silou_E;

    /* - title - */
    SDL_assert(Silou->equipment.num == 0);
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Silou_test.json")), Silou);
    SDL_assert(Silou->equipment.num == 4);

    /* - Unit equip - */
    struct InvItem in_wpn = InvItem_default;
    in_wpn.id   = ITEM_ID_FLEURET;
    in_wpn.used = 0;
    Item_Load(in_wpn.id);

    /* -- Long weapon names -- */
    Silou->flags.handedness = UNIT_HAND_LEFTIE;
    tnecs_E *silou_eq = Unit_Equipment(Silou);

    TEST_SET_EQUIPMENT(world, ITEM_ID_RETRACTABLE_WRISTBLADE, 0);
    Item_Load(seteqinvitem->id);
    seteqinvitem->used = 1;
    TEST_SET_EQUIPMENT(world, ITEM_ID_REPEATABLE_CROSSBOW, 1);
    Item_Load(seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_HONJOU_MASAMUNE, 2);
    Item_Load(seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_SILVERLIGHT_SPEAR, 3);
    Item_Load(seteqinvitem->id);

    idm->eq_todrop = ITEM1;
    ItemDropMenu_Update(idm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_item_drop", "ItemDropMenu_0.png"), renderer,
                            idm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    idm->eq_todrop = ITEM2;
    ItemDropMenu_Update(idm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_item_drop", "ItemDropMenu_1.png"), renderer,
                            idm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    idm->eq_todrop = ITEM3;
    ItemDropMenu_Update(idm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_item_drop", "ItemDropMenu_2.png"), renderer,
                            idm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    idm->eq_todrop = ITEM4;
    ItemDropMenu_Update(idm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_item_drop", "ItemDropMenu_3.png"), renderer,
                            idm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SDL_free -- */
    Unit_Free(Silou);
    n9Patch_Free(&n9patch);
    SDL_FreeSurface(surface);
    PixelFont_Free(idm->pixelnours, true);
    PixelFont_Free(idm->pixelnours_big, true);
    ItemDropMenu_Free(idm);
    Weapons_All_Free();
    DTAB_FREE(gl_items_dtab);
    DTAB_FREE(gl_weapons_dtab);

    SDL_DestroyRenderer(renderer);
    tnecs_finale(&world);
    gl_world = NULL;
}

#undef TEST_SET_EQUIPMENT
