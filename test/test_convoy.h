#include "nourstest.h"
#include "convoy.h"
#include "game/game.h"
#include "game/unit.h"

void testConvoyfull() {

    struct Convoy convoy = Convoy_default;
    struct Inventory_item temp = Inventory_item_default;
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);

    convoy.weapons_dtab = weapons_dtab;
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(!Convoy_isFull(&convoy));
    for (int i = 0; i < convoy.size - 1; i++) {
        Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    }
    nourstest_true(Convoy_isFull(&convoy));
    Convoy_Withdraw(&convoy, 0);
    nourstest_true(!Convoy_isFull(&convoy));
    Convoy_Withdraw(&convoy, 1);
    nourstest_true(!Convoy_isFull(&convoy));
    temp.id = ITEM_ID_DAMAS_LANCE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    nourstest_true(!Convoy_isFull(&convoy));
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    nourstest_true(Convoy_isFull(&convoy));
    Game_Weapons_Free(&weapons_dtab);
}

void testConvoysort() {

    struct Convoy convoy = Convoy_default;
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);

    convoy.weapons_dtab = weapons_dtab;
    struct Inventory_item temp = Inventory_item_default;
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_WRATH_SWORD;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_FLEURET;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_DAMAS_SWORD;
    temp.used = 0;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_DAMAS_SWORD;
    temp.used = 1;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    temp.used = 1;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    temp.used = 2;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    temp.used = 20;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    temp.used = 21;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    temp.used = 10;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    temp.used = 11;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    temp.used = 12;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_IRON_SWORD;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_STEEL_SWORD;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_STEEL_LANCE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);

    temp.id = ITEM_ID_LONG_BOW;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_BOW);
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_BOW);
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_BOW);

    convoy.sort_direction = SOTA_CONVOY_SORT_HIGH2LOW;
    Convoy_Sort(&convoy, WEAPON_STAT_HIT);
    for (int_fast16_t i = ITEM_TYPE_EXP_NULL + 1; i < ITEM_TYPE_NUM; i++) {
        for (int_fast16_t j = convoy.cumnum[i - 1]; j < (convoy.cumnum[i] - 1); j++) {
            SDL_Log("i,j, %ld %ld", i, j);
            struct Inventory_item item1 = convoy.items[j];
            SDL_assert(item1.id > ITEM_NULL);
            struct Inventory_item item2 = convoy.items[j + 1];
            SDL_assert(item2.id > ITEM_NULL);
            SDL_Log("item1.id, item2.id %ld %ld", item1.id, item2.id);
            struct Weapon *weapon1 = (struct Weapon *)DTAB_GET(convoy.weapons_dtab, item1.id);
            SDL_assert(weapon1 != NULL);
            struct Weapon *weapon2 = (struct Weapon *)DTAB_GET(convoy.weapons_dtab, item2.id);
            SDL_assert(weapon2 != NULL);
            SDL_Log("weapon1,weapon2, %hhu %hhu", weapon1->stats.hit, weapon2->stats.hit);
            nourstest_true(weapon1->item->type == weapon2->item->type);
            nourstest_true(weapon1->stats.hit >= weapon2->stats.hit);
        }
    }

    convoy.sort_direction = SOTA_CONVOY_SORT_LOW2HIGH;
    Convoy_Sort(&convoy, WEAPON_STAT_HIT);
    for (int_fast16_t i = ITEM_TYPE_EXP_NULL + 1; i < ITEM_TYPE_NUM; i++) {
        for (int_fast16_t j = convoy.cumnum[i - 1]; j < (convoy.cumnum[i] - 1); j++) {
            SDL_Log("i,j, %ld %ld", i, j);
            struct Inventory_item item1 = convoy.items[j];
            SDL_assert(item1.id > ITEM_NULL);
            struct Inventory_item item2 = convoy.items[j + 1];
            SDL_assert(item2.id > ITEM_NULL);
            SDL_Log("item1.id, item2.id %ld %ld", item1.id, item2.id);
            struct Weapon *weapon1 = (struct Weapon *)DTAB_GET(convoy.weapons_dtab, item1.id);
            SDL_assert(weapon1 != NULL);
            struct Weapon *weapon2 = (struct Weapon *)DTAB_GET(convoy.weapons_dtab, item2.id);
            SDL_assert(weapon2 != NULL);
            SDL_Log("weapon1,weapon2, %d %d", weapon1->stats.hit, weapon2->stats.hit);
            nourstest_true(weapon1->item->type == weapon2->item->type);
            nourstest_true(weapon1->stats.hit <= weapon2->stats.hit);
        }
    }

    // Convoy_Stats_Print(&convoy, ITEM_TYPE_EXP_SWORD, WEAPON_STAT_HIT);
    // Convoy_AllStats_Print(&convoy, ITEM_TYPE_EXP_SWORD);

    convoy.sort_direction = SOTA_CONVOY_SORT_HIGH2LOW;
    Convoy_Sort(&convoy, WEAPON_STAT_pATTACK);
    for (int_fast16_t i = ITEM_TYPE_EXP_NULL + 1; i < ITEM_TYPE_NUM; i++) {
        for (int_fast16_t j = convoy.cumnum[i - 1]; j < (convoy.cumnum[i] - 1); j++) {
            SDL_Log("i,j, %ld %ld", i, j);
            struct Inventory_item item1 = convoy.items[j];
            SDL_assert(item1.id > ITEM_NULL);
            struct Inventory_item item2 = convoy.items[j + 1];
            SDL_assert(item2.id > ITEM_NULL);
            SDL_Log("item1.id, item2.id %ld %ld", item1.id, item2.id);
            struct Weapon *weapon1 = (struct Weapon *)DTAB_GET(convoy.weapons_dtab, item1.id);
            SDL_assert(weapon1 != NULL);
            struct Weapon *weapon2 = (struct Weapon *)DTAB_GET(convoy.weapons_dtab, item2.id);
            SDL_assert(weapon2 != NULL);
            SDL_Log("weapon1,weapon2, %d %d", weapon1->stats.attack[DAMAGE_TYPE_PHYSICAL],
                    weapon2->stats.attack[DAMAGE_TYPE_PHYSICAL]);
            nourstest_true(weapon1->item->type == weapon2->item->type);
            nourstest_true(weapon1->stats.attack[DAMAGE_TYPE_PHYSICAL] >=
                           weapon2->stats.attack[DAMAGE_TYPE_PHYSICAL]);
        }
    }

    // Convoy_Stats_Print(&convoy, ITEM_TYPE_EXP_SWORD, WEAPON_STAT_pATTACK);
    // Convoy_AllStats_Print(&convoy, ITEM_TYPE_EXP_SWORD);

    convoy.sort_direction = SOTA_CONVOY_SORT_LOW2HIGH;
    Convoy_Sort(&convoy, WEAPON_STAT_pATTACK);
    for (int_fast16_t i = ITEM_TYPE_EXP_NULL + 1; i < ITEM_TYPE_NUM; i++) {
        for (int_fast16_t j = convoy.cumnum[i - 1]; j < (convoy.cumnum[i] - 1); j++) {
            SDL_Log("i,j, %ld %ld", i, j);
            struct Inventory_item item1 = convoy.items[j];
            SDL_assert(item1.id > ITEM_NULL);
            struct Inventory_item item2 = convoy.items[j + 1];
            SDL_assert(item2.id > ITEM_NULL);
            SDL_Log("item1.id, item2.id %ld %ld", item1.id, item2.id);
            struct Weapon *weapon1 = (struct Weapon *)DTAB_GET(convoy.weapons_dtab, item1.id);
            SDL_assert(weapon1 != NULL);
            struct Weapon *weapon2 = (struct Weapon *)DTAB_GET(convoy.weapons_dtab, item2.id);
            SDL_assert(weapon2 != NULL);
            SDL_Log("weapon1,weapon2, %d %d", weapon1->stats.attack[DAMAGE_TYPE_PHYSICAL],
                    weapon2->stats.attack[DAMAGE_TYPE_PHYSICAL]);
            nourstest_true(weapon1->item->type == weapon2->item->type);
            nourstest_true(weapon1->stats.attack[DAMAGE_TYPE_PHYSICAL] <=
                           weapon2->stats.attack[DAMAGE_TYPE_PHYSICAL]);
        }
    }

    // Convoy_Stats_Print(&convoy, ITEM_TYPE_EXP_SWORD, WEAPON_STAT_HIT);
    // Convoy_AllStats_Print(&convoy, ITEM_TYPE_EXP_SWORD);
    Game_Weapons_Free(&weapons_dtab);
}

