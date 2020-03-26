#ifndef SHARED_HPP
#define SHARED_HPP

#include "enums.hpp"
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
extern std::vector<std::string> skillNames(unsigned long long int in_skills);
extern short unsigned int makeEquippable(unsigned char in_class_index);
extern std::string unitName(unsigned short int in_id);

#endif /* SHARED_HPP */