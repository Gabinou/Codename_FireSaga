#include "utilities.h"

// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
// #endif  STB_SPRINTF_IMPLEMENTATION
#include "stb_ds.h"

#include "stb_sprintf.h"

// sizeof vs strlen -> weird bytes if using strlen

ecs_entity_t entity_isIn(ecs_entity_t in_ent, ecs_entity_t * in_possible, uint8_t num_possible) {
    ecs_entity_t out_ent = 0;
    for (uint8_t i = 0; i < num_possible; i++) {
        if (in_ent == in_possible[i]) {
            out_ent = in_possible[i];
            break;
        }
    }
    return (out_ent);
}

void utilities_printHelp() {
    printf("\nusage: " EXE_NAME " [options] \n");
    printf("Getting help: \n");
    printf("  /?, -?                 show help \n");
    printf("  --help, -help, help    show help \n");
    printf("debug options: \n");
    printf("  -s  filename     load save state   (ignored unless -m specified)\n");
    printf("  -m  map_index    load game to map  (ignored unless -s specified)\n");
}

void utilities_inputFlag_Read(const char in_flag, const char * in_argstr, struct Input_Arguments * in_args) {
    SDL_Log("utilities_inputFlag_Read");
    switch (in_flag) {
        case 's': // Save
            if (in_args->save_filename != NULL) {
                free(in_args->save_filename);
            }
            in_args->save_filename = (char *) SDL_malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
            strncpy(in_args->save_filename, in_argstr, strlen(in_args->save_filename));
            break;
        case 'm': // Map
            in_args->map_index = strtol(in_argstr, NULL, 10);
            break;
        default:
            SDL_Log("%s is an invalid input flag");
            break;
    }
}

bool utilities_isHelp(char * arg) {
    SDL_Log("utilities_isHelp");
    SDL_assert(arg);
    uint8_t arg_len = strlen(arg);
    bool out = (strncmp(arg, "--help", arg_len) == 0) || (strncmp(arg, "-help", arg_len) == 0) || (strncmp(arg, "help", arg_len) == 0) || (strncmp(arg, "?", arg_len) == 0)  || (strncmp(arg, "/?", arg_len) == 0) || (strncmp(arg, "-?", arg_len) == 0) || (strncmp(arg, "--?", arg_len) == 0);
    return (out);
}

bool utilities_isInputFlag(char * arg) {
    SDL_Log("utilities_isInputFlag");
    SDL_assert(arg);
    bool isFlag = (strlen(arg) == 2) && ((memchr(arg, '-', strlen(arg)) != NULL) || (memchr(arg, '/', strlen(arg)) != NULL));
    return (isFlag);
}

struct Input_Arguments utilities_parseInputs(int32_t argc, char * argv[]) {
    SDL_Log("utilities_parseInputs");

    struct Input_Arguments out_args = Input_Arguments_default;

    if ((argc > 1) && (argc < MAX_INPUT_ARGS)) {

        for (uint8_t i = 1; i < argc; ++i) {// help check loop
            if (utilities_isHelp(argv[i])) {
                utilities_printHelp();
                exit(NO_ERROR);
            }
        }

        for (uint8_t i = 1; i < argc; ++i) {// arg read loop
            if ((utilities_isInputFlag(argv[i])) && ((i + 1) < argc)) {
                if (utilities_isInputFlag(argv[i + 1])) {
                    // also inputs should never be negative numbers.
                    SDL_Log("%s is also flag", argv[i + 1]);
                    char * temp_error = STRINGIZE_ERROR(ERROR_InputFlagFollowsInputFlag);
                    printf("ERROR: %s\n", temp_error);
                    free(temp_error);
                    exit(ERROR_InputFlagFollowsInputFlag);
                } else {
                    utilities_inputFlag_Read(argv[i][1], argv[i + 1], &out_args);
                }
            }
        }

    } else if (argc > MAX_INPUT_ARGS) {
        SDL_Log("Too many input arguments %d, max: %d", argc, MAX_INPUT_ARGS);
        SDL_Log("Aborting");
        exit(ERROR_TooManyInputArgs);
    }
    return (out_args);
}

