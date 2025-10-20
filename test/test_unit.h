
#include "RNG.h"
#include "tnecs.h"
#include "enums.h"
#include "weapon.h"
#include "weapon.h"
#include "platform.h"
#include "nourstest.h"
#include "reinforcement.h"

#include "game/game.h"
#include "game/unit.h"

#include "unit/unit.h"
#include "unit/range.h"
#include "unit/flags.h"
#include "unit/bonus.h"
#include "unit/stats.h"

#define TEST_SET_EQUIPMENT(world, ID, eq) \
    seteqentity  = IES_E_CREATE_wC(world, Inventory_item_ID);\
    seteqinvitem = IES_GET_C(world, seteqentity, Inventory_item);\
    seteqinvitem->id = ID;\
    silou_eq[eq - ITEM1] = seteqentity;


void test_canEquip_Range(void) {
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab,    struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab,      struct Item);

#include "register/components.h"

    /* --- Init Silou --- */
    struct Unit Silou = Unit_default;
    Unit_Init(&Silou);
    Silou.flags.equippable = ITEM_TYPE_LANCE;

    /* --- Equipping --- */
    Item_Load(ITEM_ID_GLADIUS);
    {
        Range range = {1, 2};
        nourstest_true(Unit_canEquip_Range(ITEM_ID_GLADIUS, &range, RANGE_ANY));
        nourstest_true(Unit_canEquip_Range(ITEM_ID_GLADIUS, &range, RANGE_INPUT));
    }
    {
        Range range = {2, 2};
        nourstest_true(Unit_canEquip_Range(ITEM_ID_GLADIUS, &range, RANGE_ANY));
        nourstest_true(!Unit_canEquip_Range(ITEM_ID_GLADIUS, &range, RANGE_INPUT));
    }

    /* --- Free --- */
    Unit_Free(&Silou);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    tnecs_finale(&world);
    gl_world = NULL;
}

void test_canEquip_Type(void) {
    struct Unit Silou = Unit_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);
    Unit_Init(&Silou);
    Silou.flags.equippable = 0;
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == false);

    Silou.flags.equippable = ITEM_TYPE_SWORD | ITEM_TYPE_OFFHAND | ITEM_TYPE_SHIELD;
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == true);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == true);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == true);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == true);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == false);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == false);

    Silou.flags.equippable = -1;
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == true);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == true);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == true);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == true);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == true);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == true);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == true);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == true);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == true);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == true);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == true);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == true);

    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    Unit_Free(&Silou);
}

void test_skills(void) {
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;

#include "register/components.h"

    int distance = 1;
    struct Unit Silou = Unit_default;
    struct Unit Hamilcar = Unit_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);
    Item_Load(ITEM_ID_FLEURET);
    struct Weapon *fleuret = DTAB_GET(gl_weapons_dtab, ITEM_ID_FLEURET);
    nourstest_true(fleuret != NULL);
    nourstest_true(fleuret->flags.canAttack);
    nourstest_true(Item_Typecode(&fleuret->item) == ITEM_TYPE_SWORD);
    nourstest_true(fleuret->item.ids.id == ITEM_ID_FLEURET);
    nourstest_true(Weapon_canAttack(fleuret));
    Unit_Init(&Silou);
    Unit_Init(&Hamilcar);
    //                           hp, str, mag, agi, dex, fth, luck, def, res, con, move
    struct Unit_stats in_stats = {17,  6,  2,  7,  7, 7,  7,  4,  5,  6, 5};
    Unit_setClassind(&Silou, UNIT_CLASS_FENCER);
    Unit_setClassind(&Hamilcar, UNIT_CLASS_FENCER);

    Unit_setStats(&Silou, in_stats);
    Unit_setStats(&Hamilcar, in_stats);
    Silou.flags.skills = UNIT_SKILL_PINPRICK;
    tnecs_E fleuret_ent  = IES_E_CREATE_wC(world, Inventory_item_ID);
    Inventory_item *inv_fleuret         = IES_GET_C(world, fleuret_ent, Inventory_item);
    inv_fleuret->id = ITEM_ID_FLEURET;

    Unit_Item_Drop(&Silou,           UNIT_HAND_LEFT);
    Unit_Item_Takeat(&Silou, fleuret_ent, UNIT_HAND_LEFT);
    Unit_Item_Drop(&Hamilcar,           UNIT_HAND_LEFT);
    Unit_Item_Takeat(&Hamilcar, fleuret_ent, UNIT_HAND_LEFT);
    Unit_Equip(&Silou, UNIT_HAND_LEFT, UNIT_HAND_LEFT);
    Unit_Equip(&Hamilcar, UNIT_HAND_LEFT, UNIT_HAND_LEFT);
    nourstest_true(Unit_isEquipped(&Hamilcar, UNIT_HAND_LEFT));
    nourstest_true(Unit_isEquipped(&Silou, UNIT_HAND_LEFT));
    nourstest_true(Unit_canAttack(&Hamilcar));
    nourstest_true(Unit_canAttack(&Silou));
    Unit_stats ES_S = Unit_effectiveStats(&Silou);
    Unit_stats ES_H = Unit_effectiveStats(&Hamilcar);
    Unit_computedStats(&Silou,      distance, ES_S);
    Unit_computedStats(&Hamilcar,   distance, ES_H);

    /* --- Free --- */
    Unit_Free(&Silou);
    Unit_Free(&Hamilcar);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    tnecs_finale(&world);
    gl_world = NULL;
}

