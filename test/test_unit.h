#include "minctest.h"
#include "platform.h"
#include "unit.h"

void test_promotion() {

}

void test_canEquip() {
    SDL_Log("test_canEquip");
    struct Unit equiper = Unit_default;
    Unit_setClassind(&equiper, UNIT_CLASS_MERCENARY);
    struct Weapons_hash * weapons1 = NULL;
    equiper.weapons = &weapons1;
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_DUKE);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_ARCHER);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_MARKSMAN);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_ANGEL);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_RADIANT_CLAW) == 1);

    Unit_setClassind(&equiper, UNIT_CLASS_DEMON);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 1);

    Unit_setClassind(&equiper, UNIT_CLASS_ANGELIC_INCARNATE);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_DEMONIC_INCARNATE);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_FENCER);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_DUELIST);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_MOUSQUETAIRE);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_PICKPOCKET);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_THIEF);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_ASSASSIN);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_MERCENARY);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_HERO);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_CORSAIR);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_IRON_AXE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_VIKING);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_STEEL_AXE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_RAVAGER);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_DAMAS_AXE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_PIKEMAN);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_IRON_LANCE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_MAGE);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_BATTLEMAGE);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_TROUBADOUR);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_SAGE);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_PRIEST);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_BISHOP);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_ORACLE);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);

    Unit_setClassind(&equiper, UNIT_CLASS_CLERIC);
    lok(Unit_canEquip(&equiper, ITEM_NAME_FLEURET) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MAIN_GAUCHE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_PITCHFORK) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HOE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_SHORT_BOW) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ADAPTED_SHIELD) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_POT_LID) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_ICICLE) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_NETHER) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_MOONLIGHT) == 0);
    lok(Unit_canEquip(&equiper, ITEM_NAME_HEAL) == 1);
    lok(Unit_canEquip(&equiper, ITEM_NAME_CLAW) == 0);
}


