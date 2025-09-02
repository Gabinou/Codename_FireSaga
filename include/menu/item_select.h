#ifndef ISM_H
#define ISM_H
/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** ItemSelectMenu: Menu to select ANY item
**
*/

#include "enums.h"
#include "structs.h"

/* Requirements
**  - Draw all items in equipment
**      - Dynamic size
**  - Draw hands for items currently equipped
**  - Unit equipment does not change after unit is set */

/* --- FORWARD DECLARATIONS --- */
struct Map;
struct Menu;
struct n9Patch;
struct n4Directions;

enum ISM_HANDS {
    ISM_HANDS_TILESIZE          =  16,
    ISM_HANDS_NONE              =  -1,
    ISM_HANDS_BIG_L             =   0,
    ISM_HANDS_SMALL_L           =   1,
    ISM_HANDS_SMALL_R           =   2,
    ISM_HANDS_BIG_R             =   3,
    ISM_HANDL_X                 =  10,
    ISM_HANDL_Y                 =   6,
    ISM_HANDR_X                 = 150,
    ISM_HANDR_Y                 =   6,
    ISM_HAND_SMALLX_OFFSET      =   2,
    ISM_HAND_SMALLY_OFFSET      =   3,
    ISM_STRONGHAND_Y_OFFSET     =   7,
    ISM_WEAKHAND_Y_OFFSET       =   6,
    ISM_TWOHAND_Y_OFFSET        =   2,
};

enum ISM_MENU {
    ISM_PATCH_X_PIXELS          =  8,
    ISM_PATCH_Y_PIXELS          =  8,
    ISM_PATCH_X_SIZE            = 23,
    ISM_PATCH_Y_SIZE            = 12,
    ISM_N9PATCH_SCALE_X         =  4,
    ISM_N9PATCH_SCALE_Y         =  4,

    ISM_ROW_HEIGHT              = ASCII_GLYPH_HEIGHT + 6,

    ISM_ITEM_MIN_WIDTH          = 60,
    ISM_HEADER_LEFT             = 12,
    ISM_HEADER_RIGHT            = 12,
    ISM_LEFT_OF_TEXT            = 38,
    ISM_RIGHT_OF_TEXT           = 48,
    ISM_BOTTOM_OF_TEXT          =  8,
    ISM_TOP_OF_TEXT             =  8,
    ISM_HAND_RIGHT_OFFSET       = 42,
    ISM_HAND_RIGHT_OFFSET_SMALL = 38,

    ISM_SPACE                   =  4,

    ISM_ICONTOTEXT_W            = 15,
    ISM_HIGHLIGHT_EXTRA_W       =  3,

    ISM_ICON_W                  = 12,
    ISM_ICON_H                  = 12,
    ISM1_X_OFFSET               = ISM_SPACE + ISM_HANDS_TILESIZE,
    ISM1_Y_OFFSET               =  7,

    ISM1_DURA_X_OFFSET          = ISM1_X_OFFSET + ISM_ICON_W,
    ISM1_DURA_Y_OFFSET          = ISM1_Y_OFFSET,

    ISM1_NAME_X_OFFSET          = ISM1_DURA_X_OFFSET + 8,
    ISM1_NAME_Y_OFFSET          = ISM1_Y_OFFSET,

    ISM2_X_OFFSET               = ISM1_X_OFFSET,
    ISM3_X_OFFSET               = ISM1_X_OFFSET,
    ISM4_X_OFFSET               = ISM1_X_OFFSET,
    ISM5_X_OFFSET               = ISM1_X_OFFSET,
    ISM6_X_OFFSET               = ISM1_X_OFFSET,
    ISM2_Y_OFFSET               = ISM1_Y_OFFSET + ISM_ICON_H + 2,
    ISM3_Y_OFFSET               = ISM2_Y_OFFSET + ISM_ICON_H + 2,
    ISM4_Y_OFFSET               = ISM3_Y_OFFSET + ISM_ICON_H + 2,
    ISM5_Y_OFFSET               = ISM4_Y_OFFSET + ISM_ICON_H + 2,
    ISM6_Y_OFFSET               = ISM5_Y_OFFSET + ISM_ICON_H + 2,
};

enum ISM_ELEMS {
    ISM_ELEM_NULL   = -1,
    ISM_ELEM_ITEM1  =  0,
    ISM_ELEM_ITEM2,
    ISM_ELEM_ITEM3,
    ISM_ELEM_ITEM4,
    ISM_ELEM_ITEM5,
    ISM_ELEM_ITEM6,
    ISM_ELEMS_NUM,
};

typedef struct ItemSelectMenu {
    b32 update;
    struct Point pos; /* [pixels] */
    SDL_Texture *texture;
    SDL_Texture *texture_hands;

    tnecs_E _unit_E;
    struct Map *map;

    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_big;

    /* Number of items in equipment */
    i32 _num;
    i32 _max_width;

    i32 selected_eq;

    u8 black;
    u8 white;
} ItemSelectMenu;
extern const ItemSelectMenu ItemSelectMenu_default;

/* --- Constructors/Destructors --- */
ItemSelectMenu *ItemSelectMenu_Alloc(void);
void ItemSelectMenu_Free(ItemSelectMenu *ism);

/* --- Management --- */
void ItemSelectMenu_Load(ItemSelectMenu *sm,
                         SDL_Renderer   *r,
                         struct n9Patch *n9);

tnecs_E ItemSelectMenu_Select(    ItemSelectMenu *ism, int s);
tnecs_E ItemSelectMenu_Selected(  ItemSelectMenu *ism);

void ItemSelectMenu_Size(   ItemSelectMenu  *ism,
                            struct n9Patch  *n9);

void ItemSelectMenu_Unit(   Menu *mc,
                            tnecs_E unit);

Point ItemSelectMenu_Name_Pos(  ItemSelectMenu  *ism,
                                n9Patch         *n9,
                                i32              line);
Point ItemSelectMenu_Dura_Pos(  ItemSelectMenu  *ism,
                                n9Patch         *n9,
                                i32              line);

/* --- Elem --- */
i32 ItemSelectMenu_Elem_Move(struct Menu *mc, i32 direction);
void ItemSelectMenu_Elem_Pos(   ItemSelectMenu  *ism,
                                Menu            *mc);

/* --- Drawing --- */
void ItemSelectMenu_Draw(   struct  Menu    *mc,
                            SDL_Texture     *rt,
                            SDL_Renderer    *r);
void ItemSelectMenu_Update( struct ItemSelectMenu   *ism,
                            struct n9Patch          *n9,
                            SDL_Texture             *rt,
                            SDL_Renderer            *r);

#endif /* ISM_H */
