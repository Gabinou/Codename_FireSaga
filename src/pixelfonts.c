#include "pixelfonts.h"

uf8 pixelfont_big_y_offset[ASCII_GLYPH_NUM] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

uf8 pixelfont_y_offset[ASCII_GLYPH_NUM] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};


struct PixelFont PixelFont_default =  {
    .texture            = NULL,
    .surface            = NULL,
    .glyph_space        = PIXELFONT_GLYPH_SPACE,
    .word_space         = PIXELFONT_WORD_SPACE,
    .glyph_width        = ASCII_GLYPH_WIDTH,
    .glyph_height       = ASCII_GLYPH_HEIGHT,
    .col_len            = ASCII_CHARSET_COL_LEN,
    .row_len            = ASCII_CHARSET_ROW_LEN,
    .charset_num        = ASCII_CHARSET_NUM,
    .glyph_bbox_width   = NULL,
    .glyph_bbox_height  = NULL,
    .y_offset           = NULL,
    .scroll_speed       = SCROLL_TIME_FAST,
    .scroll_len         = 0,
};

struct PixelFont TextureFont_default =  {
    .texture            = NULL,
    .surface            = NULL,
    .glyph_space        = TEXTUREFONT_GLYPH_SPACE,
    .word_space         = TEXTUREFONT_WORD_SPACE,
    .glyph_width        = TEXTUREFONT_GLYPH_WIDTH,
    .glyph_height       = TEXTUREFONT_GLYPH_HEIGHT,
    .col_len            = TEXTURE_CHARSET_COL_LEN,
    .row_len            = TEXTURE_CHARSET_ROW_LEN,
    .charset_num        = TEXTURE_CHARSET_ROW_LEN * TEXTURE_CHARSET_COL_LEN,
    .glyph_bbox_width   = NULL,
    .glyph_bbox_height  = NULL,
    .y_offset           = NULL,
    .scroll_speed       = SCROLL_TIME_FAST,
    .scroll_len         = 0,
};

/*--- Constructors/Destructors --- */
struct PixelFont *PixelFont_Alloc() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct PixelFont *font  = malloc(sizeof(struct PixelFont));
    SDL_assert(font);
    *font = PixelFont_default;
    font->glyph_bbox_width  = calloc(font->charset_num, sizeof(*font->glyph_bbox_width));
    font->glyph_bbox_height = calloc(font->charset_num, sizeof(*font->glyph_bbox_height));
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (font);
}

struct PixelFont *TextureFont_Alloc(uf8 row_len, uf8 col_len) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct PixelFont *font = malloc(sizeof(struct PixelFont));
    SDL_assert(font);
    *font = TextureFont_default;

    font->charset_num = row_len * col_len;
    font->row_len     = row_len;
    font->col_len     = col_len;

    font->glyph_bbox_width  = malloc(font->charset_num * sizeof(*font->glyph_bbox_width));
    font->glyph_bbox_height = malloc(font->charset_num * sizeof(*font->glyph_bbox_height));

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (font);
}

