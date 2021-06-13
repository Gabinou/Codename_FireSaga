#include "menu.h"

void MenuModuleImport(ecs_world_t * in_world) {
    ECS_MODULE(in_world, MenuModule);
    ECS_COMPONENT(in_world, Menu);
    ECS_SET_COMPONENT(Menu);
    ECS_EXPORT_COMPONENT(Menu);
}

void HUDModuleImport(ecs_world_t * in_world) {
    ECS_MODULE(in_world, HUDModule);
    ECS_COMPONENT(in_world, fillUpBar);
    ECS_SET_COMPONENT(fillUpBar);
    ECS_EXPORT_COMPONENT(fillUpBar);
}

struct fillUpBar fillUpBar_default = {
    // is a component to be icon in MenuOption
    .texture_outside_3patch = NULL,
    .texture_fill = NULL,
    .fill_size_patches = {16, 1},
    .fill_size_pixels = {4, 4},
    .fill_patch_pixels = {8, 8},
    .outside_patch_pixels = {8, 8},
    .pos = {0, 0},
    .fill = 0, // [0, 2] because overfill
};

void fillUpBar_computeRects(struct fillUpBar * in_bar) {
    in_bar->src_outside_left.w = in_bar->outside_patch_pixels.x;
    in_bar->src_outside_left.h = in_bar->outside_patch_pixels.y;
    in_bar->src_outside_left.x = 0;
    in_bar->src_outside_left.y = 0;

    in_bar->src_outside_mid.w = in_bar->outside_patch_pixels.x;
    in_bar->src_outside_mid.h = in_bar->outside_patch_pixels.y;
    in_bar->src_outside_mid.x = in_bar->outside_patch_pixels.x;
    in_bar->src_outside_mid.y = 0;

    in_bar->src_outside_right.w = in_bar->outside_patch_pixels.x;
    in_bar->src_outside_right.h = in_bar->outside_patch_pixels.y;
    in_bar->src_outside_right.x = in_bar->outside_patch_pixels.x * 2;
    in_bar->src_outside_right.y = 0;

    in_bar->src_fill.w = in_bar->fill_patch_pixels.x;
    in_bar->src_fill.h = in_bar->fill_patch_pixels.y;
    in_bar->src_fill.x = 0;
    in_bar->src_fill.y = 0;
}

struct MenuOption MenuOption_default = {
    .width = 32,
    .data = NULL,
    .ent_text = 0,
    .ent_icon = 0,
    .enabled = 1,
    .childMenu = 0,
    .childEvent = 0,
    .posf_icon = {0.1f, 0.5f}, // overrides menu's
    .posf_text = {0.2f, 0.5f}, // overrides menu's
    .pad_text = {4, 0, 0, 4},
    .pad_icon = {1, 1, 1, 1},
    .pad_cell = {1, 1, 1, 1},
};

#define REGISTER_ENUM(x, y, z) struct MenuOption MenuOption_##x;
#include "names/menu_options.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y, z) MenuOption_##x.childMenu = y;\
MenuOption_##x.childEvent = z;
void Menu_Options_init() {
    SDL_Log("Menu_Options_init");
#include "names/menu_options.h"
}
#undef REGISTER_ENUM


struct Menu Menu_default = {
    .enabled = 1,
    .id = 0,
    .patch_pixels = {8, 8},
    .size_patches = {3, 3},
    .pos = {0, 0},

    .max_width_icon = 32,
    .max_width_text = 0,
    .max_width_cols = 256,
    .max_height_rows = 32,
    .posf_icon = {0.1f, 0.5f},
    .posf_text = {0.2f, 0.5f},
    .font = NULL,

    .row_num = 0,
    .col_num = 0,
    .col_widths = NULL,
    .row_height = 16,
    .menuoptions = NULL,
    .texture_9patch = NULL,

    .src_midright = {0, 0, 16, 16},
    .src_midleft = {0, 0, 16, 16},
    .src_topright = {0, 0, 16, 16},
    .src_topleft = {0, 0, 16, 16},
    .src_bottomright = {0, 0, 16, 16},
    .src_bottomleft = {0, 0, 16, 16},
    .src_top = {0, 0, 16, 16},
    .src_middle = {0, 0, 16, 16},
    .src_bottom = {0, 0, 16, 16},
};


void Menu_Cols_init(struct Menu * in_menu) {
    SDL_Log("Menu_Cols_init");
    if (in_menu->col_widths != NULL) {
        arrfree(in_menu->col_widths);
    }

    SDL_assert(in_menu->col_num > 0);
    arrsetlen(in_menu->col_widths, in_menu->col_num);
    for (size_t col; col < in_menu->col_num; col++) {
        in_menu->col_widths[col] = 0;
    }
}

