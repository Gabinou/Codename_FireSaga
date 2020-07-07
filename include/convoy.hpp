#ifndef CONVOY_HPP
#define CONVOY_HPP

#include "filesystem.hpp"
#include "enums.hpp"
#include "structs.hpp"
#include "physfs/physfs.h"
#include "tinyxml2.h"
#include "utilities.hpp"
#include "item.hpp"
#include "weapon.hpp"
#include "string.h"
#include <string>
#include <algorithm>

class Convoy : public XML_IO, JSON_IO {
private:
    uint16_t bank = 0;
    uint8_t booksnum = 0;
    Quantity quantity;
    bool full = false;
    std::unordered_map<int16_t, Weapon> * weapons = new std::unordered_map<int16_t, Weapon>;

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
    Inventory_item withdraw(int16_t index, int16_t type);
    void earn(int16_t in_money);
    void spend(int16_t out_money);

    void quicksort(int16_t arr[], int16_t low, int16_t high, int16_t wpntype);
    int16_t partition(int16_t arr[], int16_t low, int16_t high, int16_t wpntype);
    void swap(int16_t arr[], int16_t ind1, int16_t ind2);
    void swapWpn(int16_t wpntype, int16_t ind1, int16_t ind2);

    void sortStats(int16_t wpntype, int16_t stattype);
    void sortused(int16_t wpntype);
    void sort(int16_t wpntype, int16_t stattype);

    bool isFull();
    int16_t getQuantity(int16_t wpntype);
    Quantity getQuantity();
    std::vector<int16_t> getStats(int16_t wpntype, int16_t stattype);
    Inventory_item * getItems(int16_t wpntype);
    void setItems(int16_t wpntype, Inventory_item * in_items);
    void setWeapons(std::unordered_map<int16_t, Weapon> * in_weapons);
    void checkWeapon(int16_t in_id);

    void printContents(int16_t wpntype); // Useless?
    void printStats(int16_t wpntype, int16_t stattype);

    void clear();

    using JSON_IO::writeJSON;
    using JSON_IO::readJSON;
    void readJSON(cJSON * in_json);
    void writeJSON(cJSON * in_json);

    using XML_IO::writeXML;
    using XML_IO::readXML;
    void writeXML(tinyxml2::XMLDocument * in_doc, tinyxml2::XMLElement * in_pConvoy);
    void readXML(tinyxml2::XMLElement * in_pConvoy);
};

#endif /* CONVOY_HPP */