void test_io(void) {
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;

#include "register/components.h"

    struct Unit unit1 = Unit_default;
    struct Unit unit2 = Unit_default;
    struct Unit unit3 = Unit_default;
    struct Unit unit4 = Unit_default;
    struct Unit unit5 = Unit_default;
    gl_weapons_dtab  = DTAB_INIT(gl_weapons_dtab,  struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);

    Unit_Init(&unit1);
    Unit_Init(&unit2);
    Unit_Init(&unit3);
    Unit_Init(&unit4);
    Unit_Init(&unit5);


    /*                              hp, str, mag, agi, dex, fth, luck, def, res, con, move, prof */
    struct Unit_stats in_caps =    {48,  14,  25,  32,  34,  28,   28,  19,  40,  15,    0, 25};
    struct Unit_stats in_stats =   {17,   6,   2,   7,   7,   7,    7,   4,   5,   6,    5,  3};
    struct Unit_stats in_growths = {60,  50,  20,  60,  70,  30,   40,  30,  20,  10,    0, 10};
    struct Unit_stats out_stats     = Unit_stats_default;
    // struct Unit_stats out_caps      = Unit_stats_default;
    struct Unit_stats *out_growths  = NULL;
    struct Inventory_item in_wpn    = Inventory_item_default;
    // struct Inventory_item out_wpn   = Inventory_item_default;

    Unit_id_set(&unit1, UNIT_ID_SILOU);
    Unit_setClassind(&unit1, UNIT_CLASS_FENCER);
    unit1.flags.sex     = UNIT_SEX_F;
    unit1.stats.caps    = in_caps;
    unit1.stats.bases   = in_stats;
    unit1.stats.current = in_stats;
    Unit_Stats_Growths_Set(&unit1, in_growths);
    unit1.level.base_exp      = 0;
    unit1.level.exp           = 0;

    tnecs_E fleuret_ent  = IES_E_CREATE_wC(world, Inventory_item_ID);
    Inventory_item *inv_fleuret         = IES_GET_C(world, fleuret_ent, Inventory_item);
    inv_fleuret->id = ITEM_ID_FLEURET;
    Unit_Item_Take(&unit1, fleuret_ent);

    tnecs_E kitchen_knife  = IES_E_CREATE_wC(world, Inventory_item_ID);
    Inventory_item *inv_kitchen_knife         = IES_GET_C(world, kitchen_knife, Inventory_item);
    inv_kitchen_knife->id = ITEM_ID_KITCHEN_KNIFE;
    Unit_Item_Take(&unit1, kitchen_knife);

    tnecs_E pot_lid  = IES_E_CREATE_wC(world, Inventory_item_ID);
    Inventory_item *inv_pot_lid         = IES_GET_C(world, pot_lid, Inventory_item);
    inv_pot_lid->id = ITEM_ID_POT_LID;
    Unit_Item_Take(&unit1, pot_lid);

    out_stats = unit1.stats.current;
    nourstest_true(s8equal(Unit_Name(&unit1), s8_literal("Silou")));
    nourstest_true(Unit_Sex(&unit1) == UNIT_SEX_F);
    nourstest_true(in_stats.hp      == out_stats.hp);
    nourstest_true(in_stats.str     == out_stats.str);
    nourstest_true(in_stats.mag     == out_stats.mag);
    nourstest_true(in_stats.agi     == out_stats.agi);
    nourstest_true(in_stats.dex     == out_stats.dex);
    nourstest_true(in_stats.luck    == out_stats.luck);
    nourstest_true(in_stats.def     == out_stats.def);
    nourstest_true(in_stats.res     == out_stats.res);
    nourstest_true(in_stats.con     == out_stats.con);
    nourstest_true(in_stats.move    == out_stats.move);
    nourstest_true(in_stats.prof    == out_stats.prof);
    out_growths = Unit_Stats_Growths(&unit1);
    nourstest_true(in_growths.hp    == out_growths->hp);
    nourstest_true(in_growths.str   == out_growths->str);
    nourstest_true(in_growths.mag   == out_growths->mag);
    nourstest_true(in_growths.agi   == out_growths->agi);
    nourstest_true(in_growths.dex   == out_growths->dex);
    nourstest_true(in_growths.luck  == out_growths->luck);
    nourstest_true(in_growths.def   == out_growths->def);
    nourstest_true(in_growths.res   == out_growths->res);
    nourstest_true(in_growths.con   == out_growths->con);
    nourstest_true(in_growths.move  == out_growths->move);
    nourstest_true(in_growths.prof  == out_growths->prof);

    // base_stats and current_stats are not the same
    unit1.stats.current.hp      += 1;
    unit1.stats.current.str     += 1;
    unit1.stats.current.mag     += 1;
    unit1.stats.current.agi     += 1;
    unit1.stats.current.dex     += 1;
    unit1.stats.current.luck    += 1;
    unit1.stats.current.def     += 1;
    unit1.stats.current.res     += 1;
    unit1.stats.current.con     += 1;
    unit1.stats.current.move    += 1;
    unit1.stats.current.prof    += 1;
    nourstest_true(unit1.stats.bases.hp   != unit1.stats.current.hp);
    nourstest_true(unit1.stats.bases.str  != unit1.stats.current.str);
    nourstest_true(unit1.stats.bases.mag  != unit1.stats.current.mag);
    nourstest_true(unit1.stats.bases.agi  != unit1.stats.current.agi);
    nourstest_true(unit1.stats.bases.dex  != unit1.stats.current.dex);
    nourstest_true(unit1.stats.bases.luck != unit1.stats.current.luck);
    nourstest_true(unit1.stats.bases.def  != unit1.stats.current.def);
    nourstest_true(unit1.stats.bases.res  != unit1.stats.current.res);
    nourstest_true(unit1.stats.bases.con  != unit1.stats.current.con);
    nourstest_true(unit1.stats.bases.move != unit1.stats.current.move);
    nourstest_true(unit1.stats.bases.prof != unit1.stats.current.prof);

    Unit_lvlUp(&unit1);
    Unit_lvlUp(&unit1);
    out_stats = unit1.stats.bases;
    nourstest_true(in_stats.hp      == out_stats.hp);
    nourstest_true(in_stats.str     == out_stats.str);
    nourstest_true(in_stats.mag     == out_stats.mag);
    nourstest_true(in_stats.agi     == out_stats.agi);
    nourstest_true(in_stats.dex     == out_stats.dex);
    nourstest_true(in_stats.luck    == out_stats.luck);
    nourstest_true(in_stats.def     == out_stats.def);
    nourstest_true(in_stats.res     == out_stats.res);
    nourstest_true(in_stats.con     == out_stats.con);
    nourstest_true(in_stats.move    == out_stats.move);
    nourstest_true(in_stats.prof    == out_stats.prof);
    out_stats = unit1.stats.current;
    nourstest_true(in_stats.hp      <= out_stats.hp);
    nourstest_true(in_stats.str     <= out_stats.str);
    nourstest_true(in_stats.mag     <= out_stats.mag);
    nourstest_true(in_stats.agi     <= out_stats.agi);
    nourstest_true(in_stats.dex     <= out_stats.dex);
    nourstest_true(in_stats.luck    <= out_stats.luck);
    nourstest_true(in_stats.def     <= out_stats.def);
    nourstest_true(in_stats.res     <= out_stats.res);
    nourstest_true(in_stats.con     <= out_stats.con);
    nourstest_true(in_stats.move    <= out_stats.move);
    nourstest_true(in_stats.prof    <= out_stats.prof);
    if (PHYSFS_stat("saves", NULL) == 0) {
        PHYSFS_mkdir("saves");
    }
    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "unit_test.json")), &unit1, false);
    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "unit_test.binou")), &unit1, false);
    jsonio_readJSON(s8_literal(PATH_JOIN("saves", "unit_test.json")), &unit4);
    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "unit_rewrite.json")), &unit4, false);
    nourstest_true(PHYSFS_exists(PATH_JOIN("saves", "unit_test.json"))      != 0);
    nourstest_true(PHYSFS_exists(PATH_JOIN("saves", "unit_rewrite.json"))   != 0);
    nourstest_true(Filesystem_fequal(PATH_JOIN("saves", "unit_test.json"),
                                     PATH_JOIN("saves", "unit_rewrite.json")));
    Unit_Free(&unit1);
    Unit_Free(&unit2);
    Unit_Free(&unit3);
    Unit_Free(&unit4);
    Unit_Free(&unit5);

    /* --- Free --- */
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    tnecs_finale(&world);
    gl_world = NULL;
}

void test_growth(void) {
    rng_sequence_breaker_miss_growth = true;
    struct Unit Silou = Unit_default;
    Unit_Init(&Silou);
    /*                              hp, str, mag, agi, dex, fth, luck, def, res, con, move, prof */
    struct Unit_stats in_stats = {  17,   6,   2,   7,   7,   7,    7,   4,   5,   6,   4,    2};
    struct Unit_stats in_caps = {   48,  14,  25,  32,  34,  28,   28,  19,  40,  15,   20,  25};
    struct Unit_stats in_growths = {60,  50,  20,  60,  70,  30,   40,  30,  20,  10,   10,  20};

    // struct Unit_stats out_stats     = Unit_stats_default;
    // struct Unit_stats out_caps      = Unit_stats_default;

    Silou.level.exp     = 0; /* lvl 1 */
    Silou.stats.caps    = in_caps;
    Silou.stats.bases   = in_stats;
    Silou.stats.current = in_stats;
    Unit_Stats_Growths_Set(&Silou, in_growths);

    URN_debug       = 11;
    /* Testing the Sequence Breaker */
    /* After a sequence of 1 no level up, con and move SHould level up because of increased rate.
    /* First level up: con and move don't grow*/
    Unit_lvlUp(&Silou);
    /*                              hp, str, mag, agi, dex, fth, luck, def, res, con, move, prof */
    struct Unit_stats temp_growths = {1,   1,   1,   1,   1,   1,   1,    1,   1,   0,   0,  1};

    struct Unit_stats *grown = Unit_Stats_Grown(&Silou);
    nourstest_true(grown[0].hp      == temp_growths.hp);
    nourstest_true(grown[0].str     == temp_growths.str);
    nourstest_true(grown[0].agi     == temp_growths.agi);
    nourstest_true(grown[0].dex     == temp_growths.dex);
    nourstest_true(grown[0].fth     == temp_growths.fth);
    nourstest_true(grown[0].luck    == temp_growths.luck);
    nourstest_true(grown[0].def     == temp_growths.def);
    nourstest_true(grown[0].res     == temp_growths.res);
    nourstest_true(grown[0].con     == temp_growths.con);  /* 0 at first */
    nourstest_true(grown[0].move    == temp_growths.move); /* 0 at first */
    nourstest_true(grown[0].prof    == temp_growths.prof);
    nourstest_true(DARR_NUM(grown)  == true);

    Unit_stats *growths = Unit_Stats_Growths(&Silou);
    nourstest_true(growths->move == 10);
    nourstest_true(growths->con  == 10);
    nourstest_true(growths->prof == 20);

    nourstest_true(Silou.rng_sequence.res.len  == false);
    nourstest_true(Silou.rng_sequence.move.len == true);
    nourstest_true(Silou.rng_sequence.con.len  == true);
    nourstest_true(Silou.rng_sequence.prof.len == false);

    /* Second level up: con and move grow, cause of increased rate */
    Unit_lvlUp(&Silou);

    grown = Unit_Stats_Grown(&Silou);
    nourstest_true(DARR_NUM(grown) == 2);
    nourstest_true(grown[1].hp      == temp_growths.hp);
    nourstest_true(grown[1].str     == temp_growths.str);
    nourstest_true(grown[1].agi     == temp_growths.agi);
    nourstest_true(grown[1].dex     == temp_growths.dex);
    nourstest_true(grown[1].fth     == temp_growths.fth);
    nourstest_true(grown[1].luck    == temp_growths.luck);
    nourstest_true(grown[1].def     == temp_growths.def);
    nourstest_true(grown[1].res     == temp_growths.res);
    nourstest_true(grown[1].con     ==
                   true); /* grows now, SB eff rate is 10*1.2 = 12 > 11  */
    nourstest_true(grown[1].move    ==
                   true); /* grows now, SB eff rate is 10*1.2 = 12 > 11  */
    nourstest_true(grown[1].prof    == temp_growths.prof);

    nourstest_true(Silou.rng_sequence.con.len    == false);
    nourstest_true(Silou.rng_sequence.move.len   == false);
    nourstest_true(Silou.rng_sequence.prof.len   == false);

    nourstest_true(Silou.rng_sequence.prof.eff_rate == in_growths.prof);
    nourstest_true(Silou.rng_sequence.move.eff_rate  > in_growths.move);
    nourstest_true(Silou.rng_sequence.con.eff_rate   > in_growths.con);

    /* Runing of sequence breaker: no more sequences, no more increasing rates */
    rng_sequence_breaker_miss_growth = false;
    Unit_lvlUp(&Silou);
    grown = Unit_Stats_Grown(&Silou);
    nourstest_true(DARR_NUM(grown) == 3);
    nourstest_true(grown[2].hp      == temp_growths.hp);
    nourstest_true(grown[2].str     == temp_growths.str);
    nourstest_true(grown[2].agi     == temp_growths.agi);
    nourstest_true(grown[2].dex     == temp_growths.dex);
    nourstest_true(grown[2].fth     == temp_growths.fth);
    nourstest_true(grown[2].luck    == temp_growths.luck);
    nourstest_true(grown[2].def     == temp_growths.def);
    nourstest_true(grown[2].res     == temp_growths.res);
    nourstest_true(grown[2].con     == temp_growths.con);
    nourstest_true(grown[2].move    == temp_growths.move);
    nourstest_true(grown[2].prof    == temp_growths.prof);

    nourstest_true(Silou.rng_sequence.res.len  == false);
    nourstest_true(Silou.rng_sequence.con.len  == false);
    nourstest_true(Silou.rng_sequence.move.len == false);
    nourstest_true(Silou.rng_sequence.prof.len == false);

    Unit_lvlUp(&Silou);
    nourstest_true(DARR_NUM(grown)  == 4);
    nourstest_true(grown[3].con     == temp_growths.con);
    nourstest_true(grown[3].move    == temp_growths.move);
    nourstest_true(grown[3].prof    == temp_growths.prof);
    nourstest_true(Silou.rng_sequence.prof.len      == false);
    nourstest_true(Silou.rng_sequence.move.len      == false);
    nourstest_true(Silou.rng_sequence.con.len       == false);

    Unit_Free(&Silou);
}

