#include "nourstest.h"
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
    size_t order;
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_ERWIN);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Erwin")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_KIARA);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Kiara")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_SILOU);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Silou")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_PIROU);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Pirou")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_SERVIL);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Servil")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_PERIGNON);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Perignon")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_LAUZON);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Lauzon")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_NICOLE);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Nicole")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_OTTO);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Otto")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_SEBASTIAN);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Sebastian")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_MICHAEL);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Michael")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_CHASSE);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Chasse")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_PIERRE);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Pierre")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_MELLY);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Melly")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_SIMON);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Simon")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_RAYAN);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Rayan")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_TEHARON);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Teharon")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_KAKWI);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Kakwi")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_EUGENE);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Eugene")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_LUCRECE);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Lucrece")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_ZINEDAN);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Zinedan")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_ZIDINE);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Zidine")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_SAWISK);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Sawisk")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_BANDIT);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Bandit")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_DUELIST);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Duelist")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_PICKPOCKET);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Pickpocket")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_THIEF);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Thief")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_ARCHER);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Archer")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_MARKSMAN);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Marksman")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_CORSAIR);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Corsair")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_VIKING);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Viking")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_RAVAGER);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Ravager")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_HOPLITE);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Hoplite")));
    order = *(u16 *)DTAB_GET(global_unitOrders, UNIT_ID_FENCER);
    nourstest_true(s8equal(s8_var(global_unitNames[order]), s8_literal("Fencer")));

    SDL_Log("test_str");

    s8 temp = s8_mut("test_utilities");
    nourstest_true(s8equal(s8_literal("test utilities"), s8_replaceSingle(temp, '_', ' ')));

    memcpy(temp.data, "test_utilities", temp.len);
    nourstest_true(s8equal(s8_literal("Test_Utilities"), s8_camelCase(temp, '_', 2)));

    memcpy(temp.data, "test_utilities", temp.len);
    nourstest_true(s8equal(s8_literal("Test Utilities"), s8_camelCase(temp, ' ', 2)));

    s8_free(&temp);
    temp = s8_mut("vial of light");
    nourstest_true(s8equal(s8_literal("Vial of Light"), s8_camelCase(temp, ' ', 2)));

    memcpy(temp.data, "vial of light", temp.len);
    nourstest_true(s8equal(s8_literal("Vial Of Light"), s8_camelCase(temp, ' ', 1)));

    memcpy(temp.data, "Vial Of Light", temp.len);
    nourstest_true(s8equal(s8_literal("vial of light"), s8_toLower(temp)));
    s8_free(&temp);

    if (PHYSFS_stat(SAVE_FOLDER, NULL)) {
        PHYSFS_mkdir(SAVE_FOLDER);
    }
    nourstest_true(Filesystem_fequal("test_results.txt", "test_results.txt"));

    SDL_Log("test_tilenames");
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_PLAIN]),     s8_literal("Plain"))     );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_BUSH]),      s8_literal("Bush"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_FOREST]),    s8_literal("Forest"))    );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_THICKET]),   s8_literal("Thicket"))   );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_SNAG]),      s8_literal("Snag"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_BRIDGE]),    s8_literal("Bridge"))    );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_HOLE]),      s8_literal("Hole"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_ROAD]),      s8_literal("Road"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_SEA]),       s8_literal("Sea"))       );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_LAKE]),      s8_literal("Lake"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_RIVER]),     s8_literal("River"))     );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_WATERFALL]), s8_literal("Waterfall")) );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_CLIFF]),     s8_literal("Cliff"))     );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_REEF]),      s8_literal("Reef"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_HILL]),      s8_literal("Hill"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_MOUNTAIN]),  s8_literal("Mountain"))  );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_PEAK]),      s8_literal("Peak"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_CAVE]),      s8_literal("Cave"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_BOULDER]),   s8_literal("Boulder"))   );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_SAND]),      s8_literal("Sand"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_WASTELAND]), s8_literal("Wasteland")) );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_ROCK]),      s8_literal("Rock"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_BONES]),     s8_literal("Bones"))     );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_FLOOR]),     s8_literal("Floor"))     );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_PILLAR]),    s8_literal("Pillar"))    );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_WALL]),      s8_literal("Wall"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_DOOR]),      s8_literal("Door"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_THRONE]),    s8_literal("Throne"))    );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_GATE]),      s8_literal("Gate"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_FENCE]),     s8_literal("Fence"))     );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_SHOP]),      s8_literal("Shop"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_ARMORY]),    s8_literal("Armory"))    );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_VILLAGE]),   s8_literal("Village"))   );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_FORT]),      s8_literal("Fort"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_CASTLE]),    s8_literal("Castle"))    );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_SNOW]),      s8_literal("Snow"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_GLACIER]),   s8_literal("Glacier"))   );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_ICE]),       s8_literal("Ice"))       );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_UNDERWOOD]), s8_literal("Underwood")) );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_ROOT]),      s8_literal("Root"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_VINE]),      s8_literal("Vine"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_TRUNK]),     s8_literal("Trunk"))     );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_TREE]),      s8_literal("Tree"))      );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_TRAIL]),     s8_literal("Trail"))     );
    nourstest_true(s8equal(s8_var(global_tilenames[TILE_SHADOW]),    s8_literal("Shadow"))    );

    SDL_Log("test_wpn_names");
    u16 wpn_typecode;
    uint64_t temp_item_id;
    char **wpn_names;
    wpn_typecode = ITEM_TYPE_SWORD + ITEM_TYPE_LANCE;
    wpn_names = Names_wpnType(wpn_typecode);
    nourstest_true(s8equal(s8_var(wpn_names[0]), s8_literal("Sword")));
    nourstest_true(s8equal(s8_var(wpn_names[1]), s8_literal("Lance")));
    nourstest_true(DARR_NUM(wpn_names) == 2);

    for (size_t i = 0; i < DARR_NUM(wpn_names); i++) {
        free(wpn_names[i]);
    }

    DARR_FREE(wpn_names);
    temp_item_id = ITEM_ID_WOODEN_SWORD;
}
