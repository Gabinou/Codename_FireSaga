

#define CURRENT_FILENAME test_scene
REGISTER_SCENE_START(CURRENT_FILENAME)
#ifdef SCRIPT_IMPORT
#include REGISTER_ENUM(CURRENT_FILENAME)
#else
REGISTER_ENUM(CURRENT_FILENAME)
#endif /* SCRIPT_IMPORT */
REGISTER_SCENE_END(CURRENT_FILENAME)
#undef CURRENT_FILENAME


// #define CURRENT_FILENAME Support_Erwin_Kiara_C
// #ifdef SCRIPT_IMPORT
// #include REGISTER_ENUM(CURRENT_FILENAME)
// #else
// REGISTER_ENUM(CURRENT_FILENAME)
// #endif /* SCRIPT_IMPORT */
// #undef CURRENT_FILENAME

