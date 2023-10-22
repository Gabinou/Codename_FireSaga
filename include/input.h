#ifndef INPUT_H
#define INPUT_H

#include "enums.h"
#include "parg.h"
#include "nourstest.h"
#include "structs.h"
#include "globals.h"
#include "utilities.h"

/* Note: no argp on Windows.                                */
/*  Nuance: MSYS2 has libargp, but NOT ported to mingw64.   */

extern struct Input_Arguments {
    uf16  cutScene;         /* plays the scene then exits */
    uf16  talkScene;
    u8   map_index;
    u8   startup_mode;
    bool  print_help;       /* If help requested anywhere, print help and exit  */
    char *save_filename;    /* debug saves are outside normal save integers     */
} Input_Arguments_default;

extern struct Input_Arguments Input_parseInputs(int argc, char *argv[]);

#endif /* INPUT_H */