void test_reinforcements(void) {
    struct Unit Corsair = Unit_default;
    struct Reinforcement reinf = Reinforcement_default;
    reinf.levelups = 2;
    Unit_Init(&Corsair);

    Unit_Reinforcement_Levelups(&Corsair, &reinf);
    struct Unit_stats *grown = Unit_Stats_Grown(&Corsair);

    nourstest_true(DARR_NUM(grown) == reinf.levelups);
    Unit_Reinforcement_Levelups(&Corsair, &reinf);
    nourstest_true(DARR_NUM(grown) == reinf.levelups);

    Unit_Free(&Corsair);
}

void test_bonus_decay(void) {
    struct Bonus_Stats bonus1 = Bonus_Stats_default;
    struct Bonus_Stats bonus2 = Bonus_Stats_default;
    struct Bonus_Stats bonus3 = Bonus_Stats_default;
    struct Bonus_Stats bonus4 = Bonus_Stats_default;
    struct Bonus_Stats bonus5 = Bonus_Stats_default;

    bonus1.turns = 3;
    bonus2.turns = 0;
    bonus3.turns = 3;
    bonus4.turns = 1;
    bonus5.turns = 2;
    struct Unit Silou   = Unit_default;
    Silou.stats.bonus_stack   = DARR_INIT(Silou.stats.bonus_stack,  struct Bonus_Stats, 2);
    Unit_Bonus_Add(&Silou, bonus1);
    Unit_Bonus_Add(&Silou, bonus2);
    Unit_Bonus_Add(&Silou, bonus3);
    Unit_Bonus_Add(&Silou, bonus4);
    Unit_Bonus_Add(&Silou, bonus5);
    nourstest_true(DARR_NUM(Silou.stats.bonus_stack) == 5);
    nourstest_true(Silou.stats.bonus_stack[0].turns == 3);
    nourstest_true(Silou.stats.bonus_stack[1].turns == false);
    nourstest_true(Silou.stats.bonus_stack[2].turns == 3);
    nourstest_true(Silou.stats.bonus_stack[3].turns == true);
    nourstest_true(Silou.stats.bonus_stack[4].turns == 2);

    Unit_Bonus_Persistent_Decay(&Silou);
    nourstest_true(DARR_NUM(Silou.stats.bonus_stack) == 4);
    nourstest_true(Silou.stats.bonus_stack[0].turns == 2);
    nourstest_true(Silou.stats.bonus_stack[1].turns == 2);
    nourstest_true(Silou.stats.bonus_stack[2].turns == false);
    nourstest_true(Silou.stats.bonus_stack[3].turns == true);

    Unit_Bonus_Persistent_Decay(&Silou);
    nourstest_true(DARR_NUM(Silou.stats.bonus_stack) == 3);
    nourstest_true(Silou.stats.bonus_stack[0].turns == true);
    nourstest_true(Silou.stats.bonus_stack[1].turns == true);
    nourstest_true(Silou.stats.bonus_stack[2].turns == false);

    Unit_Bonus_Persistent_Decay(&Silou);

    nourstest_true(DARR_NUM(Silou.stats.bonus_stack) == 2);
    nourstest_true(Silou.stats.bonus_stack[0].turns == false);
    nourstest_true(Silou.stats.bonus_stack[1].turns == false);

    Unit_Bonus_Persistent_Decay(&Silou);
    nourstest_true(DARR_NUM(Silou.stats.bonus_stack) == false);

    // TODO test instant aura decay.
    struct Bonus_Stats bonus_instant1 = Bonus_Stats_default;
    struct Bonus_Stats bonus_instant2 = Bonus_Stats_default;
    struct Bonus_Stats bonus_instant3 = Bonus_Stats_default;

    bonus_instant1.turns = -1;
    bonus_instant2.turns =  0;
    bonus_instant3.turns = -2;
    Unit_Bonus_Add(&Silou, bonus_instant1);
    Unit_Bonus_Add(&Silou, bonus_instant2);
    Unit_Bonus_Add(&Silou, bonus_instant3);
    nourstest_true(DARR_NUM(Silou.stats.bonus_stack) == 3);
    Unit_Bonus_Instant_Decay(&Silou);
    nourstest_true(DARR_NUM(Silou.stats.bonus_stack) == true);

    Unit_Bonus_Add(&Silou, bonus_instant1);
    Unit_Bonus_Add(&Silou, bonus_instant3);
    Unit_Bonus_Persistent_Decay(&Silou);
    nourstest_true(DARR_NUM(Silou.stats.bonus_stack) == 2);

    /* SDL_free */
    DARR_FREE(Silou.stats.bonus_stack);
}