void testConvoyWriteRead() {

    struct Convoy convoy = Convoy_default;
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);

    convoy.weapons_dtab = weapons_dtab;
    struct Inventory_item temp = Inventory_item_default;
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_NULL] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_SWORD] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_LANCE] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_AXE] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_BOW] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_TRINKET] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_OFFHAND] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_ELEMENTAL] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_DEMONIC] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_ANGELIC] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_SHIELD] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_STAFF] == 0);

    temp.id = ITEM_ID_KITCHEN_KNIFE;
    size_t item_num = 0;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items_num == ++item_num);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_NULL] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_SWORD] == 1);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_LANCE] == 1);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_AXE] == 1);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_BOW] == 1);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_TRINKET] == 1);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_OFFHAND] == 1);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_ELEMENTAL] == 1);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_DEMONIC] == 1);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_ANGELIC] == 1);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_SHIELD] == 1);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_STAFF] == 1);

    temp.id = ITEM_ID_KITCHEN_KNIFE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_WRATH_SWORD;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_FLEURET;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_DAMAS_SWORD;
    temp.used = 0;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_DAMAS_SWORD;
    temp.used = 1;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    temp.used = 1;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    temp.used = 2;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    temp.used = 20;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    temp.used = 21;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    temp.used = 10;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    temp.used = 11;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    temp.used = 12;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_KITCHEN_KNIFE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_IRON_SWORD;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_DAMAS_LANCE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    nourstest_true(convoy.items_num == ++item_num);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_NULL] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_SWORD] == 15);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_LANCE] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_AXE] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_BOW] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_TRINKET] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_OFFHAND] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_ELEMENTAL] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_DEMONIC] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_ANGELIC] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_SHIELD] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_STAFF] == 16);

    temp.id = ITEM_ID_LONG_BOW;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_BOW);
    nourstest_true(convoy.items_num == ++item_num);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_NULL] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_SWORD] == 15);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_LANCE] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_AXE] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_BOW] == 17);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_TRINKET] == 17);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_OFFHAND] == 17);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_ELEMENTAL] == 17);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_DEMONIC] == 17);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_ANGELIC] == 17);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_SHIELD] == 17);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_STAFF] == 17);

    Convoy_Withdraw(&convoy, 0);
    nourstest_true(convoy.items_num == --item_num);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_NULL] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_SWORD] == 14);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_LANCE] == 15);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_AXE] == 15);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_BOW] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_TRINKET] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_OFFHAND] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_ELEMENTAL] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_DEMONIC] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_ANGELIC] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_SHIELD] == 16);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_STAFF] == 16);

    temp.id = ITEM_ID_ACHILLES;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_VEL;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_IRON_LANCE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_TONBOKIRI;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_DAMAS_LANCE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_WRATH_LANCE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_WRATH_LANCE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_TRIDENT;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    nourstest_true(convoy.items_num == ++item_num);
    temp.id = ITEM_ID_SPEAR;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    nourstest_true(convoy.items_num == ++item_num);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_NULL] == 0);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_SWORD] == 14);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_LANCE] == 24);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_AXE] == 24);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_BOW] == 25);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_TRINKET] == 25);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_OFFHAND] == 25);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_ELEMENTAL] == 25);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_DEMONIC] == 25);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_ANGELIC] == 25);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_SHIELD] == 25);
    nourstest_true(convoy.cumnum[ITEM_TYPE_EXP_STAFF] == 25);

    Convoy_Withdraw(&convoy, 20);

    if (PHYSFS_stat("saves", NULL) == 0) {
        PHYSFS_mkdir("saves");
    }
    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "convoy_test.json")), &convoy, false);
    nourstest_true(PHYSFS_exists(PATH_JOIN("saves", "convoy_test.json")));
    struct Convoy convoy2 = Convoy_default;
    convoy2.weapons_dtab = weapons_dtab;

    jsonio_readJSON(s8_literal(PATH_JOIN("saves", "convoy_test.json")), &convoy2);
    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "convoy_rewrite.json")), &convoy2, false);
    nourstest_true(PHYSFS_exists(PATH_JOIN("saves", "convoy_rewrite.json")));
    nourstest_true(Filesystem_fequal(PATH_JOIN("saves", "convoy_rewrite.json"),
                                     PATH_JOIN("saves", "convoy_test.json")));
    Game_Weapons_Free(&weapons_dtab);
}

