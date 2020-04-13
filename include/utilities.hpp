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

extern std::vector<std::string> skillNames(unsigned long long int in_skillscode);
extern std::vector<std::string> wpnEffects(long unsigned int in_effect);
extern std::vector<std::string> unitTypes(unsigned short int in_typecode);
extern std::vector<std::string> wpnTypes(short unsigned int in_typecode);
extern unsigned char unitid2army(short unsigned int in_unitid);

extern std::vector<std::string> getTilenames(std::vector<short int> in_tilesindex);
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
extern std::vector<short unsigned int> equippableCodes;
extern void makeEquippableCodes();
extern std::vector<std::string> armyNames;
extern void makeArmyNames();


extern void loadUtilities();

extern void printarr(int arr[], int size);
extern void printvec(std::vector<int> vec);

template <typename T>
extern std::vector<T> cpprange(T ind1, T ind2) {
    std::vector<T> out;

    for (int i = ind1; i < ind2; i++) {
        out.push_back(i);
    }

    return (out);
}
extern std::vector<int> cppwhere(int tofind, std::vector<int> vec);
extern std::vector<int> cppuniques(std::vector<int> vec);

// extern void heal(Unit * in_unit, unsigned char heal);
// extern * (Unit) makeheal(Weapon in_weapon);

#endif /* SHARED_HPP */