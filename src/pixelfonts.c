#include "pixelfonts.h"

u8 pixelfont_big_y_offset[ASCII_GLYPH_NUM] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0,
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

u8 pixelfont_y_offset[ASCII_GLYPH_NUM] = {
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

struct TextLines TextLines_default =  {
    .lines      = NULL,
    .lines_len  = NULL,
    .line_num   = 0,
    .line_len   = 0,
};

struct PixelFont PixelFont_default =  {
    .glyph_space        = PIXELFONT_GLYPH_SPACE,
    .word_space         = PIXELFONT_WORD_SPACE,
    .glyph_width        = ASCII_GLYPH_WIDTH,
    .glyph_height       = ASCII_GLYPH_HEIGHT,
    .col_len            = ASCII_CHARSET_COL_LEN,
    .row_len            = ASCII_CHARSET_ROW_LEN,
    .charset_num        = ASCII_CHARSET_NUM,
    .scroll_speed       = SCROLL_TIME_FAST,
    .black              = SOTA_BLACK,
    .white              = SOTA_WHITE,
};

struct PixelFont TextureFont_default =  {
    .glyph_space        = TEXTUREFONT_GLYPH_SPACE,
    .word_space         = TEXTUREFONT_WORD_SPACE,
    .glyph_width        = TEXTUREFONT_GLYPH_WIDTH,
    .glyph_height       = TEXTUREFONT_GLYPH_HEIGHT,
    .col_len            = TEXTURE_CHARSET_COL_LEN,
    .row_len            = TEXTURE_CHARSET_ROW_LEN,
    .charset_num        = TEXTURE_CHARSET_ROW_LEN * TEXTURE_CHARSET_COL_LEN,
    .scroll_speed       = SCROLL_TIME_FAST,
    .black              = SOTA_BLACK,
    .white              = SOTA_WHITE,
};

/*--- Constructors/Destructors --- */
struct PixelFont *PixelFont_Alloc(void) {
    struct PixelFont *font  = malloc(sizeof(struct PixelFont));
    SDL_assert(font);
    *font = PixelFont_default;
    font->glyph_bbox_width  = calloc(font->charset_num, sizeof(*font->glyph_bbox_width));
    font->glyph_bbox_height = calloc(font->charset_num, sizeof(*font->glyph_bbox_height));
    return (font);
}

void PixelFont_Free(struct PixelFont *font, b32 isfree) {
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
        SDL_free(font->glyph_bbox_width);
        font->glyph_bbox_width = NULL;
    }
    if (font->glyph_bbox_height != NULL) {
        SDL_free(font->glyph_bbox_height);
        font->glyph_bbox_height = NULL;
    }
    if (isfree) {
        if (font != NULL) {
            SDL_free(font);
            font = NULL;
        }
    }
}

