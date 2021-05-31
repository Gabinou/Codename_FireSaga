#include "minctest.h"
#include "utilities.h"
#include "enums.h"

void test_utilities() {
    SDL_Log("test_utilities");


    SDL_Log("test_utilities_army2alignment");
    lok(utilities_army2alignment(ARMY_FRIENDLY) == ALIGNMENT_FRIENDLY);
    lok(utilities_army2alignment(ARMY_ERWIN) == ALIGNMENT_FRIENDLY);
    lok(utilities_army2alignment(ARMY_FREE_MILITIA) == ALIGNMENT_FRIENDLY);
    lok(utilities_army2alignment(ARMY_NEUTRAL) == ALIGNMENT_FRIENDLY);
    lok(utilities_army2alignment(ARMY_IMPERIAL) == ALIGNMENT_FRIENDLY);
    lok(utilities_army2alignment(ARMY_HAMILCAR) == ALIGNMENT_ENEMY);
    lok(utilities_army2alignment(ARMY_FIRST_EXPEDITIONARY_FORCE) == ALIGNMENT_ENEMY);
    lok(utilities_army2alignment(ARMY_THEOCRACY) == ALIGNMENT_FRIENDLY);
    lok(utilities_army2alignment(ARMY_VOLDAN) == ALIGNMENT_FRIENDLY);
    lok(utilities_army2alignment(ARMY_KEWAC) == ALIGNMENT_ENEMY);
    lok(utilities_army2alignment(ARMY_KESIRU) == ALIGNMENT_FRIENDLY);
    lok(utilities_army2alignment(ARMY_ENEMY) == ALIGNMENT_ENEMY);
    lok(utilities_army2alignment(ARMY_BANDITS) == ALIGNMENT_ENEMY);

    SDL_Log("test_unitNames");
    lok(strcmp(unitNames[UNIT_NAME_ERWIN], "Erwin") == 0);
    lok(strcmp(unitNames[UNIT_NAME_KIARA], "Kiara") == 0);
    lok(strcmp(unitNames[UNIT_NAME_SILOU], "Silou") == 0);
    lok(strcmp(unitNames[UNIT_NAME_PIROU], "Pirou") == 0);
    lok(strcmp(unitNames[UNIT_NAME_SERVIL], "Servil") == 0);
    lok(strcmp(unitNames[UNIT_NAME_PERIGNON], "Perignon") == 0);
    lok(strcmp(unitNames[UNIT_NAME_LAUZON], "Lauzon") == 0);
    lok(strcmp(unitNames[UNIT_NAME_NICOLE], "Nicole") == 0);
    lok(strcmp(unitNames[UNIT_NAME_OTTO], "Otto") == 0);
    lok(strcmp(unitNames[UNIT_NAME_SEBASTIAN], "Sebastian") == 0);
    lok(strcmp(unitNames[UNIT_NAME_MICHAEL], "Michael") == 0);
    lok(strcmp(unitNames[UNIT_NAME_CHASSE], "Chasse") == 0);
    lok(strcmp(unitNames[UNIT_NAME_PIERRE], "Pierre") == 0);
    lok(strcmp(unitNames[UNIT_NAME_MELLY], "Melly") == 0);
    lok(strcmp(unitNames[UNIT_NAME_SIMON], "Simon") == 0);
    lok(strcmp(unitNames[UNIT_NAME_RAYAN], "Rayan") == 0);
    lok(strcmp(unitNames[UNIT_NAME_TEHARON], "Teharon") == 0);
    lok(strcmp(unitNames[UNIT_NAME_KAKWI], "Kakwi") == 0);
    lok(strcmp(unitNames[UNIT_NAME_EUGENE], "Eugene") == 0);
    lok(strcmp(unitNames[UNIT_NAME_LUCRECE], "Lucrece") == 0);
    lok(strcmp(unitNames[UNIT_NAME_ZINEDAN], "Zinedan") == 0);
    lok(strcmp(unitNames[UNIT_NAME_ZIDINE], "Zidine") == 0);
    lok(strcmp(unitNames[UNIT_NAME_SAWISK], "Sawisk") == 0);
    lok(strcmp(unitNames[UNIT_NAME_BANDIT], "Bandit") == 0);
    lok(strcmp(unitNames[UNIT_NAME_DUELIST], "Duelist") == 0);
    lok(strcmp(unitNames[UNIT_NAME_PICKPOCKET], "Pickpocket") == 0);
    lok(strcmp(unitNames[UNIT_NAME_THIEF], "Thief") == 0);
    lok(strcmp(unitNames[UNIT_NAME_ASSASSIN], "Assassin") == 0);
    lok(strcmp(unitNames[UNIT_NAME_ARCHER], "Archer") == 0);
    lok(strcmp(unitNames[UNIT_NAME_MARKSMAN], "Marksman") == 0);
    lok(strcmp(unitNames[UNIT_NAME_MERCENARY], "Mercenary") == 0);
    lok(strcmp(unitNames[UNIT_NAME_HERO], "Hero") == 0);
    lok(strcmp(unitNames[UNIT_NAME_CORSAIR], "Corsair") == 0);
    lok(strcmp(unitNames[UNIT_NAME_VIKING], "Viking") == 0);
    lok(strcmp(unitNames[UNIT_NAME_RAVAGER], "Ravager") == 0);
    // lok(strcmp(unitNames[UNIT_NAME_CAVALIER], "Cavalier") == 0);
    lok(strcmp(unitNames[UNIT_NAME_PALADIN], "Paladin") == 0);
    lok(strcmp(unitNames[UNIT_NAME_FENCER], "Fencer") == 0);
    lok(strcmp(unitNames[UNIT_NAME_MOUSQUETAIRE], "Mousquetaire") == 0);

    SDL_Log("test_str");
    char * temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);
    strcpy(temp_str, "test_utilities");
    lok(strcmp("test utilities", str_replaceSingle(temp_str, '_', ' ')) == 0);
    strcpy(temp_str, "test_utilities");
    lok(strcmp("Test_Utilities", str_camelCase(temp_str, '_', 2)) == 0);
    strcpy(temp_str, "test utilities");
    lok(strcmp("Test Utilities", str_camelCase(temp_str, ' ', 2)) == 0);
    strcpy(temp_str, "vial of light");
    lok(strcmp("Vial of Light", str_camelCase(temp_str, ' ', 2)) == 0);
    lok(strcmp("Vial Of Light", str_camelCase(temp_str, ' ', 1)) == 0);
    strcpy(temp_str, "Vial of Light");
    lok(strcmp("vial of light", str_toLower(temp_str)) == 0);

    if (PHYSFS_stat(SAVE_FOLDER, NULL) == 0) {
        PHYSFS_mkdir(SAVE_FOLDER);
    }
    lok(fequal("test_results.txt", "test_results.txt"));
    int32_t arrtest1[5] = {3, 4, 5, 1, 2};
    quicksort(arrtest1, 0, 4);
    lok(arrtest1[0] == 1);
    lok(arrtest1[1] == 2);
    lok(arrtest1[2] == 3);
    lok(arrtest1[3] == 4);
    lok(arrtest1[4] == 5);

    SDL_Log("test_menuOptionnames");
    lok(strcmp(menuOptionnames[MENU_OPTION_ITEMS], "Items") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_TALK], "Talk") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_RESCUE], "Rescue") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_SEIZE], "Seize") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_ESCAPE], "Escape") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_ATTACK], "Attack") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_SHOP], "Shop") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_ARMORY], "Armory") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_VILLAGE], "Village") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_CONVOY], "Convoy") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_TRADE], "Trade") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_STAFF], "Staff") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_MAP], "Map") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_WAIT], "Wait") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_UNITS], "Units") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_OBJECTIVES], "Objectives") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_OPTIONS], "Options") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_END_TURN], "End Turn") == 0);
    lok(strcmp(menuOptionnames[MENU_OPTION_GLOBAL_RANGE], "Global Range") == 0);

    SDL_Log("test_wpn_names");
    uint16_t wpn_typecode;
    uint64_t temp_item_id;
    char ** wpn_names;
    wpn_typecode = ITEM_TYPE_SWORD + ITEM_TYPE_LANCE;
    wpn_names = utilities_wpnType(wpn_typecode);
    lok(strcmp(wpn_names[0], "Sword") == 0);
    lok(strcmp(wpn_names[1], "Lance") == 0);
    lok(DARR_NUM(wpn_names) == 2);
    temp_item_id = ITEM_NAME_WOODEN_SWORD;
    lok(strcmp(hmget(itemNames, temp_item_id), "Wooden Sword") == 0);
}
