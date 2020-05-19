#ifndef CAMP_HPP
#define CAMP_HPP

#include "structs.hpp"
#include "enums.hpp"
#include "unit.hpp"
#include "game.hpp"
#include <math.h>

class Camp {

private:
    Game * game;
    std::unordered_map<short int, Unit> party;
    // Job priority: cook > stablehand > guard > clergymen > scribe > assistant 
    std::vector librarians;
    std::vector cooks;
    std::vector guards;
    std::vector scribe;
    std::vector stablehands;
    std::vector assistant; // Kiara?
    std::vector clergymen;
    std::vector storagemaster;

    float frac_librarian = 0.1;
    float frac_cooks = 0.1;
    float frac_guards = 0.2;
    float frac_stablehands = 0.1;
    float frac_clergymen = 0.15;
    float frac_storagemaster = 0.1;
    // max_librarians should be floor(0.1 x Max army)
    // guards is twice that.
    unsigned char max_librarians = 4;
    unsigned char max_cooks = 4;
    unsigned char max_guards = 8;
    unsigned char max_scribe = 1;
    unsigned char max_stablehands = 4;
    unsigned char max_assistant = 1;
    unsigned char max_clergymen = 4;
    unsigned char max_storagemaster = 4;

    short int optimal_librarians;
    short int optimal_cooks;
    short int optimal_guards;
    short int optimal_stablehands;
    short int optimal_storagemaster;
    short int optimal_clergymen;
    short int optimal_scribe;
    short int optimal_assistant;
    short int party_size;


    // 30 jobs. 
    // + 1 -> Erwin is the leader.
    // + 1 -> armory merchant traveling with you.
    // + 1 -> item shop merchant traveling with you.
public:
    Camp();
    ~Camp();

    void setGame(Game * in_game);
    void updateParty();
    void makeJobNumbers();
    void fillJobs();
    // using XML_IO::writeXML;
    // using XML_IO::readXML;
    // void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pTile);
    // void readXML(tinyxml2::XMLElement * in_pTile);
};

#endif /* CAMP_HPP */