
#include "fps_fsm.h"
#include "scene.h"
#include "nmath.h"
#include "game/game.h"
#include "game/map.h"
#include "map/map.h"
#include "game/cursor.h"
#include "AI.h"
#include "map/render.h"
#include "events.h"
#include "log.h"

// const fsm_main_t fsm_cFrame_s[GAME_STATE_NUM] = {
//     /* GAME_STATE_START         */ NULL,
//     /* GAME_STATE_Combat        */ NULL,
//     /* GAME_STATE_Scene_Talk    */ NULL,
//     /* GAME_STATE_Scene_FMV     */ NULL,
//     /* GAME_STATE_Gameplay_Map  */ &fsm_cFrame_sGmpMap,
//     /* GAME_STATE_Gameplay_Camp */ NULL,
//     /* GAME_STATE_Preparation   */ NULL,
//     /* GAME_STATE_Title_Screen  */ NULL,
//     /* GAME_STATE_Animation     */ NULL
// };

// const fsm_main_t fsm_cFrame_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
//     /* GAME_SUBSTATE_START          */ NULL,
//     /* GAME_SUBSTATE_MAP_MINIMAP    */ NULL,
//     /* GAME_SUBSTATE_MENU           */ NULL,
//     /* GAME_SUBSTATE_MAP_UNIT_MOVES */ NULL,
//     /* GAME_SUBSTATE_MAP_COMBAT     */ NULL,
//     /* GAME_SUBSTATE_MAP_NPCTURN    */ &fsm_cFrame_sGmpMap_ssMapNPC,
//     /* GAME_SUBSTATE_SAVING         */ NULL,
//     /* GAME_SUBSTATE_STANDBY        */ NULL,
//     /* GAME_SUBSTATE_MAP_CANDIDATES */ NULL,
//     /* GAME_SUBSTATE_CUTSCENE       */ NULL,
//     /* GAME_SUBSTATE_MAP_ANIMATION  */ NULL,
// };

/* --- CONTROL ---*/
//  - What is fsm_cFrame used for?
//      - AI        -> system?

// void fsm_cFrame_sGmpMap(              Game *IES) {
//     if (fsm_cFrame_sGmpMap_ss[Game_Substate_Current(IES)] != NULL) {
//         fsm_cFrame_sGmpMap_ss[Game_Substate_Current(IES)](IES);
//     }
// }

/* --- AI CONTROL HAPPENS HERE --- */
// void fsm_cFrame_sGmpMap_ssMapNPC(struct Game *IES) {
// Game_AI_Enemy_Turn(IES);
// }
