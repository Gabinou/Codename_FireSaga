#include "camp.hpp"
#include "probability.hpp"
#include "stb_sprintf.h"

Camp::Camp() {
    for (int16_t i = 0; i < UNIT::NAME::PC_END; i++) {
        priority_jobs.push_back(-1);
        priority_jobs.push_back(-1);
        forbidden_jobs.push_back(-1);
    }

    setJSONElement("Camp");
    setChapter(1);
}

Camp::~Camp() {

}

std::vector<uint16_t> Camp::getParty() {
    return (party);
}

void Camp::plusChapter() {
    chapter++;
    checkChapter();
}

void Camp::writeJSON(cJSON * in_jcamp) {
    if (in_jcamp != NULL) {
        cJSON * jpriority = cJSON_CreateArray();
        cJSON * jprevious = cJSON_CreateArray();
        cJSON * jforbidden = cJSON_CreateArray();
        cJSON * temp;

        for (int16_t i = 0; i < UNIT::NAME::PC_END; i++) {
            temp = cJSON_CreateNumber(priority_jobs[i]);
            cJSON_AddItemToArray(jpriority, temp);
            temp = cJSON_CreateNumber(priority_jobs[i]);
            cJSON_AddItemToArray(jprevious, temp);
            temp = cJSON_CreateNumber(forbidden_jobs[i]);
            cJSON_AddItemToArray(jforbidden, temp);
        }

        cJSON_AddItemToObject(in_jcamp, "Priority job", jpriority);
        cJSON_AddItemToObject(in_jcamp, "Previous job", jprevious);
        cJSON_AddItemToObject(in_jcamp, "Forbidden job", jforbidden);
    }
}

void Camp::readJSON(cJSON * in_jcamp) {
    cJSON * jprevious = cJSON_GetObjectItemCaseSensitive(in_jcamp, "Previous job");
    cJSON * jforbidden = cJSON_GetObjectItemCaseSensitive(in_jcamp, "Forbidden job");
    cJSON * jpriority = cJSON_GetObjectItemCaseSensitive(in_jcamp, "Priority job");
    int32_t previous_size = cJSON_GetArraySize(jprevious);
    int32_t forbidden_size = cJSON_GetArraySize(jforbidden);
    int32_t priority_size = cJSON_GetArraySize(jpriority);

    if (previous_size != UNIT::NAME::PC_END) {
        SDL_Log("Previous job array size is incorrect");
    }

    if (forbidden_size != UNIT::NAME::PC_END) {
        SDL_Log("Forbidden job array size is incorrect");
    }

    if (priority_size != UNIT::NAME::PC_END) {
        SDL_Log("Priority job array size is incorrect");
    }

    priority_jobs.clear();
    priority_jobs.clear();
    forbidden_jobs.clear();
    cJSON * jtemp;

    for (int16_t i = 0; i < UNIT::NAME::PC_END; i++) {
        jtemp = cJSON_GetArrayItem(jprevious, i);
        previous_jobs.push_back(cJSON_GetNumberValue(jtemp));
        jtemp = cJSON_GetArrayItem(jpriority, i);
        priority_jobs.push_back(cJSON_GetNumberValue(jtemp));
        jtemp = cJSON_GetArrayItem(jforbidden, i);
        forbidden_jobs.push_back(cJSON_GetNumberValue(jtemp));
    }
}

std::vector<uint16_t> Camp::getExclusions() {
    return (exclusions);
}

void Camp::addExclusion(uint16_t in_unit) {
    exclusions.push_back(in_unit);
}


void Camp::checkChapter() {

    switch (chapter) {
        // case CHAPTER::TEST:
        //     break;

        // case CHAPTER::CHAP_2:
        //     priority_jobs[UNIT::NAME::KIARA] = CAMPJOB::CLERGYMAN;
        //     break;

        // case CHAPTER::CHAP_2:
        //     break;

        default:
            SDL_Log("in_chapter is invalid");
    }
}


void Camp::setChapter(int8_t in_chapter) {
    chapter = in_chapter;
    checkChapter();
}

void Camp::setpriorityJob(uint16_t in_unit, int8_t in_job) {
    priority_jobs[in_unit] = in_job;
}

void Camp::setforbiddenJob(uint16_t in_unit, int8_t in_job) {
    forbidden_jobs[in_unit] = in_job;
}

void Camp::setpreviousJob(uint16_t in_unit, int8_t in_job) {
    previous_jobs[in_unit] = in_job;
}

std::vector<int8_t> Camp::getforbiddenJobs() {
    return (forbidden_jobs);
}

std::vector<int8_t> Camp::getpriorityJobs() {
    return (priority_jobs);
}

std::vector<int8_t> Camp::getpreviousJobs() {
    return (forbidden_jobs);
}

std::vector<uint16_t> Camp::getPartyStack() {
    return (party_stack);
}

void Camp::makePartyStack() {
    party_stack.clear();

    for (int16_t i = 0; i < party.size(); i++) {
        if (!cppisin(party[i], exclusions)) {
            if (priority_jobs[party[i]] > -1) {
                jobs[priority_jobs[party[i]]].push_back(party[i]);
            } else {
                if (getURN() > 49) {
                    party_stack.push_back(party[i]);
                } else {
                    party_stack.insert(party_stack.begin(), party[i]);
                }
            }
        }
    }
}

