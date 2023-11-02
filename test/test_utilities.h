#include "nourstest.h"
#include "utilities.h"
#include "enums.h"

void test_utilities() {


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
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal("Erwin")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_KIARA);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Kiara")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_SILOU);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Silou")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_PIROU);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Pirou")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_SERVIL);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Servil")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_PERIGNON);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Perignon")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_LAUZON);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Lauzon")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_NICOLE);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Nicole")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_OTTO);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Otto")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_SEBASTIAN);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Sebastian")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_MICHAEL);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Michael")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_CHASSE);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Chasse")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_PIERRE);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Pierre")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_MELLY);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Melly")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_SIMON);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Simon")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_RAYAN);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Rayan")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_TEHARON);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Teharon")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_KAKWI);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Kakwi")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_EUGENE);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Eugene")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_LUCRECE);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Lucrece")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_ZINEDAN);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Zinedan")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_ZIDINE);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Zidine")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_SAWISK);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Sawisk")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_BANDIT);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Bandit")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_DUELIST);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Duelist")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_PICKPOCKET);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Pickpocket")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_THIEF);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Thief")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_ARCHER);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Archer")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_MARKSMAN);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Marksman")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_CORSAIR);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Corsair")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_VIKING);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Viking")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_RAVAGER);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Ravager")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_HOPLITE);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Hoplite")) == 0);
    index = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_FENCER);
    nourstest_true(s8equal( s8_var(global_unitNames[index]), s8_literal( "Fencer")) == 0);

    SDL_Log("test_str");
    char *temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);
    strcpy(temp_str, "test_utilities");
    nourstest_true(s8equal(s8_literal("test utilities"), s8_var(nstr_replaceSingle(temp_str, '_',
                                                                ' '))) == 0);
    strcpy(temp_str, "test_utilities");
    nourstest_true(s8equal(s8_literal("Test_Utilities"), s8_var(nstr_camelCase(temp_str, '_',
                                                                2))) == 0);
    strcpy(temp_str, "test utilities");
    nourstest_true(s8equal(s8_literal("Test Utilities"), s8_var(nstr_camelCase(temp_str, ' ',
                                                                2))) == 0);
    strcpy(temp_str, "vial of light");
    nourstest_true(s8equal(s8_literal("Vial of Light"), s8_var(nstr_camelCase(temp_str, ' ', 2))) == 0);
    nourstest_true(s8equal(s8_literal("Vial Of Light"), s8_var(nstr_camelCase(temp_str, ' ', 1))) == 0);
    strcpy(temp_str, "Vial of Light");
    nourstest_true(s8equal(s8_literal("vial of light"), s8_var(nstr_toLower(temp_str))) == 0);
    free(temp_str);

    if (PHYSFS_stat(SAVE_FOLDER, NULL) == 0) {
        PHYSFS_mkdir(SAVE_FOLDER);
    }
    nourstest_true(Filesystem_fequal("test_results.txt", "test_results.txt"));

    SDL_Log("test_tilenames");
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_PLAIN]),     s8_literal("Plain"))      == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_BUSH]),      s8_literal("Bush"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_FOREST]),    s8_literal("Forest"))     == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_THICKET]),   s8_literal("Thicket"))    == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_SNAG]),      s8_literal("Snag"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_BRIDGE]),    s8_literal("Bridge"))     == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_HOLE]),      s8_literal("Hole"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_ROAD]),      s8_literal("Road"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_SEA]),       s8_literal("Sea"))        == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_LAKE]),      s8_literal("Lake"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_RIVER]),     s8_literal("River"))      == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_WATERFALL]), s8_literal("Waterfall"))  == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_CLIFF]),     s8_literal("Cliff"))      == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_REEF]),      s8_literal("Reef"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_HILL]),      s8_literal("Hill"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_MOUNTAIN]),  s8_literal("Mountain"))   == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_PEAK]),      s8_literal("Peak"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_CAVE]),      s8_literal("Cave"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_BOULDER]),   s8_literal("Boulder"))    == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_SAND]),      s8_literal("Sand"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_WASTELAND]), s8_literal("Wasteland"))  == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_ROCK]),      s8_literal("Rock"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_BONES]),     s8_literal("Bones"))      == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_FLOOR]),     s8_literal("Floor"))      == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_PILLAR]),    s8_literal("Pillar"))     == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_WALL]),      s8_literal("Wall"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_DOOR]),      s8_literal("Door"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_THRONE]),    s8_literal("Throne"))     == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_GATE]),      s8_literal("Gate"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_FENCE]),     s8_literal("Fence"))      == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_SHOP]),      s8_literal("Shop"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_ARMORY]),    s8_literal("Armory"))     == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_VILLAGE]),   s8_literal("Village"))    == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_FORT]),      s8_literal("Fort"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_CASTLE]),    s8_literal("Castle"))     == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_SNOW]),      s8_literal("Snow"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_GLACIER]),   s8_literal("Glacier"))    == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_ICE]),       s8_literal("Ice"))        == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_UNDERWOOD]), s8_literal("Underwood"))  == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_ROOT]),      s8_literal("Root"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_VINE]),      s8_literal("Vine"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_TRUNK]),     s8_literal("Trunk"))      == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_TREE]),      s8_literal("Tree"))       == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_TRAIL]),     s8_literal("Trail"))      == 0);
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_SHADOW]),    s8_literal("Shadow"))     == 0);

    SDL_Log("test_wpn_names");
    u16 wpn_typecode;
    uint64_t temp_item_id;
    char **wpn_names;
    wpn_typecode = ITEM_TYPE_SWORD + ITEM_TYPE_LANCE;
    wpn_names = Names_wpnType(wpn_typecode);
    nourstest_true(s8equal(s8_var(wpn_names[0]), s8_literal("Sword")) == 0);
    nourstest_true(s8equal(s8_var(wpn_names[1]), s8_literal("Lance")) == 0);
    nourstest_true(DARR_NUM(wpn_names) == 2);

    for (size_t i = 0; i < DARR_NUM(wpn_names); i++) {
        free(wpn_names[i]);
    }

    DARR_FREE(wpn_names);
    temp_item_id = ITEM_ID_WOODEN_SWORD;
}
