
#include "jsonio.h"

struct cJSON * parseJSON(const char * filename) {
    SDL_Log("Parsing JSON file %s", filename);
    PHYSFS_file * fp;
    fp = PHYSFS_openRead(filename);
    SDL_assert(fp);
    struct cJSON * json = NULL;
    uint32_t filelen = PHYSFS_fileLength(fp);
    char filebuffer[filelen];
    PHYSFS_readBytes(fp, filebuffer, filelen);
    PHYSFS_close(fp);
    json = cJSON_ParseWithLength(filebuffer, filelen);
    return (json);
}

void readJSON(const char * filename, const void * struct_ptr) {
    SDL_Log("readJSON file: %s", filename);
    struct cJSON * json = parseJSON(filename);
    uint8_t json_element_id = *(uint8_t *)struct_ptr;
    SDL_Log("Reading JSON element %d %s", json_element_id, jsonElementnames[json_element_id]);
    SDL_assert(json != NULL);
    SDL_assert((json_element_id > 0) && (json_element_id < JSON_END));
    struct cJSON * jelement = cJSON_GetObjectItem(json, jsonElementnames[json_element_id]);
    SDL_assert(jelement != NULL);
    switch (json_element_id) {
        case JSON_TILE:
            Tile_readJSON((struct Tile *)struct_ptr, jelement);
            break;
        case JSON_ITEM:
            Item_readJSON((struct Item *)struct_ptr, jelement);
            break;
        case JSON_WEAPON:
            Weapon_readJSON((struct Weapon *)struct_ptr, jelement);
            break;
        // case JSON_CAMP:
        //     Camp_readJSON((struct Camp *)struct_ptr, jelement);
        //     break;
        case JSON_UNIT:
            Unit_readJSON((struct Unit *)struct_ptr, jelement);
            break;
        case JSON_MAP:
            Map_readJSON((struct Map *)struct_ptr, jelement);
            break;
        // case JSON_SCENE:
        // Scene_readJSON((struct Scene *)struct_ptr, jelement);
        // break;
        case JSON_CONVOY:
            Convoy_readJSON((struct Convoy *)struct_ptr, jelement);
            break;
    }
}

void writeJSON(const char * filename, const void * struct_ptr, const bool append) {
    SDL_Log("Writing Json to %s:", filename);
    PHYSFS_file * fp = NULL;
    struct cJSON * json = cJSON_CreateObject();
    uint8_t json_element_id = *(uint8_t *)struct_ptr;
    SDL_Log("Writing JSON element %s", jsonElementnames[json_element_id]);
    if ((json_element_id > 0) && (json_element_id < JSON_END)) {
        struct cJSON * jelement = cJSON_CreateObject();
        if (append) {
            fp = PHYSFS_openAppend(filename);
        } else {
            fp = PHYSFS_openWrite(filename);
        }
        if (!fp) {
            SDL_Log("Could not open %s for writing\n", filename);
            exit(ERROR_CannotOpenFile);
        } else {
            switch (json_element_id) {
                case JSON_TILE:
                    Tile_writeJSON((struct Tile *)struct_ptr, jelement);
                    break;
                case JSON_ITEM:
                    Item_writeJSON((struct Item *)struct_ptr, jelement);
                    break;
                case JSON_WEAPON:
                    Weapon_writeJSON((struct Weapon *)struct_ptr, jelement);
                    break;
                // case JSON_SCENE:
                // Scene_writeJSON((struct Scene *)struct_ptr, jelement);
                // break;
                // case JSON_CAMP:
                //     Camp_writeJSON((struct Camp *)struct_ptr, jelement);
                //     break;
                case JSON_MAP:
                    Map_writeJSON((struct Map *)struct_ptr, jelement);
                    break;
                case JSON_UNIT:
                    Unit_writeJSON((struct Unit *)struct_ptr, jelement);
                    break;
                case JSON_CONVOY:
                    Convoy_writeJSON((struct Convoy *)struct_ptr, jelement);
                    break;
            }
            cJSON_AddItemToObject(json, jsonElementnames[json_element_id], jelement);
            printJSON(fp, json);
            PHYSFS_close(fp);
            cJSON_Delete(json);
        }
    }   else {
        SDL_Log("JSON element not set");
        exit(ERROR_JSONElementNotSet);
    }
}
