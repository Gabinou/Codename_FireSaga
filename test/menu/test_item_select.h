
#include "tnecs.h"
#include "globals.h"
#include "nourstest.h"

#include "menu/item_select.h"

#define TEST_SET_EQUIPMENT(world, ID, eq) \
    seteqentity  = IES_E_CREATE_wC(world, Inventory_item_ID);\
    seteqinvitem = IES_GET_C(world, seteqentity, Inventory_item);\
    seteqinvitem->id = ID;\
    Weapon_Load(gl_weapons_dtab, seteqinvitem->id);\
    Unit_Item_Take(Silou, seteqentity);


void test_menu_item_select(void) {
    /* -- Preliminaries -- */
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;

#include "register/components.h"

    sota_mkdir("menu_item_select");
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;

    /* -- Weapon dtab -- */
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab,   struct Item);

    /* -- Create n9patch -- */
    struct n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;

    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    /* -- Create ItemSelectMenu -- */
    struct ItemSelectMenu *ism = ItemSelectMenu_Alloc();
    ItemSelectMenu_Load(ism, renderer, &n9patch);
    Point size = n9Patch_Pixels_Total(&n9patch);
    SDL_assert(size.x > 0);
    SDL_assert(size.y > 0);

    /* - loading fonts - */
    ism->pixelnours     = PixelFont_Alloc();
    ism->pixelnours_big = PixelFont_Alloc();
    char *pathnours = PATH_JOIN("..", "assets", "fonts",
                                "pixelnours.png");
    char *pathbig   = PATH_JOIN("..", "assets", "fonts",
                                "pixelnours_Big.png");
    PixelFont_Load(ism->pixelnours,     renderer, pathnours);
    PixelFont_Load(ism->pixelnours_big, renderer, pathbig);
    SDL_assert(ism->pixelnours);
    SDL_assert(ism->pixelnours_big);

    /* -- Create Unit -- */
    tnecs_E silou_ent = IES_E_CREATE_wC(gl_world, Unit_ID);
    SDL_assert(silou_ent != TNECS_NULL);
    Unit *Silou = IES_GET_C(gl_world, silou_ent, Unit);
    Unit_Init(Silou);
    ism->_unit = silou_ent;

    /* -- Load items -- */
    Silou->flags.handedness = UNIT_HAND_LEFTIE;
    tnecs_E *silou_eq = Unit_Equipment(Silou);

    TEST_SET_EQUIPMENT(world, ITEM_ID_RETRACTABLE_WRISTBLADE, 0);
    seteqinvitem->used = 1;
    TEST_SET_EQUIPMENT(world, ITEM_ID_REPEATABLE_CROSSBOW, 1);
    TEST_SET_EQUIPMENT(world, ITEM_ID_HONJOU_MASAMUNE, 2);
    TEST_SET_EQUIPMENT(world, ITEM_ID_SILVERLIGHT_SPEAR, 3);
    SDL_assert(Unit_Equipment_Num(Silou) > 0);

    size = n9Patch_Pixels_Total(&n9patch);
    SDL_assert(size.x > 0);
    SDL_assert(size.y > 0);

    ItemSelectMenu_Update(ism, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(
            PATH_JOIN("menu_item_select", "ism_0.png"),
            renderer, ism->texture,
            SDL_PIXELFORMAT_ARGB8888, render_target
    );

    /* -- SDL_free -- */
    Unit_Free(Silou);
    n9Patch_Free(&n9patch);
    SDL_FreeSurface(surface);
    PixelFont_Free(ism->pixelnours, true);
    PixelFont_Free(ism->pixelnours_big, true);
    ItemSelectMenu_Free(ism);
    Weapons_All_Free(gl_weapons_dtab);
    DTAB_FREE(gl_items_dtab);
    DTAB_FREE(gl_weapons_dtab);

    SDL_DestroyRenderer(renderer);
    tnecs_finale(&world);
    gl_world = NULL;


}

#undef TEST_SET_EQUIPMENT
