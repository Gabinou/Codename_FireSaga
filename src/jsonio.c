
#include "jsonio.h"
json_read_t  json_read_funcs [JSON_END] = {
    /* JSON_NULL        */ NULL,
    /* JSON_ITEM        */ Item_readJSON,
    /* JSON_TILE        */ Tile_readJSON,
    /* JSON_UNIT        */ Unit_readJSON,
    /* JSON_WEAPON      */ Weapon_readJSON,
    /* JSON_MAP         */ Map_readJSON,
    /* JSON_SCENE       */ Scene_readJSON,
    /* JSON_CONVOY      */ Convoy_readJSON,
    /* JSON_NARRATIVE   */ NULL,
    /* JSON_CAMP        */ Camp_readJSON,
    /* JSON_SPRITESHEET */ Sprite_readJSON,
};

json_write_t json_write_funcs[JSON_END] = {
    /* JSON_NULL        */ NULL,
    /* JSON_ITEM        */ Item_writeJSON,
    /* JSON_TILE        */ Tile_writeJSON,
    /* JSON_UNIT        */ Unit_writeJSON,
    /* JSON_WEAPON      */ Weapon_writeJSON,
    /* JSON_MAP         */ Map_writeJSON,
    /* JSON_SCENE       */ NULL,
    /* JSON_CONVOY      */ Convoy_writeJSON,
    /* JSON_NARRATIVE   */ NULL,
    /* JSON_CAMP        */ Camp_writeJSON,
    /* JSON_SPRITESHEET */ NULL,
};

struct cJSON *jsonio_parseJSON(const char *filename) {
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);
    /* Error if file doesn't exist */
    if (!PHYSFS_exists(filename)) {
        SOTA_Log_Debug("%s " STRINGIZE(__LINE__), __func__);
        SOTA_Log_Debug("File %s does not exist", filename);
        exit(ERROR_CannotOpenFile);
    }

    /* Error if file can't be read */
    PHYSFS_file *fp = PHYSFS_openRead(filename);
    if (fp == NULL) {
        SOTA_Log_Debug("%s " STRINGIZE(__LINE__), __func__);
        SOTA_Log_Debug("Could not read JSON file");
        exit(ERROR_CannotOpenFile);
    }

    /* Read the file's bytes */
    uint32_t filelen = PHYSFS_fileLength(fp);
    char filebuffer[filelen];
    PHYSFS_readBytes(fp, filebuffer, filelen);
    PHYSFS_close(fp);

    /* Actually parse the JSON */
    struct cJSON *jfile = cJSON_ParseWithLength(filebuffer, filelen);
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);
    return (jfile);
}

void jsonio_readJSON(const char *filename, void *struct_ptr) {
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);
    SOTA_Log_Debug("%s:", filename);

    /* Parse the json file */
    struct cJSON *jfile = jsonio_parseJSON(filename);

    /* Get the json element id */
    u8 jelem_id = *(u8 *)struct_ptr;
    char *elem_name = jsonElementnames[jelem_id];
    SOTA_Log_Debug("Reading JSON element %d %s", jelem_id, elem_name);
    SDL_assert(jfile != NULL);
    if (jelem_id >= JSON_END) {
        SDL_Log("JSON element not set");
        exit(ERROR_JSONElementNotSet);
    }

    /* Get the json element */
    struct cJSON *jelement = cJSON_GetObjectItem(jfile, elem_name);
    if (jelement == NULL) {
        SDL_Log("JSON element %s does not exist in %s", elem_name, filename);
        exit(ERROR_JSONElementNotSet);
    }

    SDL_assert(jelement != NULL);

    /* Actually read the json file */
    if (json_read_funcs[jelem_id] != NULL)
        json_read_funcs[jelem_id](struct_ptr, jelement);

    /* Clean the jfile */
    if (jfile != NULL)
        cJSON_Delete(jfile);
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);
}

void jsonio_writeJSON(const char *filename, const void *struct_ptr, bool append) {
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);
    SOTA_Log_Debug("%s:", filename);

    /* Parse the json file */
    PHYSFS_file *fp = NULL;
    struct cJSON *json = cJSON_CreateObject();

    /* Get the json element id */
    u8 jelem_id = *(u8 *)struct_ptr;
    SOTA_Log_Debug("Writing JSON element %s", jsonElementnames[jelem_id]);
    if (jelem_id >= JSON_END) {
        SOTA_Log_Debug("JSON element not set");
        exit(ERROR_JSONElementNotSet);
    }

    /* Create the json element */
    struct cJSON *jelement = cJSON_CreateObject();

    /* Open the file */
    if (append)
        fp = PHYSFS_openAppend(filename);
    else
        fp = PHYSFS_openWrite(filename);

    if (!fp) {
        SOTA_Log_Debug("Could not open %s for writing\n", filename);
        exit(ERROR_CannotOpenFile);
    }

    /* Write to the json element */
    if (json_write_funcs[jelem_id] != NULL)
        json_write_funcs[jelem_id](struct_ptr, jelement);

    /* Actually write to the file */
    cJSON_AddItemToObject(json, jsonElementnames[jelem_id], jelement);
    Filesystem_printJSON(fp, json);

    /* Clean the file */
    PHYSFS_close(fp);
    if (json != NULL)
        cJSON_Delete(json);
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);
}
