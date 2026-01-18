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

#include "structs.h"
#include "palette.h"
#include "utilities.h"
#include "pixelfonts.h"
#include "filesystem.h"

const TextLines TextLines_default =  {0};

const PixelFont PixelFont_default =  {
    .space = {
        .glyph          = PIXELFONT_GLYPH_SPACE,
        .word           = PIXELFONT_WORD_SPACE,
    },

    .glyph = {
        .size = {
            .x          = ASCII_GLYPH_WIDTH,
            .y          = ASCII_GLYPH_HEIGHT,
        },
        .len = {
            .col        = ASCII_CHARSET_COL_LEN,
            .row        = ASCII_CHARSET_ROW_LEN,
        },
    },
    .scroll = {
        .speed          = SCROLL_TIME_FAST,
    },
    .colors = {
        .black          = SOTA_BLACK,
        .white          = SOTA_WHITE,
    }
};

const PixelFont TextureFont_default =  {
    .space = {
        .glyph          = TEXTUREFONT_GLYPH_SPACE,
        .word           = TEXTUREFONT_WORD_SPACE,
    },
    .glyph = {
        .size = {
            .x          = TEXTUREFONT_GLYPH_WIDTH,
            .y          = TEXTUREFONT_GLYPH_HEIGHT,
        },

        .len = {
            .col        = TEXTURE_CHARSET_COL_LEN,
            .row        = TEXTURE_CHARSET_ROW_LEN,
        },
    },
    .scroll = {
        .speed          = SCROLL_TIME_FAST,
    },
    .colors = {
        .black          = SOTA_BLACK,
        .white          = SOTA_WHITE,
    },
};


