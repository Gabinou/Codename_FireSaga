#include "minctest.h"
#include "camp.hpp"

void printJobs(std::vector<std::vector<short unsigned int>> in_jobs) {
    for (int job = 0; job < in_jobs.size(); jop++) {
        SDL_Log("Job: %s", jobNames[job].c_str());

        for (int worker = 0; worker < in_jobs[job].size(); worker++) {
            SDL_Log("Worker: %s", unitNames[in_jobs[job][worker]].c_str());
        }
    }
}


void test_camp() {
    std::vector<short unsigned int> test_party1 = {UNIT::NAME::ERWIN,
                                                   UNIT::NAME::KIARA,
                                                  };
    std::vector<short unsigned int> test_party2 = {UNIT::NAME::ERWIN,
                                                   UNIT::NAME::KIARA, UNIT::NAME::SILOU, UNIT::NAME::PIROU, UNIT::NAME::SERVIL,
                                                   UNIT::NAME::PERIGNON, UNIT::NAME::LAUZON, UNIT::NAME::NICOLE, UNIT::NAME::OTTO,
                                                   UNIT::NAME::SEBASTIAN, UNIT::NAME::MICHAEL, UNIT::NAME::CHASSE, UNIT::NAME::PIERRE,
                                                   UNIT::NAME::MELLY, UNIT::NAME::SIMON, UNIT::NAME::EUGENE, UNIT::NAME::TEHARON,
                                                   UNIT::NAME::TEKAK, UNIT::NAME::LUCRECE,
                                                  };

    Camp test_camp;
    test_camp.setParty(test_party1);

    std::vector<std::vector<short unsigned int>> temp_jobs = test_camp.getJobs();

    SDL_Log

}
