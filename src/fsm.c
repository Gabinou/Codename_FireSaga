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
** fsm: Finite State Machine for game.state.top
**  - Different behavior according to Game State
**
*/

#include "AI.h"
#include "fsm.h"
#include "arrow.h"
#include "nmath.h"
#include "scene.h"
#include "events.h"
#include "slider.h"
#include "sprite.h"
#include "octant.h"
#include "globals.h"
#include "cutscene.h"
#include "position.h"
#include "platform.h"
#include "utilities.h"
#include "pathfinding.h"

#include "game/map.h"
#include "game/game.h"
#include "game/unit.h"
#include "game/menu.h"
#include "game/popup.h"
#include "game/cursor.h"

#include "map/path.h"
#include "map/ontile.h"
#include "map/render.h"

#include "menu/fsm.h"
#include "menu/menu.h"
#include "menu/stats.h"
#include "menu/option_fsm.h"
#include "menu/map_action.h"
#include "menu/deployment.h"
#include "menu/staff_select.h"

#include "popup/unit.h"
#include "popup/tile.h"
#include "popup/popup.h"

#include "systems/slide.h"

#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/range.h"
#include "unit/equipment.h"

/* --- ACTIONS --- */
const fsm_eGmp2Stby_s_t fsm_eGmp2Stby_s[GAME_STATE_NUM] = {
    [GAME_STATE_START]          = NULL,
    [GAME_STATE_Combat]         = NULL,
    [GAME_STATE_Scene_Talk]     = NULL,
    [GAME_STATE_Cutscene]       = NULL,
    [GAME_STATE_Gameplay_Map]   = &fsm_eGmp2Stby_sGmpMap,
    [GAME_STATE_Gameplay_Camp]  = NULL,
    [GAME_STATE_Preparation]    = NULL,
    [GAME_STATE_Title_Screen]   = NULL,
    [GAME_STATE_Animation]      = NULL,
};

/* --- Input events --- */
const fsm_eAcpt_s_t fsm_eAcpt_s[GAME_STATE_NUM] = {
    [GAME_STATE_START]          = NULL,
    [GAME_STATE_Combat]         = NULL,
    [GAME_STATE_Scene_Talk]     = &fsm_eAcpt_sScnTalk,
    [GAME_STATE_Cutscene]       = &fsm_eAcpt_sCutScn,
    [GAME_STATE_Gameplay_Map]   = &fsm_eAcpt_sGmpMap,
    [GAME_STATE_Gameplay_Camp]  = NULL,
    [GAME_STATE_Preparation]    = &fsm_eAcpt_sPrep,
    [GAME_STATE_Title_Screen]   = &fsm_eAcpt_sTtlScrn,
    [GAME_STATE_Animation]      = NULL,
};

const fsm_eStats_s_t fsm_eStats_s[GAME_STATE_NUM] = {
    [GAME_STATE_START]          = NULL,
    [GAME_STATE_Combat]         = NULL,
    [GAME_STATE_Scene_Talk]     = NULL,
    [GAME_STATE_Cutscene]       = NULL,
    [GAME_STATE_Gameplay_Map]   = &fsm_eStats_sGmpMap,
    [GAME_STATE_Gameplay_Camp]  = NULL,
    [GAME_STATE_Preparation]    = &fsm_eStats_sPrep,
    [GAME_STATE_Title_Screen]   = NULL,
    [GAME_STATE_Animation]      = NULL,
};

const fsm_eStart_s_t fsm_eStart_s[GAME_STATE_NUM] = {
    [GAME_STATE_START]          = NULL,
    [GAME_STATE_Combat]         = NULL,
    [GAME_STATE_Scene_Talk]     = &fsm_eStart_sScnTalk,
    [GAME_STATE_Cutscene]       = &fsm_eStart_sCutScn,
    [GAME_STATE_Gameplay_Map]   = NULL,
    [GAME_STATE_Gameplay_Camp]  = NULL,
    [GAME_STATE_Preparation]    = &fsm_eStart_sPrep,
    [GAME_STATE_Title_Screen]   = NULL,
    [GAME_STATE_Animation]      = NULL
};

const fsm_eCncl_s_t fsm_eCncl_s[GAME_STATE_NUM] = {
    [GAME_STATE_START]          = NULL,
    [GAME_STATE_Combat]         = NULL,
    [GAME_STATE_Scene_Talk]     = &fsm_eAcpt_sScnTalk,
    [GAME_STATE_Cutscene]       = NULL,
    [GAME_STATE_Gameplay_Map]   = &fsm_eCncl_sGmpMap,
    [GAME_STATE_Gameplay_Camp]  = NULL,
    [GAME_STATE_Preparation]    = &fsm_eCncl_sPrep,
    [GAME_STATE_Title_Screen]   = NULL,
    [GAME_STATE_Animation]      = NULL,
};

const fsm_eMenuRight_s_t fsm_eMenuRight_s[GAME_STATE_NUM] = {
    [GAME_STATE_START]          = NULL,
    [GAME_STATE_Combat]         = NULL,
    [GAME_STATE_Scene_Talk]     = &fsm_eMenuRight_sScnTalk,
    [GAME_STATE_Cutscene]       = NULL,
    [GAME_STATE_Gameplay_Map]   = &fsm_eMenuRight_sGmpMap,
    [GAME_STATE_Gameplay_Camp]  = NULL,
    [GAME_STATE_Preparation]    = NULL,
    [GAME_STATE_Title_Screen]   = NULL,
    [GAME_STATE_Animation]      = NULL,
};

const fsm_eMenuLeft_s_t fsm_eMenuLeft_s[GAME_STATE_NUM] = {
    [GAME_STATE_START]          = NULL,
    [GAME_STATE_Combat]         = NULL,
    [GAME_STATE_Scene_Talk]     = &fsm_eMenuRight_sScnTalk,
    [GAME_STATE_Cutscene]       = NULL,
    [GAME_STATE_Gameplay_Map]   = &fsm_eMenuLeft_sGmpMap,
    [GAME_STATE_Gameplay_Camp]  = NULL,
    [GAME_STATE_Preparation]    = &fsm_eMenuLeft_sPrep,
    [GAME_STATE_Title_Screen]   = NULL,
    [GAME_STATE_Animation]      = NULL,
};

const fsm_eCrsMvs_s_t fsm_eCrsMvs_s[GAME_STATE_NUM] = {
    [GAME_STATE_START]          = NULL,
    [GAME_STATE_Combat]         = NULL,
    [GAME_STATE_Scene_Talk]     = NULL,
    [GAME_STATE_Cutscene]       = NULL,
    [GAME_STATE_Gameplay_Map]   = &fsm_eCrsMvs_sGmpMap,
    [GAME_STATE_Gameplay_Camp]  = NULL,
    [GAME_STATE_Preparation]    = &fsm_eCrsMvs_sPrep,
    [GAME_STATE_Title_Screen]   = NULL,
    [GAME_STATE_Animation]      = NULL,
};

const fsm_eCrsMvd_s_t fsm_eCrsMvd_s[GAME_STATE_NUM] = {
    [GAME_STATE_START]          = NULL,
    [GAME_STATE_Combat]         = NULL,
    [GAME_STATE_Scene_Talk]     = NULL,
    [GAME_STATE_Cutscene]       = NULL,
    [GAME_STATE_Gameplay_Map]   = &fsm_eCrsMvd_sGmpMap,
    [GAME_STATE_Gameplay_Camp]  = NULL,
    [GAME_STATE_Preparation]    = NULL,
    [GAME_STATE_Title_Screen]   = NULL,
    [GAME_STATE_Animation]      = NULL,
};

/* --- SUB-ACTIONS --- */
const fsm_eGmp2Stby_s_t fsm_eGmp2Stby_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = NULL,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = &fsm_eGmp2Stby_ssMapUnitMv,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = NULL,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = NULL,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL,
};