void PixelFont_Load(struct PixelFont *font, SDL_Renderer *renderer, char *fontname) {
    SDL_assert(fontname != NULL);
    SDL_assert(font != NULL);
    font->surface = Filesystem_Surface_Load(fontname, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(font->surface != NULL);
    SDL_assert(font->surface->format->palette == palette_SOTA);
    // SDL_SaveBMP(font->surface, "outsurface.bmp");
    SDL_assert(renderer != NULL);
    font->palette = palette_SOTA;
    font->texture = SDL_CreateTextureFromSurface(renderer, font->surface);
    SDL_assert(font->texture);
    PixelFont_Compute_Glyph_BBox(font);
}

struct PixelFont *TextureFont_Alloc(u8 row_len, u8 col_len) {
    struct PixelFont *font = malloc(sizeof(struct PixelFont));
    SDL_assert(font);
    *font = TextureFont_default;
    font->palette = palette_SOTA;
    font->istexturefont = true;
    font->charset_num = row_len * col_len;
    font->row_len     = row_len;
    font->col_len     = col_len;

    font->glyph_bbox_width  = malloc(font->charset_num * sizeof(*font->glyph_bbox_width));
    font->glyph_bbox_height = malloc(font->charset_num * sizeof(*font->glyph_bbox_height));

    return (font);
}

/*--- Internals --- */
void PixelFont_Swap_Palette(struct PixelFont *font, SDL_Renderer *renderer, i8 NEWw, i8 NEWb) {
    i8 Oldb = font->black, Oldw = font->white;
    Palette_Colors_Swap(font->palette, renderer, &font->surface, &font->texture,
                        Oldw, Oldb, NEWw, NEWb);
}

void TextLines_Realloc(struct TextLines *textlines, size_t len) {
    SDL_assert(len > textlines->line_len);
    if (textlines->lines == NULL) {
        textlines->lines = calloc(len, sizeof(*textlines->lines));
        SDL_assert(textlines->lines != NULL);
    } else {
        /* Re-Allocation */
        size_t bytesize = len * sizeof(*textlines->lines);
        char **buffer = SDL_realloc(textlines->lines, bytesize);
        SDL_assert(buffer != NULL);
        textlines->lines = buffer;

        /* Setting new memory to 0 */
        bytesize  = (len - textlines->line_len) * sizeof(*textlines->lines);
        memset(textlines->lines + textlines->line_len, 0, bytesize);
    }

    if (textlines->lines_len == NULL) {
        textlines->lines_len = calloc(len, sizeof(*textlines->lines_len));
        SDL_assert(textlines->lines_len != NULL);
    } else {
        SDL_assert(len > textlines->line_len);
        /* Re-Allocation */
        textlines->lines_len = SDL_realloc(textlines->lines_len, len * sizeof(*textlines->lines_len));

        /* Setting memory to 0 */
        size_t bytesize  = (len - textlines->line_len) * sizeof(*textlines->lines_len);
        memset(textlines->lines_len + textlines->line_len, 0, bytesize);
    }
    textlines->line_len = len;
}

void TextLines_Free(struct TextLines *textlines) {
    if (textlines->lines != NULL) {
        for (int i = 0; i < textlines->line_len; i++) {
            if (textlines->lines[i] != NULL) {
                SDL_free(textlines->lines[i]);
                textlines->lines[i] = NULL;
            }
        }
        SDL_free(textlines->lines);
        textlines->lines = NULL;
    }
    if (textlines->lines_len != NULL) {
        SDL_free(textlines->lines_len);
        textlines->lines_len = NULL;
    }
}

struct TextLines PixelFont_Lines_Len(struct PixelFont *font,  char *text, size_t line_len_px) {
    size_t len_char = strlen(text);
    return (PixelFont_Lines(font, text, len_char, line_len_px));
}

struct TextLines PixelFont_Lines(struct PixelFont *font,  char *text, size_t len_char,
                                 size_t line_len_px) {
    SDL_assert(font != NULL);
    SDL_assert(font->glyph_bbox_width != NULL);
    struct TextLines textlines = TextLines_default;
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
        SDL_Log("Get start of current line from next line %d", line_i);

        textlines.line_num += 1;

        /* Check if previous line was assigned correctly */
        if (textlines.line_num > 1) {
            SDL_assert(textlines.lines[line_i] != NULL);
        }

        if (textlines.line_num > textlines.line_len) {
            TextLines_Realloc(&textlines, (textlines.line_len * 2));
        }

        /* -- Get char that exceeds line pixel length -- */
        current_break = PixelFont_NextLine_Break(font, text, current_start, len_char, line_len_px);
        line_len_char = current_break - current_start;
        SDL_assert((line_len_char >= 0));

        SDL_Log("%d %d %d", current_break, len_char, line_len_px);
        /* -- Break: text fits in final row -- */
        if (current_break >= len_char) {
            SDL_Log("Break: text fits in final row %d", line_i);
            textlines.lines[line_i] = calloc(line_len_char + 1, sizeof(char));
            memcpy(textlines.lines[line_i], text + current_start, line_len_char);

            /* -- Measure line length -- */
            textlines.lines_len[line_i] = strlen(textlines.lines[line_i]);
            SDL_assert(textlines.lines_len[line_i] > 0);
            break;
        }

        /* -- Text doesn't fit in final row -- */
        /* - If current_break a space, break line there - */
        if (text[current_break] == ' ') {
            SDL_Log("Text doesn't fit in final row %d", line_i);
            /* Push current_break one space, beginning new line */
            int line_i = textlines.line_num - 1;
            textlines.lines[line_i] = calloc(line_len_char + 1, sizeof(char));
            SDL_assert(textlines.lines[line_i] !=  NULL);
            memcpy(textlines.lines[line_i], text + current_start, line_len_char);
            next_start = current_break + 1;

            /* -- Measure line length -- */
            textlines.lines_len[line_i] = strlen(textlines.lines[line_i]);
            SDL_assert(textlines.lines_len[line_i] > 0);
            continue;
        }

        SDL_Log("Add hyphen if last char is not a space");
        next_start = NextLine_Start(text, current_start, current_break, line_len_char);
        SDL_assert(next_start <= current_break);
        current_break = next_start;

        SDL_assert(current_start <= len_char);
        line_len_char = current_break - current_start;

        /* -- Add hyphen if last char is not a space -- */
        b32 add_hyphen = (text[current_break - 1] != ' ');
        /* true:  need 1 more char to put hyphen */
        /* false: last char is a space and is removed */
        line_len_char += add_hyphen - 1 + add_hyphen;
        SDL_assert(line_len_char >= 0);

        /* -- Copy test line -- */
        textlines.lines[line_i] = calloc(line_len_char + 1, sizeof(char));
        memcpy(textlines.lines[line_i], text + current_start, line_len_char);

        /* -- Add hyphen if necessary -- */
        if (add_hyphen) {

            textlines.lines[line_i][line_len_char - 1] = '-';
        }

        /* -- Measure line length -- */
        textlines.lines_len[line_i] = strlen(textlines.lines[line_i]);
        SDL_assert(textlines.lines_len[line_i] > 0);
    }

    SDL_Log("textlines.line_num %d", textlines.line_num);

    return (textlines);
}

