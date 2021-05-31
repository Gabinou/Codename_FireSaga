
#include "systemRender.h"

void drawSprite(ecs_iter_t * it) {
    ECS_IMPORT(it->world, controllerMouseModule);
    ECS_IMPORT(it->world, controllerGamepadModule);
    struct Position * position_ptr = ecs_column(it, struct Position, 1);
    struct Sprite * sprite_ptr = ecs_column(it, struct Sprite, 2);
    bool ent_hasmouse, ent_hasgamepad;
    for (uint16_t ent = 0; ent < it->count; ent++) {
        // SDL_Log("Sprite entity: %d", ent);
        ent_hasmouse = (ecs_get(it->world, it->entities[ent], controllerMouse) != NULL);
        ent_hasgamepad = (ecs_get(it->world, it->entities[ent], controllerGamepad) != NULL);
        if ((!ent_hasmouse) && (!ent_hasgamepad)) {
            if (sprite_ptr[ent].visible) {

                sprite_ptr[ent].objectivepos.x = (int32_t)lround(position_ptr[ent].tilemap_pos.x * position_ptr[ent].scale[0]);
                sprite_ptr[ent].objectivepos.y = (int32_t)lround(position_ptr[ent].tilemap_pos.y * position_ptr[ent].scale[1]);

                if (sprite_ptr[ent].update_wait > 0) {

                    if (sprite_ptr[ent].animated) {
                        Sprite_loop(&sprite_ptr[ent]);
                    }

                    sprite_ptr[ent].update_timer += it->delta_time;
                    if (sprite_ptr[ent].update_timer > sprite_ptr[ent].update_wait) {
                        bool slide_int = 0;
                        Sprite_slide(&sprite_ptr[ent], slide_int, it->delta_time);
                        sprite_ptr[ent].update_timer = 0.0f;
                    }
                }

                sprite_ptr[ent].destrect.x = sprite_ptr[ent].objectivepos.x + position_ptr[ent].offset_px.x;
                sprite_ptr[ent].destrect.y = sprite_ptr[ent].objectivepos.y + position_ptr[ent].offset_px.y;

                SDL_RenderCopy(Game_renderer, sprite_ptr[ent].texture, &sprite_ptr[ent].srcrect, &sprite_ptr[ent].destrect);

            }
        }
    }
}

void drawCursor(ecs_iter_t * it) {
    struct Position * position_ptr = ecs_column(it, struct Position, 1);
    struct Sprite * sprite_ptr = ecs_column(it, struct Sprite, 2);
    struct controllerGamepad * gamepad_ptr = ecs_column(it, struct controllerGamepad, 3);
    struct controllerKeyboard * keyboard_ptr = ecs_column(it, struct controllerKeyboard, 4);
    for (uint16_t ent = 0; ent < it->count; ent++) {
        if (sprite_ptr[ent].visible) {
            sprite_ptr[ent].objectivepos.x = position_ptr[ent].tilemap_pos.x * position_ptr[ent].scale[0] - sprite_ptr[ent].destrect.w / 4;
            sprite_ptr[ent].objectivepos.y = position_ptr[ent].tilemap_pos.y * position_ptr[ent].scale[1] - sprite_ptr[ent].destrect.h / 4;
            if (sprite_ptr[ent].update_wait > 0) {

                if (sprite_ptr[ent].animated) {
                    Sprite_loop(&sprite_ptr[ent]);
                }

                sprite_ptr[ent].update_timer += it->delta_time;
                if (sprite_ptr[ent].update_timer > sprite_ptr[ent].update_wait) {
                    bool slide_int = ((keyboard_ptr[ent].timeheld_move > CURSOR_FASTTIME) || (gamepad_ptr[ent].timeheld_move > CURSOR_FASTTIME));
                    Sprite_slide(&sprite_ptr[ent], slide_int, it->delta_time);
                    sprite_ptr[ent].update_timer = 0.0f;
                }
            }

            sprite_ptr[ent].destrect.x = sprite_ptr[ent].objectivepos.x + position_ptr[ent].offset_px.x;
            sprite_ptr[ent].destrect.y = sprite_ptr[ent].objectivepos.y + position_ptr[ent].offset_px.y;

            SDL_RenderCopy(Game_renderer, sprite_ptr[ent].texture, &sprite_ptr[ent].srcrect, &sprite_ptr[ent].destrect);
        }
    }
}