const fsm_eCrsMvs_s_t fsm_eCrsMvs_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = &fsm_eCrsMvs_ssMenu,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = NULL,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = NULL,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eCrsMvs_s_t fsm_eCrsMvs_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = NULL,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = &fsm_eCrsMvs_sGmpMap_ssMapUnitMv,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = &fsm_eCrsMvs_sGmpMap_ssStby,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = &fsm_eCrsMvs_sGmpMap_ssMapCndt,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eCrsMvd_s_t fsm_eCrsMvd_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = NULL,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = &fsm_eCrsMvd_sGmpMap_ssMapUnitMv,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = &fsm_eCrsMvd_sGmpMap_ssStby,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = &fsm_eCrsMvd_sGmpMap_ssMapCndt,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eCrsMvs_s_t fsm_eCrsMvs_sPrep_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = NULL,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = NULL,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = &fsm_eCrsMvs_sPrep_ssMapCndt,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eAcpt_s_t fsm_eStart_sPrep_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = &fsm_eStart_sPrep_ssMenu,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = NULL,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = &fsm_eStart_sPrep_ssMapCndt,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eMenuLeft_sPrep_ss_t fsm_eMenuLeft_sPrep_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = &fsm_eMenuLeft_sPrep_ssMenu,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = NULL,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = &fsm_eMenuLeft_sPrep_ssMapCndt,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eAcpt_s_t fsm_eAcpt_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = &fsm_eAcpt_sGmpMap_ssMapMini,
    [GAME_SUBSTATE_MENU]            = &fsm_eAcpt_sGmpMap_ssMenu,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = &fsm_eAcpt_sGmpMap_ssMapUnitMv,
    [GAME_SUBSTATE_MAP_COMBAT]      = &fsm_eAcpt_sGmpMap_ssMapCmbt,
    [GAME_SUBSTATE_MAP_NPCTURN]     = &fsm_eAcpt_sGmpMap_ssMapNPC,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = &fsm_eAcpt_sGmpMap_ssStby,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = &fsm_eAcpt_sGmpMap_ssMapCndt,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eAcpt_s_t fsm_eAcpt_sPrep_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = &fsm_eAcpt_sGmpMap_ssMenu,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = NULL,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = &fsm_eAcpt_sPrep_ssMapCndt,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eCncl_s_t fsm_eCncl_sPrep_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = &fsm_eCncl_sPrep_ssMenu,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = NULL,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = &fsm_eCncl_sPrep_ssMapCndt,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eCncl_s_t fsm_eCncl_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = &fsm_eCncl_sGmpMap_ssMenu,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = &fsm_eCncl_sGmpMap_ssMapUnitMv,
    [GAME_SUBSTATE_MAP_COMBAT]      = &fsm_eCncl_sGmpMap_ssMapCmbt,
    [GAME_SUBSTATE_MAP_NPCTURN]     = &fsm_eCncl_sGmpMap_ssMapNPC,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = &fsm_eCncl_sGmpMap_ssStby,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = &fsm_eCncl_sGmpMap_ssMapCndt,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eStats_s_t fsm_eStats_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = NULL,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = &fsm_eStats_sGmpMap_ssStby,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = &fsm_eStats_sGmpMap_ssStby,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eStats_s_t fsm_eStats_sPrep_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = &fsm_eStats_sPrep_ssMenu,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = NULL,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = &fsm_eStats_sPrep_ssMapCndt,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eUnitDsel_s_t fsm_eUnitDsel_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = &fsm_eUnitDsel_ssMenu,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = &fsm_eUnitDsel_ssMapUnitMv,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = NULL,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = NULL,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eUnitSel_s_t fsm_eUnitSel_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = NULL,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = &fsm_eUnitSel_ssStby,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = NULL,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eUnitDng_s_t fsm_eUnitDng_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = NULL,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = &fsm_eUnitDng_ssStby,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = NULL,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eMenuRight_s_t fsm_eMenuRight_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = &fsm_eMenuRight_sGmpMap_ssMenu,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = &fsm_eMenuRight_sGmpMap_ssStby,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = NULL,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eMenuLeft_s_t fsm_eMenuLeft_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = &fsm_eMenuLeft_sGmpMap_ssMenu,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = &fsm_eMenuLeft_sGmpMap_ssStby,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = NULL,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eCrsHvUnit_s_t fsm_eCrsHvUnit_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = NULL,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = &fsm_eCrsHvUnit_ssStby,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = &fsm_eCrsHvUnit_ssMapCndt,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eCrsDeHvUnit_s_t fsm_eCrsDeHvUnit_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = NULL,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = &fsm_eCrsDeHvUnit_ssStby,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = &fsm_eCrsDeHvUnit_ssMapCndt,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

/* -- FSM: Input_globalRange EVENT -- */
const fsm_eGlbRng_s_t fsm_eGlbRng_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = NULL,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = NULL,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = &fsm_eGlbRng_ssStby,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = NULL,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL
};

const fsm_eCmbtEnd_ss_t fsm_eCmbtEnd_ss[GAME_SUBSTATE_NUM] = {
    [GAME_SUBSTATE_START]           = NULL,
    [GAME_SUBSTATE_MAP_MINIMAP]     = NULL,
    [GAME_SUBSTATE_MENU]            = NULL,
    [GAME_SUBSTATE_MAP_UNIT_MOVES]  = NULL,
    [GAME_SUBSTATE_MAP_COMBAT]      = NULL,
    [GAME_SUBSTATE_MAP_NPCTURN]     = &fsm_eCmbtEnd_ssMapNPC,
    [GAME_SUBSTATE_SAVING]          = NULL,
    [GAME_SUBSTATE_STANDBY]         = NULL,
    [GAME_SUBSTATE_MAP_CANDIDATES]  = &fsm_eCmbtEnd_ssMapCndt,
    [GAME_SUBSTATE_CUTSCENE]        = NULL,
    [GAME_SUBSTATE_MAP_ANIMATION]   = NULL,
};

/* -- FSM: Input_globalRange EVENT -- */
void fsm_eGlbRng_ssStby(struct Game *sota) {
    /* --- Toggle global range --- */

    Map *map = Game_Map(sota);
    for (int i = 0; i < DARR_NUM(map->units.onfield.enemies); i++) {
        tnecs_E entity = map->units.onfield.enemies[i];

        /* - Compute new dangermap  - */
        i32 *temp_danger = Map_Danger_Compute(map, entity);

        if (map->flags.show_globalRange) {
            /* Currently showing global map, removing */
            Map_Global_Danger_Sub(map, temp_danger);
        } else {
            /* Currently not showing global map, adding */
            Map_Global_Danger_Add(map, temp_danger);
        }
    }

    if (map->flags.show_globalRange) {
        /* Currently showing global map, removing */
        /* Plus, adding back player dangermap */
        Map_Palettemap_Autoset(map, MAP_OVERLAY_DANGER, TNECS_NULL);
        Map_Danger_Perimeter_Compute(map, map->darrs.dangermap);
        map->darrs.rendered_dangermap = map->darrs.dangermap;
    } else {
        /* Currently not showing global map, adding */
        Map_Palettemap_Autoset(map, MAP_OVERLAY_GLOBAL_DANGER, TNECS_NULL);
        Map_Danger_Perimeter_Compute(map, map->darrs.global_dangermap);
        map->darrs.rendered_dangermap = map->darrs.global_dangermap;
    }

    /* Switching show_globalRange to new mode */
    map->flags.show_globalRange = !map->flags.show_globalRange;
}

// --- FSM ACTION AND SUBACTION DEFINITIONS ---

// -- FSM: Cursor_Hovers_Unit --
void fsm_eCrsHvUnit_ssStby(struct Game *sota, tnecs_E hov_ent) {
    /* --- Show popup_unit --- */
    SDL_assert(hov_ent > TNECS_NULL);
    // TODO: put unit popup Loading into Map/Gameplay loading functions
    tnecs_E popup_ent = sota->popups.arr[POPUP_TYPE_HUD_UNIT];
    if (popup_ent == TNECS_NULL)
        Game_PopUp_Unit_Create(sota);
    popup_ent = sota->popups.arr[POPUP_TYPE_HUD_UNIT];
    SDL_assert(popup_ent != TNECS_NULL);

    struct PopUp *popup = IES_GET_C(gl_world, popup_ent, PopUp);
    struct PopUp_Unit *popup_unit = popup->data;
    PopUp_Unit_Set(popup_unit, sota);
    SDL_assert(popup_unit->unit != NULL);
    popup->visible = true;
    const struct Position *cursor_pos = IES_GET_C(gl_world, sota->cursor.entity,
                                                  Position);
    // TODO: use struct Point everywhere, replace .x = with struct equality
    struct Point pos = cursor_pos->tilemap_pos;

    Game_PopUp_Unit_Place(sota, pos);

    // struct Slider *slider  = IES_GET_C(gl_world, popup_ent, Slider);
    struct SliderOffscreen *offscreen;
    offscreen = IES_GET_C(gl_world, popup_ent, SliderOffscreen);
    offscreen->reverse = false;

    struct Unit *unit_ontile = IES_GET_C(gl_world, hov_ent, Unit);
    if (Unit_isWaiting(unit_ontile)) {
        return;
    }

    /* -- Compute attackmap and movemap -- */
    Map *map = Game_Map(sota);
    map->flags.update   = true;

    /* - MapAct settings for attacktolist - */
    MapAct map_to = MapAct_default;

    /* Don't show movemap if AI never moves */
    struct Unit_AI *ai = IES_GET_C(gl_world, hov_ent, Unit_AI);
    map_to.move = (ai == NULL) ? true : !Unit_AI_Move_Never(ai);

    map_to.archetype        = ITEM_ARCHETYPE_STAFF;
    map_to.eq_type          = LOADOUT_EQUIPMENT;
    map_to.output_type      = ARRAY_MATRIX;
    map_to.aggressor        = hov_ent;
    map_to.mode_movetile    = MOVETILE_INCLUDE;

    /* - healtopmap - */
    Map_Act_To(map, map_to);

    /* - attacktomap - */
    map_to.archetype     = ITEM_ARCHETYPE_WEAPON;
    Map_Act_To(map, map_to);

    // SDL_Log("MOVE");
    // matrix_print(map->darrs.movemap, Map_row_len(map), Map_col_len(map));
    // SDL_Log("ATK");
    // matrix_print(map->darrs.attacktomap, Map_row_len(map), Map_col_len(map));
    // SDL_Log("HEAL");
    // matrix_print(map->darrs.healtomap, Map_row_len(map), Map_col_len(map));

    int rangemap = Unit_Rangemap_Get(unit_ontile);

    /* - Compute new stackmap with recomputed attacktomap - */
    int overlays = MAP_OVERLAY_MOVE + MAP_OVERLAY_DANGER + MAP_OVERLAY_GLOBAL_DANGER;
    if (rangemap        == RANGEMAP_HEALMAP) {
        overlays += MAP_OVERLAY_HEAL;
        Map_Palettemap_Autoset(map, overlays, hov_ent);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        overlays += MAP_OVERLAY_ATTACK;
        Map_Palettemap_Autoset(map, overlays, TNECS_NULL);
    }

    /* Stack all overlay maps */
    if (map->flags.show_globalRange) {
        Map_Stacked_Dangermap_Compute(map, map->darrs.global_dangermap);
    } else {
        Map_Stacked_Dangermap_Compute(map, map->darrs.dangermap);
    }
    map->flags.show_icons = SotA_isPC(Unit_Army(unit_ontile));

    /* -- Changing animation loop to Taunt -- */
    struct Sprite *sprite = IES_GET_C(gl_world, hov_ent, Sprite);
    b32 animated = IES_E_HAS_C(gl_world, hov_ent, Timer);
    if ((sprite->spritesheet != NULL) && (animated) && (! Unit_isWaiting(unit_ontile))) {
        if (sprite->spritesheet->loop_num == MAP_UNIT_LOOP_NUM) {
            Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_TAUNT, sprite->flip);
            Sprite_Animation_Loop(sprite);
        }
    }
}

