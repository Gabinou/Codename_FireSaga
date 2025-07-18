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

#include "camp.h"
#include "cJSON.h"
#include "SDL.h"

const i32 max_jobs[CAMPJOB_END] = {
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

void Camp_Job_Hire(struct Camp *camp, i32 unit_id, i32 job_id) {
    SDL_assert(camp->jobs.workers[job_id] < (max_jobs[job_id]));
    SDL_assert(camp->jobs.forbidden[unit_id] != job_id);
    i32 *jobs_arr = (i32 *)&camp->jobs.guards;
    i32 *job_arr = jobs_arr + ((job_id - 1) * CAMP_JOB_MAX);
    job_arr[camp->jobs.workers[job_id]++] = unit_id;
}

void Camp_Job_Fire(struct Camp *camp, i32 unit_id, i32 job_id) {
    SDL_assert(camp->jobs.workers[job_id - 1] < (max_jobs[job_id - 1] - 1));
    i32 *jobs_arr = (i32 *)&camp->jobs.guards;
    i32 *job_arr = jobs_arr + ((job_id - 1) * CAMP_JOB_MAX);
    int_fast8_t index = -1;
    for (int_fast8_t i = 0; i < CAMP_JOB_MAX; i++) {
        if (unit_id == job_arr[i]) {
            index = i;
            break;
        }
    }
    SDL_assert((index > -1) && (index < CAMP_JOB_MAX));
    job_arr[index] = CAMP_JOB_NULL;
    if (index < --camp->jobs.workers[job_id])
        memmove(job_arr + index, job_arr + index + 1,
                (CAMP_JOB_MAX - index - 1) * sizeof(*job_arr));

}

void Camp_Job_Forbid(struct Camp *camp, i32 unit_id, i32 job_id) {
    camp->jobs.forbidden[unit_id] = job_id;
}

i32 Camp_hasJob(struct Camp *camp, i32 unit_id) {
    i32 *jobs_arr = (i32 *)&camp->jobs.guards;
    b32 found = false;
    i32 has_job = CAMP_JOB_NULL;
    for (i32 job_id = 1; job_id < (CAMPJOB_END); job_id++) {
        i32 *job_arr = jobs_arr + ((job_id - 1) * CAMP_JOB_MAX);
        for (i32 i = 0; i < CAMP_JOB_MAX; i++) {
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

i32 *Camp_Job_Get(struct Camp *camp, i32 job_id) {
    SDL_assert(camp->jobs.workers[job_id] < (max_jobs[job_id] - 1));
    i32 *jobs_arr = (i32 *)&camp->jobs.guards;
    i32 *job_arr = jobs_arr + (job_id * CAMP_JOB_MAX);
    return (job_arr);
}

void Camp_Jobs_Clear(struct Camp *camp) {
    i32 *jobs_arr = (i32 *)&camp->jobs.guards;
    for (int_fast8_t i = CAMPJOB_GUARD; i < CAMPJOB_END ; ++i) {
        i32 *job_arr = jobs_arr + ((i - 1) * CAMP_JOB_MAX);
        job_arr[0] = CAMP_JOB_NULL;
        job_arr[1] = CAMP_JOB_NULL;
        job_arr[2] = CAMP_JOB_NULL;
        job_arr[3] = CAMP_JOB_NULL;
    }
}

void Camp_writeJSON(const void *input, cJSON *in_jcamp) {
    struct Camp *camp = (struct Camp *) input;
    SDL_assert(in_jcamp != NULL);
    i32 *jobs_arr = (i32 *)&camp->jobs.guards;
    cJSON *jjobs = cJSON_CreateArray();
    for (i32 i = 0; i < (CAMPJOB_END * CAMP_JOB_MAX); i++) {
        cJSON *temp = cJSON_CreateNumber(*(jobs_arr + i));
        cJSON_AddItemToArray(jjobs, temp);
    }
    cJSON *jforbidden = cJSON_CreateArray();
    for (i32 i = 0; i < UNIT_ID_PC_END; i++) {
        cJSON *temp = cJSON_CreateNumber(camp->jobs.forbidden[i]);
        cJSON_AddItemToArray(jforbidden, temp);
    }
    cJSON_AddItemToObject(in_jcamp, "Forbidden", jforbidden);
    cJSON_AddItemToObject(in_jcamp, "Jobs", jjobs);
}

void Camp_readJSON(void *input, const cJSON *in_jcamp) {
    struct Camp *camp = (struct Camp *) input;
    cJSON *jjobs = cJSON_GetObjectItemCaseSensitive(in_jcamp, "Jobs");
    i32 *jobs_arr = (i32 *)&camp->jobs.guards;
    for (i32 i = 0; i < (CAMPJOB_END * CAMP_JOB_MAX); i++) {
        cJSON *jtemp = cJSON_GetArrayItem(jjobs, i);
        *(jobs_arr + i) = cJSON_GetNumberValue(jtemp);
    }
    cJSON *jforbidden = cJSON_GetObjectItemCaseSensitive(in_jcamp, "Forbidden");
    i32 forbidden_size = cJSON_GetArraySize(jforbidden);
    SDL_assert(forbidden_size == UNIT_ID_PC_END);
    for (i32 i = 0; i < UNIT_ID_PC_END; i++) {
        cJSON *jtemp = cJSON_GetArrayItem(jforbidden, i);
        camp->jobs.forbidden[i] = cJSON_GetNumberValue(jtemp);
    }
}
