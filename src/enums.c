
#include "enums.h"

#define REGISTER_ENUM(x) const uint64_t UNIT_SKILL_##x = 1ULL << UNIT_SKILL_EXP_##x;
#include "names/skills.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) const uint64_t ITEM_EFFECT_##x = 1ULL << ITEM_EFFECT_EXP_##x;
#include "names/items_effects.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) const uint64_t UNIT_PC_ALIVE_##x = 1ULL << UNIT_NAME_##x;
#include "names/units_PC.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) const uint64_t UNIT_PC_RECRUITED_##x = 1ULL << UNIT_NAME_##x;
#include "names/units_PC.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) const uint64_t UNIT_NPC_ALIVE_##x = 1ULL << (UNIT_NAME_##x - UNIT_NAME_NPC_START);
#include "names/units_NPC.h"
#undef REGISTER_ENUM