void test_combat_stats() {
    SDL_Log("test_combat_stats");
    struct Unit * attacker = (struct Unit *)malloc(sizeof(struct Unit));
    * attacker = Unit_default;
    struct Unit * defender = (struct Unit *)malloc(sizeof(struct Unit));
    * defender = Unit_default;
    struct Damage defender_damage;
    struct Damage attacker_damage;
    struct Combat_Forecast_Stats temp_stats;
    struct Computed_Stats attacker_computed;
    struct Computed_Stats defender_computed;
    // struct Unit defender = Unit_default;
    struct Point attacker_pos = Point_default;
    struct Point defender_pos = Point_default;
    //hp,str,mag,agi,dex,luck,def,res,con,move
    struct Unit_stats attacker_stats = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    struct Unit_stats defender_stats = {18,  7,  3,  8,  8,   8,  5,  6,  7, 6};
    struct Weapons_hash * weapons1 = NULL;
    hmdefault(weapons1, Weapon_default);
    Unit_setClassind(defender, UNIT_CLASS_MERCENARY);
    Unit_setClassind(attacker, UNIT_CLASS_MERCENARY);

    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id = ITEM_NAME_FLEURET;
    Unit_Item_Add(defender, in_wpn);
    Unit_Item_Add(attacker, in_wpn);
    attacker->weapons = &weapons1;
    defender->weapons = &weapons1;
    bool attacker_equip_hand = UNIT_HAND_RIGHT;
    bool defender_equip_hand = UNIT_HAND_RIGHT;
    Unit_equips(defender, attacker_equip_hand);
    Unit_equips(attacker, defender_equip_hand);
    lok(Unit_iswpnEquipped(defender));
    lok(Unit_iswpnEquipped(attacker));
    Unit_setStats(defender, defender_stats);
    Unit_setStats(attacker, attacker_stats);

    lok(attacker->current_hp == attacker_stats.hp);
    lok(defender->current_hp == defender_stats.hp);
    Unit_effectiveStats(attacker);
    Unit_effectiveStats(defender);

    Unit_computedStats(attacker);
    Unit_computedStats(defender);


    attacker_computed = Unit_computedStats(attacker);
    defender_computed = Unit_computedStats(defender);
    temp_stats = Unit_Combat_Forecast_Stats(attacker, defender);
    lok((temp_stats.attacker_rates.hit == equation_rateHit(attacker_computed.hit, defender_computed.dodge)));
    lok((temp_stats.attacker_rates.hit > 0));
    lok((temp_stats.attacker_rates.crit == equation_rateCrit(attacker_computed.crit, defender_computed.favor)));
    lok((temp_stats.defender_rates.hit == equation_rateHit(defender_computed.hit, attacker_computed.dodge)));
    lok((temp_stats.defender_rates.hit > 0));
    lok((temp_stats.defender_rates.crit == equation_rateCrit(defender_computed.crit, attacker_computed.favor)));

    attacker_damage = Unit_computeDamage(attacker, defender);
    defender_damage = Unit_computeDamage(defender, attacker);

    lok((temp_stats.attacker_damage.dmg[DMG_TYPE_PHYSICAL] == attacker_damage.dmg[DMG_TYPE_PHYSICAL]));
    lok((temp_stats.attacker_damage.dmg[DMG_TYPE_MAGICAL] == attacker_damage.dmg[DMG_TYPE_MAGICAL]));
    lok((temp_stats.attacker_damage.dmg[DMG_TYPE_PHYSICAL] > 0));
    lok((temp_stats.attacker_damage.dmg[DMG_TYPE_MAGICAL] == 0));
    lok((temp_stats.defender_damage.dmg[DMG_TYPE_PHYSICAL] == defender_damage.dmg[DMG_TYPE_PHYSICAL]));
    lok((temp_stats.defender_damage.dmg[DMG_TYPE_MAGICAL] == defender_damage.dmg[DMG_TYPE_MAGICAL]));
    lok((temp_stats.defender_damage.dmg[DMG_TYPE_PHYSICAL] > 0));
    lok((temp_stats.defender_damage.dmg[DMG_TYPE_MAGICAL] == 0));
    lok((temp_stats.attacker_computed.hp == attacker_computed.hp));
    lok((temp_stats.attacker_computed.hp == attacker_stats.hp));
    lok((temp_stats.attacker_computed.hp == attacker_computed.hp));
    lok((temp_stats.attacker_computed.hp > 0));
    lok((temp_stats.attacker_computed.attack[0] == attacker_computed.attack[0]));
    lok((temp_stats.attacker_computed.attack[0] > 0));
    lok((temp_stats.attacker_computed.attack[1] == attacker_computed.attack[1]));
    lok((temp_stats.attacker_computed.attack[1] == 0));
    lok((temp_stats.attacker_computed.defense[0] == attacker_computed.defense[0]));
    lok((temp_stats.attacker_computed.defense[0] > 0));
    lok((temp_stats.attacker_computed.defense[1] == attacker_computed.defense[1]));
    lok((temp_stats.attacker_computed.defense[1] > 0));
    lok((temp_stats.attacker_computed.dodge == attacker_computed.dodge));
    lok((temp_stats.attacker_computed.favor == attacker_computed.favor));
    lok((temp_stats.attacker_computed.agony == attacker_computed.agony));
    lok((temp_stats.attacker_computed.speed == attacker_computed.speed));
    lok((temp_stats.attacker_computed.speed == attacker_computed.speed));
    lok((temp_stats.attacker_computed.move == attacker_stats.move));

    lok((temp_stats.defender_damage.dmg[DMG_TYPE_PHYSICAL] == defender_damage.dmg[DMG_TYPE_PHYSICAL]));
    lok((temp_stats.defender_damage.dmg[DMG_TYPE_MAGICAL] == defender_damage.dmg[DMG_TYPE_MAGICAL]));
    lok((temp_stats.defender_damage.dmg[DMG_TYPE_PHYSICAL] > 0));
    lok((temp_stats.defender_damage.dmg[DMG_TYPE_MAGICAL] == 0));
    lok((temp_stats.defender_damage.dmg[DMG_TYPE_PHYSICAL] == defender_damage.dmg[DMG_TYPE_PHYSICAL]));
    lok((temp_stats.defender_damage.dmg[DMG_TYPE_MAGICAL] == defender_damage.dmg[DMG_TYPE_MAGICAL]));
    lok((temp_stats.defender_damage.dmg[DMG_TYPE_PHYSICAL] > 0));
    lok((temp_stats.defender_damage.dmg[DMG_TYPE_MAGICAL] == 0));
    lok((temp_stats.defender_computed.hp == defender_computed.hp));
    lok((temp_stats.defender_computed.hp == defender_stats.hp));
    lok((temp_stats.defender_computed.hp == defender_computed.hp));
    lok((temp_stats.defender_computed.hp > 0));
    lok((temp_stats.defender_computed.attack[0] == defender_computed.attack[0]));
    lok((temp_stats.defender_computed.attack[0] > 0));
    lok((temp_stats.defender_computed.attack[1] == defender_computed.attack[1]));
    lok((temp_stats.defender_computed.attack[1] == 0));
    lok((temp_stats.defender_computed.defense[0] == defender_computed.defense[0]));
    lok((temp_stats.defender_computed.defense[0] > 0));
    lok((temp_stats.defender_computed.defense[1] == defender_computed.defense[1]));
    lok((temp_stats.defender_computed.defense[1] > 0));
    lok((temp_stats.defender_computed.dodge == defender_computed.dodge));
    lok((temp_stats.defender_computed.favor == defender_computed.favor));
    lok((temp_stats.defender_computed.agony == defender_computed.agony));
    lok((temp_stats.defender_computed.speed == defender_computed.speed));
    lok((temp_stats.defender_computed.speed == defender_computed.speed));
    lok((temp_stats.defender_computed.move == defender_stats.move));

}

