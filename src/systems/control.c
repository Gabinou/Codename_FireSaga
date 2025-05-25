
#include "systems/control.h"
#include "game/game.h"
#include "map/map.h"
#include "position.h"
#include "sprite.h"

void Cursor_Scroll_Camera(tnecs_input *input) {
    Game *IES = input->data;
    SDL_assert(IES != NULL);

    Position *pos_arr    = TNECS_COMPONENT_ARRAY(input, Position_ID);
    Sprite   *sprite_arr = TNECS_COMPONENT_ARRAY(input, Sprite_ID);

    // There should only be one cursor
    SDL_assert(input->num_entities == 1);
    Position    *pos    = pos_arr;
    Sprite      *sprite = sprite_arr;

    if (!pos->onTilemap)
        return;

    /* -- Bounds -- */
    // Outside bounds, cursor scrolls the camera
    // More zoom -> cursor scrolls camera from farther away
    // factors are screen length ratio
    float factor_max = (CAMERA_BOUNDS - CAMERA_BOUNDS_SCALE * IES->map->render.camera.zoom /
                        MAX_CAMERA_ZOOM);
    float factor_min = 1.0f - factor_max;
    SDL_assert(factor_max < 1.0f);
    SDL_assert(factor_max > 0.0f);
    SDL_assert(factor_min < 1.0f);
    SDL_assert(factor_min > 0.0f);
    SDL_assert(factor_min < factor_max);

    Point bounds_max = {
        (factor_max * IES->settings.res.x),
        (factor_max * IES->settings.res.y)
    };
    Point bounds_min = {
        (factor_min * IES->settings.res.x),
        (factor_min * IES->settings.res.y)
    };

    /* -- Cursor center position -- */
    int x = sprite->dstrect.x, y = sprite->dstrect.y;
    int w = sprite->dstrect.w, h = sprite->dstrect.h;
    Point center = {(x + w / 2), (y + h / 2)};

    /* -- Scroll amount -- */
    // TODO: scrolling is TOO SLOW
    //      - Make scroll speed same as cursor speed.
    int scroll = CAMERA_SCROLL_SPEED * SOTA_DEZOOM(IES->map->render.camera.zoom,
                                                   CAMERA_SCROLL_ZOOMFACTOR);

    /* -- Does camera need to scroll? -- */
    if (center.x >= bounds_max.x) {
        IES->map->render.camera.offset.x   -= scroll;
        IES->map->flags.camera_moved = true;
    } else if (center.x <= bounds_min.x) {
        IES->map->render.camera.offset.x   += scroll;
        IES->map->flags.camera_moved = true;
    }
    if (center.y >= bounds_max.y) {
        IES->map->render.camera.offset.y   -= scroll;
        IES->map->flags.camera_moved = true;
    } else if (center.y <= bounds_min.y) {
        IES->map->render.camera.offset.y   += scroll;
        IES->map->flags.camera_moved = true;
    }
}