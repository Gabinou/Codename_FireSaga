
#include "menu/menu.h"
#include "position.h"
#include "octant.h"
#include "palette.h"
#include "utilities.h"

const struct MenuElemDirections MenuElemDirections_default = {
    .right  = MENU_ELEM_NULL,
    .top    = MENU_ELEM_NULL,
    .left   = MENU_ELEM_NULL,
    .bottom = MENU_ELEM_NULL,
};

const struct Menu Menu_default =  {
    .elem_pos_frame   = ELEM_POS_MENU_FRAME,
    .visible          = true,
};

/* --- Menu --- */
void Menu_Elem_Free(struct Menu *mc) {
    if (mc->elem_pos != NULL) {
        SDL_free(mc->elem_pos);
        mc->elem_pos = NULL;
    }
    if (mc->cursor_pos != NULL) {
        SDL_free(mc->elem_pos);
        mc->elem_pos = NULL;
    }
    if (mc->elem_box != NULL) {
        SDL_free(mc->elem_box);
        mc->elem_box = NULL;
    }
    if (mc->elem_description != NULL) {
        SDL_free(mc->elem_description);
        mc->elem_description = NULL;
    }
    if (mc->elem_links != NULL) {
        SDL_free(mc->elem_links);
        mc->elem_links = NULL;
    }
}

i32 Periodic_Elem_Move(struct Menu *menu, i32 direction, i32 min, i32 max) {
    // Cursor can only move in 2 directions between elems
    // Cursor movement is periodid
    direction = Ternary_Direction_Straight(direction);
    SDL_assert(sizeof(*menu->elem_links) == MENU_ELEM_DIRECTIONS_PACKED_BYTESIZE);

    /* Find next elem using direction and array of links */
    i8 out = -1;
    if        (direction == SOTA_DIRECTION_BOTTOM) {
        out = menu->elem == (max - 1) ? min : menu->elem + 1;
    } else if (direction == SOTA_DIRECTION_TOP) {
        out = menu->elem == min ? max - 1 : menu->elem - 1;
    } else {
        out = menu->elem;
    }

    return (out);
}

void Menu_Elem_Set(struct Menu *mc, struct Game *sota, i8 new_elem) {
    SDL_assert(new_elem >= 0);
    SDL_assert(new_elem != MENU_ELEM_NULL);

    tnecs_entity cursor = sota->cursor.entity;
    struct Position *cursor_pos = IES_GET_COMPONENT(sota->ecs.world, cursor, Position);
    cursor_pos->pixel_pos.x = mc->elem_pos[new_elem].x;
    cursor_pos->pixel_pos.y = mc->elem_pos[new_elem].y;
    Cursor_Box_Offset(&cursor_pos->pixel_pos);
    mc->elem = new_elem;

}

i32 Menu_Elem_Move(struct Menu *menu, i32 direction) {
    /* Cursor can only move in 4 directions between elems */
    direction = Ternary_Direction_Straight(direction);
    SDL_assert(sizeof(*menu->elem_links) == MENU_ELEM_DIRECTIONS_PACKED_BYTESIZE);

    /* Cast packed struct of i8 to array */
    SDL_assert(menu->elem_links != NULL);
    i8 *links = (i8 *) &menu->elem_links[menu->elem];

    /* Find next elem using direction and array of links */
    i8 out = menu->elem;
    i32 direction_i = direction_arr_i[direction];
    if (links[direction_i] < menu->elem_num)
        out = links[direction_i] > MENU_ELEM_NULL ? links[direction_i] : menu->elem;
    return (out);
}

void Menu_Elem_Boxes_Check(struct Menu *mc) {
    /* Fit elem_box to linked elems snuggly */
    for (u32 i = 0; i < mc->elem_num; i++) {
        /* Get elem pos and initial box */
        struct Point *elem_box = &(mc->elem_box[i]);
        SDL_assert(elem_box->x == mc->elem_box[i].x);
        SDL_assert(elem_box->y == mc->elem_box[i].y);
        struct Point *elem_pos = &(mc->elem_pos[i]);
        SDL_assert(elem_pos->x == mc->elem_pos[i].x);
        SDL_assert(elem_pos->y == mc->elem_pos[i].y);
        /* - cast link struct to array - */
        i8 *links = (i8 *) & (mc->elem_links[i]);

        /* -- Fitting box to 4 linked elems if they exist -- */
        /* - right link - */
        i32 direction_i = direction_arr_i[SOTA_DIRECTION_RIGHT];
        i8 link = links[direction_i];
        SDL_assert(link < mc->elem_num);
        if (link != MENU_ELEM_NULL) {
            struct Point *link_pos = &(mc->elem_pos[link]);
            if (elem_pos->x < link_pos->x) {
                /* - shorten elem x - */
                elem_box->x = link_pos->x - elem_pos->x - 1;
            }
        }

        /* - top link - */
        direction_i = direction_arr_i[SOTA_DIRECTION_TOP];
        link = links[direction_i];
        SDL_assert(link < mc->elem_num);
        if (link != MENU_ELEM_NULL) {
            struct Point *link_box = &(mc->elem_box[link]);
            struct Point *link_pos = &(mc->elem_pos[link]);
            if (link_pos->y < elem_pos->y) {
                /* - shorten link y - */
                link_box->y = elem_pos->y - link_pos->y - 1;
            }
        }

        /* - left link - */
        direction_i = direction_arr_i[SOTA_DIRECTION_LEFT];
        link = links[SOTA_DIRECTION_LEFT];
        SDL_assert(link < mc->elem_num);
        if (link != MENU_ELEM_NULL) {
            struct Point *link_box = &(mc->elem_box[link]);
            struct Point *link_pos = &(mc->elem_pos[link]);
            if (link_pos->x < elem_pos->x) {
                /* - shorten link x - */
                link_box->x = elem_pos->x - link_pos->x - 1;
            }
        }

        /* - bottom link - */
        direction_i = direction_arr_i[SOTA_DIRECTION_BOTTOM];
        link = links[direction_i];
        SDL_assert(link < mc->elem_num);
        if (link != MENU_ELEM_NULL) {
            struct Point *link_pos = &(mc->elem_pos[link]);
            if (elem_pos->y < link_pos->y) {
                /* - shorten elem y - */
                elem_box->y = link_pos->y - elem_pos->y - 1;
            }
        }
    }
}

/* --- Debug --- */
void Menu_Elem_Boxes_Draw(struct Menu *mc, struct SDL_Renderer *renderer) {
    /* -- Set colors of elem_box to NES palette -- */
    for (i32 i = 0; i < mc->elem_num; i++) {
        i32 color = i + 2 % palette_NES->ncolors;
        u8 r = palette_NES->colors[color].r;
        u8 g = palette_NES->colors[color].g;
        u8 b = palette_NES->colors[color].b;
        SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
        struct Point pos = mc->elem_pos[i];
        struct Point box = mc->elem_box[i];
        SDL_Rect rect = {pos.x, pos.y, box.x, box.y};
        SDL_RenderDrawRect(renderer, &rect);
    }
    Utilities_DrawColor_Reset(renderer);
}
