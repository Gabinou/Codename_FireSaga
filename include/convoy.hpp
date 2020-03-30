#ifndef CONVOY_HPP
#define CONVOY_HPP

#include "enums.hpp"
#include "structs.hpp"
#include "physfs.h"
#include "tinyxml2.h"
#include "filesystem.hpp"
#include "utilities.hpp"
#include "string.h"
#include <string>

class Convoy {
    private:
        Inventory_item convoy[DEFAULT::CONVOY_SIZE], swords[DEFAULT::CONVOY_SIZE], lances[DEFAULT::CONVOY_SIZE];
        axe[DEFAULT::CONVOY_SIZE];
         bow[DEFAULT::CONVOY_SIZE];
         trinket[DEFAULT::CONVOY_SIZE];
         offhand[DEFAULT::CONVOY_SIZE];
         elemental[DEFAULT::CONVOY_SIZE];
         demonic[DEFAULT::CONVOY_SIZE];
         angelic[DEFAULT::CONVOY_SIZE];
         shield[DEFAULT::CONVOY_SIZE];
         staff[DEFAULT::CONVOY_SIZE];
         claw[DEFAULT::CONVOY_SIZE];
         item[DEFAULT::CONVOY_SIZE];

    public:
        Convoy();

        void write(const char * filename, const char * mode = "a");
        void writeXML(const char * filename, const bool append = false);
        void readXML(const char * filename);
};

#endif /* CONVOY_HPP */