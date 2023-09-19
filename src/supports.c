#include "supports.h"

struct Support Support_default = {
    .other_id =    -1,
    .other_type =  -1,
    .level =       -1
};

struct Computed_Stats Support_Bonus(struct Support support, uf16 self_type) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Computed_Stats bonus = {0};
    Support_Flat_Bonus(&bonus, support.level);
    Support_Type_Bonus(&bonus, self_type, support.level);
    Support_Type_Bonus(&bonus, support.other_type, support.level);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (bonus);
}

struct Computed_Stats Support_Bonuses(struct Support *supports, uf16 support_num, uf16 self_type) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (bonus);
}

void Support_Flat_Bonus(struct Computed_Stats *bonus, uf16 level) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(level <= SUPPORT_LEVEL_A);
    SDL_assert(level > SUPPORT_LEVEL_ZERO);
    bonus->hit += SUPPORT_HIT_FACTOR * level;
    bonus->crit += level;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Support_Type_Bonus(struct Computed_Stats *bonus, uf16 type, uf16 level) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(level <= SUPPORT_LEVEL_A);
    SDL_assert(level > SUPPORT_LEVEL_ZERO);
    SDL_assert(type > SUPPORT_TYPE_NULL);
    SDL_assert(type < SUPPORT_TYPE_NUM);

    switch (type) {
        case SUPPORT_TYPE_STRONG:
            bonus->attack[DAMAGE_TYPE_PHYSICAL] += level;
            bonus->attack[DAMAGE_TYPE_MAGICAL] += level;
            break;
        case SUPPORT_TYPE_DEFENSIVE:
            bonus->protection[DAMAGE_TYPE_PHYSICAL] += level;
            bonus->protection[DAMAGE_TYPE_MAGICAL] += level;
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
            SOTA_Log_Debug("Invalid support type");
            break;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}