void PixelFont_Load(struct PixelFont *font, SDL_Renderer *renderer, char *fontname) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(fontname != NULL);
    SDL_assert(font != NULL);
    font->surface = Filesystem_Surface_Load(fontname, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(font->surface != NULL);
    // SDL_SaveBMP(font->surface, "outsurface.bmp");

    SDL_assert(renderer != NULL);
    font->texture = SDL_CreateTextureFromSurface(renderer, font->surface);
    SDL_assert(font->texture);
    PixelFont_Compute_Glyph_BBox(font);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void PixelFont_Free(struct PixelFont *font, bool isfree) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(font != NULL);
    if (font->texture != NULL) {
        SDL_DestroyTexture(font->texture);
        font->texture = NULL;
    }
    if (font->surface != NULL) {
        SDL_FreeSurface(font->surface);
        font->surface = NULL;
    }
    if (font->glyph_bbox_width != NULL) {
        free(font->glyph_bbox_width);
        font->glyph_bbox_width = NULL;
    }
    if (font->glyph_bbox_height != NULL) {
        free(font->glyph_bbox_height);
        font->glyph_bbox_height = NULL;
    }
    if (isfree) {
        if (font != NULL) {
            free(font);
            font = NULL;
        }
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/*--- Internals --- */
void PixelFont_Swap_Palette(struct PixelFont *font, SDL_Renderer *renderer, if8 NEWw, if8 NEWb) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(NEWw < PALETTE_NES_COLOR_NUM);
    SDL_assert(NEWb < PALETTE_NES_COLOR_NUM);
    SDL_assert(palette_NES->ncolors == PALETTE_NES_COLOR_NUM);

    /* Save old colors fropm palette_NES, */
    /* before changing anything */
    if8 NESb = 1, NESw = PALETTE_NES_COLOR_NUM - 1;
    SDL_Color old_white, old_black, new_white, new_black;
    if (NEWw > -1) {
        old_white = palette_NES->colors[NESw];
        new_white = palette_NES->colors[NEWw];
    }
    if (NEWb > -1) {
        old_black = palette_NES->colors[NESb];
        new_black = palette_NES->colors[NEWb];
    }

    /* Swap colors in palette_NES */
    if (NEWw > -1) {
        palette_NES->colors[NESw].r = new_white.r;
        palette_NES->colors[NESw].g = new_white.g;
        palette_NES->colors[NESw].b = new_white.b;
    }
    if (NEWb > -1) {
        palette_NES->colors[NESb].r = new_black.r;
        palette_NES->colors[NESb].g = new_black.g;
        palette_NES->colors[NESb].b = new_black.b;
    }

    /* Swap palette of font surface, texture */
    font->surface = Filesystem_Surface_Palette_Swap(font->surface, palette_NES);
    font->texture = SDL_CreateTextureFromSurface(renderer, font->surface);

    /* Revert colors in palette_NES */
    if (NEWw > -1) {
        palette_NES->colors[NESw].g = old_white.g;
        palette_NES->colors[NESw].r = old_white.r;
        palette_NES->colors[NESw].b = old_white.b;
    }
    if (NEWb > -1) {
        palette_NES->colors[NESb].r = old_black.r;
        palette_NES->colors[NESb].g = old_black.g;
        palette_NES->colors[NESb].b = old_black.b;
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TextLines_Realloc(struct TextLines *textlines, size_t len) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(len > textlines->line_len);
    if (textlines->lines == NULL) {
        textlines->lines    = calloc(len, sizeof(*textlines->lines));
    } else {
        /* Allocation */
        textlines->lines = realloc(textlines->lines, len * sizeof(*textlines->lines));

        /* Setting memory to 0 */
        size_t bytesize  = (len - textlines->line_len) * sizeof(*textlines->lines);
        memset(textlines->lines + textlines->line_len, 0, bytesize);
    }

    if (textlines->lines_len == NULL) {
        textlines->lines_len    = calloc(len, sizeof(*textlines->lines_len));
    } else {
        SDL_assert(len > textlines->line_len);
        /* Allocation */
        textlines->lines_len = realloc(textlines->lines_len, len * sizeof(*textlines->lines_len));

        /* Setting memory to 0 */
        size_t bytesize  = (len - textlines->line_len) * sizeof(*textlines->lines_len);
        memset(textlines->lines_len + textlines->line_len, 0, bytesize);
    }
    textlines->line_len = len;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TextLines_Free(struct TextLines *textlines) {
    if (textlines->lines == NULL) {
        for (int i = 0; i < textlines->line_num; i++) {
            free(textlines->lines[i]);
        }
        free(textlines->lines);
    }
    if (textlines->lines_len == NULL) {
        free(textlines->lines_len);
    }
}

struct TextLines PixelFont_Lines_Len(struct PixelFont *font, const char *text, size_t line_len_px) {
    size_t len_char = strlen(text);
    return (PixelFont_Lines(font, text, len_char, line_len_px));
}

struct TextLines PixelFont_Lines(struct PixelFont *font, const char *text, size_t len_char,
                                 size_t line_len_px) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(font != NULL);
    SDL_assert(font->glyph_bbox_width != NULL);
    struct TextLines textlines = {0};
    TextLines_Realloc(&textlines, 4);
    SDL_assert(textlines.line_len == 4);
    int next_start    = 0; /* start of next line in text [char] */
    int current_start = 0; /* start of current line in text [char] */
    int current_break = 0; /* end of current line in text [char] */
    size_t line_len_char;  /* [char] */

    while (current_break < len_char) {
        /* Get start of current line from next line */
        current_start = next_start;

        int line_i = textlines.line_num;
        textlines.line_num += 1;

        if (textlines.line_num > textlines.line_len)
            TextLines_Realloc(&textlines, (textlines.line_len * 2));

        /* -- Get char that exceeds line pixel length -- */
        current_break = PixelFont_NextLine_Break(font, text, current_start, len_char, line_len_px);
        line_len_char = current_break - current_start;

        /* -- Break: text fits in final row -- */
        if (current_break >= (len_char - 1)) {
            textlines.lines[line_i] = calloc(line_len_char + 1, sizeof(char));
            strncpy(textlines.lines[line_i], text + current_start, line_len_char);
            break;
        }

        /* -- Text doesn't fit in final row -- */
        /* - If current_break a space, break line there - */
        if (text[current_break] == ' ') {
            /* Push current_break one space, beginning new line */
            int line_i = textlines.line_num - 1;
            textlines.lines[line_i] = calloc(line_len_char + 1, sizeof(char));
            strncpy(textlines.lines[line_i], text + current_start, line_len_char);
            next_start = current_break;
            continue;
        }
        next_start = NextLine_Start(text, current_start, current_break, line_len_char);
        SDL_assert(next_start <= current_break);
        current_break = next_start;

        SDL_assert(current_start <= len_char);
        line_len_char = current_break - current_start;

        /* -- Add hyphen if last char is not a space -- */
        bool add_hyphen = (text[current_break - 1] != ' ');
        /* true:  need 1 more char to put hyphen */
        /* false: last char is a space and is removed */
        line_len_char += add_hyphen - 1 + add_hyphen;

        /* -- Copy test line -- */
        textlines.lines[line_i] = calloc(line_len_char + 1, sizeof(char));
        strncpy(textlines.lines[line_i], text + current_start, line_len_char);

        /* -- Add hyphen if necessary -- */
        if (add_hyphen)
            textlines.lines[line_i][line_len_char - 1] = '-';

    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (textlines);
}

/* Compute number of rows text occupies. */
/* NOTE: len [char], line_len [px] */
int PixelFont_Lines_Num(struct PixelFont *font, const char *text, size_t len_char,
                        size_t line_len_px) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(line_len_px > 0);
    SDL_assert(font != NULL);
    SDL_assert(font->glyph_bbox_width != NULL);
    int next_start    = 0; /* start of next line in text [char] */
    int current_start = 0; /* start of current line in text [char] */
    int current_break = 0; /* end of current line in text [char] */
    size_t line_len_char;  /* [char] */
    int rows = 0;

    while (current_break < (len_char - 1)) {
        rows += 1;
        current_start = next_start;

        /* -- Get char that exceeds line pixel length -- */
        current_break = PixelFont_NextLine_Break(font, text, current_start, len_char, line_len_px);
        SDL_assert(current_break > current_start);
        line_len_char = current_break - current_start;
        /* -- Break: text fits in final row -- */
        if (current_break >= (len_char - 1))
            break;

        /* -- Text doesn't fit in final row -- */
        /* - If current_break a space, break line there - */
        if (text[current_break] == ' ') {
            /* Push current_break one space, beginning new line */
            next_start = current_break + 1;
            continue;
        }
        next_start = NextLine_Start(text, current_start, current_break, line_len_char);
        SDL_assert(next_start > current_start);
    }
    if (rows < 1)
        rows = 1;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (rows);
}

int PixelFont_NextLine_Break(struct PixelFont *font, const char *text, int previous_break,
                             size_t len_char, size_t line_len_px) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Find char that exceeds line pixel length, from previous start. -- */
    int width_px = 0;
    int current_break = previous_break;
    while (current_break < len_char) {
        unsigned char ascii = (unsigned char)text[current_break];
        width_px += font->glyph_bbox_width[ascii];
        if (width_px >= line_len_px)
            break;
        current_break += 1;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (current_break);
}

int NextLine_Start(const char *text, int previous_break, int current_break, size_t line_len_char) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Find char that starts a newline. -- */
    // Breaking a new line:
    //  1. If word > 4 char: Add "-" in middle of word. Send part after - to new line.
    //  2. Send word to next line

    int next_char;
    /* - If current_break is a char, need to check word length - */
    /* Get first half of length */
    char *buffer = calloc(line_len_char + 1, sizeof(buffer));
    strncpy(buffer, text + previous_break, line_len_char);
    char *space_before  = strrchr(buffer, ' ');
    size_t word_half1 = line_len_char - (space_before - buffer) - 1;

    /* Get second half of length */
    size_t word_half2;
    char *space_after = memchr(text + current_break, ' ', strlen(text + current_break));
    if (space_after != NULL) {
        SDL_assert(space_after > text);
        word_half2 = (size_t)(space_after - text) - current_break - 1;
    } else {
        word_half2 = strlen(text + current_break);
    }
    size_t word_len = word_half1 + word_half2;
    int lim = SOTA_TEXT_WORDLEN_BREAK;
    if ((word_len < lim) || (word_half2 <= (lim / 2)) || (word_half1 <= (lim / 2))) {
        /* break by sending whole word down a line e.g. current_char word start */
        next_char = current_break - word_half1;
    } else {
        /* break by putting a "-" in middle of the word and breaking there */
        /* Ex: if current_break is "i" in "artiste",
        *          1. Put "-" 1 before previous char -> "ar-tiste"
                        "i" exceeds line, "t" doesnt
        *          2. current_break on next line should be "i"-1 = "t"
        */
        next_char = current_break - 1;
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (next_char);
}

int PixelFont_Lines_Num_Len(struct PixelFont *font, const char *text, size_t line_len_px) {
    SDL_assert(font != NULL);
    SDL_assert(line_len_px > 0);
    size_t len = strlen(text);
    return (PixelFont_Lines_Num(font, text, len, line_len_px));
}

int PixelFont_Width_Len(struct PixelFont *font, const char *text) {
    size_t len = strlen(text);
    return (PixelFont_Width(font, text, len));
}

/* Compute exact width of text, including spaces */
int PixelFont_Width(struct PixelFont *font, const char *text, size_t len) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    int width = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char ascii = (unsigned char)text[i];
        width += font->glyph_bbox_width[ascii];
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (width);
}

void PixelFont_Compute_Glyph_BBox(struct PixelFont *font) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(font->surface);
    SDL_assert(SDL_ISPIXELFORMAT_INDEXED(font->surface->format->format));
    SDL_LockSurface(font->surface);
    uf8 *pixels = font->surface->pixels;

    /* Glyph cache friendly loop */
    for (size_t row = 0; row < font->row_len; row++) {
        for (size_t col = 0; col < font->col_len; col++) {
            uf8 origin_x = col * font->glyph_width;
            uf8 origin_y = row * font->glyph_height;
            uf8 max_width = 0, max_height = 0;
            /* pixel cache friendly loop */
            for (size_t x = origin_x; x < (origin_x + font->glyph_width); x++) {
                for (size_t y = origin_y; y < (origin_y + font->glyph_height); y++) {
                    /* if pixel not transparent, its in the box */
                    size_t index = Util_SDL_Surface_Index(font->surface, x, y);
                    if (pixels[index] != PALETTE_COLORKEY) {
                        max_width  = (x - origin_x) > max_width  ? (x - origin_x) : max_width;
                        max_height = (y - origin_y) > max_height ? (y - origin_y) : max_height;
                    }
                }
            }
            /* "+1" for correct width: glyph with pixels [0, 3] has 4 width */
            int index = row * font->col_len + col;
            SDL_assert(index < font->charset_num);
            if (max_width > 0)
                font->glyph_bbox_width[index]  = max_width + 1;
            if (max_height > 0)
                font->glyph_bbox_height[index] = max_height + 1;
        }
    }
    SDL_UnlockSurface(font->surface);

    /* Word space */
    font->glyph_bbox_width[' '] = font->word_space;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/*--- Scrolling --- */
int PixelFont_Scroll(struct PixelFont *font, u64 time_ns) {
    /* Timer should always reset after updating */
    int time_ms = (int)(time_ns / SOTA_us);
    bool scroll = (time_ms >= font->scroll_speed);
    font->scroll_len += scroll;
    return (scroll);
}

/*--- Writing --- */
void PixelFont_Write_Len(struct PixelFont *font, SDL_Renderer *rdr, char *text, uf32 x, uf32 y) {
    size_t len = strlen(text);
    PixelFont_Write(font, rdr, text, len, x, y);
}

void PixelFont_Write_Scroll(struct PixelFont *font, SDL_Renderer *rdr, char *text, uf32 x, uf32 y) {
    PixelFont_Write(font, rdr, text, font->scroll_len, x, y);
}

void PixelFont_Write(struct PixelFont *font, SDL_Renderer *renderer, char *text,
                     size_t len, uf32 pos_x, uf32 pos_y) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(font          != NULL);
    SDL_assert(renderer      != NULL);
    SDL_assert(font->texture != NULL);
    SDL_Rect srcrect = {0};
    SDL_Rect dstrect = {pos_x, pos_y, 0, 0};
    /* Write text to write_texture */
    for (size_t i = 0; i < len; i++) {
        unsigned char ascii = (unsigned char)text[i];
        switch (ascii) {
            case  ' ': /* - space - */
                dstrect.x += font->word_space;
                continue;
            case  '\n': /* - newline - */
                dstrect.x = pos_x;
                dstrect.y = pos_y + ASCII_GLYPH_HEIGHT + font->linespace;
                continue;
        }

        srcrect.x = text[i] % font->col_len * font->glyph_width;
        srcrect.y = text[i] / font->col_len * font->glyph_height;
        srcrect.w = dstrect.w = font->glyph_bbox_width[text[i]];
        srcrect.h = dstrect.h = font->glyph_bbox_height[text[i]];
        if (font->y_offset != NULL)
            dstrect.y += font->y_offset[ascii];
        SDL_RenderCopy(renderer, font->texture, &srcrect, &dstrect); /* slow */
        if (font->y_offset != NULL)
            dstrect.y -= font->y_offset[ascii];

        /* - move to next letter - */
        dstrect.x += (font->glyph_bbox_width[text[i]] + font->glyph_space);
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
