#include "nourstest.h"
#include "text_bubble.h"

void test_text_bubble_pointer() {
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
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "Fonts", "pixelnours.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, 1, 55);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = -bubble.height;
    TextBubble_Set_All(&bubble, "Hello, World!", bubble.target, &n9patch);
    SDL_assert(bubble.pointer.index     == TEXT_BUBBLE_DIAGONAL);
    SDL_assert(bubble.pointer.angle     == 180.0);
    SDL_assert(bubble.pointer.octant    == SOTA_DIRECTION_DIAGONAL_TL);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_HelloWorld.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_All(&bubble, "Another oneliner.", bubble.target, &n9patch);
    SDL_assert(bubble.pointer.octant    == SOTA_DIRECTION_LEFT);
    SDL_assert(bubble.pointer.index     == TEXT_BUBBLE_STRAIGHT);
    SDL_assert(bubble.pointer.angle     == 90.0);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_1line.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_All(&bubble, "Another oneliner.", bubble.target, &n9patch);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_1line_pointerR.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = - bubble.width;
    bubble.target.y = bubble.height * 2;
    TextBubble_Set_All(&bubble, "A", bubble.target, &n9patch);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_A.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    bubble.target.x = bubble.width / 2;
    bubble.target.y = - bubble.height;
    TextBubble_Set_All(&bubble, "I am a 2 line long speech. Brief.", bubble.target, &n9patch);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_2lines.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "I am a 2 line long speech. Brief.", &n9patch);
    bubble.target.x = -bubble.width;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_2lines_pointerR.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "I am a 2 line long speech. Brief.", &n9patch);
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_2lines_pointerL.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "The quick brown fox jumped over the lazy dog", &n9patch);
    bubble.target.x = bubble.width / 2;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_BrownFox.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Pixelnours_big - */
    bubble.pixelfont->y_offset     = pixelfont_big_y_offset;
    TextBubble_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "Fonts",
                                                         "pixelnours_Big.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, 1, 55);
    bubble.line_len_px  = 96;
    bubble.row_height   = ASCII_GLYPH_HEIGHT + 2;
    bubble.padding.top  = TEXT_BUBBLE_PADDING_TOP + 2;

    /* - setting - */
    TextBubble_Set_Text(&bubble, "Hello, World!", &n9patch);
    bubble.target.x = bubble.width / 2;
    bubble.target.y = bubble.height * 2;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_Big_HelloWorld.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "Another oneliner.", &n9patch);
    bubble.target.x = bubble.width * 2;
    bubble.target.y = -bubble.height;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_Big_1line.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "A", &n9patch);
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_Big_A.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "I am a 2 line long speech. Brief.", &n9patch);
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height * 2;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_Big_2lines.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "The quick brown fox jumped over the lazy dog", &n9patch);
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_Big_BrownFox.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);


    /* - setting - */
    TextBubble_Set_Text(&bubble, "The quick brown fox jumped over the lazy dog", &n9patch);
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_3lines_pointerR3.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "The quick brown fox jumped over the lazy dog", &n9patch);
    bubble.target.x = -bubble.width * 2;
    bubble.target.y = bubble.height / 2;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_3lines_pointerL3.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "The quick brown fox jumped over the lazy dog", &n9patch);
    bubble.target.x = bubble.width * 2;
    bubble.target.y = bubble.height / 3;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_3lines_pointerR2.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "The quick brown fox jumped over the lazy dog", &n9patch);
    bubble.target.x = -bubble.width * 2;
    bubble.target.y = bubble.height / 3;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_3lines_pointerL2.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "The quick brown fox jumped over the lazy dog", &n9patch);
    bubble.target.x = bubble.width * 2;
    bubble.target.y = 2 * bubble.height / 3;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_3lines_pointerR1.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "The quick brown fox jumped over the lazy dog", &n9patch);
    bubble.target.x = -bubble.width * 2;
    bubble.target.y = 2 * bubble.height / 3;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_3lines_pointerL1.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "Hello, World!", &n9patch);
    bubble.target.x = bubble.width / 3;
    bubble.target.y = bubble.height * 2;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_pointerT1.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "Hello, World!", &n9patch);
    bubble.target.x = bubble.width / 3;
    bubble.target.y = -bubble.height * 2;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_pointerB1.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);


    /* - setting - */
    TextBubble_Set_Text(&bubble, "Hello, World!", &n9patch);
    bubble.target.x = bubble.width / 2;
    bubble.target.y = bubble.height * 2;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_pointerT2.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "Hello, World!", &n9patch);
    bubble.target.x = bubble.width / 2;
    bubble.target.y = -bubble.height * 2;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_pointerB2.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);


    /* - setting - */
    TextBubble_Set_Text(&bubble, "Hello, World!", &n9patch);
    bubble.target.x = bubble.width * 2 / 3;
    bubble.target.y = bubble.height * 2;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_pointerT3.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - setting - */
    TextBubble_Set_Text(&bubble, "Hello, World!", &n9patch);
    bubble.target.x = bubble.width * 2 / 3;
    bubble.target.y = -bubble.height * 2;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    TextBubble_Pointer_Pos(&bubble, &n9patch);

    /* - rendering - */
    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_pointerB3.png"),
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
    PixelFont_Load(bubble.pixelfont, renderer, PATH_JOIN("..", "assets", "Fonts",
                                                         "pixelnours_Big.png"));
    PixelFont_Swap_Palette(bubble.pixelfont, renderer, 1, 55);
    bubble.pixelfont->scroll_speed = 0;
    bubble.line_len_px  = 96;
    bubble.row_height   = ASCII_GLYPH_HEIGHT + 2;
    bubble.padding.top  = TEXT_BUBBLE_PADDING_TOP + 2;

    /* - setting - */
    TextBubble_Set_Text(&bubble, "The quick brown fox jumped over the lazy dog.", &n9patch);
    bubble.target.x = -bubble.width;
    bubble.target.y = -bubble.height;
    TextBubble_Set_Target(&bubble, bubble.target);
    SDL_assert(bubble.pointer.index     == TEXT_BUBBLE_DIAGONAL);
    SDL_assert(bubble.pointer.angle     == 180.0);
    SDL_assert(bubble.pointer.octant    == SOTA_DIRECTION_DIAGONAL_TL);
    TextBubble_Pointer_Pos(&bubble, &n9patch);
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);
    bubble.scroll = true;
    int i = 0;
    char path_raw[128] = PATH_JOIN("popup_text_bubble", "TextBubble_Scroll_%02d.png");
    char path[128];
    size_t scroll_lim = strlen(bubble.text) - bubble.lines.line_num + 1;
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


void test_text_bubble() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    sota_mkdir("popup_text_bubble");
    test_text_bubble_pointer();
    test_text_bubble_scroll();
}
