#ifndef INVENTORY_ITEM_HPP
#define INVENTORY_ITEM_HPP
#include "shared.hpp"

/*! \file inventory_item.hpp
* \brief Representation of items in unit inventories.
*/

struct inventory_item {
    /// \var char name
    /// \brief name of weapon in inventory.
    char name[30];
    /// \var used
    /// \brief number of times item was used.
    char used;
    /// \fn inventory_item(const inventory_item&)
    /// \brief constructor of inventory_item.
    inventory_item(const inventory_item &);
    /// \fn inventory_item(std::string, char)
    /// \brief constructor of inventory_item.
    inventory_item(std::string, char);
    /// \fn inventory_item()
    /// \brief constructor of inventory_item.
    inventory_item();
    ~inventory_item();
};

/*! \var inventory_items
* \brief Contains all instances of inventory_items
*  inventory items are the represntation of the weapon
*  in the equipment of units. Made to be lighter than
*  actually copying the weapon.
*/
// I think a unordered map inventory_items should exist for every save.
extern std::unordered_map<string, struct inventory_item> inventory_items;

#endif /* INVENTORY_ITEM_HPP */