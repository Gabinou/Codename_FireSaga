
#include "bitfields.h"

/* -- Setters -- */
// For nth bit in bitfield
void Bitfield_On(u32 *bf, size_t bit) {
    SDL_assert(bf != NULL);
    size_t len = bit / BITFIELD_BITSPERLEN;
    u32 val  = (1UL << (bit - (len * BITFIELD_BITSPERLEN)));
    bf[len] |= val;
}

void Bitfield_Off(u32 *bf, size_t bit) {
    SDL_assert(bf != NULL);
    size_t len = bit / BITFIELD_BITSPERLEN;
    u32 val = ~(1UL << (bit - (len * BITFIELD_BITSPERLEN)));
    bf[len] &= val;
}

b32 Bitfield_Get(u32 *bf, size_t bit) {
    SDL_assert(bf != NULL);
    SDL_Log("bit %d", bit);
    size_t len = bit / BITFIELD_BITSPERLEN;
    SDL_Log("len %d", len);
    u32 val = (1UL << (bit % BITFIELD_BITSPERLEN));
    SDL_Log("val %lu", val);
    SDL_Log("bf[len] %lu", bf[len]);
    b32 out = (bf[len] & val) > 0;
    SDL_Log("out %d", out);
    return (out);
}

/* -- Boolean Operations -- */
void Bitfield_And(u32 *bf1, u32 *bf2, u32 *out, size_t len) {
    SDL_assert(bf1 != NULL);
    SDL_assert(bf2 != NULL);
    for (u32 i = 0; i < len; i++)
        out[i] = bf1[i] & bf2[i];
}

b32 Bitfield_All(u32 *bf1, u32 *bf2, size_t len) {
    SDL_assert(bf1 != NULL);
    SDL_assert(bf2 != NULL);
    b32 out = true;
    for (u32 i = 0; i < len; i++)
        out &= (bf1[i] == bf2[i]);
    return (out);
}

b32 Bitfield_Any(u32 *bf1, u32 *bf2, size_t len) {
    SDL_assert(bf1 != NULL);
    SDL_assert(bf2 != NULL);
    for (u32 i = 0; i < len; i++) {
        if ((bf1[i] & bf2[i]) > 0) {
            return (true);
        }
    }
    return (false);
}

// Combination Bitfield_And and Bitfield_All
// is bf1 in bf2 -> all bits set in bf1 are set in bf2
b32 Bitfield_isIn(u32 *bf1, u32 *bf2, size_t len) {
    SDL_assert(bf1 != NULL);
    SDL_assert(bf2 != NULL);
    u32 *anded = BITFIELD_INIT_LEN(len);
    Bitfield_And(bf1, bf2, anded, len);
    b32 out = Bitfield_All(bf1, anded, len);
    BITFIELD_Free(anded);
    return (out);
}

