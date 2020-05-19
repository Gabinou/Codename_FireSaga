#ifndef CAMP_HPP
#define CAMP_HPP

#include "structs.hpp"
#include "enums.hpp"
// #include "physfs.h"
#include "unit.hpp"
#include "game.hpp"
// #include "filesys/**/tem.hpp"

// class Camp : public XML_IO, JSON_IO {
class Camp {
private:
    Game * game;
    std::unordered_map<short int, Unit> party;
    // Job priority: cook > stablehand > guard > clergymen >
    std::vector librarians;
    std::vector cooks;
    std::vector guards;
    std::vector scribe;
    std::vector stablehands;
    std::vector assistant; // Kiara?
    std::vector clergymen;
public:
    Camp();
    ~Camp();

    void setGame(Game * in_game);
    void updateParty();
    void makeJobNumbers();
    // using XML_IO::writeXML;
    // using XML_IO::readXML;
    // void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pTile);
    // void readXML(tinyxml2::XMLElement * in_pTile);
};

#endif /* CAMP_HPP */