void drawMenu(ecs_iter_t * it) {
    struct Menu * menu_ptr = ecs_column(it, struct Menu, 1);
    for (uint16_t ent = 0; ent < it->count; ent++) {
        if (menu_ptr[ent].enabled) {
            Menu_Patches_Draw(&menu_ptr[ent]);
            Menu_Options_Draw(it->world, &menu_ptr[ent]);
        }
    }
}

void drawMouse(ecs_iter_t * it) {
    struct Position * position_ptr = ecs_column(it, struct Position, 1);
    struct Sprite * sprite_ptr = ecs_column(it, struct Sprite, 2);
    struct controllerMouse * mouse_ptr = ecs_column(it, struct controllerMouse, 3);
    for (uint16_t ent = 0; ent < it->count; ent++) {

        if (sprite_ptr[ent].visible) {
            SDL_Rect destrect = sprite_ptr[ent].destrect;
            struct Point pos = position_ptr[ent].pixel_pos;
            destrect.x = pos.x;
            destrect.y = pos.y;
            sprite_ptr[ent].destrect = destrect;
            SDL_RenderCopy(Game_renderer, sprite_ptr[ent].texture, &sprite_ptr[ent].srcrect, &sprite_ptr[ent].destrect);

        }
    }
}

void drawText(ecs_iter_t * it) {
    struct Position * position_ptr = ecs_column(it, struct Position, 1);
    struct Text * text_ptr = ecs_column(it, struct Text, 2);
    struct UpdateTimer * updatetimer_ptr = ecs_column(it, struct UpdateTimer, 3);
    for (uint16_t ent = 0; ent < it->count; ent++) {
        if ((text_ptr[ent].visible) && (!position_ptr[ent].onTilemap)) {
            struct Point pos = position_ptr[ent].pixel_pos;
            if (updatetimer_ptr[ent].onUpdate != NULL) {
                updatetimer_ptr[ent].frame_count++;
                updatetimer_ptr[ent].last_update += it->delta_time;
                if (updatetimer_ptr[ent].last_update >= updatetimer_ptr[ent].update_time) {
                    (*updatetimer_ptr[ent].onUpdate)(it->world, it->entities[ent], updatetimer_ptr[ent].frame_count, updatetimer_ptr[ent].last_update, NULL);
                    updatetimer_ptr[ent].frame_count = 0;
                    updatetimer_ptr[ent].last_update = 0.0f;
                }
            }
            Text_Texture_Make(&text_ptr[ent]);
            Text_Rects_Pos_Set(&text_ptr[ent], pos.x, pos.y);
            Text_draw(&text_ptr[ent]);
        }
    }
}

void systemRenderModuleImport(ecs_world_t * in_world) {
    ECS_MODULE(in_world, systemRenderModule);
    ECS_IMPORT(in_world, PositionModule);
    ECS_IMPORT(in_world, TextModule);
    ECS_IMPORT(in_world, MenuModule);
    ECS_IMPORT(in_world, SpriteModule);
    ECS_IMPORT(in_world, controllerMouseModule);
    ECS_IMPORT(in_world, controllerGamepadModule);
    ECS_IMPORT(in_world, controllerKeyboardModule);
    ECS_IMPORT(in_world, controllerTouchpadModule);
    ECS_IMPORT(in_world, TraitsModule);
    ECS_SYSTEM(in_world, drawText, EcsOnUpdate, position.module.Position, text.module.Text, TRAIT | traits.module.UpdateTimer);
    ECS_SYSTEM(in_world, drawSprite, EcsOnUpdate, position.module.Position, sprite.module.Sprite);
    ECS_SYSTEM(in_world, drawMenu, EcsOnUpdate, menu.module.Menu);
    ECS_SYSTEM(in_world, drawMouse, EcsOnUpdate, position.module.Position, sprite.module.Sprite, controller.mouse.module.controllerMouse);
    ECS_SYSTEM(in_world, drawCursor, EcsOnUpdate, position.module.Position, sprite.module.Sprite, controller.gamepad.module.controllerGamepad, controller.keyboard.module.controllerKeyboard);
    ECS_SET_ENTITY(drawText);
    ECS_SET_ENTITY(drawSprite);
    ECS_SET_ENTITY(drawMenu);
    ECS_SET_ENTITY(drawCursor);
    ECS_SET_ENTITY(drawMouse);
}