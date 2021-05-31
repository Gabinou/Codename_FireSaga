#include "text.h"
#include "stb_ds.h"

struct Text Text_default = {
    .texture = NULL,
    .text_line = {'\0'},

    // .text_color = NULL,
    // .srcrects = NULL,
    // .destrects = NULL,

    .padding = {10, 10, 10, 10}, //html style: up right down left
    .fontsize = 14,

    .visible = true,
    .spacingfactor = 1.1f,
    // .sizefactor = {1.0f, 1.0f}, // height, width
};

void onUpdate_Text(ecs_world_t * in_world, ecs_entity_t in_entity_fps, uint32_t frame_count, float last_update_s, void * data) {
    ECS_IMPORT(in_world, TextModule);
    Text * fps_text_mptr = ecs_get_mut(in_world, in_entity_fps, Text, NULL);
    snprintf(fps_text_mptr->text_line, sizeof(char) * DEFAULT_BUFFER_SIZE, "%.1f", (frame_count / last_update_s));
}

void TextModuleImport(ecs_world_t * in_world) {
    ECS_MODULE(in_world, TextModule);
    ECS_COMPONENT(in_world, Text);
    ECS_SET_COMPONENT(Text);
    ECS_EXPORT_COMPONENT(Text);
}

void Text_Fontsize_Set(struct Text * in_text, int16_t in_fontsize) {
    in_text->fontsize = in_fontsize;
}

void Text_Texture_Make(struct Text * in_text) {
    // SDL_Log("Text_Texture_Make");
    // SDL_Surface * textsurface = textToSurface(in_line, in_text_color, Game_font);
    struct SDL_Surface * textsurface = TTF_RenderText_Blended(Game_font, in_text->text_line, in_text->text_color); // Blended is better than Solid
    int32_t temp_height;
    int32_t temp_width;
    TTF_SizeUTF8(Game_font, in_text->text_line, &temp_width, &temp_height);

    in_text->srcrect.w = temp_width;
    in_text->srcrect.h = temp_height;
    in_text->srcrect.x = 0;
    in_text->srcrect.y = 0;
    in_text->destrect.w = temp_width;
    in_text->destrect.h = temp_height;
    in_text->destrect.x = 0;
    in_text->destrect.y = 0;

    in_text->texture = SDL_CreateTextureFromSurface(Game_renderer, textsurface);
    SDL_FreeSurface(textsurface);
}

void Text_Wrap() {
    // Should wrap text inside the background texture and pâdding.
    // -> increase width of destrect
    // -> by 1 char, by 2 char, by half char...
}

void Text_Rects_Pos_Set(struct Text * in_text, int16_t x, int16_t y) {
    // SDL_Log("Text_Rects_Pos_Set");
    in_text->destrect.x = x + in_text->padding[3];
    in_text->destrect.y = y + in_text->padding[0];

    int32_t temp_height;
    int32_t temp_width;

    int32_t hgt, wdt;
    SDL_QueryTexture(in_text->texture, NULL, NULL, &wdt, &hgt);
    SDL_assert(!((hgt != in_text->srcrect.h) | (wdt != in_text->srcrect.w)));
    TTF_SizeUTF8(Game_font, in_text->text_line, &temp_width, &temp_height);
    in_text->destrect.h = (int32_t)temp_height * in_text->sizefactor[0];
    in_text->destrect.w = (int32_t)temp_width * in_text->sizefactor[1];
    in_text->destrect.x = in_text->destrect.x;
    in_text->destrect.y = in_text->destrect.y;

}

void Text_draw(struct Text * in_text) {
    // SDL_Log("Drawing text");
    // Find a way to draw text letter by letter, word by word, etc. for future script.
    SDL_RenderCopy(Game_renderer, in_text->texture, &in_text->srcrect, &in_text->destrect);
}
