
#include "nourstest.h"
#include "platform.h"
#include "enums.h"
#include "weapon.h"
#include "unit/range.h"
#include "unit/flags.h"
#include "reinforcement.h"
#include "unit/unit.h"
#include "unit/bonus.h"
#include "unit/stats.h"
#include "game/game.h"
#include "game/unit.h"
#include "RNG.h"

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
    Unit_Free(&Silou);
}

void test_skills(void) {
    struct Combat_Phase combat_outcome[SOTA_COMBAT_MAX_PHASES];
    struct Point attacker_pos = {1, 2};
    struct Point defender_pos = {2, 2};
    int distance = 1;
    struct Unit Silou = Unit_default;
    struct Unit Hamilcar = Unit_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);
    Weapon_Load(gl_weapons_dtab, ITEM_ID_FLEURET);
    struct Weapon *fleuret = DTAB_GET(gl_weapons_dtab, ITEM_ID_FLEURET);
    nourstest_true(fleuret != NULL);
    nourstest_true(fleuret->canAttack);
    nourstest_true(Item_Typecode(fleuret->item) == ITEM_TYPE_SWORD);
    nourstest_true(fleuret->item->ids.id == ITEM_ID_FLEURET);
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
    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id = ITEM_ID_FLEURET;
    Unit_Item_Drop(&Silou,           UNIT_HAND_LEFT);
    Unit_Item_Takeat(&Silou, in_wpn, UNIT_HAND_LEFT);
    Unit_Item_Drop(&Hamilcar,           UNIT_HAND_LEFT);
    Unit_Item_Takeat(&Hamilcar, in_wpn, UNIT_HAND_LEFT);
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

    /* --- SDL_free --- */
    Unit_Free(&Silou);
    Unit_Free(&Hamilcar);
    Game_Weapons_Free(&gl_weapons_dtab);
}

void test_io(void) {
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
    struct Unit_stats out_caps      = Unit_stats_default;
    struct Unit_stats *out_growths  = NULL;
    struct Inventory_item in_wpn    = Inventory_item_default;
    struct Inventory_item out_wpn   = Inventory_item_default;

    Unit_id_set(&unit1, UNIT_ID_SILOU);
    Unit_setClassind(&unit1, UNIT_CLASS_FENCER);
    unit1.flags.sex     = UNIT_SEX_F;
    unit1.stats.caps    = in_caps;
    unit1.stats.bases   = in_stats;
    unit1.stats.current = in_stats;
    Unit_Stats_Growths_Set(&unit1, in_growths);
    unit1.level.base_exp      = 0;
    unit1.level.exp           = 0;
    in_wpn.id = ITEM_ID_FLEURET;
    Unit_Item_Take(&unit1, in_wpn);
    in_wpn.id = ITEM_ID_KITCHEN_KNIFE;
    Unit_Item_Take(&unit1, in_wpn);
    in_wpn.id = ITEM_ID_POT_LID;
    Unit_Item_Take(&unit1, in_wpn);
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

    Game_Weapons_Free(&gl_weapons_dtab);
}