void fsm_eCrsHvUnit_ssMapCndt(struct Game *sota, tnecs_E hov_ent) {
    /* --- Select new candidate for action --- */
    SDL_assert(hov_ent > TNECS_NULL);
    sota->hovered.unit_entity = hov_ent;
}

// -- FSM: Cursor_Dehovers_Unit --
void fsm_eCrsDeHvUnit_ssStby(struct Game *sota, tnecs_E dehov_ent) {
    /* -- Re-computing overlay -- */
    Map *map = Game_Map(sota);
    Map_Palettemap_Autoset(map, MAP_OVERLAY_DANGER + MAP_OVERLAY_GLOBAL_DANGER, TNECS_NULL);
    Map_Stacked_Dangermap_Reset(map);

    tnecs_E popup_ent = sota->popups.arr[POPUP_TYPE_HUD_UNIT];
    SDL_assert(popup_ent > TNECS_NULL);

    /* -- Getting popup -- */
    // const struct Position *cursor_pos;
    // cursor_pos = IES_GET_C(gl_world, sota->cursor.entity, Position);
    // struct Point pos    = cursor_pos->tilemap_pos;
    struct PopUp *popup = IES_GET_C(gl_world, popup_ent, PopUp);
    struct n9Patch *n9patch = &popup->n9patch;
    // struct PopUp_Unit *popup_unit = popup->data;
    // popup_unit->unit = NULL;

    /* -- Placing popup_unit out of view -- */
    struct Position *position  = IES_GET_C(gl_world, popup_ent, Position);
    struct Slider *slider  = IES_GET_C(gl_world, popup_ent, Slider);
    struct SliderOffscreen *offscreen;
    offscreen = IES_GET_C(gl_world, popup_ent, SliderOffscreen);

    SDL_Rect rect = Utilities_Rect(position, n9patch);

    Slider_Target_Offscreen_Far(slider, offscreen, &rect);
    Slider_Init(slider, &position->pixel_pos, &offscreen->target);
    offscreen->reverse = false;

    /* -- Changing animation loop to IDLE -- */
    struct Unit *unit     = IES_GET_C(gl_world, dehov_ent, Unit);
    struct Sprite *sprite = IES_GET_C(gl_world, dehov_ent, Sprite);
    SDL_assert(unit   != NULL);
    SDL_assert(sprite != NULL);
    b32 animated = IES_E_HAS_C(gl_world, dehov_ent, Timer);

    /* Only if unit doesn't wait */
    if ((sprite->spritesheet != NULL) && (animated) && (!Unit_isWaiting(unit))) {
        if (sprite->spritesheet->loop_num == MAP_UNIT_LOOP_NUM) {
            Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_IDLE, sprite->flip);
            Sprite_Animation_Loop(sprite);
        }
    }

    // In case an enemy unit was selected.
    sota->selected.unit_entity = TNECS_NULL;
}

void fsm_eCrsDeHvUnit_ssMapCndt(struct Game *sota, tnecs_E dehov_ent) {
    /* -- Reset unit loop to Idle --  */
    // ONLY FOR ATTACK
    struct Sprite *sprite = IES_GET_C(gl_world, dehov_ent, Sprite);
    Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_IDLE, sprite->flip);
    Sprite_Animation_Loop(sprite);
}

/* -- FSM: INPUT_CANCEL EVENT -- */
void fsm_eCncl_sPrep_ssMenu(struct Game *sota, tnecs_E ent) {
    SDL_assert(sota->menus.deployment > TNECS_NULL);

    /* - Pop the menu - */
    struct Menu *mc;
    tnecs_E top_menu = sota->menus.stack[DARR_NUM(sota->menus.stack) - 1];
    mc = IES_GET_C(gl_world, top_menu, Menu);

    Game_menuStack_Pop(sota, false);

    /* - Focus on new menu - */
    if (DARR_NUM(sota->menus.stack) > 0) {
        /* Return to top menu */
        mc->visible = false;
        Game_cursorFocus_onMenu(sota);
    } else {
        /* Return to Map candidates */
        Game_subState_Set(
                sota, GAME_SUBSTATE_MAP_CANDIDATES,
                "Change to map candidates in Prep"
        );

        /* - Reset potential candidates - */
        sota->targets.order     = 0;

        /* - Focus on map - */
        mc->visible = false;
        Game_cursorFocus_onMap(sota);
    }
}

void fsm_eCncl_sPrep_ssMapCndt( struct Game *sota, tnecs_E ent) {
    /* --- Starting Position -> Deployment Menu --- */
    fsm_eMenuLeft_sPrep_ssMapCndt(sota);
}

