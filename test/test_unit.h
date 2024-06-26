#include "nourstest.h"
#include "platform.h"
#include "unit/unit.h"
#include "unit/bonus.h"
#include "game/game.h"
#include "game/unit.h"
#include "RNG.h"

void test_promotion() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);

}

void test_canEquip() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    struct Unit Silou = Unit_default;
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);
    Unit_InitWweapons(&Silou, weapons_dtab);
    Unit_setClassind(&Silou, UNIT_CLASS_FENCER);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_DUKE);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_ARCHER);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_MARKSMAN);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_ANGEL);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_RADIANT_CLAW)   == 1);

    Unit_setClassind(&Silou, UNIT_CLASS_DEMON);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 1);

    Unit_setClassind(&Silou, UNIT_CLASS_ANGELIC_INCARNATE);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_DEMONIC_INCARNATE);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_FENCER);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_DUELIST);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_DUELIST);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_PICKPOCKET);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_THIEF);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_CORSAIR);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_IRON_AXE)       == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_VIKING);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_STEEL_AXE)      == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_RAVAGER);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_DAMAS_AXE)      == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_PIKEMAN);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_IRON_LANCE)     == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_MAGE);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_BATTLEMAGE);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_PRIEST);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_BISHOP);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_ORACLE);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Unit_setClassind(&Silou, UNIT_CLASS_CLERIC);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_FLEURET)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MAIN_GAUCHE)    == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_PITCHFORK)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HOE)            == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_SHORT_BOW)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ADAPTED_SHIELD) == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_POT_LID)        == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_ICICLE)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_NETHER)         == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_MOONLIGHT)      == 0);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_HEAL)           == 1);
    nourstest_true(Unit_canEquip_Type(&Silou, ITEM_ID_CLAW)           == 0);

    Game_Weapons_Free(&weapons_dtab);
    Unit_Free(&Silou);
}

void test_skills() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    struct Combat_Phase combat_outcome[SOTA_COMBAT_MAX_PHASES];
    struct TINYMT32_T tinymt32;
    RNG_Init_tinymt(&tinymt32);
    struct Point attacker_pos = {1, 2};
    struct Point defender_pos = {2, 2};
    int distance = 1;
    struct Unit Silou = Unit_default;
    struct Unit Enemy = Unit_default;
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);
    Weapon_Load(weapons_dtab, ITEM_ID_FLEURET);
    struct Weapon *fleuret = (struct Weapon *)DTAB_GET(weapons_dtab, ITEM_ID_FLEURET);
    nourstest_true(fleuret != NULL);
    nourstest_true(fleuret->canAttack);
    nourstest_true(fleuret->item->type == ITEM_TYPE_SWORD);
    nourstest_true(fleuret->item->id == ITEM_ID_FLEURET);
    nourstest_true(Weapon_canAttack(fleuret));
    Unit_InitWweapons(&Silou, weapons_dtab);
    Unit_InitWweapons(&Enemy, weapons_dtab);
    //                           hp, str, mag, agi, dex, fth, luck, def, res, con, move
    struct Unit_stats in_stats = {17,  6,  2,  7,  7, 7,  7,  4,  5,  6, 5};
    Unit_setClassind(&Silou, UNIT_CLASS_FENCER);
    Unit_setClassind(&Enemy, UNIT_CLASS_FENCER);

    Unit_setStats(&Silou, in_stats);
    Unit_setStats(&Enemy, in_stats);
    Silou.skills = UNIT_SKILL_PINPRICK;
    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id = ITEM_ID_FLEURET;
    Unit_Item_Drop(&Silou,           UNIT_HAND_WEAK);
    Unit_Item_Takeat(&Silou, in_wpn, UNIT_HAND_WEAK);
    Unit_Item_Drop(&Enemy,           UNIT_HAND_WEAK);
    Unit_Item_Takeat(&Enemy, in_wpn, UNIT_HAND_WEAK);
    Silou.equipped[UNIT_HAND_WEAK] = true;
    Enemy.equipped[UNIT_HAND_WEAK] = true;
    // Unit_Equip_inHand(&Silou, UNIT_HAND_WEAK);
    // Unit_Equip_inHand(&Enemy, UNIT_HAND_WEAK);
    nourstest_true(Enemy.equipped[UNIT_HAND_WEAK]);
    nourstest_true(Silou.equipped[UNIT_HAND_WEAK]);
    nourstest_true(Unit_canAttack(&Enemy));
    nourstest_true(Unit_canAttack(&Silou));
    Unit_computedStats(&Silou, distance);
    Unit_computedStats(&Enemy, distance);

    /* --- FREE --- */
    Unit_Free(&Silou);
    Unit_Free(&Enemy);
    Game_Weapons_Free(&weapons_dtab);
}

