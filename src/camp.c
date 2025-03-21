
#include "camp.h"
#include "cJSON.h"
#include "SDL.h"

const u8 max_jobs[CAMPJOB_END] = {
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

const struct Camp Camp_default = {
    .jsonio_header.json_element   = JSON_CAMP,
};

void Camp_Free(struct Camp *camp) {
    s8_free(&camp->jsonio_header.json_filename);
}

void Camp_Job_Hire(struct Camp *in_camp, i16 unit_id, i16 job_id) {
    SDL_assert(in_camp->workers[job_id] < (max_jobs[job_id]));
    SDL_assert(in_camp->forbidden_jobs[unit_id] != job_id);
    u8 *jobs_arr = (u8 *)&in_camp->guards;
    u8 *job_arr = jobs_arr + ((job_id - 1) * CAMP_JOB_MAX);
    job_arr[in_camp->workers[job_id]++] = unit_id;
}

void Camp_Job_Fire(struct Camp *in_camp, i16 unit_id, i16 job_id) {
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

}

void Camp_Job_Forbid(struct Camp *in_camp, i16 unit_id, i16 job_id) {
    in_camp->forbidden_jobs[unit_id] = job_id;
}

u8 Camp_hasJob(struct Camp *in_camp, i16 unit_id) {
    u8 *jobs_arr = (u8 *)&in_camp->guards;
    b32 found = false;
    u8 has_job = CAMP_JOB_NULL;
    for (i16 job_id = 1; job_id < (CAMPJOB_END); job_id++) {
        u8 *job_arr = jobs_arr + ((job_id - 1) * CAMP_JOB_MAX);
        for (i16 i = 0; i < CAMP_JOB_MAX; i++) {
            if (job_arr[i] == unit_id)
                found = true;
        }
        if (found) {
            has_job = job_id;
            break;
        }
    }
    return (has_job);
}

u8 *Camp_Job_Get(struct Camp *in_camp, i16 job_id) {
    SDL_assert(in_camp->workers[job_id] < (max_jobs[job_id] - 1));
    u8 *jobs_arr = (u8 *)&in_camp->guards;
    u8 *job_arr = jobs_arr + (job_id * CAMP_JOB_MAX);
    return (job_arr);
}

void Camp_Jobs_Clear(struct Camp *in_camp) {
    u8 *jobs_arr = (u8 *)&in_camp->guards;
    for (int_fast8_t i = CAMPJOB_GUARD; i < CAMPJOB_END ; ++i) {
        u8 *job_arr = jobs_arr + ((i - 1) * CAMP_JOB_MAX);
        job_arr[0] = CAMP_JOB_NULL;
        job_arr[1] = CAMP_JOB_NULL;
        job_arr[2] = CAMP_JOB_NULL;
        job_arr[3] = CAMP_JOB_NULL;
    }
}

void Camp_writeJSON( void *input, cJSON *in_jcamp) {
    struct Camp *in_camp = (struct Camp *) input;
    SDL_assert(in_jcamp != NULL);
    u8 *jobs_arr = (u8 *)&in_camp->guards;
    cJSON *jjobs = cJSON_CreateArray();
    for (i16 i = 0; i < (CAMPJOB_END * CAMP_JOB_MAX); i++) {
        cJSON *temp = cJSON_CreateNumber(*(jobs_arr + i));
        cJSON_AddItemToArray(jjobs, temp);
    }
    cJSON *jforbidden = cJSON_CreateArray();
    for (i16 i = 0; i < UNIT_ID_PC_END; i++) {
        cJSON *temp = cJSON_CreateNumber(in_camp->forbidden_jobs[i]);
        cJSON_AddItemToArray(jforbidden, temp);
    }
    cJSON_AddItemToObject(in_jcamp, "Forbidden", jforbidden);
    cJSON_AddItemToObject(in_jcamp, "Jobs", jjobs);
}

void Camp_readJSON(void *input,  cJSON *in_jcamp) {
    struct Camp *in_camp = (struct Camp *) input;
    cJSON *jjobs = cJSON_GetObjectItemCaseSensitive(in_jcamp, "Jobs");
    u8 *jobs_arr = (u8 *)&in_camp->guards;
    for (i16 i = 0; i < (CAMPJOB_END * CAMP_JOB_MAX); i++) {
        cJSON *jtemp = cJSON_GetArrayItem(jjobs, i);
        *(jobs_arr + i) = cJSON_GetNumberValue(jtemp);
    }
    cJSON *jforbidden = cJSON_GetObjectItemCaseSensitive(in_jcamp, "Forbidden");
    int32_t forbidden_size = cJSON_GetArraySize(jforbidden);
    SDL_assert(forbidden_size == UNIT_ID_PC_END);
    for (i16 i = 0; i < UNIT_ID_PC_END; i++) {
        cJSON *jtemp = cJSON_GetArrayItem(jforbidden, i);
        in_camp->forbidden_jobs[i] = cJSON_GetNumberValue(jtemp);
    }
}
