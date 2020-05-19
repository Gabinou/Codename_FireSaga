#ifndef CAMP_HPP
#define CAMP_HPP

#include "structs.hpp"
#include "enums.hpp"
#include "unit.hpp"
#include "game.hpp"
#include <math.h>
#include <queue>

class Camp {

private:
    std::unordered_map<short int, Unit> party;
    std::unordered_map<short unsigned int, char> previous_job;
    std::unordered_map<short unsigned int, char> priority_job;
    std::unordered_map<short unsigned int, std::vector<char>> forbidden_job;
    std::queue<unsigned char> job_queue;

    std::vector<short unsigned int> party_stack;
    // Job priority: cook > stablehand > guard > clergymen > scribe > assistant
    // librarian, cook, guard, scribe, stablehand, asssitant, clergyman, storagemaster.
    std::vector<std::vector<short unsigned int>> jobs;
    // max_librarians should be floor(0.1 x Max army)
    // guards is twice that.
    std::vector<float> fracs = {0.1, 0.1, 0.2, 0, 0.1, 0, 0.15, 0.1};
    std::vector<unsigned char> max = {4, 4, 8, 1, 4, 1, 4, 4};
    // 30 jobs.
    // + 1 -> Erwin is the leader.
    // + 1 -> armory merchant traveling with you.
    // + 1 -> item shop merchant traveling with you.
    std::vector<unsigned char> optimal;
    short int party_size;
    char chapter = -1;

public:
    Camp();
    ~Camp();

    void setChapter(char in_chapter);
    void setParty(std::unordered_map<short int, Unit> in_party);
    void makePartyStack();

    void makeJobQueue();
    void makeJobNumbers();
    void assignJobs();

    // using XML_IO::writeXML;
    // using XML_IO::readXML;
    // void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pTile);
    // void readXML(tinyxml2::XMLElement * in_pTile);
};

#endif /* CAMP_HPP */