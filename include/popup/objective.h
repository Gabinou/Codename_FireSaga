#ifndef POPUP_OBJECTIVE_H
#define POPUP_OBJECTIVE_H

#include "enums.h"
#include "structs.h"
#include "SDL.h"

/* What should the objective popup do? */
// - Main objective
//    - Only that?
// - Sub-objective
//    - One only?

/* --- FORWARD DECLARATIONS --- */
struct Settings;
struct n9Patch;
struct cJSON;
struct PopUp;

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
    b32 update;
    char *objective;
    char *sub_objective;
    int text_width;
    SDL_Texture *texture;
    SDL_Texture *texture_header;
    struct PixelFont *pixelnours;
    struct Padding padding;
} PopUp_Objective;
extern const struct PopUp_Objective PopUp_Objective_default;

/* --- ructor/Destructor --- */
void PopUp_Objective_Free(struct PopUp_Objective *po);
void PopUp_Objective_Load(struct PopUp_Objective *po, SDL_Renderer *r, struct n9Patch *n9);
struct PopUp_Objective *PopUp_Objective_Alloc(void);

/* --- Setters --- */
void PopUp_Objective_Set_Obj(   struct PopUp_Objective *po, char *obj);
void PopUp_Objective_Set_SubObj(struct PopUp_Objective *po, char *obj);

/* --- Computers --- */
void PopUp_Objective_Compute_Size(struct PopUp_Objective *po, struct n9Patch *n9patch);

/* --- Drawing --- */
void PopUp_Objective_Draw(struct PopUp *p, struct Point pos,
                          SDL_Texture *rt, SDL_Renderer *r);
void PopUp_Objective_Update(struct PopUp_Objective *po, struct n9Patch *n9,
                            SDL_Texture *rt, SDL_Renderer *r);

#endif /* POPUP_OBJECTIVE_H */
