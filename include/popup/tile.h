#ifndef PT_H
#define PT_H

#include "enums.h"
#include "n9patch.h"
#include "game/game.h"
#include "map/tiles.h"
#include "popup/popup.h"
#include "pixelfonts.h"
#include "SDL.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATIONS --- */
struct Settings;
struct cJSON;

enum POPUP_TILE {
    PT_TEXT_SCALE_X     =  1,
    PT_TEXT_SCALE_Y     =  1,
    PT_N9PATCH_SCALE_X  =  4,
    PT_N9PATCH_SCALE_Y  =  4,
    PT_PATCH_X_SIZE     =  8,
    PT_PATCH_Y_SIZE     =  6,
    PT_PATCH_PIXELS     =  8,
    PT_HEADER_X         = 22,
    PT_HEADER_Y         =  0,
    PT_HEADER_W         = 24,
    PT_HEADER_H         =  5,

    PT_LINE_HEIGHT      = 8,

    PT_NAME_X           = 24,
    PT_NAME_Y           =  7,

    PT_PROT_X           = 13,
    PT_PROT_Y           = PT_NAME_Y + PT_LINE_HEIGHT + 1,

    PT_PROT_STAT_X      = PT_PROT_X + 32,
    PT_PROT_STAT_Y      = PT_PROT_Y -  1,

    PT_AVOID_X          =  8,
    PT_AVOID_Y          = PT_PROT_Y + PT_LINE_HEIGHT + 2,
    PT_AVOID_STAT_X     = PT_PROT_STAT_X,
    PT_AVOID_STAT_Y     = PT_AVOID_Y - 1,

    PT_HEAL_X           = 11,
    PT_HEAL_Y           = PT_AVOID_Y + PT_LINE_HEIGHT + 2,
    PT_HEAL_STAT_X      = PT_PROT_STAT_X,
    PT_HEAL_STAT_Y      = PT_HEAL_Y - 1,

    PT_OFFSET_FACTOR_X  = 200,
    PT_OFFSET_FACTOR_Y  = 200,
};

#define POPUP_POS(xory, offset) (offset * n9patch->scale.xory)

typedef struct PopUp_Tile {
    b32 update;
    struct Tile *tile;
    SDL_Texture *texture;
    SDL_Texture *texture_header;
    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_big;
    u32 corner; /* SOTA_DIRECTION_DIAGONAL */
    struct Point offset;
    struct Point cursor_limit_min;
    struct Point cursor_limit_max;
} PopUp_Tile;
extern struct PopUp_Tile PopUp_Tile_default;

/* --- ructor/Destructor --- */
void PopUp_Tile_Free(struct PopUp_Tile *pt);
void PopUp_Tile_Load(struct PopUp_Tile *pt, SDL_Renderer *r, struct n9Patch *n9);

/* --- Positioning --- */
struct Point PopUp_Tile_Center_Name(struct PopUp_Tile *pt, struct n9Patch *n9patch,
                                    char *n, size_t len);
struct Point PopUp_Tile_Center_Heal( struct PopUp_Tile *pt, char *nb);
struct Point PopUp_Tile_Center_Prot( struct PopUp_Tile *pt, char *mb);
struct Point PopUp_Tile_Center_Avoid(struct PopUp_Tile *pt, char *nb);

void PopUp_Tile_Limits(struct PopUp *p, struct PopUp_Tile *pt, struct Settings *s);
struct Point PopUp_Tile_Offset(struct PopUp_Tile *pt, struct Settings *s);
struct Point PopUp_Tile_Position(struct PopUp *p, struct PopUp_Tile *pt,
                                 struct n9Patch *n9patch, struct Settings *settings,
                                 struct Point *tilemap_pos, i8 direction);

/* --- Setters --- */
void PopUp_Tile_Set(struct PopUp_Tile *pt, struct Game *sota);

/* --- Drawing --- */
void PopUp_Tile_Draw(struct PopUp *p, struct Point pos,
                     SDL_Texture *rt, SDL_Renderer *r);
void PopUp_Tile_Update(struct PopUp_Tile *pt, struct n9Patch *n9patch,
                       SDL_Texture *rt, SDL_Renderer *r);

#endif /* PT_H */
