
#include "nourstest.h"
#include "convoy.h"
#include "globals.h"
#include "item.h"
#include "game/game.h"
#include "game/unit.h"

void testConvoyfull() {
    struct Convoy convoy = Convoy_default;
    struct Inventory_item temp = Inventory_item_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);

    ntest(_Convoy_Size(&convoy) == convoy.num_wagons * CONVOY_WAGON_SIZE);
    // void           Convoy_Deposit(  Convoy *c,
    //                                 Inventory_item i);
    // Inventory_item Convoy_Withdraw( Convoy *c, i32 i);



    Game_Items_Free(&gl_items_dtab);
    Game_Weapons_Free(&gl_weapons_dtab);
    gl_items_dtab   = NULL;
    gl_weapons_dtab = NULL;
}

void testConvoyWriteRead() {
    // struct Convoy convoy = Convoy_default;
    // gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);

    // Convoy_Withdraw(&convoy, 20);

    // if (PHYSFS_stat("saves", NULL) == 0) {
    //     PHYSFS_mkdir("saves");
    // }
    // jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "convoy_test.json")), &convoy, false);
    // nourstest_true(PHYSFS_exists(PATH_JOIN("saves", "convoy_test.json")));
    // struct Convoy convoy2 = Convoy_default;

    // jsonio_readJSON(s8_literal(PATH_JOIN("saves", "convoy_test.json")), &convoy2);
    // jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "convoy_rewrite.json")), &convoy2, false);
    // nourstest_true(PHYSFS_exists(PATH_JOIN("saves", "convoy_rewrite.json")));
    // nourstest_true(Filesystem_fequal(PATH_JOIN("saves", "convoy_rewrite.json"),
    //                                  PATH_JOIN("saves", "convoy_test.json")));
    // Game_Weapons_Free(&gl_weapons_dtab);

    // Convoy_Free(&convoy);
    // Convoy_Free(&convoy2);
}

void test_convoy() {
    testConvoyfull();
    testConvoyWriteRead();
}
