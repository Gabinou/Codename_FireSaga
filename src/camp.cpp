#include "camp.hpp"
#include "probability.hpp"
#include "stb_sprintf.h"

Camp::Camp() {
    for (short int i = 0; i < UNIT::NAME::PC_END; i++) {
        previous_job.push_back(-1);
        priority_job.push_back(-1);
        forbidden_job.push_back(-1);
    }
}

Camp::~Camp() {

}

std::vector<short unsigned int> Camp::getParty() {
    return (party);
}

void Camp::plusChapter() {
    chapter++;
}

void Camp::writeJSON(cJSON * in_jcamp) {
    if (in_jcamp != NULL) {
        cJSON * jpriority = cJSON_CreateArray();
        cJSON * jprevious = cJSON_CreateArray();
        cJSON * jforbidden = cJSON_CreateArray();

        cJSON * temp;

        for (short int i = 0; i < UNIT::NAME::PC_END; i++) {
            temp = cJSON_CreateNumber(priority_job[i]);
            cJSON_AddItemToArray(jpriority, temp);
            temp = cJSON_CreateNumber(previous_job[i]);
            cJSON_AddItemToArray(jprevious, temp);
            temp = cJSON_CreateNumber(forbidden_job[i]);
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
    int previous_size = cJSON_GetArraySize(jprevious);
    int forbidden_size = cJSON_GetArraySize(jforbidden);
    int priority_size = cJSON_GetArraySize(jpriority);

    if (previous_size != UNIT::NAME::PC_END) {
        SDL_Log("Previous job array size is incorrect");
    }

    if (forbidden_size != UNIT::NAME::PC_END) {
        SDL_Log("Forbidden job array size is incorrect");
    }

    if (priority_size != UNIT::NAME::PC_END) {
        SDL_Log("Priority job array size is incorrect");
    }

    previous_job.clear();
    priority_job.clear();
    forbidden_job.clear();
    cJSON * jtemp;

    for (short int i = 0; i < UNIT::NAME::PC_END; i++) {
        jtemp = cJSON_GetArrayItem(jprevious, i);
        previous_job.push_back(cJSON_GetNumberValue(jtemp));
        jtemp = cJSON_GetArrayItem(jpriority, i);
        priority_job.push_back(cJSON_GetNumberValue(jtemp));
        jtemp = cJSON_GetArrayItem(jforbidden, i);
        forbidden_job.push_back(cJSON_GetNumberValue(jtemp));
    }
}

void Camp::setChapter(char in_chapter) {
    chapter = in_chapter;

    switch (chapter) {
        case 0:
        case 1:
            priority_job[UNIT::NAME::KIARA] = CAMPJOB::CLERGYMAN;
            break;

        case 2:
            break;

        case 3:
            break;

        case 4:
            break;

        case 5:
            break;

        case 6:
            break;

        case 7:
            break;

        case 8:
            break;

        case 9:
            break;

        case 10:
            break;

        case 11:
            break;

        case 12:
            break;

        case 13:
            break;

        case 14:
            break;

        case 15:
            break;

        case 16:
            break;

        case 17:
            break;

        case 18:
            break;

        case 19:
            break;

        case 20:
            break;

        case 21:
            break;
    }
}

void Camp::setpriorityJob(short unsigned int in_unit, char in_job) {
    priority_job[in_unit] = in_job;
}

void Camp::setforbiddenJob(short unsigned int in_unit, char in_job) {
    forbidden_job[in_unit] = in_job;
}

void Camp::makePartyStack() {
    party_stack.clear();

    for (short int i = 0; i < party.size(); i++) {
        if (party[i] != UNIT::NAME::ERWIN) {
            if (priority_job[party[i]] > -1) {
                jobs[priority_job[party[i]]].push_back(party[i]);
            } else {
                if (getURN() > 49) {
                    party_stack.push_back(party[i]);
                } else {
                    party_stack.insert(party_stack.begin(), party[i]);
                }
            }
        } else {
            SDL_Log("Party member's id is not the same as unordered map index");
        }
    }
}

std::vector<std::vector<short unsigned int>> Camp::getJobs() {
    return (jobs);
}


void Camp::makeJobQueue() {
    std::queue<unsigned char> empty;
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


void Camp::setParty(std::vector<short unsigned int> in_party) {
    party = in_party;
}

void Camp::clearJobs() {
    for (short int job = 0; job < jobs.size(); job++) {
        for (short int worker = 0; worker < jobs[job].size(); worker++) {
            previous_job[jobs[job][worker]] = job;
        }
    }
}

bool Camp::checkJobs() {
    bool out = false;

    if (jobs.size() == optimal.size()) {
        int num_filled = 0;

        for (short int i = 0; i < jobs.size(); i++) {
            if (jobs[i].size() >= optimal[i]) {
                num_filled++;

            }
        }

        out = (num_filled == 8);
    } else {
        SDL_Log("number of jobs and optimal is not the same");
    }

    return (out);
}

void Camp::assignJobs() {
    makeJobNumbers();
    makeJobQueue();
    makePartyStack();

    short int unit_ind;
    jobs.clear();

    while (party_stack.size() > 0) {
        unit_ind = party_stack[party_stack.size() - 1];

        if (jobs[job_queue.front()].size() >= optimal[job_queue.front()]) {
            job_queue.push(job_queue.front());
            job_queue.pop();
            continue;
        }

        if (forbidden_job[unit_ind] == job_queue.front()) {
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
    }
}

void Camp::makeJobNumbers() {
    party_size = party.size();

    optimal[0] = std::min((unsigned char)max[0], (unsigned char)floor(fracs[0] * party_size));
    optimal[1] = std::min((unsigned char)max[1], (unsigned char)std::max((unsigned char)floor(fracs[1] * party_size), (unsigned char)1));
    optimal[2] = std::min((unsigned char)max[2], (unsigned char)std::max((unsigned char)floor(fracs[2] * party_size), (unsigned char)1));
    optimal[4] = std::min((unsigned char)max[4], (unsigned char)std::max((unsigned char)floor(fracs[4] * party_size), (unsigned char)1));
    optimal[6] = std::min((unsigned char)max[6], (unsigned char)std::max((unsigned char)floor(fracs[6] * party_size), (unsigned char)1));
    optimal[7] = std::min((unsigned char)max[7], (unsigned char)floor(fracs[0] * party_size));

    if (chapter < 10) {
        optimal[3] = 0;
    } else {
        optimal[3] = 1;
    }

    if (chapter < 10) {
        optimal[5] = 0;
    } else {
        optimal[5] = 1;
    }


}
