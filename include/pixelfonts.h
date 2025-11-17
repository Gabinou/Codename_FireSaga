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
    TEXTURE_CHARSET_COL_LEN     =  8,
    TEXTURE_CHARSET_ROW_LEN     =  8,
    TEXTUREFONT_WORD_SPACE      =  0,
    TEXTUREFONT_GLYPH_SPACE     =  1,
    TEXTUREFONT_GLYPH_HEIGHT    = 16,
    TEXTUREFONT_GLYPH_WIDTH     = 16,
};

enum SCROLLING {
    SCROLL_TIME_FAST            =  50,
    SCROLL_TIME_MID             = 100,
    SCROLL_TIME_SLOW            = 200,
};

enum PIXELFONT_OFFSET {
    PIXELNOURS_Y_OFFSET         = 1,
    PIXELNOURS_BIG_Y_OFFSET     = 2,
    PIXELNOURS_GOTHIC_Y_OFFSET  = 0,
};

extern const u8 pixelfont_y_offset[ASCII_GLYPH_NUM];
extern const u8 pixelfont_big_y_offset[ASCII_GLYPH_NUM];

/* --- DEFINITION --- */
typedef struct TextLines {
    char **lines;
    i32   *lines_len;
    i32    line_num;
    i32    line_len;
} TextLines;
extern const TextLines TextLines_default;

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
    Point *bbox;
    const u8 *y_offset;
} PixelFont_Glyph;

typedef struct PixelFont {
    /* TODO clean */
    SDL_Texture *texture;  /* ASCII order */
    SDL_Surface *surface;  /* ASCII order */
    SDL_Texture *write_texture;
    SDL_Palette *palette;

    /* bbox: bounding box of each glyph.
    **  - Assumin each glyph
    **  Computed with PixelFont_Compute_Glyph_BBox.
    **  TODO:   make constants arrays for used fonts,
    **          like y_offset. */
    PixelFont_Glyph glyph;
    u8 *glyph_bbox_width;
    u8 *glyph_bbox_height;

    u8  glyph_width;
    u8  glyph_height;
    const u8 *y_offset; /* for each glyph */

    /* Text Scrolling */
    PixelFont_Scroll scroll;
    // i32  scroll_speed;   /* [ms] time until new character is rendered */
    // i32  scroll_len;   /* [pixels/char] to render */

    PixelFont_Space space;
    i8  glyph_space;    /* [pixels] */
    u8  word_space;     /* [pixels] */
    i8  linespace;      /* [pixels] new line */

    Length len;

    PixelFont_Colors colors;
    // i8 black;
    // i8 white;

    b32 istexturefont;
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
struct TextLines PixelFont_Lines(    PixelFont *f, char *t, size_t l, size_t ll);
struct TextLines PixelFont_Lines_Len(PixelFont *f, char *t, size_t ll);

i32 PixelFont_Lines_Num(    PixelFont *f,  char *t, size_t l, size_t ll);
i32 PixelFont_Lines_Num_Len(PixelFont *f,  char *t, size_t ll);

i32 PixelFont_NextLine_Break(PixelFont *f,  char *t, i32 pb, size_t l, size_t ll);
i32 NextLine_Start(char *text, i32 pb, i32 cb, size_t l);

/* --- Width in [pixels] --- */
i32 PixelFont_Width(    PixelFont *f,  char *t, size_t l);
i32 PixelFont_Width_Len(PixelFont *f,  char *t);

i32 PixelFont_Glyph_Num(const PixelFont *f);

/* - Glyph_BoundingBox: - */
// Note: Detect bounding box of each glyph of the font by looking at pixel values
void PixelFont_Compute_Glyph_BBox(PixelFont *font);

/*--- Scrolling --- */
/* Outputs true if the text scrolled this frame */
i32 PixelFont_isScroll(PixelFont *f, u64 time_ns);

/* --- Writing --- */
// TEXTURE FONT NOTE:
// For PixelFont_Write be usable for Texturefont,
// skip glyph 32, cause it is reserved for SPACE
// col_len 16: 1st cell in 3rd row / col_len 8: 1st cell in 5rd row
void PixelFont_Write(             PixelFont *f, SDL_Renderer *r, char *t,
                                  size_t len, u32 px, u32 py);
void PixelFont_Write_Len(         PixelFont *f, SDL_Renderer *r, char *t,
                                  u32 px, u32 py);
void PixelFont_Write_Scroll(      PixelFont *f, SDL_Renderer *r, char *t,
                                  u32 px, u32 py);
void PixelFont_Write_Centered(    PixelFont *font, SDL_Renderer *rdr,
                                  char *text, size_t len, u32 x, u32 y);
void PixelFont_Write_Centered_Len(PixelFont *f, SDL_Renderer *r, char *t,
                                  u32 px, u32 py);

#endif /* PIXELFONTS_H */
