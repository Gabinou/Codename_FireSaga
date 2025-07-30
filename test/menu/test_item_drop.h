
#include "nourstest.h"
#include "tnecs.h"
#include "globals.h"
#include "menu/item_drop.h"

#define TEST_SET_EQUIPMENT(world, ID, eq) \
    seteqentity  = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Inventory_item_ID);\
    seteqinvitem = IES_GET_COMPONENT(world, seteqentity, Inventory_item);\
    seteqinvitem->id = ID;\
    silou_eq[eq] = seteqentity;

void test_menu_item_drop() {
    /* -- Preliminaries -- */
    tnecs_world *world = NULL;
    tnecs_world_genesis(&world);
    gl_world = world;

#include "register/components.h"

    sota_mkdir("menu_item_drop");
    tnecs_entity    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;

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
    SDL_assert(Silou.equipment.num == 0);
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Silou_test.json")), &Silou);
    SDL_assert(Silou.equipment.num == 4);

    /* - Unit equip - */
    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id   = ITEM_ID_FLEURET;
    in_wpn.used = 0;
    Weapon_Load(gl_weapons_dtab, in_wpn.id);

    /* -- Long weapon names -- */
    Silou.flags.handedness = UNIT_HAND_LEFTIE;
    tnecs_entity *silou_eq = Unit_Equipment(&Silou);

    TEST_SET_EQUIPMENT(world, ITEM_ID_RETRACTABLE_WRISTBLADE, 0);
    Weapon_Load(gl_weapons_dtab, seteqinvitem->id);
    seteqinvitem->used = 1;
    TEST_SET_EQUIPMENT(world, ITEM_ID_REPEATABLE_CROSSBOW, 1);
    Weapon_Load(gl_weapons_dtab, seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_HONJOU_MASAMUNE, 2);
    Weapon_Load(gl_weapons_dtab, seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_SILVERLIGHT_SPEAR, 3);
    Weapon_Load(gl_weapons_dtab, seteqinvitem->id);

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
    Weapons_All_Free(gl_weapons_dtab);
    DTAB_FREE(gl_items_dtab);
    DTAB_FREE(gl_weapons_dtab);

    SDL_DestroyRenderer(renderer);
    tnecs_world_destroy(&world);
    gl_world = NULL;
}

#undef TEST_SET_EQUIPMENT