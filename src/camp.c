#include "camp.h"
#include "stb_sprintf.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

struct Camp Camp_default = {
    .json_element = JSON_CAMP,
    .exclusions = NULL,
    // .exclusions = (uint16_t *)UNIT_NAME_ERWIN,
    .fracs = {0.05f, 0.05f, 0.15f, 0.0f, 0.05f, 0.0f, 0.1f, 0.05f, 0.05f},
    .max_jobs = {4, 4, 8, 1, 4, 1, 4, 4, 2},
    .party = NULL,
    .party_stack = NULL,
    .previous_jobs = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    .priority_jobs = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    .forbidden_jobs = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    .jobs = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    .job_queue = NULL,
    .chapter = 0,
};


void Camp_writeJSON(struct Camp * in_camp, cJSON * in_jcamp) {
    SDL_assert(in_jcamp != NULL);
    cJSON * jpriority = cJSON_CreateArray();
    cJSON * jprevious = cJSON_CreateArray();
    cJSON * jforbidden = cJSON_CreateArray();
    cJSON * temp;
    for (int16_t i = 0; i < UNIT_NAME_PC_END; i++) {
        temp = cJSON_CreateNumber(in_camp->priority_jobs[i]);
        cJSON_AddItemToArray(jpriority, temp);
        temp = cJSON_CreateNumber(in_camp->priority_jobs[i]);
        cJSON_AddItemToArray(jprevious, temp);
        temp = cJSON_CreateNumber(in_camp->forbidden_jobs[i]);
        cJSON_AddItemToArray(jforbidden, temp);
    }
    cJSON_AddItemToObject(in_jcamp, "Priority job", jpriority);
    cJSON_AddItemToObject(in_jcamp, "Previous job", jprevious);
    cJSON_AddItemToObject(in_jcamp, "Forbidden job", jforbidden);
}

void Camp_readJSON(struct Camp * in_camp, cJSON * in_jcamp) {
    cJSON * jprevious = cJSON_GetObjectItemCaseSensitive(in_jcamp, "Previous job");
    cJSON * jforbidden = cJSON_GetObjectItemCaseSensitive(in_jcamp, "Forbidden job");
    cJSON * jpriority = cJSON_GetObjectItemCaseSensitive(in_jcamp, "Priority job");
    int32_t previous_size = cJSON_GetArraySize(jprevious);
    int32_t forbidden_size = cJSON_GetArraySize(jforbidden);
    int32_t priority_size = cJSON_GetArraySize(jpriority);
    if (previous_size != UNIT_NAME_PC_END) {
        SDL_Log("Previous job array size is incorrect");
    }
    if (forbidden_size != UNIT_NAME_PC_END) {
        SDL_Log("Forbidden job array size is incorrect");
    }
    if (priority_size != UNIT_NAME_PC_END) {
        SDL_Log("Priority job array size is incorrect");
    }
    cJSON * jtemp;
    for (int16_t i = 0; i < UNIT_NAME_PC_END; i++) {
        jtemp = cJSON_GetArrayItem(jprevious, i);
        in_camp->previous_jobs[i] = cJSON_GetNumberValue(jtemp);
        jtemp = cJSON_GetArrayItem(jpriority, i);
        in_camp->priority_jobs[i] = cJSON_GetNumberValue(jtemp);
        jtemp = cJSON_GetArrayItem(jforbidden, i);
        in_camp->forbidden_jobs[i] = cJSON_GetNumberValue(jtemp);
    }
}


void Camp_Chapter_Check(struct Camp * in_camp) {
    switch (in_camp->chapter) {
        case CHAPTER_TEST:
            break;
        case CHAPTER_1:
            arrput(in_camp->exclusions, UNIT_NAME_ERWIN);
            break;
        case CHAPTER_2:
            in_camp->priority_jobs[UNIT_NAME_KIARA] = CAMPJOB_CLERGYMAN;
            break;
        case CHAPTER_1GAIDEN:
            break;
        default:
            SDL_Log("in_chapter is invalid");
    }
}

void Camp_partyStack_Make(struct Camp * in_camp) {
    arrfree(in_camp->party_stack);
    size_t party_size = arrlen(in_camp->party);
    uint16_t party_member;
    int8_t priority_job;
    // SDL_Log("party_size %d", party_size);
    bool excluded = false;
    for (uint8_t i = 0; i < party_size; i++) {
        // SDL_Log("party_size i %d", i);
        party_member = in_camp->party[i];
        for (uint8_t j = 0; j < arrlen(in_camp->exclusions); j++) {
            if (in_camp->exclusions[j] == party_member) {
                excluded = true;
            }
        }
        if (!excluded) {
            priority_job = in_camp->priority_jobs[party_member];
            // SDL_Log("party_member: %d", party_member);
            // SDL_Log("priority_jobs: %d", priority_job);
            if (priority_job > -1) {
                arrput(in_camp->jobs[priority_job], party_member);
            } else {
                if (URN_global() > 49) {
                    arrput(in_camp->party_stack, party_member);
                } else {
                    arrins(in_camp->party_stack, 0, party_member);
                }
            }
        }
        excluded = false;
    }
}

void Camp_JobQueue_Compute(struct Camp * in_camp) {
    arrfree(in_camp->job_queue);
    // Job priority: cook > stablehand > guard > storagemaster > clergymen > librarian > scribe > assistant -> Magic researcher
    arrput(in_camp->job_queue, CAMPJOB_COOK);
    arrput(in_camp->job_queue, CAMPJOB_STABLEHAND);
    arrput(in_camp->job_queue, CAMPJOB_GUARD);
    arrput(in_camp->job_queue, CAMPJOB_STORAGEMASTER);
    arrput(in_camp->job_queue, CAMPJOB_CLERGYMAN);
    arrput(in_camp->job_queue, CAMPJOB_LIBRARIAN);
    arrput(in_camp->job_queue, CAMPJOB_SCRIBE);
    arrput(in_camp->job_queue, CAMPJOB_ASSISTANT);
    arrput(in_camp->job_queue, CAMPJOB_MAGICRESEARCH);
}


