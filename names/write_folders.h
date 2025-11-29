
#ifndef REGISTER_ENUM
    /* ndef REGISTER_ENUM means #include is meant
    **  to get only the defines. So a "noop" REGISTER_ENUM
    **  is defined to ignore X macro REGISTER_ENUM stuff. */
    #define REGISTER_ENUM(dir)
#endif /* REGISTER_ENUM */

#define GAME_SAVE_DIR       "saves"
#define GAME_BUILD_DIR      "build"
#define GAME_INSTALL_DIR    "install"
REGISTER_ENUM(GAME_SAVE_DIR)

/* In case REGISTER_ENUM is defined in this include, undef.
**  1. Multiple undefs -> OK
**  2. BUT need to redefine REGISTER_ENUM for every include. */
#undef REGISTER_ENUM
