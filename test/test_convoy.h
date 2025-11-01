/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** Test for Convoy
**
*/

#include "item.h"
#include "convoy.h"
#include "globals.h"
#include "nourstest.h"

#include "game/game.h"
#include "game/unit.h"

void test_convoy_full() {
    Convoy convoy            = Convoy_default;
    InvItem invitem   = InvItem_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab, Item);

    ntest(convoy.num_wagons > 0);
    ntest(_Convoy_Size(&convoy) > 0);
    ntest(_Convoy_Size(&convoy) == convoy.num_wagons * CONVOY_WAGON_SIZE);
    ntest(_Convoy_Num_Items(&convoy) == 0);
    ntest(!Convoy_isFull(&convoy));

    /* Adding sword to convoy */
    invitem.id = ITEM_ID_GLADIUS;
    SDL_assert(!Item_Pure_ID_isValid(invitem.id));
    Item_Load(invitem.id);
    i32 id = Convoy_Deposit(&convoy, invitem);
    int id_with_1 = id + 1;
    ntest(_Convoy_Num_Items(&convoy)    == 1);
    ntest(_Convoy_Index2Order(id)       == 0);

    /* used sword should be inserted *BEFORE* */
    invitem.used = 1;
    Item_Load(invitem.id);
    u16 type = _Item_Type(invitem.id);

    id = Convoy_Deposit(&convoy, invitem);
    ntest(_Convoy_Num_Items(&convoy)    == 2);
    ntest(_Convoy_Index2Order(id)       == 0);

    invitem.used = 2;
    Item_Load(invitem.id);
    type = _Item_Type(invitem.id);

    id = Convoy_Deposit(&convoy, invitem);
    ntest(_Convoy_Num_Items(&convoy)    == 3);
    ntest(_Convoy_Index2Order(id)       == 0);

    /* Check used of all inserted swords */
    ntest(Convoy_Item(&convoy, id + 0).used  == 2);
    ntest(Convoy_Item(&convoy, id + 1).used  == 1);
    ntest(Convoy_Item(&convoy, id + 2).used  == 0);

    /* Adding lance to convoy */
    invitem.id      = ITEM_ID_IRON_LANCE;
    Item_Load(invitem.id);
    invitem.used    = 5;
    id = Convoy_Deposit(&convoy, invitem);
    int id_lance = id;
    ntest(_Convoy_Index2Type(id)    == ITEM_TYPE_EXP_LANCE);
    ntest(_Convoy_Index2Order(id)       == 0);
    ntest(_Convoy_Num_Items(&convoy)    == 4);
    invitem.used    = 10;
    id = Convoy_Deposit(&convoy, invitem);
    ntest(_Convoy_Index2Type(id)    == ITEM_TYPE_EXP_LANCE);
    ntest(_Convoy_Index2Order(id)       == 0);
    ntest(_Convoy_Num_Items(&convoy)    == 5);

    /* --- Withdrawing from convoy --- */
    ntest(Convoy_Item(&convoy, id_with_1 - 1).used == 2);
    ntest(Convoy_Item(&convoy, id_with_1 + 0).used == 1);
    ntest(Convoy_Item(&convoy, id_with_1 + 1).used == 0);
    ntest(Convoy_Item(&convoy, id_lance).used == 10);
    ntest(Convoy_Item(&convoy, id_lance + 1).used == 5);
    ntest(_Convoy_Num_Items(&convoy)    == 5);

    Convoy_Withdraw(&convoy, id_with_1);
    ntest(Convoy_Item(&convoy, id_with_1 - 1).used == 2);
    ntest(Convoy_Item(&convoy, id_with_1 + 0).used == 0);
    ntest(_Convoy_Num_Items(&convoy)    == 4);
    ntest(Convoy_Item(&convoy, id_lance).used == 10);
    ntest(Convoy_Item(&convoy, id_lance + 1).used == 5);

    Convoy_Withdraw(&convoy, id_with_1 - 1);
    ntest(Convoy_Item(&convoy, id_with_1 - 1).used == 0);
    ntest(_Convoy_Num_Items(&convoy)    == 3);
    ntest(Convoy_Item(&convoy, id_lance).used == 10);
    ntest(Convoy_Item(&convoy, id_lance + 1).used == 5);

    /* --- Filling convoy --- */
    // convoy.num_items[ITEM_TYPE_EXP_SWORD] = 0;
    Convoy_Clear(&convoy);
    ntest(convoy.num_items[ITEM_TYPE_EXP_SWORD] == 0);
    ntest(convoy.num_items[ITEM_TYPE_EXP_LANCE] == 0);

    convoy.num_items[ITEM_TYPE_EXP_LANCE] = _Convoy_Size(&convoy);
    ntest(Convoy_isFull(&convoy));
    ntest(_Convoy_Num_Items(&convoy) == _Convoy_Size(&convoy));

    /* On deposit nothing happens, convoy is full */
    id = Convoy_Deposit(&convoy, invitem);
    ntest(Convoy_isFull(&convoy));
    ntest(_Convoy_Num_Items(&convoy) == _Convoy_Size(&convoy));

    /* Free */
    Game_Items_Free(&gl_items_dtab);
    Game_Weapons_Free(&gl_weapons_dtab);
    gl_items_dtab   = NULL;
    gl_weapons_dtab = NULL;
}