void Menu_optionRects_Compute(struct Menu * in_menu) {
    SDL_Log("Menu_optionRects_Compute");
    SDL_assert(in_menu->menuoptions != NULL);
    for (size_t col; col < in_menu->col_num; col++) {
        for (size_t row; row < in_menu->row_num; row++) {

        }
    }

}

void Menu_Rects_Compute(struct Menu * in_menu) {
    SDL_Log("Menu_optionRects_Compute");
    in_menu->src_topleft.w = in_menu->patch_pixels.x;
    in_menu->src_topleft.h = in_menu->patch_pixels.y;
    in_menu->src_topleft.x = 0;
    in_menu->src_topleft.y = 0;

    in_menu->src_top.w = in_menu->patch_pixels.x;
    in_menu->src_top.h = in_menu->patch_pixels.y;
    in_menu->src_top.x = in_menu->patch_pixels.x;
    in_menu->src_top.y = 0;

    in_menu->src_topright.w = in_menu->patch_pixels.x;
    in_menu->src_topright.h = in_menu->patch_pixels.y;
    in_menu->src_topright.x = in_menu->patch_pixels.x * 2;
    in_menu->src_topright.y = 0;

    in_menu->src_midright.w = in_menu->patch_pixels.x;
    in_menu->src_midright.h = in_menu->patch_pixels.y;
    in_menu->src_midright.x = in_menu->patch_pixels.x * 2;
    in_menu->src_midright.y = in_menu->patch_pixels.y;

    in_menu->src_midleft.w = in_menu->patch_pixels.x;
    in_menu->src_midleft.h = in_menu->patch_pixels.y;
    in_menu->src_midleft.x = 0;
    in_menu->src_midleft.y = in_menu->patch_pixels.y;

    in_menu->src_bottomright.w = in_menu->patch_pixels.x;
    in_menu->src_bottomright.h = in_menu->patch_pixels.y;
    in_menu->src_bottomright.x = in_menu->patch_pixels.x * 2;
    in_menu->src_bottomright.y = in_menu->patch_pixels.y * 2;

    in_menu->src_bottomleft.w = in_menu->patch_pixels.x;
    in_menu->src_bottomleft.h = in_menu->patch_pixels.y;
    in_menu->src_bottomleft.x = 0;
    in_menu->src_bottomleft.y = in_menu->patch_pixels.y * 2;

    in_menu->src_middle.w = in_menu->patch_pixels.x;
    in_menu->src_middle.h = in_menu->patch_pixels.y;
    in_menu->src_middle.x = in_menu->patch_pixels.x;
    in_menu->src_middle.y = in_menu->patch_pixels.y;

    in_menu->src_bottom.w = in_menu->patch_pixels.x;
    in_menu->src_bottom.h = in_menu->patch_pixels.y;
    in_menu->src_bottom.x = in_menu->patch_pixels.x;
    in_menu->src_bottom.y = in_menu->patch_pixels.x * 2;
}


uint16_t Menu_cellsWidths_Compute(ecs_world_t * in_world, struct Menu * in_menu) {
    SDL_Log("Menu_cellsWidths_Compute");
    for (uint8_t col = 0; col < in_menu->col_num; col++) {
        for (uint8_t row = 0; row < in_menu->row_num; row++) {
            SDL_Log("%d %d", row, col);
            Menu_cellWidth_Compute(in_world, in_menu, row, col);
        }
    }
}

uint16_t Menu_cellWidth_Compute_tnecs(tnecs_world_t * in_world, struct Menu * in_menu, size_t row, size_t col) {
    SDL_Log("Menu_cellWidth_Compute_tnecs");
    struct MenuOption cell = in_menu->menuoptions[row + in_menu->row_num * col];
    struct Text * text_ptr = TNECS_GET_COMPONENT(in_world, cell.ent_text, Text);
    struct Sprite * icon_sprite_ptr = TNECS_GET_COMPONENT(in_world, cell.ent_icon, Sprite);
    int32_t temp_width;

    // SDL_Log("text_lines: %s", text_mptr->text_line);
    SDL_assert(in_menu->font != NULL);
    TTF_SizeUTF8(in_menu->font, text_ptr->text_line, &temp_width, NULL);
    // SDL_Log("TTF_SizeUTF8: %d", temp_width);

    if (temp_width > in_menu->max_width_cols) { temp_width = in_menu->max_width_cols; }

    int32_t temp_text_width = cell.pad_text.left + cell.pad_text.right + temp_width;
    int32_t temp_icon_width = icon_sprite_ptr->destrect.w < in_menu->max_width_icon ? icon_sprite_ptr->destrect.w : in_menu->max_width_icon;

    temp_icon_width += cell.pad_icon.left + cell.pad_icon.right;
    cell.width = temp_icon_width + temp_text_width;
    if (cell.width > in_menu->col_widths[col]) { in_menu->col_widths[col] = cell.width; }
}

