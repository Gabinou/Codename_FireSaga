
#include "nourstest.h"
#include "text_box.h"

void test_TEXT_BOX_Tail() {
    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    struct Text_Box bubble = TEXT_BOX_Default;

    /* -- Create n9patch -- */
    // render_target is NULL cause there is render_target!
    struct n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;
    /* - Pixelnours - */
    bubble.pixelfont = PixelFont_Alloc();
    bubble.pixelfont->y_offset     = pixelfont_y_offset;
    Text_Box_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts", "pixelnours.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, SOTA_BLACK, SOTA_WHITE);

    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);
    SDL_assert(bubble.tail.index     == TEXT_BOX_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_HelloWorld.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = 10;
    TEXT_BOX_Set_All(&bubble, "Another oneliner.", bubble.target, &n9patch);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_LEFT);
    SDL_assert(bubble.tail.index     == TEXT_BOX_STRAIGHT);
    SDL_assert(bubble.tail.angle     == 90.0);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_1line.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height / 2;
    TEXT_BOX_Set_All(&bubble, "Another oneliner.", bubble.target, &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_1line_tailR.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = - bubble.width;
    bubble.target.y = bubble.height * 2;
    TEXT_BOX_Set_All(&bubble, "A", bubble.target, &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_A.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width / 2;
    bubble.target.y = - bubble.height;
    TEXT_BOX_Set_All(&bubble, "I am a 2 line long speech. Brief.", bubble.target, &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_2lines.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    TEXT_BOX_Set_All(&bubble, "I am a 2 line long speech. Brief.", bubble.target, &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_2lines_tailR.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    TEXT_BOX_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_Pixelnours_minus.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    TEXT_BOX_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_Pixelnours_majus.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TEXT_BOX_Set_Text(&bubble, "I am a 2 line long speech. Brief.", &n9patch);
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height / 2;
    TEXT_BOX_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TEXT_BOX_Tail_Pos(&bubble, &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_2lines_tailL.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TEXT_BOX_Set_All(&bubble, "A quick brown fox jumps over the lazy dog", bubble.target,
                     &n9patch);
    bubble.target.x = bubble.width / 2;
    bubble.target.y = 2 * bubble.height;
    TEXT_BOX_Set_All(&bubble, "A quick brown fox jumps over the lazy dog", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_BrownFox.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Pixelnours_big - */
    PixelFont_Free(bubble.pixelfont, true);
    Text_Box_Load(&bubble, renderer, &n9patch);
    bubble.pixelfont = PixelFont_Alloc();
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_Big.png"));
    bubble.pixelfont->y_offset     = pixelfont_big_y_offset;
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, SOTA_BLACK, SOTA_WHITE);
    bubble.line_len_px  = 96;
    bubble.row_height   = bubble.pixelfont->glyph_height + 2;
    bubble.padding.top  = TEXT_BOX_PADDING_TOP + 2;

    /* - setting - */
    bubble.target.x = bubble.width / 2;
    bubble.target.y = bubble.height * 2;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_Big_HelloWorld.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2;
    bubble.target.y = -bubble.height;
    TEXT_BOX_Set_All(&bubble, "Another oneliner.", bubble.target, &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_Big_1line.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height / 2;
    TEXT_BOX_Set_All(&bubble, "A", bubble.target, &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_Big_A.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height * 2;
    TEXT_BOX_Set_All(&bubble, "I am a 2 line long speech. Brief.", bubble.target, &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_Big_2lines.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 4;
    bubble.target.y = bubble.height * 4;
    TEXT_BOX_Set_All(&bubble, "A quick brown fox jumps over the lazy dog", bubble.target,
                     &n9patch);
    SDL_assert(bubble.lines.line_num == 3);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_Big_BrownFox.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height / 2;
    TEXT_BOX_Set_All(&bubble, "A quick brown fox jumps over the lazy dog.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_3lines_tailR3.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width * 2;
    bubble.target.y = bubble.height / 2;
    TEXT_BOX_Set_All(&bubble, "A quick brown fox jumps over the lazy dog.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_3lines_tailL3.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height / 3;
    TEXT_BOX_Set_All(&bubble, "A quick brown fox jumps over the lazy dog.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_3lines_tailR2.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width * 2;
    bubble.target.y = bubble.height / 3;
    TEXT_BOX_Set_All(&bubble, "A quick brown fox jumps over the lazy dog.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_3lines_tailL2.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2;
    bubble.target.y = 2 * bubble.height / 3;
    TEXT_BOX_Set_All(&bubble, "A quick brown fox jumps over the lazy dog\0", bubble.target,
                     &n9patch);
    bubble.target.x = bubble.width * 2;
    bubble.target.y = 2 * bubble.height / 3;
    TEXT_BOX_Set_All(&bubble, "A quick brown fox jumps over the lazy dog", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_3lines_tailR1.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width * 2;
    bubble.target.y = 2 * bubble.height / 3;
    TEXT_BOX_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_3lines_tailL1.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width / 3;
    bubble.target.y = bubble.height * 2;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_TailT1.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width / 3;
    bubble.target.y = -bubble.height * 2;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_TailB1.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width / 2;
    bubble.target.y = bubble.height * 2;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_TailT2.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width / 2;
    bubble.target.y = -bubble.height * 2;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_TailB2.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2 / 3;
    bubble.target.y = bubble.height * 2;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_TailT3.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2 / 3;
    bubble.target.y = -bubble.height * 2;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_TailB3.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* SDL_free */
    PixelFont_Free(bubble.pixelfont, true);
    Text_Box_Free(&bubble);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}

void test_TEXT_BOX_Scroll() {
    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    struct Text_Box bubble = TEXT_BOX_Default;

    /* -- Create n9patch -- */
    // render_target is NULL cause there is render_target!
    struct n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;
    /* - Pixelnours - */
    bubble.pixelfont = PixelFont_Alloc();
    bubble.pixelfont->y_offset     = pixelfont_big_y_offset;
    Text_Box_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_Big.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, SOTA_BLACK, SOTA_WHITE);
    bubble.pixelfont->scroll_speed = 0;
    bubble.line_len_px  = 96;
    bubble.row_height   = bubble.pixelfont->glyph_height + 2;
    bubble.padding.top  = TEXT_BOX_PADDING_TOP + 2;

    /* - setting - */
    TEXT_BOX_Set_Text(&bubble, "portez ce vieux whisky au juge blond qui fume.", &n9patch);
    bubble.target.x = -bubble.width;
    bubble.target.y = -bubble.height;
    TEXT_BOX_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.tail.index     == TEXT_BOX_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);
    TEXT_BOX_Tail_Pos(&bubble, &n9patch);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    bubble.scroll = true;
    int i = 0;
    char path_raw[128] = PATH_JOIN("popup_Text_Box", "TEXT_BOX_Scroll_%02d.png");
    char path[128];
    size_t scroll_lim = bubble.text.num - bubble.lines.line_num + 1;
    while (bubble.pixelfont->scroll_len <= scroll_lim) {
        sprintf(path, path_raw, i);

        /* - rendering - */
        Text_Box_Update(&bubble, &n9patch, render_target, renderer);
        Filesystem_Texture_Dump(path, renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

        /* - scrolling - */
        PixelFont_Scroll(bubble.pixelfont, 1);
        i++;
    }

    /* SDL_free */
    PixelFont_Free(bubble.pixelfont, true);
    Text_Box_Free(&bubble);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}

void test_TEXT_BOX_Scroll_vertical() {
    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    struct Text_Box bubble = TEXT_BOX_Default;

    /* -- Create n9patch -- */
    // render_target is NULL cause there is render_target!
    struct n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;
    /* - Pixelnours - */
    bubble.pixelfont = PixelFont_Alloc();
    bubble.pixelfont->y_offset     = pixelfont_big_y_offset;
    Text_Box_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_Big.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, SOTA_BLACK, SOTA_WHITE);
    bubble.pixelfont->scroll_speed = 0;
    bubble.line_len_px  = 96;
    bubble.row_height   = bubble.pixelfont->glyph_height + 2;
    bubble.padding.top  = TEXT_BOX_PADDING_TOP + 2;
    bubble.line_num_max = 2;

    /* - setting - */
    TEXT_BOX_Set_Text(&bubble,
                      "A quick brown fox jumps over the lazy dog. Twice and thrice and once another! Open your eyes and call your mother!",
                      &n9patch);
    bubble.target.x = -bubble.width;
    bubble.target.y = -bubble.height;
    TEXT_BOX_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.tail.index     == TEXT_BOX_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);
    TEXT_BOX_Tail_Pos(&bubble, &n9patch);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    bubble.scroll = true;
    int i = 0;
    char path_raw[128] = PATH_JOIN("popup_Text_Box", "TEXT_BOX_Text_Scroll_%03d.png");
    char path[128];
    size_t scroll_lim = bubble.text.num - bubble.lines.line_num + 1;
    while (bubble.pixelfont->scroll_len <= scroll_lim) {
        sprintf(path, path_raw, i);

        /* - rendering - */
        Text_Box_Update(&bubble, &n9patch, render_target, renderer);
        Filesystem_Texture_Dump(path, renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

        /* - scrolling - */
        PixelFont_Scroll(bubble.pixelfont, 1);
        i++;
    }

    /* SDL_free */
    PixelFont_Free(bubble.pixelfont, true);
    Text_Box_Free(&bubble);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}

void test_TEXT_BOX_VScroll_Anim() {
    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    struct Text_Box bubble = TEXT_BOX_Default;

    /* -- Create n9patch -- */
    // render_target is NULL cause there is render_target!
    struct n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;
    /* - Pixelnours - */
    bubble.pixelfont = PixelFont_Alloc();
    bubble.pixelfont->y_offset     = pixelfont_big_y_offset;
    Text_Box_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_Big.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, SOTA_BLACK, SOTA_WHITE);
    bubble.pixelfont->scroll_speed = 0;
    bubble.line_len_px  = 72;
    bubble.row_height   = bubble.pixelfont->glyph_height + 2;
    bubble.padding.top  = TEXT_BOX_PADDING_TOP + 2;
    bubble.line_num_max = 0;

    /* - setting - */
    TEXT_BOX_Set_Text(&bubble, "A quick brown fox jumps over the lazy dog.", &n9patch);
    bubble.target.x = -bubble.width;
    bubble.target.y = -bubble.height;
    TEXT_BOX_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.tail.index     == TEXT_BOX_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);
    TEXT_BOX_Tail_Pos(&bubble, &n9patch);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    bubble.scroll = false;

    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    char *temp_path = PATH_JOIN("popup_Text_Box",  "Popup_TEXT_BOX_VScroll_Original.png");
    Filesystem_Texture_Dump(temp_path, renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);
    TEXT_BOX_Copy_VScroll(&bubble, renderer, render_target);
    SDL_assert(bubble.texture_vscroll != NULL);
    temp_path = PATH_JOIN("popup_Text_Box",  "Popup_TEXT_BOX_VScroll_Copy.png");
    Filesystem_Texture_Dump(temp_path, renderer, bubble.texture_vscroll, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Actually scrolling animation -- */
    char path_raw[128] = PATH_JOIN("popup_Text_Box", "TEXT_BOX_VScroll_%03d.png");
    char path[128];
    bubble.vscroll_anim  = true;
    bubble.vscroll_dir   = TEXT_BOX_VSCROLL_TOP;
    int i = 0;
    while (bubble.vscroll_anim) {
        sprintf(path, path_raw, i);

        /* - rendering - */
        Text_Box_Update(&bubble, &n9patch, render_target, renderer);
        Filesystem_Texture_Dump(path, renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

        /* - scrolling - */
        TEXT_BOX_VScroll(&bubble, renderer);
        i++;
    }

    /* SDL_free */
    PixelFont_Free(bubble.pixelfont, true);
    Text_Box_Free(&bubble);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}

void test_TEXT_BOX_Pixelfont16() {
    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    struct Text_Box bubble = TEXT_BOX_Default;

    /* -- Create n9patch -- */
    // render_target is NULL cause there is render_target!
    struct n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;

    /* - Pixelnours 16 - */
    bubble.pixelfont = PixelFont_Alloc();
    bubble.pixelfont->glyph_height  = 16;
    bubble.pixelfont->glyph_width   = 16;
    Text_Box_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_16.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, SOTA_BLACK, SOTA_WHITE);
    bubble.pixelfont->scroll_speed = 0;
    bubble.line_len_px      = 128;
    bubble.row_height       = bubble.pixelfont->glyph_height;
    bubble.padding.top      = TEXT_BOX_PADDING_TOP;
    bubble.padding.bottom   = 0;
    bubble.line_num_max     = 0;

    /* --- RENDERING BUBBLES --- */
    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);
    SDL_assert(bubble.tail.index     == TEXT_BOX_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_16_HelloWorld.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.padding.bottom   = 1;
    TEXT_BOX_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_Pixelnours_16_minus.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.line_len_px = 164;
    TEXT_BOX_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_Pixelnours_16_majus.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- Setting BLUE bg for text bubble --- */
    TEXT_BOX_Colors_Set(&bubble, SOTA_MENU_BLUE, SOTA_WHITE);
    TEXT_BOX_Colors_Swap(&bubble, renderer, &n9patch);

    /* - Pixelnours 16 for BLUE - */
    PixelFont_Free(bubble.pixelfont, true);
    bubble.pixelfont = PixelFont_Alloc();
    bubble.pixelfont->glyph_height  = 16;
    bubble.pixelfont->glyph_width   = 16;
    Text_Box_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_16.png"));
    bubble.pixelfont->scroll_speed = 0;
    bubble.line_len_px      = 128;
    bubble.row_height       = bubble.pixelfont->glyph_height;
    bubble.padding.top      = TEXT_BOX_PADDING_TOP + 2;
    bubble.padding.bottom   = 1;
    bubble.line_num_max     = 0;

    /* --- RENDERING BUBBLES --- */
    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);
    SDL_assert(bubble.tail.index     == TEXT_BOX_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_16_HelloWorld_Blue.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.padding.bottom   = 3;
    bubble.line_len_px = 128;
    TEXT_BOX_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_Pixelnours_16_minus_Blue.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.padding.bottom = 0;
    bubble.line_len_px = 164;
    TEXT_BOX_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME.", bubble.target,
                     &n9patch);
    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_Pixelnours_16_majus_Blue.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- Setting BLUE bg for text bubble --- */
    TEXT_BOX_Colors_Set(&bubble, SOTA_WHITE, SOTA_BLACK);
    TEXT_BOX_Colors_Swap(&bubble, renderer, &n9patch);
    TEXT_BOX_Colors_Set(&bubble, SOTA_BLACK, SOTA_WHITE);
    TEXT_BOX_Colors_Swap(&bubble, renderer, &n9patch);

    bubble.pixelfont->scroll_speed  = 0;
    bubble.line_len_px              = 128;
    bubble.row_height               = bubble.pixelfont->glyph_height;
    bubble.padding.top              = TEXT_BOX_PADDING_TOP + 2;
    bubble.padding.bottom           = 1;
    bubble.line_num_max             = 0;

    /* --- RENDERING BUBBLES --- */
    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);
    SDL_assert(bubble.tail.index     == TEXT_BOX_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_16_HelloWorld_Black.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.padding.bottom   = 3;
    bubble.line_len_px = 128;
    TEXT_BOX_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_Pixelnours_16_minus_Black.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.padding.bottom = 0;
    bubble.line_len_px = 164;
    TEXT_BOX_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME.", bubble.target,
                     &n9patch);
    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_Pixelnours_16_majus_Black.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    /* SDL_free */
    PixelFont_Free(bubble.pixelfont, true);
    Text_Box_Free(&bubble);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}

void test_TEXT_BOX_Pixelfont16_tight() {
    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    struct Text_Box bubble = TEXT_BOX_Default;

    /* -- Create n9patch -- */
    // render_target is NULL cause there is render_target!
    struct n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;

    /* - Pixelnours 16 - */
    bubble.pixelfont = PixelFont_Alloc();
    bubble.pixelfont->glyph_height  = 16;
    bubble.pixelfont->glyph_width   = 16;
    Text_Box_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_16_tight.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, SOTA_BLACK, SOTA_WHITE);
    bubble.pixelfont->scroll_speed = 0;
    bubble.line_len_px      = 128;
    bubble.row_height       = bubble.pixelfont->glyph_height;
    bubble.padding.top      = TEXT_BOX_PADDING_TOP;
    bubble.padding.bottom   = 0;
    bubble.line_num_max     = 0;

    /* --- RENDERING BUBBLES --- */
    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);
    SDL_assert(bubble.tail.index     == TEXT_BOX_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_16_tight_HelloWorld.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.padding.bottom   = 1;
    TEXT_BOX_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_Pixelnours_16_tight_minus.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.line_len_px = 164;
    TEXT_BOX_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_Pixelnours_16_tight_majus.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- Setting BLUE bg for text bubble --- */
    TEXT_BOX_Colors_Set(&bubble, SOTA_MENU_BLUE, SOTA_WHITE);
    TEXT_BOX_Colors_Swap(&bubble, renderer, &n9patch);

    /* - Pixelnours 16 for BLUE - */
    PixelFont_Free(bubble.pixelfont, true);
    bubble.pixelfont = PixelFont_Alloc();
    bubble.pixelfont->glyph_height  = 16;
    bubble.pixelfont->glyph_width   = 16;
    Text_Box_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_16_tight.png"));
    bubble.pixelfont->scroll_speed = 0;
    bubble.line_len_px      = 128;
    bubble.row_height       = bubble.pixelfont->glyph_height;
    bubble.padding.top      = TEXT_BOX_PADDING_TOP + 2;
    bubble.padding.bottom   = 1;
    bubble.line_num_max     = 0;

    /* --- RENDERING BUBBLES --- */
    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);
    SDL_assert(bubble.tail.index     == TEXT_BOX_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_16_tight_HelloWorld_Blue.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.padding.bottom   = 3;
    bubble.line_len_px = 128;
    TEXT_BOX_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box",
                                      "TEXT_BOX_Pixelnours_16_tight_minus_Blue.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.padding.bottom = 0;
    bubble.line_len_px = 164;
    TEXT_BOX_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME.", bubble.target,
                     &n9patch);
    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box",
                                      "TEXT_BOX_Pixelnours_16_tight_majus_Blue.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- Setting BLUE bg for text bubble --- */
    TEXT_BOX_Colors_Set(&bubble, SOTA_WHITE, SOTA_BLACK);
    TEXT_BOX_Colors_Swap(&bubble, renderer, &n9patch);
    TEXT_BOX_Colors_Set(&bubble, SOTA_BLACK, SOTA_WHITE);
    TEXT_BOX_Colors_Swap(&bubble, renderer, &n9patch);

    bubble.pixelfont->scroll_speed  = 0;
    bubble.line_len_px              = 128;
    bubble.row_height               = bubble.pixelfont->glyph_height;
    bubble.padding.top              = TEXT_BOX_PADDING_TOP + 2;
    bubble.padding.bottom           = 1;
    bubble.line_num_max             = 0;

    /* --- RENDERING BUBBLES --- */
    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);
    SDL_assert(bubble.tail.index     == TEXT_BOX_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_16_tight_HelloWorld_Black.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.padding.bottom   = 3;
    bubble.line_len_px = 128;
    TEXT_BOX_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box",
                                      "TEXT_BOX_Pixelnours_16_tight_minus_Black.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.padding.bottom = 0;
    bubble.line_len_px = 164;
    TEXT_BOX_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME.", bubble.target,
                     &n9patch);
    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box",
                                      "TEXT_BOX_Pixelnours_16_tight_majus_Black.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    /* SDL_free */
    PixelFont_Free(bubble.pixelfont, true);
    Text_Box_Free(&bubble);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}

void test_TEXT_BOX_Pixelfont16_minus() {
    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    struct Text_Box bubble = TEXT_BOX_Default;

    /* -- Create n9patch -- */
    // render_target is NULL cause there is render_target!
    struct n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;

    /* - Pixelnours 16 - */
    bubble.pixelfont = PixelFont_Alloc();
    bubble.pixelfont->glyph_height  = 16;
    bubble.pixelfont->glyph_width   = 16;
    Text_Box_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_16.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, SOTA_BLACK, SOTA_WHITE);
    bubble.pixelfont->scroll_speed = 0;
    bubble.pixelfont->glyph_space  = -1;
    bubble.line_len_px      = 128;
    bubble.row_height       = bubble.pixelfont->glyph_height;
    bubble.padding.top      = TEXT_BOX_PADDING_TOP;
    bubble.padding.bottom   = 0;
    bubble.line_num_max     = 0;

    /* --- RENDERING BUBBLES --- */
    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);
    SDL_assert(bubble.tail.index     == TEXT_BOX_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_16_minus_HelloWorld.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.padding.bottom   = 1;
    TEXT_BOX_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_Pixelnours_16_minus_minus.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.line_len_px = 164;
    TEXT_BOX_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "TEXT_BOX_Pixelnours_16_minus_majus.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- Setting BLUE bg for text bubble --- */
    TEXT_BOX_Colors_Set(&bubble, SOTA_MENU_BLUE, SOTA_WHITE);
    TEXT_BOX_Colors_Swap(&bubble, renderer, &n9patch);

    /* - Pixelnours 16 for BLUE - */
    PixelFont_Free(bubble.pixelfont, true);
    bubble.pixelfont = PixelFont_Alloc();
    bubble.pixelfont->glyph_height  = 16;
    bubble.pixelfont->glyph_width   = 16;
    Text_Box_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "fonts",
                                                         "pixelnours_16.png"));
    bubble.pixelfont->scroll_speed = 0;
    bubble.line_len_px      = 128;
    bubble.row_height       = bubble.pixelfont->glyph_height;
    bubble.padding.top      = TEXT_BOX_PADDING_TOP + 2;
    bubble.padding.bottom   = 1;
    bubble.line_num_max     = 0;

    /* --- RENDERING BUBBLES --- */
    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);
    SDL_assert(bubble.tail.index     == TEXT_BOX_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_16_minus_HelloWorld_Blue.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.padding.bottom   = 3;
    bubble.line_len_px = 128;
    TEXT_BOX_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box",
                                      "TEXT_BOX_Pixelnours_16_minus_minus_Blue.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.padding.bottom = 0;
    bubble.line_len_px = 164;
    TEXT_BOX_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME.", bubble.target,
                     &n9patch);
    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box",
                                      "TEXT_BOX_Pixelnours_16_minus_majus_Blue.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- Setting BLUE bg for text bubble --- */
    TEXT_BOX_Colors_Set(&bubble, SOTA_WHITE, SOTA_BLACK);
    TEXT_BOX_Colors_Swap(&bubble, renderer, &n9patch);
    TEXT_BOX_Colors_Set(&bubble, SOTA_BLACK, SOTA_WHITE);
    TEXT_BOX_Colors_Swap(&bubble, renderer, &n9patch);

    bubble.pixelfont->scroll_speed  = 0;
    bubble.line_len_px              = 128;
    bubble.row_height               = bubble.pixelfont->glyph_height;
    bubble.padding.top              = TEXT_BOX_PADDING_TOP + 2;
    bubble.padding.bottom           = 1;
    bubble.line_num_max             = 0;

    /* --- RENDERING BUBBLES --- */
    /* - setting - */
    bubble.target.x = -100;
    bubble.target.y = -100;
    TEXT_BOX_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);
    SDL_assert(bubble.tail.index     == TEXT_BOX_DIAGONAL);
    SDL_assert(bubble.tail.angle     == 180.0);
    SDL_assert(bubble.tail.octant    == SOTA_DIRECTION_TOPLEFT);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box", "Text_Box_16_minus_HelloWorld_Black.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.padding.bottom   = 3;
    bubble.line_len_px = 128;
    TEXT_BOX_Set_All(&bubble, "portez ce vieux whisky au juge blond qui fume.", bubble.target,
                     &n9patch);

    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box",
                                      "TEXT_BOX_Pixelnours_16_minus_minus_Black.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    bubble.padding.bottom = 0;
    bubble.line_len_px = 164;
    TEXT_BOX_Set_All(&bubble, "PORTEZ CE VIEUX WHISKY AU JUGE BLOND QUI FUME.", bubble.target,
                     &n9patch);
    /* - rendering - */
    Text_Box_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_Text_Box",
                                      "TEXT_BOX_Pixelnours_16_minus_majus_Black.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    /* SDL_free */
    PixelFont_Free(bubble.pixelfont, true);
    Text_Box_Free(&bubble);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}

void test_Text_Box() {
    sota_mkdir("popup_Text_Box");
    test_TEXT_BOX_VScroll_Anim();
    test_TEXT_BOX_Tail();
    test_TEXT_BOX_Scroll();
    test_TEXT_BOX_Scroll_vertical();
    test_TEXT_BOX_Pixelfont16();
    test_TEXT_BOX_Pixelfont16_tight();
    test_TEXT_BOX_Pixelfont16_minus();
}
