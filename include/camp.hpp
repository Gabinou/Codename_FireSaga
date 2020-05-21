#ifndef CAMP_HPP
#define CAMP_HPP

#include "structs.hpp"
#include "enums.hpp"
#include "unit.hpp"
#include "game.hpp"
#include <math.h>
#include "filesystem.hpp"
#include <queue>

class Camp: public JSON_IO {
private:
    std::vector<short unsigned int> party;
    std::vector<short unsigned int> party_stack;
    std::vector<char> previous_job;
    std::vector<char> priority_job;
    std::vector<char> forbidden_job;
    std::vector<std::vector<short unsigned int>> jobs;
    std::queue<unsigned char> job_queue;

    // librarian, cook, guard, scribe, stablehand, asssitant, clergyman, storagemaster.
    // max_librarians should be floor(0.1 x Max army)
    // guards is twice that.
    std::vector<float> fracs = {0.1, 0.1, 0.2, 0, 0.1, 0, 0.15, 0.1};
    std::vector<unsigned char> max = {4, 4, 8, 1, 4, 1, 4, 4};
    // 30 jobs.
    // + 1 -> Erwin is the leader.
    // + 2/3 -> War council members? Pérignon/Chassé/ Pierre et Simon Laplace.?
    // + 1 -> armory merchant traveling with you.
    // + 1 -> item shop merchant traveling with you.
    std::vector<unsigned char> optimal;
    short int party_size;
    char chapter = -1;

public:
    Camp();
    ~Camp();

    void plusChapter();
    void setChapter(char in_chapter);
    void setParty(std::vector<short unsigned int> in_party);
    void makePartyStack();

    void makeJobQueue();
    void makeJobNumbers();
    void assignJobs();
    bool checkJobs();
    void clearJobs();

    void setpriorityJob(short unsigned int in_unit, char in_job);
    void setforbiddenJob(short unsigned int in_unit, char in_job);

    std::vector<std::vector<short unsigned int>> getJobs();
    std::queue<unsigned char> getJobqueue();

    using JSON_IO::writeJSON;
    using JSON_IO::readJSON;
    void readJSON(cJSON * in_json);
    void writeJSON(cJSON * in_json);
};

#endif /* CAMP_HPP */