uint16_t Menu_cellWidth_Compute(ecs_world_t * in_world, struct Menu * in_menu, size_t row, size_t col) {
    SDL_Log("Menu_cellWidth_Compute");
    ECS_IMPORT(in_world, TextModule);
    ECS_IMPORT(in_world, SpriteModule);
    struct MenuOption cell = in_menu->menuoptions[row + in_menu->row_num * col];
    struct Text * text_mptr = ecs_get_mut(in_world, cell.ent_text, Text, NULL);
    struct Sprite * icon_sprite_mptr = ecs_get_mut(in_world, cell.ent_icon, Sprite, NULL);
    int32_t temp_width;

    // SDL_Log("text_lines: %s", text_mptr->text_line);
    SDL_assert(in_menu->font != NULL);
    TTF_SizeUTF8(in_menu->font, text_mptr->text_line, &temp_width, NULL);
    // SDL_Log("TTF_SizeUTF8: %d", temp_width);

    if (temp_width > in_menu->max_width_cols) { temp_width = in_menu->max_width_cols; }

    int32_t temp_text_width = cell.pad_text.left + cell.pad_text.right + temp_width;
    int32_t temp_icon_width = icon_sprite_mptr->destrect.w < in_menu->max_width_icon ? icon_sprite_mptr->destrect.w : in_menu->max_width_icon;

    temp_icon_width += cell.pad_icon.left + cell.pad_icon.right;
    cell.width = temp_icon_width + temp_text_width;
    if (cell.width > in_menu->col_widths[col]) { in_menu->col_widths[col] = cell.width; }
    // SDL_Log("Menu_cellWidth_Compute: cell.width: %d", cell.width);
    // SDL_Log("Menu_cellWidth_Compute: in_menu->col_widths[col]: %d", in_menu->col_widths[col]);
}

uint16_t Menu_rowHeight_Compute_tnecs(tnecs_world_t * in_world, struct Menu * in_menu) {
    SDL_Log("Menu_rowHeight_Compute_tnecs");
    if (in_menu->menuoptions != NULL) {
        struct MenuOption cell = in_menu->menuoptions[0];
        struct Text * text_ptr = TNECS_GET_COMPONENT(in_world, cell.ent_text, Text);
        int32_t temp_height;
        TTF_SizeUTF8(in_menu->font, text_ptr->text_line, NULL, &temp_height);
        in_menu->row_height = temp_height > INT16_MAX ? INT16_MAX : temp_height;
    } else {
        in_menu->row_height = 32;
    }
}


uint16_t Menu_rowHeight_Compute(ecs_world_t * in_world, struct Menu * in_menu) {
    SDL_Log("Menu_rowHeight_Compute");
    ECS_IMPORT(in_world, TextModule);
    if (in_menu->menuoptions != NULL) {
        struct MenuOption cell = in_menu->menuoptions[0];
        Text * text_mptr = ecs_get_mut(in_world, cell.ent_text, Text, NULL);
        int32_t temp_height;
        TTF_SizeUTF8(in_menu->font, text_mptr->text_line, NULL, &temp_height);
        in_menu->row_height = temp_height > INT16_MAX ? INT16_MAX : temp_height;
    } else {
        in_menu->row_height = 32;
    }
}

uint16_t Menu_Patchsize_Compute(struct Menu * in_menu) {
    SDL_Log("Menu_Patchsize_Compute");
    in_menu->size_patches.x = in_menu->size_pixels.x / in_menu->patch_pixels.x + 2;
    in_menu->size_patches.y = in_menu->size_pixels.y / in_menu->patch_pixels.y + 2;
    if (in_menu->size_patches.y < 3) {in_menu->size_patches.y = 3;}
    if (in_menu->size_patches.x < 3) {in_menu->size_patches.x = 3;}
}

uint16_t Menu_Height_Compute(struct Menu * in_menu) {
    SDL_Log("Menu_Height_Compute");
    int32_t temp_height = 0;
    temp_height = in_menu->row_height * in_menu->row_num;
    in_menu->size_pixels.y = temp_height + in_menu->pad_menu.top + in_menu->pad_menu.bottom;
}

uint16_t Menu_Width_Compute(struct Menu * in_menu) {
    SDL_Log("Menu_Height_Compute");
    int32_t temp_width = 0;
    struct MenuOption cell;
    for (uint8_t col = 0; col < in_menu->col_num; col++) {
        temp_width += (int32_t)in_menu->col_widths[col];
        SDL_Log("in_menu->col_widths[col] %d", in_menu->col_widths[col]);
        SDL_Log("temp_width %d", temp_width);
    }
    in_menu->size_pixels.x = temp_width + in_menu->pad_menu.left + in_menu->pad_menu.right;
    SDL_Log("in_menu->size_pixels.x %d", in_menu->size_pixels.x);
}

