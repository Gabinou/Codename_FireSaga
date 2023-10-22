#include "camp.h"

u8 max_jobs[CAMPJOB_END] = {
    0,
    CAMP_GUARD_MAX,
    CAMP_LIBRARIAN_MAX,
    CAMP_ASSISTANT_MAX,
    CAMP_COOK_MAX,
    CAMP_SCRIBE_MAX,
    CAMP_STABLEHAND_MAX,
    CAMP_CLERGYMAN_MAX,
    CAMP_STORAGEMASTER_MAX,
    CAMP_MAGICRESEARCHER_MAX,
};

struct Camp Camp_default = {
    .json_element = JSON_CAMP,

    .guards = {CAMP_JOB_NULL},
    .librarians = {CAMP_JOB_NULL},
    .assistants = {CAMP_JOB_NULL},
    .cooks = {CAMP_JOB_NULL},
    .scribes = {CAMP_JOB_NULL},
    .stablehands = {CAMP_JOB_NULL},
    .clergymen = {CAMP_JOB_NULL},
    .storagemasters = {CAMP_JOB_NULL},
    .magicresearchers = {CAMP_JOB_NULL},
    .workers = {0},

    .forbidden_jobs = {CAMP_JOB_NULL},
};

void Camp_Job_Hire(struct Camp *in_camp, uint_fast16_t unit_id, uint_fast16_t job_id) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(in_camp->workers[job_id] < (max_jobs[job_id]));
    SDL_assert(in_camp->forbidden_jobs[unit_id] != job_id);
    u8 *jobs_arr = (u8 *)&in_camp->guards;
    u8 *job_arr = jobs_arr + ((job_id - 1) * CAMP_JOB_MAX);
    job_arr[in_camp->workers[job_id]++] = unit_id;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Camp_Job_Fire(struct Camp *in_camp, uint_fast16_t unit_id, uint_fast16_t job_id) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(in_camp->workers[job_id - 1] < (max_jobs[job_id - 1] - 1));
    u8 *jobs_arr = (u8 *)&in_camp->guards;
    u8 *job_arr = jobs_arr + ((job_id - 1) * CAMP_JOB_MAX);
    int_fast8_t index = -1;
    for (int_fast8_t i = 0; i < CAMP_JOB_MAX; i++) {
        if (unit_id == job_arr[i]) {
            index = i;
            break;
        }
    }
    SDL_assert((index > -1) && (index < CAMP_JOB_MAX));
    job_arr[index] = CAMP_JOB_NULL;
    if (index < --in_camp->workers[job_id])
        memmove(job_arr + index, job_arr + index + 1,
                (CAMP_JOB_MAX - index - 1) * sizeof(*job_arr));
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Camp_Job_Forbid(struct Camp *in_camp, uint_fast16_t unit_id, uint_fast16_t job_id) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    in_camp->forbidden_jobs[unit_id] = job_id;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

u8 Camp_hasJob(struct Camp *in_camp, uint_fast16_t unit_id) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u8 *jobs_arr = (u8 *)&in_camp->guards;
    bool found = false;
    u8 has_job = CAMP_JOB_NULL;
    for (int_fast16_t job_id = 1; job_id < (CAMPJOB_END); job_id++) {
        u8 *job_arr = jobs_arr + ((job_id - 1) * CAMP_JOB_MAX);
        for (int_fast16_t i = 0; i < CAMP_JOB_MAX; i++) {
            if (job_arr[i] == unit_id)
                found = true;
        }
        if (found) {
            has_job = job_id;
            break;
        }
    }
    return (has_job);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

u8 *Camp_Job_Get(struct Camp *in_camp, uint_fast16_t job_id) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(in_camp->workers[job_id] < (max_jobs[job_id] - 1));
    u8 *jobs_arr = (u8 *)&in_camp->guards;
    u8 *job_arr = jobs_arr + (job_id * CAMP_JOB_MAX);
    return (job_arr);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Camp_Jobs_Clear(struct Camp *in_camp) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u8 *jobs_arr = (u8 *)&in_camp->guards;
    for (int_fast8_t i = CAMPJOB_GUARD; i < CAMPJOB_END ; ++i) {
        u8 *job_arr = jobs_arr + ((i - 1) * CAMP_JOB_MAX);
        job_arr[0] = CAMP_JOB_NULL;
        job_arr[1] = CAMP_JOB_NULL;
        job_arr[2] = CAMP_JOB_NULL;
        job_arr[3] = CAMP_JOB_NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Camp_writeJSON(const void *input, cJSON *in_jcamp) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Camp *in_camp = (struct Camp *) input;
    SDL_assert(in_jcamp != NULL);
    u8 *jobs_arr = (u8 *)&in_camp->guards;
    cJSON *jjobs = cJSON_CreateArray();
    for (int_fast16_t i = 0; i < (CAMPJOB_END * CAMP_JOB_MAX); i++) {
        cJSON *temp = cJSON_CreateNumber(*(jobs_arr + i));
        cJSON_AddItemToArray(jjobs, temp);
    }
    cJSON *jforbidden = cJSON_CreateArray();
    for (int_fast16_t i = 0; i < UNIT_ID_PC_END; i++) {
        cJSON *temp = cJSON_CreateNumber(in_camp->forbidden_jobs[i]);
        cJSON_AddItemToArray(jforbidden, temp);
    }
    cJSON_AddItemToObject(in_jcamp, "Forbidden", jforbidden);
    cJSON_AddItemToObject(in_jcamp, "Jobs", jjobs);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Camp_readJSON(void *input, const cJSON *const in_jcamp) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Camp *in_camp = (struct Camp *) input;
    cJSON *jjobs = cJSON_GetObjectItemCaseSensitive(in_jcamp, "Jobs");
    u8 *jobs_arr = (u8 *)&in_camp->guards;
    for (int_fast16_t i = 0; i < (CAMPJOB_END * CAMP_JOB_MAX); i++) {
        cJSON *jtemp = cJSON_GetArrayItem(jjobs, i);
        *(jobs_arr + i) = cJSON_GetNumberValue(jtemp);
    }
    cJSON *jforbidden = cJSON_GetObjectItemCaseSensitive(in_jcamp, "Forbidden");
    int32_t forbidden_size = cJSON_GetArraySize(jforbidden);
    SDL_assert(forbidden_size == UNIT_ID_PC_END);
    for (int_fast16_t i = 0; i < UNIT_ID_PC_END; i++) {
        cJSON *jtemp = cJSON_GetArrayItem(jforbidden, i);
        in_camp->forbidden_jobs[i] = cJSON_GetNumberValue(jtemp);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