void test_growth(void) {
    rng_sequence_breaker_miss_growth = true;
    struct Unit Silou = Unit_default;
    Unit_Init(&Silou);
    /*                              hp, str, mag, agi, dex, fth, luck, def, res, con, move, prof */
    struct Unit_stats in_stats = {  17,   6,   2,   7,   7,   7,    7,   4,   5,   6,   4,    2};
    struct Unit_stats in_caps = {   48,  14,  25,  32,  34,  28,   28,  19,  40,  15,   20,  25};
    struct Unit_stats in_growths = {60,  50,  20,  60,  70,  30,   40,  30,  20,  10,   10,  20};

    struct Unit_stats out_stats     = Unit_stats_default;
    struct Unit_stats out_caps      = Unit_stats_default;
    struct Unit_stats *out_growths  = NULL;

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
    out_growths = Unit_Stats_Growths(&Silou);

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
    struct Unit Silou = Unit_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab, struct Item);
    Unit_Init(&Silou);

    Unit_setClassind(&Silou, UNIT_CLASS_FENCER);
    Weapon_Load(gl_weapons_dtab, ITEM_ID_FLEURET);
    struct Weapon *weapon = DTAB_GET(gl_weapons_dtab, ITEM_ID_FLEURET);
    /* Try to equip a one hand weapon when already in other hand */

    Unit_Unequip(&Silou, UNIT_HAND_LEFT);
    Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
    Inventory_item *silou_eq = Unit_Equipment(&Silou);
    silou_eq[0].id    = ITEM_ID_FLEURET;
    silou_eq[1].id    = ITEM_ID_FLEURET;
    silou_eq[2].id    = ITEM_ID_FLEURET;
    silou_eq[3].id    = ITEM_ID_FLEURET;
    silou_eq[4].id    = ITEM_ID_FLEURET;
    silou_eq[5].id    = ITEM_ID_FLEURET;

    i32 mode = TWO_HAND_EQ_MODE_STRICT;
    // Left handed Weapon
    weapon->handedness = WEAPON_HAND_LEFT;
    nourstest_true( Unit_canEquip_OneHand(&Silou,  ITEM1, UNIT_HAND_LEFT, mode));
    nourstest_true(!Unit_canEquip_OneHand(&Silou,  ITEM1, UNIT_HAND_RIGHT, mode));

    // Right handed Weapon
    weapon->handedness = WEAPON_HAND_RIGHT;
    nourstest_true(!Unit_canEquip_OneHand(&Silou, ITEM1, UNIT_HAND_LEFT, mode));
    nourstest_true( Unit_canEquip_OneHand(&Silou, ITEM1, UNIT_HAND_RIGHT, mode));

    weapon->handedness = WEAPON_HAND_ONE;
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
    weapon->handedness = WEAPON_HAND_ANY;
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
    weapon->handedness = WEAPON_HAND_TWO;
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
    Game_Weapons_Free(&gl_weapons_dtab);
}