void swap(int32_t arr[], size_t ind1, size_t ind2) {
    int32_t buffer;
    buffer = arr[ind1];
    arr[ind1] = arr[ind2];
    arr[ind2] = buffer;
}

size_t partition(int32_t arr[], size_t low, size_t high) {
    size_t pivot = (size_t)arr[high];
    size_t i = low - 1;
    for (size_t j = low; j < high; j++) {
        if ((size_t)arr[j] < pivot) {
            swap(arr, ++i, j);
        }
    }
    swap(arr, (size_t)(i + 1), high);
    return (i + 1);
}

void quicksort(int32_t arr[], size_t low, size_t high) {
    size_t pi;
    if (low < high) {
        pi = partition(arr, low, high);
        quicksort(arr, low, (size_t)(pi - 1));
        quicksort(arr, (size_t)(pi + 1), high);
    }
}

bool fequal(const char * filename1, const char * filename2) {
    bool out = false;
    PHYSFS_file * fp1 = PHYSFS_openRead(filename1);
    PHYSFS_file * fp2 = PHYSFS_openRead(filename2);
    SDL_assert(fp1);
    SDL_assert(fp2);
    uint32_t filelen1 = PHYSFS_fileLength(fp1);
    uint32_t filelen2 = PHYSFS_fileLength(fp2);
    char filebuffer1[filelen1];
    char filebuffer2[filelen2];
    PHYSFS_readBytes(fp1, filebuffer1, filelen1);
    PHYSFS_readBytes(fp2, filebuffer2, filelen2);
    PHYSFS_close(fp1);
    PHYSFS_close(fp2);
    if (filelen1 == filelen2) {
        out = true;
        for (uint32_t i = 0; i < filelen1; i++) {
            if (filebuffer1[i] != filebuffer2[i]) {
                out = false;
            }
        }
    }
    return (out);
}

struct Uint64_t_nestedhash * class_lvlup_skills;
struct Uint64_t_nestedhash * unit_lvlup_skills;
void makeLvlupskills() {
    // hmdefault(class_lvlup_skills, NULL);
    // hmdefault(unit_lvlup_skills, NULL);
    // hmput(class_lvlup_skills, UNIT_CLASS_DUKE, NULL);
    // hmput(hmget(class_lvlup_skills, UNIT_CLASS_DUKE), 7, UNIT_SKILL_CANTO);
    // hmput(hmget(lvlup_skills, UNIT_CLASS_DUKE), 7, UNIT_SKILL_CANTO);
}

struct Uint64_t_hash * promotion_bonus_skills = NULL;
void makePromotionbonusskills() {
    uint64_t temp_skill = 0;
    uint64_t temp_class = 0;
#define REGISTER_ENUM(x, y, z, i) temp_class = UNIT_CLASS_##x;\
    temp_skill = UNIT_SKILL_##i;\
    hmput(promotion_bonus_skills, temp_class, temp_skill);
#include "names/classes.h"
#undef REGISTER_ENUM
}

void makePromotionbonusstats() {
    SDL_Log("makePromotionbonusstats");
    struct Unit_stats temp = Unit_stats_default;
    uint64_t temp_class = 0;
    struct Units_stats_hash * promotion_bonus_stats = NULL;
#define REGISTER_ENUM(x, ihp, istr, imag, iagi, idex, iluck, idef, ires, icon, imove, iprof) temp.hp = ihp;\
temp.str = istr;\
temp.mag = imag;\
temp.agi = iagi;\
temp.dex = idex;\
temp.luck = iluck;\
temp.def = idef;\
temp.res = ires;\
temp.con = icon;\
temp.move = imove;\
temp.prof = iprof;\
temp_class = UNIT_CLASS_##x;\
hmput(promotion_bonus_stats, temp_class, temp);
#include "names/promotion_bonuses.h"
#undef REGISTER_ENUM
}