void test_combat_death() {
    SDL_Log("test_combat_death");
    struct Unit attacker = Unit_default;
    struct Unit defender = Unit_default;
    struct Damage defender_damage;
    struct Damage attacker_damage;
    struct Combat_Forecast_Stats temp_stats;
    struct Combat_Forecast_Flow temp_flow;
    struct Combat_Forecast_Death temp_death;
    struct Computed_Stats attacker_computed;
    struct Computed_Stats defender_computed;
    // struct Unit defender = Unit_default;
    struct Point attacker_pos = Point_default;
    struct Point defender_pos = Point_default;
    //hp,str,mag,agi,dex,luck,def,res,con,move
    struct Unit_stats attacker_stats = {17,  6,  2,  7,  7,   7,  1,  2,  6, 5};
    struct Unit_stats defender_stats = {17,  6,  2,  7,  7,   7,  1,  2,  6, 5};
    struct Weapons_hash * weapons1 = NULL;
    hmdefault(weapons1, Weapon_default);
    Unit_setClassind(&defender, UNIT_CLASS_MERCENARY);
    Unit_setClassind(&attacker, UNIT_CLASS_MERCENARY);

    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id = ITEM_NAME_FLEURET;
    Unit_Item_Add(&defender, in_wpn);
    Unit_Item_Add(&attacker, in_wpn);
    attacker.weapons = &weapons1;
    defender.weapons = &weapons1;
    bool attacker_equip_hand = UNIT_HAND_RIGHT;
    bool defender_equip_hand = UNIT_HAND_RIGHT;
    Unit_equips(&defender, attacker_equip_hand);
    Unit_equips(&attacker, defender_equip_hand);
    lok(Unit_iswpnEquipped(&defender));
    lok(Unit_iswpnEquipped(&attacker));
    Unit_setStats(&defender, defender_stats);
    Unit_setStats(&attacker, attacker_stats);

    Unit_effectiveStats(&attacker);
    Unit_effectiveStats(&defender);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    Unit_computedStats(&attacker);
    Unit_computedStats(&defender);

    temp_stats = Unit_Combat_Forecast_Stats(&attacker, &defender);
    temp_flow = Unit_Combat_Forecast_Flow(&attacker, &attacker_pos, &defender, &defender_pos);
    temp_death = Unit_Combat_Forecast_Death(temp_stats, temp_flow);
    lok(temp_flow.defender_retaliates == Unit_canAttack(&defender, &defender_pos, &attacker, &attacker_pos));
    lok(temp_flow.attacker_doubles == 0);
    lok(temp_flow.defender_doubles == 0);
    lok((temp_death.attacker_certain == false));
    lok((temp_death.defender_certain == false));
    lok((temp_death.attacker_possible == false));
    lok((temp_death.defender_possible == false));

    attacker.current_hp = 1;
    Unit_computedStats(&attacker);
    Unit_computedStats(&defender);
    lok((attacker.current_hp == 1));
    temp_stats = Unit_Combat_Forecast_Stats(&attacker, &defender);
    temp_flow = Unit_Combat_Forecast_Flow(&attacker, &attacker_pos, &defender, &defender_pos);
    temp_death = Unit_Combat_Forecast_Death(temp_stats, temp_flow);
    lok(temp_flow.defender_retaliates == Unit_canAttack(&defender, &defender_pos, &attacker, &attacker_pos));
    lok(temp_flow.attacker_doubles == 0);
    lok(temp_flow.defender_doubles == 0);
    lok((temp_death.attacker_certain == true));
    lok((temp_death.defender_certain == false));
    lok((temp_death.attacker_possible == false));
    lok((temp_death.defender_possible == false));

    attacker.current_hp = 17;
    defender.current_hp = 1;
    Unit_computedStats(&attacker);
    Unit_computedStats(&defender);
    lok((attacker.current_hp == 17));
    lok((defender.current_hp == 1));
    temp_stats = Unit_Combat_Forecast_Stats(&attacker, &defender);
    temp_flow = Unit_Combat_Forecast_Flow(&attacker, &attacker_pos, &defender, &defender_pos);
    temp_death = Unit_Combat_Forecast_Death(temp_stats, temp_flow);
    lok(temp_flow.defender_retaliates == Unit_canAttack(&defender, &defender_pos, &attacker, &attacker_pos));
    lok(temp_flow.attacker_doubles == 0);
    lok(temp_flow.defender_doubles == 0);
    lok((temp_death.attacker_certain == false));
    lok((temp_death.defender_certain == true));
    lok((temp_death.attacker_possible == false));
    lok((temp_death.defender_possible == false));


    defender.current_hp = 17;
    attacker.current_hp = 1;
    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 3;
    Unit_computedStats(&attacker);
    Unit_computedStats(&defender);
    temp_stats = Unit_Combat_Forecast_Stats(&attacker, &defender);
    temp_flow = Unit_Combat_Forecast_Flow(&attacker, &attacker_pos, &defender, &defender_pos);
    temp_death = Unit_Combat_Forecast_Death(temp_stats, temp_flow);
    lok(temp_flow.defender_retaliates == Unit_canAttack(&defender, &defender_pos, &attacker, &attacker_pos));
    lok(temp_flow.attacker_doubles == 0);
    lok(temp_flow.defender_doubles == 0);
    lok((temp_death.attacker_certain == false));
    lok((temp_death.defender_certain == false));
    lok((temp_death.attacker_possible == false));
    lok((temp_death.defender_possible == false));

    attacker.current_hp = 1;
    defender.current_hp = 1;
    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    Unit_computedStats(&attacker);
    Unit_computedStats(&defender);
    lok((attacker.current_hp == 1));
    temp_stats = Unit_Combat_Forecast_Stats(&attacker, &defender);
    temp_flow = Unit_Combat_Forecast_Flow(&attacker, &attacker_pos, &defender, &defender_pos);
    temp_death = Unit_Combat_Forecast_Death(temp_stats, temp_flow);
    lok(temp_flow.defender_retaliates == Unit_canAttack(&defender, &defender_pos, &attacker, &attacker_pos));
    lok(temp_flow.attacker_doubles == 0);
    lok(temp_flow.defender_doubles == 0);
    lok((temp_death.attacker_certain == false));
    lok((temp_death.defender_certain == false));
    lok((temp_death.attacker_possible == true));
    lok((temp_death.defender_possible == true));

}

