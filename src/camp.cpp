#include "camp.hpp"
#include "probability.hpp"
#include "stb_sprintf.h"

Camp::Camp() {
    for (int i = 0; i < UNIT::NAME::PC_END; i++) {
        previous_job[i] = -1;
        priority_job[i] = -1;
        forbidden_job[i] = -1;
    }
}

Camp::~Camp() {

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


void Camp::makePartyStack() {
    party_stack.clear();

    for (auto member : party) {
        if ((member.first == member.second.getid()) && (member.first != UNIT::NAME::ERWIN)) {
            if (priority_job[member.first] > -1) {
                jobs[priority_job[member.first]].push_back(member.first);
            } else {
                if (getURN() > 49) {
                    party_stack.push_back(member.first);
                } else {
                    party_stack.insert(party_stack.begin(), member.first);
                }
            }
        } else {
            SDL_Log("Party member's id is not the same as unordered map index");
        }
    }
}

void Camp::makeJobQueue() {
    std::queue<unsigned char> empty;
    std::swap(job_queue, empty);
    job_queue.push(CAMPJOB::LIBRARIAN);
    job_queue.push(CAMPJOB::COOK);
    job_queue.push(CAMPJOB::GUARD);
    job_queue.push(CAMPJOB::SCRIBE);
    job_queue.push(CAMPJOB::STABLEHAND);
    job_queue.push(CAMPJOB::ASSISTANT);
    job_queue.push(CAMPJOB::CLERGYMAN);
    job_queue.push(CAMPJOB::STORAGEMASTER);
}


void Camp::setParty(std::unordered_map<short int, Unit> in_party) {
    party = in_party;
}

void Camp::assignJobs() {

    int jobnum = 0;
    bool jobfound = false;
    short int unit_ind;

    while (party_stack.size() > 0) {
        unit_ind = party_stack[party_stack.size() - 1];

        while ((jobnum < 8) && (!jobfound)) {
            switch (unit_ind) {
                case UNIT::NAME::ERWIN:
                case UNIT::NAME::KIARA:
                case UNIT::NAME::SILOU:
                    jobs[job_queue.front()] = unit_ind;
                    break;

                case UNIT::NAME::SERVIL:
                case UNIT::NAME::PERIGNON:
                case UNIT::NAME::POET:
                case UNIT::NAME::RELIABLE:
                case UNIT::NAME::COWARD:
                case UNIT::NAME::JAIGEN1H:
                case UNIT::NAME::HOTTIE:
                    break;
            }

            job_queue.push(job_queue.front());
            job_queue.pop();
            jobnum++;
        }

        jobnum = 0;
        party_stack.pop_back();
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