#define REGISTER_ENUM(x, y, z) temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
strncpy(temp_str, #x, sizeof(#x));\
menuOptionnames[MENU_OPTION_##x] = str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')), ' ', 2);

char ** menuOptionnames = NULL;
void Menu_MakeOptionnames() {
    char * temp_str = NULL;
    menuOptionnames = calloc(MENU_OPTION_END, sizeof(*menuOptionnames));
#include "names/menu_options.h"
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
strncpy(temp_str, #x, sizeof(#x));\
menuNames[MENU_##x], str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')), ' ', 2);

char ** menuNames = NULL;
void Menu_MakeNames() {
    SDL_Log("Menu_MakeNames");
    char * temp_str = NULL;
    menuNames = calloc(MENU_END, sizeof(*menuNames));
#include "names/menu.h"
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x)  temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
strncpy(temp_str, #x, sizeof(#x));\
gamesubStatenames[GAME_SUBSTATE_##x] = str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')), ' ', 2);

char ** gamesubStatenames = NULL;
void makegamesubStatenames() {
    SDL_Log("makegamesubStatenames");
    char * temp_str = NULL;
    gamesubStatenames = calloc(GAME_SUBSTATE_END, sizeof(*gamesubStatenames));
#include "names/game_substates.h"
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
strncpy(temp_str, #x, sizeof(#x));\
gameStatenames[GAME_STATE_##x] = str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')), ' ', 2);

char ** gameStatenames = NULL;
void makegameStatenames() {
    SDL_Log("makegameStatenames");
    char * temp_str = NULL;
    gameStatenames = calloc(GAME_STATE_END, sizeof(*gameStatenames));
#include "names/game_states.h"
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
strcpy(temp_str, "");\
strcat(temp_str, "maps"PHYSFS_SEPARATOR);\
strcat(temp_str, #y);\
mapFilenames[CHAPTER_##x] =  temp_str;

char ** mapFilenames = NULL;
void makemapFilenames() {
    SDL_Log("makemapFilenames");
    char * temp_str = NULL;
    mapFilenames = calloc(CHAPTER_END, sizeof(*mapFilenames));
#include "names/chapters.h"
}
#undef REGISTER_ENUM

char * Unitstats2str(struct Unit_stats in_stats) {
    char * out = SDL_malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
    stbsp_sprintf(out, "%02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d", in_stats.hp, in_stats.str, in_stats.mag, in_stats.agi, in_stats.dex, in_stats.luck, in_stats.def, in_stats.res, in_stats.con, in_stats.move, in_stats.prof);
    return (out);
}

char * Wpnstats2str(struct Weapon_stats in_stats) {
    char * out = SDL_malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
    stbsp_sprintf(out, "%02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d", in_stats.Pmight, in_stats.Mmight, in_stats.combat.hit, in_stats.combat.dodge, in_stats.combat.crit, in_stats.combat.favor, in_stats.wgt, in_stats.uses, in_stats.prof, in_stats.range[0], in_stats.range[1], in_stats.hand[0], in_stats.hand[1], in_stats.dmg_type);
    return (out);
}

uint8_t utilities_army2alignment(const uint8_t in_army) {
    SDL_Log("utilities_army2alignment");
    uint8_t alignment = NO_ALIGNMENT;
    switch (in_army) {
        case ARMY_FRIENDLY:
        case ARMY_ERWIN:
        case ARMY_FREE_MILITIA:
        case ARMY_NEUTRAL:
        case ARMY_THEOCRACY:
        case ARMY_IMPERIAL:
        case ARMY_VOLDAN:
        case ARMY_KESIRU:
            alignment = ALIGNMENT_FRIENDLY;
            break;
        case ARMY_ENEMY:
        case ARMY_KEWAC:
        case ARMY_BANDITS:
        case ARMY_HAMILCAR:
        case ARMY_FIRST_EXPEDITIONARY_FORCE:
            alignment = ALIGNMENT_ENEMY;
            break;
        default:
            SDL_Log("army is invalid");
    }
    return (alignment);
}


bool utilities_isPC(const uint8_t army) {
    bool out = false;
    switch (army) {
        case ARMY_FRIENDLY:
        case ARMY_ERWIN:
        case ARMY_FREE_MILITIA:
            out = true;
            break;
        case ARMY_NEUTRAL:
        case ARMY_ENEMY:
        case ARMY_IMPERIAL:
        case ARMY_THEOCRACY:
        case ARMY_VOLDAN:
            out = false;
            break;
        default:
            SDL_Log("army is invalid");
    }
    return (out);
}

#define REGISTER_ENUM(x) if flagsum_isIn(in_typecode, UNIT_TYPE_##x) {\
    temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
    strncpy(temp_str, #x, sizeof(#x));\
    DARR_PUT(out, str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')), ' ', 2));\
    }
char ** utilities_unitTypes(uint16_t in_typecode) {
    char * temp_str = NULL;
    char ** out = DARR_INIT(char *, UNIT_TYPE_END);
#include "names/units_types.h"
    return (out);
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) if flagsum_isIn(in_effect, ITEM_EFFECT_##x) {\
    temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
    strncpy(temp_str, #x, sizeof(#x));\
    DARR_PUT(wpn_effects, str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')), ' ', 2));\
    }

char ** utilities_wpnEffects(uint64_t in_effect) {
    SDL_Log("utilities_wpnEffects");
    char * temp_str = NULL;
    char ** wpn_effects = DARR_INIT(char *, UNIT_TYPE_END);
    DARR_LEN(wpn_effects) = UNIT_TYPE_END;
    DARR_NUM(wpn_effects) = 0;
#include "names/items_effects.h"
    return (wpn_effects);
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x)  temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
strncpy(temp_str, #x, sizeof(#x));\
jsonElementnames[JSON_##x] = str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')),' ', 2);

char ** jsonElementnames = NULL;
void makejsonElementnames() {
    SDL_Log("makejsonElementnames");
    char * temp_str = NULL;
    jsonElementnames = calloc(JSON_END, sizeof(*jsonElementnames));
#include "names/json_elements.h"
    strcpy(jsonElementnames[JSON_WEAPON], "Item");
}
#undef REGISTER_ENUM

struct Shops_hash * shop_inventories = NULL;
void makeShops() {
    SDL_Log("makeShops");
    struct Shop_item * temp_shop = NULL;
    struct Shop_item temp_item = Shop_item_default;
    uint64_t temp_shop_id;
    // hmdefault(shop_inventories, NULL);
    // temp_item.id = ITEM_NAME_HOE;
    // temp_item.num = -1;
    // arrput(temp_shop, temp_item);
    // temp_shop_id = SHOP_CAMP_CHAPTER_1;
    // hmput(shop_inventories, temp_shop_id, temp_shop);
    // arrfree(temp_shop);
    // temp_item.id = ITEM_NAME_HOE;
    // temp_item.num = -1;
    // arrput(temp_shop, temp_item);
    // hmput(shop_inventories, temp_shop_id, temp_shop);
    // temp_shop_id = SHOP_CAMROSE;
    // hmput(shop_inventories, temp_shop_id, temp_shop);
}

void utilities_allNames_Print(const char * foldername) {
    SDL_Log("utilities_allNames_Print");
    SDL_Log("foldername %s", foldername);
    char filename[DEFAULT_BUFFER_SIZE] = "";
    char buffer[DEFAULT_BUFFER_SIZE] = "";
    FILE * fp = NULL;
    strcat(filename, foldername);
    strcat(filename, "utilities_unitArmies.txt");
    SDL_Log("filename %s", filename);

    fp = fopen(filename, "w+");
    SDL_assert(fp != NULL);
    for (uint8_t i = ARMY_START; i < (ARMY_END - 1); i++) {
        fprintf(fp, "%d %s \n", i, armyNames[i]);
    }
    fclose(fp);
    memset(filename, 0, sizeof(filename));

    strcat(filename, foldername);
    strcat(filename, "utilities_unitStates.txt");
    SDL_Log("filename %s", filename);
    fp = fopen(filename, "w+");
    SDL_assert(fp != NULL);
    for (uint8_t i = UNIT_STATE_EXP_START; i < (UNIT_STATE_EXP_END - 1); i++) {
        fprintf(fp, "%d %s \n", i, unitStates[i]);
    }
    fclose(fp);
    memset(filename, 0, sizeof(filename));

    strcat(filename, foldername);
    strcat(filename, "utilities_jsonElementnames.txt");
    SDL_Log("filename %s", filename);
    fp = fopen(filename, "w+");
    SDL_assert(fp != NULL);
    for (uint8_t i = JSON_START; i < (JSON_END - 1); i++) {
        fprintf(fp, "%d %s \n", i, jsonElementnames[i]);
    }
    fclose(fp);
    memset(filename, 0, sizeof(filename));

    strcat(filename, foldername);
    strcat(filename, "utilities_campjobNames.txt");
    SDL_Log("filename %s", filename);
    fp = fopen(filename, "w+");
    SDL_assert(fp != NULL);
    for (uint8_t i = CAMPJOB_START; i < (CAMPJOB_END - 1); i++) {
        fprintf(fp, "%d %s \n", i, campjobNames[i]);
    }
    fclose(fp);
    memset(filename, 0, sizeof(filename));

    strcat(filename, foldername);
    strcat(filename, "utilities_gameStatenames.txt");
    SDL_Log("filename %s", filename);
    fp = fopen(filename, "w+");
    SDL_assert(fp != NULL);
    for (uint8_t i = GAME_STATE_START; i < (GAME_STATE_END - 1); i++) {
        fprintf(fp, "%d %s \n", i, gameStatenames[i]);
    }
    fclose(fp);
    memset(filename, 0, sizeof(filename));

    strcat(filename, foldername);
    strcat(filename, "utilities_gamesubStatenames.txt");
    SDL_Log("filename %s", filename);
    fp = fopen(filename, "w+");
    SDL_assert(fp != NULL);
    for (uint8_t i = GAME_SUBSTATE_START; i < (GAME_SUBSTATE_END - 1); i++) {
        fprintf(fp, "%d %s \n", i, gamesubStatenames[i]);
    }
    fclose(fp);
    memset(filename, 0, sizeof(filename));

    strcat(filename, foldername);
    strcat(filename, "utilities_menuOptionnames.txt");
    SDL_Log("filename %s", filename);
    fp = fopen(filename, "w+");
    SDL_assert(fp != NULL);
    for (uint8_t i = MENU_OPTION_START; i < arrlen(menuOptionnames); i++) {
        fprintf(fp, "%d %s \n", i, menuOptionnames[i]);
    }
    fclose(fp);
    memset(filename, 0, sizeof(filename));

    strcat(filename, foldername);
    strcat(filename, "utilities_menuNames.txt");
    SDL_Log("filename %s", filename);
    fp = fopen(filename, "w+");
    SDL_assert(fp != NULL);
    for (uint8_t i = MENU_START; i < (MENU_END - 1); i++) {
        fprintf(fp, "%d %s \n", i, menuNames[i]);
    }
    fclose(fp);
    memset(filename, 0, sizeof(filename));

    strcat(filename, foldername);
    strcat(filename, "utilities_unitNames.txt");
    SDL_Log("filename %s", filename);
    fp = fopen(filename, "w+");
    SDL_assert(fp != NULL);
    for (uint8_t i = UNIT_NAME_START; i < (UNIT_NAME_END - 1); i++) {
        fprintf(fp, "%d %s \n", i, unitNames[i]);
    }
    fclose(fp);
    memset(filename, 0, sizeof(filename));

    strcat(filename, foldername);
    strcat(filename, "utilities_itemNames.txt");
    SDL_Log("filename %s", filename);
    fp = fopen(filename, "w+");
    SDL_assert(fp != NULL);
    for (uint16_t i = ITEM_NAME_SWORD_START; i < ITEM_NAME_SWORD_END; i++) {
        fprintf(fp, "%d %s \n", i, hmget(itemNames, i));
    }
    for (uint16_t i = ITEM_NAME_LANCE_START; i < ITEM_NAME_LANCE_END; i++) {
        fprintf(fp, "%d %s \n", i, hmget(itemNames, i));
    }
    for (uint16_t i = ITEM_NAME_AXE_START; i < ITEM_NAME_AXE_END; i++) {
        fprintf(fp, "%d %s \n", i, hmget(itemNames, i));
    }
    for (uint16_t i = ITEM_NAME_BOW_START; i < ITEM_NAME_BOW_END; i++) {
        fprintf(fp, "%d %s \n", i, hmget(itemNames, i));
    }
    for (uint16_t i = ITEM_NAME_SHIELD_START; i < ITEM_NAME_SHIELD_END; i++) {
        fprintf(fp, "%d %s \n", i, hmget(itemNames, i));
    }
    for (uint16_t i = ITEM_NAME_OFFHAND_START; i < ITEM_NAME_OFFHAND_END; i++) {
        fprintf(fp, "%d %s \n", i, hmget(itemNames, i));
    }
    for (uint16_t i = ITEM_NAME_ELEMENTAL_START; i < ITEM_NAME_ELEMENTAL_END; i++) {
        fprintf(fp, "%d %s \n", i, hmget(itemNames, i));
    }
    for (uint16_t i = ITEM_NAME_MAGIC_START; i < ITEM_NAME_MAGIC_END; i++) {
        fprintf(fp, "%d %s \n", i, hmget(itemNames, i));
    }
    for (uint16_t i = ITEM_NAME_DEMONIC_START; i < ITEM_NAME_DEMONIC_END; i++) {
        fprintf(fp, "%d %s \n", i, hmget(itemNames, i));
    }
    for (uint16_t i = ITEM_NAME_ANGELIC_START; i < ITEM_NAME_ANGELIC_END; i++) {
        fprintf(fp, "%d %s \n", i, hmget(itemNames, i));
    }
    for (uint16_t i = ITEM_NAME_STAFF_START; i < ITEM_NAME_STAFF_END; i++) {
        fprintf(fp, "%d %s \n", i, hmget(itemNames, i));
    }
    for (uint16_t i = ITEM_NAME_CLAW_START; i < ITEM_NAME_CLAW_END; i++) {
        fprintf(fp, "%d %s \n", i, hmget(itemNames, i));
    }
    for (uint16_t i = ITEM_NAME_ITEM_START; i < ITEM_NAME_ITEM_END; i++) {
        fprintf(fp, "%d %s \n", i, hmget(itemNames, i));
    }
    for (uint16_t i = ITEM_NAME_BOOK_START; i < ITEM_NAME_BOOK_END; i++) {
        fprintf(fp, "%d %s \n", i, hmget(itemNames, i));
    }
    fclose(fp);
    memset(filename, 0, sizeof(filename));

    strcat(filename, foldername);
    strcat(filename, "utilities_statNames.txt");
    SDL_Log("filename %s", filename);
    fp = fopen(filename, "w+");
    SDL_assert(fp != NULL);
    for (uint8_t i = ITEM_STAT_START; i < (ITEM_STAT_END - 1); i++) {
        fprintf(fp, "%d %s \n", i, statNames[i]);
    }
    fclose(fp);
    memset(filename, 0, sizeof(filename));

    strcat(filename, foldername);
    strcat(filename, "utilities_sexNames.txt");
    SDL_Log("filename %s", filename);
    fp = fopen(filename, "w+");
    SDL_assert(fp != NULL);
    for (uint8_t i = SEX_F; i < (SEX_M + 1); i++) {
        fprintf(fp, "%d %s \n", i, sexNames[i]);
    }
    fclose(fp);
    memset(filename, 0, sizeof(filename));

    strcat(filename, foldername);
    strcat(filename, "utilities_classNames.txt");
    SDL_Log("filename %s", filename);
    fp = fopen(filename, "w+");
    SDL_assert(fp != NULL);
    for (uint8_t i = UNIT_CLASS_START; i < UNIT_CLASS_END; i++) {
        fprintf(fp, "%d %s \n", i, classNames[i]);
    }
    fclose(fp);
    memset(filename, 0, sizeof(filename));

    strcat(filename, foldername);
    strcat(filename, "utilities_tilenames.txt");
    SDL_Log("filename %s", filename);
    fp = fopen(filename, "w+");
    SDL_assert(fp != NULL);
    for (uint8_t i = TILE_START; i < TILE_END; i++) {
        if (strcmp(hmget(tilenames, i), "") != 0) {
            fprintf(fp, "%d %s \n", i, hmget(tilenames, i));
        }
    }
    fclose(fp);
    memset(filename, 0, sizeof(filename));
}

void utilities_allNames_Load() {
    makeShops();
    makeunitNames();
    maketileNames();
    makecampjobNames();
    makestatNames();
    makesexNames();
    makeunitStates();
    makejsonElementnames();
    Menu_MakeOptionnames();
    Menu_MakeNames();
    makegameStatenames();
    makegamesubStatenames();
    makeclassNames();
    makeitemNames();
    makemapFilenames();
    makeequippableClasscodes();
    makeArmyNames();
    makePromotionbonusstats();
}

void utilities_clean() {

}

#define REGISTER_ENUM(x) temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
strncpy(temp_str, #x, sizeof(#x));\
armyNames[ARMY_##x] = str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')),' ', 2);
char ** armyNames = NULL;
void makeArmyNames() {
    SDL_Log("makeArmyNames");
    char * temp_str = NULL;
    armyNames = calloc(ARMY_END, sizeof(*armyNames));
#include "names/armies.h"
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) if flagsum_isIn(in_typecode, ITEM_TYPE_##x) {\
    temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
    strncpy(temp_str, #x, sizeof(#x));\
    DARR_PUT(type_names, str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')),' ', 2));\
}
char ** utilities_wpnType(uint16_t in_typecode) {
    SDL_Log("utilities_wpnType");
    char ** type_names = DARR_INIT(char *, ITEM_TYPE_END);
    DARR_LEN(type_names) = ITEM_TYPE_END;
    DARR_NUM(type_names) = 0;
    char * temp_str;
#include "names/items_types.h"
    return (type_names);
}
#undef REGISTER_ENUM

struct String_hash * itemNames = NULL;
void makeitemNames() {
    SDL_Log("makeitemNames");
    char * temp_str = NULL;
    hmdefault(itemNames, NAME_ITEM_INVALID);
    uint64_t temp_id;
#define REGISTER_ENUM(x, y) temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
strncpy(temp_str, #x, sizeof(#x));\
temp_id = y;\
hmput(itemNames, temp_id, str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')), ' ', 2));
#include "names/items.h"
}
#undef REGISTER_ENUM

struct String_hash * tilenames = NULL;
void maketileNames() {
    SDL_Log("maketileNames");
    char * temp_str = NULL;
    uint64_t temp_id;
#define REGISTER_ENUM(x, y) temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
strncpy(temp_str, #x, sizeof(#x));\
temp_id = y;\
hmput(tilenames, temp_id, str_camelCase(str_toLower(temp_str), ' ', 2));
#include "names/tiles.h"
    free(temp_str);
    hmdefault(tilenames, "");
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
strncpy(temp_str, #x, sizeof(#x));\
campjobNames[CAMPJOB_##x] = str_camelCase(str_toLower(temp_str), ' ', 2);

char ** campjobNames = NULL;
void makecampjobNames() {
    SDL_Log("makecampjobNames");
    campjobNames = calloc(CAMPJOB_END, sizeof(*campjobNames));
    char * temp_str = NULL;
#include "names/camp_jobs.h"
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
strncpy(temp_str, #x, sizeof(#x));\
unitNames[UNIT_NAME_##x] = str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')), ' ', 2);

char ** unitNames = NULL;
void makeunitNames() {
    SDL_Log("makeunitNames");
    char * temp_str = NULL;
    unitNames = calloc(UNIT_NAME_END, sizeof(*unitNames));
    temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);
    strncpy(temp_str, "", sizeof(""));
    unitNames[0] = temp_str;
#include "names/units_PC.h"
#include "names/units_NPC.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y, z, i) temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
    strncpy(temp_str, #x, sizeof(#x));\
    unitNames[UNIT_NAME_##x] = str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')), ' ', 2);

#include "names/classes.h"
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x)  temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
strncpy(temp_str, #x, sizeof(#x));\
statNames[ITEM_STAT_##x] = str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')), ' ', 2);

char ** statNames = NULL;
void makestatNames() {
    SDL_Log("makestatNames");
    char * temp_str = NULL;
    statNames = calloc(ITEM_STAT_END, sizeof(*statNames));
    temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);
    strncpy(temp_str, "", sizeof(""));
    unitNames[0] = temp_str;
#include "names/items_stats.h"
}

#undef REGISTER_ENUM
char ** sexNames = NULL;
void makesexNames() {
    SDL_Log("makesexNames");
    sexNames = (char **)calloc(SEX_NUM, sizeof(*sexNames));
    sexNames[SEX_F] = (char *) malloc(2);
    strncpy(sexNames[SEX_F], "F\0", 2);
    sexNames[SEX_M] = (char *) malloc(2);
    strncpy(sexNames[SEX_M], "M\0", 2);
}

#define REGISTER_ENUM(x) temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
strncpy(temp_str, #x, sizeof(#x));\
unitStates[UNIT_STATE_##x] =  str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')), ' ', 2);

char ** unitStates = NULL;
void makeunitStates() {
    SDL_Log("makesexNames");
    char * temp_str = NULL;
    unitStates = calloc(UNIT_STATE_END, sizeof(*unitStates));
#include "names/units_states.h"
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y, z, i) case UNIT_CLASS_##x:\
    mvt_type = UNIT_MVT_##z;\
    break;
uint8_t utilities_mvtType(uint8_t in_class_index) {
    // DESIGN: Mvt type now also depends on HORSE/PEGASUS
    uint8_t mvt_type;
    switch (in_class_index) {
#include "names/classes.h"
        default:
            SDL_Log("class index is invalid");
    }
    return (mvt_type);
}
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y, z, i) temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
strncpy(temp_str, #x, sizeof(#x));\
classNames[UNIT_CLASS_##x] = str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')),' ', 2);

char ** classNames =  NULL;
void makeclassNames() {
    SDL_Log("makesexNames");
    char * temp_str = NULL;
    classNames = calloc(UNIT_CLASS_END, sizeof(*classNames));
#include "names/classes.h"
}
#undef  REGISTER_ENUM

#define REGISTER_ENUM(x) if flagsum_isIn(in_skillscode, UNIT_SKILL_##x) {\
    temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
    strncpy(temp_str, #x, sizeof(#x));\
    DARR_PUT(skill_names, str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')),' ', 2));\
}
char ** utilities_skillNames(uint64_t in_skillscode) {
    SDL_Log("utilities_skillNames");
    char ** skill_names = DARR_INIT(char *, UNIT_SKILL_END);
    DARR_LEN(skill_names) = UNIT_SKILL_END;
    DARR_NUM(skill_names) = 0;
    char * temp_str = NULL;
#include "names/skills.h"
    return (skill_names);
}
#undef  REGISTER_ENUM

#define REGISTER_ENUM(x) if flagsum_isIn(in_statecode, UNIT_STATE_##x) {\
    temp_str = (char *) malloc(DEFAULT_BUFFER_SIZE);\
    strncpy(temp_str, #x, sizeof(#x));\
    DARR_PUT(state_names, str_camelCase(str_toLower(str_replaceSingle(temp_str, '_', ' ')),' ', 2));\
}
char ** utilities_unitstateNames(uint32_t in_statecode) {
    SDL_Log("utilities_unitstateNames");
    char * temp_str = NULL;
    char ** state_names = DARR_INIT(char *, UNIT_STATE_END);
    DARR_LEN(state_names) = UNIT_SKILL_END;
    DARR_NUM(state_names) = 0;
#include "names/units_states.h"
    return (state_names);
}
#undef  REGISTER_ENUM

#define REGISTER_ENUM(x, y, z, i) DARR_PUT(equippableClasscodes, y);
uint16_t * equippableClasscodes;
void makeequippableClasscodes() {
    SDL_Log("utilities_unitstateNames");
    equippableClasscodes = DARR_INIT(uint16_t, UNIT_CLASS_END);
    DARR_LEN(equippableClasscodes) = UNIT_CLASS_END;
    DARR_NUM(equippableClasscodes) = 0;
    DARR_PUT(equippableClasscodes, 0);
#include "names/classes.h"
}
#undef  REGISTER_ENUM
