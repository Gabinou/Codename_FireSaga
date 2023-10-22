#include "nourstest.h"
#include "utilities.h"
#include "enums.h"

void test_utilities() {
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);

    SDL_Log("test_Unit_army2alignment");
    nourstest_true(SotA_army2alignment(ARMY_FRIENDLY) == ALIGNMENT_FRIENDLY);
    nourstest_true(SotA_army2alignment(ARMY_ERWIN) == ALIGNMENT_FRIENDLY);
    nourstest_true(SotA_army2alignment(ARMY_FREE_MILITIA) == ALIGNMENT_FRIENDLY);
    nourstest_true(SotA_army2alignment(ARMY_NEUTRAL) == ALIGNMENT_FRIENDLY);
    nourstest_true(SotA_army2alignment(ARMY_IMPERIAL) == ALIGNMENT_FRIENDLY);
    nourstest_true(SotA_army2alignment(ARMY_HAMILCAR) == ALIGNMENT_ENEMY);
    nourstest_true(SotA_army2alignment(ARMY_FIRST_EXPEDITIONARY_FORCE) == ALIGNMENT_ENEMY);
    nourstest_true(SotA_army2alignment(ARMY_THEOCRACY) == ALIGNMENT_FRIENDLY);
    nourstest_true(SotA_army2alignment(ARMY_VOLDAN) == ALIGNMENT_FRIENDLY);
    nourstest_true(SotA_army2alignment(ARMY_KEWAC) == ALIGNMENT_ENEMY);
    nourstest_true(SotA_army2alignment(ARMY_KESIRU) == ALIGNMENT_FRIENDLY);
    nourstest_true(SotA_army2alignment(ARMY_ENEMY) == ALIGNMENT_ENEMY);
    nourstest_true(SotA_army2alignment(ARMY_BANDITS) == ALIGNMENT_ENEMY);

    SDL_Log("test_unitNames");
    size_t index;
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_ERWIN);
    nourstest_true(strcmp(global_unitNames[index], "Erwin") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_KIARA);
    nourstest_true(strcmp(global_unitNames[index], "Kiara") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_SILOU);
    nourstest_true(strcmp(global_unitNames[index], "Silou") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_PIROU);
    nourstest_true(strcmp(global_unitNames[index], "Pirou") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_SERVIL);
    nourstest_true(strcmp(global_unitNames[index], "Servil") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_PERIGNON);
    nourstest_true(strcmp(global_unitNames[index], "Perignon") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_LAUZON);
    nourstest_true(strcmp(global_unitNames[index], "Lauzon") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_NICOLE);
    nourstest_true(strcmp(global_unitNames[index], "Nicole") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_OTTO);
    nourstest_true(strcmp(global_unitNames[index], "Otto") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_SEBASTIAN);
    nourstest_true(strcmp(global_unitNames[index], "Sebastian") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_MICHAEL);
    nourstest_true(strcmp(global_unitNames[index], "Michael") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_CHASSE);
    nourstest_true(strcmp(global_unitNames[index], "Chasse") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_PIERRE);
    nourstest_true(strcmp(global_unitNames[index], "Pierre") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_MELLY);
    nourstest_true(strcmp(global_unitNames[index], "Melly") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_SIMON);
    nourstest_true(strcmp(global_unitNames[index], "Simon") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_RAYAN);
    nourstest_true(strcmp(global_unitNames[index], "Rayan") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_TEHARON);
    nourstest_true(strcmp(global_unitNames[index], "Teharon") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_KAKWI);
    nourstest_true(strcmp(global_unitNames[index], "Kakwi") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_EUGENE);
    nourstest_true(strcmp(global_unitNames[index], "Eugene") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_LUCRECE);
    nourstest_true(strcmp(global_unitNames[index], "Lucrece") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_ZINEDAN);
    nourstest_true(strcmp(global_unitNames[index], "Zinedan") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_ZIDINE);
    nourstest_true(strcmp(global_unitNames[index], "Zidine") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_SAWISK);
    nourstest_true(strcmp(global_unitNames[index], "Sawisk") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_BANDIT);
    nourstest_true(strcmp(global_unitNames[index], "Bandit") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_DUELIST);
    nourstest_true(strcmp(global_unitNames[index], "Duelist") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_PICKPOCKET);
    nourstest_true(strcmp(global_unitNames[index], "Pickpocket") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_THIEF);
    nourstest_true(strcmp(global_unitNames[index], "Thief") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_ARCHER);
    nourstest_true(strcmp(global_unitNames[index], "Archer") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_MARKSMAN);
    nourstest_true(strcmp(global_unitNames[index], "Marksman") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_CORSAIR);
    nourstest_true(strcmp(global_unitNames[index], "Corsair") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_VIKING);
    nourstest_true(strcmp(global_unitNames[index], "Viking") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_RAVAGER);
    nourstest_true(strcmp(global_unitNames[index], "Ravager") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_HOPLITE);
    nourstest_true(strcmp(global_unitNames[index], "Hoplite") == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_FENCER);
    nourstest_true(strcmp(global_unitNames[index], "Fencer") == 0);

    SDL_Log("test_str");
    char *temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);
    strcpy(temp_str, "test_utilities");
    nourstest_true(strcmp("test utilities", nstr_replaceSingle(temp_str, '_', ' ')) == 0);
    strcpy(temp_str, "test_utilities");
    nourstest_true(strcmp("Test_Utilities", nstr_camelCase(temp_str, '_', 2)) == 0);
    strcpy(temp_str, "test utilities");
    nourstest_true(strcmp("Test Utilities", nstr_camelCase(temp_str, ' ', 2)) == 0);
    strcpy(temp_str, "vial of light");
    nourstest_true(strcmp("Vial of Light", nstr_camelCase(temp_str, ' ', 2)) == 0);
    nourstest_true(strcmp("Vial Of Light", nstr_camelCase(temp_str, ' ', 1)) == 0);
    strcpy(temp_str, "Vial of Light");
    nourstest_true(strcmp("vial of light", nstr_toLower(temp_str)) == 0);
    free(temp_str);

    if (PHYSFS_stat(SAVE_FOLDER, NULL) == 0) {
        PHYSFS_mkdir(SAVE_FOLDER);
    }
    nourstest_true(Filesystem_fequal("test_results.txt", "test_results.txt"));

    SDL_Log("test_tilenames");
    nourstest_true(strcmp(global_tilenames[TILE_PLAIN],     "Plain")      == 0);
    nourstest_true(strcmp(global_tilenames[TILE_BUSH],      "Bush")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_FOREST],    "Forest")     == 0);
    nourstest_true(strcmp(global_tilenames[TILE_THICKET],   "Thicket")    == 0);
    nourstest_true(strcmp(global_tilenames[TILE_SNAG],      "Snag")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_BRIDGE],    "Bridge")     == 0);
    nourstest_true(strcmp(global_tilenames[TILE_HOLE],      "Hole")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_ROAD],      "Road")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_SEA],       "Sea")        == 0);
    nourstest_true(strcmp(global_tilenames[TILE_LAKE],      "Lake")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_RIVER],     "River")      == 0);
    nourstest_true(strcmp(global_tilenames[TILE_WATERFALL], "Waterfall")  == 0);
    nourstest_true(strcmp(global_tilenames[TILE_CLIFF],     "Cliff")      == 0);
    nourstest_true(strcmp(global_tilenames[TILE_REEF],      "Reef")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_HILL],      "Hill")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_MOUNTAIN],  "Mountain")   == 0);
    nourstest_true(strcmp(global_tilenames[TILE_PEAK],      "Peak")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_CAVE],      "Cave")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_BOULDER],   "Boulder")    == 0);
    nourstest_true(strcmp(global_tilenames[TILE_SAND],      "Sand")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_WASTELAND], "Wasteland")  == 0);
    nourstest_true(strcmp(global_tilenames[TILE_ROCK],      "Rock")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_BONES],     "Bones")      == 0);
    nourstest_true(strcmp(global_tilenames[TILE_FLOOR],     "Floor")      == 0);
    nourstest_true(strcmp(global_tilenames[TILE_PILLAR],    "Pillar")     == 0);
    nourstest_true(strcmp(global_tilenames[TILE_WALL],      "Wall")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_DOOR],      "Door")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_THRONE],    "Throne")     == 0);
    nourstest_true(strcmp(global_tilenames[TILE_GATE],      "Gate")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_FENCE],     "Fence")      == 0);
    nourstest_true(strcmp(global_tilenames[TILE_SHOP],      "Shop")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_ARMORY],    "Armory")     == 0);
    nourstest_true(strcmp(global_tilenames[TILE_VILLAGE],   "Village")    == 0);
    nourstest_true(strcmp(global_tilenames[TILE_FORT],      "Fort")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_CASTLE],    "Castle")     == 0);
    nourstest_true(strcmp(global_tilenames[TILE_SNOW],      "Snow")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_GLACIER],   "Glacier")    == 0);
    nourstest_true(strcmp(global_tilenames[TILE_ICE],       "Ice")        == 0);
    nourstest_true(strcmp(global_tilenames[TILE_UNDERWOOD], "Underwood")  == 0);
    nourstest_true(strcmp(global_tilenames[TILE_ROOT],      "Root")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_VINE],      "Vine")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_TRUNK],     "Trunk")      == 0);
    nourstest_true(strcmp(global_tilenames[TILE_TREE],      "Tree")       == 0);
    nourstest_true(strcmp(global_tilenames[TILE_TRAIL],     "Trail")      == 0);
    nourstest_true(strcmp(global_tilenames[TILE_SHADOW],    "Shadow")     == 0);

    SDL_Log("test_wpn_names");
    u16 wpn_typecode;
    uint64_t temp_item_id;
    char **wpn_names;
    wpn_typecode = ITEM_TYPE_SWORD + ITEM_TYPE_LANCE;
    wpn_names = Names_wpnType(wpn_typecode);
    nourstest_true(strcmp(wpn_names[0], "Sword") == 0);
    nourstest_true(strcmp(wpn_names[1], "Lance") == 0);
    nourstest_true(DARR_NUM(wpn_names) == 2);

    for (size_t i = 0; i < DARR_NUM(wpn_names); i++) {
        free(wpn_names[i]);
    }

    DARR_FREE(wpn_names);
    temp_item_id = ITEM_ID_WOODEN_SWORD;
}