void test_io() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    struct Unit unit1 = Unit_default;
    struct Unit unit2 = Unit_default;
    struct Unit unit3 = Unit_default;
    struct Unit unit4 = Unit_default;
    struct Unit unit5 = Unit_default;
    struct dtab *weapons_dtab  = DTAB_INIT(weapons_dtab,  struct Weapon);
    struct dtab *weapons_dtab2 = DTAB_INIT(weapons_dtab2, struct Weapon);
    Unit_InitWweapons(&unit1, weapons_dtab);
    Unit_InitWweapons(&unit2, weapons_dtab);
    Unit_InitWweapons(&unit3, weapons_dtab);
    Unit_InitWweapons(&unit4, weapons_dtab);
    Unit_InitWweapons(&unit5, weapons_dtab);
    /*                              hp, str, mag, agi, dex, fth, luck, def, res, con, move, prof */
    struct Unit_stats in_caps =    {48,  14,  25,  32,  34,  28,   28,  19,  40,  15,    0, 25};
    struct Unit_stats in_stats =   {17,   6,   2,   7,   7,   7,    7,   4,   5,   6,    5,  3};
    struct Unit_stats in_growths = {60,  50,  20,  60,  70,  30,   40,  30,  20,  10,    0, 10};
    struct Unit_stats out_stats     = Unit_stats_default;
    struct Unit_stats out_caps      = Unit_stats_default;
    struct Unit_stats out_growths   = Unit_stats_default;
    struct Inventory_item in_wpn    = Inventory_item_default;
    struct Inventory_item out_wpn   = Inventory_item_default;

    Unit_setid(&unit1, UNIT_ID_SILOU);
    Unit_setClassind(&unit1, UNIT_CLASS_FENCER);
    unit1.sex           = UNIT_SEX_M;
    unit1.caps_stats    = in_caps;
    unit1.base_stats    = in_stats;
    unit1.current_stats = in_stats;
    unit1.growths       = in_growths;
    unit1.base_exp      = 0;
    unit1.exp           = 0;
    in_wpn.id = ITEM_ID_FLEURET;
    Unit_Item_Take(&unit1, in_wpn);
    in_wpn.id = ITEM_ID_KITCHEN_KNIFE;
    Unit_Item_Take(&unit1, in_wpn);
    in_wpn.id = ITEM_ID_POT_LID;
    Unit_Item_Take(&unit1, in_wpn);
    out_stats = unit1.current_stats;
    nourstest_true(s8equal(unit1.name, s8_literal("Silou")));
    nourstest_true(unit1.sex == 1);
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
    out_growths = unit1.growths;
    nourstest_true(in_growths.hp    == out_growths.hp);
    nourstest_true(in_growths.str   == out_growths.str);
    nourstest_true(in_growths.mag   == out_growths.mag);
    nourstest_true(in_growths.agi   == out_growths.agi);
    nourstest_true(in_growths.dex   == out_growths.dex);
    nourstest_true(in_growths.luck  == out_growths.luck);
    nourstest_true(in_growths.def   == out_growths.def);
    nourstest_true(in_growths.res   == out_growths.res);
    nourstest_true(in_growths.con   == out_growths.con);
    nourstest_true(in_growths.move  == out_growths.move);
    nourstest_true(in_growths.prof  == out_growths.prof);

    // base_stats and current_stats are not the same
    unit1.current_stats.hp      += 1;
    unit1.current_stats.str     += 1;
    unit1.current_stats.mag     += 1;
    unit1.current_stats.agi     += 1;
    unit1.current_stats.dex     += 1;
    unit1.current_stats.luck    += 1;
    unit1.current_stats.def     += 1;
    unit1.current_stats.res     += 1;
    unit1.current_stats.con     += 1;
    unit1.current_stats.move    += 1;
    unit1.current_stats.prof    += 1;
    nourstest_true(unit1.base_stats.hp   != unit1.current_stats.hp);
    nourstest_true(unit1.base_stats.str  != unit1.current_stats.str);
    nourstest_true(unit1.base_stats.mag  != unit1.current_stats.mag);
    nourstest_true(unit1.base_stats.agi  != unit1.current_stats.agi);
    nourstest_true(unit1.base_stats.dex  != unit1.current_stats.dex);
    nourstest_true(unit1.base_stats.luck != unit1.current_stats.luck);
    nourstest_true(unit1.base_stats.def  != unit1.current_stats.def);
    nourstest_true(unit1.base_stats.res  != unit1.current_stats.res);
    nourstest_true(unit1.base_stats.con  != unit1.current_stats.con);
    nourstest_true(unit1.base_stats.move != unit1.current_stats.move);
    nourstest_true(unit1.base_stats.prof != unit1.current_stats.prof);

    Unit_lvlUp(&unit1);
    Unit_lvlUp(&unit1);
    out_stats = unit1.base_stats;
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
    out_stats = unit1.current_stats;
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
    Game_Weapons_Free(&weapons_dtab);
    Game_Weapons_Free(&weapons_dtab2);
}

