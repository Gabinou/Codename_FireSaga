

#include "unit/flags.h"
#include "unit/unit.h"

b32 Unit_Sex(const Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (1);
    }
    return (unit->flags.sex);
}
b32 Unit_isAlive(const Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (unit->flags.alive);
}
void Unit_Alive_set(Unit *unit, b32 alive) {
    if (unit == NULL) {
        SDL_assert(false);
        return;
    }
    unit->flags.alive = alive;
}
b32 Unit_isWaiting(const Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (1);
    }
    return (unit->flags.waits);
}
void Unit_Waiting_set(Unit *unit, b32 wait) {
    if (unit == NULL) {
        SDL_assert(false);
        return;
    }
    unit->flags.waits = wait;
}
i8  Unit_Handedness(const Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (unit->flags.handedness);
}
void Unit_Handedness_set(Unit *unit, i8 handedness) {
    if (unit == NULL) {
        SDL_assert(false);
        return;
    }
    unit->flags.handedness = handedness;
}
u16 Unit_Equippable(const Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (unit->flags.equippable);
}
void Unit_Equippable_set(Unit *unit, u16 equippable) {
    if (unit == NULL) {
        SDL_assert(false);
        return;
    }
    unit->flags.equippable = equippable;
}
i32  Unit_Movement(Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (0);
    }
    Unit_stats eff_stats = Unit_effectiveStats(unit);
    return (eff_stats.move);
}
b32 Unit_isTalkable(const Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (unit->flags.talkable);
}
b32 Unit_showsDanger(const Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (unit->render.show_danger);
}
void Unit_showsDanger_set(Unit *unit, b32 show_danger) {
    if (unit == NULL) {
        SDL_assert(false);
        return;
    }
    unit->render.show_danger = show_danger;
}
b32 Unit_isDivineShield(const Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (unit->flags.divine_shield);
}
void Unit_DivineShield_set(Unit *unit, b32 divine_shield) {
    if (unit == NULL) {
        SDL_assert(false);
        return;
    }
    unit->flags.divine_shield = divine_shield;
}

u64 Unit_Skills(const Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (unit->flags.skills);
}
void Unit_Skill_Add(Unit *unit, u64 skill) {
    if (unit == NULL) {
        SDL_assert(false);
        return;
    }
    unit->flags.skills |= skill;
}
i32 Unit_Current_HP(const Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (unit->hp.current);
}
i32 Unit_Current_Agony(const Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (unit->counters.agony);
}
i32 Unit_Current_Regrets(const Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (unit->counters.regrets);
}
i32 Unit_Army(const Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (unit->id.army);
}
void Unit_Army_set(Unit *unit, i32 army) {
    if (unit == NULL) {
        SDL_assert(false);
        return;
    }
    unit->id.army = army;
}
i32 Unit_Class(const Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (unit->id.class);
}
void Unit_Class_set(Unit *unit, i32 class) {
    if (unit == NULL) {
        SDL_assert(false);
        return;
    }
    unit->id.class = class;
}
i32 Unit_AI(const Unit *unit) {
    if (unit == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (unit->id.ai);
}
void Unit_AI_set(Unit *unit, i32 ai) {
    if (unit == NULL) {
        SDL_assert(false);
        return;
    }
    unit->id.ai = ai;
}

