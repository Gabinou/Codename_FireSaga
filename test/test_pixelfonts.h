
#include "nourstest.h"
#include "pixelfonts.h"
#include "text_box.h"

void test_pixelfonts_internals() {

    SDL_assert(palette_SOTA != NULL);

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    PixelFont *test_font = PixelFont_Alloc();
    test_font->glyph.size.x  = 8;
    test_font->glyph.size.y = 8;
    SDL_assert(test_font->glyph.len.row > 0);
    SDL_assert(test_font->glyph.len.col > 0);
    char *path = PATH_JOIN("..", "assets", "fonts", "pixelnours_test.png");
    PixelFont_Load(test_font, renderer, path);

    /* --- Test bounding boxes --- */
    nourstest_true(test_font->glyph.bbox[0].x       == 0);
    nourstest_true(test_font->glyph.bbox['a'].x     == 4);
    nourstest_true(test_font->glyph.bbox['0'].x     == 5);
    nourstest_true(test_font->glyph.bbox['A'].x     == 5);
    nourstest_true(test_font->glyph.bbox['B'].x     == 5);
    nourstest_true(test_font->glyph.bbox['0'].y    == 7);
    nourstest_true(test_font->glyph.bbox['a'].y    == 6);
    nourstest_true(test_font->glyph.bbox['A'].y    == 6);
    nourstest_true(test_font->glyph.bbox['B'].y    == 6);

    /* --- Test pixelfont lines --- */
    char *text;
    int line_num;
    size_t line_len_px = 64;

    /* -- 1 line -- */
    text     = "The lizard";
    SDL_assert(PixelFont_Width(test_font, text, strlen(text)) < line_len_px);
    line_num = PixelFont_Lines_Num_Len(test_font, text, line_len_px);
    nourstest_true(line_num == 1);

    text     = "Hello, World!";
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
    line_len_px = 64;
    char *text3 = "The lizard is a wizard, by joving rove. I say living hell.";
    line_num = PixelFont_Lines_Num_Len(test_font, text3, line_len_px);
    nourstest_true(line_num == 4);
    struct TextLines text_lines = PixelFont_Lines(test_font, text3, line_len_px, 0);
    nourstest_true(text_lines.line_num == 4);
    nourstest_true(text_lines.line_len == 4);
    nourstest_true(PixelFont_Width(test_font, text_lines.lines[0], 0) < line_len_px);
    // return;
    // SDL_Log("'%s' \n", text_lines.lines[0]);
    // SDL_Log("'%s' \n", text_lines.lines[1]);
    nourstest_true(PixelFont_Width(test_font, text_lines.lines[1], 0) < line_len_px);
    nourstest_true(PixelFont_Width(test_font, text_lines.lines[2], 0) < line_len_px);
    nourstest_true(PixelFont_Width(test_font, text_lines.lines[3], 0) < line_len_px);
    s8 s8temp1 = s8_var(text_lines.lines[0]);
    s8 s8temp2 = s8_literal("The lizard is a");

    SDL_assert(s8temp1.len == 15);
    SDL_assert(s8temp2.len == 15);
    SDL_assert(s8temp2.len == s8temp1.len);

    nourstest_true(s8equal(s8temp1, s8temp2));
    s8temp1 = s8_var(text_lines.lines[1]);
    s8temp2 = s8_literal("wizard, by joving");
    nourstest_true(s8equal(s8temp1, s8temp2));
    s8temp1 = s8_var(text_lines.lines[2]);
    s8temp2 = s8_literal("rove. I say living");
    nourstest_true(s8equal(s8temp1, s8temp2));
    s8temp1 = s8_var(text_lines.lines[3]);
    s8temp2 = s8_literal("hell.");
    nourstest_true(s8equal(s8temp1, s8temp2));

    /* -- Lines split by breaking words in halves -- */
    char *text4 =
            "Conglomerate a rock y baka agglomeration bakanumeration stupidification negativitiation.";
    TextLines_Free(&text_lines);
    text_lines = PixelFont_Lines(test_font, text4, line_len_px, 0);
    line_num   = PixelFont_Lines_Num_Len(test_font, text4, line_len_px);
    nourstest_true(line_num == 6);
    nourstest_true(text_lines.line_num == 6);
    nourstest_true(text_lines.line_len == 8);
    nourstest_true(PixelFont_Width(test_font, text_lines.lines[0], 0) < line_len_px);
    nourstest_true(PixelFont_Width(test_font, text_lines.lines[1], 0) < line_len_px);
    nourstest_true(PixelFont_Width(test_font, text_lines.lines[2], 0) < line_len_px);
    nourstest_true(PixelFont_Width(test_font, text_lines.lines[3], 0) < line_len_px);
    nourstest_true(PixelFont_Width(test_font, text_lines.lines[4], 0) < line_len_px);
    nourstest_true(PixelFont_Width(test_font, text_lines.lines[5], 0) < line_len_px);
    // nourstest_true(s8equal(&text_lines.lines[0], "Conglomerate a") == 0);
    // nourstest_true(s8equal(&text_lines.lines[1], "rock y baka aggl-") == 0);
    // nourstest_true(s8equal(&text_lines.lines[2], "omeration bakan-") == 0);
    // nourstest_true(s8equal(&text_lines.lines[3], "umeration stupi-") == 0);
    // nourstest_true(s8equal(&text_lines.lines[4], "dification negati-") == 0);
    // nourstest_true(s8equal(&text_lines.lines[5], "vitiation.") == 0);

    /* -- SDL_free -- */
    PixelFont_Free(test_font, true);
    TextLines_Free(&text_lines);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}