void test_bonus_stats(void) {
    struct Bonus_Stats bonus1 = Bonus_Stats_default;
    struct Unit Silou = Unit_default;
    //                              hp, str, mag, agi, dex, fth, luck, def, res, con, move
    struct Unit_stats unit_stats    = {1,  6,  2,  7,  7, 7,  7,  4,  5,  6, 5};
    Silou.stats.bases    = unit_stats;
    Silou.stats.current = unit_stats;

    /* Adding bonus stats*/
    struct Unit_stats bonus_stats   = {1,  1,  1,  1,  1, 1,  1,  1,  1,  1, 1};
    bonus1.unit_stats = bonus_stats;
    Silou.stats.bonus_stack   = DARR_INIT(Silou.stats.bonus_stack,  struct Bonus_Stats, 2);
    Unit_Bonus_Add(&Silou, bonus1);

    struct Unit_stats effective_stats = Unit_effectiveStats(&Silou);

    nourstest_true(effective_stats.hp   == (Silou.stats.current.hp      + bonus1.unit_stats.hp));
    nourstest_true(effective_stats.str  == (Silou.stats.current.str     + bonus1.unit_stats.str));
    nourstest_true(effective_stats.mag  == (Silou.stats.current.mag     + bonus1.unit_stats.mag));
    nourstest_true(effective_stats.agi  == (Silou.stats.current.agi     + bonus1.unit_stats.agi));
    nourstest_true(effective_stats.dex  == (Silou.stats.current.dex     + bonus1.unit_stats.dex));
    nourstest_true(effective_stats.fth  == (Silou.stats.current.fth     + bonus1.unit_stats.fth));
    nourstest_true(effective_stats.luck == (Silou.stats.current.luck    + bonus1.unit_stats.luck));
    nourstest_true(effective_stats.def  == (Silou.stats.current.def     + bonus1.unit_stats.def));
    nourstest_true(effective_stats.res  == (Silou.stats.current.res     + bonus1.unit_stats.res));
    nourstest_true(effective_stats.con  == (Silou.stats.current.con     + bonus1.unit_stats.con));
    nourstest_true(effective_stats.move == (Silou.stats.current.move    + bonus1.unit_stats.move));
    nourstest_true(effective_stats.prof == (Silou.stats.current.prof    + bonus1.unit_stats.prof));

    /* Adding bonus_stats2 */
    struct Unit_stats bonus_stats2   = {0,  2,  3,  4,  5, 6,  7,  8,  9,  10, 11};
    bonus1.unit_stats   = bonus_stats2;
    bonus1.turns        = 1;
    Unit_Bonus_Add(&Silou, bonus1);

    effective_stats = Unit_effectiveStats(&Silou);

    nourstest_true(effective_stats.hp   == (Silou.stats.current.hp      + bonus_stats.hp    +
                                            bonus_stats2.hp));
    nourstest_true(effective_stats.str  == (Silou.stats.current.str     + bonus_stats.str   +
                                            bonus_stats2.str));
    nourstest_true(effective_stats.mag  == (Silou.stats.current.mag     + bonus_stats.mag   +
                                            bonus_stats2.mag));
    nourstest_true(effective_stats.agi  == (Silou.stats.current.agi     + bonus_stats.agi   +
                                            bonus_stats2.agi));
    nourstest_true(effective_stats.dex  == (Silou.stats.current.dex     + bonus_stats.dex   +
                                            bonus_stats2.dex));
    nourstest_true(effective_stats.fth  == (Silou.stats.current.fth     + bonus_stats.fth   +
                                            bonus_stats2.fth));
    nourstest_true(effective_stats.luck == (Silou.stats.current.luck    + bonus_stats.luck  +
                                            bonus_stats2.luck));
    nourstest_true(effective_stats.def  == (Silou.stats.current.def     + bonus_stats.def   +
                                            bonus_stats2.def));
    nourstest_true(effective_stats.res  == (Silou.stats.current.res     + bonus_stats.res   +
                                            bonus_stats2.res));
    nourstest_true(effective_stats.con  == (Silou.stats.current.con     + bonus_stats.con   +
                                            bonus_stats2.con));
    nourstest_true(effective_stats.move == (Silou.stats.current.move    + bonus_stats.move  +
                                            bonus_stats2.move));
    nourstest_true(effective_stats.prof == (Silou.stats.current.prof    + bonus_stats.prof  +
                                            bonus_stats2.prof));

    Unit_Bonus_Persistent_Decay(&Silou);

    effective_stats = Unit_effectiveStats(&Silou);

    nourstest_true(effective_stats.hp   == (Silou.stats.current.hp      + bonus_stats2.hp));
    nourstest_true(effective_stats.str  == (Silou.stats.current.str     + bonus_stats2.str));
    nourstest_true(effective_stats.mag  == (Silou.stats.current.mag     + bonus_stats2.mag));
    nourstest_true(effective_stats.agi  == (Silou.stats.current.agi     + bonus_stats2.agi));
    nourstest_true(effective_stats.dex  == (Silou.stats.current.dex     + bonus_stats2.dex));
    nourstest_true(effective_stats.fth  == (Silou.stats.current.fth     + bonus_stats2.fth));
    nourstest_true(effective_stats.luck == (Silou.stats.current.luck    + bonus_stats2.luck));
    nourstest_true(effective_stats.def  == (Silou.stats.current.def     + bonus_stats2.def));
    nourstest_true(effective_stats.res  == (Silou.stats.current.res     + bonus_stats2.res));
    nourstest_true(effective_stats.con  == (Silou.stats.current.con     + bonus_stats2.con));
    nourstest_true(effective_stats.move == (Silou.stats.current.move    + bonus_stats2.move));
    nourstest_true(effective_stats.prof == (Silou.stats.current.prof    + bonus_stats2.prof));

    Unit_Bonus_Persistent_Decay(&Silou);

    effective_stats = Unit_effectiveStats(&Silou);

    nourstest_true(effective_stats.hp   == (Silou.stats.current.hp));
    nourstest_true(effective_stats.str  == (Silou.stats.current.str));
    nourstest_true(effective_stats.mag  == (Silou.stats.current.mag));
    nourstest_true(effective_stats.agi  == (Silou.stats.current.agi));
    nourstest_true(effective_stats.dex  == (Silou.stats.current.dex));
    nourstest_true(effective_stats.fth  == (Silou.stats.current.fth));
    nourstest_true(effective_stats.luck == (Silou.stats.current.luck));
    nourstest_true(effective_stats.def  == (Silou.stats.current.def));
    nourstest_true(effective_stats.res  == (Silou.stats.current.res));
    nourstest_true(effective_stats.con  == (Silou.stats.current.con));
    nourstest_true(effective_stats.move == (Silou.stats.current.move));
    nourstest_true(effective_stats.prof == (Silou.stats.current.prof));

    /* SDL_free */
    DARR_FREE(Silou.stats.bonus_stack);
}