void test_combat_flow() {
    SDL_Log("test_combat_flow");
    struct Combat_Forecast_Flow temp_flow;
    struct Unit attacker = Unit_default;
    struct Unit defender = Unit_default;
    struct Point attacker_pos = Point_default;
    struct Point defender_pos = Point_default;
    //hp, str, mag, agi, dex, luck, def, res, con, move
    struct Unit_stats attacker_stats = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    struct Unit_stats defender_stats = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    struct Weapons_hash * weapons1 = NULL;
    hmdefault(weapons1, Weapon_default);
    Unit_setClassind(&defender, UNIT_CLASS_MERCENARY);
    Unit_setClassind(&attacker, UNIT_CLASS_MERCENARY);

    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id = ITEM_NAME_FLEURET;
    Unit_Item_Add(&defender, in_wpn);
    Unit_Item_Add(&attacker, in_wpn);
    attacker.weapons = &weapons1;
    defender.weapons = &weapons1;
    bool attacker_equip_hand = UNIT_HAND_RIGHT;
    bool defender_equip_hand = UNIT_HAND_RIGHT;
    Unit_equips(&defender, attacker_equip_hand);
    Unit_equips(&attacker, defender_equip_hand);
    lok(Unit_iswpnEquipped(&defender));
    lok(Unit_iswpnEquipped(&attacker));
    Unit_setStats(&defender, defender_stats);
    Unit_setStats(&attacker, attacker_stats);
    lok(attacker.current_hp == attacker_stats.hp);
    lok(defender.current_hp == defender_stats.hp);
    Unit_computedStats(&attacker);
    Unit_computedStats(&defender);

    Unit_effectiveStats(&attacker);
    lok(attacker.current_stats.hp == attacker.effective_stats.hp);
    lok(attacker.current_stats.str == attacker.effective_stats.str);
    lok(attacker.current_stats.mag == attacker.effective_stats.mag);
    lok(attacker.current_stats.agi == attacker.effective_stats.agi);
    lok(attacker.current_stats.dex == attacker.effective_stats.dex);
    lok(attacker.current_stats.luck == attacker.effective_stats.luck);
    lok(attacker.current_stats.def == attacker.effective_stats.def);
    lok(attacker.current_stats.res == attacker.effective_stats.res);
    lok(attacker.current_stats.con == attacker.effective_stats.con);
    lok(attacker.current_stats.move == attacker.effective_stats.move);
    lok(attacker.current_stats.prof == attacker.effective_stats.prof);

    Unit_effectiveStats(&defender);
    lok(defender.current_stats.hp == defender.effective_stats.hp);
    lok(defender.current_stats.str == defender.effective_stats.str);
    lok(defender.current_stats.mag == defender.effective_stats.mag);
    lok(defender.current_stats.agi == defender.effective_stats.agi);
    lok(defender.current_stats.dex == defender.effective_stats.dex);
    lok(defender.current_stats.luck == defender.effective_stats.luck);
    lok(defender.current_stats.def == defender.effective_stats.def);
    lok(defender.current_stats.res == defender.effective_stats.res);
    lok(defender.current_stats.con == defender.effective_stats.con);
    lok(defender.current_stats.move == defender.effective_stats.move);
    lok(defender.current_stats.prof == defender.effective_stats.prof);

    Unit_computeHit(&attacker);
    Unit_computeHit(&defender);
    uint8_t attacker_hit;
    uint8_t defender_hit;
    struct Weapon attacker_weapon = Weapon_default;
    struct Weapon defender_weapon = Weapon_default;


    if (defender.equipped[defender_equip_hand]) {
        defender_weapon = hmget(weapons1, defender._equipment[defender_equip_hand].id);
    }
    if (attacker.equipped[attacker_equip_hand]) {
        attacker_weapon = hmget(weapons1, attacker._equipment[attacker_equip_hand].id);
    }

    attacker_hit = equation_unitHit(attacker_weapon.stats.combat.hit, attacker.effective_stats.dex, attacker.effective_stats.luck, 0);
    lok(attacker.computed_stats.hit == attacker_hit);
    defender_hit = equation_unitHit(defender_weapon.stats.combat.hit, defender.effective_stats.dex, defender.effective_stats.luck, 0);
    lok(defender.computed_stats.hit == defender_hit);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;

    Unit_computedStats(&attacker);
    Unit_computedStats(&defender);
    int8_t attacker_speed = equation_unitSpeed(attacker_weapon.stats.wgt, attacker.effective_stats.agi, attacker.effective_stats.con, attacker.effective_stats.str);
    lok(Unit_computeSpeed(&attacker) == attacker_speed);
    lok(attacker.computed_stats.speed == attacker_speed);
    lok(attacker.computed_stats.speed == 7);
    int8_t defender_speed = equation_unitSpeed(defender_weapon.stats.wgt, defender.effective_stats.agi, defender.effective_stats.con, defender.effective_stats.str);
    lok(Unit_computeSpeed(&defender) == defender_speed);
    lok(defender.computed_stats.speed == defender_speed);
    lok(defender.computed_stats.speed == 7);
    temp_flow = Unit_Combat_Forecast_Flow(&attacker, &attacker_pos, &defender, &defender_pos);
    lok(temp_flow.defender_retaliates == Unit_canAttack(&defender, &defender_pos, &attacker, &attacker_pos));
    lok(temp_flow.attacker_doubles == 0);
    lok(temp_flow.defender_doubles == 0);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 3;

    Unit_computedStats(&attacker);
    Unit_computedStats(&defender);
    attacker_speed = equation_unitSpeed(attacker_weapon.stats.wgt, attacker.effective_stats.agi, attacker.effective_stats.con, attacker.effective_stats.str);
    lok(Unit_computeSpeed(&attacker) == attacker_speed);
    lok(attacker.computed_stats.speed == attacker_speed);
    defender_speed = equation_unitSpeed(defender_weapon.stats.wgt, defender.effective_stats.agi, defender.effective_stats.con, defender.effective_stats.str);
    lok(Unit_computeSpeed(&defender) == defender_speed);
    lok(defender.computed_stats.speed == defender_speed);
    temp_flow = Unit_Combat_Forecast_Flow(&attacker, &attacker_pos, &defender, &defender_pos);
    lok(temp_flow.defender_retaliates == Unit_canAttack(&defender, &defender_pos, &attacker, &attacker_pos));
    lok(temp_flow.defender_retaliates == 0);
    lok(temp_flow.attacker_doubles == 0);
    lok(temp_flow.defender_doubles == 0);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    attacker_stats.agi = 8;
    Unit_setStats(&attacker, attacker_stats);
    Unit_computedStats(&attacker);
    Unit_computedStats(&defender);
    lok(attacker.computed_stats.speed == 8);
    temp_flow = Unit_Combat_Forecast_Flow(&attacker, &attacker_pos, &defender, &defender_pos);
    lok(temp_flow.defender_retaliates == Unit_canAttack(&defender, &defender_pos, &attacker, &attacker_pos));
    lok(temp_flow.defender_retaliates == 1);
    lok(temp_flow.attacker_doubles == 0);
    lok(temp_flow.defender_doubles == 0);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    attacker_stats.agi = 9;
    Unit_setStats(&attacker, attacker_stats);
    Unit_computedStats(&attacker);
    Unit_computedStats(&defender);
    lok(attacker.computed_stats.speed == 9);
    temp_flow = Unit_Combat_Forecast_Flow(&attacker, &attacker_pos, &defender, &defender_pos);
    lok(temp_flow.defender_retaliates == Unit_canAttack(&defender, &defender_pos, &attacker, &attacker_pos));
    lok(temp_flow.defender_retaliates == 1);
    lok(temp_flow.attacker_doubles == 0);
    lok(temp_flow.defender_doubles == 0);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    attacker_stats.agi = 10;
    Unit_setStats(&attacker, attacker_stats);
    Unit_computedStats(&attacker);
    Unit_computedStats(&defender);
    lok(attacker.computed_stats.speed == 10);
    temp_flow = Unit_Combat_Forecast_Flow(&attacker, &attacker_pos, &defender, &defender_pos);
    lok(temp_flow.defender_retaliates == Unit_canAttack(&defender, &defender_pos, &attacker, &attacker_pos));
    lok(temp_flow.defender_retaliates == 1);
    lok(temp_flow.attacker_doubles == 0);
    lok(temp_flow.defender_doubles == 0);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    attacker_stats.agi = 11;
    Unit_setStats(&attacker, attacker_stats);
    Unit_computedStats(&attacker);
    Unit_computedStats(&defender);
    lok(attacker.computed_stats.speed == 11);
    temp_flow = Unit_Combat_Forecast_Flow(&attacker, &attacker_pos, &defender, &defender_pos);
    lok(temp_flow.defender_retaliates == Unit_canAttack(&defender, &defender_pos, &attacker, &attacker_pos));
    lok(temp_flow.defender_retaliates == 1);
    lok(temp_flow.attacker_doubles == 0);
    lok(temp_flow.defender_doubles == 0);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    attacker_stats.agi = 12;
    Unit_setStats(&attacker, attacker_stats);
    Unit_computedStats(&attacker);
    Unit_computedStats(&defender);
    lok(attacker.computed_stats.speed == 12);
    temp_flow = Unit_Combat_Forecast_Flow(&attacker, &attacker_pos, &defender, &defender_pos);
    lok(temp_flow.defender_retaliates == Unit_canAttack(&defender, &defender_pos, &attacker, &attacker_pos));
    lok(temp_flow.defender_retaliates == 1);
    lok(temp_flow.attacker_doubles == 1);
    lok(Unit_canDouble(&attacker, &defender));
    lok(temp_flow.defender_doubles == 0);

    temp_flow = Unit_Combat_Forecast_Flow(&defender, &defender_pos, &attacker, &attacker_pos);
    lok(temp_flow.defender_retaliates == Unit_canAttack(&attacker, &attacker_pos, &defender, &defender_pos));
    lok(temp_flow.defender_retaliates == 1);
    lok(temp_flow.attacker_doubles == 0);
    lok(Unit_canDouble(&attacker, &defender));
    lok(temp_flow.defender_doubles == 1);

}

