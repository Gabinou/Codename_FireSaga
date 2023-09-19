#ifndef MENU_FPS_H
#define MENU_FPS_H

#include "enums.h"
#include "menu/menu.h"
#include "menu/staff_select.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Game;

/* --- TYPEDEFS --- */
typedef void (*fsm_menu_t)(struct Game *, struct MenuComponent *);

/* --- Menu FSMs --- */
/* -- NAMING CONVENTION -- */
/*  Array:      fsm_<type>_<event>
*   Type:       fsm_<type>_<event>_t
*   Function:   fsm_<type>_<event>_<INDEX>
*   <fsm_array>[<index>] -> returns <fsm_function>
*   Ex: fsm_substate_IC_MAP_UNIT_MOVES
*/

/* -- Menu-type FSMs -- */
extern fsm_menu_t fsm_menu_type_IA[MENU_TYPE_END];
extern fsm_menu_t fsm_menu_type_IC[MENU_TYPE_END];
extern fsm_menu_t fsm_menu_type_CMs[MENU_TYPE_END];

/* event_Input_Accept */
void fsm_menu_type_IA_trade(        struct Game *sota, struct MenuComponent *mc);
void fsm_menu_type_IA_stats(        struct Game *sota, struct MenuComponent *mc);
void fsm_menu_type_IA_pre_combat(   struct Game *sota, struct MenuComponent *mc);
void fsm_menu_type_IA_item_select(  struct Game *sota, struct MenuComponent *mc);
void fsm_menu_type_IA_staff_select( struct Game *sota, struct MenuComponent *mc);
void fsm_menu_type_IA_player_select(struct Game *sota, struct MenuComponent *mc);
void fsm_menu_type_IA_weapon_select(struct Game *sota, struct MenuComponent *mc);

/* event_Input_Cancel */
void fsm_menu_type_IC_stats(        struct Game *sota, struct MenuComponent *mc);
void fsm_menu_type_IC_trade(        struct Game *sota, struct MenuComponent *mc);
void fsm_menu_type_IC_pre_combat(   struct Game *sota, struct MenuComponent *mc);
void fsm_menu_type_IC_item_select(  struct Game *sota, struct MenuComponent *mc);
void fsm_menu_type_IC_staff_select( struct Game *sota, struct MenuComponent *mc);
void fsm_menu_type_IC_player_select(struct Game *sota, struct MenuComponent *mc);
void fsm_menu_type_IC_weapon_select(struct Game *sota, struct MenuComponent *mc);

/* event_Cursor_Moves */
void fsm_menu_type_CMs_item_select(  struct Game *sota, struct MenuComponent *mc);
void fsm_menu_type_CMs_staff_select( struct Game *sota, struct MenuComponent *mc);
void fsm_menu_type_CMs_weapon_select(struct Game *sota, struct MenuComponent *mc);

/* -- PSM -option FSMs -- */
// Only posible state pair [Gameplay_Map, MENU]
extern fsm_menu_t fsm_psm_options_IA[MENU_OPTION_NUM];

void fsm_psm_option_IA_quit(     struct Game *sota, struct MenuComponent *mc);
void fsm_psm_option_IA_wait(     struct Game *sota, struct MenuComponent *mc);
void fsm_psm_option_IA_staff(    struct Game *sota, struct MenuComponent *mc);
void fsm_psm_option_IA_dance(    struct Game *sota, struct MenuComponent *mc);
void fsm_psm_option_IA_items(    struct Game *sota, struct MenuComponent *mc);
void fsm_psm_option_IA_trade(    struct Game *sota, struct MenuComponent *mc);
void fsm_psm_option_IA_attack(   struct Game *sota, struct MenuComponent *mc);
void fsm_psm_option_IA_end_turn( struct Game *sota, struct MenuComponent *mc);
void fsm_psm_option_IA_debug_map(struct Game *sota, struct MenuComponent *mc);

/* -- Map candidates FSMs -- */
/* event_Input_Accept */
extern fsm_menu_t fsm_map_candidates_IA[MENU_OPTION_NUM];

void fsm_map_candidate_IA_staff( struct Game *sota, struct MenuComponent *mc);
void fsm_map_candidate_IA_dance( struct Game *sota, struct MenuComponent *mc);
void fsm_map_candidate_IA_trade( struct Game *sota, struct MenuComponent *mc);
void fsm_map_candidate_IA_attack(struct Game *sota, struct MenuComponent *mc);

/* event_Input_Cancel */
extern fsm_menu_t fsm_map_candidates_IC[MENU_OPTION_NUM];

void fsm_map_candidate_IC_dance( struct Game *sota, struct MenuComponent *mc);
void fsm_map_candidate_IC_staff( struct Game *sota, struct MenuComponent *mc);
void fsm_map_candidate_IC_trade( struct Game *sota, struct MenuComponent *mc);
void fsm_map_candidate_IC_attack(struct Game *sota, struct MenuComponent *mc);

#endif /* MENU_FPS_H */
