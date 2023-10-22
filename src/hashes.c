#include "hashes.h"

/* -- Makers -- */
u64 *supportHashes = NULL;
#define REGISTER_ENUM(x, y) strncpy(temp_str, #x, sizeof(#x));\
    temp_str[sizeof(#x)] = '\0';\
    supportHashes[UNIT_ID_##x] = tnecs_hash_djb2(nstr_toLower(temp_str));
void Hashes_supportHashes() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    char *temp_str = (char *) calloc(DEFAULT_BUFFER_SIZE, 1);
    supportHashes = calloc(UNIT_ID_END, sizeof(*supportHashes));
#include "names/units_PC.h"
#include "names/units_NPC.h"
#undef REGISTER_ENUM
#define REGISTER_ENUM(name, id, x, y, z) strncpy(temp_str, #name, sizeof(#name));\
    supportHashes[UNIT_ID_##name] = tnecs_hash_djb2(nstr_toLower(temp_str));
#include "names/classes.h"
    SDL_free(temp_str);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
#undef REGISTER_ENUM

u64 *itemHashes = NULL;
#define REGISTER_ENUM(x, y) strncpy(temp_str, #x, sizeof(#x));\
    temp_str[sizeof(#x)] = '\0';\
    DARR_PUT(itemHashes, tnecs_hash_djb2(nstr_toLower(temp_str)));
void Hashes_itemHashes() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    char *temp_str = (char *) calloc(DEFAULT_BUFFER_SIZE, 1);
    itemHashes = DARR_INIT(itemHashes, u64, 128);
#include "names/items.h"
    SDL_free(temp_str);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
#undef REGISTER_ENUM

u64 *unitHashes = NULL;
#define REGISTER_ENUM(x, y) strncpy(temp_str, #x, sizeof(#x));\
    temp_str[sizeof(#x)] = '\0';\
    unitHashes[UNIT_ID_##x] = tnecs_hash_djb2(nstr_toLower(temp_str));
void Hashes_unitHashes() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    char *temp_str = (char *) calloc(DEFAULT_BUFFER_SIZE, 1);
    unitHashes = calloc(UNIT_ID_END, sizeof(*unitHashes));
#include "names/units_PC.h"
#include "names/units_NPC.h"
#undef REGISTER_ENUM
#define REGISTER_ENUM(name, id, x, y, z) strncpy(temp_str, #name, sizeof(#name));\
    unitHashes[UNIT_ID_##name] = tnecs_hash_djb2(nstr_toLower(temp_str));
#include "names/classes.h"
    SDL_free(temp_str);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
#undef REGISTER_ENUM

/* -- Hashing -- */
u16 Hashes_supportName2ID(char *name) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(name != NULL);
    size_t len = strlen(name);
    char *temp = SDL_calloc(len + 1, 1);
    strncpy(temp, name, len);
    temp[len] = '\0';
    u16 id = Hashes_supportHash2ID(tnecs_hash_djb2(nstr_toLower(temp)));
    SDL_free(temp);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (id);
}

u16 Hashes_supportHash2ID(u64 hash) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u16 id = 0;
    for (size_t i = 0; i < SUPPORT_TYPE_NUM; i++) {
        if (hash == supportHashes[i]) {
            id = i;
            break;
        }
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (id);
}

u16 Hashes_itemHash2ID(u64 hash) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u16 id = 0;
    for (size_t i = 0; i < UNIT_ID_END; i++) {
        if (hash == itemHashes[i]) {
            id = i;
            break;
        }
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (id);
}

u16 Hashes_itemName2ID(char *name) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(name != NULL);
    size_t len = strlen(name);
    char *temp = SDL_calloc(len + 1, 1);
    strncpy(temp, name, len);
    temp[len] = '\0';
    u16 id = Hashes_itemHash2ID(tnecs_hash_djb2(nstr_toLower(temp)));
    SDL_free(temp);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (id);
}

u16 Hashes_unitName2ID(char *name) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(name != NULL);
    size_t len = strlen(name);
    char *temp = SDL_calloc(len + 1, 1);
    strncpy(temp, name, len);
    temp[len] = '\0';
    u16 id = Hashes_unitHash2ID(tnecs_hash_djb2(nstr_toLower(temp)));
    SDL_free(temp);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (id);
}

u16 Hashes_unitHash2ID(u64 hash) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u16 id = 0;
    for (size_t i = 0; i < UNIT_ID_END; i++) {
        if (hash == unitHashes[i]) {
            id = i;
            break;
        }
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (id);
}

u16 Hashes_shopkeeperName2ID(char *name) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u16 out;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

u16 Hashes_shopkeeperHash2ID(u64 hash) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u16 out;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

u16 Hashes_skillName2ID(char *name) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u16 out;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    return (out);
}

u16 Hashes_skillHash2ID(u64 hash) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u16 out;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    return (out);
}
void Hashes_Free() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    if (unitHashes != NULL) {
        SDL_free(unitHashes);
        unitHashes = NULL;
    }
    if (itemHashes != NULL) {
        DARR_FREE(itemHashes);
        itemHashes = NULL;
    }
    if (supportHashes != NULL) {
        SDL_free(supportHashes);
        supportHashes = NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}