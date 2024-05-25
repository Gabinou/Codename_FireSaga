
#include "reload.h"

void Reload_Entities_Archetype(struct Game *sota, entity_reload_f reload_func,
                               char *component) {
    tnecs_component component_flag;
    size_t flag_id;

    component_flag  = tnecs_component_names2typeflag(sota->world, 1, component);
    flag_id         = tnecs_typeflagid(sota->world, component_flag);

    Reload_Entities(sota, reload_func, flag_id, component);

    /* -- Reload entities for the all component archetypes -- */
    size_t num_archetypes = sota->world->num_archetype_ids[flag_id];
    for (size_t tsub = 0; tsub < num_archetypes; tsub++) {
        size_t archetype_id = sota->world->archetype_id_bytype[flag_id][tsub];
        Reload_Entities(sota, reload_func, archetype_id, component);
    }

}

void Reload_Entities(struct Game *sota, entity_reload_f reload_func, size_t flag_id,
                     char *component) {
    size_t num_entities = sota->world->num_entities_bytype[flag_id];

    for (size_t i = 0; i < num_entities; i++) {
        tnecs_entity entity = sota->world->entities_bytype[flag_id][i];
        size_t component_id   = tnecs_component_name2id(sota->world, component);
        void *struct_ptr      = tnecs_entity_get_component(sota->world, entity, component_id);
        reload_func(struct_ptr);
    }

}

void Reload_JSON(void *struct_ptr) {
    char **json_filename  = ((char **)struct_ptr + JSON_FILENAME_bOFFSET);
    jsonio_readJSON(s8_var(*json_filename), struct_ptr);
}

void Reload_Menu(void *struct_ptr) {
    struct Menu *mc     = struct_ptr;

    if (!mc->visible) {
        return;
    }

    void *menu          = mc->data;

    char *update_byte   = ((char *)menu + MENU_UPDATE_bOFFSET);
    b32  *update        = (b32 *)update_byte;

    *update      = true;
}

void Reload_Popup(void *struct_ptr) {
    struct PopUp *pc    = struct_ptr;

    if (!pc->visible) {
        return;
    }

    void *popup         = pc->data;

    char *update_byte   = ((char *)popup + MENU_UPDATE_bOFFSET);
    b32  *update        = (b32 *)update_byte;

    *update      = true;
}

void Reload_MapHpBar(void *struct_ptr) {
    struct MapHPBar *map_hp_bar = struct_ptr;
    map_hp_bar->update = true;
}

