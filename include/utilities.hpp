#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "enums.hpp"
#include "structs.hpp"
#include "item.hpp"
#include "weapon.hpp"
#include "unit.hpp"
#include <string>
#include <vector>
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif /* STB_SPRINTF_IMPLEMENTATION */


#define LEN(arr) ((unsigned int) (sizeof (arr) / sizeof (arr)[0]))

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

extern int geometricslide(int distance, float geo_factor = 2);
extern int pingpong(int current, int upper, int lower = 0);
extern bool fequal(const char * filename1, const char * filename2);

extern std::vector<std::string> skillNames(unsigned long long int in_skillscode);
extern std::vector<std::string> wpnEffects(long unsigned int in_effect);
extern std::vector<std::string> unitTypes(unsigned short int in_typecode);
extern std::vector<std::string> wpnTypes(short unsigned int in_typecode);
extern unsigned char unitid2army(short unsigned int in_unitid);
extern std::string stats2str(Unit_stats in_stats);
extern std::string stats2str(Weapon_stats in_stats);
extern std::string gamestate2str(short unsigned int in_state);
extern std::vector<std::string> menuoptions2str(std::vector<unsigned char> in_options);

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

extern bool isFriendly(const unsigned char army1, const unsigned char army2);
extern bool isPC(const unsigned char army);

extern void loadUtilities();

template <typename T>
extern std::vector<T> cpprange(T ind1, T ind2) {
    std::vector<T> out;

    for (T i = ind1; i < ind2; i++) {
        out.push_back(i);
    }

    return (out);
}
template <typename T> extern std::vector<T> cppwhere(T tofind, std::vector<T> vec) {
    std::vector<T> found_inds;

    for (short unsigned int i = 0; i < vec.size(); i++) {
        if (vec[i] == tofind) {
            found_inds.push_back(i);
        }
    }

    return (found_inds);
}

template <typename T> extern bool cppisin(T tofind, std::vector<T> vec) {
    bool out = false;

    for (short unsigned int i = 0; i < vec.size(); i++) {
        if (vec[i] == tofind) {
            out = true;
        }
    }

    return (out);
}

template <typename T> extern std::vector<T> cppuniques(std::vector<T> vec) {
    std::vector<T> uniques;
    std::vector<T> inuniques;
    uniques.push_back(vec[0]);

    for (T i = 1; i < vec.size(); i++) {
        inuniques = cppwhere(vec[i], uniques);

        if (inuniques.size() == 0) {
            uniques.push_back(vec[i]);
        }
    }

    return (uniques);
}

template <typename T> extern void swap(T arr[], T ind1, T ind2) {
    T buffer;
    buffer = arr[ind1];
    arr[ind1] = arr[ind2];
    arr[ind2] = buffer;
}

template <typename T> extern T partition(T arr[], T low, T high) {
    T pivot = arr[high];
    T i = low - 1;

    for (T j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr, i, j);
        }
    }

    swap(arr, T(i + 1), high);
    return (i + 1);
}

template <typename T> extern void quicksort(T arr[], T low, T high) {
    T pi;

    if (low < high) {
        pi = partition(arr, low, high);
        quicksort(arr, low, T(pi - 1));
        quicksort(arr, T(pi + 1), high);
    }
}

template <typename T> extern void cppswap(std::vector<T> & vec, T ind1, T ind2) {
    T buffer;
    buffer = vec[ind1];
    vec[ind1] = vec[ind2];
    vec[ind2] = buffer;
}

template <typename T> extern T cppartition(std::vector<T> & vec, T low, T high) {
    T pivot = vec[high];
    T i = low - 1;

    for (T j = low; j < high; j++) {
        if (vec[j] < pivot) {
            i++;
            cppswap(vec, i, j);
        }
    }

    cppswap(vec, T(i + 1), high);
    return (T(i + 1));
}

template <typename T> extern void cppquicksort(std::vector<T> & vec, T low, T high) {
    T pi;

    if (low < high) {
        pi = cppartition(vec, low, high);
        cppquicksort(vec, low, T(pi - 1));
        cppquicksort(vec, T(pi + 1), high);
    }
}
// extern void heal(Unit * in_unit, unsigned char heal);
// extern * (Unit) makeheal(Weapon in_weapon);

#endif /* UTILITIES_HPP */