
#ifndef SYSTEM_CONTROL_H
#define SYSTEM_CONTROL_H

#include "tnecs.h"
#include "types.h"
#include "enums.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;

void Cursor_AI(             tnecs_In *input);
void Cursor_Moves(          tnecs_In *input);
void Control_Gamepad(       tnecs_In *input);
void Control_Touchpad(      tnecs_In *input);
void Control_Keyboard(      tnecs_In *input);
void Cursor_Scroll_Camera(  tnecs_In *input);
void Cursor_Follows_Mouse(  tnecs_In *input);
void Control_Pressed(i8 IES_b,
                     i8 *press,
                     i8 *pressed_num,
                     i32 *controller_type,
                     u32 event,
                     b32 block,
                     i32 t_held_ns,
                     i32 t_min_ns,
                     void *controller);

typedef void (*fsm_main_t)(struct Game *);

extern const fsm_main_t fsm_cfollows_s[GAME_STATE_NUM];
extern const fsm_main_t fsm_cfollows_sGmpMap_ss[GAME_SUBSTATE_NUM];
extern const fsm_main_t fsm_cfollows_sPrep_ss[GAME_SUBSTATE_NUM];
extern const fsm_main_t fsm_cmoves_s[GAME_STATE_NUM];
extern const fsm_main_t fsm_cmoves_sGmpMap_ss[GAME_SUBSTATE_NUM];
extern const fsm_main_t fsm_cmoves_sPrep_ss[GAME_SUBSTATE_NUM];

void fsm_cmoves_sPrep(              struct Game *IES);
void fsm_cmoves_sGmpMap(            struct Game *IES);
void fsm_cmoves_sTtlScrn(           struct Game *IES);

void fsm_cmoves_sGmpMap_ssMenu(     struct Game *IES);
void fsm_cmoves_sGmpMap_ssStby(     struct Game *IES);
void fsm_cmoves_sGmpMap_ssMapUnitMv(struct Game *IES);
void fsm_cmoves_sPrep_ssMapCndt(    struct Game *IES);
void fsm_cmoves_sPrep_ssMenu(       struct Game *IES);
void fsm_cmoves_sGmpMap_ssMapCndt(  struct Game *IES);

void fsm_cfollows_sPrep(              struct Game *IES);
void fsm_cfollows_sGmpMap(            struct Game *IES);
void fsm_cfollows_sTtlScrn(           struct Game *IES);

void fsm_cfollows_sGmpMap_ssMenu(     struct Game *IES);
void fsm_cfollows_sGmpMap_ssStby(     struct Game *IES);
void fsm_cfollows_sGmpMap_ssMapUnitMv(struct Game *IES);
void fsm_cfollows_sPrep_ssMenu(       struct Game *IES);

#endif /* SYSTEM_CONTROL_H */
