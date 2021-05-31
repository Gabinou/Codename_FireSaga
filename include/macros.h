#ifndef MACROS_H
#define MACROS_H

#include <stdbool.h>
#include <stdint.h>

#define STRINGIFY(x) #x
#define STRINGIZE STRINGIFY
#define CONCAT(x,y) x##y
#define EXPAND(x) x

#undef SDL_ASSERT_LEVEL
#ifdef __FIRESAGA_RELEASE__
// #define SDL_Log(...) (void)0
#define SDL_Log(...) do {SDL_Log(__VA_ARGS__);}while(0)
#elif defined(__FIRESAGA_DEBUG__)
#define SDL_Log(...) do {SDL_Log(__VA_ARGS__);}while(0)
#endif

#endif /* MACROS_H */
