#ifndef HASHES_H
#define HASHES_H

#include "enums.h"
#include "debug.h"
#include "supports.h"
#include "nstr.h"
#include "tnecs.h"
#include "nmath.h"
#include "types.h"
#include "SDL.h"

/* -- Hashes -- */
extern u64 *unitHashes;
extern u64 *itemHashes;
extern u64 *supportHashes;

/* -- Makers -- */
extern void Hashes_unitHashes();
extern void Hashes_supportHashes();
extern void Hashes_itemHashes();

/* --- Hashing --- */
extern u16 Hashes_unitHash2ID(u64 hash);
extern u16 Hashes_unitName2ID(char *name);
extern u16 Hashes_itemHash2ID(u64 hash);
extern u16 Hashes_itemName2ID(char *name);
extern u16 Hashes_shopkeeperName2ID(char *name);
extern u16 Hashes_shopkeeperHash2ID(u64 hash);
extern u16 Hashes_skillName2ID(char *name);
extern u16 Hashes_skillHash2ID(u64 hash);
extern u16 Hashes_supportName2ID(char *name);
extern u16 Hashes_supportHash2ID(u64 hash);

/* -- API -- */
extern void Hashes_Free();

#endif /* HASHES_H */
