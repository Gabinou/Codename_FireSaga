#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "enums.h"
#include "structs.h"
#include "str.h"
#include "narrative.h"
#include "flecs.h"
#include "physfs.h"
#include "platform.h"
#include "SDL2/SDL.h"
#include "stb_ds.h"

extern ecs_entity_t entity_isIn(ecs_entity_t in_ent, ecs_entity_t * in_possible, uint8_t num_possible);

extern void utilities_inputFlag_Read(const char in_flag, const char * in_argstr, struct Input_Arguments * in_args);
extern void utilities_printHelp();
extern struct Input_Arguments utilities_parseInputs(int32_t argc, char * argv[]);
extern bool utilities_isInputFlag(char * arg);

extern struct Uint64_t_nestedhash * lvlup_skills;
extern struct Units_stats_hash * promotion_bonuses;
extern void makePromotionbonusstats();
extern struct Uint64_t_hash * promotion_bonus_skills;
extern void makePromotionbonusskills();

extern bool fequal(const char * filename1, const char * filename2);

extern char ** utilities_skillNames(uint64_t in_skillscode);
extern char ** utilities_wpnEffects(uint64_t in_effect);
extern char ** utilities_unitstateNames(uint32_t in_statecode);
extern char ** utilities_unitTypes(uint16_t in_typecode);
extern char * unitstats2str(struct Unit_stats in_stats);
extern char * wpnstats2str(struct Weapon_stats in_stats);
extern char ** utilities_wpnType(uint16_t in_typecode);
// extern char ** wpnTypes(uint16_t in_typecode);

extern struct Shops_hash * shop_inventories;
extern void makeShops();

extern char ** mapFilenames;
extern void makemapFilenames();
extern char ** mapFilenames;
extern void makemapFilenames();
extern char ** unitStates;
extern void makeunitStates();
extern char ** jsonElementnames;
extern void makejsonElementnames();
extern char ** campjobNames;
extern void makecampjobNames();
extern char ** gameStatenames;
extern void makegameStatenames();
extern char ** gamesubStatenames;
extern void makegamesubStatenames();
extern char ** menuOptionnames;
extern void Menu_MakeOptionnames();
extern char ** menuNames;
extern void Menu_MakeNames();
extern char ** unitNames;
extern void makeunitNames();
#define NAME_ITEM_INVALID " "
extern struct String_hash * itemNames;
extern void makeitemNames();
extern char ** statNames;
extern void makestatNames();
extern char ** sexNames;
extern void makesexNames();
extern char ** classNames;
extern void makeclassNames();
extern struct String_hash * tilenames;
extern void maketileNames();
extern uint16_t * equippableClasscodes;
extern void makeequippableClasscodes();
extern char ** armyNames;
extern void makeArmyNames();
extern char ** errorNames;
extern void makeErrorNames();

#define STRINGIZE_ERROR(x) str_slicefromStart(str_replaceSingle(#x, '_', ' '), 6)
#define STRINGIZE_MENU_OPTION(x) str_slicefromStart(str_camelCase(str_toLower(str_replaceSingle(#x, '_', ' ')), ' ', 2), 12)
#define STRINGIZE_MENU(x) str_slicefromStart(str_camelCase(str_toLower(str_replaceSingle(#x, '_', ' ')), ' ', 2), 5)
#define STRINGIZE_GAME_SUBSTATE(x) str_slicefromStart(str_camelCase(str_toLower(str_replaceSingle(#x, '_', ' ')), ' ', 2), 14)
#define STRINGIZE_GAME_STATE(x) str_slicefromStart(str_camelCase(str_toLower(str_replaceSingle(#x, '_', ' ')), ' ', 2), 11)
#define STRINGIZE_JSON(x) str_slicefromStart(str_camelCase(str_toLower(str_replaceSingle(#x, '_', ' ')),' ', 2), 5)
#define STRINGIZE_UNIT_ARMY(x) str_slicefromStart(str_camelCase(str_toLower(str_replaceSingle(#x, '_', ' ')),' ', 2), 10)
#define STRINGIZE_ITEM_NAME(x) str_slicefromStart(str_camelCase(str_toLower(str_replaceSingle(#x, '_', ' ')),' ', 2), 10)
#define STRINGIZE_TILE(x) str_slicefromStart(str_camelCase(str_toLower(#x), ' ', 2), 5)
#define STRINGIZE_CAMPJOB(x) str_slicefromStart(str_camelCase(str_toLower(#x), ' ', 2), 8)
#define STRINGIZE_UNIT_NAME(x) str_slicefromStart(str_camelCase(str_toLower(str_replaceSingle(#x, '_', ' ')), ' ', 2), 10)
#define STRINGIZE_ITEM_STAT(x) str_slicefromStart(str_camelCase(str_toLower(str_replaceSingle(#x, '_', ' ')), ' ', 2), 10)
#define STRINGIZE_UNIT_STATE(x) str_slicefromStart(str_camelCase(str_toLower(str_replaceSingle(#x, '_', ' ')), ' ', 2), 11)
#define STRINGIZE_UNIT_CLASS(x) str_slicefromStart(str_camelCase(str_toLower(str_replaceSingle(#x, '_', ' ')),' ', 2), 11)

extern uint8_t utilities_army2alignment(const uint8_t army1);
extern bool utilities_isPC(const uint8_t army);

extern uint8_t utilities_mvtType(uint8_t in_class_index);
extern void utilities_allNames_Load();
extern void utilities_allNames_Print(const char * filename);

extern void utilities_clean();

extern void swap(int32_t arr[], size_t ind1, size_t ind2);
extern size_t partition(int32_t arr[], size_t low, size_t high);
extern void quicksort(int32_t arr[], size_t low, size_t high);

#endif /* UTILITIES_H */