void test_canEquip_OneHand() {
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;

#include "register/components.h"

    struct Unit Silou = Unit_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab, struct Item);
    Unit_Init(&Silou);

    Unit_setClassind(&Silou, UNIT_CLASS_FENCER);
    Item_Load(ITEM_ID_FLEURET);
    struct Weapon *weapon = DTAB_GET(gl_weapons_dtab, ITEM_ID_FLEURET);
    /* Try to equip a one hand weapon when already in other hand */
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;

    Unit_Unequip(&Silou, UNIT_HAND_LEFT);
    Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
    tnecs_E *silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, ITEM1);
    SDL_assert(TNECS_NULL != Unit_InvItem_Entity(&Silou, ITEM1));

    SDL_assert(Unit_Id_Equipment(&Silou, ITEM1) == ITEM_ID_FLEURET);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, ITEM2);
    SDL_assert(Unit_Id_Equipment(&Silou, ITEM2) == ITEM_ID_FLEURET);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, ITEM3);
    SDL_assert(Unit_Id_Equipment(&Silou, ITEM3) == ITEM_ID_FLEURET);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, ITEM4);
    SDL_assert(Unit_Id_Equipment(&Silou, ITEM4) == ITEM_ID_FLEURET);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, ITEM5);
    SDL_assert(Unit_Id_Equipment(&Silou, ITEM5) == ITEM_ID_FLEURET);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, ITEM6);
    SDL_assert(Unit_Id_Equipment(&Silou, ITEM6) == ITEM_ID_FLEURET);

    i32 mode = TWO_HAND_EQ_MODE_STRICT;
    // Left handed Weapon
    Weapon_Handedness_Set(weapon, WEAPON_HAND_LEFT);
    nourstest_true( Unit_canEquip_OneHand(&Silou,  ITEM1, UNIT_HAND_LEFT, mode));
    nourstest_true(!Unit_canEquip_OneHand(&Silou,  ITEM1, UNIT_HAND_RIGHT, mode));

    // Right handed Weapon
    Weapon_Handedness_Set(weapon, WEAPON_HAND_RIGHT);
    nourstest_true(!Unit_canEquip_OneHand(&Silou, ITEM1, UNIT_HAND_LEFT, mode));
    nourstest_true( Unit_canEquip_OneHand(&Silou, ITEM1, UNIT_HAND_RIGHT, mode));

    Weapon_Handedness_Set(weapon, WEAPON_HAND_ONE);
    for (i32 eq = ITEM1; eq < SOTA_EQUIPMENT_SIZE; eq++) {
        // One handed Weapon already equipped in left hand
        Unit_Equip(&Silou, UNIT_HAND_LEFT, eq);
        Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
        nourstest_true( Unit_canEquip_OneHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true(!Unit_canEquip_OneHand(&Silou, eq, UNIT_HAND_RIGHT, mode));

        // One handed Weapon already equipped in right hand
        Unit_Unequip(&Silou, UNIT_HAND_LEFT);
        Unit_Equip(&Silou, UNIT_HAND_RIGHT, eq);
        nourstest_true(!Unit_canEquip_OneHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true( Unit_canEquip_OneHand(&Silou, eq, UNIT_HAND_RIGHT, mode));
    }

    // Any handed Weapon
    Weapon_Handedness_Set(weapon, WEAPON_HAND_ANY);
    for (i32 eq = ITEM1; eq < SOTA_EQUIPMENT_SIZE; eq++) {
        // One handed Weapon already equipped in left hand
        Unit_Equip(&Silou, UNIT_HAND_LEFT, eq);
        Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
        nourstest_true(Unit_canEquip_OneHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true(Unit_canEquip_OneHand(&Silou, eq, UNIT_HAND_RIGHT, mode));

        // One handed Weapon already equipped in right hand
        Unit_Unequip(&Silou, UNIT_HAND_LEFT);
        Unit_Equip(&Silou, UNIT_HAND_RIGHT, eq);
        nourstest_true(Unit_canEquip_OneHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true(Unit_canEquip_OneHand(&Silou, eq, UNIT_HAND_RIGHT, mode));
    }

    // Two handed Weapon
    Weapon_Handedness_Set(weapon, WEAPON_HAND_TWO);
    for (i32 eq = ITEM1; eq < SOTA_EQUIPMENT_SIZE; eq++) {
        // One handed Weapon already equipped in left hand
        Unit_Equip(&Silou, UNIT_HAND_LEFT, eq);
        Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
        nourstest_true(Unit_canEquip_OneHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true(Unit_canEquip_OneHand(&Silou, eq, UNIT_HAND_RIGHT, mode));

        // One handed Weapon already equipped in right hand
        Unit_Unequip(&Silou, UNIT_HAND_LEFT);
        Unit_Equip(&Silou, UNIT_HAND_RIGHT, eq);
        nourstest_true(Unit_canEquip_OneHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true(Unit_canEquip_OneHand(&Silou, eq, UNIT_HAND_RIGHT, mode));
    }

    /* --- Free --- */
    Unit_Free(&Silou);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    tnecs_finale(&world);
    gl_world = NULL;
}

void test_canEquip_TwoHand() {
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;

#include "register/components.h"

    struct Unit Silou = Unit_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab, struct Item);
    Unit_Init(&Silou);
    Unit_setClassind(&Silou, UNIT_CLASS_FENCER);
    Item_Load(ITEM_ID_FLEURET);
    Item_Load(ITEM_ID_RAPIERE);
    Item_Load(ITEM_ID_HEAL);
    Weapon *weapon = DTAB_GET(gl_weapons_dtab, ITEM_ID_FLEURET);

    /* Try to equip a one hand weapon when already in other hand */
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;
    Unit_Unequip(&Silou, UNIT_HAND_LEFT);
    Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
    tnecs_E *silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, ITEM1);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, ITEM2);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, ITEM3);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, ITEM4);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, ITEM5);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, ITEM6);

    /* Left handed Weapon */
    Weapon_Handedness_Set(weapon, WEAPON_HAND_LEFT);
    i32 mode = TWO_HAND_EQ_MODE_STRICT;
    nourstest_true(Unit_canEquip_TwoHand(&Silou,  ITEM1, UNIT_HAND_LEFT, mode));
    nourstest_true(Unit_canEquip_TwoHand(&Silou,  ITEM1, UNIT_HAND_RIGHT, mode));

    /* Right handed Weapon */
    Weapon_Handedness_Set(weapon, WEAPON_HAND_RIGHT);
    nourstest_true(Unit_canEquip_TwoHand(&Silou, ITEM1, UNIT_HAND_LEFT, mode));
    nourstest_true(Unit_canEquip_TwoHand(&Silou, ITEM1, UNIT_HAND_RIGHT, mode));

    Weapon_Handedness_Set(weapon, WEAPON_HAND_ONE);
    for (i32 eq = ITEM1; eq < SOTA_EQUIPMENT_SIZE; eq++) {
        // One handed Weapon already equipped in left hand
        Unit_Equip(&Silou, UNIT_HAND_LEFT, eq);
        Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_RIGHT, mode));

        /* One handed Weapon already equipped in right hand */
        Unit_Unequip(&Silou, UNIT_HAND_LEFT);
        Unit_Equip(&Silou, UNIT_HAND_RIGHT, eq);
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_RIGHT, mode));
    }

    /* --- Any handed Weapon --- */
    Weapon_Handedness_Set(weapon, WEAPON_HAND_ANY);
    for (i32 eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; eq++) {
        /* One handed Weapon already equipped in left hand */
        int eq_opposite = SOTA_EQUIPMENT_SIZE - eq + ITEM1;
        Unit_Equip(&Silou, UNIT_HAND_LEFT, eq_opposite);
        Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_RIGHT, mode));

        /* One handed Weapon already equipped in right hand */
        Unit_Unequip(&Silou, UNIT_HAND_LEFT);
        Unit_Equip(&Silou, UNIT_HAND_RIGHT, eq_opposite);
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_RIGHT, mode));
    }

    /* --- Two handed Weapon --- */
    Weapon_Handedness_Set(weapon, WEAPON_HAND_TWO);
    for (i32 eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; eq++) {
        // Other weapon weapon equipped in left hand
        int eq_opposite = SOTA_EQUIPMENT_SIZE - eq + ITEM1;
        Unit_Equip(&Silou, UNIT_HAND_LEFT, eq_opposite);
        Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
        nourstest_true( Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true(!Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_RIGHT, mode));

        /* Other weapon weapon equipped in right hand */
        Unit_Unequip(&Silou, UNIT_HAND_LEFT);
        Unit_Equip(&Silou, UNIT_HAND_RIGHT, eq_opposite);
        nourstest_true(!Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true( Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_RIGHT, mode));
    }

    /* Try to equip a two handed weapon when already in different hands */
    struct Weapon *weapon2 = DTAB_GET(gl_weapons_dtab, ITEM_ID_RAPIERE);
    Weapon_Handedness_Set(weapon, WEAPON_HAND_TWO);
    Weapon_Handedness_Set(weapon2, WEAPON_HAND_TWO);

    /* Try to equip a one hand weapon when already in other hand */
    Unit_Equip(&Silou, UNIT_HAND_LEFT, ITEM1);
    Unit_Unequip(&Silou, UNIT_HAND_RIGHT);

    silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, ITEM1);
    TEST_SET_EQUIPMENT(world, ITEM_ID_RAPIERE, ITEM2);

    nourstest_true(!Unit_canEquip_TwoHand(&Silou, ITEM2, UNIT_HAND_RIGHT, mode));
    nourstest_true( Unit_canEquip_TwoHand(&Silou, ITEM1, UNIT_HAND_RIGHT, mode));

    /* Try to equip a one hand weapon when already in other hand with type
         that can't be twohanded */
    Unit_Equip(&Silou,      UNIT_HAND_LEFT, ITEM1);
    Unit_Unequip(&Silou,    UNIT_HAND_RIGHT);
    silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, ITEM1);
    TEST_SET_EQUIPMENT(world, ITEM_ID_RAPIERE, ITEM2);

    Weapon_Handedness_Set(weapon, WEAPON_HAND_ANY);
    Weapon_Handedness_Set(weapon2, WEAPON_HAND_ANY);
    weapon->item.type.top  = ITEM_TYPE_ELEMENTAL;
    weapon->item.type.top  = ITEM_TYPE_ANGELIC;
    nourstest_true( Unit_canEquip_TwoHand(&Silou, ITEM2, UNIT_HAND_RIGHT, mode));
    nourstest_true( Unit_canEquip_OneHand(&Silou, ITEM1, UNIT_HAND_RIGHT, mode));

    /* Try to equip staff  */
    Unit_Unequip(&Silou, UNIT_HAND_LEFT);
    Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
    silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_HEAL, ITEM1);
    nourstest_true( Unit_canEquip_TwoHand(&Silou, ITEM1, UNIT_HAND_RIGHT, mode));
    nourstest_true( Unit_canEquip_TwoHand(&Silou, ITEM1, UNIT_HAND_LEFT, mode));

    /* --- Free --- */
    Unit_Free(&Silou);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    tnecs_finale(&world);
    gl_world = NULL;
}

void test_canEquip_Users(void) {
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;

#include "register/components.h"

    struct Unit Silou = Unit_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab, struct Item);

    Unit_Init(&Silou);
    int id = ITEM_ID_FLEURET;
    Item_Load(id);
    Unit_id_set(&Silou, UNIT_ID_SILOU);

    struct Weapon *weapon = DTAB_GET(gl_weapons_dtab, id);

    int eq = 0;
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;
    tnecs_E *silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, id, eq);
    if (weapon->item.users.id != NULL) {
        DARR_FREE(weapon->item.users.id);
        weapon->item.users.id    = NULL;
    }

    nourstest_true(Unit_canEquip_Users(&Silou, id));
    u16 *users = DARR_INIT(users, u16, 4);
    users[0] = UNIT_ID_ERWIN;
    users[1] = UNIT_ID_ERWIN;
    users[2] = UNIT_ID_ERWIN;
    users[3] = UNIT_ID_ERWIN;
    DARR_NUM(users) = 4;
    weapon->item.users.id = users;
    nourstest_true(!Unit_canEquip_Users(&Silou, id));

    users[3] = UNIT_ID_SILOU;
    nourstest_true( Unit_canEquip_Users(&Silou, id));

    /* --- Free --- */
    Unit_Free(&Silou);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    tnecs_finale(&world);
    gl_world = NULL;
}

void test_canEquip_Archetype(void) {
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;

#include "register/components.h"

    struct Unit Silou = Unit_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab,   struct Item);

    Unit_Init(&Silou);
    int id = ITEM_ID_FLEURET;
    Item_Load(id);
    Unit_id_set(&Silou, UNIT_ID_SILOU);

    struct Weapon *weapon = DTAB_GET(gl_weapons_dtab, id);

    int eq = 0;
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;
    tnecs_E *silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, id, eq);

    nourstest_true( Unit_canEquip_Archetype(id, ITEM_ARCHETYPE_NULL));
    nourstest_true( !Unit_canEquip_Archetype(id, ITEM_ARCHETYPE_ITEM));
    nourstest_true( Unit_canEquip_Archetype(id, ITEM_ARCHETYPE_WEAKHAND));
    nourstest_true( Unit_canEquip_Archetype(id, ITEM_ARCHETYPE_WEAPON));
    nourstest_true(!Unit_canEquip_Archetype(id, ITEM_ARCHETYPE_SHIELD));
    nourstest_true(!Unit_canEquip_Archetype(id, ITEM_ARCHETYPE_STAFF));

    /* --- Free --- */
    Unit_Free(&Silou);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    tnecs_finale(&world);
    gl_world = NULL;
}

