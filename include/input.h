#ifndef INPUT_H
#define INPUT_H

#include "enums.h"
#include "parg.h"
#include "structs.h"
#include "utilities.h"

/* Note: no argp on Windows.                                */
/*  Nuance: MSYS2 has libargp, but NOT ported to mingw64.   */
extern struct parg_state parg_state_default;

struct Input_Arguments Input_parseInputs(int argc, char *argv[]);

#endif /* INPUT_H */
