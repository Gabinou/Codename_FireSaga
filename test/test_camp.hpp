#include "minctest.h"
#include "camp.hpp"
#include "structs.hpp"

void printJobs(std::vector<std::vector<short unsigned int>> in_jobs) {
    for (int job = 0; job < in_jobs.size(); job++) {
        SDL_Log("Job: %s", jobNames[job].c_str());

        for (int worker = 0; worker < in_jobs[job].size(); worker++) {
            SDL_Log("Worker: %s", unitNames[in_jobs[job][worker]].c_str());
        }
    }
}


void test_camp() {
    SDL_Log("test_camp");
    std::vector<short unsigned int> in_party1 = {UNIT::NAME::ERWIN,
                                                 UNIT::NAME::KIARA,
                                                };
    std::vector<short unsigned int> in_party2 = {UNIT::NAME::ERWIN,
                                                 UNIT::NAME::KIARA, UNIT::NAME::SILOU, UNIT::NAME::PIROU, UNIT::NAME::SERVIL,
                                                 UNIT::NAME::PERIGNON, UNIT::NAME::LAUZON, UNIT::NAME::NICOLE, UNIT::NAME::OTTO,
                                                 UNIT::NAME::SEBASTIAN, UNIT::NAME::MICHAEL, UNIT::NAME::CHASSE, UNIT::NAME::PIERRE,
                                                 UNIT::NAME::MELLY, UNIT::NAME::SIMON, UNIT::NAME::EUGENE, UNIT::NAME::TEHARON,
                                                 UNIT::NAME::TEKAK, UNIT::NAME::LUCRECE,
                                                }; //19
    std::vector<char> previous_job;
    std::vector<char> priority_job;
    std::vector<char> forbidden_job;

    for (short int i = 0; i < UNIT::NAME::PC_END; i++) {
        previous_job.push_back(-1);
        priority_job.push_back(-1);
        forbidden_job.push_back(-1);
    }

    Camp test_camp1, test_camp2;

    test_camp1.setParty(in_party1);
    std::vector<short unsigned int> out_party1;
    out_party1 = test_camp1.getParty();
    lok(out_party1 == in_party1);

    std::vector<char> out_priorityjobs;
    out_priorityjobs = test_camp1.getpriorityJobs();
    lok(out_priorityjobs[UNIT::NAME::KIARA] == CAMPJOB::CLERGYMAN);

    std::queue<unsigned char> out_queue;
    test_camp1.makeJobQueue();
    out_queue = test_camp1.getJobqueue();
    lok(out_queue.front() == CAMPJOB::COOK);
    out_queue.pop();
    lok(out_queue.front() == CAMPJOB::STABLEHAND);
    out_queue.pop();
    lok(out_queue.front() == CAMPJOB::GUARD);
    out_queue.pop();
    lok(out_queue.front() == CAMPJOB::STORAGEMASTER);
    out_queue.pop();
    lok(out_queue.front() == CAMPJOB::CLERGYMAN);
    out_queue.pop();
    lok(out_queue.front() == CAMPJOB::LIBRARIAN);
    out_queue.pop();
    lok(out_queue.front() == CAMPJOB::SCRIBE);
    out_queue.pop();
    lok(out_queue.front() == CAMPJOB::ASSISTANT);

    std::vector<unsigned char> out_optimal;
    test_camp1.makeJobNumbers();
    out_optimal = test_camp1.getOptimaljobs();
    lok(out_optimal[CAMPJOB::LIBRARIAN] == 0);
    lok(out_optimal[CAMPJOB::COOK] == 1);
    lok(out_optimal[CAMPJOB::GUARD] == 1);
    lok(out_optimal[CAMPJOB::SCRIBE] == 0);
    lok(out_optimal[CAMPJOB::STABLEHAND] == 1);
    lok(out_optimal[CAMPJOB::ASSISTANT] == 0);
    lok(out_optimal[CAMPJOB::CLERGYMAN] == 1);
    lok(out_optimal[CAMPJOB::STORAGEMASTER] == 0);

    test_camp1.clearJobs();
    test_camp1.makePartyStack();
    std::vector<std::vector<short unsigned int>> out_jobs;
    out_jobs = test_camp1.getJobs();
    lok(out_jobs[CAMPJOB::LIBRARIAN].size() == 0);
    lok(out_jobs[CAMPJOB::COOK].size() == 0);
    lok(out_jobs[CAMPJOB::GUARD].size() == 0);
    lok(out_jobs[CAMPJOB::SCRIBE].size() == 0);
    lok(out_jobs[CAMPJOB::STABLEHAND].size() == 0);
    lok(out_jobs[CAMPJOB::ASSISTANT].size() == 0);
    lok(out_jobs[CAMPJOB::CLERGYMAN].size() == 1);
    lok(out_jobs[CAMPJOB::STORAGEMASTER].size() == 0);

    if (out_jobs[CAMPJOB::CLERGYMAN].size() == 1) {
        lok(out_jobs[CAMPJOB::CLERGYMAN][0] == UNIT::NAME::KIARA);
    }


    test_camp2.setParty(in_party2);
    std::vector<short unsigned int> out_party2;
    out_party2 = test_camp2.getParty();
    lok(out_party2 == in_party2);

    out_priorityjobs = test_camp2.getpriorityJobs();
    lok(out_priorityjobs[UNIT::NAME::KIARA] == CAMPJOB::CLERGYMAN);

    test_camp2.makeJobQueue();
    out_queue = test_camp2.getJobqueue();
    lok(out_queue.front() == CAMPJOB::COOK);
    out_queue.pop();
    lok(out_queue.front() == CAMPJOB::STABLEHAND);
    out_queue.pop();
    lok(out_queue.front() == CAMPJOB::GUARD);
    out_queue.pop();
    lok(out_queue.front() == CAMPJOB::STORAGEMASTER);
    out_queue.pop();
    lok(out_queue.front() == CAMPJOB::CLERGYMAN);
    out_queue.pop();
    lok(out_queue.front() == CAMPJOB::LIBRARIAN);
    out_queue.pop();
    lok(out_queue.front() == CAMPJOB::SCRIBE);
    out_queue.pop();
    lok(out_queue.front() == CAMPJOB::ASSISTANT);

    test_camp2.makeJobNumbers();
    out_optimal = test_camp2.getOptimaljobs();
    lok(out_optimal[CAMPJOB::LIBRARIAN] == 1);
    lok(out_optimal[CAMPJOB::COOK] == 1);
    lok(out_optimal[CAMPJOB::GUARD] == 3);
    lok(out_optimal[CAMPJOB::SCRIBE] == 0);
    lok(out_optimal[CAMPJOB::STABLEHAND] == 1);
    lok(out_optimal[CAMPJOB::ASSISTANT] == 0);
    lok(out_optimal[CAMPJOB::CLERGYMAN] == 2);
    lok(out_optimal[CAMPJOB::STORAGEMASTER] == 1);

    // test_camp2.clearJobs();
    // test_camp2.makePartyStack();
    // std::vector<std::vector<short unsigned int>> out_jobs;
    // out_jobs = test_camp1.getJobs();
    // lok(out_jobs[CAMPJOB::LIBRARIAN].size() == 0);
    // lok(out_jobs[CAMPJOB::COOK].size() == 0);
    // lok(out_jobs[CAMPJOB::GUARD].size() == 0);
    // lok(out_jobs[CAMPJOB::SCRIBE].size() == 0);
    // lok(out_jobs[CAMPJOB::STABLEHAND].size() == 0);
    // lok(out_jobs[CAMPJOB::ASSISTANT].size() == 0);
    // lok(out_jobs[CAMPJOB::CLERGYMAN].size() == 1);
    // lok(out_jobs[CAMPJOB::STORAGEMASTER].size() == 0);

    // if (out_jobs[CAMPJOB::CLERGYMAN].size() == 1) {
    //     lok(out_jobs[CAMPJOB::CLERGYMAN][0] == UNIT::NAME::KIARA);
    // }


    // test_camp1.assignJobs();
}