void test_canEquip(void) {
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;

#include "register/components.h"

    //  - Does the loadout make sense for unit/class/selection
    struct Unit Silou = Unit_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);

    Unit_Init(&Silou);
    Unit_id_set(&Silou, UNIT_ID_SILOU);
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;

    /* --- Can prof everything --- */
    Silou.stats.current.prof = 100;

    /* --- Staff user that can't twohand --- */
    Unit_setClassind(&Silou, UNIT_CLASS_VESTAL);
    Silou.flags.equippable = ITEM_TYPE_STAFF;
    // Unit_Equip(&Silou, UNIT_HAND_LEFT, 0);
    // Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
    tnecs_E *silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET,  ITEM1);
    TEST_SET_EQUIPMENT(world, ITEM_ID_RAPIERE,  ITEM2);
    TEST_SET_EQUIPMENT(world, ITEM_ID_HEAL,     ITEM3);

    /* Nothing in either hand */
    canEquip can_equip  = canEquip_default;
    can_equip.hand      = UNIT_HAND_LEFT;

    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));

    canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, ITEM3);
    nourstest_true( Unit_canEquip(&Silou, can_equip));

    /* Something in either hand */
    canEquip_Loadout(&can_equip, UNIT_HAND_LEFT, ITEM1);
    canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, ITEM2);

    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));

    /* Something in either hand */
    canEquip_Loadout(&can_equip, UNIT_HAND_LEFT,  ITEM1);
    canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, ITEM2);

    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));

    /* --- Staff user that can twohand with skill --- */
    Silou.flags.skills |= PASSIVE_SKILL_STAFF_ONE_HAND;

    /* -- Stronghand NOT equipped -- */
    can_equip.hand      = UNIT_HAND_LEFT;
    canEquip_Loadout_None(&can_equip, UNIT_HAND_LEFT);
    canEquip_Loadout_None(&can_equip, UNIT_HAND_RIGHT);

    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true( Unit_canEquip(&Silou, can_equip));

    /* -- Stronghand equipped -- */
    canEquip_Loadout(&can_equip, UNIT_HAND_LEFT,  ITEM3);
    canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, ITEM3);

    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true( Unit_canEquip(&Silou, can_equip));

    /* --- Mage that can't two hand --- */
    Unit_setClassind(&Silou, UNIT_CLASS_MAGE);
    Silou.flags.equippable = ITEM_TYPE_ANGELIC | ITEM_TYPE_DEMONIC | ITEM_TYPE_ELEMENTAL;

    silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_WIND_SPEAR,           ITEM1);
    TEST_SET_EQUIPMENT(world, ITEM_ID_DOWNFALL,             ITEM2);
    TEST_SET_EQUIPMENT(world, ITEM_ID_SILVERLIGHT_SPEAR,    ITEM3);

    /* -- Stronghand NOT equipped -- */
    canEquip_Loadout_None(&can_equip, UNIT_HAND_LEFT);
    canEquip_Loadout_None(&can_equip, UNIT_HAND_RIGHT);

    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true( Unit_canEquip(&Silou, can_equip));

    /* -- Stronghand equipped -- */
    canEquip_Loadout(&can_equip, UNIT_HAND_LEFT,  ITEM1);
    canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, ITEM1);
    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));

    canEquip_Loadout(&can_equip, UNIT_HAND_LEFT,  ITEM2);
    canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, ITEM2);
    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));

    /* --- Mage that can two hand due to skill --- */
    Silou.flags.skills = PASSIVE_SKILL_MAGIC_ONE_HAND;
    /* -- Stronghand NOT equipped -- */
    canEquip_Loadout_None(&can_equip, UNIT_HAND_LEFT);
    canEquip_Loadout_None(&can_equip, UNIT_HAND_RIGHT);

    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true( Unit_canEquip(&Silou, can_equip));

    /* -- Stronghand equipped -- */
    canEquip_Loadout(&can_equip, UNIT_HAND_LEFT,  ITEM1);
    canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, ITEM1);
    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));

    canEquip_Loadout(&can_equip, UNIT_HAND_LEFT,  ITEM1);
    canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, ITEM1);
    can_equip.two_hands_mode = TWO_HAND_EQ_MODE_STRICT;
    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    can_equip.two_hands_mode = TWO_HAND_EQ_MODE_LOOSE;
    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true( Unit_canEquip(&Silou, can_equip));

    can_equip.two_hands_mode = TWO_HAND_EQ_MODE_LOOSE;
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    can_equip.two_hands_mode = TWO_HAND_EQ_MODE_STRICT;
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));

    can_equip.two_hands_mode = TWO_HAND_EQ_MODE_LOOSE;
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    can_equip.two_hands_mode = TWO_HAND_EQ_MODE_STRICT;
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));

    /* --- Normal physical soldier --- */
    Silou.flags.equippable = ITEM_TYPE_LANCE | ITEM_TYPE_SHIELD | ITEM_TYPE_SWORD;
    silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_IRON_LANCE,       ITEM1);
    TEST_SET_EQUIPMENT(world, ITEM_ID_WOODEN_SHIELD,    ITEM2);
    TEST_SET_EQUIPMENT(world, ITEM_ID_WRATH_LANCE,      ITEM3);

    /* -- Stronghand NOT equipped -- */
    canEquip_Loadout_None(&can_equip, UNIT_HAND_LEFT);
    canEquip_Loadout_None(&can_equip, UNIT_HAND_RIGHT);

    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true( Unit_canEquip(&Silou, can_equip));

    /* -- Stronghand equipped -- */
    canEquip_Loadout(&can_equip, UNIT_HAND_LEFT,  ITEM1);
    canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, ITEM1);

    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true( Unit_canEquip(&Silou, can_equip));

    /* -- Mage test -- */
    Silou.flags.equippable = ITEM_TYPE_ELEMENTAL | ITEM_TYPE_SHIELD | ITEM_TYPE_LANCE;
    silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_BALL_LIGHTNING,   ITEM1);
    TEST_SET_EQUIPMENT(world, ITEM_ID_SILVERLIGHT_SPEAR, ITEM2);
    TEST_SET_EQUIPMENT(world, ITEM_ID_LEATHER_SHIELD,   ITEM3);
    TEST_SET_EQUIPMENT(world, ITEM_ID_SALVE,            ITEM4);

    canEquip_Loadout_None(&can_equip, UNIT_HAND_LEFT);
    canEquip_Loadout_None(&can_equip, UNIT_HAND_RIGHT);

    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true( Unit_canEquip(&Silou, can_equip));

    /* --- Free --- */
    Unit_Free(&Silou);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    tnecs_finale(&world);
    gl_world = NULL;
}