void test_io() {
    SDL_Log("test_io");
    struct Unit unit1 = Unit_default;
    struct Unit unit2 = Unit_default;
    struct Unit unit3 = Unit_default;
    struct Unit unit4 = Unit_default;
    struct Unit unit5 = Unit_default;
    struct Unit_stats in_stats = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    struct Unit_stats in_caps = {48, 14, 25, 32, 34,  28, 19, 40, 15, 0};
    struct Unit_stats in_growths = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    struct Unit_stats out_stats = Unit_stats_default;
    struct Unit_stats out_caps = Unit_stats_default;
    struct Unit_stats out_growths = Unit_stats_default;
    struct Inventory_item in_wpn = Inventory_item_default;
    struct Inventory_item out_wpn = Inventory_item_default;
    struct Weapons_hash * weapons1 = NULL;
    hmdefault(weapons1, Weapon_default);
    unit1.weapons = &weapons1;
    struct Weapons_hash * weapons4 = NULL;
    hmdefault(weapons4, Weapon_default);
    unit4.weapons = &weapons4;

    Unit_setid(&unit1, UNIT_NAME_SILOU);
    Unit_setClassind(&unit1, UNIT_CLASS_MERCENARY);
    SDL_Log("Setting Sex");
    Unit_setSex(&unit1, UNIT_SEX_M);
    SDL_Log("Setting Sex");
    unit1.caps_stats = in_caps;
    unit1.base_stats = in_stats;
    unit1.current_stats = in_stats;
    unit1.growths = in_growths;
    unit1.base_exp = 0;
    unit1.exp = 0;
    in_wpn.id = ITEM_NAME_FLEURET;
    Unit_Item_Add(&unit1, in_wpn);
    in_wpn.id = ITEM_NAME_KITCHEN_KNIFE;
    Unit_Item_Add(&unit1, in_wpn);
    in_wpn.id = ITEM_NAME_POT_LID;
    Unit_Item_Add(&unit1, in_wpn);
    out_stats = unit1.current_stats;
    lok(strcmp(unit1.name, "Silou") == 0);
    lok(unit1._sex == 1);
    lok(in_stats.hp == out_stats.hp);
    lok(in_stats.str == out_stats.str);
    lok(in_stats.mag == out_stats.mag);
    lok(in_stats.agi == out_stats.agi);
    lok(in_stats.dex == out_stats.dex);
    lok(in_stats.luck == out_stats.luck);
    lok(in_stats.def == out_stats.def);
    lok(in_stats.res == out_stats.res);
    lok(in_stats.con == out_stats.con);
    lok(in_stats.move == out_stats.move);
    lok(in_stats.prof == out_stats.prof);
    out_growths = unit1.growths;
    lok(in_growths.hp == out_growths.hp);
    lok(in_growths.str == out_growths.str);
    lok(in_growths.mag == out_growths.mag);
    lok(in_growths.agi == out_growths.agi);
    lok(in_growths.dex == out_growths.dex);
    lok(in_growths.luck == out_growths.luck);
    lok(in_growths.def == out_growths.def);
    lok(in_growths.res == out_growths.res);
    lok(in_growths.con == out_growths.con);
    lok(in_growths.move == out_growths.move);
    lok(in_growths.prof == out_growths.prof);

    // base_stats and current_stats are not the same
    unit1.current_stats.hp += 1;
    unit1.current_stats.str += 1;
    unit1.current_stats.mag += 1;
    unit1.current_stats.agi += 1;
    unit1.current_stats.dex += 1;
    unit1.current_stats.luck += 1;
    unit1.current_stats.def += 1;
    unit1.current_stats.res += 1;
    unit1.current_stats.con += 1;
    unit1.current_stats.move += 1;
    unit1.current_stats.prof += 1;
    lok(unit1.base_stats.hp != unit1.current_stats.hp);
    lok(unit1.base_stats.str != unit1.current_stats.str);
    lok(unit1.base_stats.mag != unit1.current_stats.mag);
    lok(unit1.base_stats.agi != unit1.current_stats.agi);
    lok(unit1.base_stats.dex != unit1.current_stats.dex);
    lok(unit1.base_stats.luck != unit1.current_stats.luck);
    lok(unit1.base_stats.def != unit1.current_stats.def);
    lok(unit1.base_stats.res != unit1.current_stats.res);
    lok(unit1.base_stats.con != unit1.current_stats.con);
    lok(unit1.base_stats.move != unit1.current_stats.move);
    lok(unit1.base_stats.prof != unit1.current_stats.prof);

    Unit_lvlUp(&unit1);
    Unit_lvlUp(&unit1);
    out_stats = unit1.base_stats;
    lok(in_stats.hp == out_stats.hp);
    lok(in_stats.str == out_stats.str);
    lok(in_stats.mag == out_stats.mag);
    lok(in_stats.agi == out_stats.agi);
    lok(in_stats.dex == out_stats.dex);
    lok(in_stats.luck == out_stats.luck);
    lok(in_stats.def == out_stats.def);
    lok(in_stats.res == out_stats.res);
    lok(in_stats.con == out_stats.con);
    lok(in_stats.move == out_stats.move);
    lok(in_stats.prof == out_stats.prof);
    out_stats = unit1.current_stats;
    lok(in_stats.hp <= out_stats.hp);
    lok(in_stats.str <= out_stats.str);
    lok(in_stats.mag <= out_stats.mag);
    lok(in_stats.agi <= out_stats.agi);
    lok(in_stats.dex <= out_stats.dex);
    lok(in_stats.luck <= out_stats.luck);
    lok(in_stats.def <= out_stats.def);
    lok(in_stats.res <= out_stats.res);
    lok(in_stats.con <= out_stats.con);
    lok(in_stats.move <= out_stats.move);
    lok(in_stats.prof <= out_stats.prof);
    if (PHYSFS_stat("saves", NULL) == 0) {
        PHYSFS_mkdir("saves");
    }
    SDL_Log("io write read");
    writeJSON("saves"PHYSFS_SEPARATOR"unit_test.json", &unit1, false);
    SDL_Log("write1");
    writeJSON("saves"PHYSFS_SEPARATOR"unit_test.binou", &unit1, false);
    SDL_Log("write2");
    readJSON("saves"PHYSFS_SEPARATOR"unit_test.json", &unit4);
    SDL_Log("read1");
    writeJSON("saves"PHYSFS_SEPARATOR"unit_rewrite.json", &unit4, false);
    SDL_Log("write2");
    lok(PHYSFS_exists("saves"PHYSFS_SEPARATOR"unit_test.json") != 0);
    lok(PHYSFS_exists("saves"PHYSFS_SEPARATOR"unit_rewrite.json") != 0);
    lok(fequal("saves"PHYSFS_SEPARATOR"unit_test.json", "saves"PHYSFS_SEPARATOR"unit_rewrite.json"));
}


void test_unit() {
    SDL_Log("test_unit");
    utilities_allNames_Load();
    test_io();
    test_combat_flow();
    test_combat_stats();
    test_combat_death();
    test_promotion();
    test_canEquip();
}
