#ifndef PIXELFONTS_H
#define PIXELFONTS_H

#include "enums.h"
#include "SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct TextLines;

/* --- ANTS --- */
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
typedef struct PixelFont {
    SDL_Texture *texture;  /* ASCII order */
    SDL_Surface *surface;  /* ASCII order */
    SDL_Texture *write_texture;
    SDL_Palette *palette;
    u8 *glyph_bbox_width;
    u8 *glyph_bbox_height;
    const u8 *y_offset; /* for each glyph */
    b32 istexturefont;
    /* Text Scrolling */
    int  scroll_speed;   /* [ms] time until new character is rendered */
    int  scroll_len;   /* [pixels/char] to render */
    u16 charset_num;
    i8  glyph_space;    /* [pixels] */
    u8  word_space;     /* [pixels] */
    u8  glyph_width;
    u8  glyph_height;
    u8  col_len;
    u8  row_len;
    i8  linespace;      /* [pixels] space to add to glyph height for new line */
    i8 black;
    i8 white;
} PixelFont;
extern const PixelFont PixelFont_default;
extern const struct PixelFont TextureFont_default;

/* --- Constructors/Destructors --- */
struct PixelFont *PixelFont_Alloc(void);
struct PixelFont *TextureFont_Alloc(u8 row_len, u8 col_len);

void PixelFont_Init_tnecs(void *p);
void PixelFont_Init(struct PixelFont *f);

void PixelFont_Free_tnecs(void *p);
void PixelFont_Free(struct PixelFont *f, b32 isfree);
void PixelFont_Load(struct PixelFont *f, SDL_Renderer *r, char *fontname);

/* --- Internals --- */
void PixelFont_Swap_Palette(struct PixelFont *f, SDL_Renderer *r, i8 w, i8 b);

/* --- Lines --- */
/* -- Counting lines -- */
void TextLines_Free(   struct TextLines *tl);
void TextLines_Realloc(struct TextLines *tl, size_t len);

/* -- Splitting lines -- */
struct TextLines PixelFont_Lines(    struct PixelFont *f, char *t, size_t l, size_t ll);
struct TextLines PixelFont_Lines_Len(struct PixelFont *f, char *t, size_t ll);

int PixelFont_Lines_Num(    struct PixelFont *f,  char *t, size_t l, size_t ll);
int PixelFont_Lines_Num_Len(struct PixelFont *f,  char *t, size_t ll);

int PixelFont_NextLine_Break(struct PixelFont *f,  char *t, int pb, size_t l, size_t ll);
int NextLine_Start(char *text, int pb, int cb, size_t l);

/* --- Width in [pixels] --- */
int PixelFont_Width(    struct PixelFont *f,  char *t, size_t l);
int PixelFont_Width_Len(struct PixelFont *f,  char *t);

/* - Glyph_BoundingBox: - */
// Note: Detect bounding box of each glyph of the font by looking at pixel values
void PixelFont_Compute_Glyph_BBox(struct PixelFont *font);

/*--- Scrolling --- */
/* Outputs true if the text scrolled this frame */
int PixelFont_Scroll(struct PixelFont *f, u64 time_ns);

/* --- Writing --- */
// TEXTURE FONT NOTE:
// For PixelFont_Write be usable for Texturefont,
// skip glyph 32, cause it is reserved for SPACE
// col_len 16: 1st cell in 3rd row / col_len 8: 1st cell in 5rd row
void PixelFont_Write(             struct PixelFont *f, SDL_Renderer *r, char *t,
                                  size_t len, u32 px, u32 py);
void PixelFont_Write_Len(         struct PixelFont *f, SDL_Renderer *r, char *t,
                                  u32 px, u32 py);
void PixelFont_Write_Scroll(      struct PixelFont *f, SDL_Renderer *r, char *t,
                                  u32 px, u32 py);
void PixelFont_Write_Centered(    struct PixelFont *font, SDL_Renderer *rdr,
                                  char *text, size_t len, u32 x, u32 y);
void PixelFont_Write_Centered_Len(struct PixelFont *f, SDL_Renderer *r, char *t,
                                  u32 px, u32 py);

#endif /* PIXELFONTS_H */
