#ifndef POPUP_OBJECTIVE_H
#define POPUP_OBJECTIVE_H

#include "enums.h"
#include "globals.h"
#include "n9patch.h"
#include "popup/popup.h"
#include "unit.h"
#include "pixelfonts.h"
#include "SDL2/SDL.h"
#include "stb_sprintf.h"

/* What should the objective popup do? */
// - Main objective
//    - Only that?
// - Sub-objective
//    - One only?

/* --- FORWARD DECLARATIONS --- */
struct Settings;
struct cJSON;

enum POPUP_OBJECTIVE {
    POPUP_OBJECTIVE_NULL    = -1,

    PO_PATCH_PIXELS         =  8,

    PO_PADDING_RIGHT        =  5,
    PO_PADDING_TOP          =  5,
    PO_PADDING_LEFT         =  7,
    PO_PADDING_BOTTOM       = 11,
    PO_ROW_NUM              =  2,
};

typedef struct PopUp_Objective {
    char *objective;
    char *sub_objective;
    int text_width;
    SDL_Texture *texture;
    SDL_Texture *texture_header;
    struct PixelFont *pixelnours;
    struct Padding padding;

    bool update;
} PopUp_Objective;
extern struct PopUp_Objective PopUp_Objective_default;

/* --- Constructor/Destructor --- */
extern void PopUp_Objective_Free(struct PopUp_Objective *po);
extern void PopUp_Objective_Load(struct PopUp_Objective *po, SDL_Renderer *r, struct n9Patch *n9);
extern struct PopUp_Objective *PopUp_Objective_Alloc();

/* --- Setters --- */
extern void PopUp_Objective_Set_Obj(   struct PopUp_Objective *po, char *obj);
extern void PopUp_Objective_Set_SubObj(struct PopUp_Objective *po, char *obj);

/* --- Computers --- */
extern void PopUp_Objective_Compute_Size(struct PopUp_Objective *po, struct n9Patch *n9patch);

/* --- Drawing --- */
extern void PopUp_Objective_Draw(struct PopUp *p, struct nmath_point_int32_t pos,
                                 SDL_Texture *rt, SDL_Renderer *r);
extern void PopUp_Objective_Update(struct PopUp_Objective *po,
                                   struct n9Patch *n9patch, SDL_Texture *rt, SDL_Renderer *r);
#endif /* POPUP_OBJECTIVE_H */