void test_growth() {
    rng_sequence_breaker_miss_growth = true;
    struct Unit Silou = Unit_default;
    Unit_Init(&Silou);
    /*                              hp, str, mag, agi, dex, fth, luck, def, res, con, move, prof */
    struct Unit_stats in_stats = {  17,   6,   2,   7,   7,   7,    7,   4,   5,   6,   4,    2};
    struct Unit_stats in_caps = {   48,  14,  25,  32,  34,  28,   28,  19,  40,  15,   20,  25};
    struct Unit_stats in_growths = {60,  50,  20,  60,  70,  30,   40,  30,  20,  10,   10,  20};

    struct Unit_stats out_stats   = Unit_stats_default;
    struct Unit_stats out_caps    = Unit_stats_default;
    struct Unit_stats out_growths = Unit_stats_default;

    Silou.exp           = 0; /* lvl 1 */
    Silou.caps_stats    = in_caps;
    Silou.base_stats    = in_stats;
    Silou.current_stats = in_stats;
    Silou.growths       = in_growths;

    URN_debug       = 11;
    /* Testing the Sequence Breaker */
    /* After a sequence of 1 no level up, con and move SHould level up because of increased rate.
    /* First level up: con and move don't grow*/
    Unit_lvlUp(&Silou);
    /*                              hp, str, mag, agi, dex, fth, luck, def, res, con, move, prof */
    struct Unit_stats temp_growths = {1,   1,   1,   1,   1,   1,   1,    1,   1,   0,   0,  1};
    nourstest_true(Silou.grown_stats[0].hp      == temp_growths.hp);
    nourstest_true(Silou.grown_stats[0].str     == temp_growths.str);
    nourstest_true(Silou.grown_stats[0].agi     == temp_growths.agi);
    nourstest_true(Silou.grown_stats[0].dex     == temp_growths.dex);
    nourstest_true(Silou.grown_stats[0].fth     == temp_growths.fth);
    nourstest_true(Silou.grown_stats[0].luck    == temp_growths.luck);
    nourstest_true(Silou.grown_stats[0].def     == temp_growths.def);
    nourstest_true(Silou.grown_stats[0].res     == temp_growths.res);
    nourstest_true(Silou.grown_stats[0].con     == temp_growths.con);  /* 0 at first */
    nourstest_true(Silou.grown_stats[0].move    == temp_growths.move); /* 0 at first */
    nourstest_true(Silou.grown_stats[0].prof    == temp_growths.prof);
    nourstest_true(DARR_NUM(Silou.grown_stats)  == 1);

    nourstest_true(Silou.growths.move == 10);
    nourstest_true(Silou.growths.con  == 10);
    nourstest_true(Silou.growths.prof == 20);

    nourstest_true(Silou.res_sequence.len  == 0);
    nourstest_true(Silou.move_sequence.len == 1);
    nourstest_true(Silou.con_sequence.len  == 1);
    nourstest_true(Silou.prof_sequence.len == 0);

    /* Second level up: con and move grow, cause of increased rate */
    Unit_lvlUp(&Silou);
    nourstest_true(DARR_NUM(Silou.grown_stats) == 2);

    nourstest_true(Silou.grown_stats[1].hp      == temp_growths.hp);
    nourstest_true(Silou.grown_stats[1].str     == temp_growths.str);
    nourstest_true(Silou.grown_stats[1].agi     == temp_growths.agi);
    nourstest_true(Silou.grown_stats[1].dex     == temp_growths.dex);
    nourstest_true(Silou.grown_stats[1].fth     == temp_growths.fth);
    nourstest_true(Silou.grown_stats[1].luck    == temp_growths.luck);
    nourstest_true(Silou.grown_stats[1].def     == temp_growths.def);
    nourstest_true(Silou.grown_stats[1].res     == temp_growths.res);
    nourstest_true(Silou.grown_stats[1].con     == 1); /* grows now, SB eff rate is 10*1.2 = 12 > 11  */
    nourstest_true(Silou.grown_stats[1].move    == 1); /* grows now, SB eff rate is 10*1.2 = 12 > 11  */
    nourstest_true(Silou.grown_stats[1].prof    == temp_growths.prof);

    nourstest_true(Silou.con_sequence.len    == 0);
    nourstest_true(Silou.move_sequence.len   == 0);
    nourstest_true(Silou.prof_sequence.len   == 0);

    nourstest_true(Silou.prof_sequence.eff_rate == in_growths.prof);
    nourstest_true(Silou.move_sequence.eff_rate  > in_growths.move);
    nourstest_true(Silou.con_sequence.eff_rate   > in_growths.con);

    /* Runing of sequence breaker: no more sequences, no more increasing rates */
    rng_sequence_breaker_miss_growth = false;
    Unit_lvlUp(&Silou);
    nourstest_true(DARR_NUM(Silou.grown_stats) == 3);
    nourstest_true(Silou.grown_stats[2].hp      == temp_growths.hp);
    nourstest_true(Silou.grown_stats[2].str     == temp_growths.str);
    nourstest_true(Silou.grown_stats[2].agi     == temp_growths.agi);
    nourstest_true(Silou.grown_stats[2].dex     == temp_growths.dex);
    nourstest_true(Silou.grown_stats[2].fth     == temp_growths.fth);
    nourstest_true(Silou.grown_stats[2].luck    == temp_growths.luck);
    nourstest_true(Silou.grown_stats[2].def     == temp_growths.def);
    nourstest_true(Silou.grown_stats[2].res     == temp_growths.res);
    nourstest_true(Silou.grown_stats[2].con     == temp_growths.con);
    nourstest_true(Silou.grown_stats[2].move    == temp_growths.move);
    nourstest_true(Silou.grown_stats[2].prof    == temp_growths.prof);

    nourstest_true(Silou.res_sequence.len  == 0);
    nourstest_true(Silou.con_sequence.len  == 0);
    nourstest_true(Silou.move_sequence.len == 0);
    nourstest_true(Silou.prof_sequence.len == 0);

    Unit_lvlUp(&Silou);
    nourstest_true(DARR_NUM(Silou.grown_stats)  == 4);
    nourstest_true(Silou.grown_stats[3].con     == temp_growths.con);
    nourstest_true(Silou.grown_stats[3].move    == temp_growths.move);
    nourstest_true(Silou.grown_stats[3].prof    == temp_growths.prof);
    nourstest_true(Silou.prof_sequence.len      == 0);
    nourstest_true(Silou.move_sequence.len      == 0);
    nourstest_true(Silou.con_sequence.len       == 0);

    Unit_Free(&Silou);
}

