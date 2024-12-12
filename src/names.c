#include "names.h"
#include "platform.h"
#include "macros.h"
#include "stb_sprintf.h"

s8 sexNames[UNIT_SEX_NUM] = {
    s8_literal("F"),
    s8_literal("M")
};

s8 scenes[SCENE_NUM] = {0};
void Names_scenes(void) {
#define REGISTER_ENUM(x) scenes[SCENE_##x] = s8cat(s8_camelCase(s8_toLower(s8_mut(#x)), '_', 2), s8_literal("_"));
#include "names/scene.h"
#undef REGISTER_ENUM
}

s8 sceneTimes[SCENE_TIME_NUM] = {0};
void Names_sceneTimes(void) {
#define REGISTER_ENUM(x) sceneTimes[SCENE_TIME_##x] = s8cat(s8_camelCase(s8_toLower(s8_mut(#x)), '_', 2), s8_literal("_"));
#include "names/scene_time.h"
#undef REGISTER_ENUM
}

s8  global_unitNames[UNIT_NUM]  = {0};
u64 unitHashes[UNIT_NUM]        = {0};
struct dtab *global_unitOrders  = NULL;
void Names_unitNames(void) {
    DTAB_INIT(global_unitOrders, u64);
    SDL_assert(global_unitOrders != NULL);
    u64 order = 0;
    dtab_add(global_unitOrders, &order, UNIT_ID_NULL);
    order++;
#define REGISTER_ENUM(x, y) dtab_add(global_unitOrders, &order, UNIT_ID_##x);\
    SDL_assert(*(u64 *)dtab_get(global_unitOrders, UNIT_ID_##x) == order++);\
    global_unitNames[UNIT_ORDER_##x] = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')), ' ', 2);\
    unitHashes[UNIT_ORDER_##x] = sota_hash_djb2(global_unitNames[UNIT_ORDER_##x]);
#include "names/units_PC.h"
#include "names/units_NPC.h"
    SDL_assert(global_unitOrders->num == order + 1);
}
#undef REGISTER_ENUM

s8 Item_Name(int id) {
    SDL_assert(global_itemOrders != NULL);
    SDL_assert(global_itemNames != NULL);
    int *order = DTAB_GET(global_itemOrders, id);
    SDL_assert(order != NULL);
    return (global_itemNames[*order]);
}

int Unit_Name2Order(s8 name) {
    int order = -1;
    u64 hash = sota_hash_djb2(name);
    for (int i = 0; i < UNIT_NUM; i++) {
        if (hash == unitHashes[i]) {
            order = i;
            break;
        }
    }
    return (order);
}

s8 statNames[UNIT_STAT_MALLOC] = {0};
void Names_statNames(void) {
#define REGISTER_ENUM(x) statNames[ITEM_STAT_##x] = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')), ' ', 2);
#include "names/items_stats.h"
#undef REGISTER_ENUM
}

s8 armyNames[ARMY_MALLOC] = {0};
void Names_armyNames(void) {
#define REGISTER_ENUM(x, y) armyNames[ARMY_##x] = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')),' ', 2);
#include "names/armies.h"
#undef REGISTER_ENUM
}

s8 unitStatuses[UNIT_STATUS_END];
void Names_unitStatuses(void) {
#define REGISTER_ENUM(x) unitStatuses[UNIT_STATUS_##x] =  s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')), ' ', 2);
#include "names/units_statuses.h"
#undef REGISTER_ENUM
}

s8 global_itemNames[ITEM_NUM]   = {0};
struct dtab *global_itemOrders  = NULL;
void Names_itemNames(void) {
    DTAB_INIT(global_itemOrders, i32);
    SDL_assert(global_itemOrders != NULL);
    int order = 0;
    dtab_add(global_itemOrders, &order, ITEM_NULL);
    order++;
#define REGISTER_ENUM(x, y) dtab_add(global_itemOrders, &order, ITEM_ID_##x);\
    order++;\
    global_itemNames[ITEM_ORDER_##x] = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')), ' ', 2);
#include "names/items.h"
#undef REGISTER_ENUM
}

s8 support_types[SUPPORT_TYPE_NUM] = {0};
void Names_supportTypes(void) {
}

s8 global_tilenames[TILE_ID_MAX] = {0};
void Names_tileNames(void) {
    uint64_t temp_id = 1; /* 0 is reserved for NULL*/
#define REGISTER_ENUM(x) global_tilenames[temp_id++] = s8_camelCase(s8_toLower(s8_mut(#x)), ' ', 2);
#include "names/tiles.h"
}
#undef REGISTER_ENUM

s8 campjobNames[CAMPJOB_END] = {0};
void Names_campjobNames(void) {
#define REGISTER_ENUM(x) campjobNames[CAMPJOB_##x] = s8_camelCase(s8_toLower(s8_mut(#x)), ' ', 2);
#include "names/camp_jobs.h"
#undef REGISTER_ENUM
}

s8 menuOptionnames[MENU_OPTION_END] = {0};
void Menu_MakeOptionnames(void) {
#define REGISTER_ENUM(x, y) menuOptionnames[MENU_OPTION_##x] = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')), ' ', 2);
#include "names/menu/options.h"
#undef REGISTER_ENUM
}

s8 gamesubStatenames[GAME_SUBSTATE_END] = {0};
void Names_gamesubStatenames(void) {
#define REGISTER_ENUM(x, y) gamesubStatenames[GAME_SUBSTATE_##x] = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')), ' ', 2);
#include "names/game_substates.h"
#undef REGISTER_ENUM
}

s8 gameStatenames[GAME_STATE_END] = {0};
void Names_gameStatenames(void) {
#define REGISTER_ENUM(x, y) gameStatenames[GAME_STATE_##x] = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')), ' ', 2);
#include "names/game_states.h"
#undef REGISTER_ENUM
}

s8 mapFilenames[CHAPTER_MALLOC] = {0};
void Names_mapFilenames(void) {
#define REGISTER_ENUM(x, y, z) mapFilenames[CHAPTER_##x] = s8cat(s8_mut("assets"PHYSFS_SEPARATOR"maps"PHYSFS_SEPARATOR), s8_literal(#y));
#include "names/chapters.h"
#undef REGISTER_ENUM
}


s8 classNames[UNIT_CLASS_NUM] = {0};
void Names_classNames(void) {
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

s8 *Names_unitstateNames(uint32_t in_statecode) {
    s8 *state_names = DARR_INIT(state_names, s8, UNIT_STATUS_END);
#define REGISTER_ENUM(x) if flagsum_isIn(in_statecode, UNIT_STATUS_##x) {\
        DARR_PUT(state_names, s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')),' ', 2));\
    }
#include "names/units_statuses.h"
#undef  REGISTER_ENUM
    return (state_names);
}

u64 *class_equippables = NULL;
void Names_class_equippables(void) {
    class_equippables = DARR_INIT(class_equippables, u64, UNIT_CLASS_END);
#define REGISTER_ENUM(name, id, x, y, z) class_equippables[UNIT_CLASS_##name] = z;
#include "names/classes.h"
#undef  REGISTER_ENUM
}

s8 *Names_unitTypes(u16 typecode) {
    s8 *out = DARR_INIT(out, s8, UNIT_TYPE_END);
#define REGISTER_ENUM(x) if flagsum_isIn(typecode, UNIT_TYPE_##x) {\
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
void Names_jsonElementnames(void) {
#define REGISTER_ENUM(x) jsonElementnames[JSON_##x] = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')),' ', 2);
#include "names/json_elements.h"
#undef REGISTER_ENUM
    s8_free(&jsonElementnames[JSON_WEAPON]);
    jsonElementnames[JSON_WEAPON] = s8_mut("Item");
}

s8 *Names_wpnType(u16 typecode) {
    s8 *type_names = DARR_INIT(type_names, s8, ITEM_TYPE_END);
    s8 type_name;
#define REGISTER_ENUM(x) if flagsum_isIn(typecode, ITEM_TYPE_##x) {\
        type_name = s8_camelCase(s8_toLower(s8_replaceSingle(s8_mut(#x), '_', ' ')),' ', 2);\
        DARR_PUT(type_names, type_name);\
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
void Names_Load_All(void) {
    Names_unitNames();
    Names_scenes();
    Names_sceneTimes();
    // Hashes_supportHashes();
    // Hashes_unitHashes();
    // Hashes_itemHashes();
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

void Names_Free(void) {
    for (size_t i = MENU_OPTION_START + 1; i < MENU_OPTION_END; i++) {
        s8_free(&menuOptionnames[i]);
    }

    for (size_t i = 0; i < SCENE_TIME_NUM; i++) {
        s8_free(&sceneTimes[i]);
    }

    for (size_t i = 0; i < GAME_SUBSTATE_END; i++) {
        s8_free(&gamesubStatenames[i]);
    }
    for (size_t i = 0; i < GAME_STATE_END; i++) {
        s8_free(&gameStatenames[i]);
    }
    for (size_t i = CHAPTER_NULL + 1; i < CHAPTER_MALLOC; i++) {
        s8_free(&mapFilenames[i]);
    }
    for (size_t i = 0; i < JSON_END; i++) {
        s8_free(&jsonElementnames[i]);
    }
    for (size_t i = 0; i < ARMY_END; i++) {
        s8_free(&armyNames[i]);
    }
    for (size_t i = 0; i < ITEM_NUM; i++) {
        s8_free(&global_itemNames[i]);
    }
    for (size_t i = TILE_START; i < TILE_END; i++) {
        s8_free(&global_tilenames[i]);
    }
    for (size_t i = 0; i < CAMPJOB_END; i++) {
        s8_free(&campjobNames[i]);
    }
    for (int i = 0; i < UNIT_NUM; i++) {
        s8_free(&global_unitNames[i]);
    }
    if (global_unitOrders != NULL) {
        DTAB_FREE(global_unitOrders);
        global_unitOrders = NULL;
    }
    for (size_t i = 0; i < ITEM_STAT_END; i++) {
        s8_free(&statNames[i]);
    }
    for (size_t i = 0; i < UNIT_STATUS_END; i++) {
        s8_free(&unitStatuses[i]);
    }
    if (global_itemOrders != NULL) {
        DTAB_FREE(global_itemOrders);
        global_itemOrders = NULL;
    }
    for (size_t i = 0; i < UNIT_CLASS_END; i++) {
        s8_free(&classNames[i]);
    }
    if (class_equippables != NULL) {
        DARR_FREE(class_equippables);
        class_equippables = NULL;
    }
}

void Names_Print_All(char *foldername) {
    // SDL_Log("foldername %s", foldername);
    /* TODO: Use PHYSFS_writeBytes, PHYSFS_openWrite */

    FILE *fp = NULL;
    /* --- Army names --- */
    s8 filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_armyNames.txt"));
    // SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = ARMY_START; i < (ARMY_END - 1); i++) {
        fprintf(fp, "%d %s \n", i, armyNames[i].data);
    }
    fclose(fp);
    s8_free(&filename);

    /* --- Stat names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_unitStatuses.txt"));
    // SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = UNIT_STATUS_EXP_START; i < (UNIT_STATUS_EXP_END - 1); i++)
        fprintf(fp, "%d %s \n", i, unitStatuses[i].data);
    fclose(fp);
    s8_free(&filename);

    /* --- Json element names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_jsonElementnames.txt"));
    // SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = JSON_START; i < (JSON_END - 1); i++)
        fprintf(fp, "%d %s \n", i, jsonElementnames[i].data);
    fclose(fp);
    s8_free(&filename);

    /* --- Camp job names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_campjobNames.txt"));
    // SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = CAMPJOB_START; i < (CAMPJOB_END - 1); i++)
        fprintf(fp, "%d %s \n", i, campjobNames[i].data);
    fclose(fp);
    s8_free(&filename);

    /* --- Game State names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_gameStatenames.txt"));
    // SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = GAME_STATE_START; i < (GAME_STATE_END - 1); i++)
        fprintf(fp, "%d %s \n", i, gameStatenames[i].data);
    fclose(fp);
    s8_free(&filename);

    /* --- Menu Option names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_menuOptionnames.txt"));
    // SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = MENU_OPTION_START; i < MENU_OPTION_END; i++)
        fprintf(fp, "%d %s \n", i, menuOptionnames[i].data);
    fclose(fp);
    s8_free(&filename);

    /* --- Game substate names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_gamesubStatenames.txt"));
    // SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = GAME_SUBSTATE_START; i < (GAME_SUBSTATE_END - 1); i++)
        fprintf(fp, "%d %s \n", i, gamesubStatenames[i].data);
    fclose(fp);
    s8_free(&filename);

    /* --- Unit names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_unitNames.txt"));
    // SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (size_t i = 0; i < UNIT_NUM; i++)
        fprintf(fp, "%zu %s \n", i, global_unitNames[i].data);
    fclose(fp);
    s8_free(&filename);

    /* --- Item names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_itemNames.txt"));
    // SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (size_t i = 0; i < global_itemOrders->num; i++)
        fprintf(fp, "%zu %s \n", i, global_itemNames[i].data);
    fclose(fp);

    /* --- State names --- */
    s8_free(&filename);
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_statNames.txt"));
    // SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = ITEM_STAT_START; i < (ITEM_STAT_END - 1); i++)
        fprintf(fp, "%d %s \n", i, statNames[i].data);
    fclose(fp);

    /* --- Sex names --- */
    s8_free(&filename);
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("utilitiessexNames.txt"));
    // SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = UNIT_SEX_F; i < (UNIT_SEX_M + 1); i++)
        fprintf(fp, "%d %s \n", i, sexNames[i].data);
    fclose(fp);

    /* --- Class names --- */
    s8_free(&filename);
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_classNames.txt"));
    // SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    for (u8 i = UNIT_CLASS_START; i < UNIT_CLASS_END; i++)
        fprintf(fp, "%d %s \n", i, classNames[i].data);
    fclose(fp);
    s8_free(&filename);

    /* --- Tile names --- */
    filename = s8_mut(foldername);
    filename = s8cat(filename, s8_literal("Utilities_tilenames.txt"));
    // SDL_Log("filename %s", filename.data);
    fp = fopen(filename.data, "w+");
    SDL_assert(fp != NULL);
    SDL_assert(global_tilenames != NULL);
    s8 s8null = s8_literal("");
    for (u8 i = TILE_START; i < TILE_END; i++) {
        s8 tilename = global_tilenames[i];
        if (!s8equal(tilename, s8null))
            fprintf(fp, "%d %s \n", i, tilename.data);
    }
    fclose(fp);
    s8_free(&filename);
}

u64 sota_hash_djb2(s8 str) {
    /* djb2 hashing algorithm by Dan Bernstein. */
    /* Description: This algorithm (k=33) was first reported by dan bernstein many */
    /* years ago in comp.lang.c. Another version of this algorithm (now favored by bernstein) */
    /* uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number 33 */
    /* (why it works better than many other constants, prime or not) has never been adequately explained. */
    /* [1] https://stackoverflow.com/questions/7666509/hash-function-for-string */
    /* [2] http://www.cse.yorku.ca/~oz/hash.html */
    u64 hash     = 5381;
    i32 str_char =    0;
    while ((str_char = *str.data++))
        hash = ((hash << 5) + hash) + str_char; /* hash * 33 + c */
    return (hash);
}

