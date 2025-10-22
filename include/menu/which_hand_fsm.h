#ifndef WHICH_HAND_FSM_H
#define WHICH_HAND_FSM_H
/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** whm_fsm: Finite State Machine for which hand menu
**  - Parent menu & menu option
**  - eAcpt only
**
*/

#include "enums.h"

#include "menu/which_hand.h"
#include "menu/item_action.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Game;
struct Menu;

typedef void (*fsm_whm_t)(struct Game *, struct Menu *);

extern const fsm_whm_t fsm_WHM_m[MENU_TYPE_END];
void fsm_WHM_mIAM(struct Game *s, struct Menu *mc);

extern const fsm_whm_t fsm_WHM_mIAM_mo[IAM_OPTION_NUM];
void fsm_WHM_mIAM_moUse(    struct Game *s, struct Menu *mc);
void fsm_WHM_mIAM_moEquip(  struct Game *s, struct Menu *mc);

#endif /* WHICH_HAND_FSM_H */
