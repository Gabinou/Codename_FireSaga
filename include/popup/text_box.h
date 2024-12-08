#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include "enums.h"
#include "nmath.h"
#include "n9patch.h"
#include "pixelfonts.h"
#include "nstr.h"
#include "menu/stats.h"
#include "stb_sprintf.h"
#include "SDL.h"

/* --- Text Box --- */
/* -- Design objectives -- */
//  - Render text for player to read
//      - For SceneTalk, etc.
//      - Wrapping text, multiple lines
//      - n9patch
//      - Speaker box OR tail pointing to speaker


/* Octants around text bubble:
//      X -> BELOW      X -> EQUAL         X -> ABOVE
//                    |             |
//   ..._DIAGONAL_TL  |   ..._TOP   |  ..._DIAGONAL_TR      Y -> ABOVE
//                    |             |
//  ------------------o------------- ---------------------
//       ..._LEFT     | text bubble | SOTA_DIRECTION_RIGHT  Y -> EQUAL
//  ------------------ ------------- ---------------------
//                    |             |
//   ..._DIAGONAL_BL  | ..._BOTTOM  | ..._DIAGONAL_BR       Y -> BELOW
//                    |             |
// Note: origin, position of all objects is in top left corner (o)

// Half around point:
//                     ^
//     x -> false      |     x -> true        y -> false
//                    o-------------
// ------------------ | text bubble | -> tail -------------
//                     -------------
//                     |                      y -> true
// Note: half = (pos_object > pos_tail)

/* --- FORWARD DECLARATIONS --- */

enum SOTA_TEXT_BOX {
    TEXT_BOX_TEXTURE_PATCH_WIDTH =  4,
    TEXT_BOX_TEXTURE_CORNER      =  3,
    TEXT_BOX_TEXTURE_MID         =  7,
    TEXT_BOX_PATCH_PIXELS        =  8,
    TEXT_BOX_SCALE               =  4,
    TEXT_BOX_DEFAULT_BG_COLOR    = 55, /* White */
    TEXT_BOX_DEFAULT_LINE_COLOR  =  1, /* Black */
};

enum SOTA_TEXT_BOX_N9PATCH {
    TEXT_BOX_PADDING_LEFT        = 7,
    TEXT_BOX_PADDING_TOP         = 4,
    TEXT_BOX_PADDING_BOTTOM      = 4,
    TEXT_BOX_PADDING_RIGHT       = 3,
};

enum SOTA_TEXT_BOX_Tail {
    TEXT_BOX_MIN_WIDTH           =  TEXT_BOX_PATCH_PIXELS * 3,
    TEXT_BOX_TAIL_SIZE           =  8,
    TEXT_BOX_NULL                = -1,
    TEXT_BOX_STRAIGHT            =  0,
    TEXT_BOX_DIAGONAL            =  1,
    TEXT_BOX_RENDER_PAD          =  6,
    TEXT_BOX_COPY_PAD            =  5,
    TEXT_BOX_VSCROLL_SPEED       =  1,
    TEXT_BOX_VSCROLL_BOTTOM      =  0,
    TEXT_BOX_VSCROLL_TOP         =  1,
};

typedef struct TEXT_BOX_Tail {
    double              angle; /* [degree] clockwise */
    int                 octant; /* Octant around bubble target is in */
    int                 index;

    SDL_RendererFlip    flip;
    SDL_Surface         *surface;
    SDL_Texture         *texture;
    SDL_Rect            dstrect;
    struct Point        pos;

    b32 half : 1;  /* Half around tail target is in */
} TEXT_BOX_Tail;

typedef struct Text_Box {
    b32 update;
    s8       text;
    int      width;
    int      height;
    int      row_height;
    int      line_len_px;
    int      line_num_max;
    int      vscroll;
    int      vscroll_speed;

    struct TextLines            lines;
    struct Padding              padding;
    struct Point                target; /* Where to point at */
    struct TEXT_BOX_Tail     tail;
    SDL_Surface                *surface;
    SDL_Texture                *texture;
    SDL_Texture                *texture_vscroll;
    struct PixelFont           *pixelfont;

    /* -- Color swapping: mostly useful for black and white text bubble. -- */
    // Line color: normally black
    // BG color: normally white
    i8 old_bg_color;
    i8 old_line_color;
    i8 bg_color;
    i8 line_color;

    b32 scroll          : 1;
    b32 animating       : 1;
    b32 vscroll_dir     : 1;
    b32 vscroll_anim    : 1;
    b32 enable_tail     : 1;
    SDL_Palette *palette;
} Text_Box;
extern struct Text_Box TEXT_BOX_Default;

/* --- Constructor/Destrgit auctor --- */
void Text_Box_Free(struct Text_Box *b);
void Text_Box_Load(struct Text_Box *b, SDL_Renderer *r, struct n9Patch *n9patch);

/* --- Setters --- */
void TEXT_BOX_Set_Text(    struct Text_Box *b, char *t, struct n9Patch *n9patch);
void TEXT_BOX_Set_Target(  struct Text_Box *b, struct Point t);
void TEXT_BOX_Compute_Size(struct Text_Box *b, struct n9Patch *n9patch);

/* --- Internals --- */
void TEXT_BOX_Tail_Pos(   struct Text_Box *b, struct n9Patch *n9patch);
void TEXT_BOX_Tail_Flip(  struct Text_Box *b);
void TEXT_BOX_Tail_Angle( struct Text_Box *b);
int  TEXT_BOX_Tail_Octant(struct Text_Box *b);

void TEXT_BOX_Set_All(struct Text_Box *b, char *t, struct Point ta,
                      struct n9Patch *n9patch);

/* --- Colors --- */
void TEXT_BOX_Colors_Set(struct Text_Box *b,   i8 bg, i8 line);
void TEXT_BOX_Colors_Swap(struct Text_Box *b, SDL_Renderer *r, struct n9Patch *n9);

/* --- Scrolling --- */
void TEXT_BOX_VScroll(     struct Text_Box *b, SDL_Renderer *r);
void TEXT_BOX_Copy_VScroll(struct Text_Box *b, SDL_Renderer *r, SDL_Texture *rt);

/* --- Drawing --- */
void TEXT_BOX_Draw(  struct PopUp *p,       struct Point pos,
                     SDL_Texture *rt, SDL_Renderer *r);
void Text_Box_Update(struct Text_Box *b, struct n9Patch *n9patch,
                     SDL_Texture *rt, SDL_Renderer *r);

/* -- Drawing elements -- */
void Text_Box_Write(       struct Text_Box *b, SDL_Renderer *r);
void TEXT_BOX_Tail_Draw(   struct Text_Box *b, SDL_Renderer *r);
void TEXT_BOX_VScroll_Draw(struct Text_Box *b, SDL_Renderer *r);


#endif /* TEXT_BOX_H */