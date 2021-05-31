#ifndef TEXT_H
#define TEXT_H

#include "filesystem.h"
#include "SDL2/SDL.h"
#include "game.h"
#include "flecs.h"

typedef struct Text {
    SDL_Texture * texture;
    char text_line[DEFAULT_BUFFER_SIZE];
    SDL_Color text_color;
    SDL_Rect srcrect;
    SDL_Rect destrect;

    int16_t padding[4]; //html style: up right down left
    int16_t fontsize;

    bool visible;
    float spacingfactor;
    float sizefactor[2]; // height, width
} Text;
extern struct Text Text_default;

typedef struct TextModule {
    ECS_DECLARE_COMPONENT(Text);
} TextModule;

void TextModuleImport(ecs_world_t * in_world);
#define TextModuleImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, Text);

extern void Text_Texture_Make(struct Text * in_text);
extern void Text_Rects_Pos_Set(struct Text * in_text, int16_t x, int16_t y);
extern void Text_draw(struct Text * in_text);
extern void onUpdate_Text(ecs_world_t * in_world, ecs_entity_t in_entity_fps, uint32_t frame_count, float last_update, void * data);

#endif /* TEXT_H */
