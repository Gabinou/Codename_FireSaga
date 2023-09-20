#include "nourstest.h"
#include "text_bubble.h"


void test_text_bubble() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);

    sota_mkdir("popup_text_bubble");

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    struct Text_Bubble bubble = TextBubble_default;

    /* -- Create n9patch -- */
    // render_target is NULL cause there is render_target!
    struct n9Patch n9patch;
    SDL_Texture *render_target = NULL;

    /* - loading fonts - */
    bubble.pixelnours = PixelFont_Alloc();
    TextBubble_Load(&bubble, renderer, &n9patch);
    PixelFont_Load(bubble.pixelnours, renderer, PATH_JOIN("..", "assets", "Fonts", "pixelnours.png"));
    bubble.line_len_px = 64;

    TextBubble_Set_Text(&bubble, "Hello, World!");
    SDL_assert(bubble.width  > 0);
    SDL_assert(bubble.height > 0);

    TextBubble_Update(&bubble, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_text_bubble", "TextBubble_HelloWorld.png"),
                            renderer, bubble.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    PixelFont_Free(bubble.pixelnours, true);
    TextBubble_Free(&bubble);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}