void fsm_eCncl_sPrep(struct Game *sota, tnecs_E canceller) {
    if (fsm_eCncl_sPrep_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eCncl_sPrep_ss[Game_Substate_Current(sota)](sota, canceller);
}

void fsm_eCncl_sGmpMap(struct Game *sota, tnecs_E canceller) {
    if (fsm_eCncl_sGmpMap_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eCncl_sGmpMap_ss[Game_Substate_Current(sota)](sota, canceller);
}

void fsm_eUnitDng_ssStby(struct Game *sota, tnecs_E selector_entity) {
    /* --- Show dangermap for selected unit --- */
    /* -- Preliminaries -- */
    tnecs_E selected = sota->selected.unit_entity;
    SDL_assert(selector_entity > TNECS_NULL);
    SDL_assert(sota->selected.unit_entity > TNECS_NULL);

    struct Unit *unit = IES_GET_C(gl_world, selected, Unit);

    /* -- Skip if enemy can't attack -- */
    if (!Unit_canAttack(unit)) {
        SDL_Log("Enemy unit can't attack");
        return;
    }

    Map *map = Game_Map(sota);
    if (map->flags.show_globalRange) {
        SDL_Log("Global range/danger shown. Do nothing for Unit dangermap.");
        return;
    }

    /* -- Computing new dangermap -- */
    // struct Position *pos    = IES_GET_C(gl_world, selector_entity, Position);
    i32 *temp_danger        = Map_Danger_Compute(map, selected);
    // int map_index = pos->tilemap_pos.y * Map_col_len(map) + pos->tilemap_pos.x;
    if (Unit_showsDanger(unit)) {
        Map_Danger_Sub(map, temp_danger);
        Map_Palettemap_Autoset(map,
                               MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_DANGER + MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK, TNECS_NULL);
        Map_Danger_Perimeter_Compute(map, map->darrs.dangermap);
        map->darrs.rendered_dangermap = map->darrs.dangermap;

        Unit_showsDanger_set(unit, false);
    } else {
        Map_Danger_Add(map, temp_danger);
        Map_Palettemap_Autoset(map, MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_DANGER, TNECS_NULL);
        Map_Danger_Perimeter_Compute(map, map->darrs.dangermap);
        map->darrs.rendered_dangermap = map->darrs.dangermap;
        Unit_showsDanger_set(unit, true);
    }

    // matrix_print(map->darrs.attacktomap, Map_row_len(map), Map_col_len(map));
}

void fsm_eCncl_sGmpMap_ssStby(struct Game *sota, tnecs_E canceller) {
    SDL_assert(canceller > 0);
    SDL_assert(Game_State_Current(sota) == GAME_STATE_Gameplay_Map);
    /* -- Preliminaries -- */
    Map *map = Game_Map(sota);
    tnecs_E *data1 = IES_calloc(1, sizeof(data1));
    *data1 = canceller;
    struct Unit *unit_ontile;
    struct Position *pos = IES_GET_C(gl_world, canceller, Position);
    struct Point cpos = pos->tilemap_pos;
    tnecs_E ontile = map->darrs.unitmap[cpos.y * Map_col_len(map) + cpos.x];

    if (ontile == TNECS_NULL) {
        return;
    }

    /* -- Show danger map if enemy -- */
    unit_ontile = IES_GET_C(gl_world, ontile, Unit);
    SDL_assert(unit_ontile);

    tnecs_E *data2 = IES_calloc(1, sizeof(data2));
    *data2 = ontile;
    if (!SotA_isPC(Unit_Army(unit_ontile)) && Unit_showsDanger(unit_ontile))
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Danger, data1, data2);

}

void fsm_eCncl_sGmpMap_ssMapCndt(struct Game *sota, tnecs_E canceller) {
    Map *map = Game_Map(sota);

    /* 1. Dehover candidate defendant */
    SDL_assert(sota->cursor.entity != TNECS_NULL);
    const struct Position *cursor_pos;
    cursor_pos = IES_GET_C(gl_world, sota->cursor.entity, Position);

    struct Point pos = cursor_pos->tilemap_pos;
    int current_i = pos.y * Map_col_len(map) + pos.x;
    tnecs_E ontile = map->darrs.unitmap[current_i];

    SDL_assert(ontile > TNECS_NULL);
    /* TODO: limit calling FSM function directly. */
    fsm_eCrsDeHvUnit_ssMapCndt(sota, ontile);

    /* 2. set state to Menu */
    Game_subState_Set(
            sota, GAME_SUBSTATE_MENU,
            "Cancel Selection of candidates"
    );

    /* --- eCncl_mo independent of parent menu --- */
    if (fsm_eCncl_mo[sota->selected.menu_option] != NULL)
        fsm_eCncl_mo[sota->selected.menu_option](sota, NULL);
}

void fsm_eCncl_sGmpMap_ssMenu(Game *sota, tnecs_E canceller) {
    tnecs_E ent_topop = sota->menus.stack[DARR_NUM(sota->menus.stack) - 1];
    SDL_assert(ent_topop > TNECS_NULL);
    Menu *mc_topop = IES_GET_C(gl_world, ent_topop, Menu);

    if (fsm_eCncl_m[mc_topop->type] != NULL)
        fsm_eCncl_m[mc_topop->type](sota, mc_topop);

    if (DARR_NUM(sota->menus.stack) == 0) {
        Game_cursorFocus_onMap(sota);
    }
}

void fsm_eCncl_sGmpMap_ssMapUnitMv( Game *sota, 
                                    tnecs_E canceller) {
    /* --- Hide movemap, return unit to starting pos --- */
    Map *map = Game_Map(sota);
    map->arrow->show = false;

    /* Return unit to initial pos, deselect */
    if (sota->selected.unit_entity > TNECS_NULL) {
        tnecs_E *data2 = IES_calloc(1, sizeof(data2));
        *data2 = sota->selected.unit_entity;
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Icon_Return, NULL, NULL);
        Event_Emit( __func__, SDL_USEREVENT,
                    event_Unit_Deselect, 
                    NULL, data2);
    }

    /* Make Popup_Tile visible */
    tnecs_E popup_ent = sota->popups.arr[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup = IES_GET_C(gl_world, popup_ent, PopUp);
    SDL_assert(popup != NULL);
    popup->visible = true;

    /* Reset cursor lastpos to current pos */
    // prevents cursor dehovering from far away
    const struct Position *cursor_pos;
    cursor_pos = IES_GET_C(gl_world, sota->cursor.entity, Position);
    sota->cursor.lastpos.x = cursor_pos->tilemap_pos.x;
    sota->cursor.lastpos.y = cursor_pos->tilemap_pos.y;
}

void fsm_eCncl_sGmpMap_ssAnim(struct Game *sota, tnecs_E canceller) {
}

void fsm_eCncl_sGmpMap_ssMapCmbt(struct Game *sota, tnecs_E canceller) {
}

void fsm_eCncl_sGmpMap_ssMapNPC(struct Game *sota, tnecs_E canceller) {

}

// -- FSM: CURSOR_MOVES EVENT --
void fsm_eCrsMvs_sGmpMap(struct Game *sota, tnecs_E mover_entity,
                         struct Point *cursor_move) {
    if (fsm_eCrsMvs_sGmpMap_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eCrsMvs_sGmpMap_ss[Game_Substate_Current(sota)](sota, mover_entity, cursor_move);
}

void fsm_eCrsMvs_sPrep(struct Game *sota, tnecs_E mover_entity,
                       struct Point *cursor_move) {
    if (fsm_eCrsMvs_sPrep_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eCrsMvs_sPrep_ss[Game_Substate_Current(sota)](sota, mover_entity, cursor_move);
}

void fsm_eCrsMvs_sGmpMap_ssStby(struct Game *sota, tnecs_E mover_entity,
                                struct Point *cursor_move) {
    tnecs_E cursor = sota->cursor.entity;
    struct Position *cursor_pos = IES_GET_C(gl_world, cursor, Position);
    struct Slider   *cursor_sl  = IES_GET_C(gl_world, cursor, Slider);
    struct Sprite   *cursor_sp  = IES_GET_C(gl_world, cursor, Sprite);

    /* Actually move the cursor from cursor_move_data set by systemControl */
    // Note: always on tilemap
    Position_Pos_Add(cursor_pos, sota->cursor.move.x, sota->cursor.move.y);
    Cursor_Target(cursor_sl, cursor_sp, cursor_pos);
    sota->cursor.move.x = 0;
    sota->cursor.move.y = 0;

    /* -- Give tile to popup -- */
    tnecs_E ent = sota->popups.arr[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup = IES_GET_C(gl_world, ent, PopUp);
    struct PopUp_Tile *popup_tile = popup->data;
    PopUp_Tile_Set(popup_tile, sota);
}

void fsm_eCrsMvs_ssMenu(Game *sota, tnecs_E mover_entity,
                        Point *cursor_move) {
    /* Find menu elem in direction */
    tnecs_E menu = sota->menus.stack[DARR_NUM(sota->menus.stack) - 1];
    struct Menu *mc = IES_GET_C(gl_world, menu, Menu);

    /* menu elem move FSM to find next elem depending on menu type */
    SDL_assert(mc->type > MENU_TYPE_START);
    SDL_assert(mc->type < MENU_TYPE_END);
    SDL_assert(menu_elem_move[mc->type] != NULL);
    i32 new_elem = menu_elem_move[mc->type](mc,  sota->cursor.moved_direction);

    /* - TODO: MAKE FUNCTION - */
    /* - Move to cursor to new_elem - */
    SDL_assert(new_elem > MENU_ELEM_NULL);
    Menu_Elem_Set(mc, sota, new_elem);
    sota->cursor.move.x = 0;
    sota->cursor.move.y = 0;

    if (fsm_eCrsMvs_m[mc->type] != NULL)
        fsm_eCrsMvs_m[mc->type](sota, mc);
}

void fsm_eCrsMvs_sGmpMap_ssMapCndt(Game *sota, tnecs_E mover_entity,
                                   Point *cursor_move) {
    SDL_assert(sota->targets.patients != sota->targets.defendants);

    /* Find menu elem in direction */
    SDL_assert(sota->targets.candidates != NULL);
    int num = DARR_NUM(sota->targets.candidates);
    SDL_assert(num > 0);
    if ((sota->cursor.move.x > 0) || (sota->cursor.move.y > 0)) {
        sota->targets.order = (sota->targets.order + 1) % num;
    } else if ((sota->cursor.move.x < 0) || (sota->cursor.move.y < 0)) {
        sota->targets.order = (sota->targets.order - 1 + num) % num;
    }

    /* --- eCrsMvs_mo independent of parent menu --- */
    if (fsm_eCrsMvs_mo[sota->selected.menu_option] != NULL)
        fsm_eCrsMvs_mo[sota->selected.menu_option](sota, NULL);

    Game_Cursor_Move_toCandidate(sota);

    /* Reset cursor_move */
    sota->cursor.move.x = 0;
    sota->cursor.move.y = 0;

    /* Update pre_combat_popup */
    if (sota->targets.candidates == sota->targets.defendants) {
        Game_PopUp_Pre_Combat_Enable(sota);
    }
}

void fsm_eCrsMvs_sGmpMap_ssMapUnitMv(struct Game *sota,
                                     tnecs_E mover_entity,
                                     struct Point *cursor_move) {

    /* -- Move cursor -- */
    tnecs_E cursor         = sota->cursor.entity;
    struct Position *cursor_pos = IES_GET_C(gl_world, cursor, Position);
    struct Slider   *cursor_sl  = IES_GET_C(gl_world, cursor, Slider);
    struct Sprite   *cursor_sp  = IES_GET_C(gl_world, cursor, Sprite);

    /* Always on tilemap */
    Position_Pos_Add(cursor_pos, sota->cursor.move.x, sota->cursor.move.y);
    Cursor_Target(cursor_sl, cursor_sp, cursor_pos);
    sota->cursor.move.x = 0;
    sota->cursor.move.y = 0;

    /* -- give tile to popup -- */
    tnecs_E ent = sota->popups.arr[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup = IES_GET_C(gl_world, ent, PopUp);
    struct PopUp_Tile *popup_tile = popup->data;
    PopUp_Tile_Set(popup_tile, sota);
}

// -- FSM: CURSOR_MOVED EVENT --
void fsm_eCrsMvd_sGmpMap(struct Game *sota, tnecs_E mover_entity,
                         struct Point *cursor_move) {
    SDL_assert(sota->cursor.entity != TNECS_NULL);

    struct Position *cursor_pos;
    cursor_pos = IES_GET_C(gl_world, sota->cursor.entity, Position);
    struct Point *pos = &cursor_pos->tilemap_pos;

    if (fsm_eCrsMvd_sGmpMap_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eCrsMvd_sGmpMap_ss[Game_Substate_Current(sota)](sota, mover_entity, pos);

    Game_PopUp_Tile_Place(sota, *pos);
    if (sota->hovered.unit_entity == TNECS_NULL) {
        Game_PopUp_Unit_Place(sota, *pos);
    }
}

void fsm_eCrsMvd_sGmpMap_ssStby(struct Game *sota, tnecs_E mover_entity,
                                struct Point *cursor_move) {

    // SDL_assert(sota->cursor.moved_direction > -1);
    SDL_assert(sota->cursor.entity != TNECS_NULL);
    const struct Position *cursor_pos;
    cursor_pos = IES_GET_C(gl_world, sota->cursor.entity, Position);

    struct Point pos          = cursor_pos->tilemap_pos;
    struct Point previous_pos = sota->cursor.lastpos;
    Map *map = Game_Map(sota);
    int previous_i = previous_pos.y * Map_col_len(map) + previous_pos.x;
    int current_i  = pos.y * Map_col_len(map) + pos.x;
    tnecs_E unit_entity_previoustile = map->darrs.unitmap[previous_i];
    // NOTE: unit_entity_previoustile might be different than selected_unit_entity
    //     because
    tnecs_E ontile = map->darrs.unitmap[current_i];

    /* unit hovering/dehovering */
    if (unit_entity_previoustile != TNECS_NULL) {
        tnecs_E *data1 = IES_calloc(1, sizeof(data1));
        *data1 = unit_entity_previoustile;
        Event_Emit( __func__, SDL_USEREVENT,
                    event_Cursor_Dehovers_Unit, 
                    data1, NULL);
    }
    if (ontile != TNECS_NULL) {
        tnecs_E *data2 = IES_calloc(1, sizeof(data2));
        *data2 = ontile;
        Event_Emit( __func__, SDL_USEREVENT,
                    event_Cursor_Hovers_Unit, 
                    NULL, data2);
    }

    sota->cursor.lastpos.x = pos.x;
    sota->cursor.lastpos.y = pos.y;
}

void fsm_eCrsMvd_sGmpMap_ssMapCndt(struct Game *sota, tnecs_E mover_entity,
                                   struct Point *cursor_move) {

    Map *map = Game_Map(sota);
    const struct Position *cursor_pos;
    cursor_pos = IES_GET_C(gl_world, sota->cursor.entity, Position);
    struct Point pos = cursor_pos->tilemap_pos;
    // SDL_assert(sota->cursor.moved_direction > -1);
    struct Point previous_pos;

    struct Point moved = Ternary_Moved(sota->cursor.moved_direction);
    previous_pos.x = pos.x + moved.x;
    previous_pos.y = pos.y + moved.y;

    int previous_i = previous_pos.y * Map_col_len(map) + previous_pos.x;
    int current_i = pos.y * Map_col_len(map) + pos.x;
    tnecs_E unit_entity_previoustile = map->darrs.unitmap[previous_i];
    tnecs_E ontile = map->darrs.unitmap[current_i];

    /* unit hovering/dehovering */
    if (unit_entity_previoustile != TNECS_NULL) {
        tnecs_E *data1 = IES_calloc(1, sizeof(data1));
        *data1 = unit_entity_previoustile;
        Event_Emit( __func__, SDL_USEREVENT,
                    event_Cursor_Dehovers_Unit,
                    data1, NULL);
    }
    if (ontile != TNECS_NULL) {
        tnecs_E *data2 = IES_calloc(1, sizeof(data2));
        *data2 = ontile;
        Event_Emit( __func__, SDL_USEREVENT,
                    event_Cursor_Hovers_Unit,
                    NULL, data2);
    }
}

void fsm_eCrsMvs_sPrep_ssMapCndt(struct Game  *sota, tnecs_E mover_entity,
                                 struct Point *nope) {
    /* --- Move cursor to next starting position on map --- */
    // TODO: stop cursor moving so fast
    Map *map = Game_Map(sota);

    tnecs_E cursor = sota->cursor.entity;
    struct Position *cursor_pos = IES_GET_C(gl_world, cursor, Position);
    struct Slider   *cursor_sl  = IES_GET_C(gl_world, cursor, Slider);
    struct Sprite   *cursor_sp  = IES_GET_C(gl_world, cursor, Sprite);

    /* Actually move the cursor from cursor_move_data set by systemControl */
    // Note: always on tilemap
    int num_pos = DARR_NUM(map->start_pos.arr);
    if ((sota->cursor.move.x > 0) || (sota->cursor.move.y > 0)) {
        sota->targets.order = sota->targets.order >= (num_pos - 1) ? 0 : sota->targets.order + 1;
    } else if ((sota->cursor.move.x < 0) || (sota->cursor.move.y < 0)) {
        sota->targets.order = sota->targets.order <= 0 ? num_pos - 1 : sota->targets.order - 1;
    }
    struct Menu *mc;
    mc = IES_GET_C(gl_world, sota->menus.deployment, Menu);
    struct DeploymentMenu *dm = mc->data;
    SDL_assert(dm != NULL);
    i32 start_pos_i = DeploymentMenu_Map_StartPos(dm, sota->targets.order);
    struct Point next_pos = map->start_pos.arr[start_pos_i];
    Position_Pos_Set(cursor_pos, next_pos.x, next_pos.y);

    // Always on tilemap
    Cursor_Target(cursor_sl, cursor_sp, cursor_pos);
    sota->cursor.move.x = 0;
    sota->cursor.move.y = 0;
}

void fsm_eCrsMvd_sGmpMap_ssMapUnitMv(struct Game *sota, tnecs_E mover_entity,
                                     struct Point *cursor_pos) {
    Map *map = Game_Map(sota);

    /* -- Unit follows cursor movement  -- */
    struct Position *selected_pos;
    selected_pos = IES_GET_C(gl_world, sota->selected.unit_entity, Position);
    selected_pos->tilemap_pos.x = cursor_pos->x;
    selected_pos->tilemap_pos.y = cursor_pos->y;
    selected_pos->pixel_pos.x = selected_pos->tilemap_pos.x * selected_pos->scale[0];
    selected_pos->pixel_pos.y = selected_pos->tilemap_pos.y * selected_pos->scale[1];

    /* -- Move arrow -- */
    struct Arrow *arrow = map->arrow;

    Arrow_Path_Add(arrow, map->size, cursor_pos->x, cursor_pos->y);

    /* -- Update map_unit loop to follow arrow direction -- */
    struct Sprite *sprite;
    sprite = IES_GET_C(gl_world, sota->selected.unit_entity, Sprite);
    int path_num = DARR_NUM(arrow->path) / TWO_D;

    if ((path_num > 0) && (sprite->spritesheet != NULL)) {
        i32 previous_x = arrow->path[(path_num - 2) * TWO_D];
        i32 previous_y = arrow->path[((path_num - 2) * TWO_D) + 1];
        if (sprite->spritesheet->loop_num == MAP_UNIT_LOOP_NUM) {
            struct Point move = {cursor_pos->x - previous_x, cursor_pos->y - previous_y};
            int direction = Ternary_Direction(move);
            int loop      = Utilities_Loop(direction, sprite->flip);
            Spritesheet_Loop_Set(sprite->spritesheet, loop, sprite->flip);
            Sprite_Dstrect_Relative(sprite, &selected_pos->pixel_pos, &map->render.camera);
            Sprite_Animation_Loop(sprite);
        }
    }

}

// -- FSM: Gameplay_Return2Standby EVENT --
void fsm_eGmp2Stby_ssMapUnitMv(struct Game *sota, tnecs_E ent) {

    if (sota->selected.unit_entity > 0)
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Icon_Return, NULL, NULL);

}

void fsm_eGmp2Stby_sGmpMap(struct Game *sota, tnecs_E controller_entity) {
    if (sota->selected.unit_entity > TNECS_NULL) {
        tnecs_E *data1 = IES_calloc(1, sizeof(data1));
        *data1 = controller_entity;

        tnecs_E *data2 = IES_calloc(1, sizeof(data2));
        *data2 = sota->selected.unit_entity;
        if (sota->selected.unit_entity > 0)
            Event_Emit( __func__, SDL_USEREVENT,
                        event_Unit_Deselect,
                        data1, data2);
    }

    Map *map = Game_Map(sota);
    map->flags.show_overlay = false;
}

/* -- FSM: Input_Start EVENT -- */
void fsm_eStart_sScnTalk(struct Game *sota, tnecs_E accepter) {
    /* --- Skip scene --- */
    SDL_assert(sota->narrative.scene > TNECS_NULL);
    struct Scene *scene = IES_GET_C(gl_world, sota->narrative.scene, Scene);
    Scene_Finish(scene, sota);
}

void fsm_eStart_sCutScn(struct Game *sota, tnecs_E nope) {
    /* --- Finish Cutscene --- */
    SDL_assert(sota->narrative.cutscene > TNECS_NULL);
    struct Cutscene *cutscene = IES_GET_C(gl_world, sota->narrative.cutscene, Cutscene);
    Cutscene_Finish(cutscene, sota);
}

void fsm_eStart_sPrep(struct Game *sota, tnecs_E accepter) {
    /* --- Preparation done: Start Map gameplay --- */
    // TODO: check that in prep stat, menu is deployment
    if (fsm_eStart_sPrep_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eStart_sPrep_ss[Game_Substate_Current(sota)](sota, accepter);
}

void fsm_eStart_sPrep_ssMapCndt(struct Game *sota, tnecs_E ent) {
    /* --- Start battle --- */
    SDL_assert(sota->menus.deployment > TNECS_NULL);
    struct Menu *mc = IES_GET_C(gl_world, sota->menus.deployment, Menu);
    Game_Battle_Start(sota, mc);
    SDL_assert(mc != NULL);
}

void fsm_eStart_sPrep_ssMenu(struct Game *sota, tnecs_E ent) {
    /* --- Preparation done: Start Map gameplay --- */
    SDL_assert(DARR_NUM(sota->menus.stack) > 0);
    tnecs_E top_menu = sota->menus.stack[DARR_NUM(sota->menus.stack) - 1];
    SDL_assert(top_menu > TNECS_NULL);
    SDL_assert(top_menu == sota->menus.deployment);
    struct Menu *mc = IES_GET_C(gl_world, top_menu, Menu);
    SDL_assert(mc != NULL);

    /* TODO: Are you sure menu */
    /* --- Start battle --- */
    Game_Battle_Start(sota, mc);
}

/* -- FSM: Input_Accept EVENT -- */
void fsm_eAcpt_sScnTalk(struct Game *sota, tnecs_E nope) {
    /* --- Get next line at normal speed --- */
    SDL_assert(sota->narrative.scene > TNECS_NULL);
    struct Scene *scene = IES_GET_C(gl_world, sota->narrative.scene, Scene);
    if (Scene_Statement_Next(scene) < 0) {
        Scene_Finish(scene, sota);
    }
}

void fsm_eAcpt_sCutScn(struct Game *sota, tnecs_E nope) {
    /* --- Finish cutscene --- */
    SDL_assert(sota->narrative.cutscene > TNECS_NULL);
    struct Cutscene *cutscene = IES_GET_C(gl_world, sota->narrative.cutscene, Cutscene);
    Cutscene_Finish(cutscene, sota);
}

void fsm_eAcpt_sGmpMap(struct Game *sota, tnecs_E accepter) {
    if (fsm_eAcpt_sGmpMap_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eAcpt_sGmpMap_ss[Game_Substate_Current(sota)](sota, accepter);
}

void fsm_eAcpt_sPrep(struct Game *sota, tnecs_E accepter) {
    if (fsm_eAcpt_sPrep_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eAcpt_sPrep_ss[Game_Substate_Current(sota)](sota, accepter);
}

void fsm_eAcpt_sTtlScrn(struct Game *sota, tnecs_E accepter) {
    if (Game_Substate_Current(sota) != GAME_SUBSTATE_MENU) {
        SDL_Log("Wrong substate %d on Title_Screen state", Game_Substate_Current(sota));
        exit(ERROR_Generic);
    }
    /* TODO: fsm_eAcpt_sTtlScrn_ssMenu */
    fsm_eAcpt_sTtlScrn_ssMenu(sota, accepter);
}

/* -- Input_Accept -- */
void fsm_eAcpt_sGmpMap_ssMapCndt(Game *sota, tnecs_E canceller) {
    /* Selecting candidate for action on the on map.
    **  1. Action was previously decided using menu_option
    **      All actions with candidates through menu options */
    /* --- eAcpt_mo independent of parent menu --- */
    if (fsm_eAcpt_mo[sota->selected.menu_option] != NULL)
        fsm_eAcpt_mo[sota->selected.menu_option](sota, NULL);
}

void fsm_eAcpt_sGmpMap_ssStby(Game *sota, tnecs_E accepter) {
    SDL_assert(sota->cursor.entity);
    const struct Position *cursor_pos;
    cursor_pos = IES_GET_C( gl_world,
                            sota->cursor.entity, Position);
    SDL_assert(cursor_pos != NULL);
    Map *map = Game_Map(sota);
    struct Point pos = cursor_pos->tilemap_pos;
    tnecs_E ontile = map->darrs.unitmap[pos.y * Map_col_len(map) + pos.x];
    if (ontile != TNECS_NULL) {
        /* -- select unit -- */
        tnecs_E *data2 = IES_calloc(1, sizeof(data2));
        *data2 = ontile;
        Event_Emit( __func__, SDL_USEREVENT,
                    event_Unit_Select,
                    NULL, data2);
    } else {
        /* -- draw map_action menu -- */
        if (sota->menus.map_action == TNECS_NULL) {
            Game_MapActionMenu_Create(sota);
        }
        SDL_assert(sota->menus.map_action > TNECS_NULL);
        Game_MapActionMenu_Update(sota);
        Game_menuStack_Push(sota, sota->menus.map_action);

        Game_cursorFocus_onMenu(sota);
        Game_subState_Set(  sota, GAME_SUBSTATE_MENU,
                            "no unit was selected");
        Event_Emit(__func__, SDL_USEREVENT,
                   event_Menu_Created,
                   &sota->menus.map_action, NULL);
    }
}

void fsm_eAcpt_sPrep_ssMapCndt(struct Game *sota, tnecs_E accepter_entity) {
    /* Select a unit on starting position, or move it */
    SDL_assert(sota->menus.deployment > TNECS_NULL);
    struct Menu *mc;
    mc = IES_GET_C(gl_world, sota->menus.deployment, Menu);
    SDL_assert(mc != NULL);
    struct DeploymentMenu *dm = mc->data;
    SDL_assert(dm != NULL);

    if (dm->start_pos_order1 >= 0) {
        /* Unit was selected previously, exchange with currently selected tile */
        DeploymentMenu_Map_StartPos_Select(dm, sota->targets.order);
        SDL_assert(dm->start_pos_order1 >= 0);
        SDL_assert(dm->start_pos_order2 >= 0);
        if (dm->start_pos_order1 == dm->start_pos_order2) {
            dm->start_pos_order1 = -1;
            dm->start_pos_order2 = -1;
            return;
        }
        Map *map = Game_Map(sota);
        struct Point pos1 = map->start_pos.arr[dm->start_pos_order1];
        struct Point pos2 = map->start_pos.arr[dm->start_pos_order2];

        // tnecs_E old_ent = Map_Unit_Get(map, pos1.x, pos1.y);
        // tnecs_E new_ent = Map_Unit_Get(map, pos2.x, pos2.y);

        // size_t index1 = pos1.y * Map_col_len(map) + pos1.x;
        // size_t index2 = pos2.y * Map_col_len(map) + pos2.x;
        Map_Unit_Swap(map, pos1.x, pos1.y, pos2.x, pos2.y);
        i32 pos1_i = DeploymentMenu_Map_Find_Pos(dm, map, pos1.x, pos1.y);
        i32 pos2_i = DeploymentMenu_Map_Find_Pos(dm, map, pos2.x, pos2.y);

        if (pos1_i != pos2_i) {
            DeploymentMenu_Map_Swap(dm);
        } else {
            DeploymentMenu_Map_StartPos_Deselect(dm);
        }
    } else {
        /* No unit was selected previously, selecting now */
        DeploymentMenu_Map_StartPos_Select(dm, sota->targets.order);
    }
}

void fsm_eAcpt_sTtlScrn_ssMenu(Game *sota, tnecs_E accepter_entity) {
    /* Possible state pairs: [?, MENU] */

    SDL_assert(DARR_NUM(sota->menus.stack) > 0);
    tnecs_E top_menu = sota->menus.stack[DARR_NUM(sota->menus.stack) - 1];
    SDL_assert(top_menu > TNECS_NULL);
    struct Menu *mc_topop = IES_GET_C(gl_world, top_menu, Menu);

    if (fsm_eAcpt_m[mc_topop->type] != NULL)
        fsm_eAcpt_m[mc_topop->type](sota, mc_topop);

    Event_Emit( __func__, SDL_USEREVENT,
                event_Menu_Select,
                NULL, NULL);
}

void fsm_eAcpt_sGmpMap_ssMenu(Game *sota, tnecs_E accepter_entity) {
    /* Possible state pairs: [?, MENU] */

    SDL_assert(DARR_NUM(sota->menus.stack) > 0);
    tnecs_E top_menu = sota->menus.stack[DARR_NUM(sota->menus.stack) - 1];
    SDL_assert(top_menu > TNECS_NULL);
    struct Menu *mc_topop = IES_GET_C(gl_world, top_menu, Menu);

    if (fsm_eAcpt_m[mc_topop->type] != NULL)
        fsm_eAcpt_m[mc_topop->type](sota, mc_topop);

    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Select, NULL, NULL);
}

void fsm_eAcpt_sGmpMap_ssMapUnitMv(struct Game *sota, tnecs_E accepter_entity) {
    /* - Unit should have been selected - */
    SDL_assert(sota->selected.unit_entity != TNECS_NULL);
    /* - Skip if friendly on tile - */
    const struct Position *cursor_pos;
    cursor_pos          = IES_GET_C(gl_world, sota->cursor.entity, Position);
    Map *map = Game_Map(sota);
    int current_i       = cursor_pos->tilemap_pos.y * Map_col_len(map) +
                          cursor_pos->tilemap_pos.x;
    tnecs_E ontile = map->darrs.unitmap[current_i];

    /* - Someone else already occupies tile -> Do nothing - */
    if ((ontile != TNECS_NULL) && (sota->selected.unit_entity != ontile)) {
        // TODO: ontile should be a friendly unit.
        // SDL_assert();
        return;
    }

    tnecs_E unit_ent = sota->selected.unit_entity;

    /* - Reset potential candidates - */
    sota->targets.order         = 0;
    sota->targets.candidates    = NULL;

    /* - Make popup_tile invisible - */
    tnecs_E popup_ent = sota->popups.arr[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup      = IES_GET_C(gl_world, popup_ent, PopUp);
    if (popup != NULL) {
        popup->visible = false;
    }

    /* - Get selected unit on tile - */
    struct Position *selected_pos;
    Unit *unit   = IES_GET_C(gl_world, unit_ent, Unit);
    selected_pos = IES_GET_C(gl_world, unit_ent, Position);

    /* - Unit should be PC - */
    SDL_assert(SotA_isPC(Unit_Army(unit)));

    /* -- Creating Unit Action Menu -- */
    if (sota->menus.unit_action == TNECS_NULL) {
        Game_UnitActionMenu_Create(sota);
    }
    Game_menuStack_Push(sota, sota->menus.unit_action);

    /* - Send event_Menu_Created Event to set substate - */
    Event_Emit( __func__, SDL_USEREVENT,
                event_Menu_Created,
                &sota->menus.unit_action, NULL);
    sota->cursor.lastpos.x = cursor_pos->tilemap_pos.x;
    sota->cursor.lastpos.y = cursor_pos->tilemap_pos.y;

    /* - Moving unit to new tile - */
    sota->selected.unit_moved_position.x    = cursor_pos->tilemap_pos.x;
    sota->selected.unit_moved_position.y    = cursor_pos->tilemap_pos.y;
    Point initial   = sota->selected.unit_initial_position;
    Point moved     = sota->selected.unit_moved_position;
    if ((initial.x != moved.x) ||
        (initial.y != moved.y))
        Map_Unit_Move(  map,
                        initial.x,  initial.y,
                        moved.x,    moved.y);

    Position_Pos_Set(selected_pos, moved.x, moved.y);

    /* - Compute new stackmap with recomputed attacktomap - */
    /* - MapAct settings for attacktolist - */
    MapAct map_to = MapAct_default;

    /* Don't show movemap if AI never moves */
    map_to.move         = false;
    map_to.archetype    = ITEM_ARCHETYPE_STAFF;
    map_to.eq_type      = LOADOUT_EQUIPMENT;
    map_to.output_type  = ARRAY_MATRIX;
    map_to.aggressor    = unit_ent;

    /* - healtopmap - */
    Map_Act_To(map, map_to);

    /* - attacktomap - */
    map_to.archetype     = ITEM_ARCHETYPE_WEAPON;
    Map_Act_To(map, map_to);

    // SDL_Log("ATK");
    // matrix_print(map->darrs.attacktomap, Map_row_len(map), Map_col_len(map));
    // SDL_Log("HEAL");
    // matrix_print(map->darrs.healtomap, Map_row_len(map), Map_col_len(map));

    int rangemap = Unit_Rangemap_Get(unit);
    if (rangemap == RANGEMAP_HEALMAP) {
        Map_Palettemap_Autoset(map, MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL,
                               unit_ent);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        Map_Palettemap_Autoset(map,
                               MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK, TNECS_NULL);
    }
    Map_Stacked_Dangermap_Compute(map, map->darrs.dangermap);

    /* - Update Standard passive aura for all units - */
    SDL_assert(map->armies.current >= 0);
    SDL_assert(map->armies.current < DARR_NUM(map->armies.onfield));
    i32 army = map->armies.onfield[map->armies.current];
    SDL_assert(army == Unit_Army(unit)); /* only units in current army should be moving */
    Map_Bonus_Remove_Instant(map, Unit_Army(unit));
    Map_Bonus_Standard_Apply(map, Unit_Army(unit));

    /* - Pre-menu update computation for content - */
    Game_preUnitAction_Targets(sota, unit_ent);

    /* - Update menu to create content - */
    Game_UnitActionMenu_Update(sota, unit_ent);

    /* - Focusing cursor on Menu - */
    Game_cursorFocus_onMenu(sota);
    Menu *mc = IES_GET_C(   gl_world,
                            sota->menus.unit_action, Menu);
    SDL_assert(mc->visible == true);
}

void fsm_eAcpt_sGmpMap_ssMapMini(struct Game *sota, tnecs_E accepter_entity) {
}

void fsm_eAcpt_sGmpMap_ssAnim(struct Game *sota, tnecs_E accepter_entity) {
}

void fsm_eAcpt_sGmpMap_ssMapCmbt(struct Game *sota, tnecs_E accepter_entity) {
}

void fsm_eAcpt_sGmpMap_ssMapNPC(struct Game *sota, tnecs_E accepter_entity) {
}

/* Input_Stats */
void fsm_eStats_sPrep(  struct Game *sota, tnecs_E ent) {
    if (fsm_eStats_sPrep_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eStats_sPrep_ss[Game_Substate_Current(sota)](sota, TNECS_NULL);
}

void fsm_eStats_sGmpMap(struct Game *sota, tnecs_E ent) {
    fsm_eStats_sGmpMap_ssStby(sota, ent);
}

void fsm_eStats_sPrep_ssMapCndt(struct Game *sota, tnecs_E ent) {
    /* Find which unit is hovered on map */
    SDL_assert(sota->cursor.entity);
    Map *map = Game_Map(sota);
    struct Position *cursor_pos = IES_GET_C(gl_world, sota->cursor.entity, Position);
    SDL_assert(cursor_pos != NULL);
    struct Point pos = cursor_pos->tilemap_pos;
    tnecs_E ontile = map->darrs.unitmap[pos.y * Map_col_len(map) + pos.x];

    /* Enabling stats menu for hovered unit */
    if (ontile > TNECS_NULL) {
        Game_StatsMenu_Enable(sota, ontile);
    }
}

void fsm_eStats_sPrep_ssMenu(  struct Game *sota, tnecs_E ent) {
    SDL_assert(sota->menus.deployment > TNECS_NULL);

    struct Menu *mc;
    mc = IES_GET_C(gl_world, sota->menus.deployment, Menu);

    if (fsm_eStats_m[mc->type] != NULL)
        fsm_eStats_m[mc->type](sota, mc);
}

/* Displaying stats menu */
void fsm_eStats_sGmpMap_ssStby(struct Game *sota, tnecs_E accepter) {
    Map *map = Game_Map(sota);
    SDL_assert((Game_State_Current(sota) == GAME_STATE_Gameplay_Map) ||
               (Game_State_Current(sota) == GAME_STATE_Preparation));

    /* Find which unit is hovered on map */
    SDL_assert(sota->cursor.entity);
    struct Position *cursor_pos = IES_GET_C(gl_world, sota->cursor.entity, Position);
    SDL_assert(cursor_pos != NULL);
    struct Point pos = cursor_pos->tilemap_pos;
    tnecs_E ontile = map->darrs.unitmap[pos.y * Map_col_len(map) + pos.x];

    /* Enabling stats menu for hovered unit */
    if (ontile > TNECS_NULL)
        Game_StatsMenu_Enable(sota, ontile);
    else {
        /* -- TODO: draw a map marker here, like tear ring saga -- */
        // SDL_Log("NO UNIT ON TILE");
    }

    /* - Make popup_tile invisible - */
    tnecs_E popup_ent = sota->popups.arr[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup    = IES_GET_C(gl_world, popup_ent, PopUp);
    if (popup != NULL)
        popup->visible = false;

    /* - Make popup_unit invisible - */
    popup_ent   = sota->popups.arr[POPUP_TYPE_HUD_UNIT];
    popup       = IES_GET_C(gl_world, popup_ent, PopUp);
    if (popup != NULL)
        popup->visible = false;
}

// -- FSM: UNIT_SELECT EVENT --
void fsm_eUnitSel_ssStby(struct Game *sota, tnecs_E selector_entity) {
    struct Unit *selected_unit;
    selected_unit = IES_GET_C(gl_world, sota->selected.unit_entity, Unit);
    struct Position *selected_pos;
    selected_pos = IES_GET_C(gl_world, sota->selected.unit_entity, Position);
    sota->selected.unit_initial_position.x = selected_pos->tilemap_pos.x;
    sota->selected.unit_initial_position.y = selected_pos->tilemap_pos.y;

    if (!SotA_isPC(Unit_Army(selected_unit))) {
        /* - Enemy unit was selected - */
        Event_Emit( __func__, SDL_USEREVENT,
                    event_Unit_Danger, 
                    NULL, NULL);
        return;
    }

    /* - Friendly unit was selected - */
    if (!Unit_isWaiting(selected_unit)) {
        /* - Friendly unit can move - */
        sota->combat.aggressor             = sota->selected.unit_entity;

        /* Make popup_unit invisible */
        // TODO: GO OFFSCREEN
        tnecs_E popup_ent = sota->popups.arr[POPUP_TYPE_HUD_UNIT];
        struct PopUp *popup = IES_GET_C(gl_world, popup_ent, PopUp);
        SDL_assert(popup != NULL);
        // popup->visible = false;
        Event_Emit( __func__, SDL_USEREVENT,
                    event_Unit_Moves,
                    NULL, NULL);
    }
}

// -- FSM: UNIT_DESELECT EVENT --
void fsm_eUnitDsel_ssMenu(struct Game *sota, tnecs_E selector) {
    Game_subState_Set(
            sota, GAME_SUBSTATE_STANDBY,
            "Unit was deselected during menu (unit waits?)"
    );
}

void fsm_eUnitDsel_ssMapUnitMv(struct Game *sota, tnecs_E selector) {
    /*  -- Hide arrow -- */
    Map *map = Game_Map(sota);
    map->arrow->show = false;

    /*  -- Reset map overlay to danger only -- */
    // struct Position *selected_pos;
    // selected_pos = IES_GET_C(gl_world, sota->selected.unit_entity, Position);
    struct Position *pos = IES_GET_C(gl_world, selector, Position);
    struct Point initial = sota->selected.unit_initial_position;
    if ((pos->tilemap_pos.x != initial.x) || (pos->tilemap_pos.y != initial.y)) {
        // Only if cursor not on unit.
        // If cursor is on unit, movemap and attackmap should be shown
        Map_Palettemap_Reset(map);
        Map_Palettemap_Autoset(map, MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_DANGER, TNECS_NULL);
        Map_Stacked_Dangermap_Reset(map);
    }

    Game_subState_Set(
            sota, GAME_SUBSTATE_STANDBY,
            "Unit was deselected during movement"
    );
}

// -- FSM: INPUT_MENURIGHT EVENT --
void fsm_eMenuRight_sGmpMap(struct Game *sota, i32 controller_type) {
    if (fsm_eMenuRight_sGmpMap_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eMenuRight_sGmpMap_ss[Game_Substate_Current(sota)](sota, controller_type);
}

void fsm_eMenuRight_sScnTalk(struct Game *sota, i32 controller_type) {
    fsm_eAcpt_sScnTalk(sota, TNECS_NULL);
}

void fsm_eMenuRight_sGmpMap_ssMenu(struct Game *sota, i32 controller_type) {
    /* -- Pop previous menu -- */
    b32 destroy = false;
    tnecs_E popped = Game_menuStack_Pop(sota, destroy);
    SDL_assert(popped > 0);

    /* - Hide previous menu - */
    struct Menu *mc_popped = IES_GET_C(gl_world, popped, Menu);
    mc_popped->visible = false;

    /* -- Create New menu -- */
    /* - Get unit ontile - */
    Map *map = Game_Map(sota);
    struct Position *cursor_pos;
    cursor_pos = IES_GET_C(gl_world, sota->cursor.entity, Position);
    struct Point pos = cursor_pos->tilemap_pos;
    tnecs_E ontile = map->darrs.unitmap[pos.y * Map_col_len(map) + pos.x];

    /* - Determine which menu is the next one - */
    SDL_assert((mc_popped->type == MENU_TYPE_STATS) || (mc_popped->type == MENU_TYPE_GROWTHS));
    i32 current_id = stats_menu_cycle_inv[mc_popped->type];
    i32 new_id = (current_id == (STATS_MENU_CYCLE_NUM - 1)) ? 0 : ++current_id;
    struct Menu *new_menu_comp;
    switch (stats_menu_cycle[new_id]) {
        case MENU_TYPE_STATS:
            SDL_Log("NEW MENU_TYPE_STATS");
            Game_StatsMenu_Enable(sota, ontile);
            new_menu_comp = IES_GET_C(gl_world, sota->menus.stats, Menu);
            new_menu_comp->visible = true;
            break;
        case MENU_TYPE_GROWTHS:
            SDL_Log("NEW MENU_TYPE_GROWTHS");
            Game_GrowthsMenu_Enable(sota, ontile);
            new_menu_comp = IES_GET_C(gl_world, sota->menus.growths, Menu);
            new_menu_comp->visible = true;
            break;
        default:
            SDL_assert(false);
            break;
    }
}

void fsm_eMenuRight_sGmpMap_ssStby(struct Game *sota, i32 controller_type) {
}

// -- FSM: INPUT_MENULEFT EVENT --
void fsm_eMenuLeft_sGmpMap(struct Game *sota, i32 controller_type) {
    if (fsm_eMenuLeft_sGmpMap_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eMenuLeft_sGmpMap_ss[Game_Substate_Current(sota)](sota, controller_type);
}

void fsm_eMenuLeft_sPrep(struct Game *sota, i32 controller_type) {
    if (fsm_eMenuLeft_sPrep_ss[Game_Substate_Current(sota)] != NULL)
        fsm_eMenuLeft_sPrep_ss[Game_Substate_Current(sota)](sota);
}

void fsm_eMenuLeft_sGmpMap_ssMenu(struct Game *sota, i32 controller_type) {
    /* -- Pop previous menu -- */
    b32 destroy = false;
    tnecs_E popped = Game_menuStack_Pop(sota, destroy);
    SDL_assert(popped > 0);

    /* - Hide previous menu - */
    struct Menu *mc_popped = IES_GET_C(gl_world, popped, Menu);
    mc_popped->visible = false;

    /* -- Create New menu -- */
    /* - Get unit ontile - */
    struct Position *cursor_pos;
    cursor_pos = IES_GET_C(gl_world, sota->cursor.entity, Position);
    Map *map = Game_Map(sota);
    struct Point pos = cursor_pos->tilemap_pos;
    tnecs_E ontile = map->darrs.unitmap[pos.y * Map_col_len(map) + pos.x];

    /* - Determine which menu is the next one - */
    SDL_assert((mc_popped->type == MENU_TYPE_STATS)         ||
               (mc_popped->type == MENU_TYPE_GROWTHS)       ||
               (mc_popped->type == MENU_TYPE_DEPLOYMENT));
    i32 current_id = stats_menu_cycle_inv[mc_popped->type];
    i32 new_id = (current_id <= 0) ? (STATS_MENU_CYCLE_NUM - 1) : --current_id;
    struct Menu *new_menu_comp;
    switch (stats_menu_cycle[new_id]) {
        case MENU_TYPE_STATS:
            Game_StatsMenu_Enable(sota, ontile);
            new_menu_comp = IES_GET_C(gl_world, sota->menus.stats, Menu);
            new_menu_comp->visible = true;
            break;
        case MENU_TYPE_GROWTHS:
            Game_GrowthsMenu_Enable(sota, ontile);
            new_menu_comp = IES_GET_C(gl_world, sota->menus.growths, Menu);
            new_menu_comp->visible = true;
            break;
        default:
            SDL_assert(false);
            break;
    }
}

void fsm_eMenuLeft_sGmpMap_ssStby(struct Game *sota, i32 controller_type) {

}

void fsm_eMenuLeft_sPrep_ssMenu(struct Game *sota) {
    /* --- Deployment Menu -> Starting Position --- */
    SDL_assert(sota->menus.deployment > TNECS_NULL);
    SDL_assert(sota->menus.stack[0] == sota->menus.deployment);
    SDL_assert(DARR_NUM(sota->menus.stack) == 1);
    DARR_POP(sota->menus.stack);

    Game_subState_Set(
            sota,  GAME_SUBSTATE_MAP_CANDIDATES,
            "Change to map candidates"
    );

    /* - Reset potential candidates - */
    sota->targets.order     = 0;

    /* - Focus on map - */
    struct Menu *mc;
    mc = IES_GET_C(gl_world, sota->menus.deployment, Menu);
    mc->visible = false;

    Game_cursorFocus_onMap(sota);
    /* - Set candidates starting positions to dm - */
    SDL_assert(Game_State_Current(sota) == GAME_STATE_Preparation);
}

void fsm_eMenuLeft_sPrep_ssMapCndt(struct Game *sota) {
    /* --- Starting Position -> Deployment Menu --- */
    SDL_assert(sota->menus.deployment > TNECS_NULL);
    SDL_assert(DARR_NUM(sota->menus.stack) == 0);

    Game_subState_Set(
            sota, GAME_SUBSTATE_MENU,
            "Change to menu"
    );

    /* - Focus on menu - */
    DARR_PUT(sota->menus.stack, sota->menus.deployment);

    struct Menu *mc;
    mc = IES_GET_C(gl_world, sota->menus.deployment, Menu);
    mc->visible = true;
    Game_cursorFocus_onMenu(sota);

    struct DeploymentMenu *dm = mc->data;
    DeploymentMenu_UnitOrder_Reset(dm);

    /* - Place cursor on dm->elem == map candidate - */
    SDL_assert(Game_State_Current(sota) == GAME_STATE_Preparation);
}

void fsm_eCmbtEnd_ssMapNPC(  struct Game *sota) {
    /* Make all popups invisible */

    /* --- Invisible Popups --- */
    /* -- Popup_Loadout_stats -- */
    tnecs_E popup_ent = sota->popups.arr[POPUP_TYPE_HUD_LOADOUT_STATS];
    if (popup_ent > TNECS_NULL) {
        struct PopUp *popup_ptr = IES_GET_C(gl_world, popup_ent, PopUp);
        SDL_assert(popup_ptr != NULL);
        popup_ptr->visible = false;
    }

    /* -- Setting game substate -- */
    Game_subState_Set(
            sota, GAME_SUBSTATE_MAP_NPCTURN,
            "Control returns to AI"
    );
}

void fsm_eCmbtEnd_ssMapCndt( struct Game *sota) {
    // Return to standby substate -> 
    //      ONLY IF ON FRIENDLY TURN
    Event_Emit( __func__, SDL_USEREVENT,
                event_Gameplay_Return2Standby, 
                NULL, NULL);
}
