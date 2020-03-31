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
        unsigned int bank = 0;
        unsigned char quantity = 0;

        Inventory_item convoy[DEFAULT::CONVOY_SIZE], sword[DEFAULT::CONVOY_SIZE],
                       lance[DEFAULT::CONVOY_SIZE], axe[DEFAULT::CONVOY_SIZE],
                       bow[DEFAULT::CONVOY_SIZE], trinket[DEFAULT::CONVOY_SIZE],
                       offhand[DEFAULT::CONVOY_SIZE], elemental[DEFAULT::CONVOY_SIZE],
                       demonic[DEFAULT::CONVOY_SIZE], angelic[DEFAULT::CONVOY_SIZE],
                       shield[DEFAULT::CONVOY_SIZE], staff[DEFAULT::CONVOY_SIZE],
                       claw[DEFAULT::CONVOY_SIZE], item[DEFAULT::CONVOY_SIZE];

    public:
        Convoy();

        void deposit(Inventory_item in_item);
        void withdraw(int index);
        void earn(int in_money);
        void spend(int out_money);

        void write(const char * filename, const char * mode = "a");
        void writeXML(const char * filename, const bool append = false);
        void readXML(const char * filename);
};

#endif /* CONVOY_HPP */