#ifndef CAMP_H
#define CAMP_H

#include <math.h>
#include "narrative.h"
#include "enums.h"
#include "utilities.h"
#include "jsonio.h"
#include "filesystem.h"
#include "probability.h"

extern SDL_Renderer * Game_renderer;

struct Item;

typedef struct Camp {
    uint8_t json_element;
    uint16_t * exclusions;
    float fracs[CAMPJOB_END + 1];
    uint8_t max_jobs[CAMPJOB_END + 1];
    uint16_t * party;
    uint16_t * party_stack;
    int8_t previous_jobs[UNIT_NAME_PC_END];
    int8_t priority_jobs[UNIT_NAME_PC_END];
    int8_t forbidden_jobs[UNIT_NAME_PC_END];
    uint16_t * jobs[CAMPJOB_END + 1];
    uint8_t * job_queue;

    // librarian,
    // cook, -> cooking bonus
    // guard,
    // scribe, -> write notes
    // stablehand, ->
    // assistant, -> orders things? Gets lower prices on stuff
    // clergyman,
    // storagemaster,
    // magic researcher.
// SOME PEOPLE ARE GOOD AT THE JOB. OTHERS NOT.

    // 30 jobs.
    // + 1 -> Erwin is the leader.
    // + 2/3 -> War council members? Pérignon/Chassé/ Pierre et Simon Laplace.?
    // War council members should still have tasks.
    // + 1 -> armory merchant traveling with you.
    // + 1 -> item shop merchant traveling with you.
    uint8_t * optimal_jobs;
    int16_t party_size;
    int8_t chapter;
} Camp;
extern struct Camp Camp_default;

extern void Camp_writeJSON(struct Camp * in_camp, cJSON * in_jcamp);
extern void Camp_readJSON(struct Camp * in_camp, cJSON * in_jcamp);
extern void Camp_Chapter_Check(struct Camp * in_camp);
extern void Camp_partyStack_Make(struct Camp * in_camp);

extern void Camp_JobQueue_Compute(struct Camp * in_camp);
extern void Camp_Jobs_Clear(struct Camp * in_camp);
extern uint8_t Camp_Jobs_Count(struct Camp * in_camp);
extern bool Camp_Jobs_Check(struct Camp * in_camp);
extern void Camp_Jobs_Assign(struct Camp * in_camp);
extern void Camp_Jobs_Compute(struct Camp * in_camp);


#endif /* CAMP_H */