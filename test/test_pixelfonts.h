#include "nourstest.h"
#include "pixelfonts.h"

void test_pixelfonts() {
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);
    SDL_assert(palette_NES != NULL);

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    struct PixelFont *test_font = PixelFont_Alloc();
    char *path = PATH_JOIN("..", "assets", "Fonts", "pixelnours_test.png");
    PixelFont_Load(test_font, renderer, path);

    /* --- Test bounding boxes --- */
    nourstest_true(test_font->glyph_bbox_width[0]       == 0);
    nourstest_true(test_font->glyph_bbox_width['a']     == 4);
    nourstest_true(test_font->glyph_bbox_width['0']     == 5);
    nourstest_true(test_font->glyph_bbox_width['A']     == 5);
    nourstest_true(test_font->glyph_bbox_width['B']     == 5);
    nourstest_true(test_font->glyph_bbox_height['0']    == 7);
    nourstest_true(test_font->glyph_bbox_height['a']    == 6);
    nourstest_true(test_font->glyph_bbox_height['A']    == 6);
    nourstest_true(test_font->glyph_bbox_height['B']    == 6);

    /* --- Test pixelfont lines --- */
    char *text;
    int line_num;
    size_t line_len_px = 64;

    /* -- 1 line -- */
    text     = "The lizard";
    SDL_assert(PixelFont_Width(test_font, text, strlen(text)) < line_len_px);
    line_num = PixelFont_Lines_Num_Len(test_font, text, line_len_px);
    nourstest_true(line_num == 1);

    /* -- 2 line -- */
    text     = "The lizard is a wizard";
    line_num = PixelFont_Lines_Num_Len(test_font, text, line_len_px);
    nourstest_true(line_num == 2);

    /* -- 3 line -- */
    text     = "The lizard is a wizard, by joving rove. I say living hell.";
    line_num = PixelFont_Lines_Num_Len(test_font, text, line_len_px);
    nourstest_true(line_num == 4);

    /* -- Lines split by breaking whole words -- */
    char *text3 = "The lizard is a wizard, by joving rove. I say living hell.";
    line_num = PixelFont_Lines_Num_Len(test_font, text3, line_len_px);
    nourstest_true(line_num == 4);
    struct TextLines text_lines = PixelFont_Lines_Len(test_font, text3, line_len_px);
    nourstest_true(text_lines.line_num == 4);
    nourstest_true(text_lines.line_len == 4);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[0]) < line_len_px);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[1]) < line_len_px);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[2]) < line_len_px);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[3]) < line_len_px);
    nourstest_true(strcmp(text_lines.lines[0], "The lizard is a") == 0);
    nourstest_true(strcmp(text_lines.lines[1], "wizard, by joving") == 0);
    nourstest_true(strcmp(text_lines.lines[2], "rove. I say living") == 0);
    nourstest_true(strcmp(text_lines.lines[3], "hell.") == 0);

    /* -- Lines split by breaking words in halves -- */
    char *text4 =
            "Conglomerate a rock y baka agglomeration bakanumeration stupidification negativitiation.";
    TextLines_Free(&text_lines);
    text_lines = PixelFont_Lines_Len(test_font, text4, line_len_px);
    line_num = PixelFont_Lines_Num_Len(test_font, text4, line_len_px);
    nourstest_true(line_num == 6);
    nourstest_true(text_lines.line_num == 6);
    nourstest_true(text_lines.line_len == 8);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[0]) < line_len_px);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[1]) < line_len_px);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[2]) < line_len_px);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[3]) < line_len_px);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[4]) < line_len_px);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[5]) < line_len_px);
    nourstest_true(strcmp(text_lines.lines[0], "Conglomerate a") == 0);
    nourstest_true(strcmp(text_lines.lines[1], "rock y baka aggl-") == 0);
    nourstest_true(strcmp(text_lines.lines[2], "omeration bakan-") == 0);
    nourstest_true(strcmp(text_lines.lines[3], "umeration stupi-") == 0);
    nourstest_true(strcmp(text_lines.lines[4], "dification negati-") == 0);
    nourstest_true(strcmp(text_lines.lines[5], "vitiation.") == 0);

    /* -- FREE -- */
    PixelFont_Free(test_font, true);
    TextLines_Free(&text_lines);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}