void test_canEquip_TwoHand() {
    struct Unit Silou = Unit_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);
    Unit_Init(&Silou);
    Unit_setClassind(&Silou, UNIT_CLASS_FENCER);
    Weapon_Load(gl_weapons_dtab, ITEM_ID_FLEURET);
    Weapon_Load(gl_weapons_dtab, ITEM_ID_RAPIERE);
    Weapon_Load(gl_weapons_dtab, ITEM_ID_HEAL);
    struct Weapon *weapon = DTAB_GET(gl_weapons_dtab, ITEM_ID_FLEURET);

    /* Try to equip a one hand weapon when already in other hand */
    Unit_Unequip(&Silou, UNIT_HAND_LEFT);
    Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
    Inventory_item *silou_eq = Unit_Equipment(&Silou);
    silou_eq[0].id    = ITEM_ID_FLEURET;
    silou_eq[1].id    = ITEM_ID_FLEURET;
    silou_eq[2].id    = ITEM_ID_FLEURET;
    silou_eq[3].id    = ITEM_ID_FLEURET;
    silou_eq[4].id    = ITEM_ID_FLEURET;
    silou_eq[5].id    = ITEM_ID_FLEURET;

    // Left handed Weapon
    weapon->handedness = WEAPON_HAND_LEFT;
    i32 mode = TWO_HAND_EQ_MODE_STRICT;
    nourstest_true(Unit_canEquip_TwoHand(&Silou,  ITEM1, UNIT_HAND_LEFT, mode));
    nourstest_true(Unit_canEquip_TwoHand(&Silou,  ITEM1, UNIT_HAND_RIGHT, mode));

    // Right handed Weapon
    weapon->handedness = WEAPON_HAND_RIGHT;
    nourstest_true(Unit_canEquip_TwoHand(&Silou, ITEM1, UNIT_HAND_LEFT, mode));
    nourstest_true(Unit_canEquip_TwoHand(&Silou, ITEM1, UNIT_HAND_RIGHT, mode));

    weapon->handedness = WEAPON_HAND_ONE;
    for (i32 eq = ITEM1; eq < SOTA_EQUIPMENT_SIZE; eq++) {
        // One handed Weapon already equipped in left hand
        Unit_Equip(&Silou, UNIT_HAND_LEFT, eq);
        Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_RIGHT, mode));

        // One handed Weapon already equipped in right hand
        Unit_Unequip(&Silou, UNIT_HAND_LEFT);
        Unit_Equip(&Silou, UNIT_HAND_RIGHT, eq);
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_RIGHT, mode));
    }

    // Any handed Weapon
    weapon->handedness = WEAPON_HAND_ANY;
    for (i32 eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; eq++) {
        // One handed Weapon already equipped in left hand
        int eq_opposite = SOTA_EQUIPMENT_SIZE - eq + ITEM1;
        Unit_Equip(&Silou, UNIT_HAND_LEFT, eq_opposite);
        Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_RIGHT, mode));

        // One handed Weapon already equipped in right hand
        Unit_Unequip(&Silou, UNIT_HAND_LEFT);
        Unit_Equip(&Silou, UNIT_HAND_RIGHT, eq_opposite);
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true(Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_RIGHT, mode));
    }

    // Two handed Weapon
    weapon->handedness = WEAPON_HAND_TWO;
    for (i32 eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; eq++) {
        // Other weapon weapon equipped in left hand
        int eq_opposite = SOTA_EQUIPMENT_SIZE - eq + ITEM1;
        Unit_Equip(&Silou, UNIT_HAND_LEFT, eq_opposite);
        Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
        nourstest_true( Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true(!Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_RIGHT, mode));

        // Other weapon weapon equipped in right hand
        Unit_Unequip(&Silou, UNIT_HAND_LEFT);
        Unit_Equip(&Silou, UNIT_HAND_RIGHT, eq_opposite);
        nourstest_true(!Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_LEFT, mode));
        nourstest_true( Unit_canEquip_TwoHand(&Silou, eq, UNIT_HAND_RIGHT, mode));
    }

    /* Try to equip a two  two handes weapon when already in different hands */
    struct Weapon *weapon2 = DTAB_GET(gl_weapons_dtab, ITEM_ID_RAPIERE);
    weapon->handedness  = WEAPON_HAND_TWO;
    weapon2->handedness = WEAPON_HAND_TWO;

    /* Try to equip a one hand weapon when already in other hand */
    Unit_Equip(&Silou, UNIT_HAND_LEFT, ITEM1);
    Unit_Unequip(&Silou, UNIT_HAND_RIGHT);

    silou_eq = Unit_Equipment(&Silou);
    silou_eq[0].id    = ITEM_ID_FLEURET;
    silou_eq[1].id    = ITEM_ID_RAPIERE;

    nourstest_true(!Unit_canEquip_TwoHand(&Silou, ITEM2, UNIT_HAND_RIGHT, mode));
    nourstest_true( Unit_canEquip_TwoHand(&Silou, ITEM1, UNIT_HAND_RIGHT, mode));

    /* Try to equip a one hand weapon when already in other hand with type
         that can't be twohanded */
    Unit_Equip(&Silou,      UNIT_HAND_LEFT, ITEM1);
    Unit_Unequip(&Silou,    UNIT_HAND_RIGHT);
    silou_eq = Unit_Equipment(&Silou);
    silou_eq[0].id    = ITEM_ID_FLEURET;
    silou_eq[1].id    = ITEM_ID_RAPIERE;

    weapon->handedness  = WEAPON_HAND_ANY;
    weapon2->handedness = WEAPON_HAND_ANY;
    weapon->item->type.top  = ITEM_TYPE_ELEMENTAL;
    weapon->item->type.top  = ITEM_TYPE_ANGELIC;
    nourstest_true( Unit_canEquip_TwoHand(&Silou, ITEM2, UNIT_HAND_RIGHT, mode));
    nourstest_true( Unit_canEquip_OneHand(&Silou, ITEM1, UNIT_HAND_RIGHT, mode));

    /* Try to equip staff  */
    Unit_Unequip(&Silou, UNIT_HAND_LEFT);
    Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
    silou_eq = Unit_Equipment(&Silou);
    silou_eq[0].id    = ITEM_ID_HEAL;

    nourstest_true( Unit_canEquip_TwoHand(&Silou, ITEM1, UNIT_HAND_RIGHT, mode));
    nourstest_true( Unit_canEquip_TwoHand(&Silou, ITEM1, UNIT_HAND_LEFT, mode));

    Game_Weapons_Free(&gl_weapons_dtab);
}

