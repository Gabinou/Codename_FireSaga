#ifndef SUPPORTS_H
#define SUPPORTS_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "macros.h"
#include "globals.h"
#include "debug.h"
#include "SDL2/SDL.h"

/* --- ENUMS --- */
#define REGISTER_ENUM(x) SUPPORT_TYPE_##x,
enum SUPPORT_TYPES {
    SUPPORT_TYPE_NULL = 0,
#include "names/support_types.h"
    SUPPORT_TYPE_NUM,
};
#undef REGISTER_ENUM

/* --- CONSTANTS --- */
enum SUPPORT_LEVELS {
    SUPPORT_LEVEL_NULL = 0,
    SUPPORT_LEVEL_ZERO = 0,
    SUPPORT_LEVEL_C = 1,
    SUPPORT_LEVEL_B = 2,
    SUPPORT_LEVEL_A = 3,
};

#define SUPPORT_DODGE_FACTOR 2
#define SUPPORT_CRIT_FACTOR 2
#define SUPPORT_HIT_FACTOR 5

/* --- STRUCTS --- */
struct Support {
    u16 other_id;
    u16 other_type;
    i8 level;
};

/* --- Support Object --- */
struct Computed_Stats Support_Bonus(  struct Support  s, u16 st);
struct Computed_Stats Support_Bonuses(struct Support *s, u16 sn, u16 st);

/* --- Support Computed_Stats --- */
void Support_Type_Bonus(struct Computed_Stats *b, u16 t, u16 l);
void Support_Flat_Bonus(struct Computed_Stats *b, u16 l);

#endif /* SUPPORTS_H */
