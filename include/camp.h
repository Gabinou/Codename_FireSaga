/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** Camp to manage army in between chapters
**
*/

#ifndef CAMP_H
#define CAMP_H

#include "enums.h"
#include "jsonio.h"
#include "nstr.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct cJSON;

enum SOTA_CAMP {
    // Bonus for Guards if an ambush is prevented
    CAMP_BONUSEXP_AMBUSH    = 50,
    CAMP_JOB_NULL           = 0,
};

enum SOTA_CAMP_JOB_canFIGHT {
    CAMP_GUARD_canFIGHT           = false,
    CAMP_LIBRARIAN_canFIGHT       = true,
    CAMP_ASSISTANT_canFIGHT       = true,
    CAMP_COOK_canFIGHT            = true,
    CAMP_SCRIBE_canFIGHT          = true,
    CAMP_STABLEHAND_canFIGHT      = true,
    CAMP_CLERGYMAN_canFIGHT       = true,
    CAMP_STORAGEMASTER_canFIGHT   = true,
    CAMP_MAGICRESEARCHER_canFIGHT = false,
};

enum SOTA_CAMP_JOB_EXP_BONUS {
    CAMP_GUARD_EXP_BONUS            = 50,
    CAMP_LIBRARIAN_EXP_BONUS        = 10,
    CAMP_ASSISTANT_EXP_BONUS        = 10,
    CAMP_COOK_EXP_BONUS             = 20,
    CAMP_SCRIBE_EXP_BONUS           = 20,
    CAMP_STABLEHAND_EXP_BONUS       = 20,
    CAMP_CLERGYMAN_EXP_BONUS        = 20,
    CAMP_STORAGEMASTER_EXP_BONUS    = 10,
    CAMP_MAGICRESEARCHER_EXP_BONUS  = 50,
};

enum SOTA_CAMP_JOB_MAX {
    CAMP_GUARD_MAX           = 4,
    CAMP_LIBRARIAN_MAX       = 2,
    CAMP_ASSISTANT_MAX       = 1,
    CAMP_COOK_MAX            = 3,
    CAMP_SCRIBE_MAX          = 2,
    CAMP_STABLEHAND_MAX      = 2,
    CAMP_CLERGYMAN_MAX       = 2,
    CAMP_STORAGEMASTER_MAX   = 2,
    CAMP_MAGICRESEARCHER_MAX = 2,
    CAMP_JOB_MAX             = 4,
    CAMP_JOB_TOTAL = CAMP_GUARD_MAX + CAMP_LIBRARIAN_MAX + CAMP_ASSISTANT_MAX + CAMP_COOK_MAX + CAMP_SCRIBE_MAX + CAMP_STABLEHAND_MAX + CAMP_CLERGYMAN_MAX + CAMP_STORAGEMASTER_MAX + CAMP_MAGICRESEARCHER_MAX,
};

extern const u8 max_jobs[CAMPJOB_END];

// Accessible places during camp.
typedef struct Camp_Places {
    // TODO: find latin names
    /* Food -> jobs, bonuses? */
    b32 mess_tent;

    /* Traveling Chapel
    **  1. Purging regrets
    **  2. ? */
    b32 traveling_chapel;

    /* Wandering Library
    **  1. Magic research -> new magic weapons
    **  2. Weapon infusion */
    b32 wandering_library;

    /* Praetorium: tent of *legatus* (commander).
    **  1. War council (*consilium belli*) held there */
    b32 praetorium;

    /* Water: Rivers, lakes, etc.
    **  1. Events: Fishing, bathing, conversations */
    b32 water;

    /* Armory on Wheels
    **  1. Weapon repair
    **  2. New weapons? */
    b32 armory_on_wheels;

    /* Merchant Caravan:
    **  1. Shops */
    b32 merchant_caravan;

    /* mobile_stables
    **  1. Mount animations 
    **  2. Jobs */
    b32 mobile_stables;
} Camp_Places;

typedef struct Camp_Jobs {
    // TODO
} Camp_Jobs;

// No camp automation.
// BUT jobs are kept between chapters.
typedef struct Camp {
    jsonIO_Header jsonio_header;

    /* All jobs */
    // TODO: put all jobs in Camp_jobs struct
    u8 guards[CAMP_JOB_MAX];
    u8 librarians[CAMP_JOB_MAX];
    u8 assistants[CAMP_JOB_MAX];
    u8 cooks[CAMP_JOB_MAX];
    u8 scribes[CAMP_JOB_MAX];
    u8 stablehands[CAMP_JOB_MAX];
    u8 clergymen[CAMP_JOB_MAX];
    u8 storagemasters[CAMP_JOB_MAX];
    u8 magicresearchers[CAMP_JOB_MAX];
    u8 workers[CAMPJOB_END];
    // forbidden_jobs: characters don't want to do anymore
    // Character must have worked the job before.
    u8 forbidden_jobs[UNIT_ID_PC_END];

    Camp_Places places;

} Camp;
extern const struct Camp Camp_default;

void Camp_Free(Camp *camp);

/* --- API --- */
void Camp_Job_Hire(     Camp *c, i16 u, i16 j);
void Camp_Job_Fire(     Camp *c, i16 u, i16 j);
void Camp_Job_Forbid(   Camp *c, i16 u, i16 j);

/* --- isCan --- */
u8 Camp_hasJob(Camp *in_camp, i16 unit_id);

/* --- Internals --- */
u8 *Camp_Job_Get(       Camp *in_camp, i16 job_id);
void Camp_Jobs_Clear(   Camp *in_camp);

/* --- I/O --- */
void Camp_readJSON(       void *input,  const cJSON *json);
void Camp_writeJSON(const void *input,        cJSON *json);

#endif /* CAMP_H */