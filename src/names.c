#include "names.h"
#include "stb_sprintf.h"

#define REGISTER_ENUM(x) temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
    memcpy(temp_str, #x, sizeof(#x));\
    sceneTimes[SCENE_TIME_##x] = nstr_camelCase(nstr_toLower(temp_str), '_', 2);\
    strcat(sceneTimes[SCENE_TIME_##x], "_");
char **sceneTimes;
void Names_sceneTimes() {
    sceneTimes = calloc(SCENE_TIME_NUM, sizeof(*sceneTimes));
    char *temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);
    memcpy(temp_str, "", sizeof(""));
    sceneTimes[0] = temp_str;
#include "names/scene_time.h"
}
#undef REGISTER_ENUM

char **global_unitNames = NULL;
struct dtab *global_unitOrders = NULL;
void Names_unitNames() {
    char *temp_str = NULL;
    size_t order = 0;

    global_unitNames = DARR_INIT(global_unitNames, char *, 128);
    SDL_assert(global_unitNames != NULL);

    DTAB_INIT(global_unitOrders, u16);
    SDL_assert(global_unitOrders != NULL);
    dtab_add(global_unitOrders, &order, UNIT_NULL);
    order++;

    temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);
    memcpy(temp_str, "", sizeof(""));
    DARR_PUT(global_unitNames, temp_str);

#define REGISTER_ENUM(x, y) temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
    memcpy(temp_str, #x, sizeof(#x));\
    SDL_Log("%d", UNIT_ID_##x);\
    SDL_Log("%s", #x);\
    dtab_add(global_unitOrders, &order, UNIT_ID_##x);\
    SDL_assert(*(u16 *)dtab_get(global_unitOrders, UNIT_ID_##x) == order);\
    order++;\
    DARR_PUT(global_unitNames, nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')), ' ', 2));
#include "names/units_PC.h"
#include "names/units_NPC.h"
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x)  temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
    memcpy(temp_str, #x, sizeof(#x));\
    statNames[ITEM_STAT_##x] = nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')), ' ', 2);

char **statNames = NULL;
void Names_statNames() {
    char *temp_str = NULL;
    statNames = calloc(ITEM_STAT_END, sizeof(*statNames));
    temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);
    memcpy(temp_str, "", sizeof(""));
    statNames[0] = temp_str;
#include "names/items_stats.h"
}

#undef REGISTER_ENUM
char **sexNames = NULL;
void Names_sexNames() {
    sexNames = (char **)calloc(UNIT_SEX_NUM, sizeof(*sexNames));
    sexNames[UNIT_SEX_F] = (char *) SDL_malloc(2);
    memcpy(sexNames[UNIT_SEX_F], "F\0", 2);
    sexNames[UNIT_SEX_M] = (char *) SDL_malloc(2);
    memcpy(sexNames[UNIT_SEX_M], "M\0", 2);
}

#define REGISTER_ENUM(x) temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
    memcpy(temp_str, #x, sizeof(#x));\
    armyNames[ARMY_##x] = nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')),' ', 2);
char **armyNames = NULL;
void Names_armyNames() {
    char *temp_str = NULL;
    armyNames = calloc(ARMY_END, sizeof(*armyNames));
#include "names/armies.h"
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
    memcpy(temp_str, #x, sizeof(#x));\
    unitStates[UNIT_STATUS_##x] =  nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')), ' ', 2);

char **unitStates = NULL;
void Names_unitStates() {
    char *temp_str = NULL;
    unitStates = calloc(UNIT_STATUS_END, sizeof(*unitStates));
#include "names/units_statuses.h"
}
#undef REGISTER_ENUM

char **global_itemNames         = NULL;
struct dtab *global_itemOrders  = NULL;
void Names_itemNames() {
    size_t order = 0;

    global_itemNames = DARR_INIT(global_itemNames, char *, 128);
    DARR_PUT(global_itemNames, ITEM_NAME_INVALID);

    DTAB_INIT(global_itemOrders, i32);
    SDL_assert(global_itemOrders != NULL);
    // SDL_assert(order == global_itemOrders->num);
    dtab_add(global_itemOrders, &order, ITEM_NULL);
    order++;

    char *temp_str = NULL;
#define REGISTER_ENUM(x, y) temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
    memcpy(temp_str, #x, sizeof(#x));\
    dtab_add(global_itemOrders, &order, ITEM_ID_##x);\
    order++;\
    DARR_PUT(global_itemNames, nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')), ' ', 2));
#include "names/items.h"
    SDL_assert(order == DARR_NUM(global_itemNames));
}
#undef REGISTER_ENUM

char **support_types = NULL;
void Names_supportTypes() {
    support_types = SDL_malloc(TILE_ID_MAX * sizeof(global_tilenames));
}

char **global_tilenames = NULL;
i32 *global_tilesID = NULL;
void Names_tileNames() {
    global_tilenames = SDL_malloc(TILE_ID_MAX * sizeof(global_tilenames));
    char *temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);
    memcpy(temp_str, "NULL", sizeof("NULL"));
    global_tilenames[0] = temp_str;
    uint64_t temp_id = 1; /* 0 is reserved for NULL*/
    // size_t num_tile = 0;
#define REGISTER_ENUM(x) temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
    memcpy(temp_str, #x, sizeof(#x));\
    global_tilenames[temp_id++] = nstr_camelCase(nstr_toLower(temp_str), ' ', 2);
    // temp_id++;
#include "names/tiles.h"
    // SDL_free(temp_str);
    // hmdefault(global_tilenames, "");
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
    memcpy(temp_str, #x, sizeof(#x));\
    campjobNames[CAMPJOB_##x] = nstr_camelCase(nstr_toLower(temp_str), ' ', 2);

char **campjobNames = NULL;
void Names_campjobNames() {
    campjobNames = calloc(CAMPJOB_END, sizeof(*campjobNames));
    char *temp_str = NULL;
#include "names/camp_jobs.h"
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
    memcpy(temp_str, #x, sizeof(#x));\
    menuOptionnames[MENU_OPTION_##x] = nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')), ' ', 2);
char **menuOptionnames = NULL;
void Menu_MakeOptionnames() {
    char *temp_str = NULL;
    menuOptionnames = calloc(MENU_OPTION_END, sizeof(*menuOptionnames));
#include "names/menu/options.h"
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
    memcpy(temp_str, #x, sizeof(#x));\
    gamesubStatenames[GAME_SUBSTATE_##x] = nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')), ' ', 2);

char **gamesubStatenames = NULL;
void Names_gamesubStatenames() {
    char *temp_str = NULL;
    gamesubStatenames = calloc(GAME_SUBSTATE_END, sizeof(*gamesubStatenames));
#include "names/game_substates.h"
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
    memcpy(temp_str, #x, sizeof(#x));\
    gameStatenames[GAME_STATE_##x] = nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')), ' ', 2);

char **gameStatenames = NULL;
void Names_gameStatenames() {
    char *temp_str = NULL;
    gameStatenames = calloc(GAME_STATE_END, sizeof(*gameStatenames));
#include "names/game_states.h"
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y, z) temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
    memcpy(temp_str, "\0", 1);\
    strcat(temp_str, "assets"PHYSFS_SEPARATOR"Maps"PHYSFS_SEPARATOR);\
    strcat(temp_str, #y);\
    mapFilenames[CHAPTER_##x] =  temp_str;

char **mapFilenames = NULL;
void Names_mapFilenames() {
    char *temp_str = NULL;
    mapFilenames = calloc(CHAPTER_END, sizeof(*mapFilenames));
#include "names/chapters.h"
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(name, id, x, y, z) temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
    memcpy(temp_str, #name, sizeof(#name));\
    classNames[UNIT_CLASS_##name] = nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')),' ', 2);

char **classNames =  NULL;
void Names_classNames() {
    char *temp_str = NULL;
    classNames = calloc(UNIT_CLASS_END, sizeof(*classNames));
#include "names/classes.h"
}
#undef  REGISTER_ENUM

#define REGISTER_ENUM(x) if flagsum_isIn(in_skillscode, UNIT_SKILL_##x) {\
        temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
        memcpy(temp_str, #x, sizeof(#x));\
        DARR_PUT(skill_names, nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')),' ', 2));\
    }
char **Names_skillNames(uint64_t in_skillscode) {
    char **skill_names = DARR_INIT(skill_names, char *, UNIT_SKILL_END);
    char *temp_str = NULL;
#include "names/skills_passive.h"
#include "names/skills_active.h"
    return (skill_names);
}
#undef  REGISTER_ENUM

#define REGISTER_ENUM(x) if flagsum_isIn(in_statecode, UNIT_STATUS_##x) {\
        temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
        memcpy(temp_str, #x, sizeof(#x));\
        DARR_PUT(state_names, nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')),' ', 2));\
    }
char **Names_unitstateNames(uint32_t in_statecode) {
    char *temp_str = NULL;
    char **state_names = DARR_INIT(state_names, char *, UNIT_STATUS_END);
#include "names/units_statuses.h"
    return (state_names);
}
#undef  REGISTER_ENUM

#define REGISTER_ENUM(name, id, x, y, z) class_equippables[UNIT_CLASS_##name] = z;
u16 *class_equippables;
void Names_class_equippables() {
    class_equippables = DARR_INIT(class_equippables, u16, UNIT_CLASS_END);
    // DARR_PUT(class_equippables, 0);
#include "names/classes.h"
}
#undef  REGISTER_ENUM

#define REGISTER_ENUM(x) if flagsum_isIn(in_typecode, UNIT_TYPE_##x) {\
        temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
        memcpy(temp_str, #x, sizeof(#x));\
        DARR_PUT(out, nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')), ' ', 2));\
    }
char **Names_unitTypes(u16 in_typecode) {
    char *temp_str = NULL;
    char **out = DARR_INIT(out, char *, UNIT_TYPE_END);
#include "names/units_types.h"
    return (out);
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) if flagsum_isIn(in_effect, ITEM_EFFECT_##x) {\
        temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
        memcpy(temp_str, #x, sizeof(#x));\
        DARR_PUT(wpn_effects, nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')), ' ', 2));\
    }
char **Names_wpnEffects(u64 in_effect) {
    char *temp_str = NULL;
    char **wpn_effects = DARR_INIT(wpn_effects, char *, UNIT_TYPE_END);
#include "names/items_effects.h"
    return (wpn_effects);
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x)  temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
    memcpy(temp_str, #x, sizeof(#x));\
    jsonElementnames[JSON_##x] = nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')),' ', 2);

char **jsonElementnames = NULL;
void Names_jsonElementnames() {
    char *temp_str = NULL;
    jsonElementnames = calloc(JSON_END, sizeof(*jsonElementnames));
#include "names/json_elements.h"
    memcpy(jsonElementnames[JSON_WEAPON], "Item\0", 5);
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) if flagsum_isIn(in_typecode, ITEM_TYPE_##x) {\
        temp_str = (char *) SDL_malloc(DEFAULT_BUFFER_SIZE);\
        memcpy(temp_str, #x, sizeof(#x));\
        DARR_PUT(type_names, nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')),' ', 2));\
    }
char **Names_wpnType(u16 in_typecode) {
    char **type_names = DARR_INIT(type_names, char *, ITEM_TYPE_END);
    char *temp_str;
#include "names/items_types.h"
    return (type_names);
}
#undef REGISTER_ENUM

void Names_wpnType_Free(char **type_names) {
    for (int i = 0; i < DARR_NUM(type_names); ++i) {
        if (type_names[i] != NULL)
            SDL_free(type_names[i]);
    }
    DARR_FREE(type_names);
}


/* --- API --- */
void Names_Load_All() {
    Names_unitNames();
    Names_sceneTimes();
    Hashes_supportHashes();
    Hashes_unitHashes();
    Hashes_itemHashes();
    Names_tileNames();
    Names_campjobNames();
    Names_statNames();
    Names_sexNames();
    Menu_MakeOptionnames();
    Names_unitStates();
    Names_jsonElementnames();
    Names_gameStatenames();
    Names_gamesubStatenames();
    Names_classNames();
    Names_itemNames();
    Names_mapFilenames();
    Names_class_equippables();
    Names_armyNames();
}

void Names_Free() {

    SDL_Log("menuOptionnames");
    if (menuOptionnames != NULL) {
        for (size_t i = MENU_OPTION_START + 1; i < MENU_OPTION_END; i++) {
            if (menuOptionnames[i] != NULL) {
                SDL_free(menuOptionnames[i]);
                menuOptionnames[i] = NULL;
            }
        }
        SDL_free(menuOptionnames);
    }

    if (sceneTimes != NULL) {
        for (size_t i = 0; i < SCENE_TIME_NUM; i++) {
            SDL_free(sceneTimes[i]);
            sceneTimes[i] = NULL;
        }
        SDL_free(sceneTimes);
        sceneTimes = NULL;
    }

    SDL_Log("gamesubStatenames");
    for (size_t i = 0; i < GAME_SUBSTATE_END; i++) {
        if (gamesubStatenames[i] != NULL) {
            SDL_free(gamesubStatenames[i]);
            gamesubStatenames[i] = NULL;
        }
    }
    if (gamesubStatenames != NULL) {
        SDL_free(gamesubStatenames);
        gamesubStatenames = NULL;
    }
    SDL_Log("gameStatenames");
    for (size_t i = 0; i < GAME_STATE_END; i++) {
        if (gameStatenames[i] != NULL) {
            SDL_free(gameStatenames[i]);
            gameStatenames[i] = NULL;
        }
    }
    if (gameStatenames != NULL) {
        SDL_free(gameStatenames);
        gameStatenames = NULL;
    }
    SDL_Log("mapFilenames");
    for (size_t i = 0; i < CHAPTER_END; i++) {
        if (mapFilenames[i] != NULL) {
            SDL_free(mapFilenames[i]);
            mapFilenames[i] = NULL;
        }
    }
    if (mapFilenames != NULL) {
        SDL_free(mapFilenames);
        mapFilenames = NULL;
    }
    SDL_Log("jsonElementnames");
    for (size_t i = 0; i < JSON_END; i++) {
        if (jsonElementnames[i] != NULL) {
            SDL_free(jsonElementnames[i]);
            jsonElementnames[i] = NULL;
        }
    }
    if (jsonElementnames != NULL) {
        SDL_free(jsonElementnames);
        jsonElementnames = NULL;
    }
    SDL_Log("armyNames");
    for (size_t i = 0; i < ARMY_END; i++) {
        if (armyNames[i] != NULL) {
            SDL_free(armyNames[i]);
            armyNames[i] = NULL;
        }
    }
    if (armyNames != NULL) {
        SDL_free(armyNames);
        armyNames = NULL;
    }
    SDL_Log("global_itemNames");
    for (size_t i = 0; i < DARR_NUM(global_itemNames); i++) {
        // Names that start with Space are invalid.
        if (global_itemNames[i][0] != ' ') {
            SDL_free(global_itemNames[i]);
            global_itemNames[i] = NULL;
        }
    }
    if (global_itemNames != NULL) {
        DARR_FREE(global_itemNames);
        global_itemNames = NULL;
    }
    if (global_itemOrders != NULL) {
        DTAB_FREE(global_itemOrders);
        global_itemOrders = NULL;
    }
    SDL_Log("global_tilenames i");
    for (size_t i = TILE_START; i < TILE_END; i++) {
        if (global_tilenames[i] != NULL) {
            SDL_free(global_tilenames[i]);
            global_tilenames[i] = NULL;
        }
    }
    SDL_Log("global_tilenames");
    if (global_tilenames  != NULL) {
        SDL_free(global_tilenames);
        global_tilenames = NULL;
    }
    SDL_Log("global_tilesID");
    if (global_tilesID  != NULL) {
        DARR_FREE(global_tilesID);
        global_tilesID = NULL;
    }
    SDL_Log("campjobNames");
    for (size_t i = 0; i < CAMPJOB_END; i++) {
        if (campjobNames[i]  != NULL) {
            SDL_free(campjobNames[i]);
            campjobNames[i] = NULL;
        }
    }
    if (campjobNames  != NULL) {
        SDL_free(campjobNames);
        campjobNames = NULL;
    }
    SDL_Log("global_unitNames");
    if (global_unitNames != NULL) {
        for (int i = 0; i < DARR_NUM(global_unitNames); i++) {
            if (global_unitNames[i] != NULL) {
                SDL_free(global_unitNames[i]);
                global_unitNames[i] = NULL;
            }
        }
        DARR_FREE(global_unitNames);
        global_unitNames = NULL;
    }
    if (global_unitOrders != NULL) {
        DTAB_FREE(global_unitOrders);
        global_unitOrders = NULL;
    }

    SDL_Log("statNames");
    for (size_t i = 0; i < ITEM_STAT_END; i++) {
        if (statNames[i] != NULL) {
            SDL_free(statNames[i]);
            statNames[i] = NULL;
        }
    }
    if (statNames != NULL) {
        SDL_free(statNames);
        statNames = NULL;
    }
    SDL_Log("sexNames");
    for (size_t i = 0; i < UNIT_SEX_NUM; i++) {
        if (sexNames[i] != NULL) {
            SDL_free(sexNames[i]);
            sexNames[i] = NULL;
        }
    }
    if (sexNames != NULL) {
        SDL_free(sexNames);
        sexNames = NULL;
    }
    SDL_Log("unitStates");
    for (size_t i = 0; i < UNIT_STATUS_END; i++) {
        if (unitStates[i] != NULL) {
            SDL_free(unitStates[i]);
            unitStates[i] = NULL;
        }
    }
    if (unitStates != NULL) {
        SDL_free(unitStates);
        unitStates = NULL;
    }
    SDL_Log("classNames");
    for (size_t i = 0; i < UNIT_CLASS_END; i++) {
        if (classNames[i] != NULL) {
            SDL_free(classNames[i]);
            classNames[i] = NULL;
        }
    }
    if (classNames != NULL) {
        SDL_free(classNames);
        classNames = NULL;
    }
    if (class_equippables != NULL) {
        DARR_FREE(class_equippables);
        class_equippables = NULL;
    }
}

void Names_Print_All(const char *foldername) {
    SDL_Log("foldername %s", foldername);
    FILE *fp = NULL;
    /* --- Army names --- */
    s8 filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_unitArmies.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = ARMY_START; i < (ARMY_END - 1); i++)
        fprintf(fp, "%d %s \n", i, armyNames[i]);
    fclose(fp);
    s8_free(&filename);

    /* --- Stat names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_unitStates.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = UNIT_STATUS_EXP_START; i < (UNIT_STATUS_EXP_END - 1); i++)
        fprintf(fp, "%d %s \n", i, unitStates[i]);
    fclose(fp);
    s8_free(&filename);

    /* --- Json element names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_jsonElementnames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = JSON_START; i < (JSON_END - 1); i++)
        fprintf(fp, "%d %s \n", i, jsonElementnames[i]);
    fclose(fp);
    s8_free(&filename);

    /* --- Camp job names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_campjobNames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = CAMPJOB_START; i < (CAMPJOB_END - 1); i++)
        fprintf(fp, "%d %s \n", i, campjobNames[i]);
    fclose(fp);
    s8_free(&filename);

    /* --- Game State names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_gameStatenames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = GAME_STATE_START; i < (GAME_STATE_END - 1); i++)
        fprintf(fp, "%d %s \n", i, gameStatenames[i]);
    fclose(fp);
    s8_free(&filename);

    /* --- Menu Option names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_menuOptionnames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = MENU_OPTION_START; i < MENU_OPTION_END; i++)
        fprintf(fp, "%d %s \n", i, menuOptionnames[i]);
    fclose(fp);
    s8_free(&filename);

    /* --- Game substate names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_gamesubStatenames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = GAME_SUBSTATE_START; i < (GAME_SUBSTATE_END - 1); i++)
        fprintf(fp, "%d %s \n", i, gamesubStatenames[i]);
    fclose(fp);
    s8_free(&filename);

    /* --- Unit names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_unitNames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (size_t i = 0; i < DARR_NUM(global_unitNames); i++)
        fprintf(fp, "%zu %s \n", i, global_unitNames[i]);
    fclose(fp);
    s8_free(&filename);

    /* --- Item names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_itemNames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (size_t i = 0; i < DARR_NUM(global_itemNames); i++)
        fprintf(fp, "%zu %s \n", i, global_itemNames[i]);
    fclose(fp);

    /* --- State names --- */
    s8_free(&filename);
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_statNames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = ITEM_STAT_START; i < (ITEM_STAT_END - 1); i++)
        fprintf(fp, "%d %s \n", i, statNames[i]);
    fclose(fp);

    /* --- Sex names --- */
    s8_free(&filename);
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("utilitiessexNames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = UNIT_SEX_F; i < (UNIT_SEX_M + 1); i++)
        fprintf(fp, "%d %s \n", i, sexNames[i]);
    fclose(fp);

    /* --- Class names --- */
    s8_free(&filename);
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_classNames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = UNIT_CLASS_START; i < UNIT_CLASS_END; i++)
        fprintf(fp, "%d %s \n", i, classNames[i]);
    fclose(fp);
    s8_free(&filename);

    /* --- Tile names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_tilenames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    SDL_assert(global_tilenames != NULL);
    s8 s8null = s8_literal("");
    for (u8 i = TILE_START; i < TILE_END; i++) {
        s8 tilename = s8_var(global_tilenames[i]);
        if (!s8equal(tilename, s8null))
            fprintf(fp, "%d %s \n", i, global_tilenames[i]);
    }
    fclose(fp);
    s8_free(&filename);
}
