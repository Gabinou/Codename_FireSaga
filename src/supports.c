#include "supports.h"

const struct Support Support_default = {0};

struct Computed_Stats Support_Bonus(struct Support support, u16 self_type) {
    struct Computed_Stats bonus = {0};
    Support_Flat_Bonus(&bonus, support.level);
    Support_Type_Bonus(&bonus, self_type, support.level);
    Support_Type_Bonus(&bonus, support.other_type, support.level);

    return (bonus);
}

struct Computed_Stats Support_Bonuses(struct Support *supports, u16 support_num, u16 self_type) {
    SDL_assert(self_type > SUPPORT_TYPE_NULL);
    SDL_assert(self_type < SUPPORT_TYPE_NUM);
    struct Computed_Stats bonus = {0};

    for (int i = 0; i < support_num; i++) {
        if ((supports[i].level > SUPPORT_LEVEL_A) || (supports[i].level <= SUPPORT_LEVEL_ZERO))
            continue;

        Support_Flat_Bonus(&bonus, supports[i].level);
        Support_Type_Bonus(&bonus, self_type, supports[i].level);
        Support_Type_Bonus(&bonus, supports[i].other_type, supports[i].level);
    }

    return (bonus);
}

void Support_Flat_Bonus(struct Computed_Stats *bonus, u16 level) {
    SDL_assert(level <= SUPPORT_LEVEL_A);
    SDL_assert(level > SUPPORT_LEVEL_ZERO);
    bonus->hit += SUPPORT_HIT_FACTOR * level;
    bonus->crit += level;
}

void Support_Type_Bonus(struct Computed_Stats *bonus, u16 type, u16 level) {
    SDL_assert(level <= SUPPORT_LEVEL_A);
    SDL_assert(level > SUPPORT_LEVEL_ZERO);
    SDL_assert(type > SUPPORT_TYPE_NULL);
    SDL_assert(type < SUPPORT_TYPE_NUM);

    switch (type) {
        case SUPPORT_TYPE_STRONG:
            bonus->attack[DMG_PHYSICAL] += level;
            bonus->attack.magical += level;
            break;
        case SUPPORT_TYPE_DEFENSIVE:
            bonus->protection[DMG_PHYSICAL] += level;
            bonus->protection.magical += level;
            break;
        case SUPPORT_TYPE_DODGY:
            bonus->dodge += SUPPORT_DODGE_FACTOR * level;
            bonus->favor += level;
            break;
        case SUPPORT_TYPE_SPEEDY:
            bonus->speed += level;
            break;
        case SUPPORT_TYPE_WILD:
            bonus->crit += SUPPORT_CRIT_FACTOR * level;
            break;
        default:
            SDL_Log("Invalid support type");
            break;
    }
}