#ifndef SHARED_HPP
#define SHARED_HPP

#include "enums.hpp"
#include "weapon.hpp"
#include "unit.hpp"
#include <string>
#include <vector>

#define LEN(arr) ((unsigned int) (sizeof (arr) / sizeof (arr)[0]))

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

extern int geometricslide(int distance, float geo_factor = 2);
extern int pingpong(int current, int upper, int lower = 0);

extern std::string sexName(bool in_sex);
extern unsigned char mvtType(unsigned char in_class_index);
extern std::string className(unsigned char in_class_index);
extern std::vector<std::string> skillNames(unsigned long long int in_skillscode);

extern std::vector<std::string> wpnEffects(long unsigned int in_effect);

extern short unsigned int makeEquippable(unsigned char in_class_index);
extern std::string unitName(unsigned short int in_id);
extern std::string statName(unsigned short int in_id);
extern std::string wpnName(unsigned short int in_id);
extern std::string wpnType(short unsigned int in_type);
extern std::vector<std::string> unitType(unsigned short int in_typecode);
extern std::vector<std::string> wpnTypes(short unsigned int in_typecode);


// wpnType() COULD JUST BE A VECTOR.
// ->statName also.
// ->unitName also.
// ->sexName also.
// NO NEED TO RUN CODE EVERYTIME. JUST USE A VECTOR.
// cannot be used for wpnTypes()

extern std::vector<std::string> unitNames;
extern void makeunitNames();
extern std::vector<std::string> statNames;
extern void makestatNames();
extern std::vector<std::string> sexNames;
extern void makesexNames();
extern std::vector<std::string> classNames;
extern void makeclassNames();
extern std::vector<unsigned char> mvtTypes;
extern void makemvtTypes();

extern void printarr(int arr[], int size);
extern void printvec(std::vector<int> vec);
extern int * cuniques(int arr[], int size);
extern int * cwhere(int tofind, int arr[], int size);
extern std::vector<int> cpprange(int ind1, int ind2);
extern std::vector<int> cppwhere(int tofind, std::vector<int> vec);
extern std::vector<int> cppuniques(std::vector<int> vec);

// extern void heal(Unit * in_unit, unsigned char heal);
// extern * (Unit) makeheal(Weapon in_weapon);

#endif /* SHARED_HPP */