void test_canEquip_Users(void) {
    struct Unit Silou = Unit_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);

    Unit_Init(&Silou);
    int id = ITEM_ID_FLEURET;
    Weapon_Load(gl_weapons_dtab, id);
    Unit_id_set(&Silou, UNIT_ID_SILOU);

    struct Weapon *weapon = DTAB_GET(gl_weapons_dtab, id);

    int eq = 0;
    Inventory_item *silou_eq = Unit_Equipment(&Silou);
    silou_eq[eq].id = id;
    weapon->item->users.id    = NULL;

    nourstest_true(Unit_canEquip_Users(&Silou, id));
    u16 *users = DARR_INIT(users, u16, 4);
    users[0] = UNIT_ID_ERWIN;
    users[1] = UNIT_ID_ERWIN;
    users[2] = UNIT_ID_ERWIN;
    users[3] = UNIT_ID_ERWIN;
    DARR_NUM(users) = 4;
    weapon->item->users.id = users;
    nourstest_true(!Unit_canEquip_Users(&Silou, id));

    users[3] = UNIT_ID_SILOU;
    nourstest_true( Unit_canEquip_Users(&Silou, id));

    Game_Weapons_Free(&gl_weapons_dtab);
}

void test_canEquip_Archetype(void) {
    struct Unit Silou = Unit_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);

    Unit_Init(&Silou);
    int id = ITEM_ID_FLEURET;
    Weapon_Load(gl_weapons_dtab, id);
    Unit_id_set(&Silou, UNIT_ID_SILOU);

    struct Weapon *weapon = DTAB_GET(gl_weapons_dtab, id);

    int eq = 0;
    Inventory_item *silou_eq = Unit_Equipment(&Silou);
    silou_eq[eq].id = id;

    nourstest_true( Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_NULL));
    nourstest_true( Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_ITEM));
    nourstest_true( Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_WEAKHAND));
    nourstest_true( Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_WEAPON));
    nourstest_true(!Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_SHIELD));
    nourstest_true(!Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_STAFF));

    weapon->item->type.top = ITEM_TYPE_STAFF;
    nourstest_true( Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_NULL));
    nourstest_true( Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_ITEM));
    nourstest_true(!Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_WEAKHAND));
    nourstest_true(!Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_WEAPON));
    nourstest_true(!Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_SHIELD));
    nourstest_true( Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_STAFF));

    weapon->item->type.top = ITEM_TYPE_SHIELD;
    nourstest_true( Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_NULL));
    nourstest_true( Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_ITEM));
    nourstest_true( Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_WEAKHAND));
    nourstest_true(!Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_WEAPON));
    nourstest_true( Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_SHIELD));
    nourstest_true(!Unit_canEquip_Archetype(&Silou, id, ITEM_ARCHETYPE_STAFF));

    Game_Weapons_Free(&gl_weapons_dtab);
}

void test_canEquip(void) {
    //  - Does the loadout make sense for unit/class/selection
    struct Unit Silou = Unit_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);

    Unit_Init(&Silou);
    Unit_id_set(&Silou, UNIT_ID_SILOU);

    /* --- Staff user that can't twohand --- */
    Unit_setClassind(&Silou, UNIT_CLASS_VESTAL);
    Silou.flags.equippable = ITEM_TYPE_STAFF;
    // Unit_Equip(&Silou, UNIT_HAND_LEFT, 0);
    // Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
    Inventory_item *silou_eq = Unit_Equipment(&Silou);
    silou_eq[0].id              = ITEM_ID_FLEURET;
    silou_eq[1].id              = ITEM_ID_RAPIERE;
    silou_eq[2].id              = ITEM_ID_HEAL;

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
    silou_eq[0].id  = ITEM_ID_WIND_SPEAR;
    silou_eq[1].id  = ITEM_ID_DOWNFALL;
    silou_eq[2].id  = ITEM_ID_SILVERLIGHT_SPEAR;

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
    silou_eq[0].id = ITEM_ID_IRON_LANCE;
    silou_eq[1].id = ITEM_ID_WOODEN_SHIELD;
    silou_eq[2].id = ITEM_ID_WRATH_LANCE;

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
    silou_eq[0].id = ITEM_ID_BALL_LIGHTNING;
    silou_eq[1].id = ITEM_ID_SILVERLIGHT_SPEAR;
    silou_eq[2].id = ITEM_ID_LEATHER_SHIELD;
    silou_eq[3].id = ITEM_ID_SALVE;

    canEquip_Loadout_None(&can_equip, UNIT_HAND_LEFT);
    canEquip_Loadout_None(&can_equip, UNIT_HAND_RIGHT);

    canEquip_Eq(&can_equip, ITEM1);
    nourstest_true( Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM2);
    nourstest_true(!Unit_canEquip(&Silou, can_equip));
    canEquip_Eq(&can_equip, ITEM3);
    nourstest_true( Unit_canEquip(&Silou, can_equip));

    Game_Weapons_Free(&gl_weapons_dtab);
}