void test_range(void) {
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;

#include "register/components.h"

    struct Range range1     = {0};
    struct Range range2     = {0};
    struct Range range_out  = {0};
    range1.min = 1;
    range1.max = 2;
    range2      = Range_default;
    range_out   = _Ranges_Combine(range1, range2);
    nourstest_true(range_out.min == range1.min);
    nourstest_true(range_out.max == range1.max);

    Ranges_Combine(&range2, range1);
    nourstest_true(range2.min == range1.min);
    nourstest_true(range2.max == range1.max);

    /*  - Does the loadout make sense for unit/class/selection - */
    struct Unit Silou = Unit_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab, struct Item);

    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;
    Unit_Init(&Silou);
    Unit_id_set(&Silou, UNIT_ID_SILOU);

    Unit_Unequip(&Silou, UNIT_HAND_LEFT);
    Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
    tnecs_E *silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET,      ITEM1);
    TEST_SET_EQUIPMENT(world, ITEM_ID_RAPIERE,      ITEM2);
    TEST_SET_EQUIPMENT(world, ITEM_ID_GLAIVE,       ITEM3);
    TEST_SET_EQUIPMENT(world, ITEM_ID_IRON_SWORD,   ITEM4);
    TEST_SET_EQUIPMENT(world, ITEM_ID_UCHIGATANA,   ITEM5);
    TEST_SET_EQUIPMENT(world, ITEM_ID_EXSANGUE,     ITEM6);

    Item_Load(Unit_InvItem(&Silou, ITEM1)->id);
    Item_Load(Unit_InvItem(&Silou, ITEM2)->id);
    Item_Load(Unit_InvItem(&Silou, ITEM3)->id);
    Item_Load(Unit_InvItem(&Silou, ITEM4)->id);
    Item_Load(Unit_InvItem(&Silou, ITEM5)->id);
    Item_Load(Unit_InvItem(&Silou, ITEM6)->id);
    struct Weapon *wpns[SOTA_EQUIPMENT_SIZE];
    wpns[0] = DTAB_GET(gl_weapons_dtab, Unit_InvItem(&Silou, ITEM1)->id);
    wpns[1] = DTAB_GET(gl_weapons_dtab, Unit_InvItem(&Silou, ITEM2)->id);
    wpns[2] = DTAB_GET(gl_weapons_dtab, Unit_InvItem(&Silou, ITEM3)->id);
    wpns[3] = DTAB_GET(gl_weapons_dtab, Unit_InvItem(&Silou, ITEM4)->id);
    wpns[4] = DTAB_GET(gl_weapons_dtab, Unit_InvItem(&Silou, ITEM5)->id);
    wpns[5] = DTAB_GET(gl_weapons_dtab, Unit_InvItem(&Silou, ITEM6)->id);
    SDL_assert(wpns[0] != NULL);
    SDL_assert(wpns[1] != NULL);
    SDL_assert(wpns[2] != NULL);
    SDL_assert(wpns[3] != NULL);
    SDL_assert(wpns[4] != NULL);
    SDL_assert(wpns[5] != NULL);

    /* Swords */
    wpns[0]->stats.range.min = 1;
    wpns[0]->stats.range.max = 1;
    /* Magic weapons, lance, axes */
    wpns[1]->stats.range.min = 1;
    wpns[1]->stats.range.max = 2;
    SDL_assert(wpns[1]->stats.range.min == 1);
    SDL_assert(wpns[1]->stats.range.max == 2);
    /* Magic weapons -> siege tomes */
    wpns[2]->stats.range.min = 3;
    wpns[2]->stats.range.max = 7;
    SDL_assert(wpns[2]->stats.range.min == 3);
    SDL_assert(wpns[2]->stats.range.max == 7);
    /* Bows */
    wpns[3]->stats.range.min = 2;
    wpns[3]->stats.range.max = 2;
    wpns[4]->stats.range.min = 2;
    wpns[4]->stats.range.max = 3;
    wpns[5]->stats.range.min = 2;
    wpns[5]->stats.range.max = 4;

    struct Weapon *wpn = DTAB_GET(gl_weapons_dtab, Unit_InvItem(&Silou, ITEM2)->id);
    SDL_assert(wpn->stats.range.min == 1);
    SDL_assert(wpn->stats.range.max == 2);

    struct Range range;

    /* Unit_Range_Eq */
    Silou.flags.equippable = ITEM_TYPE_SWORD;
    for (int eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; ++eq) {
        Unit_Range_Eq(  &Silou, eq, ITEM_ARCHETYPE_WEAPON,
                        &range);
        nourstest_true(Range_Valid(range));
        Range wpn_range = Weapon_Range(wpns[eq - ITEM1]);
        nourstest_true(wpn_range.min == range.min);
        nourstest_true(wpn_range.max == range.max);
        Unit_Range_Eq(&Silou, eq, ITEM_TYPE_LANCE, &range);
        nourstest_true(!Range_Valid(range));
    }

    /* Unit_Range_Id */
    for (int eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; ++eq) {
        int id = Unit_Id_Equipment(&Silou, eq);
        Unit_Range_Id(  &Silou, id, ITEM_ARCHETYPE_WEAPON,
                        &range);
        nourstest_true(Range_Valid(range));
        Range wpn_range = Weapon_Range(wpns[eq - ITEM1]);
        nourstest_true(wpn_range.min == range.min);
        nourstest_true(wpn_range.max == range.max);

        Unit_Range_Id(&Silou, id, ITEM_TYPE_LANCE, &range);
        nourstest_true(!Range_Valid(range));
    }

    wpn = DTAB_GET(gl_weapons_dtab, Unit_Id_Equipment(&Silou, ITEM2));
    SDL_assert(wpn->stats.range.min == 1);
    SDL_assert(wpn->stats.range.max == 2);

    /* Unit_Range_Loadout */
    Unit_Equip(&Silou, UNIT_HAND_LEFT,  ITEM1);
    Unit_Equip(&Silou, UNIT_HAND_RIGHT, ITEM2);
    Unit_Range_Equipped(&Silou, ITEM_TYPE_SWORD, &range);
    Range wpn_range1 = Weapon_Range(wpns[ITEM1]);
    Range wpn_range2 = Weapon_Range(wpns[ITEM2]);
    Range wpn_range3 = Weapon_Range(wpns[ITEM3]);
    Range wpn_range4 = Weapon_Range(wpns[ITEM4]);
    Range wpn_range5 = Weapon_Range(wpns[ITEM5]);
    nourstest_true(range.min == wpn_range1.min);
    nourstest_true(range.max == wpn_range1.max);

    Unit_Equip(&Silou, UNIT_HAND_LEFT,  ITEM2);
    Unit_Equip(&Silou, UNIT_HAND_RIGHT, ITEM3);
    Unit_Range_Equipped(&Silou, ITEM_TYPE_SWORD, &range);
    nourstest_true(range.min == wpn_range1.min);
    nourstest_true(range.max == wpn_range2.max);

    Unit_Equip(&Silou, UNIT_HAND_LEFT,  ITEM5);
    Unit_Equip(&Silou, UNIT_HAND_RIGHT, ITEM6);
    Unit_Range_Equipped(&Silou, ITEM_TYPE_SWORD, &range);
    nourstest_true(range.min == wpn_range4.min);
    nourstest_true(range.max == wpn_range5.max);

    Unit_Equip(&Silou, UNIT_HAND_LEFT,  ITEM4);
    Unit_Equip(&Silou, UNIT_HAND_RIGHT, ITEM5);
    Unit_Range_Equipped(&Silou, ITEM_TYPE_SWORD, &range);
    nourstest_true(range.min == wpn_range3.min);
    nourstest_true(range.max == wpn_range4.max);

    /* Unit_Range_Equipment */
    silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, ITEM_NULL,        ITEM1);
    TEST_SET_EQUIPMENT(world, ITEM_ID_RAPIERE,  ITEM2);
    TEST_SET_EQUIPMENT(world, ITEM_ID_GLAIVE,   ITEM3);
    TEST_SET_EQUIPMENT(world, ITEM_NULL,        ITEM4);
    TEST_SET_EQUIPMENT(world, ITEM_NULL,        ITEM5);
    TEST_SET_EQUIPMENT(world, ITEM_NULL,        ITEM6);
    Unit_Range_Equipment(&Silou, ITEM_TYPE_SWORD, &range);
    wpn_range1 = Weapon_Range(wpns[ITEM1]);
    wpn_range2 = Weapon_Range(wpns[ITEM2]);

    nourstest_true(range.min == wpn_range1.min);
    nourstest_true(range.max == wpn_range2.max);

    silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, ITEM_NULL,            ITEM1);
    TEST_SET_EQUIPMENT(world, ITEM_NULL,            ITEM2);
    TEST_SET_EQUIPMENT(world, ITEM_NULL,            ITEM3);
    TEST_SET_EQUIPMENT(world, ITEM_ID_IRON_SWORD,   ITEM4);
    TEST_SET_EQUIPMENT(world, ITEM_ID_UCHIGATANA,   ITEM5);
    TEST_SET_EQUIPMENT(world, ITEM_ID_EXSANGUE,     ITEM6);
    Unit_Range_Equipment(&Silou, ITEM_TYPE_SWORD, &range);
    nourstest_true(range.min == wpns[3]->stats.range.min);
    nourstest_true(range.max == wpns[5]->stats.range.max);

    silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET,      ITEM1);
    TEST_SET_EQUIPMENT(world, ITEM_NULL,            ITEM2);
    TEST_SET_EQUIPMENT(world, ITEM_ID_GLAIVE,       ITEM3);
    TEST_SET_EQUIPMENT(world, ITEM_NULL,            ITEM4);
    TEST_SET_EQUIPMENT(world, ITEM_NULL,            ITEM5);
    TEST_SET_EQUIPMENT(world, ITEM_NULL,            ITEM6);
    Unit_Range_Equipment(&Silou, ITEM_TYPE_SWORD, &range);
    nourstest_true(!Range_Valid(range));

    /* --- Free --- */
    Unit_Free(&Silou);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    tnecs_finale(&world);
    gl_world = NULL;
}

void test_status(void) {

}

void test_tetrabrachios(void) {

}

