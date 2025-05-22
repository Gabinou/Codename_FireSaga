
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

    for (size_t order = 0; order < input->num_entities; order++) {
        Position    *pos    = (pos_arr      + order);
        Sprite      *sprite = (sprite_arr   + order);

        if (!pos->onTilemap)
            return;

        float factor_max = (CAMERA_BOUNDS - CAMERA_BOUNDS_SCALE * IES->map->render.camera.zoom /
                            MAX_CAMERA_ZOOM);
        float factor_min = 1.0f - factor_max;

        int x = sprite->dstrect.x, y = sprite->dstrect.y;
        int w = sprite->dstrect.w, h = sprite->dstrect.h;
        int offset = CAMERA_SCROLL_SPEED * IES->map->render.camera.zoom / CAMERA_SCROLL_ZOOMFACTOR;

        if ((x + w / 2) >= (factor_max * IES->settings.res.x)) {
            IES->map->render.camera.offset.x   -= offset;
            IES->map->flags.camera_moved = true;
        } else if ((x + w / 2) <= (factor_min * IES->settings.res.x)) {
            IES->map->render.camera.offset.x   += offset;
            IES->map->flags.camera_moved = true;
        }
        if ((y + h / 2) >= (factor_max * IES->settings.res.y)) {
            IES->map->render.camera.offset.y   -= offset;
            IES->map->flags.camera_moved = true;
        } else if ((y + h / 2) <= (factor_min * IES->settings.res.y)) {
            IES->map->render.camera.offset.y   += offset;
            IES->map->flags.camera_moved = true;
        }
    }
}