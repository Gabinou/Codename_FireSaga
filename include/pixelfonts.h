#ifndef PIXELFONTS_H
#define PIXELFONTS_H

#include "enums.h"
#include "filesystem.h"
#include "globals.h"
#include "debug.h"
#include "macros.h"
#include "SDL2/SDL.h"

/* --- CONSTANTS --- */
enum PIXELFONT {
    ASCII_CHARSET_ROW_LEN       = 16,
    ASCII_CHARSET_COL_LEN       = 16,
    ASCII_GLYPH_HEIGHT          =  8,
    ASCII_GLYPH_WIDTH           =  8,
    ASCII_CHARSET_NUM           = ASCII_CHARSET_ROW_LEN * ASCII_CHARSET_COL_LEN,
    PIXELFONT_WORD_SPACE        =  2,
    PIXELFONT_GLYPH_SPACE       =  0,
    PIXELFONT_HEIGHT            =  8,
    ASCII_GLYPH_NUM             = 0x0100
};

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

extern uf8 pixelfont_y_offset[ASCII_GLYPH_NUM];
extern uf8 pixelfont_big_y_offset[ASCII_GLYPH_NUM];

/* --- DEFINITION --- */
typedef struct PixelFont {
    SDL_Texture *texture;  /* ASCII order */
    SDL_Surface *surface;  /* ASCII order */
    SDL_Texture *write_texture;
    uf8  glyph_space;    /* [pixels] */
    uf8  word_space;     /* [pixels] */
    uf8  glyph_width;
    uf8  glyph_height;
    uf8  col_len;
    uf8  row_len;
    if8  linespace;      /* [pixels] space to add to glyph height for new line */
    uf16 charset_num;
    uf8 *glyph_bbox_width;
    uf8 *glyph_bbox_height;
    uf8 *y_offset;
    if8  white_color;    /* replaces white */
    if8  black_color;    /* replaces black */
    /* Text Scrolling */
    int  scroll_speed;   /* [ms] time until new character is rendered */
    int  scroll_len;   /* [pixels/char] to render */
} PixelFont;
extern struct PixelFont PixelFont_default;
extern struct PixelFont TextureFont_default;

/* --- Constructors/Destructors --- */
struct PixelFont *PixelFont_Alloc();
struct PixelFont *TextureFont_Alloc(uf8 row_len, uf8 col_len);

void PixelFont_Free(struct PixelFont *f, bool free);
void PixelFont_Load(struct PixelFont *f, SDL_Renderer *r, char *fontname);

/* --- Internals --- */
void PixelFont_Swap_Palette(struct PixelFont *f, SDL_Renderer *r, if8 w, if8 b);

/* --- Lines --- */
/* -- Counting lines -- */
void TextLines_Free(   struct TextLines *tl);
void TextLines_Realloc(struct TextLines *tl, size_t len);

/* -- Splitting lines -- */
struct TextLines PixelFont_Lines(    struct PixelFont *f, const char *t, size_t l, size_t ll);
struct TextLines PixelFont_Lines_Len(struct PixelFont *f, const char *t, size_t ll);

int PixelFont_Lines_Num(     struct PixelFont *f, const char *t, size_t l, size_t ll);
int PixelFont_Lines_Num_Len( struct PixelFont *f, const char *t, size_t ll);

int PixelFont_NextLine_Break(struct PixelFont *f, const char *t, int pb, size_t l, size_t ll);
int NextLine_Start(const char *text, int pb, int cb, size_t l);

/* --- Width in [pixels] --- */
int PixelFont_Width(       struct PixelFont *f, const char *t, size_t l);
int PixelFont_Width_Len(   struct PixelFont *f, const char *t);

/* - Glyph_BoundingBox: - */
// Note: Detect bounding box of each glyph of the font by looking at pixel values
void PixelFont_Compute_Glyph_BBox(struct PixelFont *font);

/*--- Scrolling --- */
/* Outputs true if the text scrolled this frame */
int PixelFont_Scroll(struct PixelFont *f, u64 time_ns);

/*--- Writing --- */
// TEXTURE FONT NOTE:
// For PixelFont_Write be usable for Texturefont,
// skip glyph 32, cause it is reserved for SPACE
// col_len 16: 1st cell in 3rd row / col_len 8: 1st cell in 5rd row
void PixelFont_Write(       struct PixelFont *f, SDL_Renderer *r, char *t, size_t len, uf32 px,
                            uf32 py);
void PixelFont_Write_Len(   struct PixelFont *f, SDL_Renderer *r, char *t, uf32 px, uf32 py);
void PixelFont_Write_Scroll(struct PixelFont *f, SDL_Renderer *r, char *t, uf32 px, uf32 py);

#endif /* PIXELFONTS_H */