void test_wpn_or_item() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    struct Unit Silou = Unit_default;
    Unit_Init(&Silou);
    struct dtab *weapons_dtab   = DTAB_INIT(weapons_dtab, struct Weapon);
    struct dtab *items_dtab     = DTAB_INIT(items_dtab,   struct Item);
    Silou.weapons_dtab  = weapons_dtab;
    Silou.items_dtab    = items_dtab;

    Silou.handedness = UNIT_HAND_LEFTIE;
    Silou._equipment[0].id = ITEM_ID_RETRACTABLE_WRISTBLADE;
    Silou._equipment[1].id = ITEM_ID_REPEATABLE_CROSSBOW;
    Silou._equipment[2].id = ITEM_ID_HONJOU_MASAMUNE;
    Silou._equipment[3].id = ITEM_ID_SILVERLIGHT_SPEAR;
    Silou._equipment[4].id = ITEM_ID_PANACEA;

    int eq_id = 0;
    struct WpnorItem wpn_or_item = Unit_WpnorItem(&Silou, eq_id);
    nourstest_true(wpn_or_item.dtab          == SOTA_DTAB_WEAPONS);
    nourstest_true(wpn_or_item.wpn->item->id == Silou._equipment[eq_id].id);
    nourstest_true(wpn_or_item.wpn->item->id == ITEM_ID_RETRACTABLE_WRISTBLADE);
    nourstest_true(wpn_or_item.item          == NULL);

    eq_id = 1;
    wpn_or_item = Unit_WpnorItem(&Silou, eq_id);
    nourstest_true(wpn_or_item.dtab          == SOTA_DTAB_WEAPONS);
    nourstest_true(wpn_or_item.wpn->item->id == Silou._equipment[eq_id].id);
    nourstest_true(wpn_or_item.wpn->item->id == ITEM_ID_REPEATABLE_CROSSBOW);
    nourstest_true(wpn_or_item.item          == NULL);

    eq_id = 2;
    wpn_or_item = Unit_WpnorItem(&Silou, eq_id);
    nourstest_true(wpn_or_item.dtab          == SOTA_DTAB_WEAPONS);
    nourstest_true(wpn_or_item.wpn->item->id == Silou._equipment[eq_id].id);
    nourstest_true(wpn_or_item.wpn->item->id == ITEM_ID_HONJOU_MASAMUNE);
    nourstest_true(wpn_or_item.item          == NULL);

    eq_id = 3;
    wpn_or_item = Unit_WpnorItem(&Silou, eq_id);
    nourstest_true(wpn_or_item.dtab          == SOTA_DTAB_WEAPONS);
    nourstest_true(wpn_or_item.wpn->item->id == Silou._equipment[eq_id].id);
    nourstest_true(wpn_or_item.wpn->item->id == ITEM_ID_SILVERLIGHT_SPEAR);
    nourstest_true(wpn_or_item.item          == NULL);

    eq_id = 4;
    wpn_or_item = Unit_WpnorItem(&Silou, eq_id);
    nourstest_true(wpn_or_item.dtab          == SOTA_DTAB_ITEMS);
    nourstest_true(wpn_or_item.item->id      == Silou._equipment[eq_id].id);
    nourstest_true(wpn_or_item.item->id      == ITEM_ID_PANACEA);
    nourstest_true(wpn_or_item.wpn           == NULL);

    Game_Items_Free(&items_dtab);
    Game_Weapons_Free(&weapons_dtab);
    Unit_Free(&Silou);
}

