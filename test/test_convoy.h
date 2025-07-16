
#include "nourstest.h"
#include "convoy.h"
#include "globals.h"
#include "item.h"
#include "game/game.h"
#include "game/unit.h"

void test_convoy_full() {
    Convoy convoy            = Convoy_default;
    Inventory_item invitem   = Inventory_item_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);

    ntest(convoy.num_wagons > 0);
    ntest(_Convoy_Size(&convoy) > 0);
    ntest(_Convoy_Size(&convoy) == convoy.num_wagons * CONVOY_WAGON_SIZE);
    ntest(_Convoy_Num_Items(&convoy) == 0);
    ntest(!Convoy_isFull(&convoy));

    /* Adding sword to convoy */
    invitem.id = ITEM_ID_GLADIUS;
    Weapon_Load(gl_weapons_dtab, invitem.id);
    i32 id = Convoy_Deposit(&convoy, invitem);
    ntest(_Convoy_Num_Items(&convoy)    == 1);
    ntest(_Convoy_Index2Order(id)       == 0);

    /* used sword should be inserted *BEFORE* */
    invitem.used = 1;
    Weapon_Load(gl_weapons_dtab, invitem.id);
    u16 type = Item_ID2Type(invitem.id);

    id = Convoy_Deposit(&convoy, invitem);
    ntest(_Convoy_Num_Items(&convoy)    == 2);
    ntest(_Convoy_Index2Order(id)       == 0);

    invitem.used = 2;
    Weapon_Load(gl_weapons_dtab, invitem.id);
    type = Item_ID2Type(invitem.id);

    id = Convoy_Deposit(&convoy, invitem);
    ntest(_Convoy_Num_Items(&convoy)    == 3);
    ntest(_Convoy_Index2Order(id)       == 0);

    /* Check used of all inserted swords */
    ntest(Convoy_Item(&convoy, id + 0).used  == 2);
    ntest(Convoy_Item(&convoy, id + 1).used  == 1);
    ntest(Convoy_Item(&convoy, id + 2).used  == 0);

    /* Adding lance to convoy */
    invitem.id      = ITEM_ID_IRON_LANCE;
    Weapon_Load(gl_weapons_dtab, invitem.id);
    invitem.used    = 5;
    id = Convoy_Deposit(&convoy, invitem);
    ntest(_Convoy_Index2Type(id)    == ITEM_TYPE_EXP_LANCE);
    ntest(_Convoy_Index2Order(id)       == 0);
    ntest(_Convoy_Num_Items(&convoy)    == 4);
    invitem.used    = 10;
    id = Convoy_Deposit(&convoy, invitem);
    ntest(_Convoy_Index2Type(id)    == ITEM_TYPE_EXP_LANCE);
    ntest(_Convoy_Index2Order(id)       == 0);
    ntest(_Convoy_Num_Items(&convoy)    == 5);

    Game_Items_Free(&gl_items_dtab);
    Game_Weapons_Free(&gl_weapons_dtab);
    gl_items_dtab   = NULL;
    gl_weapons_dtab = NULL;

    /* --- Filling convoy --- */
    // convoy.num_items[ITEM_TYPE_EXP_SWORD] = 0;
    Convoy_Clear(&convoy);
    convoy.num_items[ITEM_TYPE_EXP_LANCE] = _Convoy_Size(&convoy);
    ntest(Convoy_isFull(&convoy));
    ntest(_Convoy_Num_Items(&convoy) == _Convoy_Size(&convoy));

    /* On deposit nothing happens, convoy is full */
    id = Convoy_Deposit(&convoy, invitem);
    ntest(Convoy_isFull(&convoy));
    ntest(_Convoy_Num_Items(&convoy) == _Convoy_Size(&convoy));
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

}

void test_convoy() {
    test_convoy_full();
    test_convoy_bank();
    test_convoy_io();
}
