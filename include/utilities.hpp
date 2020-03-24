#ifndef SHARED_HPP
#define SHARED_HPP

#include "enums.hpp"


#define LEN(arr) ((unsigned int) (sizeof (arr) / sizeof (arr)[0]))

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

extern int geometricslide(int distance, float geo_factor = 2);
extern int pingpong(int current, int upper, int lower = 0);

extern void sexName(char * name, bool in_sex);
extern unsigned char mvtType(unsigned char in_class_index);
extern std::string className(unsigned char in_class_index);

#endif /* SHARED_HPP */