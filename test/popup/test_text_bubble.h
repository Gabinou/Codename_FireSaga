#include "nourstest.h"
#include "text_bubble.h"

void test_Text_Bubble_Tail() {
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
    bubble.pixelfont->y_offset     = pixelfont_y_offset;
    TextBubble_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts", "pixelnours.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, 1, 55);

    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    TextBubble_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);
    SDL_assert(bubble.tail.index     == TEXT_BUBBLE_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_HelloWorld.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = 10;
    TextBubble_Set_All(&bubble, "Another oneliner.", bubble.target, &n9patch);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_LEFT);
    SDL_assert(bubble.tail.index     == TEXT_BUBBLE_STRAIGHT);
    SDL_assert(bubble.tail.angle     == 90.0);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_1line.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_All(&bubble, "Another oneliner.", bubble.target, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_1line_tailR.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = - bubble.width;
    bubble.target.y = bubble.height * 2;
    TextBubble_Set_All(&bubble, "A", bubble.target, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_A.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width / 2;
    bubble.target.y = - bubble.height;
    TextBubble_Set_All(&bubble, "I am a 2 line long speech. Brief.", bubble.target, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_2lines.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_All(&bubble, "I am a 2 line long speech. Brief.", bubble.target, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_2lines_tailR.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume.", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_pixelnours_minus.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME.", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_pixelnours_majus.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);


    /* - setting - */
    TextBubble_Set_Text(&bubble, "I am a 2 line long speech. Brief.", &n9patch);
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Tail_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_2lines_tailL.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_All(&bubble, "A quick brown fox jumps over the lazy dog", bubble.target,
                       &n9patch);
    bubble.target.x = bubble.width / 2;
    bubble.target.y = 2 * bubble.height;
    TextBubble_Set_All(&bubble, "A quick brown fox jumps over the lazy dog", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_BrownFox.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Pixelnours_big - */
    PixelFont_Free(bubble.pixelfont, true);
    TextBubble_Load(&bubble, renderer, &n9patch);
    bubble.pixelfont = PixelFont_Alloc();
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_Big.png"));
    bubble.pixelfont->y_offset     = pixelfont_big_y_offset;
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, 1, 55);
    bubble.line_len_px  = 96;
    bubble.row_height   = bubble.pixelfont->glyph_height + 2;
    bubble.padding.top  = TEXT_BUBBLE_PADDING_TOP + 2;

    /* - setting - */
    bubble.target.x = bubble.width / 2;
    bubble.target.y = bubble.height * 2;
    TextBubble_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_Big_HelloWorld.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2;
    bubble.target.y = -bubble.height;
    TextBubble_Set_All(&bubble, "Another oneliner.", bubble.target, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_Big_1line.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_All(&bubble, "A", bubble.target, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_Big_A.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height * 2;
    TextBubble_Set_All(&bubble, "I am a 2 line long speech. Brief.", bubble.target, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_Big_2lines.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 4;
    bubble.target.y = bubble.height * 4;
    TextBubble_Set_All(&bubble, "A quick brown fox jumps over the lazy dog", bubble.target,
                       &n9patch);
    SDL_assert(bubble.lines.line_num == 3);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_Big_BrownFox.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_All(&bubble, "A quick brown fox jumps over the lazy dog.", bubble.target,
                       &n9patch);


    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_3lines_tailR3.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width * 2;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_All(&bubble, "A quick brown fox jumps over the lazy dog.", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_3lines_tailL3.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height / 3;
    TextBubble_Set_All(&bubble, "A quick brown fox jumps over the lazy dog.", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_3lines_tailR2.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width * 2;
    bubble.target.y = bubble.height / 3;
    TextBubble_Set_All(&bubble, "A quick brown fox jumps over the lazy dog.", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_3lines_tailL2.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2;
    bubble.target.y = 2 * bubble.height / 3;
    TextBubble_Set_All(&bubble, "A quick brown fox jumps over the lazy dog\0", bubble.target,
                       &n9patch);
    bubble.target.x = bubble.width * 2;
    bubble.target.y = 2 * bubble.height / 3;
    TextBubble_Set_All(&bubble, "A quick brown fox jumps over the lazy dog", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_3lines_tailR1.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width * 2;
    bubble.target.y = 2 * bubble.height / 3;
    TextBubble_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME.", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_3lines_tailL1.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width / 3;
    bubble.target.y = bubble.height * 2;
    TextBubble_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_tailT1.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width / 3;
    bubble.target.y = -bubble.height * 2;
    TextBubble_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_tailB1.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);


    /* - setting - */
    bubble.target.x = bubble.width / 2;
    bubble.target.y = bubble.height * 2;
    TextBubble_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_tailT2.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width / 2;
    bubble.target.y = -bubble.height * 2;
    TextBubble_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_tailB2.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);


    /* - setting - */
    bubble.target.x = bubble.width * 2 / 3;
    bubble.target.y = bubble.height * 2;
    TextBubble_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_tailT3.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2 / 3;
    bubble.target.y = -bubble.height * 2;
    TextBubble_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_tailB3.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* FREE */
    PixelFont_Free(bubble.pixelfont, true);
    TextBubble_Free(&bubble);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}

void test_text_bubble_scroll() {
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
    bubble.pixelfont->y_offset     = pixelfont_big_y_offset;
    TextBubble_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_Big.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, 1, 55);
    bubble.pixelfont->scroll_speed = 0;
    bubble.line_len_px  = 96;
    bubble.row_height   = bubble.pixelfont->glyph_height + 2;
    bubble.padding.top  = TEXT_BUBBLE_PADDING_TOP + 2;

    /* - setting - */
    TextBubble_Set_Text(&bubble, "portez ce vieux whisky au juge blond qui fume.", &n9patch);
    bubble.target.x = -bubble.width;
    bubble.target.y = -bubble.height;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.tail.index     == TEXT_BUBBLE_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);
    TextBubble_Tail_Pos(&bubble, &n9patch);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    bubble.scroll = true;
    int i = 0;
    char path_raw[128] = PATH_JOIN("popup_text_bubble", "TextBubble_Scroll_%02d.png");
    char path[128];
    size_t scroll_lim = bubble.text.num - bubble.lines.line_num + 1;
    while (bubble.pixelfont->scroll_len <= scroll_lim) {
        sprintf(path, path_raw, i);

        /* - rendering - */
        TextBubble_Update(&bubble, &n9patch, render_target, renderer);
        Filesystem_Texture_Dump(path, renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

        /* - scrolling - */
        PixelFont_Scroll(bubble.pixelfont, 1);
        i++;
    }

    /* FREE */
    PixelFont_Free(bubble.pixelfont, true);
    TextBubble_Free(&bubble);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}

void test_text_bubble_scroll_vertical() {
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
    bubble.pixelfont->y_offset     = pixelfont_big_y_offset;
    TextBubble_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_Big.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, 1, 55);
    bubble.pixelfont->scroll_speed = 0;
    bubble.line_len_px  = 96;
    bubble.row_height   = bubble.pixelfont->glyph_height + 2;
    bubble.padding.top  = TEXT_BUBBLE_PADDING_TOP + 2;
    bubble.line_num_max = 2;

    /* - setting - */
    TextBubble_Set_Text(&bubble,
                        "A quick brown fox jumps over the lazy dog. Twice and thrice and once another! Open your eyes and call your mother!",
                        &n9patch);
    bubble.target.x = -bubble.width;
    bubble.target.y = -bubble.height;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.tail.index     == TEXT_BUBBLE_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);
    TextBubble_Tail_Pos(&bubble, &n9patch);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    bubble.scroll = true;
    int i = 0;
    char path_raw[128] = PATH_JOIN("popup_text_bubble", "TextBubble_Text_Scroll_%03d.png");
    char path[128];
    size_t scroll_lim = bubble.text.num - bubble.lines.line_num + 1;
    while (bubble.pixelfont->scroll_len <= scroll_lim) {
        sprintf(path, path_raw, i);

        /* - rendering - */
        TextBubble_Update(&bubble, &n9patch, render_target, renderer);
        Filesystem_Texture_Dump(path, renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

        /* - scrolling - */
        PixelFont_Scroll(bubble.pixelfont, 1);
        i++;
    }

    /* FREE */
    PixelFont_Free(bubble.pixelfont, true);
    TextBubble_Free(&bubble);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}

void test_text_bubble_VScroll_Anim() {
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
    bubble.pixelfont->y_offset     = pixelfont_big_y_offset;
    TextBubble_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_Big.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, 1, 55);
    bubble.pixelfont->scroll_speed = 0;
    bubble.line_len_px  = 72;
    bubble.row_height   = bubble.pixelfont->glyph_height + 2;
    bubble.padding.top  = TEXT_BUBBLE_PADDING_TOP + 2;
    bubble.line_num_max = 0;

    /* - setting - */
    TextBubble_Set_Text(&bubble, "A quick brown fox jumps over the lazy dog.", &n9patch);
    bubble.target.x = -bubble.width;
    bubble.target.y = -bubble.height;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.tail.index     == TEXT_BUBBLE_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);
    TextBubble_Tail_Pos(&bubble, &n9patch);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    bubble.scroll = false;

    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    char *temp_path = PATH_JOIN("popup_text_bubble",  "Popup_TextBubble_VScroll_Original.png");
    Filesystem_Texture_Dump(temp_path, renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);
    TextBubble_Copy_VScroll(&bubble, renderer, render_target);
    SDL_assert(bubble.texture_vscroll != NULL);
    temp_path = PATH_JOIN("popup_text_bubble",  "Popup_TextBubble_VScroll_Copy.png");
    Filesystem_Texture_Dump(temp_path, renderer, bubble.texture_vscroll, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Actually scrolling animation -- */
    char path_raw[128] = PATH_JOIN("popup_text_bubble", "TextBubble_VScroll_%03d.png");
    char path[128];
    bubble.vscroll_anim  = true;
    bubble.vscroll_dir   = TEXT_BUBBLE_VSCROLL_TOP;
    int i = 0;
    while (bubble.vscroll_anim) {
        sprintf(path, path_raw, i);

        /* - rendering - */
        TextBubble_Update(&bubble, &n9patch, render_target, renderer);
        Filesystem_Texture_Dump(path, renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

        /* - scrolling - */
        TextBubble_VScroll(&bubble, renderer);
        i++;
    }

    /* FREE */
    PixelFont_Free(bubble.pixelfont, true);
    TextBubble_Free(&bubble);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}

void test_Text_Bubble_pixelfont16() {
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
    bubble.pixelfont->glyph_height  = 16;
    bubble.pixelfont->glyph_width   = 16;
    TextBubble_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_16.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, 1, 55);
    bubble.pixelfont->scroll_speed = 0;
    bubble.line_len_px  = 96;
    bubble.row_height   = bubble.pixelfont->glyph_height;
    bubble.padding.top  = TEXT_BUBBLE_PADDING_TOP + 2;
    bubble.line_num_max = 0;

    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    TextBubble_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);
    SDL_assert(bubble.tail.index     == TEXT_BUBBLE_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_16_HelloWorld.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume.", bubble.target,
                       &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_pixelnours_16_minus.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME.", bubble.target,
                       &n9patch);


    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_pixelnours_16_majus.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);


}

void test_text_bubble() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    sota_mkdir("popup_text_bubble");
    test_text_bubble_VScroll_Anim();
    test_Text_Bubble_Tail();
    test_text_bubble_scroll();
    test_text_bubble_scroll_vertical();
    test_Text_Bubble_pixelfont16();
}
