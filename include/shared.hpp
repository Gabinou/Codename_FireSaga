#ifndef SHARED_HPP
#define SHARED_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <memory>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "linalg.hpp"
#include "enums.hpp"
#include "structs.hpp"

#define LEN(arr) ((unsigned int) (sizeof (arr) / sizeof (arr)[0]))

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

extern int geometricslide(int distance, float geo_factor = 2);
extern int pingpong(int current, int upper, int lower = 0);

extern std::vector<std::string> wpntype2str(short unsigned int in_type);

#endif /* SHARED_HPP */