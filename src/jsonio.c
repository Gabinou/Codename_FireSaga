
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

struct cJSON *jsonio_parseJSON(s8 filename) {

    /* Error if file doesn't exist */
    if (!PHYSFS_exists(filename.data)) {
        SDL_Log("File %s does not exist", filename.data);
        SDL_assert(false);
        exit(ERROR_CannotOpenFile);
    }

    /* Error if file can't be read */
    PHYSFS_file *fp = PHYSFS_openRead(filename.data);
    if (fp == NULL) {
        SDL_Log("%s " STRINGIZE(__LINE__), __func__);
        SDL_Log("Could not read JSON file");
        exit(ERROR_CannotOpenFile);
    }

    /* Read the file's bytes */
    uint32_t filelen = PHYSFS_fileLength(fp);
    char filebuffer[filelen];
    PHYSFS_readBytes(fp, filebuffer, filelen);
    PHYSFS_close(fp);

    /* Actually parse the JSON */
    struct cJSON *jfile = cJSON_ParseWithLength(filebuffer, filelen);

    return (jfile);
}

void jsonio_readJSON(s8 filename, void *struct_ptr) {
    SDL_Log("Reading JSON: '%s'", filename.data);

    /* Make mutable filename */
    s8 filename_mut = {0};
    filename_mut = s8cpy(filename_mut, filename);
    SDL_assert(filename_mut.num == filename.num);

    /* Parse the json file */
    struct cJSON *jfile = jsonio_parseJSON(filename);
    if (jfile == NULL) {
        SDL_Log("Could not parse JSON file '%s'", filename.data);
        exit(ERROR_JSONElementNotSet);
    }

    /* Get the json element id */
    u8 jelem_id = *(u8 *)struct_ptr;
    s8 elem_name = jsonElementnames[jelem_id];
    SDL_Log("Reading JSON element %d %s", jelem_id, elem_name.data);
    if (jelem_id >= JSON_END) {
        SDL_Log("JSON element not set");
        exit(ERROR_JSONElementNotSet);
    }

    /* Get the json element */
    struct cJSON *jelement = cJSON_GetObjectItem(jfile, elem_name.data);
    if (jelement == NULL) {
        SDL_Log("JSON element %s does not exist in %s", elem_name.data, filename.data);
        exit(ERROR_JSONElementNotSet);
    }

    /* Actually read the json file */
    if (json_read_funcs[jelem_id] != NULL)
        json_read_funcs[jelem_id](struct_ptr, jelement);

    /* Set json_filename in struct to input filename */
    char *byte_ptr = (char *)struct_ptr;
    char *json_filenamew  = (byte_ptr + JSON_FILENAME_bOFFSET);
    s8 *json_filename   = (s8 *)json_filenamew;
    SDL_assert(json_filename->data == NULL);
    SDL_assert(json_filename->num  == 0);
    SDL_assert(json_filename->len  == 0);
    *json_filename = filename_mut;
    // json_filename->data = filename_mut.data;
    // json_filename->len  = filename_mut.len;
    // json_filename->num  = filename_mut.num;
    SDL_Log("sizeof(char) %d", sizeof(struct Weapon));
    SDL_Log("sizeof(char) %d", sizeof(char));
    SDL_Log("sizeof(u8) %d", sizeof(u8));
    SDL_Log("JSON_FILENAME_bOFFSET %d", JSON_FILENAME_bOFFSET);
    SDL_Log("filename_mut.data %p", filename_mut.data);
    SDL_Log("json_filename->data %p", json_filename->data);
    // SDL_Log("json_elem %d %d", *json_elem, JSON_WEAPON);
    SDL_Log("json_filename '%s' %d %d", json_filename->data, json_filename->len, json_filename->num);
    SDL_Log("json_filename '%s' %d %d", json_filename->data, json_filename->len, json_filename->num);

    /* Clean the jfile */
    if (jfile != NULL)
        cJSON_Delete(jfile);
    SDL_Log("json_filename->data %p", json_filename->data);
    SDL_Log("json_filename '%s' %d %d", json_filename->data, json_filename->len, json_filename->num);
}

void jsonio_writeJSON(s8 filename, const void *struct_ptr, bool append) {
    SDL_Log("%s:", filename.data);

    /* Parse the json file */
    PHYSFS_file *fp = NULL;
    struct cJSON *json = cJSON_CreateObject();

    /* Get the json element id */
    u8 jelem_id = *(u8 *)struct_ptr;
    s8 elem_name = jsonElementnames[jelem_id];
    SDL_Log("Writing JSON element %s", elem_name.data);
    if (jelem_id >= JSON_END) {
        SDL_Log("JSON element not set");
        exit(ERROR_JSONElementNotSet);
    }

    /* Create the json element */
    struct cJSON *jelement = cJSON_CreateObject();

    /* Open the file */
    if (append)
        fp = PHYSFS_openAppend(filename.data);
    else
        fp = PHYSFS_openWrite(filename.data);

    if (!fp) {
        SDL_Log("Could not open %s for writing\n", filename.data);
        exit(ERROR_CannotOpenFile);
    }

    /* Write to the json element */
    if (json_write_funcs[jelem_id] != NULL)
        json_write_funcs[jelem_id](struct_ptr, jelement);

    /* Actually write to the file */
    cJSON_AddItemToObject(json, elem_name.data, jelement);
    Filesystem_printJSON(fp, json);

    /* Clean the file */
    PHYSFS_close(fp);
    if (json != NULL)
        cJSON_Delete(json);

}
