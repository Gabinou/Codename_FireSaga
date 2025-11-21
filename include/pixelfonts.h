#ifndef PIXELFONTS_H
#define PIXELFONTS_H
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
** pixelfonts: Homemade ASCII/UTF-8 pixel art fonts
**
*/

#include "enums.h"

#include "SDL.h"

/* --- CONSTANTS --- */
enum TEXTUREFONT {
    TEXTURE_CHARSET_COL_LEN     =   8,
    TEXTURE_CHARSET_ROW_LEN     =   8,
    TEXTUREFONT_WORD_SPACE      =   0,
    TEXTUREFONT_GLYPH_SPACE     =   1,
    TEXTUREFONT_GLYPH_HEIGHT    =  16,
    TEXTUREFONT_GLYPH_WIDTH     =  16,
};

enum SCROLLING {
    SCROLL_TIME_FAST            =  50,
    SCROLL_TIME_MID             = 100,
    SCROLL_TIME_SLOW            = 200,
};

enum PIXELFONT_OFFSET {
    PIXELNOURS_Y_OFFSET         =   1,
    PIXELNOURS_BIG_Y_OFFSET     =   2,
    PIXELNOURS_GOTHIC_Y_OFFSET  =   0,
};

extern const u8 pixelfont_y_offset[ASCII_GLYPH_NUM];
extern const u8 pixelfont_big_y_offset[ASCII_GLYPH_NUM];

/* --- DEFINITION --- */
/* -- TextLines -- */
/* Lines split into multiple lines by pixelfonts by
**  setting a max pixel length. */
typedef struct TextLines {
    char **lines;
    i32   *lines_len;
    i32    line_num;
    i32    line_len;
} TextLines;
extern const TextLines TextLines_default;

/* -- PixelFont sub-structs -- */
typedef struct PixelFont_Space {
    i32  glyph;    /* [pixels] */
    i32  word;     /* [pixels] */
    i32  line;     /* [pixels] */
} PixelFont_Space;

typedef struct PixelFont_Colors {
    i32 black;
    i32 white;
} PixelFont_Colors;

typedef struct PixelFont_Scroll {
    i32  speed; /* [ms] time until new character is rendered */
    i32  len;   /* [pixels/char] to render */
} PixelFont_Scroll;

typedef struct PixelFont_Glyph {
    Point  size;
    Point *bbox; /* bounding box of each glyph */
    const u8 *y_offset;
    Length len;     /* or num? */
} PixelFont_Glyph;

typedef struct PixelFont_Platform {
    SDL_Texture *texture;  /* ASCII order */
    SDL_Surface *surface;  /* ASCII order */
    SDL_Palette *palette;
} PixelFont_Platform;

/* -- PixelFont inputs -- */
typedef struct PixelFont_In {
    SDL_Renderer *renderer;

    char    *text;
    Point    pos;
    size_t   len;
    b32      scroll;
    b32      centered;
    // TODO: change to i32 for SOTA_TEXT enum
    b32      istexturefont;
} PixelFont_In;

/* -- PixelFont struct -- */
// rename to pxfont
typedef struct PixelFont {
    PixelFont_Glyph     glyph;
    PixelFont_Space     space;
    PixelFont_Colors    colors;
    PixelFont_Scroll    scroll;
    PixelFont_Platform  platform;
} PixelFont;
extern const PixelFont PixelFont_default;
extern const PixelFont TextureFont_default;

/* --- Constructors/Destructors --- */
PixelFont *PixelFont_Alloc(void);
PixelFont *TextureFont_Alloc(u8 rlen, u8 clen);

void PixelFont_Init_tnecs(void *p);
void PixelFont_Init(PixelFont *f);

void PixelFont_Free_tnecs(void *p);
void PixelFont_Free(PixelFont *f, b32 isfree);
void PixelFont_Load(PixelFont *f, SDL_Renderer *r,
                    char *fontname);

/* --- Internals --- */
void PixelFont_Swap_Palette(PixelFont *f, SDL_Renderer *r,
                            i8 w, i8 b);

/* --- Lines --- */
/* -- Counting lines -- */
void TextLines_Free(   TextLines *tl);
void TextLines_Realloc(TextLines *tl, size_t len);

/* -- Splitting lines -- */
TextLines PixelFont_Lines(    PixelFont *f, char *t, size_t l, size_t ll);
TextLines PixelFont_Lines_Len(PixelFont *f, char *t, size_t ll);

i32 PixelFont_Lines_Num(    PixelFont *f,  char *t, size_t l, size_t ll);
i32 PixelFont_Lines_Num_Len(PixelFont *f,  char *t, size_t ll);

i32 PixelFont_NextLine_Break(PixelFont *f,  char *t, i32 pb, size_t l, size_t ll);
i32 NextLine_Start(char *text, i32 pb, i32 cb, size_t l);

/* --- Width in [pixels] --- */
i32 PixelFont_Width(    PixelFont *f,  char *t, size_t l);
i32 PixelFont_Width_Len(PixelFont *f,  char *t);

i32 PixelFont_Glyph_Num(    const PixelFont *f);
i32 PixelFont_Space_Line(   const PixelFont *f);
i32 PixelFont_Space_Word(   const PixelFont *f);
i32 PixelFont_Space_Glyph(  const PixelFont *f);
i32 PixelFont_Scroll_Len(   const PixelFont *f);
Point PixelFont_Glyph_Size( const PixelFont *f);

/* -- Glyph: -- */
void      PixelFont_Glyph_yOffset_W(PixelFont *font,
                                    const u8 *arr);
const u8* PixelFont_Glyph_yOffset_R(const PixelFont *font);
void PixelFont_Compute_Glyph_BBox(PixelFont *font);

/*--- Scrolling --- */
/* Outputs true if the text scrolled this frame */
i32 PixelFont_isScroll(PixelFont *f, u64 time_ns);

/* --- Writing --- */
void PixelFont_Write(PixelFont *f, PixelFont_In in);

#endif /* PIXELFONTS_H */
