#include "unit/party.h"

void test_party() {
    s8 path_party = s8_literal("units"PHYSFS_SEPARATOR"debug_party"PHYSFS_SEPARATOR"debug.json");

    struct Party party = Party_default;
    jsonio_readJSON(path_party, &party);
}