/* Compute number of rows text occupies. */
/* NOTE: len [char], line_len [px] */
int PixelFont_Lines_Num(struct PixelFont *font,  char *text, size_t len_char,
                        size_t line_len_px) {
    SDL_assert(line_len_px > 0);
    SDL_assert(font != NULL);
    SDL_assert(font->glyph_bbox_width != NULL);
    int next_start    = 0; /* start of next line in text [char] */
    int current_start = 0; /* start of current line in text [char] */
    int current_break = 0; /* end of current line in text [char] */
    size_t line_len_char;  /* [char] */
    int rows = 0;

    while (current_break < len_char) {
        rows += 1;
        current_start = next_start;

        /* -- Get char that exceeds line pixel length -- */
        current_break = PixelFont_NextLine_Break(font, text, current_start, len_char, line_len_px);
        SDL_assert(current_break > current_start);
        line_len_char = current_break - current_start;
        /* -- Break: text fits in final row -- */
        if (current_break >= len_char)
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
    return (rows);
}

int PixelFont_NextLine_Break(struct PixelFont *font,  char *text, int previous_break,
                             size_t len_char, size_t line_len_px) {
    /* -- Find char that exceeds line pixel length, from previous start. -- */
    int width_px = 0;
    int current_break = previous_break;
    while (current_break < len_char) {
        unsigned char ascii = (unsigned char)text[current_break];
        width_px += font->glyph_bbox_width[ascii];
        // SDL_Log("width_px, line_len_px: %d, %d");
        if (width_px >= line_len_px) {
            break;
        }
        current_break += 1;
    }
    return (current_break);
}

int NextLine_Start( char *text, int previous_break, int current_break, size_t line_len_char) {
    /* -- Find char that starts a newline. -- */
    // Breaking a new line:
    //  1. If word > 4 char: Add "-" in middle of word. Send part after - to new line.
    //  2. Send word to next line
    int next_char;
    /* - If current_break is a char, need to check word length - */
    /* Get first half of length */
    char *buffer = calloc(line_len_char + 1, sizeof(buffer));
    memcpy(buffer, text + previous_break, line_len_char);
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

    SDL_free(buffer);
    return (next_char);
}

int PixelFont_Lines_Num_Len(struct PixelFont *font,  char *text, size_t line_len_px) {
    SDL_assert(font != NULL);
    SDL_assert(line_len_px > 0);
    size_t len = strlen(text);
    return (PixelFont_Lines_Num(font, text, len, line_len_px));
}

int PixelFont_Width_Len(struct PixelFont *font,  char *text) {
    SDL_assert(font != NULL);
    SDL_assert(text != NULL);
    size_t len = strlen(text);
    return (PixelFont_Width(font, text, len));
}

/* Compute exact width of text, including spaces */
int PixelFont_Width(struct PixelFont *font,  char *text, size_t len) {
    SDL_assert(font                     != NULL);
    SDL_assert(text                     != NULL);
    SDL_assert(font->glyph_bbox_width   != NULL);
    int width = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char ascii = (unsigned char)text[i];
        SDL_assert(ascii < font->charset_num);
        width += font->glyph_bbox_width[ascii];
    }
    return (width);
}

