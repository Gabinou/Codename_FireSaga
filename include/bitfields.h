#ifndef BITFIELDS_H
#define BITFIELDS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "types.h"
#include "SDL.h"

/* BITFIELDS */
/* Array of bits */
enum BITFIELD {BITFIELD_BITSPERLEN = 32};

#define BITFIELD_LEN(bits) ((bits)/BITFIELD_BITSPERLEN + 1 * ((bits % BITFIELD_BITSPERLEN) > 0))
#define BITFIELD_INIT_BITS(bits) BITFIELD_INIT_BITS_(bits)
#define BITFIELD_INIT_BITS_(bits) (calloc((BITFIELD_LEN(bits)), sizeof(u32)))
#define BITFIELD_INIT_LEN(len) (calloc(len, sizeof(u32)))
#define BITFIELD_SDL_free(bitfield) SDL_free(bitfield)

/* -- Setters -- */
// For nth bit in bitfield
extern void Bitfield_On( u32 *bf, size_t bit);
extern void Bitfield_Off(u32 *bf, size_t bit);

/* -- Boolean Operations -- */
extern void Bitfield_And( u32 *bf1, u32 *bf2, u32 *out, size_t len);
extern bool Bitfield_All( u32 *bf1, u32 *bf2, size_t len);
extern bool Bitfield_isIn(u32 *bf1, u32 *bf2, size_t len);

#endif /* BITFIELDS_H */
