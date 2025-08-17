
#include "reload.h"
#include "jsonio.h"
#include "globals.h"

#include "bars/map_hp.h"

#include "game/game.h"

#include "menu/menu.h"

#include "popup/popup.h"

void Reload_Entities_Archetype(struct Game *sota,
                               entity_reload_f reload_func,
                               char *component) {
    tnecs_C component_flag;
    size_t flag_id;

    component_flag  = IES_C_ID_2_A(1, component);
    flag_id         = tnecs_A_id(gl_world, component_flag);

    Reload_Entities(sota, reload_func, flag_id, component);

    /* -- Reload Es for the all component As -- */
    size_t num_A = gl_world->byA.num_A_ids[flag_id];
    for (size_t tsub = 0; tsub < num_A; tsub++) {
        size_t archetype_id = gl_world->byA.subA[flag_id][tsub];
        Reload_Entities(sota, reload_func, archetype_id, component);
    }

}

void Reload_Entities(struct Game *sota,
                     entity_reload_f reload_func,
                     size_t flag_id,
                     char *component) {
    // TODO: component_id input
    // size_t num_Es = gl_world->byA.num_Es[flag_id];

    // for (size_t i = 0; i < num_Es; i++) {
    //     tnecs_E entity = gl_world->byA.Es[flag_id][i];
    //     size_t component_id   = tnecs_C_TYPE2ID(component);
    //     void *struct_ptr      = tnecs_get_component(gl_world, entity, component_id);
    //     reload_func(struct_ptr);
    // }

}

void Reload_JSON(void *struct_ptr) {
    SDL_Log("Reload_JSON");
    char **json_filename  = ((char **)struct_ptr + JSON_FILENAME_bOFFSET);
    jsonio_readJSON(s8_var(*json_filename), struct_ptr);
}

void Reload_Menu(void *struct_ptr) {
    struct Menu *mc     = struct_ptr;

    if (!mc->visible) {
        return;
    }

    // void *menu          = mc->data;

    // TODO
    // char *update_byte   = ((char *)menu + MENU_UPDATE_bOFFSET);
    // b32  *update        = (b32 *)update_byte;
    // *update      = true;
}

void Reload_Popup(void *struct_ptr) {
    struct PopUp *pc    = struct_ptr;

    if (!pc->visible) {
        return;
    }

    // void *popup         = pc->data;

    // TODO
    // char *update_byte   = ((char *)popup + MENU_UPDATE_bOFFSET);
    // b32  *update        = (b32 *)update_byte;
    // *update      = true;
}

void Reload_MapHpBar(void *struct_ptr) {
    struct MapHPBar *map_hp_bar = struct_ptr;
    map_hp_bar->update = true;
}

