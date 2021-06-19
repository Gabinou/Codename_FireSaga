
#include "systemRender.h"

void drawSprite(tnecs_system_input_t * in_input) {
    SDL_Log("drawSprite");
    struct Position * position_ptr = TNECS_COMPONENTS_LIST(in_input, Position);
    SDL_Log("got position %d", (position_ptr == NULL));
    struct Sprite * sprite_ptr = TNECS_COMPONENTS_LIST(in_input, Sprite);
    SDL_Log("got sprite %d", (sprite_ptr == NULL));
    bool ent_hasmouse, ent_hasgamepad;
    SDL_Log("in_input->num_entities %d", in_input->num_entities);
    for (uint16_t ent = 0; ent < in_input->num_entities; ent++) {
        SDL_Log("Sprite entity: %d", ent);
        SDL_Log("in_input->entity_typeflag_id: %d", in_input->entity_typeflag_id);

        SDL_Log("in_input->world->num_entities_bytype[in_input->entity_typeflag_id]: %d", in_input->world->num_entities_bytype[in_input->entity_typeflag_id]);
        SDL_Log("in_input->world->entities_bytype[in_input->entity_typeflag_id][ent] %d", in_input->world->entities_bytype[in_input->entity_typeflag_id][ent]);
        SDL_Log("in_input->world->entities[in_input->world->entities_bytype[in_input->entity_typeflag_id][ent]] %d", in_input->world->entities[in_input->world->entities_bytype[in_input->entity_typeflag_id][ent]]);

        ent_hasmouse = TNECS_ENTITY_HASCOMPONENT(in_input->world, in_input->world->entities_bytype[in_input->entity_typeflag_id][ent], controllerMouse);
        SDL_Log("ent_hasmouse %d", ent_hasmouse);
        ent_hasgamepad = TNECS_ENTITY_HASCOMPONENT(in_input->world, in_input->world->entities_bytype[in_input->entity_typeflag_id][ent], controllerGamepad);
        SDL_Log("ent_hasgamepad %d", ent_hasgamepad);
        if ((!ent_hasmouse) && (!ent_hasgamepad)) {
            SDL_Log("in ");
            if (sprite_ptr[ent].visible) {
                SDL_Log("visible ");

                sprite_ptr[ent].objectivepos.x = (int32_t)lround(position_ptr[ent].tilemap_pos.x * position_ptr[ent].scale[0]);
                sprite_ptr[ent].objectivepos.y = (int32_t)lround(position_ptr[ent].tilemap_pos.y * position_ptr[ent].scale[1]);
                SDL_Log("objectivepos ");

                if (sprite_ptr[ent].update_wait > 0) {
                    SDL_Log("waitenough ");

                    if (sprite_ptr[ent].animated) {
                        SDL_Log("animated ");
                        Sprite_loop(&sprite_ptr[ent]);
                    }

                    sprite_ptr[ent].update_timer += in_input ->deltat;
                    if (sprite_ptr[ent].update_timer > sprite_ptr[ent].update_wait) {
                        SDL_Log("isupdate ");
                        bool slide_int = 0;
                        Sprite_slide(&sprite_ptr[ent], slide_int, in_input->deltat);
                        sprite_ptr[ent].update_timer = 0.0f;
                    }
                }

                sprite_ptr[ent].destrect.x = sprite_ptr[ent].objectivepos.x + position_ptr[ent].offset_px.x;
                sprite_ptr[ent].destrect.y = sprite_ptr[ent].objectivepos.y + position_ptr[ent].offset_px.y;
                SDL_Log("destrect ");

                SDL_RenderCopy(Game_renderer, sprite_ptr[ent].texture, &sprite_ptr[ent].srcrect, &sprite_ptr[ent].destrect);
                SDL_Log("Rendered ");

            }
        }
    }
    SDL_Log("Out");
}