void test_reinforcements() {
    struct Unit Corsair = Unit_default;
    struct Reinforcement reinf = Reinforcement_default;
    reinf.levelups = 2;
    Unit_Init(&Corsair);

    Unit_Reinforcement_Levelups(&Corsair, &reinf);
    nourstest_true(DARR_NUM(Corsair.grown_stats) == reinf.levelups);
    Unit_Reinforcement_Levelups(&Corsair, &reinf);
    nourstest_true(DARR_NUM(Corsair.grown_stats) == reinf.levelups);

    Unit_Free(&Corsair);
}

void test_bonus_decay() {
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
    struct Unit Silou = Unit_default;
    Silou.bonus_stack   = DARR_INIT(Silou.bonus_stack,  struct Bonus_Stats, 2);
    Unit_Bonus_Add(&Silou, bonus1);
    Unit_Bonus_Add(&Silou, bonus2);
    Unit_Bonus_Add(&Silou, bonus3);
    Unit_Bonus_Add(&Silou, bonus4);
    Unit_Bonus_Add(&Silou, bonus5);
    nourstest_true(DARR_NUM(Silou.bonus_stack) == 5);
    nourstest_true(Silou.bonus_stack[0].turns == 3);
    nourstest_true(Silou.bonus_stack[1].turns == 0);
    nourstest_true(Silou.bonus_stack[2].turns == 3);
    nourstest_true(Silou.bonus_stack[3].turns == 1);
    nourstest_true(Silou.bonus_stack[4].turns == 2);

    Unit_Bonus_Persistent_Decay(&Silou);
    nourstest_true(DARR_NUM(Silou.bonus_stack) == 4);
    nourstest_true(Silou.bonus_stack[0].turns == 2);
    nourstest_true(Silou.bonus_stack[1].turns == 2);
    nourstest_true(Silou.bonus_stack[2].turns == 0);
    nourstest_true(Silou.bonus_stack[3].turns == 1);

    Unit_Bonus_Persistent_Decay(&Silou);
    nourstest_true(DARR_NUM(Silou.bonus_stack) == 3);
    nourstest_true(Silou.bonus_stack[0].turns == 1);
    nourstest_true(Silou.bonus_stack[1].turns == 1);
    nourstest_true(Silou.bonus_stack[2].turns == 0);

    Unit_Bonus_Persistent_Decay(&Silou);

    nourstest_true(DARR_NUM(Silou.bonus_stack) == 2);
    nourstest_true(Silou.bonus_stack[0].turns == 0);
    nourstest_true(Silou.bonus_stack[1].turns == 0);

    Unit_Bonus_Persistent_Decay(&Silou);
    nourstest_true(DARR_NUM(Silou.bonus_stack) == 0);

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
    nourstest_true(DARR_NUM(Silou.bonus_stack) == 3);
    Unit_Bonus_Instant_Decay(&Silou);
    nourstest_true(DARR_NUM(Silou.bonus_stack) == 1);

    Unit_Bonus_Add(&Silou, bonus_instant1);
    Unit_Bonus_Add(&Silou, bonus_instant3);
    Unit_Bonus_Persistent_Decay(&Silou);
    nourstest_true(DARR_NUM(Silou.bonus_stack) == 2);

    /* Free */
    DARR_FREE(Silou.bonus_stack);
}

