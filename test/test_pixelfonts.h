#include "nourstest.h"
#include "pixelfonts.h"
#include "text_bubble.h"

void test_pixelfonts_internals() {

    SDL_assert(palette_SOTA != NULL);

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    struct PixelFont *test_font = PixelFont_Alloc();
    char *path = PATH_JOIN("..", "assets", "fonts", "pixelnours_test.png");
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
    text_lines = PixelFont_Lines_Len(test_font, text4, line_len_px);
    line_num   = PixelFont_Lines_Num_Len(test_font, text4, line_len_px);
    nourstest_true(line_num == 6);
    nourstest_true(text_lines.line_num == 6);
    nourstest_true(text_lines.line_len == 8);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[0]) < line_len_px);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[1]) < line_len_px);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[2]) < line_len_px);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[3]) < line_len_px);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[4]) < line_len_px);
    nourstest_true(PixelFont_Width_Len(test_font, text_lines.lines[5]) < line_len_px);
    // nourstest_true(s8equal(&text_lines.lines[0], "Conglomerate a") == 0);
    // nourstest_true(s8equal(&text_lines.lines[1], "rock y baka aggl-") == 0);
    // nourstest_true(s8equal(&text_lines.lines[2], "omeration bakan-") == 0);
    // nourstest_true(s8equal(&text_lines.lines[3], "umeration stupi-") == 0);
    // nourstest_true(s8equal(&text_lines.lines[4], "dification negati-") == 0);
    // nourstest_true(s8equal(&text_lines.lines[5], "vitiation.") == 0);

    /* -- FREE -- */
    PixelFont_Free(test_font, true);
    TextLines_Free(&text_lines);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}

void test_pixelfonts_render() {
    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    struct Text_Bubble bubble = TextBubble_default;

    /* -- Create n9patch -- */
    // render_target is NULL cause there is render_target!
    struct n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;
    /* - Pixelnours - */
    bubble.pixelfont = PixelFont_Alloc();
    bubble.pixelfont->y_offset = pixelfont_y_offset;
    TextBubble_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts", "pixelnours.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, SOTA_BLACK, SOTA_WHITE);
    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    TextBubble_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume. ?!", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_BoW_french.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME. ?! ", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_BoW_FRENCH.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Pixelnours_big - */
    PixelFont_Free(bubble.pixelfont, true);
    bubble.pixelfont = PixelFont_Alloc();
    TextBubble_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_Big.png"));
    bubble.pixelfont->y_offset     = pixelfont_big_y_offset;
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, SOTA_BLACK, SOTA_WHITE);
    bubble.line_len_px  = 96;
    bubble.row_height   = ASCII_GLYPH_HEIGHT + 2;
    bubble.padding.top  = TEXT_BUBBLE_PADDING_TOP + 2;

    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    TextBubble_Set_All(&bubble, "a quick brown fox jumps over the lazy dog. ?!", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_Big_BoW_english.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_All(&bubble, "A QUICK BROWN FOX JUMPS OVER THE LAZY DOG. ?! ", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_Big_BoW_ENGLISH.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Bubble with blue 9patch -- */
    TextBubble_Colors_Swap(&bubble, renderer, &n9patch);
    char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
    SDL_DestroyTexture(n9patch.texture);
    n9patch.texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(n9patch.texture != NULL);
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, SOTA_WHITE, -1);

    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    TextBubble_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume. ?!", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_french.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME. ?! ", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_FRENCH.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Pixelnours_big - */
    TextBubble_Load(&bubble, renderer, &n9patch);
    path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
    SDL_DestroyTexture(n9patch.texture);
    n9patch.texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(n9patch.texture != NULL);

    PixelFont_Free(bubble.pixelfont, true);
    bubble.pixelfont = PixelFont_Alloc();
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_Big.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, -1, -1);
    bubble.pixelfont->y_offset     = pixelfont_big_y_offset;
    bubble.line_len_px  = 96;
    bubble.row_height   = ASCII_GLYPH_HEIGHT + 2;
    bubble.padding.top  = TEXT_BUBBLE_PADDING_TOP + 2;

    /* - setting - */
    TextBubble_Colors_Set(&bubble, SOTA_MENU_BLUE, SOTA_WHITE);
    TextBubble_Colors_Swap(&bubble, renderer, &n9patch);

    bubble.target.x = -100;
    bubble.target.y = -100;
    TextBubble_Set_All(&bubble, "a quick brown fox jumps over the lazy dog. ?!", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_Big_english.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_All(&bubble, "A QUICK BROWN FOX JUMPS OVER THE LAZY DOG. ?! ", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("pixelfont", "Pixelnours_Big_ENGLISH.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* FREE */
    PixelFont_Free(bubble.pixelfont, true);
    TextBubble_Free(&bubble);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}

void test_pixelfonts() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    sota_mkdir("pixelfont");
    test_pixelfonts_internals();
    test_pixelfonts_render();
}