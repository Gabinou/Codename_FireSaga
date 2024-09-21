
#include "unit/boss.h"

int army_icons[ARMY_MALLOC] = {
    /* NULL                         */ BOSS_ICON_STGEORGE,
    /* FRIENDLY                     */ BOSS_ICON_STGEORGE,
    /* ENEMY                        */ BOSS_ICON_STGEORGE,
    /* ERWIN                        */ BOSS_ICON_STGEORGE,
    /* FREE_MILITIA                 */ BOSS_ICON_STGEORGE,
    /* NEUTRAL                      */ BOSS_ICON_STGEORGE,
    /* IMPERIAL                     */ BOSS_ICON_VOLDAN,
    /* HAMILCAR                     */ BOSS_ICON_HAMILCAR,
    /* FIRST_EXPEDITIONARY_FORCE    */ BOSS_ICON_HAMILCAR,
    /* THEOCRACY                    */ BOSS_ICON_STGEORGE,
    /* VOLDAN                       */ BOSS_ICON_VOLDAN,
    /* KEWAC                        */ BOSS_ICON_KEWAC,
    /* KESIRU                       */ BOSS_ICON_KESIRU,
    /* BANDITS                      */ BOSS_ICON_STGEORGE,
};

struct Boss Boss_default = {
    .icon       = BOSS_ICON_STGEORGE,
    .srcrect    = {
        .w = BOSS_ICON_WIDTH,
        .h = BOSS_ICON_HEIGHT
    },
};

void Boss_Free(struct Boss *boss) {
    if (boss->texture != NULL) {
        SDL_DestroyTexture(boss->texture);
    }
}

void Boss_Icon_Load(struct Boss *boss, SDL_Renderer *renderer) {

    if (boss->texture != NULL)
        SDL_DestroyTexture(boss->texture);

    /* --- Create icon texture by blitting ---  */
    /* To give boss->texture SDL_TEXTUREACCESS_STATIC */

    /* Read png with all boss icons */
    char *path = PATH_JOIN("..", "assets", "GUI", "Icon", "Icon_Boss.png");
    SDL_Surface *icons = Filesystem_Surface_Load(path, SDL_PIXELFORMAT_INDEX8);

    /* Copy icon with id to texture */
    SDL_Surface *icon = Filesystem_indexedSurface_Init(BOSS_ICON_WIDTH, BOSS_ICON_HEIGHT);
    SDL_Rect srcrect = boss->srcrect;
    srcrect.x = boss->icon % SOTA_COL_LEN * srcrect.w;
    srcrect.y = boss->icon / SOTA_COL_LEN * srcrect.h;
    int success = SDL_BlitSurface(icons, &srcrect, icon, NULL);
    boss->texture = SDL_CreateTextureFromSurface(renderer, icon);
    SDL_FreeSurface(icons);
    SDL_FreeSurface(icon);
}

void Boss_Icon_Pos(struct Boss *boss, struct Camera *camera,
                   struct Position *pos, struct Map *map) {
    /* Position of boss icon drawn on map,
     * computed from position component */

    int offset_x = BOSS_ICON_OFFSET_X;
    int offset_y = BOSS_ICON_OFFSET_Y;

    struct Point pixel_pos = pos->pixel_pos;
    float zoom = camera->zoom;
    boss->dstrect.x = SOTA_ZOOM((pixel_pos.x + offset_x), zoom) + camera->offset.x;
    boss->dstrect.y = SOTA_ZOOM((pixel_pos.y + offset_y), zoom) + camera->offset.y;
    boss->dstrect.w = SOTA_ZOOM((boss->srcrect.w), zoom);
    boss->dstrect.h = SOTA_ZOOM((boss->srcrect.h), zoom);
}

void Boss_Icon_Draw(struct Boss  *boss, struct Position *pos,
                    SDL_Renderer *renderer) {
    SDL_assert(pos->onTilemap);

    SDL_RenderCopy(renderer, boss->texture, &boss->srcrect, &boss->dstrect);
}
