#ifndef TEXT_BUBBLE_H
#define TEXT_BUBBLE_H

#include "enums.h"
#include "nmath.h"
#include "n9patch.h"
#include "pixelfonts.h"
#include "nstr.h"
#include "menu/stats.h"
#include "stb_sprintf.h"
#include "SDL.h"

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

enum SOTA_TEXT_BUBBLE {
    TEXT_BUBBLE_TEXTURE_PATCH_WIDTH =  4,
    TEXT_BUBBLE_TEXTURE_CORNER      =  3,
    TEXT_BUBBLE_TEXTURE_MID         =  7,
    TEXT_BUBBLE_PATCH_PIXELS        =  8,
    TEXT_BUBBLE_SCALE               =  4,
    TEXT_BUBBLE_DEFAULT_BG_COLOR    = 55, /* White */
    TEXT_BUBBLE_DEFAULT_LINE_COLOR  =  1, /* Black */
};

enum SOTA_TEXT_BUBBLE_N9PATCH {
    TEXT_BUBBLE_PADDING_LEFT        = 7,
    TEXT_BUBBLE_PADDING_TOP         = 4,
    TEXT_BUBBLE_PADDING_BOTTOM      = 4,
    TEXT_BUBBLE_PADDING_RIGHT       = 3,
};

enum SOTA_Text_Bubble_Tail {
    TEXT_BUBBLE_MIN_WIDTH           =  TEXT_BUBBLE_PATCH_PIXELS * 3,
    Text_Bubble_Tail_SIZE           =  8,
    TEXT_BUBBLE_NULL                = -1,
    TEXT_BUBBLE_STRAIGHT            =  0,
    TEXT_BUBBLE_DIAGONAL            =  1,
    TEXT_BUBBLE_RENDER_PAD          =  6,
    TEXT_BUBBLE_COPY_PAD            =  5,
    TEXT_BUBBLE_VSCROLL_SPEED       =  1,
    TEXT_BUBBLE_VSCROLL_BOTTOM      =  0,
    TEXT_BUBBLE_VSCROLL_TOP         =  1,
};

struct Text_Bubble_Tail {
    double              angle; /* [degree] clockwise */
    int                 octant; /* Octant around bubble target is in */
    int                 index;

    SDL_RendererFlip    flip;
    SDL_Surface         *surface;
    SDL_Texture         *texture;
    SDL_Rect            dstrect;
    struct Point        pos;

    bool half : 1;  /* Half around tail target is in */
};

struct Text_Bubble {
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
    struct Text_Bubble_Tail     tail;
    SDL_Surface                *surface;
    SDL_Texture                *texture;
    SDL_Texture                *texture_vscroll;
    struct PixelFont           *pixelfont;

    i8 old_bg_color;
    i8 old_line_color;
    i8 bg_color;
    i8 line_color;

    bool scroll         : 1;
    bool animating      : 1;
    bool vscroll_dir    : 1;
    bool vscroll_anim   : 1;
};
extern struct Text_Bubble TextBubble_default;

/* --- ructor/Destructor --- */
void TextBubble_Free(struct Text_Bubble *b);
void TextBubble_Load(struct Text_Bubble *b, SDL_Renderer *r, struct n9Patch *n9patch);

/* --- Setters --- */
void TextBubble_Set_Text(    struct Text_Bubble *b, char *t, struct n9Patch *n9patch);
void TextBubble_Set_Target(  struct Text_Bubble *b, struct Point t);
void TextBubble_Compute_Size(struct Text_Bubble *b, struct n9Patch *n9patch);

/* --- Internals --- */
void TextBubble_Tail_Pos(   struct Text_Bubble *b, struct n9Patch *n9patch);
void TextBubble_Tail_Flip(  struct Text_Bubble *b);
void TextBubble_Tail_Angle( struct Text_Bubble *b);
int  TextBubble_Tail_Octant(struct Text_Bubble *b);

void TextBubble_Set_All(struct Text_Bubble *b, char *t, struct Point ta,
                        struct n9Patch *n9patch);

/* --- Colors --- */
void TextBubble_Colors_Set(struct Text_Bubble *b,   i8 bg, i8 line);
void TextBubble_Colors_Swap(struct Text_Bubble *b, SDL_Renderer *r, struct n9Patch *n9);

/* --- Scrolling --- */
void TextBubble_VScroll(     struct Text_Bubble *b, SDL_Renderer *r);
void TextBubble_Copy_VScroll(struct Text_Bubble *b, SDL_Renderer *r, SDL_Texture *rt);

/* --- Drawing --- */
void TextBubble_Draw(  struct PopUp *p,       struct Point pos,
                       SDL_Texture *rt, SDL_Renderer *r);
void TextBubble_Update(struct Text_Bubble *b, struct n9Patch *n9patch,
                       SDL_Texture *rt, SDL_Renderer *r);

/* -- Drawing elements -- */
void TextBubble_Write(       struct Text_Bubble *b, SDL_Renderer *r);
void TextBubble_Tail_Draw(   struct Text_Bubble *b, SDL_Renderer *r);
void TextBubble_VScroll_Draw(struct Text_Bubble *b, SDL_Renderer *r);


#endif /* TEXT_BUBBLE_H */