#ifndef UNIT_CLASS_HPP
#define UNIT_CLASS_HPP
using namespace std;
#include "shared.hpp"
#include "generic.hpp"
#include "inventory_item.hpp"
/*! \file unit_class.hpp
* \brief Unit_class class: For all unit classes.
*/
class unit_class: public generic {
    // Lightweight implementation. Unit should not inherit from this class. I think it should cont the name of the unit_class instead, as a human-readable pointer of sorts to the class.
public:
    bool mounted, flying, armored, promoted, dragon, usable_wpn[10];
    /// \var bool mounted
    /// \brief 1/0 mounted. For effective weapons
    /// \var bool flying
    /// \brief 1/0 flying. For effective weapons and movement    
    /// \var bool armored
    /// \brief 1/0 unit amored. For effective weapons
    /// \var bool promoted
    /// \brief 1/0 unit promoted. For exp on enemy kill.
    /// \var bool promoted
    /// \brief 1/0 unit is a dragon (or is mounted on one). For effective weapons.
    /// \var bool usable_wpn
    /// \brief Which weapon can be used. 
    // Should it be a list string instead? 
    // Pros: Human readable.
    // Cons: Slower, heavier.
    
    unsigned char stats_caps[11];
    
    unit_class(std::string, bool, bool, bool, bool, bool, std::vector<unsigned int>);
    ~unit_class();
    unit_class();
    void read(const char*, char);
    /// \fn read
    /// \brief read txt file, with format of write function.
    void write(string);
};

/*! \var all_unit_classes
* \brief Contains all instances of unit classess.
*  Uses the unit classe's name as std::unordered_map's key.
*/

extern std::unordered_map<string, unit_class> all_unit_classes;

#endif /* UNIT_CLASS_HPP */