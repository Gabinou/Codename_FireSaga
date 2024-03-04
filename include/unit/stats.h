#ifndef UNIT_STATS_H
#define UNIT_STATS_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "filesystem.h"
#include "weapon.h"
#include "supports.h"
#include "unit/mount.h"
#include "nmath.h"
#include "jsonio.h"
#include "RNG.h"
#include "physfs.h"
#include "equations.h"
#include "platform.h"
#include "utilities.h"
#include "combat.h"
#include "unit/anim.h"
#include "unit/status.h"
#include "unit/boss.h"
#include "unit/stats.h"
#include "unit/mount.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Weapon;
struct Damage;
struct Position;
struct Map;
struct RNG_Sequence;
struct Reinforcement;

/* --- Unit stats --- */
struct Unit_stats Unit_stats_plus(     struct Unit_stats stats1, struct Unit_stats stats2);
struct Unit_stats Unit_stats_minus(    struct Unit_stats stats1, struct Unit_stats stats2);
struct Unit_stats Unit_stats_div_cst(  struct Unit_stats stats,  int cst);
struct Unit_stats Unit_stats_plus_cst( struct Unit_stats stats1, int cst);
struct Unit_stats Unit_stats_minus_cst(struct Unit_stats stats,  int cst);

#endif /* UNIT_STATS_H */