const u8 pixelfont_big_y_offset[ASCII_GLYPH_NUM] = {
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

const u8 pixelfont_y_offset[ASCII_GLYPH_NUM] = {
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

/*--- Constructors/Destructors --- */
PixelFont *PixelFont_New(void) {
    PixelFont *font = IES_malloc(sizeof(PixelFont));
    IES_check_ret(font, NULL);

    *font = PixelFont_default;
    i32 num = PixelFont_Glyph_Num(font);
    font->glyph.bbox = IES_calloc(num, sizeof(*font->glyph.bbox));
    return (font);
}

void PixelFont_Init_tnecs(void *voidfont) {
    IES_check(voidfont);

    PixelFont_Init(voidfont);
}

void PixelFont_Init(PixelFont *font) {
    IES_check(font);

    *font = PixelFont_default;
}

void PixelFont_Free_tnecs(void *voidfont) {
    IES_check(voidfont);

    PixelFont_Delete(voidfont);
}

void PixelFont_Delete(PixelFont *font) {
    IES_check(font);

    PixelFont_Free(font);
    IES_free(font);
}

void PixelFont_Free(PixelFont *font) {
    IES_check(font);

    if (font->platform.texture != NULL) {
        SDL_DestroyTexture(font->platform.texture);
        font->platform.texture = NULL;
    }
    if (font->platform.surface != NULL) {
        SDL_FreeSurface(font->platform.surface);
        font->platform.surface = NULL;
    }
    if (font->glyph.bbox != NULL) {
        SDL_free(font->glyph.bbox);
        font->glyph.bbox = NULL;
    }
}

void PixelFont_Load(PixelFont *font, SDL_Renderer *renderer,
                    char *name) {
    IES_check(font);
    IES_check(name);
    IES_check(renderer);

    /* -- Loading pixelfont from filename -- */
    font->platform.surface = Filesystem_Surface_Load(name,
                                                     SDL_PIXELFORMAT_INDEX8);
    IES_check(font->platform.surface != NULL);
    IES_check(font->platform.surface->format->palette == palette_SOTA);

#ifdef PIXELFONT_CHECK
    SDL_SaveBMP(font->platform.surface, "outsurface.bmp");
#endif /* PIXELFONT_CHECK */

    /* -- Create texture from surface -- */
    font->platform.palette = palette_SOTA;
    font->platform.texture = SDL_CreateTextureFromSurface(renderer, font->platform.surface);
    IES_check(font->platform.texture);

    PixelFont_Compute_Glyph_BBox(font);
}

PixelFont *TextureFont_New(u8 row_len, u8 col_len) {
    PixelFont *font = IES_malloc(sizeof(PixelFont));
    IES_check_ret(font, NULL);
    *font = TextureFont_default;
    font->platform.palette  = palette_SOTA;
    font->glyph.len.row     = row_len;
    font->glyph.len.col     = col_len;
    i32 num = PixelFont_Glyph_Num(font);

    font->glyph.bbox = IES_malloc(num * sizeof(*font->glyph.bbox));

    return (font);
}

/*--- Internals --- */
void PixelFont_Glyph_yOffset_W(PixelFont *font, const u8 *arr) {
    IES_check(font);
    font->glyph.y_offset = arr;
}

const u8 *PixelFont_Glyph_yOffset_R(const PixelFont *font) {
    IES_check_ret(font, NULL);
    return (font->glyph.y_offset);
}

Point PixelFont_Glyph_Size(const PixelFont *font) {
    IES_check_ret(font, Point_default);
    return (font->glyph.size);
}

i32 PixelFont_Glyph_Num(const PixelFont *font) {
    IES_check_ret(font, 0);
    return (font->glyph.len.row * font->glyph.len.col);
}

i32 PixelFont_Scroll_Len(const PixelFont *font) {
    IES_check_ret(font, 0);
    return (font->scroll.len);
}

i32 PixelFont_Space_Line(   const PixelFont *font) {
    IES_check_ret(font, 0);
    return (font->space.line);
}

i32 PixelFont_Space_Word(   const PixelFont *font) {
    IES_check_ret(font, 0);
    return (font->space.word);
}

i32 PixelFont_Space_Glyph(  const PixelFont *font) {
    IES_check_ret(font, 0);
    return (font->space.glyph);
}

void PixelFont_Swap_Palette(PixelFont *font, SDL_Renderer *renderer,
                            i8 NEWw, i8 NEWb) {
    IES_check(font);
    IES_check(renderer);

    i8 Oldb = font->colors.black;
    i8 Oldw = font->colors.white;
    Palette_Colors_Swap(font->platform.palette,  renderer,
                        &font->platform.surface, &font->platform.texture,
                        Oldw,           Oldb,
                        NEWw,           NEWb);
}

void TextLines_Realloc(TextLines *textlines, size_t len) {
    IES_check(textlines);
    IES_check(len > textlines->line_len);

    if (textlines->lines == NULL) {
        textlines->lines = IES_calloc(len, sizeof(*textlines->lines));
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
        textlines->lines_len = IES_calloc(len, sizeof(*textlines->lines_len));
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

void TextLines_Free(TextLines *textlines) {
    if (textlines == NULL) {
        return;
    }
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

TextLines PixelFont_Lines(PixelFont *font, char *text,
                          size_t len_char, size_t line_len_px) {
    /* Splitting input text into multiple lines */
    IES_check_ret(font             != NULL, TextLines_default);
    IES_check_ret(font->glyph.bbox != NULL, TextLines_default);

    if (len_char == 0) {
        len_char = strlen(text);
    }
    IES_check_ret(len_char > 0, TextLines_default);

    if (line_len_px == 0) {
        line_len_px = PixelFont_Width(font, text, len_char);
    }
    IES_check_ret(line_len_px > 0, TextLines_default);

    TextLines textlines = TextLines_default;
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
        // SDL_Log("Get start of current line from next line %d", line_i);

        textlines.line_num += 1;

        /* Check if previous line was assigned correctly */
        if (textlines.line_num > 1) {
            SDL_assert(textlines.lines[line_i - 1] != NULL);
        }

        if (textlines.line_num > textlines.line_len) {
            TextLines_Realloc(&textlines, (textlines.line_len * 2));
        }

        /* -- Get char that exceeds line pixel length -- */
        current_break = PixelFont_NextLine_Break(font, text, current_start, len_char, line_len_px);
        line_len_char = current_break - current_start;
        SDL_assert((line_len_char >= 0));

        // SDL_Log("%d %d %d", current_break, len_char, line_len_px);
        /* -- Break: text fits in final row -- */
        if (current_break >= len_char) {
            // SDL_Log("Break: text fits in final row %d", line_i);
            textlines.lines[line_i] = IES_calloc(line_len_char + 1, sizeof(char));
            memcpy(textlines.lines[line_i], text + current_start, line_len_char);

            /* -- Measure line length -- */
            textlines.lines_len[line_i] = strlen(textlines.lines[line_i]);
            SDL_assert(textlines.lines_len[line_i] > 0);
            break;
        }

        /* -- Text doesn't fit in final row -- */
        /* - If current_break a space, break line there - */
        if (text[current_break] == ' ') {
            // SDL_Log("Text doesn't fit in final row %d", line_i);
            /* Push current_break one space, beginning new line */
            int line_i = textlines.line_num - 1;
            textlines.lines[line_i] = IES_calloc(line_len_char + 1, sizeof(char));
            SDL_assert(textlines.lines[line_i] !=  NULL);
            memcpy(textlines.lines[line_i], text + current_start, line_len_char);
            next_start = current_break + 1;

            /* -- Measure line length -- */
            textlines.lines_len[line_i] = strlen(textlines.lines[line_i]);
            SDL_assert(textlines.lines_len[line_i] > 0);
            continue;
        }

        // SDL_Log("Add hyphen if last char is not a space");
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
        textlines.lines[line_i] = IES_calloc(line_len_char + 1, sizeof(char));
        memcpy(textlines.lines[line_i], text + current_start, line_len_char);

        /* -- Add hyphen if necessary -- */
        if (add_hyphen) {

            textlines.lines[line_i][line_len_char - 1] = '-';
        }

        /* -- Measure line length -- */
        textlines.lines_len[line_i] = strlen(textlines.lines[line_i]);
        SDL_assert(textlines.lines_len[line_i] > 0);
    }

    // SDL_Log("textlines.line_num %d", textlines.line_num);
    return (textlines);
}

int PixelFont_Lines_Num(PixelFont *font,  char *text,
                        size_t len_char, size_t line_len_px) {
    /* Compute number of rows text occupies. */
    /* NOTE: len [char], line_len [px] */
    IES_check_ret(text             != NULL, 0);
    IES_check_ret(font             != NULL, 0);
    IES_check_ret(font->glyph.bbox != NULL, 0);

    if (len_char == 0) {
        len_char = strlen(text);
    }
    IES_check_ret(len_char   > 0,        0);

    if (line_len_px == 0) {
        line_len_px = PixelFont_Width(font, text, len_char);
    }
    IES_check_ret(line_len_px   > 0,        0);

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
        if (current_break >= len_char) {
            break;
        }

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

int PixelFont_NextLine_Break(PixelFont *font, char *text,
                             int previous_break,
                             size_t len_char, size_t line_len_px) {
    /* -- Find char that exceeds line pixel length,
    **    from previous start. -- */
    IES_check_ret(font, 0);
    IES_check_ret(text, 0);

    int width_px        = 0;
    int current_break   = previous_break;
    while (current_break < len_char) {
        unsigned char ascii = (unsigned char)text[current_break];
        width_px += font->glyph.bbox[ascii].x;
        // SDL_Log("width_px, line_len_px: %d, %d");
        if (width_px >= line_len_px) {
            break;
        }
        current_break += 1;
    }
    return (current_break);
}

int NextLine_Start(char *text, int previous_break,
                   int current_break, size_t line_len_char) {
    /* -- Find char that starts a newline. -- */
    // Breaking a new line:
    //  1. If word > 4 char: Add "-" in middle of word. Send part after - to new line.
    //  2. Send word to next line
    int next_char;
    /* - If current_break is a char, need to check word length - */
    /* Get first half of length */
    char *buffer = IES_calloc(line_len_char + 1, sizeof(buffer));
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
    if ((word_len < lim)            ||
        (word_half2 <= (lim / 2))   ||
        (word_half1 <= (lim / 2))) {
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

int PixelFont_Width(PixelFont *font, char *text,
                    size_t len) {
    /* -- Compute exact width of text, including spaces -- */
    IES_check_ret(font             != NULL, 0);
    IES_check_ret(text             != NULL, 0);
    IES_check_ret(font->glyph.bbox != NULL, 0);
    i32 width   = 0;
    i32 num     = PixelFont_Glyph_Num(font);
    if (len == 0) {
        len = strlen(text);
    }

    for (size_t i = 0; i < len; i++) {
        unsigned char ascii = (unsigned char)text[i];
        if (ascii >= num) {
            IES_assert(0);
            continue;
        }
        width += font->glyph.bbox[ascii].x;
    }
    return (width);
}

void PixelFont_Compute_Glyph_BBox(PixelFont *font) {
    /* Find bounding box of each glyph in the font.
    **      Goes through each pixel in glyph grid
    **      and finds the max height, max width */
    IES_check(font);
    IES_check(font->platform.surface);
    IES_check(SDL_ISPIXELFORMAT_INDEXED(font->platform.surface->format->format));
    // Filesystem_Surface_Dump("pixelfont_test_write.png", font->platform.surface);

    SDL_LockSurface(font->platform.surface);
    u8 *pixels = font->platform.surface->pixels;

    /* Glyph cache friendly loop */
    for (size_t row = 0; row < font->glyph.len.row; row++) {
        for (size_t col = 0; col < font->glyph.len.col; col++) {
            // SDL_Log("col row %d %d", col, row);
            u8 origin_x = col * font->glyph.size.x;
            u8 origin_y = row * font->glyph.size.y;
            // SDL_Log("origin %d %d", origin_x, origin_y);
            u8 max_width = 0, max_height = 0;
            /* pixel cache friendly loop */
            for (size_t x = origin_x; x < (origin_x + font->glyph.size.x); x++) {
                for (size_t y = origin_y; y < (origin_y + font->glyph.size.y); y++) {
                    /* if pixel not transparent, its in the box */
                    size_t index = Util_SDL_Surface_Index(font->platform.surface, x, y);
                    if (pixels[index] != SOTA_COLORKEY) {
                        // SDL_Log("pixels[index] %d %d %d", index, PALETTE_COLORKEY, pixels[index]);
                        max_width  = (x - origin_x) > max_width  ? (x - origin_x) : max_width;
                        max_height = (y - origin_y) > max_height ? (y - origin_y) : max_height;
                    }
                }
            }
            int index = row * font->glyph.len.col + col;

            SDL_assert(index < PixelFont_Glyph_Num(font));
            if (max_width > 0) {
                /* Note: "+1" for correct width, glyph with
                **       [0, 3]px has 4 width */
                font->glyph.bbox[index].x  = max_width + 1;
            }
            if (max_height > 0) {
                font->glyph.bbox[index].y = max_height + 1;
            }
        }
    }
    SDL_UnlockSurface(font->platform.surface);

    /* Word space */
    unsigned char ascii = (unsigned char)' ';
    i32 num = PixelFont_Glyph_Num(font);
    if (ascii < num) {
        font->glyph.bbox[ascii].x = font->space.word;
    }

}

/*--- Scrolling --- */
int PixelFont_isScroll(PixelFont *font, u64 time_ns) {
    /* Timer should always reset after updating */
    IES_check_ret(font, 0);

    int time_ms = (int)(time_ns / SOTA_us);
    b32 scroll = (time_ms >= font->scroll.speed);
    font->scroll.len += scroll;
    return (scroll);
}

/*--- Writing --- */
void PixelFont_Write(PixelFont *font, PixelFont_In in) {
    IES_check(font);
    IES_check(in.text);
    IES_check(in.renderer);
    IES_check(font->platform.texture);

    if (in.len == 0) {
        in.len = strlen(in.text);
    }

    if (in.scroll) {
        in.len = font->scroll.len > in.len ? in.len : font->scroll.len;
    }
    IES_check(in.len);

    Point pos = in.pos;
    if (in.align == SOTA_TEXT_CENTER) {
        /* pos.x is center */
        i32 width = PixelFont_Width(font, in.text, in.len);
        pos.x -= (width / 2);
    } else if (in.align == SOTA_TEXT_RIGHT) {
        /* pos.x is rightward limit */
        i32 width = PixelFont_Width(font, in.text, in.len);
        pos.x -= width;
    }

    SDL_Rect srcrect = {0};
    SDL_Rect dstrect = {pos.x, pos.y, 0, 0};
    /* Write text to write_texture */
    for (size_t i = 0; i < in.len; i++) {
        unsigned char ascii = (unsigned char)in.text[i];

        /* --- Spaces between words, lines --- */
        switch (ascii * !in.istexturefont) {
            case  ' ': /* - space, ' ' == 32 - */
                dstrect.x += font->space.word;
                continue;
            case  '\n': /* - newline, '\n' == 13 - */
                dstrect.x = pos.x;
                dstrect.y = pos.y + font->glyph.size.y + font->space.line;
                continue;
        }

        srcrect.x = ascii % font->glyph.len.col * font->glyph.size.x;
        srcrect.y = ascii / font->glyph.len.col * font->glyph.size.y;
        srcrect.w = dstrect.w = font->glyph.bbox[ascii].x;
        srcrect.h = dstrect.h = font->glyph.bbox[ascii].y;
        if (font->glyph.y_offset != NULL) {
            dstrect.y += font->glyph.y_offset[ascii];
        }
        SDL_RenderCopy(in.renderer, font->platform.texture, &srcrect, &dstrect); /* slow */
        if (font->glyph.y_offset != NULL) {
            dstrect.y -= font->glyph.y_offset[ascii];
        }

        /* - move to next letter - */
        dstrect.x += (font->glyph.bbox[ascii].x + font->space.glyph);
    }
}