struct Point Menu_cursorPos_Compute(struct Menu * in_menu) {
    SDL_Log("Menu_cursorPos_Compute");
    struct Point out = Point_default;

    SDL_assert(in_menu->col_widths != NULL);
    if (in_menu->cursor_pos.x > 0) {
        for (size_t i = 0;  i < in_menu->cursor_pos.x; i++) {
            out.x += in_menu->col_widths[i];
        }
    }
    out.x += in_menu->pos.x + in_menu->cursor_shift.x;
    out.y = in_menu->cursor_pos.y * in_menu->row_height + in_menu->pos.y + in_menu->cursor_shift.y;

    return (out);
}

void Menu_Options_Draw_tnecs(tnecs_world_t * in_world, struct Menu * in_menu) {
    struct MenuOption cell;
    Text * text_mptr;
    for (uint8_t col = 0; col < in_menu->col_num; col++) {
        for (uint8_t row = 0; row < in_menu->row_num; row++) {
            text_mptr  =  TNECS_GET_COMPONENT(in_world, cell.ent_text, Text);
            // struct Menu * menu_ptr = TNECS_COMPONENTS_LIST(in_input, Menu);

            // SDL_RenderCopy(Game_renderer, in_menu->texture, &srcrect, &destrect);
            Text_Texture_Make(text_mptr);
            Text_Rects_Pos_Set(text_mptr, in_menu->pos.x + col * in_menu->col_widths[col], in_menu->pos.y + row * in_menu->row_height);
            Text_draw(text_mptr);
            // SDL_Log("text_mptr->text_lines %s", text_mptr->text_line);
        }
    }
}

void Menu_Options_Draw(ecs_world_t * in_world, struct Menu * in_menu) {
    ECS_IMPORT(in_world, TextModule);
    struct MenuOption cell;
    Text * text_mptr;
    for (uint8_t col = 0; col < in_menu->col_num; col++) {
        for (uint8_t row = 0; row < in_menu->row_num; row++) {
            cell = in_menu->menuoptions[row + in_menu->row_num * col];
            text_mptr = ecs_get_mut(in_world, cell.ent_text, Text, NULL);
            // SDL_RenderCopy(Game_renderer, in_menu->texture, &srcrect, &destrect);
            Text_Texture_Make(text_mptr);
            Text_Rects_Pos_Set(text_mptr, in_menu->pos.x + col * in_menu->col_widths[col], in_menu->pos.y + row * in_menu->row_height);
            Text_draw(text_mptr);
            // SDL_Log("text_mptr->text_lines %s", text_mptr->text_line);
            ecs_modified(in_world, cell.ent_text, Text);
        }
    }
}


void Menu_Patches_Draw(struct Menu * in_menu) {
    SDL_Rect destrect;
    SDL_Rect srcrect;
    destrect.w = in_menu->patch_pixels.x;
    destrect.h = in_menu->patch_pixels.y;

    for (size_t patch_y = 0; patch_y < in_menu->size_patches.y; patch_y++) {
        for (size_t patch_x = 0; patch_x < in_menu->size_patches.x; patch_x++) {
            destrect.x = patch_x * in_menu->patch_pixels.x + in_menu->pos.x;
            destrect.y = patch_y * in_menu->patch_pixels.y + in_menu->pos.y;
            if ((patch_x == 0) && (patch_y == 0)) {
                srcrect = in_menu->src_topleft;
            } else if ((patch_x == 0) && (patch_y == (in_menu->size_patches.y - 1))) {
                srcrect = in_menu->src_bottomleft;
            } else if ((patch_x == (in_menu->size_patches.x - 1)) && (patch_y == 0)) {
                srcrect = in_menu->src_topright;
            } else if ((patch_x == (in_menu->size_patches.x - 1)) && (patch_y == (in_menu->size_patches.y - 1))) {
                srcrect = in_menu->src_bottomright;
            } else if ((patch_x > 0) && (patch_y == (in_menu->size_patches.y - 1))) {
                srcrect = in_menu->src_bottom;
            } else if ((patch_x == (in_menu->size_patches.x - 1)) && (patch_y > 0)) {
                srcrect = in_menu->src_midright;
            } else if ((patch_x == 0) && (patch_y > 0)) {
                srcrect = in_menu->src_midleft;
            } else if ((patch_x > 0) && (patch_y == 0)) {
                srcrect = in_menu->src_top;
            } else {
                srcrect = in_menu->src_middle;
            }
            SDL_RenderCopy(Game_renderer, in_menu->texture_9patch, &srcrect, &destrect);
        }
    }
}