void Camp_Jobs_Clear(struct Camp * in_camp) {
    for (int8_t i = 0; i < CAMPJOB_END ; ++i) {
        arrfree(in_camp->jobs[i]);
    }
}

uint8_t Camp_Jobs_Count(struct Camp * in_camp) {
    uint8_t total_jobs = 0;
    for (int8_t i = 0; i < CAMPJOB_END ; ++i) {
        total_jobs += arrlen(in_camp->jobs[i]);
    }
    return (total_jobs);
}

bool Camp_Jobs_Check(struct Camp * in_camp) {
    // SDL_Log("Checking jobs");
    bool out = false;
    uint8_t num_filled = 0;
    for (int8_t i = 0; i < CAMPJOB_END; i++) {
        // SDL_Log("i: %d", i);
        if (arrlen(in_camp->jobs[i]) >= in_camp->optimal_jobs[i]) {
            num_filled++;
        }
    }
    // SDL_Log("Done");
    out = (num_filled == CAMPJOB_END);
    // SDL_Log("Out %d", out);
    return (out);
}

void Camp_Jobs_Assign(struct Camp * in_camp) {
    int16_t unit_ind;
    int8_t party_len = arrlen(in_camp->party_stack);
    uint8_t current_job;
    while (party_len > 0) {
        // SDL_Log("party_stack size %d", party_len);
        unit_ind = in_camp->party_stack[party_len - 1];
        // SDL_Log("Stacking unit %d, %s", unit_ind, unitNames[unit_ind]);
        current_job = in_camp->job_queue[0];
        // SDL_Log("Current job %d", current_job);
        if (arrlen(in_camp->jobs[current_job]) >= in_camp->optimal_jobs[current_job]) {
            // SDL_Log("Job Full");
            arrput(in_camp->job_queue, current_job);
            arrdel(in_camp->job_queue, 0);
            continue;
        }
        if (in_camp->forbidden_jobs[unit_ind] == current_job) {
            // SDL_Log("Found forbidden job");
            arrput(in_camp->job_queue, current_job);
            arrdel(in_camp->job_queue, 0);
            continue;
        }
        arrput(in_camp->jobs[current_job], unit_ind);
        SDL_Log("Assigned Job %s to %s", campjobNames[current_job], unitNames[unit_ind]);
        if (Camp_Jobs_Check(in_camp)) {
            break;
        }
        arrdel(in_camp->party_stack, party_len - 1);
        party_len = arrlen(in_camp->party_stack);
        arrput(in_camp->job_queue, current_job);
        arrdel(in_camp->job_queue, 0);
        SDL_Log("job_queue size %d", arrlen(in_camp->job_queue));
        SDL_Log("party_stack size %d", party_len);
    }
}

void Camp_Jobs_Compute(struct Camp * in_camp) {
    in_camp->party_size = arrlen(in_camp->party);
    arrfree(in_camp->optimal_jobs);
    arrsetlen(in_camp->optimal_jobs, CAMPJOB_END);
    in_camp->optimal_jobs[CAMPJOB_LIBRARIAN] = fmin((uint8_t)in_camp->max_jobs[CAMPJOB_LIBRARIAN], (uint8_t)floor(in_camp->fracs[CAMPJOB_LIBRARIAN] * in_camp->party_size));
    in_camp->optimal_jobs[CAMPJOB_COOK] = fmin((uint8_t)in_camp->max_jobs[CAMPJOB_COOK], (uint8_t)fmax((uint8_t)floor(in_camp->fracs[CAMPJOB_COOK] * in_camp->party_size), (uint8_t)1));
    in_camp->optimal_jobs[CAMPJOB_GUARD] = fmin((uint8_t)in_camp->max_jobs[CAMPJOB_GUARD], (uint8_t)fmax((uint8_t)floor(in_camp->fracs[CAMPJOB_GUARD] * in_camp->party_size), (uint8_t)1));
    in_camp->optimal_jobs[CAMPJOB_STABLEHAND] = fmin((uint8_t)in_camp->max_jobs[CAMPJOB_STABLEHAND], (uint8_t)fmax((uint8_t)floor(in_camp->fracs[CAMPJOB_STABLEHAND] * in_camp->party_size), (uint8_t)1));
    in_camp->optimal_jobs[CAMPJOB_CLERGYMAN] = fmin((uint8_t)in_camp->max_jobs[CAMPJOB_CLERGYMAN], (uint8_t)fmax((uint8_t)floor(in_camp->fracs[CAMPJOB_CLERGYMAN] * in_camp->party_size), (uint8_t)1));
    in_camp->optimal_jobs[CAMPJOB_STORAGEMASTER] = fmin((uint8_t)in_camp->max_jobs[CAMPJOB_STORAGEMASTER], (uint8_t)floor(in_camp->fracs[CAMPJOB_STORAGEMASTER] * in_camp->party_size));
    if (in_camp->chapter < 10) {
        in_camp->optimal_jobs[CAMPJOB_SCRIBE] = 0;
        in_camp->optimal_jobs[CAMPJOB_MAGICRESEARCH] = 0;
        in_camp->optimal_jobs[CAMPJOB_ASSISTANT] = 0;
    } else {
        in_camp->optimal_jobs[CAMPJOB_SCRIBE] = 1;
        in_camp->optimal_jobs[CAMPJOB_MAGICRESEARCH] = 2;
        in_camp->optimal_jobs[CAMPJOB_ASSISTANT] = 1;
    }
}


