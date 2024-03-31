#include "nourstest.h"
#include "unit/party.h"

#define PARTY_FOLDER "units", "test_party"
#define DEBUG_MAP_FOLDER "units", "debug_map"

void test_party() {
    s8 path_party = s8_literal(PATH_JOIN(PARTY_FOLDER, "debug.json"));

    struct Party party = Party_default;
    Party_Folder(&party, PATH_JOIN(PARTY_FOLDER));
    SDL_assert(party.folder.data != NULL);
    jsonio_readJSON(path_party, &party);

    /* --- Reading Party JSON only test --- */
    nourstest_true(party.names != NULL);
    nourstest_true(DARR_NUM(party.names) == 1);
    nourstest_true(s8equal(s8_literal("Erwin"), party.names[0]));

    nourstest_true(party.ids != NULL);
    nourstest_true(DARR_NUM(party.ids) == 1);
    nourstest_true(party.ids[0] == 2012);

    nourstest_true(party.filenames != NULL);
    nourstest_true(DARR_NUM(party.filenames) == 1);
    s8 path = s8_literal(PATH_JOIN(DEBUG_MAP_FOLDER, "Boss.json"));
    nourstest_true(s8equal(path, party.filenames[0]));

    /* --- Creating filenames for ids --- */
    Party_Ids2Filenames(&party);
    nourstest_true(DARR_NUM(party.filenames) == 2);
    path = s8_literal(PATH_JOIN(DEBUG_MAP_FOLDER, "Boss.json"));
    nourstest_true(s8equal(path, party.filenames[0]));
    path = s8_literal(PATH_JOIN(PARTY_FOLDER, "Corsair.json"));
    nourstest_true(s8equal(path, party.filenames[1]));

    /* --- Creating filenames for ids --- */
    Party_Names2Filenames(&party);
    nourstest_true(DARR_NUM(party.filenames) == 3);
    path = s8_literal(PATH_JOIN(DEBUG_MAP_FOLDER, "Boss.json"));
    nourstest_true(s8equal(path, party.filenames[0]));
    path = s8_literal(PATH_JOIN(PARTY_FOLDER, "Corsair.json"));
    nourstest_true(s8equal(path, party.filenames[1]));
    path = s8_literal(PATH_JOIN(PARTY_FOLDER, "Erwin.json"));
    nourstest_true(s8equal(path, party.filenames[2]));

    Party_Free(&party);
}

#undef PARTY_FOLDER