void test_bonus_stats() {
    struct Bonus_Stats bonus1 = Bonus_Stats_default;
    struct Unit Silou = Unit_default;
    //                              hp, str, mag, agi, dex, fth, luck, def, res, con, move
    struct Unit_stats unit_stats    = {1,  6,  2,  7,  7, 7,  7,  4,  5,  6, 5};
    Silou.base_stats    = unit_stats;
    Silou.current_stats = unit_stats;

    /* Adding bonus stats*/
    struct Unit_stats bonus_stats   = {1,  1,  1,  1,  1, 1,  1,  1,  1,  1, 1};
    bonus1.unit_stats = bonus_stats;
    Silou.bonus_stack   = DARR_INIT(Silou.bonus_stack,  struct Bonus_Stats, 2);
    Unit_Bonus_Add(&Silou, bonus1);

    struct Unit_stats effective_stats = Unit_effectiveStats(&Silou);

    nourstest_true(effective_stats.hp   == (Silou.current_stats.hp      + bonus1.unit_stats.hp));
    nourstest_true(effective_stats.str  == (Silou.current_stats.str     + bonus1.unit_stats.str));
    nourstest_true(effective_stats.mag  == (Silou.current_stats.mag     + bonus1.unit_stats.mag));
    nourstest_true(effective_stats.agi  == (Silou.current_stats.agi     + bonus1.unit_stats.agi));
    nourstest_true(effective_stats.dex  == (Silou.current_stats.dex     + bonus1.unit_stats.dex));
    nourstest_true(effective_stats.fth  == (Silou.current_stats.fth     + bonus1.unit_stats.fth));
    nourstest_true(effective_stats.luck == (Silou.current_stats.luck    + bonus1.unit_stats.luck));
    nourstest_true(effective_stats.def  == (Silou.current_stats.def     + bonus1.unit_stats.def));
    nourstest_true(effective_stats.res  == (Silou.current_stats.res     + bonus1.unit_stats.res));
    nourstest_true(effective_stats.con  == (Silou.current_stats.con     + bonus1.unit_stats.con));
    nourstest_true(effective_stats.move == (Silou.current_stats.move    + bonus1.unit_stats.move));
    nourstest_true(effective_stats.prof == (Silou.current_stats.prof    + bonus1.unit_stats.prof));

    /* Adding bonus_stats2 */
    struct Unit_stats bonus_stats2   = {0,  2,  3,  4,  5, 6,  7,  8,  9,  10, 11};
    bonus1.unit_stats   = bonus_stats2;
    bonus1.turns        = 1;
    Unit_Bonus_Add(&Silou, bonus1);

    effective_stats = Unit_effectiveStats(&Silou);

    nourstest_true(effective_stats.hp   == (Silou.current_stats.hp      + bonus_stats.hp    +
                                            bonus_stats2.hp));
    nourstest_true(effective_stats.str  == (Silou.current_stats.str     + bonus_stats.str   +
                                            bonus_stats2.str));
    nourstest_true(effective_stats.mag  == (Silou.current_stats.mag     + bonus_stats.mag   +
                                            bonus_stats2.mag));
    nourstest_true(effective_stats.agi  == (Silou.current_stats.agi     + bonus_stats.agi   +
                                            bonus_stats2.agi));
    nourstest_true(effective_stats.dex  == (Silou.current_stats.dex     + bonus_stats.dex   +
                                            bonus_stats2.dex));
    nourstest_true(effective_stats.fth  == (Silou.current_stats.fth     + bonus_stats.fth   +
                                            bonus_stats2.fth));
    nourstest_true(effective_stats.luck == (Silou.current_stats.luck    + bonus_stats.luck  +
                                            bonus_stats2.luck));
    nourstest_true(effective_stats.def  == (Silou.current_stats.def     + bonus_stats.def   +
                                            bonus_stats2.def));
    nourstest_true(effective_stats.res  == (Silou.current_stats.res     + bonus_stats.res   +
                                            bonus_stats2.res));
    nourstest_true(effective_stats.con  == (Silou.current_stats.con     + bonus_stats.con   +
                                            bonus_stats2.con));
    nourstest_true(effective_stats.move == (Silou.current_stats.move    + bonus_stats.move  +
                                            bonus_stats2.move));
    nourstest_true(effective_stats.prof == (Silou.current_stats.prof    + bonus_stats.prof  +
                                            bonus_stats2.prof));

    Unit_Bonus_Persistent_Decay(&Silou);

    effective_stats = Unit_effectiveStats(&Silou);

    nourstest_true(effective_stats.hp   == (Silou.current_stats.hp      + bonus_stats2.hp));
    nourstest_true(effective_stats.str  == (Silou.current_stats.str     + bonus_stats2.str));
    nourstest_true(effective_stats.mag  == (Silou.current_stats.mag     + bonus_stats2.mag));
    nourstest_true(effective_stats.agi  == (Silou.current_stats.agi     + bonus_stats2.agi));
    nourstest_true(effective_stats.dex  == (Silou.current_stats.dex     + bonus_stats2.dex));
    nourstest_true(effective_stats.fth  == (Silou.current_stats.fth     + bonus_stats2.fth));
    nourstest_true(effective_stats.luck == (Silou.current_stats.luck    + bonus_stats2.luck));
    nourstest_true(effective_stats.def  == (Silou.current_stats.def     + bonus_stats2.def));
    nourstest_true(effective_stats.res  == (Silou.current_stats.res     + bonus_stats2.res));
    nourstest_true(effective_stats.con  == (Silou.current_stats.con     + bonus_stats2.con));
    nourstest_true(effective_stats.move == (Silou.current_stats.move    + bonus_stats2.move));
    nourstest_true(effective_stats.prof == (Silou.current_stats.prof    + bonus_stats2.prof));

    Unit_Bonus_Persistent_Decay(&Silou);

    effective_stats = Unit_effectiveStats(&Silou);

    nourstest_true(effective_stats.hp   == (Silou.current_stats.hp));
    nourstest_true(effective_stats.str  == (Silou.current_stats.str));
    nourstest_true(effective_stats.mag  == (Silou.current_stats.mag));
    nourstest_true(effective_stats.agi  == (Silou.current_stats.agi));
    nourstest_true(effective_stats.dex  == (Silou.current_stats.dex));
    nourstest_true(effective_stats.fth  == (Silou.current_stats.fth));
    nourstest_true(effective_stats.luck == (Silou.current_stats.luck));
    nourstest_true(effective_stats.def  == (Silou.current_stats.def));
    nourstest_true(effective_stats.res  == (Silou.current_stats.res));
    nourstest_true(effective_stats.con  == (Silou.current_stats.con));
    nourstest_true(effective_stats.move == (Silou.current_stats.move));
    nourstest_true(effective_stats.prof == (Silou.current_stats.prof));

    /* Free */
    DARR_FREE(Silou.bonus_stack);
}

void test_unit() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);

    // test_promotion();
    // test_canEquip(); /* BROKEN */
    test_skills();
    test_wpn_or_item();
    // test_io();
    test_growth();
    test_bonus_decay();
    test_bonus_stats();
    test_reinforcements();

    URN_debug = -1;
}
