#ifndef MENU_H
#define MENU_H

#include "narrative.h"
#include "enums.h"
#include "events.h"
#include "str.h"
#include "text.h"
#include "unit.h"
#include "SDL2/SDL_ttf.h"

extern SDL_Renderer * Game_renderer;
struct Game;
struct Unit;

typedef struct fillUpBar {
    // Unaligned size 132 bytes
    // Aligned size 132 bytes
    SDL_Texture * texture_outside_3patch;
    SDL_Texture * texture_fill;
    struct Point outside_size_patches;
    struct Point outside_size_pixels;
    struct Point fill_size_patches;
    struct Point fill_size_pixels;
    struct Point fill_patch_pixels;
    struct Point outside_patch_pixels;
    struct Point pos;
    float fill; // between 0 and 1
    SDL_Rect src_fill;
    SDL_Rect src_outside_left;
    SDL_Rect src_outside_mid;
    SDL_Rect src_outside_right;
} fillUpBar;

extern struct fillUpBar fillUpBar_default;

// a menu Manage of some sort draws menu according to the number of menu objects in it, or something.
extern struct MenuOption {
    // Unaligned size 120 bytes
    // Aligned size 112 bytes
    void * data; // pointer to unit/item
    int32_t width; // pixels

    void * data1_childMenu;
    void * data2_childMenu;

    void * data1_childEvent;
    void * data2_childEvent;

    tnecs_entity_t ent_text;

    tnecs_entity_t ent_icon;

    struct Pointf posf_icon; // [0, 1] inside cell
    struct Pointf posf_text; // [0, 1] inside cell
    // menuoption Paddings overrides menu's
    struct Padding pad_text;
    struct Padding pad_icon;
    struct Padding pad_cell;

    uint32_t childEvent;
    uint8_t childMenu;
    bool enabled;

} MenuOption_default;

#define REGISTER_ENUM(x, y, z) extern struct MenuOption MenuOption_##x;
#include "names/menu_options.h"
#undef REGISTER_ENUM

typedef struct Menu {
    // Unaligned size 576 bytes
    // Aligned size 572 bytes
    char name[DEFAULT_BUFFER_SIZE];
    char asset_name[DEFAULT_BUFFER_SIZE];

    SDL_Rect src_midright;
    SDL_Rect src_midleft;
    SDL_Rect src_topright;
    SDL_Rect src_topleft;
    SDL_Rect src_bottomright;
    SDL_Rect src_bottomleft;
    SDL_Rect src_top;
    SDL_Rect src_middle;
    SDL_Rect src_bottom;

    uint8_t id;
    bool enabled;
    uint8_t fontsize;
    uint8_t row_menuoptions;
    uint8_t col_menuoptions;
    uint8_t row_num;
    uint8_t col_num;


    int32_t * col_widths; // should be a singluar col_width for cursor?
    int32_t row_height;

    int32_t max_width_icon;
    int32_t max_width_text;

    int32_t max_width_cols;
    int32_t max_height_rows; // Above this height, menu scrolls or something?

    struct Padding pad_icon;
    struct Padding pad_text;
    struct Padding pad_cell;
    struct Padding pad_menu;

    struct Pointf posf_icon; // [0, 1]
    struct Pointf posf_text; // [0, 1]

    struct Point patch_pixels; //
    struct Point size_patches; //

    struct Point size_pixels; //
    struct Point pos; // pixels

    struct Point cursor_pos; // patches
    struct Point cursor_shift; // pixels

    SDL_Texture * texture_9patch;
    struct MenuOption * menuoptions;


    TTF_Font * font;
} Menu;
extern struct Menu Menu_default;

extern void Menu_Rects_Compute(struct Menu * in_menu);
extern void Menu_optionRects_Compute(struct Menu * in_menu);
extern uint16_t Menu_cellWidth_Compute(tnecs_world_t * in_world, struct Menu * in_menu, size_t row, size_t col);
extern uint16_t Menu_cellsWidths_Compute(tnecs_world_t * in_world, struct Menu * in_menu);

extern uint16_t Menu_Width_Compute(struct Menu * in_menu);
extern uint16_t Menu_Height_Compute(struct Menu * in_menu);
extern uint16_t Menu_Patchsize_Compute(struct Menu * in_menu);
extern uint16_t Menu_rowHeight_Compute(tnecs_world_t * in_world, struct Menu * in_menu);
extern void Menu_Cols_init(struct Menu * in_menu);

extern struct Point Menu_cursorPos_Compute(struct Menu * in_menu);

extern struct Menu ** menus;
extern void loadMenus();
extern void loadMenu(uint8_t in_menuind);
extern void unloadMenu(uint8_t in_menuind);

void Menu_Patches_Draw(struct Menu * in_menu);

extern void Menu_Options_Draw(tnecs_world_t * in_world, struct Menu * in_menu);
extern void Menu_Options_init();

struct MenuOption * Menu_Weapons_options(struct Unit * in_attacker);
struct MenuOption * menu_staffs_options(struct Unit * in_healer);
struct MenuOption * menu_defenders_options(tnecs_entity_t * defenders, uint8_t num_defenders);
struct MenuOption * menu_patients_options(tnecs_entity_t * patients, uint8_t num_patients);
struct MenuOption * menu_talkers_options(tnecs_entity_t * talkers, uint8_t num_talkers);
struct MenuOption * menu_traders_options(tnecs_entity_t * traders, uint8_t num_traders);
struct MenuOption * menu_spectators_options(tnecs_entity_t * spectators, uint8_t num_spectators);
struct MenuOption * menu_rescuees_options(tnecs_entity_t * rescuees, uint8_t num_rescuees);

#endif /* MENU_H */
