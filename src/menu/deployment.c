
#include "menu/deployment.h"

/* --- STATIC FUNCTIONS DECLARATIONS --- */
/* --- Constructors/Destructors --- */
static void _DeploymentMenu_Free_Icons(struct DeploymentMenu *dm);

/* --- Loading --- */
static void _DeploymentMenu_Load_Icons(struct DeploymentMenu *dm, SDL_Renderer   *r);

/* --- Drawing --- */
static void _DeploymentMenu_Draw_Unit(      struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Mount(     struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Weapons(   struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Headers(   struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Content(   struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Scroll_Bar(struct DeploymentMenu *dm, SDL_Renderer *r);

struct DeploymentMenu DeploymentMenu_default = {
    .update         = true,
    .pos            = {-1, -1},

    .party_size     =   0,
    .top_unit       =   0,   /* Up   - Down  scrolling [0, party_size] */
    .page           =   0,   /* Left - Right scrolling [0, DM_PAGE_NUM]*/

    .party          = NULL,
    .map            = NULL,
    .font_wpns      = NULL,
    .pixelnours     = NULL,
    .pixelnours_16  = NULL,
    .pixelnours_big = NULL,

    .texture        = NULL,
    .texture_mount  = NULL,
};

struct DeploymentMenu *DeploymentMenu_Alloc(void) {
    struct DeploymentMenu *dm = SDL_malloc(sizeof(struct DeploymentMenu));
    SDL_assert(dm);
    *dm = DeploymentMenu_default;

    dm->pixelnours     = PixelFont_Alloc();
    dm->pixelnours_16  = PixelFont_Alloc();
    dm->pixelnours_big = PixelFont_Alloc();
    SDL_assert(dm->pixelnours     != NULL);
    SDL_assert(dm->pixelnours_16  != NULL);
    SDL_assert(dm->pixelnours_big != NULL);
    return (dm);
}

void DeploymentMenu_Free(struct DeploymentMenu *dm) {
    SDL_assert(dm != NULL);
    if (dm->texture != NULL) {
        SDL_DestroyTexture(dm->texture);
        dm->texture = NULL;
    }
    if (dm->pixelnours != NULL) {
        PixelFont_Free(dm->pixelnours, true);
        dm->pixelnours = NULL;
    }
    if (dm->pixelnours_16 != NULL) {
        PixelFont_Free(dm->pixelnours_16, true);
        dm->pixelnours_16 = NULL;
    }

    if (dm->pixelnours_big != NULL) {
        PixelFont_Free(dm->pixelnours_big, true);
        dm->pixelnours_big = NULL;
    }

    if (dm != NULL) {
        SDL_free(dm);
        dm = NULL;
    }
}

void DeploymentMenu_Load(struct DeploymentMenu *dm, SDL_Renderer *renderer,
                         struct n9Patch *n9patch) {
    n9Patch_Free(n9patch);
    n9patch->patch_pixels.x = MENU_PATCH_PIXELS;
    n9patch->patch_pixels.y = MENU_PATCH_PIXELS;
    n9patch->size_patches.x = DM_PATCH_X_SIZE;
    n9patch->size_patches.y = DM_PATCH_Y_SIZE;
    n9patch->scale.x        = DM_N9PATCH_SCALE_X;
    n9patch->scale.y        = DM_N9PATCH_SCALE_Y;
    n9patch->size_pixels.x  = (MENU_PATCH_PIXELS * DM_PATCH_X_SIZE);
    n9patch->size_pixels.y  = (MENU_PATCH_PIXELS * DM_PATCH_Y_SIZE);

    if (n9patch->texture == NULL) {
        char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }
}


void DeploymentMenu_Draw(struct Menu *mc, SDL_Texture *rt, SDL_Renderer *renderer) {
    struct DeploymentMenu   *dm         = (struct DeploymentMenu *)mc->data;
    struct n9Patch          *n9patch    = &mc->n9patch;

    SDL_assert(dm != NULL);

    if (dm->update) {
        DeploymentMenu_Update(dm, n9patch, rt, renderer);
        dm->update = false;
    }

    /* TODO: set position of DeploymentMenu */
    SDL_Rect dstrect = {
        .w = n9patch->size_pixels.x * n9patch->scale.x,
        .h = n9patch->size_pixels.y * n9patch->scale.y,
        .x = dm->pos.x,
        .y = dm->pos.y,
    };
    SDL_assert(dm->texture != NULL);
    SDL_RenderCopy(renderer, dm->texture, NULL, &dstrect);
    Utilities_DrawColor_Reset(renderer);
}


void DeploymentMenu_Update(struct DeploymentMenu *dm, struct n9Patch *n9patch,
                           SDL_Texture *rt, SDL_Renderer *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(renderer != NULL);
    SDL_assert(dm       != NULL);
    /* - variable declaration/ ants definition - */
    SDL_assert(n9patch->size_pixels.x > 0);
    SDL_assert(n9patch->size_pixels.y > 0);
    SDL_assert(n9patch->scale.x       > 0);
    SDL_assert(n9patch->scale.y       > 0);

    /* - create render target texture - */
    if (dm->texture == NULL) {
        int x  = n9patch->size_pixels.x, y = n9patch->size_pixels.y;
        dm->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_TARGET, x, y);
        SDL_assert(dm->texture != NULL);
        SDL_SetTextureBlendMode(dm->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, dm->texture);
    SDL_assert(dm->texture != NULL);

    /* --- RENDERING DEPLOYMENT-MENU --- */
    /* -- PATCHES DRAW -- */
    int scale_x         = n9patch->scale.x;
    int scale_y         = n9patch->scale.y;
    n9patch->scale.x    = 1;
    n9patch->scale.y    = 1;
    n9patch->pos.x      = 0;
    n9patch->pos.y      = 0;
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x    = scale_x;
    n9patch->scale.y    = scale_y;

    SDL_SetRenderTarget(renderer, rt);

    SDL_assert(dm->texture);
}
