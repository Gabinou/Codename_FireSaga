#ifndef TEXT_BUBBLE_H
#define TEXT_BUBBLE_H

#include "enums.h"
#include "globals.h"
#include "nmath.h"
#include "n9patch.h"
#include "pixelfonts.h"
#include "nstr.h"
#include "menu/stats.h"
#include "stb_sprintf.h"
#include "SDL2/SDL.h"

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
// Note: actual position of all objects is in top left corner (o)

// Half around point:
//                           ^
//     x -> false            |     x -> true        y -> false
//                    o-------------
// ------------------ | text bubble | -> Pointer ---------------------
//                     -------------
//                           |                       y -> true
// Note: half = (pos_object > pos_pointer)

/* --- FORWARD DECLARATIONS --- */

enum SOTA_TEXT_BUBBLE {
    TEXT_BUBBLE_TEXTURE_PATCH_WIDTH = 4,
    TEXT_BUBBLE_TEXTURE_CORNER      = 3,
    TEXT_BUBBLE_TEXTURE_MID         = 7,
    TEXT_BUBBLE_PATCH_PIXELS        = 8,
    TEXT_BUBBLE_SCALE               = 4,
};
enum SOTA_TEXT_BUBBLE_N9PATCH {
    TEXT_BUBBLE_PADDING_LEFT   = 7,
    TEXT_BUBBLE_PADDING_TOP    = 4,
    TEXT_BUBBLE_PADDING_BOTTOM = 4,
    TEXT_BUBBLE_PADDING_RIGHT  = 3,
};

enum SOTA_TEXT_BUBBLE_POINTER {
    TEXT_BUBBLE_MIN_WIDTH    =  TEXT_BUBBLE_PATCH_PIXELS * 3,
    TEXT_BUBBLE_POINTER_SIZE =  8,
    TEXT_BUBBLE_NULL         = -1,
    TEXT_BUBBLE_STRAIGHT     =  0,
    TEXT_BUBBLE_DIAGONAL     =  1,
    TEXT_BUBBLE_RENDER_PAD   =  6,
};

struct Text_Bubble_Pointer {
    struct Point pos;
    SDL_RendererFlip flip;
    SDL_Texture *texture;
    SDL_Rect     dstrect;
    double angle;   /* [degree] clockwise */
    int octant;     /* Octant around bubble target is in */
    int index;
    bool half : 1;  /* Half around pointer target is in */
};

struct Text_Bubble {
    int width;
    int height;
    int row_height;
    int line_len_px;
    int line_num;
    char *text;
    struct TextLines lines;
    struct Padding padding;
    /* Origin is top left of bubble*/
    struct Point target; /* Where arrow bubble should point */
    struct PixelFont *pixelfont;
    struct Text_Bubble_Pointer pointer;
    SDL_Texture *texture;
    bool update;
};
extern struct Text_Bubble TextBubble_default;

/* --- Constructor/Destructor --- */
extern void TextBubble_Free(struct Text_Bubble *b);
extern void TextBubble_Load(struct Text_Bubble *b, SDL_Renderer *r, struct n9Patch *n9patch);

/* --- Setters --- */
extern void TextBubble_Set_Text(    struct Text_Bubble *b, const char *t, struct n9Patch *n9patch);
extern void TextBubble_Set_Target(  struct Text_Bubble *b, struct Point t);
extern void TextBubble_Compute_Size(struct Text_Bubble *b, struct n9Patch *n9patch);

/* --- Internals --- */

extern void TextBubble_Pointer_Pos(  struct Text_Bubble *b, struct n9Patch *n9patch);
extern void TextBubble_Pointer_Flip( struct Text_Bubble *b);
extern void TextBubble_Pointer_Angle(struct Text_Bubble *b);
extern int TextBubble_Pointer_Octant(struct Text_Bubble *b);

extern void TextBubble_Set_All(struct Text_Bubble *b, const char *t, struct Point ta, struct n9Patch *n9patch);

/* --- Drawing --- */
extern void TextBubble_Draw(  struct PopUp *p, struct Point pos,
                              SDL_Texture *rt, SDL_Renderer *r);
extern void TextBubble_Update(struct Text_Bubble *b, struct n9Patch *n9patch,
                              SDL_Texture *rt, SDL_Renderer *r);

/* -- Drawing elements -- */
extern void TextBubble_Write(       struct Text_Bubble *b, SDL_Renderer *r);
extern void TextBubble_Pointer_Draw(struct Text_Bubble *b, SDL_Renderer *r);


#endif /* TEXT_BUBBLE_H */