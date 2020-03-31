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
        Quantity quantity;
        bool full;

        Inventory_item all[DEFAULT::CONVOY_SIZE], swords[DEFAULT::CONVOY_SIZE],
                       lances[DEFAULT::CONVOY_SIZE], axes[DEFAULT::CONVOY_SIZE],
                       bows[DEFAULT::CONVOY_SIZE], trinkets[DEFAULT::CONVOY_SIZE],
                       offhands[DEFAULT::CONVOY_SIZE], elemental[DEFAULT::CONVOY_SIZE],
                       demonic[DEFAULT::CONVOY_SIZE], angelic[DEFAULT::CONVOY_SIZE],
                       shields[DEFAULT::CONVOY_SIZE], staffs[DEFAULT::CONVOY_SIZE],
                       claws[DEFAULT::CONVOY_SIZE], items[DEFAULT::CONVOY_SIZE];
    public:
        Convoy();

        void deposit(Inventory_item in_item);
        Inventory_item withdraw(int index, int type);
        void sort();
        void exch(int ind1, int ind2, Inventory_item * in_item);
        void exchall(int ind1, int ind2);
        void earn(int in_money);
        void spend(int out_money);

        void write(const char * filename, const char * mode = "a");
        void writeXML(const char * filename, const bool append = false);
        void readXML(const char * filename);
};

#endif /* CONVOY_HPP */