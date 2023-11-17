
#include "bitfields.h"

/* -- Setters -- */
// For nth bit in bitfield
void Bitfield_On(u32 *bf, size_t bit) {
    size_t len = bit / BITFIELD_BITSPERLEN;
    bf[len] |= (1UL << (bit - (len * BITFIELD_BITSPERLEN)));
}

void Bitfield_Off(u32 *bf, size_t bit) {
    size_t len = bit / BITFIELD_BITSPERLEN;
    bf[len] &= ~(1UL << (bit - (len * BITFIELD_BITSPERLEN)));
    bf[len] &= ~(1UL << (bit - (len * BITFIELD_BITSPERLEN)));
}

/* -- Boolean Operations -- */
void Bitfield_And(u32 *bf1, u32 *bf2, u32 *out, size_t len) {
    for (u32 i = 0; i < len; i++)
        out[i] = bf1[i] & bf2[i];
}

b32 Bitfield_All(u32 *bf1, u32 *bf2, size_t len) {
    b32 out = true;
    for (u32 i = 0; i < len; i++)
        out &= (bf1[i] == bf2[i]);
    return (out);
}

// Combination Bitfield_And and Bitfield_All
// is bf1 in bf2 -> all bits set in bf1 are set in bf2
b32 Bitfield_isIn(u32 *bf1, u32 *bf2, size_t len) {
    u32 *anded = BITFIELD_INIT_LEN(len);
    Bitfield_And(bf1, bf2, anded, len);
    b32 out = Bitfield_All(bf1, anded, len);
    BITFIELD_Free(anded);
    return (out);
}