std::vector<std::vector<uint16_t>> Camp::getJobs() {
    return (jobs);
}


void Camp::makeJobQueue() {
    std::queue<uint8_t> empty;
    std::swap(job_queue, empty);
    // Job priority: cook > stablehand > guard > storagemaster > clergymen > librarian > scribe > assistant
    job_queue.push(CAMPJOB::COOK);
    job_queue.push(CAMPJOB::STABLEHAND);
    job_queue.push(CAMPJOB::GUARD);
    job_queue.push(CAMPJOB::STORAGEMASTER);
    job_queue.push(CAMPJOB::CLERGYMAN);
    job_queue.push(CAMPJOB::LIBRARIAN);
    job_queue.push(CAMPJOB::SCRIBE);
    job_queue.push(CAMPJOB::ASSISTANT);
}


void Camp::setParty(std::vector<uint16_t> in_party) {
    party = in_party;
}

void Camp::clearJobs() {
    jobs.clear();
    jobs.resize(CAMPJOB::STORAGEMASTER + 1);

    for (int32_t i = 0; i < jobs.size(); ++i) {
        jobs[i].clear();
    }
}

bool Camp::checkJobs() {
    bool out = false;

    if (jobs.size() == optimal_jobs.size()) {
        int32_t num_filled = 0;

        for (int16_t i = 0; i < jobs.size(); i++) {
            if (jobs[i].size() >= optimal_jobs[i]) {
                num_filled++;

            }
        }

        out = (num_filled == 8);
    } else {
        SDL_Log("number of jobs and optimal_jobs is not the same");
    }

    return (out);
}

void Camp::assignJobs() {
    int16_t unit_ind;

    while (party_stack.size() > 0) {
        unit_ind = party_stack[party_stack.size() - 1];
        SDL_Log("Stacking unit %d, %s", unit_ind, unitNames[unit_ind].c_str());

        if (jobs[job_queue.front()].size() >= optimal_jobs[job_queue.front()]) {
            job_queue.push(job_queue.front());
            job_queue.pop();
            continue;
        }

        if (forbidden_jobs[unit_ind] == job_queue.front()) {
            job_queue.push(job_queue.front());
            job_queue.pop();
            continue;
        }

        jobs[job_queue.front()].push_back(unit_ind);

        if (checkJobs()) {
            break;
        }

        party_stack.pop_back();
        job_queue.push(job_queue.front());
        job_queue.pop();
        SDL_Log("job_queue size %d", job_queue.size());
        SDL_Log("party_stack size %d", party_stack.size());
    }
}

std::queue<uint8_t> Camp::getJobqueue() {
    return (job_queue);
}

std::vector<uint8_t> Camp::getOptimaljobs() {
    return (optimal_jobs);
}


void Camp::makeJobNumbers() {
    party_size = party.size();
    optimal_jobs.clear();
    optimal_jobs.resize(CAMPJOB::STORAGEMASTER + 1);
    optimal_jobs[CAMPJOB::LIBRARIAN] = std::min((uint8_t)max_jobs[CAMPJOB::LIBRARIAN], (uint8_t)floor(fracs[CAMPJOB::LIBRARIAN] * party_size));
    optimal_jobs[CAMPJOB::COOK] = std::min((uint8_t)max_jobs[CAMPJOB::COOK], (uint8_t)std::max((uint8_t)floor(fracs[CAMPJOB::COOK] * party_size), (uint8_t)1));
    optimal_jobs[CAMPJOB::GUARD] = std::min((uint8_t)max_jobs[CAMPJOB::GUARD], (uint8_t)std::max((uint8_t)floor(fracs[CAMPJOB::GUARD] * party_size), (uint8_t)1));
    optimal_jobs[CAMPJOB::STABLEHAND] = std::min((uint8_t)max_jobs[CAMPJOB::STABLEHAND], (uint8_t)std::max((uint8_t)floor(fracs[CAMPJOB::STABLEHAND] * party_size), (uint8_t)1));
    optimal_jobs[CAMPJOB::CLERGYMAN] = std::min((uint8_t)max_jobs[CAMPJOB::CLERGYMAN], (uint8_t)std::max((uint8_t)floor(fracs[CAMPJOB::CLERGYMAN] * party_size), (uint8_t)1));
    optimal_jobs[CAMPJOB::STORAGEMASTER] = std::min((uint8_t)max_jobs[CAMPJOB::STORAGEMASTER], (uint8_t)floor(fracs[0] * party_size));

    if (chapter < 10) {
        optimal_jobs[CAMPJOB::SCRIBE] = 0;
    } else {
        optimal_jobs[CAMPJOB::SCRIBE] = 1;
    }

    if (chapter < 10) {
        optimal_jobs[CAMPJOB::ASSISTANT] = 0;
    } else {
        optimal_jobs[CAMPJOB::ASSISTANT] = 1;
    }


}