void PixelFont_Compute_Glyph_BBox(struct PixelFont *font) {
    SDL_Log("PixelFont_Compute_Glyph_BBox");
    SDL_assert(font->surface);
    SDL_assert(SDL_ISPIXELFORMAT_INDEXED(font->surface->format->format));
    SDL_LockSurface(font->surface);
    u8 *pixels = font->surface->pixels;

    /* Glyph cache friendly loop */
    for (size_t row = 0; row < font->row_len; row++) {
        for (size_t col = 0; col < font->col_len; col++) {
            SDL_Log("col row %d %d", col, row);
            u8 origin_x = col * font->glyph_width;
            u8 origin_y = row * font->glyph_height;
            // SDL_Log("origin %d %d", origin_x, origin_y);
            u8 max_width = 0, max_height = 0;
            /* pixel cache friendly loop */
            for (size_t x = origin_x; x < (origin_x + font->glyph_width); x++) {
                for (size_t y = origin_y; y < (origin_y + font->glyph_height); y++) {
                    /* if pixel not transparent, its in the box */
                    size_t index = Util_SDL_Surface_Index(font->surface, x, y);
                    if (pixels[index] != PALETTE_COLORKEY) {
                        SDL_Log("pixels[index] %d %d %d", index, PALETTE_COLORKEY, pixels[index]);
                        max_width  = (x - origin_x) > max_width  ? (x - origin_x) : max_width;
                        max_height = (y - origin_y) > max_height ? (y - origin_y) : max_height;
                    }
                }
            }
            /* "+1" for correct width: glyph with pixels [0, 3] has 4 width */
            int index = row * font->col_len + col;
            SDL_assert(index < font->charset_num);
            if (max_width > 0) {
                font->glyph_bbox_width[index]  = max_width + 1;
                SDL_Log("glyph_bbox_width[index] %d %d", index, font->glyph_bbox_width[index]);
            }
            if (max_height > 0) {
                font->glyph_bbox_height[index] = max_height + 1;
                SDL_Log("glyph_bbox_height[index] %d %d", index, font->glyph_bbox_height[index]);
            }
        }
    }
    SDL_UnlockSurface(font->surface);

    /* Word space */
    unsigned char ascii = (unsigned char)' ';
    if (ascii < font->charset_num) {
        font->glyph_bbox_width[ascii] = font->word_space;
    }

}

/*--- Scrolling --- */
int PixelFont_Scroll(struct PixelFont *font, u64 time_ns) {
    /* Timer should always reset after updating */
    int time_ms = (int)(time_ns / SOTA_us);
    b32 scroll = (time_ms >= font->scroll_speed);
    font->scroll_len += scroll;
    return (scroll);
}

/*--- Writing --- */
void PixelFont_Write_Len(struct PixelFont *font, SDL_Renderer *rdr, char *text,
                         u32 x, u32 y) {
    size_t len = strlen(text);
    PixelFont_Write(font, rdr, text, len, x, y);
}

void PixelFont_Write_Centered(struct PixelFont *font, SDL_Renderer *rdr,
                              char *text, size_t len, u32 x, u32 y) {
    int width = PixelFont_Width(font, text, len);
    PixelFont_Write(font, rdr, text, len, x - (width / 2), y);
}

void PixelFont_Write_Centered_Len(struct PixelFont *font, SDL_Renderer *rdr,
                                  char *text, u32 x, u32 y) {
    int width = PixelFont_Width_Len(font, text);
    PixelFont_Write_Len(font, rdr, text, x - (width / 2), y);
}

void PixelFont_Write_Scroll(struct PixelFont *font, SDL_Renderer *rdr, char *text, u32 x, u32 y) {
    size_t len = strlen(text);
    size_t to_render = font->scroll_len > len ? len : font->scroll_len;
    PixelFont_Write(font, rdr, text, to_render, x, y);
}

void PixelFont_Write(struct PixelFont *font, SDL_Renderer *renderer, char *text,
                     size_t len, u32 pos_x, u32 pos_y) {
    SDL_assert(font          != NULL);
    SDL_assert(renderer      != NULL);
    SDL_assert(font->texture != NULL);
    SDL_Rect srcrect = {0};
    SDL_Rect dstrect = {pos_x, pos_y, 0, 0};
    /* Write text to write_texture */
    for (size_t i = 0; i < len; i++) {
        unsigned char ascii = (unsigned char)text[i];

        switch (ascii * !font->istexturefont) {
            case  ' ': /* - space - */
                dstrect.x += font->word_space;
                continue;
            case  '\n': /* - newline - */
                dstrect.x = pos_x;
                dstrect.y = pos_y + font->glyph_height + font->linespace;
                continue;
        }

        srcrect.x = ascii % font->col_len * font->glyph_width;
        srcrect.y = ascii / font->col_len * font->glyph_height;
        srcrect.w = dstrect.w = font->glyph_bbox_width[ascii];
        srcrect.h = dstrect.h = font->glyph_bbox_height[ascii];
        if (font->y_offset != NULL)
            dstrect.y += font->y_offset[ascii];
        SDL_RenderCopy(renderer, font->texture, &srcrect, &dstrect); /* slow */
        if (font->y_offset != NULL)
            dstrect.y -= font->y_offset[ascii];

        /* - move to next letter - */
        dstrect.x += (font->glyph_bbox_width[ascii] + font->glyph_space);
    }
}