void test_pixelfonts_render() {
    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    struct Text_Box bubble = Text_Box_default;

    /* -- Create n9patch -- */
    // render_target is NULL cause there is render_target!
    n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;
    /* - Pixelnours - */
    bubble.pixelfont = PixelFont_Alloc();
    PixelFont_Glyph_yOffset_W(bubble.pixelfont, pixelfont_y_offset);
    Text_Bubble_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts", "pixelnours.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, SOTA_BLACK, SOTA_WHITE);
    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    Text_Box_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume. ?!", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_BoW_french.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    Text_Box_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME. ?! ", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_BoW_FRENCH.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Pixelnours_big - */
    PixelFont_Free(bubble.pixelfont, true);
    bubble.pixelfont = PixelFont_Alloc();
    Text_Bubble_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_Big.png"));
    PixelFont_Glyph_yOffset_W(bubble.pixelfont, pixelfont_big_y_offset);

    PixelFont_Swap_Palette(bubble.pixelfont, renderer, SOTA_BLACK, SOTA_WHITE);
    bubble.line_len_px  = 96;
    bubble.row_height   = ASCII_GLYPH_HEIGHT + 2;
    bubble.padding.top  = TEXT_BOX_PADDING_TOP + 2;

    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    Text_Box_Set_All(&bubble, "a quick brown fox jumps over the lazy dog. ?!", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_Big_BoW_english.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    Text_Box_Set_All(&bubble, "A QUICK BROWN FOX JUMPS OVER THE LAZY DOG. ?! ", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_Big_BoW_ENGLISH.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Bubble with blue 9patch -- */
    Text_Box_Colors_Swap(&bubble, renderer, &n9patch);
    char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
    SDL_DestroyTexture(n9patch.texture);
    n9patch.texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(n9patch.texture != NULL);
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, SOTA_WHITE, -1);

    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    Text_Box_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume. ?!", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_french.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    Text_Box_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME. ?! ", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_FRENCH.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Pixelnours_big - */
    Text_Bubble_Load(&bubble, renderer, &n9patch);
    path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
    SDL_DestroyTexture(n9patch.texture);
    n9patch.texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(n9patch.texture != NULL);

    PixelFont_Free(bubble.pixelfont, true);
    bubble.pixelfont = PixelFont_Alloc();
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_Big.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, -1, -1);
    PixelFont_Glyph_yOffset_W(bubble.pixelfont, pixelfont_big_y_offset);

    bubble.line_len_px  = 96;
    bubble.row_height   = ASCII_GLYPH_HEIGHT + 2;
    bubble.padding.top  = TEXT_BOX_PADDING_TOP + 2;

    /* - setting - */
    Text_Box_Colors_Set(&bubble, SOTA_MENU_BLUE, SOTA_WHITE);
    Text_Box_Colors_Swap(&bubble, renderer, &n9patch);

    bubble.target.x = -100;
    bubble.target.y = -100;
    Text_Box_Set_All(&bubble, "a quick brown fox jumps over the lazy dog. ?!", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_Big_english.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    Text_Box_Set_All(&bubble, "A QUICK BROWN FOX JUMPS OVER THE LAZY DOG. ?! ", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_Big_ENGLISH.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* SDL_free */
    PixelFont_Free(bubble.pixelfont, true);
    Text_Box_Free(&bubble);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}

void test_pixelfonts() {
    sota_mkdir("pixelfont");
    test_pixelfonts_internals();
    test_pixelfonts_render();
}