void drawCursor(tnecs_system_input_t * in_input) {
    SDL_Log("drawCursor");
    struct Position * position_ptr = TNECS_COMPONENTS_LIST(in_input, Position);
    struct Sprite * sprite_ptr = TNECS_COMPONENTS_LIST(in_input, Sprite);
    struct controllerKeyboard * keyboard_ptr = TNECS_COMPONENTS_LIST(in_input, controllerKeyboard);
    struct controllerGamepad * gamepad_ptr = TNECS_COMPONENTS_LIST(in_input, controllerGamepad);

    for (uint16_t ent = 0; ent < in_input->num_entities; ent++) {
        if (sprite_ptr[ent].visible) {

            sprite_ptr[ent].objectivepos.x = position_ptr[ent].tilemap_pos.x * position_ptr[ent].scale[0] - sprite_ptr[ent].destrect.w / 4;
            sprite_ptr[ent].objectivepos.y = position_ptr[ent].tilemap_pos.y * position_ptr[ent].scale[1] - sprite_ptr[ent].destrect.h / 4;
            if (sprite_ptr[ent].update_wait > 0) {

                if (sprite_ptr[ent].animated) {
                    Sprite_loop(&sprite_ptr[ent]);
                }

                sprite_ptr[ent].update_timer += in_input->deltat;
                if (sprite_ptr[ent].update_timer > sprite_ptr[ent].update_wait) {
                    bool slide_int = ((keyboard_ptr[ent].timeheld_move > CURSOR_FASTTIME) || (gamepad_ptr[ent].timeheld_move > CURSOR_FASTTIME));
                    Sprite_slide(&sprite_ptr[ent], slide_int, in_input->deltat);
                    sprite_ptr[ent].update_timer = 0.0f;
                }
            }

            sprite_ptr[ent].destrect.x = sprite_ptr[ent].objectivepos.x + position_ptr[ent].offset_px.x;
            sprite_ptr[ent].destrect.y = sprite_ptr[ent].objectivepos.y + position_ptr[ent].offset_px.y;

            SDL_RenderCopy(Game_renderer, sprite_ptr[ent].texture, &sprite_ptr[ent].srcrect, &sprite_ptr[ent].destrect);
        }
    }
}

void drawMouse(tnecs_system_input_t * in_input) {
    SDL_Log("drawMouse");
    struct Position * position_ptr = TNECS_COMPONENTS_LIST(in_input, Position);
    struct Sprite * sprite_ptr = TNECS_COMPONENTS_LIST(in_input, Sprite);
    struct controllerMouse * mouse_ptr = TNECS_COMPONENTS_LIST(in_input, controllerMouse);
    for (uint16_t ent = 0; ent < in_input->num_entities; ent++) {

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

void drawMenu(tnecs_system_input_t * in_input) {
    SDL_Log("drawMenu");
    struct Menu * menu_ptr = TNECS_COMPONENTS_LIST(in_input, Menu);
    for (uint16_t ent = 0; ent < in_input->num_entities; ent++) {
        if (menu_ptr[ent].enabled) {
            Menu_Patches_Draw(&menu_ptr[ent]);
            Menu_Options_Draw(in_input->world, &menu_ptr[ent]);
        }
    }
}

void drawText(tnecs_system_input_t * in_input) {
    SDL_Log("drawText");
    struct Position * position_ptr = TNECS_COMPONENTS_LIST(in_input, Position);
    struct Text * text_ptr = TNECS_COMPONENTS_LIST(in_input, Text);
    struct UpdateTimer * updatetimer_ptr = TNECS_COMPONENTS_LIST(in_input, UpdateTimer);
    for (uint16_t ent = 0; ent < in_input->num_entities; ent++) {
        if ((text_ptr[ent].visible) && (!position_ptr[ent].onTilemap)) {
            struct Point pos = position_ptr[ent].pixel_pos;
            if (updatetimer_ptr[ent].onUpdate != NULL) {
                updatetimer_ptr[ent].frame_count++;
                updatetimer_ptr[ent].last_update += in_input->deltat;
                if (updatetimer_ptr[ent].last_update >= updatetimer_ptr[ent].update_time) {
                    (*updatetimer_ptr[ent].onUpdate)(in_input->world, in_input->world->entities_bytype[in_input->entity_typeflag_id][ent], updatetimer_ptr[ent].frame_count, updatetimer_ptr[ent].last_update, NULL);
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
