#ifndef CONVOY_HPP
#define CONVOY_HPP

#include "enums.hpp"
#include "structs.hpp"
#include "physfs.h"
#include "tinyxml2.h"
#include "filesystem.hpp"
#include "utilities.hpp"
#include "weapon.hpp"
#include "string.h"
#include <string>

class Convoy {
    private:
        unsigned int bank = 0;
        Quantity quantity;
        bool full = false;

        Inventory_item swords[DEFAULT::CONVOY_SIZE],
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
        void earn(int in_money);
        void spend(int out_money);

        void quicksort(int arr[], int low, int high, int wpntype);
        int partition(int arr[], int low, int high, int wpntype);
        void swap(int arr[], int ind1, int ind2);
        void swapwpn(int wpntype, int ind1, int ind2);

        void sortstats(int wpntype, int stattype);
        void sortused(int wpntype);
        void sort(int wpntype, int stattype);

        void isFull();
        int getQuantity(int wpntype);
        Quantity getQuantity();
        int * getStats(int wpntype, int stattype);
        Inventory_item * getItems(int wpntype);

        void printcontents(int wpntype); // Useless?
        void printstats(int wpntype, int stattype);

        void write(const char * filename, const char * mode = "a");
        void writeXML(const char * filename, const bool append = false);
        void readXML(const char * filename);
};

extern void testConvoy();
extern void testConvoyfull();
extern void testConvoysortstats();
extern void testConvoysortused();

#endif /* CONVOY_HPP */