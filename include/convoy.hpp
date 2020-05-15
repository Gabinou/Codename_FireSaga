#ifndef CONVOY_HPP
#define CONVOY_HPP

#include "filesystem.hpp"
#include "enums.hpp"
#include "structs.hpp"
#include "physfs.h"
#include "tinyxml2.h"
#include "utilities.hpp"
#include "item.hpp"
#include "weapon.hpp"
#include "string.h"
#include <string>
#include <algorithm>

class Convoy : public XML_IO {
private:
    unsigned int bank = 0;
    unsigned char  booksnum = 0;
    Quantity quantity;
    bool full = false;

    Inventory_item books[DEFAULT::BOOKS_NUM], swords[DEFAULT::CONVOY_SIZE],
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
    void swapWpn(int wpntype, int ind1, int ind2);

    void sortStats(int wpntype, int stattype);
    void sortused(int wpntype);
    void sort(int wpntype, int stattype);

    bool isFull();
    int getQuantity(int wpntype);
    Quantity getQuantity();
    std::vector<int> getStats(int wpntype, int stattype);
    Inventory_item * getItems(int wpntype);
    void setItems(int wpntype, Inventory_item * in_items);

    void printContents(int wpntype); // Useless?
    void printStats(int wpntype, int stattype);

    void write(const char * filename, const char * mode = "a");
    using XML_IO::writeXML;
    using XML_IO::readXML;
    void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pConvoy);
    void readXML(tinyxml2::XMLElement * in_pConvoy);
};

#endif /* CONVOY_HPP */