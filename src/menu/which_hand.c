
#include "menu/which_hand.h"

i32 WhichHandMenu_Select(struct WhichHandMenu *whm, 
                         i32 elem){ 
    SDL_assert(elem < num_handedness);
    /* Ouput UNIT_EQUIP_... from elem */
    return(whm->handedness[elem]);
}

void WhichHandMenu_Draw(struct Menu     *mc,
                        SDL_Texture     *rt,
                        SDL_Renderer    *r) {

}

void WhichHandMenu_Update(struct WhichHandMenu  *whm,
                          struct n9Patch         *n9,
                          SDL_Texture            *rt,
                          SDL_Renderer           *r) {
    
}
