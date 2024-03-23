#include "nourstest.h"
#include "unit/party.h"

void test_party() {
    s8 path_party = s8_literal(PATH_JOIN("units", "debug_party", "debug.json"));

    struct Party party = Party_default;
    Party_Folder(&party, PATH_JOIN("units", "debug_party"));
    jsonio_readJSON(path_party, &party);

    nourstest_true(party.names != NULL);
    nourstest_true(DARR_NUM(party.names) == 1);
    nourstest_true(s8equal(s8_literal("Erwin"), party.names[0]));
}