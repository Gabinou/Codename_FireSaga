
#include "tnecs.h"
#include "globals.h"
#include "nourstest.h"

#include "menu/item_drop.h"

#define TEST_SET_EQUIPMENT(world, ID, eq) \
    seteqentity  = IES_E_CREATE_wC(world, Inventory_item_ID);\
    seteqinvitem = IES_GET_C(world, seteqentity, Inventory_item);\
    seteqinvitem->id = ID;\
    silou_eq[eq] = seteqentity;


void test_menu_item_select(void) {
    /* -- Preliminaries -- */
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;

#include "register/components.h"

    sota_mkdir("menu_item_drop");
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;

    /* -- Weapon dtab -- */
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab,   struct Item);

    /* -- Create Unit -- */
    struct Unit Silou = Unit_default;
    Unit_Init(&Silou);
    idm->unit = &Silou;

    /* -- Create ItemSelectMenu -- */
    struct ItemDropMenu *idm = ItemSelectMenu_Alloc();
    ItemSelectMenu_Load(idm, renderer, &n9patch);


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
    tnecs_finale(&world);
    gl_world = NULL;


}

#undef TEST_SET_EQUIPMENT
