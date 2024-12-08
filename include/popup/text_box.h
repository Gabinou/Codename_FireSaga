#ifndef Text_Box_H
#define Text_Box_H

#include "enums.h"
#include "nmath.h"
#include "n9patch.h"
#include "pixelfonts.h"
#include "nstr.h"
#include "menu/stats.h"
#include "stb_sprintf.h"
#include "SDL.h"

/* --- Text Bubble --- */
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

enum SOTA_Text_Box {
    Text_Box_TEXTURE_PATCH_WIDTH =  4,
    Text_Box_TEXTURE_CORNER      =  3,
    Text_Box_TEXTURE_MID         =  7,
    Text_Box_PATCH_PIXELS        =  8,
    Text_Box_SCALE               =  4,
    Text_Box_DEFAULT_BG_COLOR    = 55, /* White */
    Text_Box_DEFAULT_LINE_COLOR  =  1, /* Black */
};

enum SOTA_Text_Box_N9PATCH {
    Text_Box_PADDING_LEFT        = 7,
    Text_Box_PADDING_TOP         = 4,
    Text_Box_PADDING_BOTTOM      = 4,
    Text_Box_PADDING_RIGHT       = 3,
};

enum SOTA_Text_Box_Tail {
    Text_Box_MIN_WIDTH           =  Text_Box_PATCH_PIXELS * 3,
    Text_Box_Tail_SIZE           =  8,
    Text_Box_NULL                = -1,
    Text_Box_STRAIGHT            =  0,
    Text_Box_DIAGONAL            =  1,
    Text_Box_RENDER_PAD          =  6,
    Text_Box_COPY_PAD            =  5,
    Text_Box_VSCROLL_SPEED       =  1,
    Text_Box_VSCROLL_BOTTOM      =  0,
    Text_Box_VSCROLL_TOP         =  1,
};

typedef struct Text_Box_Tail {
    double              angle; /* [degree] clockwise */
    int                 octant; /* Octant around bubble target is in */
    int                 index;

    SDL_RendererFlip    flip;
    SDL_Surface         *surface;
    SDL_Texture         *texture;
    SDL_Rect            dstrect;
    struct Point        pos;

    b32 half : 1;  /* Half around tail target is in */
} Text_Box_Tail;

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
    struct Text_Box_Tail     tail;
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
extern struct Text_Box TextBubble_default;

/* --- Constructor/Destrgit auctor --- */
void TextBubble_Free(struct Text_Box *b);
void TextBubble_Load(struct Text_Box *b, SDL_Renderer *r, struct n9Patch *n9patch);

/* --- Setters --- */
void TextBubble_Set_Text(    struct Text_Box *b, char *t, struct n9Patch *n9patch);
void TextBubble_Set_Target(  struct Text_Box *b, struct Point t);
void TextBubble_Compute_Size(struct Text_Box *b, struct n9Patch *n9patch);

/* --- Internals --- */
void TextBubble_Tail_Pos(   struct Text_Box *b, struct n9Patch *n9patch);
void TextBubble_Tail_Flip(  struct Text_Box *b);
void TextBubble_Tail_Angle( struct Text_Box *b);
int  TextBubble_Tail_Octant(struct Text_Box *b);

void TextBubble_Set_All(struct Text_Box *b, char *t, struct Point ta,
                        struct n9Patch *n9patch);

/* --- Colors --- */
void TextBubble_Colors_Set(struct Text_Box *b,   i8 bg, i8 line);
void TextBubble_Colors_Swap(struct Text_Box *b, SDL_Renderer *r, struct n9Patch *n9);

/* --- Scrolling --- */
void TextBubble_VScroll(     struct Text_Box *b, SDL_Renderer *r);
void TextBubble_Copy_VScroll(struct Text_Box *b, SDL_Renderer *r, SDL_Texture *rt);

/* --- Drawing --- */
void TextBubble_Draw(  struct PopUp *p,       struct Point pos,
                       SDL_Texture *rt, SDL_Renderer *r);
void TextBubble_Update(struct Text_Box *b, struct n9Patch *n9patch,
                       SDL_Texture *rt, SDL_Renderer *r);

/* -- Drawing elements -- */
void TextBubble_Write(       struct Text_Box *b, SDL_Renderer *r);
void TextBubble_Tail_Draw(   struct Text_Box *b, SDL_Renderer *r);
void TextBubble_VScroll_Draw(struct Text_Box *b, SDL_Renderer *r);


#endif /* Text_Box_H */