
#include "nourstest.h"
#include "menu/which_hand.h"

void test_menu_which_hand(void) {
    /* --- Preliminaries --- */
    sota_mkdir("menu_which_hand");

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

    /* --- Silou hands render --- */
    Item_Handedness_Set(&item, WEAPON_HAND_ANY);

    /* -- RH only -- */
    Unit_Hand_Set(&Silou, UNIT_HAND_LEFT,   0);
    Unit_Hand_Set(&Silou, UNIT_HAND_RIGHT,  1);

    /* -- LH only -- */
    Unit_Hand_Set(&Silou, UNIT_HAND_LEFT,   1);
    Unit_Hand_Set(&Silou, UNIT_HAND_RIGHT,  0);

    /* -- 2H -- */
    Unit_Hand_Set(&Silou, UNIT_HAND_LEFT,   1);
    Unit_Hand_Set(&Silou, UNIT_HAND_RIGHT,  1);

    /* --- Item handedness render --- */
    Item_Handedness_Set(&item, WEAPON_HAND_ANY);

    WhichHandMenu_Elements(struct Menu * mc,
                           struct Unit * unit,
                           struct Item * item)

    /* --- Free --- */
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
}
