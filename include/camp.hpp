#ifndef CAMP_HPP
#define CAMP_HPP

#include "structs.hpp"
#include "enums.hpp"
#include "utilities.hpp"
#include "filesystem.hpp"
#include <queue>

class Camp: public JSON_IO {
private:
    std::vector<uint16_t> party;
    std::vector<uint16_t> party_stack;
    std::vector<uint16_t> exclusions = {UNIT::NAME::ERWIN};
    std::vector<int8_t> previous_jobs;
    std::vector<int8_t> priority_jobs;
    std::vector<int8_t> forbidden_jobs;
    std::vector<std::vector<uint16_t>> jobs;
    std::queue<uint8_t> job_queue;

    // librarian, cook, guard, scribe, stablehand, asssitant, clergyman, storagemaster.
    std::vector<float> fracs = {0.1, 0.1, 0.2, 0, 0.1, 0, 0.15, 0.1};
    std::vector<uint8_t> max_jobs = {4, 4, 8, 1, 4, 1, 4, 4};
    // 30 jobs.
    // + 1 -> Erwin is the leader.
    // + 2/3 -> War council members? Pérignon/Chassé/ Pierre et Simon Laplace.?
    // War council members should still have tasks.
    // + 1 -> armory merchant traveling with you.
    // + 1 -> item shop merchant traveling with you.
    std::vector<uint8_t> optimal_jobs;
    short int party_size;
    int8_t chapter = -1;
    void checkChapter();

public:
    Camp();
    ~Camp();

    void plusChapter();
    void setChapter(int8_t in_chapter);
    void setParty(std::vector<uint16_t> in_party);
    std::vector<uint16_t> getParty();
    void makePartyStack();
    std::vector<uint16_t> getPartyStack();

    void makeJobQueue();
    void makeJobNumbers();
    void assignJobs();
    bool checkJobs();
    void clearJobs();

    void addExclusion(uint16_t in_unit);
    std::vector<uint16_t> getExclusions();
    void setpriorityJob(uint16_t in_unit, int8_t in_job);
    std::vector<int8_t> getpriorityJobs();
    void setforbiddenJob(uint16_t in_unit, int8_t in_job);
    std::vector<int8_t> getforbiddenJobs();
    void setpreviousJob(uint16_t in_unit, int8_t in_job);
    std::vector<int8_t> getpreviousJobs();

    std::vector<std::vector<uint16_t>> getJobs();
    std::queue<uint8_t> getJobqueue();
    std::vector<uint8_t> getOptimaljobs();

    using JSON_IO::writeJSON;
    using JSON_IO::readJSON;
    void readJSON(cJSON * in_json);
    void writeJSON(cJSON * in_json);
};

#endif /* CAMP_HPP */