void testConvoyExp() {

    nourstest_true(0 == Convoy_TypeCode2Exp(NULL, 0));
    nourstest_true(1 == Convoy_TypeCode2Exp(NULL, 1));
    nourstest_true(2 == Convoy_TypeCode2Exp(NULL, 2));
    nourstest_true(1 == Convoy_TypeCode2Exp(NULL, 3));
    nourstest_true(4 == Convoy_TypeCode2Exp(NULL, 4));
    nourstest_true(1 == Convoy_TypeCode2Exp(NULL, 5));
    nourstest_true(2 == Convoy_TypeCode2Exp(NULL, 6));
    nourstest_true(1 == Convoy_TypeCode2Exp(NULL, 7));
    nourstest_true(8 == Convoy_TypeCode2Exp(NULL, 8));
    nourstest_true(1 == Convoy_TypeCode2Exp(NULL, 9));
    nourstest_true(2 == Convoy_TypeCode2Exp(NULL, 10));
    nourstest_true(1 == Convoy_TypeCode2Exp(NULL, 11));
    nourstest_true(4 == Convoy_TypeCode2Exp(NULL, 12));
    nourstest_true(1 == Convoy_TypeCode2Exp(NULL, 13));
}

void testConvoySwap() {
    struct Convoy convoy = Convoy_default;
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);

    convoy.weapons_dtab = weapons_dtab;
    struct Inventory_item temp = Inventory_item_default;

    temp.id = ITEM_ID_KITCHEN_KNIFE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    temp.id = ITEM_ID_IRON_SWORD;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_SWORD);
    nourstest_true(convoy.items[0].id == ITEM_ID_KITCHEN_KNIFE);
    nourstest_true(convoy.items[1].id == ITEM_ID_IRON_SWORD);
    Convoy_Swap(&convoy, 0, 1);
    nourstest_true(convoy.items[0].id == ITEM_ID_IRON_SWORD);
    nourstest_true(convoy.items[1].id == ITEM_ID_KITCHEN_KNIFE);
    Convoy_Swap(&convoy, 0, 1);
    nourstest_true(convoy.items[0].id == ITEM_ID_KITCHEN_KNIFE);
    nourstest_true(convoy.items[1].id == ITEM_ID_IRON_SWORD);
    Convoy_Swap(&convoy, 0, 1);

    temp.id = ITEM_ID_DAMAS_LANCE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    temp.id = ITEM_ID_STEEL_LANCE;
    Convoy_Deposit_byType(&convoy, temp, ITEM_TYPE_EXP_LANCE);
    nourstest_true(convoy.items[2].id == ITEM_ID_DAMAS_LANCE);
    nourstest_true(convoy.items[3].id == ITEM_ID_STEEL_LANCE);
    Convoy_Swap(&convoy, 2, 3);
    nourstest_true(convoy.items[2].id == ITEM_ID_STEEL_LANCE);
    nourstest_true(convoy.items[3].id == ITEM_ID_DAMAS_LANCE);
    Convoy_Swap(&convoy, 2, 3);

    Game_Weapons_Free(&weapons_dtab);
}

void test_convoy() {

    testConvoyfull();
    testConvoyWriteRead();
    testConvoysort();
    testConvoyExp();
    testConvoySwap();
}
