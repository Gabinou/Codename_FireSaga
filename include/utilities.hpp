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

extern short unsigned int makeEquippable(unsigned char in_class_index);
extern std::vector<std::string> skillNames(unsigned long long int in_skillscode);
extern std::vector<std::string> wpnEffects(long unsigned int in_effect);
extern std::vector<std::string> unitTypes(unsigned short int in_typecode);
extern std::vector<std::string> wpnTypes(short unsigned int in_typecode);

extern std::vector<std::string> unitNames;
extern void makeunitNames();
extern std::vector<std::string> wpnNames;
extern void makewpnNames();
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