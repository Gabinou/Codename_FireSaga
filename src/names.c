#include "names.h"
#include "stb_sprintf.h"

s8 sexNames[UNIT_SEX_NUM] = {
    s8_literal("F"),
    s8_literal("M")
};


s8 sceneTimes[SCENE_TIME_NUM] = {0};
void Names_sceneTimes() {
#define REGISTER_ENUM(x) sceneTimes[SCENE_TIME_##x] = s8cat(s8_camelCase(s8_toLower(s8_mut(#x)), '_', 2), s8_literal("_"));
#include "names/scene_time.h"
#undef REGISTER_ENUM
}

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
    dtab_add(global_unitOrders, &order, UNIT_ID_##x);\
    SDL_assert(*(u16 *)dtab_get(global_unitOrders, UNIT_ID_##x) == order);\
    order++;\
    DARR_PUT(global_unitNames, nstr_camelCase(nstr_toLower(nstr_replaceSingle(temp_str, '_', ' ')), ' ', 2));
#include "names/units_PC.h"
#include "names/units_NPC.h"
}
#undef REGISTER_ENUM


s8 statNames[UNIT_STAT_NUM] = {0};
void Names_statNames() {
#define REGISTER_ENUM(x) statNames[ITEM_STAT_##x] = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')), ' ', 2);
#include "names/items_stats.h"
#undef REGISTER_ENUM
}


s8 armyNames[ARMY_NUM] = {0};
void Names_armyNames() {
#define REGISTER_ENUM(x) armyNames[ARMY_##x] = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')),' ', 2);
#include "names/armies.h"
#undef REGISTER_ENUM
}


s8 unitStatuses[UNIT_STATUS_END];
void Names_unitStatuses() {
#define REGISTER_ENUM(x) unitStatuses[UNIT_STATUS_##x] =  s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')), ' ', 2);
#include "names/units_statuses.h"
#undef REGISTER_ENUM
}

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

s8 support_types[SUPPORT_TYPE_NUM] = {0};
void Names_supportTypes() {
}

char **global_tilenames = NULL;
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
#include "names/tiles.h"
}
#undef REGISTER_ENUM


s8 campjobNames[CAMPJOB_END] = {0};
void Names_campjobNames() {
#define REGISTER_ENUM(x) campjobNames[CAMPJOB_##x] = s8_camelCase(s8_toLower(s8_mut(#x)), ' ', 2);
#include "names/camp_jobs.h"
#undef REGISTER_ENUM
}


s8 menuOptionnames[MENU_OPTION_END] = {0};
void Menu_MakeOptionnames() {
#define REGISTER_ENUM(x, y) menuOptionnames[MENU_OPTION_##x] = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')), ' ', 2);
#include "names/menu/options.h"
#undef REGISTER_ENUM
}


s8 gamesubStatenames[GAME_SUBSTATE_END] = {0};
void Names_gamesubStatenames() {
#define REGISTER_ENUM(x, y) gamesubStatenames[GAME_SUBSTATE_##x] = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')), ' ', 2);
#include "names/game_substates.h"
#undef REGISTER_ENUM
}


s8 gameStatenames[GAME_STATE_END] = {0};
void Names_gameStatenames() {
#define REGISTER_ENUM(x, y) gameStatenames[GAME_STATE_##x] = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')), ' ', 2);
#include "names/game_states.h"
#undef REGISTER_ENUM
}


s8 mapFilenames[CHAPTER_END] = {0};
void Names_mapFilenames() {
#define REGISTER_ENUM(x, y, z) mapFilenames[CHAPTER_##x] = s8cat(s8_mut("assets"PHYSFS_SEPARATOR"Maps"PHYSFS_SEPARATOR), s8_literal(#y));
#include "names/chapters.h"
#undef REGISTER_ENUM
}


s8 classNames[UNIT_CLASS_NUM] = {0};
void Names_classNames() {
#define REGISTER_ENUM(name, id, x, y, z) classNames[UNIT_CLASS_##name] = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#name), '_', ' ')),' ', 2);
#include "names/classes.h"
#undef  REGISTER_ENUM
}


s8 *Names_skillNames(uint64_t in_skillscode) {
    s8 *skill_names = DARR_INIT(skill_names, s8, UNIT_SKILL_END);
#define REGISTER_ENUM(x) if flagsum_isIn(in_skillscode, UNIT_SKILL_##x) {\
        DARR_PUT(skill_names, s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')),' ', 2));\
    }
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

u16 *class_equippables = NULL;
void Names_class_equippables() {
    class_equippables = DARR_INIT(class_equippables, u16, UNIT_CLASS_END);
#define REGISTER_ENUM(name, id, x, y, z) class_equippables[UNIT_CLASS_##name] = z;
#include "names/classes.h"
#undef  REGISTER_ENUM
}

s8 *Names_unitTypes(u16 in_typecode) {
    s8 *out = DARR_INIT(out, s8, UNIT_TYPE_END);
#define REGISTER_ENUM(x) if flagsum_isIn(in_typecode, UNIT_TYPE_##x) {\
        DARR_PUT(out, s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')), ' ', 2));\
    }
#include "names/units_types.h"
#undef REGISTER_ENUM
    return (out);
}

s8 *Names_wpnEffects(u64 in_effect) {
    s8 *wpn_effects = DARR_INIT(wpn_effects, s8, UNIT_TYPE_END);
#define REGISTER_ENUM(x, y) if flagsum_isIn(in_effect, ITEM_EFFECT_##x) {\
        DARR_PUT(wpn_effects, s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')), ' ', 2));\
    }
#include "names/items_effects.h"
#undef REGISTER_ENUM
    return (wpn_effects);
}


s8 jsonElementnames[JSON_END] = {0};
void Names_jsonElementnames() {
#define REGISTER_ENUM(x) jsonElementnames[JSON_##x] = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')),' ', 2);
#include "names/json_elements.h"
#undef REGISTER_ENUM
    jsonElementnames[JSON_WEAPON] = s8_mut("Item");
}

s8 *Names_wpnType(u16 in_typecode) {
    s8 *type_names = DARR_INIT(type_names, s8, ITEM_TYPE_END);
#define REGISTER_ENUM(x) if flagsum_isIn(in_typecode, ITEM_TYPE_##x) {\
        DARR_PUT(type_names, s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')),' ', 2));\
    }
#include "names/items_types.h"
#undef REGISTER_ENUM
    return (type_names);
}

void Names_wpnType_Free(s8 *type_names) {
    for (int i = 0; i < DARR_NUM(type_names); ++i) {
        s8_free(&type_names[i]);
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
    Menu_MakeOptionnames();
    Names_unitStatuses();
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
    for (size_t i = MENU_OPTION_START + 1; i < MENU_OPTION_END; i++) {
        s8_free(&menuOptionnames[i]);
    }

    for (size_t i = 0; i < SCENE_TIME_NUM; i++) {
        s8_free(&sceneTimes[i]);
    }

    SDL_Log("gamesubStatenames");
    for (size_t i = 0; i < GAME_SUBSTATE_END; i++) {
        s8_free(&gamesubStatenames[i]);
    }
    SDL_Log("gameStatenames");
    for (size_t i = 0; i < GAME_STATE_END; i++) {
        s8_free(&gameStatenames[i]);
    }
    SDL_Log("mapFilenames");
    for (size_t i = 0; i < CHAPTER_END; i++) {
        s8_free(&mapFilenames[i]);
    }
    SDL_Log("jsonElementnames");
    for (size_t i = 0; i < JSON_END; i++) {
        s8_free(&jsonElementnames[i]);
    }
    SDL_Log("armyNames");
    for (size_t i = 0; i < ARMY_END; i++) {
        s8_free(&armyNames[i]);
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
    SDL_Log("campjobNames");
    for (size_t i = 0; i < CAMPJOB_END; i++) {
        s8_free(&campjobNames[i]);
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
        s8_free(&statNames[i]);
    }

    SDL_Log("unitStatuses");
    for (size_t i = 0; i < UNIT_STATUS_END; i++) {
        s8_free(&unitStatuses[i]);
    }

    SDL_Log("classNames");
    for (size_t i = 0; i < UNIT_CLASS_END; i++) {
        s8_free(&classNames[i]);
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
    filename = s8cat(filename, s8_literal("Utilities_unitStatuses.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = UNIT_STATUS_EXP_START; i < (UNIT_STATUS_EXP_END - 1); i++)
        fprintf(fp, "%d %s \n", i, unitStatuses[i].data);
    fclose(fp);
    s8_free(&filename);

    /* --- Json element names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_jsonElementnames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = JSON_START; i < (JSON_END - 1); i++)
        fprintf(fp, "%d %s \n", i, jsonElementnames[i].data);
    fclose(fp);
    s8_free(&filename);

    /* --- Camp job names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_campjobNames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = CAMPJOB_START; i < (CAMPJOB_END - 1); i++)
        fprintf(fp, "%d %s \n", i, campjobNames[i].data);
    fclose(fp);
    s8_free(&filename);

    /* --- Game State names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_gameStatenames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = GAME_STATE_START; i < (GAME_STATE_END - 1); i++)
        fprintf(fp, "%d %s \n", i, gameStatenames[i].data);
    fclose(fp);
    s8_free(&filename);

    /* --- Menu Option names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_menuOptionnames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = MENU_OPTION_START; i < MENU_OPTION_END; i++)
        fprintf(fp, "%d %s \n", i, menuOptionnames[i].data);
    fclose(fp);
    s8_free(&filename);

    /* --- Game substate names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_gamesubStatenames.txt"));
    SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = GAME_SUBSTATE_START; i < (GAME_SUBSTATE_END - 1); i++)
        fprintf(fp, "%d %s \n", i, gamesubStatenames[i].data);
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
        fprintf(fp, "%d %s \n", i, statNames[i].data);
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