void test_convoy_bank() {
    Convoy convoy = Convoy_default;

    ntest(Convoy_Bank(&convoy) == 0);
    Convoy_Earn(&convoy, -1);
    ntest(Convoy_Bank(&convoy) == 0);

    i32 in = 10;
    Convoy_Earn(&convoy, in);
    ntest(Convoy_Bank(&convoy) == in);
    Convoy_Spend( &convoy, -1);
    ntest(Convoy_Bank(&convoy) == in);

    i32 out = 2;
    Convoy_Spend( &convoy, out);
    ntest(Convoy_Bank(&convoy) == in - out);
    Convoy_Spend( &convoy, in);
    ntest(Convoy_Bank(&convoy) == 0);

    in = CONVOY_MONEY_MAX * 10;
    Convoy_Earn(&convoy, in);
    ntest(Convoy_Bank(&convoy) == CONVOY_MONEY_MAX);

    out = CONVOY_MONEY_MAX - CONVOY_MONEY_MAX / 3;
    Convoy_Spend( &convoy, out);
    ntest(Convoy_Bank(&convoy) == CONVOY_MONEY_MAX - out);
}

void test_convoy_io() {
    Convoy convoy            = Convoy_default;
    InvItem invitem   = InvItem_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);

    /* --- Deposit Axes --- */
    /* -- BATTLEAXE -- */
    invitem.id      = ITEM_ID_BATTLEAXE;
    Item_Load(invitem.id);
    Convoy_Deposit(&convoy, invitem);

    invitem.used    = 2;
    Convoy_Deposit(&convoy, invitem);

    invitem.used    = 5;
    Convoy_Deposit(&convoy, invitem);

    /* -- IRON_AXE -- */
    invitem.id      = ITEM_ID_IRON_AXE;
    Item_Load(invitem.id);

    Convoy_Deposit(&convoy, invitem);

    invitem.used    = 11;
    Convoy_Deposit(&convoy, invitem);
    Convoy_Deposit(&convoy, invitem);
    invitem.used    = 1;
    Convoy_Deposit(&convoy, invitem);
    invitem.used    = 11;
    Convoy_Deposit(&convoy, invitem);
    invitem.used    = 1;
    Convoy_Deposit(&convoy, invitem);


    /* --- Deposit swords --- */
    /* -- Gladius -- */
    invitem.id      = ITEM_ID_GLADIUS;
    Item_Load(invitem.id);

    invitem.used    = 1;
    Convoy_Deposit(&convoy, invitem);
    invitem.used    = 10;
    Convoy_Deposit(&convoy, invitem);
    invitem.used    = 22;
    Convoy_Deposit(&convoy, invitem);

    /* -- Kitchen knife -- */
    invitem.id      = ITEM_ID_KITCHEN_KNIFE;
    Item_Load(invitem.id);

    invitem.used    = 0;
    Convoy_Deposit(&convoy, invitem);

    invitem.used    = 7;
    Convoy_Deposit(&convoy, invitem);

    /* -- Raw Iron Slab -- */
    invitem.id      = ITEM_ID_RAW_IRON_SLAB;
    Item_Load(invitem.id);

    invitem.used    = 5;
    Convoy_Deposit(&convoy, invitem);

    invitem.used    = 0;
    Convoy_Deposit(&convoy, invitem);

    /* -- Secundus -- */
    invitem.id      = ITEM_ID_SECUNDUS;
    Item_Load(invitem.id);

    invitem.used    = 6;
    Convoy_Deposit(&convoy, invitem);

    invitem.used    = 1;
    Convoy_Deposit(&convoy, invitem);

    /* --- Deposit Lances --- */
    /* -- FIGHTING_STICK -- */
    invitem.id      = ITEM_ID_FIGHTING_STICK;
    Item_Load(invitem.id);

    Convoy_Deposit(&convoy, invitem);

    invitem.used    = 11;
    Convoy_Deposit(&convoy, invitem);

    invitem.used    = 6;
    Convoy_Deposit(&convoy, invitem);

    invitem.used    = 2;
    Convoy_Deposit(&convoy, invitem);
    Convoy_Deposit(&convoy, invitem);

    /* -- DAMAS_LANCE -- */
    invitem.id      = ITEM_ID_DAMAS_LANCE;
    Item_Load(invitem.id);
    Convoy_Deposit(&convoy, invitem);

    invitem.used    = 1;
    Convoy_Deposit(&convoy, invitem);

    invitem.used    = 0;
    Convoy_Deposit(&convoy, invitem);

    invitem.used    = 2;
    Convoy_Deposit(&convoy, invitem);

    invitem.id      = ITEM_ID_FIGHTING_STICK;
    invitem.used    = 3;
    Convoy_Deposit(&convoy, invitem);

    /* -- IRON_LANCE -- */
    invitem.id      = ITEM_ID_IRON_LANCE;
    Item_Load(invitem.id);
    Convoy_Deposit(&convoy, invitem);

    invitem.used    = 3;
    Convoy_Deposit(&convoy, invitem);
    invitem.used    = 10;
    Convoy_Deposit(&convoy, invitem);
    invitem.used    = 1;
    Convoy_Deposit(&convoy, invitem);

    invitem.id      = ITEM_ID_FIGHTING_STICK;
    Convoy_Deposit(&convoy, invitem);

    invitem.id      = ITEM_ID_DAMAS_LANCE;
    Convoy_Deposit(&convoy, invitem);

    /* -- Writing convoy -- */
    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "convoy_test.json")), &convoy, false);

    /* -- Reading convoy -- */
    Convoy convoy2            = Convoy_default;
    jsonio_readJSON(s8_literal(PATH_JOIN("saves", "convoy_test.json")), &convoy2);
    ntest(convoy.num_items[ITEM_TYPE_EXP_SWORD] ==
          convoy2.num_items[ITEM_TYPE_EXP_SWORD]);
    ntest(convoy.num_items[ITEM_TYPE_EXP_LANCE] ==
          convoy2.num_items[ITEM_TYPE_EXP_LANCE]);
    ntest(convoy.num_items[ITEM_TYPE_EXP_AXE] ==
          convoy2.num_items[ITEM_TYPE_EXP_AXE]);
    ntest(convoy.num_items[ITEM_TYPE_EXP_BOW] ==
          convoy2.num_items[ITEM_TYPE_EXP_BOW]);

    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "convoy_test22.json")), &convoy2, false);

    nourstest_true(Filesystem_fequal(PATH_JOIN("saves", "convoy_test.json"), PATH_JOIN("saves",
                                     "convoy_test2.json")));
    /* Free */
    Game_Items_Free(&gl_items_dtab);
    Game_Weapons_Free(&gl_weapons_dtab);
    gl_items_dtab   = NULL;
    gl_weapons_dtab = NULL;
}

void test_convoy() {
    test_convoy_full();
    test_convoy_bank();
    test_convoy_io();
}
