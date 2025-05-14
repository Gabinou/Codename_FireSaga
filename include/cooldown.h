#ifndef COOLDOWN_H
#define COOLDOWN_H

#include "struct.h"

/* --- FORWARD DECLARATIONS --- */


void Cooldown_Tick(struct Cooldown *cooldown);
void Cooldown_Start(struct Cooldown *cooldown);

#endif /* COOLDOWN_H */