void test_range(void) {
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

    Unit_Init(&Silou);
    Unit_id_set(&Silou, UNIT_ID_SILOU);

    Unit_Unequip(&Silou, UNIT_HAND_LEFT);
    Unit_Unequip(&Silou, UNIT_HAND_RIGHT);
    Inventory_item *silou_eq = Unit_Equipment(&Silou);
    silou_eq[0].id    = ITEM_ID_FLEURET;
    silou_eq[1].id    = ITEM_ID_RAPIERE;
    silou_eq[2].id    = ITEM_ID_GLAIVE;
    silou_eq[3].id    = ITEM_ID_IRON_SWORD;
    silou_eq[4].id    = ITEM_ID_UCHIGATANA;
    silou_eq[5].id    = ITEM_ID_EXSANGUE;

    Weapon_Load(gl_weapons_dtab, silou_eq[0].id);
    Weapon_Load(gl_weapons_dtab, silou_eq[1].id);
    Weapon_Load(gl_weapons_dtab, silou_eq[2].id);
    Weapon_Load(gl_weapons_dtab, silou_eq[3].id);
    Weapon_Load(gl_weapons_dtab, silou_eq[4].id);
    Weapon_Load(gl_weapons_dtab, silou_eq[5].id);
    struct Weapon *wpns[SOTA_EQUIPMENT_SIZE];
    wpns[0] = DTAB_GET(gl_weapons_dtab, silou_eq[0].id);
    wpns[1] = DTAB_GET(gl_weapons_dtab, silou_eq[1].id);
    wpns[2] = DTAB_GET(gl_weapons_dtab, silou_eq[2].id);
    wpns[3] = DTAB_GET(gl_weapons_dtab, silou_eq[3].id);
    wpns[4] = DTAB_GET(gl_weapons_dtab, silou_eq[4].id);
    wpns[5] = DTAB_GET(gl_weapons_dtab, silou_eq[5].id);
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

    struct Weapon *wpn = DTAB_GET(gl_weapons_dtab, silou_eq[1].id);
    SDL_assert(wpn->stats.range.min == 1);
    SDL_assert(wpn->stats.range.max == 2);

    struct Range range;

    /* Unit_Range_Eq */
    Silou.flags.equippable = ITEM_TYPE_SWORD;
    for (int eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; ++eq) {
        Unit_Range_Eq(&Silou, eq, ITEM_ARCHETYPE_WEAPON, &range);
        nourstest_true(Range_Valid(range));
        nourstest_true(wpns[eq - ITEM1]->stats.range.min == range.min);
        nourstest_true(wpns[eq - ITEM1]->stats.range.max == range.max);

        Unit_Range_Eq(&Silou, eq, ITEM_TYPE_LANCE, &range);
        nourstest_true(!Range_Valid(range));
    }

    /* Unit_Range_Id */
    for (int eq = ITEM1; eq <= SOTA_EQUIPMENT_SIZE; ++eq) {
        int id = Unit_Id_Equipment(&Silou, eq);
        Unit_Range_Id(&Silou, id, ITEM_ARCHETYPE_WEAPON, &range);
        nourstest_true(Range_Valid(range));
        nourstest_true(wpns[eq - ITEM1]->stats.range.min == range.min);
        nourstest_true(wpns[eq - ITEM1]->stats.range.max == range.max);

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
    nourstest_true(range.min == wpns[1]->stats.range.min);
    nourstest_true(range.max == wpns[1]->stats.range.max);

    Unit_Equip(&Silou, UNIT_HAND_LEFT,  ITEM2);
    Unit_Equip(&Silou, UNIT_HAND_RIGHT, ITEM3);
    Unit_Range_Equipped(&Silou, ITEM_TYPE_SWORD, &range);
    nourstest_true(range.min == wpns[1]->stats.range.min);
    nourstest_true(range.max == wpns[2]->stats.range.max);

    Unit_Equip(&Silou, UNIT_HAND_LEFT,  ITEM5);
    Unit_Equip(&Silou, UNIT_HAND_RIGHT, ITEM6);
    Unit_Range_Equipped(&Silou, ITEM_TYPE_SWORD, &range);
    nourstest_true(range.min == wpns[4]->stats.range.min);
    nourstest_true(range.max == wpns[5]->stats.range.max);

    Unit_Equip(&Silou, UNIT_HAND_LEFT,  ITEM4);
    Unit_Equip(&Silou, UNIT_HAND_RIGHT, ITEM5);
    Unit_Range_Equipped(&Silou, ITEM_TYPE_SWORD, &range);
    nourstest_true(range.min == wpns[3]->stats.range.min);
    nourstest_true(range.max == wpns[4]->stats.range.max);

    /* Unit_Range_Equipment */
    silou_eq = Unit_Equipment(&Silou);
    silou_eq[0].id    = ITEM_NULL;
    silou_eq[1].id    = ITEM_ID_RAPIERE;
    silou_eq[2].id    = ITEM_ID_GLAIVE;
    silou_eq[3].id    = ITEM_NULL;
    silou_eq[4].id    = ITEM_NULL;
    silou_eq[5].id    = ITEM_NULL;
    Unit_Range_Equipment(&Silou, ITEM_TYPE_SWORD, &range);
    nourstest_true(range.min == wpns[1]->stats.range.min);
    nourstest_true(range.max == wpns[2]->stats.range.max);

    silou_eq = Unit_Equipment(&Silou);
    silou_eq[0].id    = ITEM_NULL;
    silou_eq[1].id    = ITEM_NULL;
    silou_eq[2].id    = ITEM_NULL;
    silou_eq[3].id    = ITEM_ID_IRON_SWORD;
    silou_eq[4].id    = ITEM_ID_UCHIGATANA;
    silou_eq[5].id    = ITEM_ID_EXSANGUE;
    Unit_Range_Equipment(&Silou, ITEM_TYPE_SWORD, &range);
    nourstest_true(range.min == wpns[3]->stats.range.min);
    nourstest_true(range.max == wpns[5]->stats.range.max);

    silou_eq = Unit_Equipment(&Silou);
    silou_eq[0].id    = ITEM_ID_FLEURET;
    silou_eq[1].id    = ITEM_NULL;
    silou_eq[2].id    = ITEM_ID_GLAIVE;
    silou_eq[3].id    = ITEM_NULL;
    silou_eq[4].id    = ITEM_NULL;
    silou_eq[5].id    = ITEM_NULL;
    Unit_Range_Equipment(&Silou, ITEM_TYPE_SWORD, &range);
    nourstest_true(!Range_Valid(range));

    Game_Weapons_Free(&gl_weapons_dtab);
}

void test_status(void) {

}

void test_tetrabrachios(void) {

}

void test_unit(void) {
    test_canEquip_OneHand();
    test_canEquip_TwoHand();
    test_canEquip_Type();
    test_canEquip_Users();
    test_canEquip_Archetype();
    test_canEquip();
    test_tetrabrachios();
    test_status();

    test_range();

    test_skills();
    test_io();
    test_growth();
    test_bonus_decay();
    test_bonus_stats();
    test_reinforcements();

    URN_debug = -1;
}