void test_ComputedStats_TwoHand(void) {
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;
    gl_weapons_dtab  = DTAB_INIT(gl_weapons_dtab,  struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);

#include "register/components.h"

    /* --- Init Silou --- */
    struct Unit Silou = Unit_default;
    Unit_Init(&Silou);
    Silou.flags.equippable = ITEM_TYPE_LANCE;
    Silou.stats.current.prof = 100;

    /* --- Equip Twohanded weapons --- */
    tnecs_E *silou_eq = Unit_Equipment(&Silou);
    i32 item_id = ITEM_ID_IRON_LANCE;
    i32 eq      = ITEM1;
    Item_Load(item_id);
    TEST_SET_EQUIPMENT(world, item_id, eq);
    nourstest_true(Unit_InvItem(&Silou, eq)->id == item_id);
    Weapon *wpn = DTAB_GET(gl_weapons_dtab, Unit_InvItem(&Silou, eq)->id);
    i32 stat_2H = wpn->stats.attack.physical + 3;
    wpn->stats.attack_physical_2H = stat_2H;

    nourstest_true(Weapon_Handedness(wpn) == WEAPON_HAND_ANY);
    Unit_Equip(&Silou, UNIT_HAND_LEFT, eq);
    Unit_Equip(&Silou, UNIT_HAND_RIGHT, eq);

    nourstest_true(Unit_isEquipped(&Silou, UNIT_HAND_LEFT));
    nourstest_true(Unit_isEquipped(&Silou, UNIT_HAND_RIGHT));
    nourstest_true(Unit_Eq_Equipped(&Silou, UNIT_HAND_LEFT) == eq);
    nourstest_true(Unit_Eq_Equipped(&Silou, UNIT_HAND_RIGHT) == eq);
    nourstest_true(Unit_istwoHanding(&Silou));

    /* --- Test: stats aren't doubled --- */
    /* -- Preliminaries --- */
    Unit_stats eff_stats =  Unit_effectiveStats(&Silou);

    int dist = 1;
    i32 lance_1H_stat;
    i32 lance_2H_stat;
    Computed_Stats stats = Unit_computedStats(&Silou, dist, eff_stats);
    WeaponStatGet get = {
        .distance   = dist,
        .hand       = WEAPON_HAND_TWO,
    };
    tnecs_E wpn_ent    = Unit_InvItem_Entity(&Silou, eq);

    /* -- 2H Patt --- */
    get.stat = WEAPON_STAT_pATTACK;
    lance_2H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    nourstest_true(lance_2H_stat > 0);

    /* -- 2H Matt --- */
    get.stat = WEAPON_STAT_mATTACK;
    lance_2H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    nourstest_true(lance_2H_stat == 0);

    /* -- 2H Tatt --- */
    get.stat = WEAPON_STAT_tATTACK;
    lance_2H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    nourstest_true(lance_2H_stat == 0);

    /* -- 2H pPROTECTION --- */
    get.stat = WEAPON_STAT_pPROTECTION;
    lance_2H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    get.hand            = WEAPON_HAND_ONE;
    lance_1H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    nourstest_true(lance_2H_stat == lance_1H_stat);
    get.hand            = WEAPON_HAND_TWO;

    /* -- 2H mPROTECTION --- */
    get.stat = WEAPON_STAT_mPROTECTION;
    lance_2H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    get.hand            = WEAPON_HAND_ONE;
    lance_1H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    nourstest_true(lance_2H_stat == lance_1H_stat);
    get.hand            = WEAPON_HAND_TWO;

    /* -- 2H tPROTECTION --- */
    get.stat = WEAPON_STAT_tPROTECTION;
    lance_2H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    get.hand            = WEAPON_HAND_ONE;
    lance_1H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    nourstest_true(lance_2H_stat == lance_1H_stat);
    get.hand            = WEAPON_HAND_TWO;

    /* -- 2H TPROTECTION --- */
    get.stat = WEAPON_STAT_TPROTECTION;
    lance_2H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    get.hand            = WEAPON_HAND_ONE;
    lance_1H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    nourstest_true(lance_2H_stat == lance_1H_stat);
    get.hand            = WEAPON_HAND_TWO;

    /* -- 2H RANGEMIN --- */
    get.stat = WEAPON_STAT_RANGEMIN;
    lance_2H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    get.hand            = WEAPON_HAND_ONE;
    lance_1H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    nourstest_true(lance_2H_stat == lance_1H_stat);
    get.hand            = WEAPON_HAND_TWO;

    /* -- 2H RANGEMAX --- */
    get.stat = WEAPON_STAT_RANGEMAX;
    lance_2H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    get.hand            = WEAPON_HAND_ONE;
    lance_1H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    nourstest_true(lance_2H_stat == lance_1H_stat);
    get.hand            = WEAPON_HAND_TWO;

    /* -- 2H HIT --- */
    get.stat = WEAPON_STAT_HIT;
    lance_2H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    get.hand            = WEAPON_HAND_ONE;
    lance_1H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    nourstest_true(lance_2H_stat == lance_1H_stat);
    get.hand            = WEAPON_HAND_TWO;

    /* -- 2H DODGE --- */
    get.stat = WEAPON_STAT_DODGE;
    lance_2H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    get.hand            = WEAPON_HAND_ONE;
    lance_1H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    nourstest_true(lance_2H_stat == lance_1H_stat);
    get.hand            = WEAPON_HAND_TWO;

    /* -- 2H CRIT --- */
    get.stat = WEAPON_STAT_CRIT;
    lance_2H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    get.hand            = WEAPON_HAND_ONE;
    lance_1H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    nourstest_true(lance_2H_stat == lance_1H_stat);
    get.hand            = WEAPON_HAND_TWO;

    /* -- 2H FAVOR --- */
    get.stat = WEAPON_STAT_FAVOR;
    lance_2H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    get.hand            = WEAPON_HAND_ONE;
    lance_1H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    nourstest_true(lance_2H_stat == lance_1H_stat);
    get.hand            = WEAPON_HAND_TWO;

    /* -- 2H WGT --- */
    get.stat = WEAPON_STAT_WGT;
    lance_2H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    get.hand            = WEAPON_HAND_ONE;
    lance_1H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    nourstest_true(lance_2H_stat == lance_1H_stat);
    get.hand            = WEAPON_HAND_TWO;

    /* -- 2H PROF --- */
    wpn->stats.prof_2H = 19;
    get.stat            = WEAPON_STAT_PROF;
    get.hand            = WEAPON_HAND_TWO;
    lance_2H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    get.hand            = WEAPON_HAND_ONE;
    lance_1H_stat       = Weapon_Stat_Eff_E(wpn_ent, get);
    nourstest_true(lance_2H_stat != lance_1H_stat);
    nourstest_true(lance_2H_stat == wpn->stats.prof_2H);
    get.hand            = WEAPON_HAND_TWO;

    /* --- Free --- */
    Unit_Free(&Silou);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    tnecs_finale(&world);
    gl_world = NULL;
}

void test_equip(void) {
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab,    struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab,      struct Item);

#include "register/components.h"

    /* --- Init Silou --- */
    struct Unit Silou = Unit_default;
    Unit_Init(&Silou);
    Silou.flags.equippable = ITEM_TYPE_LANCE;
    /* --- Equipping --- */


    /* --- Free --- */
    Unit_Free(&Silou);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    tnecs_finale(&world);
    gl_world = NULL;

}

void test_inRange(void) {
    /* --- inRange_Dist --- */
    {
        Range r1 = {1, 2};
        i32 dist = 0;
        nourstest_true(!inRange_Dist(r1, dist));
    }
    {
        Range r1 = {1, 2};
        i32 dist = 1;
        nourstest_true(inRange_Dist(r1, dist));
    }
    {
        Range r1 = {1, 2};
        i32 dist = 2;
        nourstest_true(inRange_Dist(r1, dist));
    }
    {
        Range r1 = {1, 2};
        i32 dist = 3;
        nourstest_true(!inRange_Dist(r1, dist));
    }

    {
        Range r1 = {2, 3};
        i32 dist = 0;
        nourstest_true(!inRange_Dist(r1, dist));
    }
    {
        Range r1 = {2, 3};
        i32 dist = 1;
        nourstest_true(!inRange_Dist(r1, dist));
    }
    {
        Range r1 = {2, 3};
        i32 dist = 2;
        nourstest_true(inRange_Dist(r1, dist));
    }
    {
        Range r1 = {2, 3};
        i32 dist = 3;
        nourstest_true(inRange_Dist(r1, dist));
    }
    {
        Range r1 = {2, 3};
        i32 dist = 4;
        nourstest_true(!inRange_Dist(r1, dist));
    }
    {
        Range r1 = {2, 3};
        i32 dist = 4;
        nourstest_true(!inRange_Dist(r1, dist));
    }
    /* --- inRange --- */
    {
        Range r1 = {2, 3};
        Range r2 = {1, 1};
        nourstest_true(!inRange(r1, r2));
        nourstest_true(!inRange(r2, r1));
    }
    {
        Range r1 = {2, 3};
        Range r2 = {1, 2};
        nourstest_true(inRange(r1, r2));
        nourstest_true(inRange(r2, r1));
    }
    {
        Range r1 = {2, 3};
        Range r2 = {1, 3};
        nourstest_true(inRange(r1, r2));
        nourstest_true(inRange(r2, r1));
    }
    {
        Range r1 = {2, 3};
        Range r2 = {1, 4};
        nourstest_true(inRange(r1, r2));
        nourstest_true(inRange(r2, r1));
    }
    {
        Range r1 = {2, 3};
        Range r2 = {4, 4};
        nourstest_true(!inRange(r1, r2));
        nourstest_true(!inRange(r2, r1));
    }
    {
        Range r1 = {1, 1};
        Range r2 = {2, 2};
        nourstest_true(!inRange(r1, r2));
        nourstest_true(!inRange(r2, r1));
    }
}

void test_unit(void) {
    test_canEquip_OneHand();
    test_canEquip_TwoHand();
    test_canEquip_Type();
    test_canEquip_Users();
    test_canEquip_Range();
    test_canEquip_Archetype();
    test_canEquip();
    test_equip();
    test_tetrabrachios();
    test_ComputedStats_TwoHand();
    test_status();

    test_range();
    test_inRange();

    test_skills();
    test_io();
    test_growth();
    test_bonus_decay();
    test_bonus_stats();
    test_reinforcements();

    URN_debug = -1;
}

#undef TEST_SET_EQUIPMENT
