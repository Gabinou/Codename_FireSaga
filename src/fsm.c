
#include "fsm.h"
#include "scene.h"
#include "events.h"
#include "pathfinding.h"
#include "slider.h"
#include "position.h"
#include "octant.h"
#include "utilities.h"
#include "arrow.h"
#include "nmath.h"
#include "popup/popup.h"
#include "popup/unit.h"
#include "popup/tile.h"
#include "game/popup.h"
#include "game/game.h"
#include "game/unit.h"
#include "game/cursor.h"
#include "game/menu.h"
#include "game/popup.h"
#include "menu/fsm.h"
#include "menu/menu.h"
#include "menu/deployment.h"
#include "menu/staff_select.h"
#include "menu/stats.h"
#include "map/render.h"
#include "map/path.h"
#include "map/ontile.h"
#include "systems/slide.h"
#include "unit/unit.h"
#include "unit/equipment.h"
#include "unit/range.h"
#include "AI.h"
#include "sprite.h"
#include "cutscene.h"

/* --- ACTIONS --- */
const fsm_eGmp2Stby_s_t fsm_eGmp2Stby_s[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     NULL,
    /* Cutscene */       NULL,
    /* Gameplay_Map */   &fsm_eGmp2Stby_sGmpMap,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    NULL,
    /* Title_Screen */   NULL,
    /* Animation */      NULL,
};

/* --- Input events --- */
const fsm_eAcpt_s_t fsm_eAcpt_s[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     &fsm_eAcpt_sScnTalk,
    /* Cutscene */       &fsm_eAcpt_sCutScn,
    /* Gameplay_Map */   &fsm_eAcpt_sGmpMap,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    &fsm_eAcpt_sPrep,
    /* Title_Screen */   &fsm_eAcpt_sTtlScrn,
    /* Animation */      NULL,
};

const fsm_eStats_s_t fsm_eStats_s[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     NULL,
    /* Cutscene */       NULL,
    /* Gameplay_Map */   &fsm_eStats_sGmpMap,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    &fsm_eStats_sPrep,
    /* Title_Screen */   NULL,
    /* Animation */      NULL,
};

const fsm_eStart_s_t fsm_eStart_s[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     &fsm_eStart_sScnTalk,
    /* Cutscene */       &fsm_eStart_sCutScn,
    /* Gameplay_Map */   NULL,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    &fsm_eStart_sPrep,
    /* Title_Screen */   NULL,
    /* Animation */      NULL
};

const fsm_eCncl_s_t fsm_eCncl_s[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     &fsm_eAcpt_sScnTalk,
    /* Cutscene */       NULL,
    /* Gameplay_Map */   &fsm_eCncl_sGmpMap,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    &fsm_eCncl_sPrep,
    /* Title_Screen */   NULL,
    /* Animation */      NULL,
};

const fsm_eMenuRight_s_t fsm_eMenuRight_s[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     &fsm_eMenuRight_sScnTalk,
    /* Cutscene */      NULL,
    /* Gameplay_Map */   &fsm_eMenuRight_sGmpMap,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    NULL,
    /* Title_Screen */   NULL,
    /* Animation */      NULL,
};

const fsm_eMenuLeft_s_t fsm_eMenuLeft_s[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     &fsm_eMenuRight_sScnTalk,
    /* Cutscene */      NULL,
    /* Gameplay_Map */   &fsm_eMenuLeft_sGmpMap,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    &fsm_eMenuLeft_sPrep,
    /* Title_Screen */   NULL,
    /* Animation */      NULL,
};

const fsm_eCrsMvs_s_t fsm_eCrsMvs_s[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     NULL,
    /* Cutscene */       NULL,
    /* Gameplay_Map */   &fsm_eCrsMvs_sGmpMap,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    &fsm_eCrsMvs_sPrep,
    /* Title_Screen */   NULL,
    /* Animation */      NULL,
};

const fsm_eCrsMvd_s_t fsm_eCrsMvd_s[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     NULL,
    /* Cutscene */       NULL,
    /* Gameplay_Map */   &fsm_eCrsMvd_sGmpMap,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    NULL,
    /* Title_Screen */   NULL,
    /* Animation */      NULL,
};

/* --- SUB-ACTIONS --- */
const fsm_eGmp2Stby_s_t fsm_eGmp2Stby_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  &fsm_eGmp2Stby_ssMapUnitMv,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         NULL,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

/* Substate only, don't care about state*/
const fsm_eCrsMvs_s_t fsm_eCrsMvs_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_eCrsMvs_ssMenu,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         NULL,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eCrsMvs_s_t fsm_eCrsMvs_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  &fsm_eCrsMvs_sGmpMap_ssMapUnitMv,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_eCrsMvs_sGmpMap_ssStby,
    /* MAP_CANDIDATES */  &fsm_eCrsMvs_sGmpMap_ssMapCndt,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eCrsMvd_s_t fsm_eCrsMvd_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  &fsm_eCrsMvd_sGmpMap_ssMapUnitMv,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_eCrsMvd_sGmpMap_ssStby,
    /* MAP_CANDIDATES */  &fsm_eCrsMvd_sGmpMap_ssMapCndt,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

// fsm_eCrsMvd_s_t fsm_eCrsMvd_sPrep_ss[GAME_SUBSTATE_NUM] = {
//     /* NULL */            NULL,
//     /* MAP_MINIMAP */     NULL,
//     /* MENU */            NULL,
//     /* MAP_UNIT_MOVES */  NULL,
//     /* MAP_COMBAT */      NULL,
//     /* MAP_NPCTURN */     NULL,
//     /* SAVING */          NULL,
//     /* STANDBY */         NULL,
//     /* MAP_CANDIDATES */  &fsm_eCrsMvd_sPrep_ssMapCndt,
//     /* CUTSCENE */        NULL,
//     /* MAP_ANIMATION */   NULL
// };

const fsm_eCrsMvs_s_t fsm_eCrsMvs_sPrep_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         NULL,
    /* MAP_CANDIDATES */  &fsm_eCrsMvs_sPrep_ssMapCndt,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eAcpt_s_t fsm_eStart_sPrep_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_eStart_sPrep_ssMenu,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         NULL,
    /* MAP_CANDIDATES */  &fsm_eStart_sPrep_ssMapCndt,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eMenuLeft_sPrep_ss_t fsm_eMenuLeft_sPrep_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_eMenuLeft_sPrep_ssMenu,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         NULL,
    /* MAP_CANDIDATES */  &fsm_eMenuLeft_sPrep_ssMapCndt,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eAcpt_s_t fsm_eAcpt_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     &fsm_eAcpt_sGmpMap_ssMapMini,
    /* MENU */            &fsm_eAcpt_sGmpMap_ssMenu,
    /* MAP_UNIT_MOVES */  &fsm_eAcpt_sGmpMap_ssMapUnitMv,
    /* MAP_COMBAT */      &fsm_eAcpt_sGmpMap_ssMapCmbt,
    /* MAP_NPCTURN */     &fsm_eAcpt_sGmpMap_ssMapNPC,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_eAcpt_sGmpMap_ssStby,
    /* MAP_CANDIDATES */  &fsm_eAcpt_sGmpMap_ssMapCndt,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eAcpt_s_t fsm_eAcpt_sPrep_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_eAcpt_sGmpMap_ssMenu,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         NULL,
    /* MAP_CANDIDATES */  &fsm_eAcpt_sPrep_ssMapCndt,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eCncl_s_t fsm_eCncl_sPrep_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_eCncl_sPrep_ssMenu,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         NULL,
    /* MAP_CANDIDATES */  &fsm_eCncl_sPrep_ssMapCndt,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eCncl_s_t fsm_eCncl_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_eCncl_sGmpMap_ssMenu,
    /* MAP_UNIT_MOVES */  &fsm_eCncl_sGmpMap_ssMapUnitMv,
    /* MAP_COMBAT */      &fsm_eCncl_sGmpMap_ssMapCmbt,
    /* MAP_NPCTURN */     &fsm_eCncl_sGmpMap_ssMapNPC,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_eCncl_sGmpMap_ssStby,
    /* MAP_CANDIDATES */  &fsm_eCncl_sGmpMap_ssMapCndt,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eStats_s_t fsm_eStats_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_eStats_sGmpMap_ssStby,
    /* MAP_CANDIDATES */  &fsm_eStats_sGmpMap_ssStby,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eStats_s_t fsm_eStats_sPrep_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_eStats_sPrep_ssMenu,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         NULL,
    /* MAP_CANDIDATES */  &fsm_eStats_sPrep_ssMapCndt,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eUnitDsel_s_t fsm_eUnitDsel_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_eUnitDsel_ssMenu,
    /* MAP_UNIT_MOVES */  &fsm_eUnitDsel_ssMapUnitMv,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         NULL,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eUnitSel_s_t fsm_eUnitSel_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_eUnitSel_ssStby,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eUnitDng_s_t fsm_eUnitDng_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_eUnitDng_ssStby,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eMenuRight_s_t fsm_eMenuRight_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_eMenuRight_sGmpMap_ssMenu,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_eMenuRight_sGmpMap_ssStby,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eMenuLeft_s_t fsm_eMenuLeft_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_eMenuLeft_sGmpMap_ssMenu,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_eMenuLeft_sGmpMap_ssStby,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eCrsHvUnit_s_t fsm_eCrsHvUnit_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_eCrsHvUnit_ssStby,
    /* MAP_CANDIDATES */  &fsm_eCrsHvUnit_ssMapCndt,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eCrsDeHvUnit_s_t fsm_eCrsDeHvUnit_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_eCrsDeHvUnit_ssStby,
    /* MAP_CANDIDATES */  &fsm_eCrsDeHvUnit_ssMapCndt,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

/* -- FSM: Input_globalRange EVENT -- */
const fsm_eGlbRng_s_t fsm_eGlbRng_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_eGlbRng_ssStby,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL
};

const fsm_eCmbtEnd_ss_t fsm_eCmbtEnd_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     &fsm_eCmbtEnd_ssMapNPC,
    /* SAVING */          NULL,
    /* STANDBY */         NULL,
    /* MAP_CANDIDATES */  &fsm_eCmbtEnd_ssMapCndt,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};


/* -- FSM: Input_globalRange EVENT -- */
void fsm_eGlbRng_ssStby(struct Game *sota) {
    /* --- Toggle global range --- */

    for (int i = 0; i < DARR_NUM(sota->map->enemies_onfield); i++) {
        tnecs_entity entity = sota->map->enemies_onfield[i];

        /* - Compute new dangermap  - */
        i32 *temp_danger = Map_Danger_Compute(sota->map, entity);

        if (sota->map->show_globalRange) {
            /* Currently showing global map, removing */
            Map_Global_Danger_Sub(sota->map, temp_danger);
        } else {
            /* Currently not showing global map, adding */
            Map_Global_Danger_Add(sota->map, temp_danger);
        }
    }

    if (sota->map->show_globalRange) {
        /* Currently showing global map, removing */
        /* Plus, adding back player dangermap */
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_DANGER, TNECS_NULL);
        Map_Danger_Perimeter_Compute(sota->map, sota->map->dangermap);
        sota->map->rendered_dangermap = sota->map->dangermap;
    } else {
        /* Currently not showing global map, adding */
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_GLOBAL_DANGER, TNECS_NULL);
        Map_Danger_Perimeter_Compute(sota->map, sota->map->global_dangermap);
        sota->map->rendered_dangermap = sota->map->global_dangermap;
    }

    /* Switching show_globalRange to new mode */
    sota->map->show_globalRange = !sota->map->show_globalRange;
}

// --- FSM ACTION AND SUBACTION DEFINITIONS ---

// -- FSM: Cursor_Hovers_Unit --
void fsm_eCrsHvUnit_ssStby(struct Game *sota, tnecs_entity hov_ent) {
    /* --- Show popup_unit --- */
    SDL_assert(hov_ent > TNECS_NULL);
    // TODO: put unit popup Loading into Map/Gameplay loading functions
    tnecs_entity popup_ent = sota->popups[POPUP_TYPE_HUD_UNIT];
    if (popup_ent == TNECS_NULL)
        Game_PopUp_Unit_Create(sota);
    popup_ent = sota->popups[POPUP_TYPE_HUD_UNIT];

    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    struct PopUp_Unit *popup_unit = popup->data;
    PopUp_Unit_Set(popup_unit, sota);
    SDL_assert(popup_unit->unit != NULL);
    popup->visible = true;
    const struct Position *cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    // TODO: use struct Point everywhere, replace .x = with struct equality
    struct Point pos = cursor_pos->tilemap_pos;

    Game_PopUp_Unit_Place(sota, pos);

    struct Unit *unit_ontile = TNECS_GET_COMPONENT(sota->world, hov_ent, Unit);
    if (unit_ontile->waits) {
        return;
    }

    /* -- Compute attackmap and movemap -- */
    sota->map->update   = true;

    /* - MapAct settings for attacktolist - */
    MapAct map_to = MapAct_default;

    /* Don't show movemap if AI never moves */
    struct AI *ai = TNECS_GET_COMPONENT(sota->world, hov_ent, AI);
    if (ai != NULL)
        map_to.move = (ai->move != AI_MOVE_NEVER);
    else
        map_to.move = true;

    map_to.archetype        = ITEM_ARCHETYPE_STAFF;
    map_to.eq_type          = LOADOUT_EQUIPMENT;
    map_to.output_type      = ARRAY_MATRIX;
    map_to.aggressor        = hov_ent;
    map_to.mode_movetile    = MOVETILE_INCLUDE;

    /* - healtopmap - */
    Map_Act_To(sota->map, map_to);


    /* - attacktomap - */
    map_to.archetype     = ITEM_ARCHETYPE_WEAPON;
    Map_Act_To(sota->map, map_to);

    // SDL_Log("MOVE");
    // matrix_print(sota->map->movemap, sota->map->row_len, sota->map->col_len);
    // SDL_Log("ATK");
    // matrix_print(sota->map->attacktomap, sota->map->row_len, sota->map->col_len);
    // SDL_Log("HEAL");
    // matrix_print(sota->map->healtomap, sota->map->row_len, sota->map->col_len);

    int rangemap = Unit_Rangemap_Get(unit_ontile);

    /* - Compute new stackmap with recomputed attacktomap - */
    int overlays = MAP_OVERLAY_MOVE + MAP_OVERLAY_DANGER + MAP_OVERLAY_GLOBAL_DANGER;
    if (rangemap        == RANGEMAP_HEALMAP) {
        overlays += MAP_OVERLAY_HEAL;
        Map_Palettemap_Autoset(sota->map, overlays, hov_ent);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        overlays += MAP_OVERLAY_ATTACK;
        Map_Palettemap_Autoset(sota->map, overlays, TNECS_NULL);
    }

    /* Stack all overlay maps */
    if (sota->map->show_globalRange) {
        Map_Stacked_Dangermap_Compute(sota->map, sota->map->global_dangermap);
    } else {
        Map_Stacked_Dangermap_Compute(sota->map, sota->map->dangermap);
    }
    sota->map->show_icons = SotA_isPC(unit_ontile->army);

    /* -- Changing animation loop to Taunt -- */
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, hov_ent, Sprite);
    b32 animated = TNECS_ENTITY_HASCOMPONENT(sota->world, hov_ent, Timer);
    if ((sprite->spritesheet != NULL) && (animated) && (!unit_ontile->waits)) {
        if (sprite->spritesheet->loop_num == MAP_UNIT_LOOP_NUM) {
            Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_TAUNT, sprite->flip);
            Sprite_Animation_Loop(sprite);
            Sprite_Draw(sprite, sota->renderer);
        }
    }
}

void fsm_eCrsHvUnit_ssMapCndt(struct Game *sota, tnecs_entity hov_ent) {
    /* --- Select new candidate for action --- */
    SDL_assert(hov_ent > TNECS_NULL);
    sota->hovered_unit_entity = hov_ent;
}

// -- FSM: Cursor_Dehovers_Unit --
void fsm_eCrsDeHvUnit_ssStby(struct Game *sota, tnecs_entity dehov_ent) {
    /* -- Re-computing overlay -- */
    Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_DANGER + MAP_OVERLAY_GLOBAL_DANGER, TNECS_NULL);
    Map_Stacked_Dangermap_Reset(sota->map);

    tnecs_entity popup_ent = sota->popups[POPUP_TYPE_HUD_UNIT];
    SDL_assert(popup_ent > TNECS_NULL);

    /* -- Getting popup -- */
    const struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    struct Point pos    = cursor_pos->tilemap_pos;
    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    struct n9Patch *n9patch = &popup->n9patch;
    struct PopUp_Unit *popup_unit = popup->data;
    // popup_unit->unit = NULL;

    /* -- Placing popup_unit out of view -- */
    // Game_PopUp_Unit_Place(sota, pos);
    struct Position *position  = TNECS_GET_COMPONENT(sota->world, popup_ent, Position);
    struct Slider *slider  = TNECS_GET_COMPONENT(sota->world, popup_ent, Slider);
    struct SliderOffscreen *offscreen;
    offscreen = TNECS_GET_COMPONENT(sota->world, popup_ent, SliderOffscreen);
    offscreen->go_offscreen = true;
    // TODO stop resetting target?
    slider->target.x = 0;
    slider->target.y = 0;

    // SDL_Rect rect =   {
    //     .x = position->pixel_pos.x,
    //     .y = position->pixel_pos.y,
    //     .w = n9patch->patch_pixels.x * n9patch->size_patches.x,
    //     .h = n9patch->patch_pixels.y * n9patch->size_patches.y,
    // };

    SDL_Rect rect = Utilities_Rect(position, n9patch);

    Slider_Target_Offscreen(slider, offscreen, &rect);
    Slider_Start(slider, &position->pixel_pos, &offscreen->target);

    /* -- Changing animation loop to IDLE -- */
    struct Unit *unit     = TNECS_GET_COMPONENT(sota->world, dehov_ent, Unit);
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, dehov_ent, Sprite);
    SDL_assert(unit   != NULL);
    SDL_assert(sprite != NULL);
    b32 animated = TNECS_ENTITY_HASCOMPONENT(sota->world, dehov_ent, Timer);

    /* Only if unit doesn't wait */
    if ((sprite->spritesheet != NULL) && (animated) && (!unit->waits)) {
        if (sprite->spritesheet->loop_num == MAP_UNIT_LOOP_NUM) {
            Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_IDLE, sprite->flip);
            Sprite_Animation_Loop(sprite);
            Sprite_Draw(sprite, sota->renderer);
        }
    }

    // In case an enemy unit was selected.
    sota->selected_unit_entity = TNECS_NULL;

    // SDL_assert(popup_unit->unit == NULL);

}

void fsm_eCrsDeHvUnit_ssMapCndt(struct Game *sota, tnecs_entity dehov_ent) {
    /* -- Reset unit loop to Idle --  */
    // ONLY FOR ATTACK
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, dehov_ent, Sprite);
    Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_IDLE, sprite->flip);
    Sprite_Animation_Loop(sprite);
    Sprite_Draw(sprite, sota->renderer);
}

/* -- FSM: INPUT_CANCEL EVENT -- */
void fsm_eCncl_sPrep_ssMenu(struct Game *sota, tnecs_entity ent) {
    SDL_assert(sota->deployment_menu > TNECS_NULL);

    struct Menu *mc;
    tnecs_entity top_menu = sota->menu_stack[DARR_NUM(sota->menu_stack) - 1];
    mc = TNECS_GET_COMPONENT(sota->world, top_menu, Menu);

    if (fsm_eCncl_sPrep_ssMenu_m[mc->type] != NULL)
        fsm_eCncl_sPrep_ssMenu_m[mc->type](sota, mc);
}

void fsm_eCncl_sPrep_ssMapCndt( struct Game *sota, tnecs_entity ent) {
    /* --- Starting Position -> Deployment Menu --- */
    fsm_eMenuLeft_sPrep_ssMapCndt(sota);
}

void fsm_eCncl_sPrep(struct Game *sota, tnecs_entity canceller) {
    if (fsm_eCncl_sPrep_ss[sota->substate] != NULL)
        fsm_eCncl_sPrep_ss[sota->substate](sota, canceller);
}

void fsm_eCncl_sGmpMap(struct Game *sota, tnecs_entity canceller) {
    if (fsm_eCncl_sGmpMap_ss[sota->substate] != NULL)
        fsm_eCncl_sGmpMap_ss[sota->substate](sota, canceller);
}

void fsm_eUnitDng_ssStby(struct Game *sota, tnecs_entity selector_entity) {
    /* --- Show dangermap for selected unit --- */
    /* -- Preliminaries -- */
    tnecs_entity selected = sota->selected_unit_entity;
    SDL_assert(selector_entity > TNECS_NULL);
    SDL_assert(sota->selected_unit_entity > TNECS_NULL);

    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, selected, Unit);

    /* -- Skip if enemy can't attack -- */
    if (!Unit_canAttack(unit)) {
        SDL_Log("Enemy unit can't attack");
        return;
    }

    if (sota->map->show_globalRange) {
        SDL_Log("Global range/danger shown. Do nothing for Unit dangermap.");
        return;
    }

    /* -- Computing new dangermap -- */
    struct Position *pos    = TNECS_GET_COMPONENT(sota->world, selector_entity, Position);
    i32 *temp_danger        = Map_Danger_Compute(sota->map, selected);
    int map_index = pos->tilemap_pos.y * sota->map->col_len + pos->tilemap_pos.x;
    if (unit->show_danger) {
        Map_Danger_Sub(sota->map, temp_danger);
        Map_Palettemap_Autoset(sota->map,
                               MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_DANGER + MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK, TNECS_NULL);
        Map_Danger_Perimeter_Compute(sota->map, sota->map->dangermap);
        sota->map->rendered_dangermap = sota->map->dangermap;

        unit->show_danger = false;
    } else {
        Map_Danger_Add(sota->map, temp_danger);
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_DANGER, TNECS_NULL);
        Map_Danger_Perimeter_Compute(sota->map, sota->map->dangermap);
        sota->map->rendered_dangermap = sota->map->dangermap;
        unit->show_danger = true;
    }

    // matrix_print(sota->map->attacktomap, sota->map->row_len, sota->map->col_len);
}

void fsm_eCncl_sGmpMap_ssStby(struct Game *sota, tnecs_entity canceller) {
    SDL_assert(canceller > 0);
    SDL_assert(sota->state == GAME_STATE_Gameplay_Map);
    /* -- Preliminaries -- */
    *data1_entity = canceller;
    struct Unit *unit_ontile;
    struct Position *pos = TNECS_GET_COMPONENT(sota->world, canceller, Position);
    struct Point cpos = pos->tilemap_pos;
    tnecs_entity ontile = sota->map->unitmap[cpos.y * sota->map->col_len + cpos.x];

    if (ontile == TNECS_NULL) {
        return;
    }

    /* -- Show danger map if enemy -- */
    unit_ontile = TNECS_GET_COMPONENT(sota->world, ontile, Unit);
    SDL_assert(unit_ontile);
    SDL_assert(data2_entity != NULL);
    *data2_entity = ontile;
    if (!SotA_isPC(unit_ontile->army) && unit_ontile->show_danger)
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Danger, data1_entity, data2_entity);

}

void fsm_eCncl_sGmpMap_ssMapCndt(struct Game *sota, tnecs_entity canceller) {

    /* 1. Dehover candidate defendant */
    SDL_assert(sota->entity_cursor != TNECS_NULL);
    const struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);

    struct Point pos = cursor_pos->tilemap_pos;
    int current_i = pos.y * sota->map->col_len + pos.x;
    tnecs_entity ontile = sota->map->unitmap[current_i];

    SDL_assert(ontile > TNECS_NULL);
    // Try to limit calling FSM function directly.
    fsm_eCrsDeHvUnit_ssMapCndt(sota, ontile);

    /* 2. set state to Menu */
    strncpy(sota->reason, "Cancel Selection of candidates", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->reason);

    if (fsm_eCncl_sGmpMap_ssMapCndt_mo[sota->selected_menu_option] != NULL)
        fsm_eCncl_sGmpMap_ssMapCndt_mo[sota->selected_menu_option](sota, NULL);
}

void fsm_eCncl_sGmpMap_ssMenu(struct Game *sota, tnecs_entity canceller) {
    b32 destroy = false;
    tnecs_entity ent_topop = sota->menu_stack[DARR_NUM(sota->menu_stack) - 1];
    SDL_assert(ent_topop > TNECS_NULL);
    struct Menu *mc_topop = TNECS_GET_COMPONENT(sota->world, ent_topop, Menu);

    if (fsm_eCncl_sGmpMap_ssMenu_m[mc_topop->type] != NULL)
        fsm_eCncl_sGmpMap_ssMenu_m[mc_topop->type](sota, mc_topop);

    if (DARR_NUM(sota->menu_stack) == 0)
        Game_cursorFocus_onMap(sota);

}

void fsm_eCncl_sGmpMap_ssMapUnitMv(struct Game *sota, tnecs_entity canceller) {
    /* --- Hide movemap, return unit to starting pos --- */
    sota->map->arrow->show = false;

    /* Return unit to initial pos, deselect */
    if (sota->selected_unit_entity > TNECS_NULL) {
        *data2_entity = sota->selected_unit_entity;
        SDL_assert(*data1_entity > 0);
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Icon_Return, NULL, NULL);
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Deselect, data1_entity, data2_entity);
    }

    /* Make Popup_Tile visible */
    tnecs_entity popup_ent = sota->popups[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    SDL_assert(popup != NULL);
    popup->visible = true;

    /* Reset cursor lastpos to current pos */
    // prevents cursor dehovering from far away
    const struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    sota->cursor_lastpos.x = cursor_pos->tilemap_pos.x;
    sota->cursor_lastpos.y = cursor_pos->tilemap_pos.y;
}

void fsm_eCncl_sGmpMap_ssAnim(struct Game *sota, tnecs_entity canceller) {
}

void fsm_eCncl_sGmpMap_ssMapCmbt(struct Game *sota, tnecs_entity canceller) {
}

void fsm_eCncl_sGmpMap_ssMapNPC(struct Game *sota, tnecs_entity canceller) {

}

// -- FSM: CURSOR_MOVES EVENT --
void fsm_eCrsMvs_sGmpMap(struct Game *sota, tnecs_entity mover_entity,
                         struct Point *cursor_move) {
    if (fsm_eCrsMvs_sGmpMap_ss[sota->substate] != NULL)
        fsm_eCrsMvs_sGmpMap_ss[sota->substate](sota, mover_entity, cursor_move);
}

void fsm_eCrsMvs_sPrep(struct Game *sota, tnecs_entity mover_entity,
                       struct Point *cursor_move) {
    if (fsm_eCrsMvs_sPrep_ss[sota->substate] != NULL)
        fsm_eCrsMvs_sPrep_ss[sota->substate](sota, mover_entity, cursor_move);
}

void fsm_eCrsMvs_sGmpMap_ssStby(struct Game *sota, tnecs_entity mover_entity,
                                struct Point *cursor_move) {
    tnecs_entity cursor = sota->entity_cursor;
    struct Position *cursor_pos = TNECS_GET_COMPONENT(sota->world, cursor, Position);
    struct Slider   *cursor_sl  = TNECS_GET_COMPONENT(sota->world, cursor, Slider);
    struct Sprite   *cursor_sp  = TNECS_GET_COMPONENT(sota->world, cursor, Sprite);

    /* Actually move the cursor from cursor_move_data set by systemControl */
    // Note: always on tilemap
    Position_Pos_Add(cursor_pos, sota->cursor_move.x, sota->cursor_move.y);
    Cursor_Target(cursor_sl, cursor_sp, cursor_pos);
    sota->cursor_move.x = 0;
    sota->cursor_move.y = 0;

    /* -- Give tile to popup -- */
    tnecs_entity ent = sota->popups[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, ent, PopUp);
    struct PopUp_Tile *popup_tile = popup->data;
    PopUp_Tile_Set(popup_tile, sota);
}

void fsm_eCrsMvs_ssMenu(struct Game *sota, tnecs_entity mover_entity,
                        struct Point *cursor_move) {
    /* Find menu elem in direction */
    tnecs_entity menu = sota->menu_stack[DARR_NUM(sota->menu_stack) - 1];
    struct Menu *mc = TNECS_GET_COMPONENT(sota->world, menu, Menu);

    /* menu elem move FSM to find next elem depending on menu type */
    SDL_assert(mc->type > MENU_TYPE_START);
    SDL_assert(mc->type < MENU_TYPE_END);
    SDL_assert(menu_elem_move[mc->type] != NULL);
    i8 new_elem = menu_elem_move[mc->type](mc, sota->moved_direction);

    /* - TODO: MAKE FUNCTION - */
    /* - Move to cursor to new_elem - */
    SDL_assert(new_elem > MENU_ELEM_NULL);
    Menu_Elem_Set(mc, sota, new_elem);
    sota->cursor_move.x = 0;
    sota->cursor_move.y = 0;

    if (fsm_eCrsMvs_sGmpMap_ssMenu_m[mc->type] != NULL)
        fsm_eCrsMvs_sGmpMap_ssMenu_m[mc->type](sota, mc);
}

void fsm_eCrsMvs_sGmpMap_ssMapCndt(struct Game *sota, tnecs_entity mover_entity,
                                   struct Point *cursor_move) {
    SDL_assert(sota->patients != sota->defendants);

    /* Find menu elem in direction */
    SDL_assert(sota->candidates != NULL);
    int num = DARR_NUM(sota->candidates);
    SDL_assert(num > 0);
    if ((sota->cursor_move.x > 0) || (sota->cursor_move.y > 0)) {
        sota->candidate = (sota->candidate + 1) % num;
    } else if ((sota->cursor_move.x < 0) || (sota->cursor_move.y < 0)) {
        sota->candidate = (sota->candidate - 1 + num) % num;
    }

    /* Action depending on previously selected menu option */
    if (fsm_eCrsMvs_sGmpMap_mo[sota->selected_menu_option] != NULL)
        fsm_eCrsMvs_sGmpMap_mo[sota->selected_menu_option](sota, NULL);

    Game_Cursor_Move_toCandidate(sota);

    /* Reset cursor_move */
    sota->cursor_move.x = 0;
    sota->cursor_move.y = 0;

    /* Update pre_combat_popup */
    if (sota->candidates == sota->defendants) {
        Game_PopUp_Pre_Combat_Enable(sota);
    }
}

void fsm_eCrsMvs_sGmpMap_ssMapUnitMv(struct Game *sota,
                                     tnecs_entity mover_entity, struct Point *cursor_move) {

    /* -- Move cursor -- */
    tnecs_entity cursor         = sota->entity_cursor;
    struct Position *cursor_pos = TNECS_GET_COMPONENT(sota->world, cursor, Position);
    struct Slider   *cursor_sl  = TNECS_GET_COMPONENT(sota->world, cursor, Slider);
    struct Sprite   *cursor_sp  = TNECS_GET_COMPONENT(sota->world, cursor, Sprite);

    /* Always on tilemap */
    Position_Pos_Add(cursor_pos, sota->cursor_move.x, sota->cursor_move.y);
    Cursor_Target(cursor_sl, cursor_sp, cursor_pos);
    sota->cursor_move.x = 0;
    sota->cursor_move.y = 0;

    /* -- give tile to popup -- */
    tnecs_entity ent = sota->popups[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, ent, PopUp);
    struct PopUp_Tile *popup_tile = popup->data;
    PopUp_Tile_Set(popup_tile, sota);
}

// -- FSM: CURSOR_MOVED EVENT --
void fsm_eCrsMvd_sGmpMap(struct Game *sota, tnecs_entity mover_entity,
                         struct Point *cursor_move) {
    SDL_assert(sota->entity_cursor != TNECS_NULL);

    struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    struct Point *pos = &cursor_pos->tilemap_pos;

    if (fsm_eCrsMvd_sGmpMap_ss[sota->substate] != NULL)
        fsm_eCrsMvd_sGmpMap_ss[sota->substate](sota, mover_entity, pos);

    Game_PopUp_Unit_Place(sota, *pos);
    Game_PopUp_Tile_Place(sota, *pos);
}

void fsm_eCrsMvd_sGmpMap_ssStby(struct Game *sota, tnecs_entity mover_entity,
                                struct Point *cursor_move) {

    // SDL_assert(sota->moved_direction > -1);
    SDL_assert(sota->entity_cursor != TNECS_NULL);
    const struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);

    struct Point pos          = cursor_pos->tilemap_pos;
    struct Point previous_pos = sota->cursor_lastpos;
    int previous_i = previous_pos.y * sota->map->col_len + previous_pos.x;
    int current_i  = pos.y * sota->map->col_len + pos.x;
    tnecs_entity unit_entity_previoustile = sota->map->unitmap[previous_i];
    // NOTE: unit_entity_previoustile might be different than selected_unit_entity
    //     because
    tnecs_entity ontile = sota->map->unitmap[current_i];

    /* unit hovering/dehovering */
    if (unit_entity_previoustile != TNECS_NULL) {
        *data1_entity = unit_entity_previoustile;
        Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Dehovers_Unit, data1_entity, NULL);
    }
    if (ontile != TNECS_NULL) {
        *data2_entity = ontile;
        Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Hovers_Unit, NULL, data2_entity);
    }

    sota->cursor_lastpos.x = pos.x;
    sota->cursor_lastpos.y = pos.y;
}

void fsm_eCrsMvd_sGmpMap_ssMapCndt(struct Game *sota, tnecs_entity mover_entity,
                                   struct Point *cursor_move) {

    const struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    struct Point pos = cursor_pos->tilemap_pos;
    // SDL_assert(sota->moved_direction > -1);
    struct Point previous_pos;

    struct Point moved = Ternary_Moved(sota->moved_direction);
    previous_pos.x = pos.x + moved.x;
    previous_pos.y = pos.y + moved.y;

    int previous_i = previous_pos.y * sota->map->col_len + previous_pos.x;
    int current_i = pos.y * sota->map->col_len + pos.x;
    tnecs_entity unit_entity_previoustile = sota->map->unitmap[previous_i];
    tnecs_entity ontile = sota->map->unitmap[current_i];

    /* unit hovering/dehovering */
    if (unit_entity_previoustile != TNECS_NULL) {
        *data1_entity = unit_entity_previoustile;
        Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Dehovers_Unit, data1_entity, NULL);
    }
    if (ontile != TNECS_NULL) {
        *data2_entity = ontile;
        Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Hovers_Unit, NULL, data2_entity);
    }

}

void fsm_eCrsMvs_sPrep_ssMapCndt(struct Game  *sota, tnecs_entity mover_entity,
                                 struct Point *nope) {
    /* --- Move cursor to next starting position on map --- */
    // TODO: stop cursor moving so fast

    tnecs_entity cursor = sota->entity_cursor;
    struct Position *cursor_pos = TNECS_GET_COMPONENT(sota->world, cursor, Position);
    struct Slider   *cursor_sl  = TNECS_GET_COMPONENT(sota->world, cursor, Slider);
    struct Sprite   *cursor_sp  = TNECS_GET_COMPONENT(sota->world, cursor, Sprite);

    /* Actually move the cursor from cursor_move_data set by systemControl */
    // Note: always on tilemap
    int num_pos = DARR_NUM(sota->map->start_pos);
    if ((sota->cursor_move.x > 0) || (sota->cursor_move.y > 0)) {
        sota->candidate = sota->candidate >= (num_pos - 1) ? 0 : sota->candidate + 1;
    } else if ((sota->cursor_move.x < 0) || (sota->cursor_move.y < 0)) {
        sota->candidate = sota->candidate <= 0 ? num_pos - 1 : sota->candidate - 1;
    }
    struct Menu *mc;
    mc = TNECS_GET_COMPONENT(sota->world, sota->deployment_menu, Menu);
    struct DeploymentMenu *dm = mc->data;
    SDL_assert(dm != NULL);
    i32 start_pos_i = DeploymentMenu_Map_StartPos(dm, sota->candidate);
    struct Point next_pos = sota->map->start_pos[start_pos_i];
    Position_Pos_Set(cursor_pos, next_pos.x, next_pos.y);

    // Always on tilemap
    Cursor_Target(cursor_sl, cursor_sp, cursor_pos);
    sota->cursor_move.x = 0;
    sota->cursor_move.y = 0;
}

void fsm_eCrsMvd_sGmpMap_ssMapUnitMv(struct Game *sota, tnecs_entity mover_entity,
                                     struct Point *cursor_pos) {

    /* -- Unit follows cursor movement  -- */
    struct Position *selected_pos;
    selected_pos = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Position);
    selected_pos->tilemap_pos.x = cursor_pos->x;
    selected_pos->tilemap_pos.y = cursor_pos->y;
    selected_pos->pixel_pos.x = selected_pos->tilemap_pos.x * selected_pos->scale[0];
    selected_pos->pixel_pos.y = selected_pos->tilemap_pos.y * selected_pos->scale[1];

    /* -- Move arrow -- */
    struct Arrow *arrow = sota->map->arrow;

    Arrow_Path_Add(arrow, cursor_pos->x, cursor_pos->y);

    /* -- Update map_unit loop to follow arrow direction -- */
    struct Sprite *sprite;
    sprite = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Sprite);
    int path_num = DARR_NUM(arrow->pathlist) / TWO_D;

    if ((path_num > 0) && (sprite->spritesheet != NULL)) {
        i32 previous_x = arrow->pathlist[(path_num - 2) * TWO_D];
        i32 previous_y = arrow->pathlist[((path_num - 2) * TWO_D) + 1];
        if (sprite->spritesheet->loop_num == MAP_UNIT_LOOP_NUM) {
            struct Point move = {cursor_pos->x - previous_x, cursor_pos->y - previous_y};
            int direction = Ternary_Direction(move);
            int loop      = Utilities_Loop(direction, sprite->flip);
            Spritesheet_Loop_Set(sprite->spritesheet, loop, sprite->flip);
            Sprite_Dstrect_Relative(sprite, &selected_pos->pixel_pos, &sota->camera);
            Sprite_Animation_Loop(sprite);
        }
        Sprite_Draw(sprite, sota->renderer);
    }

}

// -- FSM: Gameplay_Return2Standby EVENT --
void fsm_eGmp2Stby_ssMapUnitMv(struct Game *sota, tnecs_entity ent) {

    if (sota->selected_unit_entity > 0)
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Icon_Return, NULL, NULL);

}

void fsm_eGmp2Stby_sGmpMap(struct Game *sota, tnecs_entity controller_entity) {
    if (sota->selected_unit_entity > TNECS_NULL) {

        *data1_entity = controller_entity;
        *data2_entity = sota->selected_unit_entity;
        if (sota->selected_unit_entity > 0)
            Event_Emit(__func__, SDL_USEREVENT, event_Unit_Deselect, data1_entity, data2_entity);
    }

    sota->map->show_overlay = false;
}

/* -- FSM: Input_Start EVENT -- */
void fsm_eStart_sScnTalk(struct Game *sota, tnecs_entity accepter) {
    /* --- Skip scene --- */
    SDL_assert(sota->scene > TNECS_NULL);
    struct Scene *scene = TNECS_GET_COMPONENT(sota->world, sota->scene, Scene);
    Scene_Finish(scene, sota);
}

void fsm_eStart_sCutScn(struct Game *sota, tnecs_entity nope) {
    /* --- Finish Cutscene --- */
    SDL_assert(sota->cutscene > TNECS_NULL);
    struct Cutscene *cutscene = TNECS_GET_COMPONENT(sota->world, sota->cutscene, Cutscene);
    Cutscene_Finish(cutscene, sota);
}

void fsm_eStart_sPrep(struct Game *sota, tnecs_entity accepter) {
    /* --- Preparation done: Start Map gameplay --- */
    // TODO: check that in prep stat, menu is deployment
    if (fsm_eStart_sPrep_ss[sota->substate] != NULL)
        fsm_eStart_sPrep_ss[sota->substate](sota, accepter);
}

void fsm_eStart_sPrep_ssMapCndt(struct Game *sota, tnecs_entity ent) {
    /* --- Start battle --- */
    SDL_assert(sota->deployment_menu > TNECS_NULL);
    struct Menu *mc = TNECS_GET_COMPONENT(sota->world, sota->deployment_menu, Menu);
    Game_Battle_Start(sota, mc);
    SDL_assert(mc != NULL);
}

void fsm_eStart_sPrep_ssMenu(struct Game *sota, tnecs_entity ent) {
    /* --- Preparation done: Start Map gameplay --- */
    SDL_assert(DARR_NUM(sota->menu_stack) > 0);
    tnecs_entity top_menu = sota->menu_stack[DARR_NUM(sota->menu_stack) - 1];
    SDL_assert(top_menu > TNECS_NULL);
    SDL_assert(top_menu == sota->deployment_menu);
    struct Menu *mc = TNECS_GET_COMPONENT(sota->world, top_menu, Menu);
    SDL_assert(mc != NULL);
    if (fsm_eStart_sPrep_ssMenu_m[mc->type] != NULL)
        fsm_eStart_sPrep_ssMenu_m[mc->type](sota, mc);
}

/* -- FSM: Input_Accept EVENT -- */
void fsm_eAcpt_sScnTalk(struct Game *sota, tnecs_entity nope) {
    /* --- Get next line at normal speed --- */
    SDL_assert(sota->scene > TNECS_NULL);
    struct Scene *scene = TNECS_GET_COMPONENT(sota->world, sota->scene, Scene);
    if (Scene_Statement_Next(scene) < 0) {
        Scene_Finish(scene, sota);
    }
}

void fsm_eAcpt_sCutScn(struct Game *sota, tnecs_entity nope) {
    /* --- Finish cutscene --- */
    SDL_assert(sota->cutscene > TNECS_NULL);
    struct Cutscene *cutscene = TNECS_GET_COMPONENT(sota->world, sota->cutscene, Cutscene);
    Cutscene_Finish(cutscene, sota);
}

void fsm_eAcpt_sGmpMap(struct Game *sota, tnecs_entity accepter) {
    if (fsm_eAcpt_sGmpMap_ss[sota->substate] != NULL)
        fsm_eAcpt_sGmpMap_ss[sota->substate](sota, accepter);
}

void fsm_eAcpt_sPrep(struct Game *sota, tnecs_entity accepter) {
    if (fsm_eAcpt_sPrep_ss[sota->substate] != NULL)
        fsm_eAcpt_sPrep_ss[sota->substate](sota, accepter);
}

void fsm_eAcpt_sTtlScrn(struct Game *sota, tnecs_entity accepter) {
    if (sota->substate != GAME_SUBSTATE_MENU) {
        SDL_Log("Wrong substate %d on Title_Screen state", sota->substate);
        exit(ERROR_Generic);
    }
    fsm_eAcpt_sGmpMap_ssMenu(sota, accepter);
}

/* -- Input_Accept -- */
void fsm_eAcpt_sGmpMap_ssMapCndt(struct Game *sota, tnecs_entity canceller) {
    if (fsm_eAcpt_sGmpMap_ssMapCndt_mo[sota->selected_menu_option] != NULL)
        fsm_eAcpt_sGmpMap_ssMapCndt_mo[sota->selected_menu_option](sota, NULL);
}

void fsm_eAcpt_sGmpMap_ssStby(struct Game *sota, tnecs_entity accepter) {
    SDL_assert(sota->entity_cursor);
    const struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    SDL_assert(cursor_pos != NULL);
    struct Point pos = cursor_pos->tilemap_pos;
    tnecs_entity ontile = sota->map->unitmap[pos.y * sota->map->col_len + pos.x];
    if (ontile != TNECS_NULL) {
        /* -- select unit -- */
        *data2_entity = ontile;
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Select, data1_entity, data2_entity);
    } else {
        /* -- draw map_action menu -- */
        if (sota->player_select_menus[MENU_PLAYER_SELECT_MAP_ACTION] == TNECS_NULL) {
            SDL_assert(sota->player_select_menus[MENU_PLAYER_SELECT_MAP_ACTION] == TNECS_NULL);
            Game_PlayerSelectMenu_Create(sota, MENU_PLAYER_SELECT_MAP_ACTION);
            SDL_assert(sota->player_select_menus[MENU_PLAYER_SELECT_MAP_ACTION] > TNECS_NULL);
        }
        Game_menuStack_Push(sota, sota->player_select_menus[MENU_PLAYER_SELECT_MAP_ACTION]);
        SDL_assert(sota->player_select_menus[MENU_PLAYER_SELECT_MAP_ACTION] > TNECS_NULL);
        Game_PlayerSelectMenu_Update(sota, MENU_PLAYER_SELECT_MAP_ACTION);
        Game_cursorFocus_onMenu(sota);
        strncpy(sota->reason, "no unit was selected", sizeof(sota->reason));
        Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->reason);
        Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->stats_menu, NULL);
    }
}

void fsm_eAcpt_sPrep_ssMapCndt(struct Game *sota, tnecs_entity accepter_entity) {
    /* Select a unit on starting position, or move it */
    SDL_assert(sota->deployment_menu > TNECS_NULL);
    struct Menu *mc;
    mc = TNECS_GET_COMPONENT(sota->world, sota->deployment_menu, Menu);
    SDL_assert(mc != NULL);
    struct DeploymentMenu *dm = mc->data;
    SDL_assert(dm != NULL);

    if (dm->start_pos_order1 >= 0) {
        /* Unit was selected previously, exchange with currently selected tile */
        DeploymentMenu_Map_StartPos_Select(dm, sota->candidate);
        SDL_assert(dm->start_pos_order1 >= 0);
        SDL_assert(dm->start_pos_order2 >= 0);
        if (dm->start_pos_order1 == dm->start_pos_order2) {
            dm->start_pos_order1 = -1;
            dm->start_pos_order2 = -1;
            return;
        }

        struct Point pos1 = sota->map->start_pos[dm->start_pos_order1];
        struct Point pos2 = sota->map->start_pos[dm->start_pos_order2];

        tnecs_entity old_ent = Map_Unit_Get(sota->map, pos1.x, pos1.y);
        tnecs_entity new_ent = Map_Unit_Get(sota->map, pos2.x, pos2.y);

        size_t index1 = pos1.y * sota->map->col_len + pos1.x;
        size_t index2 = pos2.y * sota->map->col_len + pos2.x;
        Map_Unit_Swap(sota->map, pos1.x, pos1.y, pos2.x, pos2.y);
        i32 pos1_i = DeploymentMenu_Map_Find_Pos(dm, sota->map, pos1.x, pos1.y);
        i32 pos2_i = DeploymentMenu_Map_Find_Pos(dm, sota->map, pos2.x, pos2.y);

        if (pos1_i != pos2_i) {
            DeploymentMenu_Map_Swap(dm);
        } else {
            DeploymentMenu_Map_StartPos_Deselect(dm);
        }
    } else {
        /* No unit was selected previously, selecting now */
        DeploymentMenu_Map_StartPos_Select(dm, sota->candidate);
    }
}

void fsm_eAcpt_sGmpMap_ssMenu(struct Game *sota, tnecs_entity accepter_entity) {
    // Possible state pairs: [?, MENU]

    SDL_assert(DARR_NUM(sota->menu_stack) > 0);
    tnecs_entity top_menu = sota->menu_stack[DARR_NUM(sota->menu_stack) - 1];
    SDL_assert(top_menu > TNECS_NULL);
    struct Menu *mc_topop = TNECS_GET_COMPONENT(sota->world, top_menu, Menu);

    if (fsm_eAcpt_sGmpMap_ssMenu_m[mc_topop->type] != NULL)
        fsm_eAcpt_sGmpMap_ssMenu_m[mc_topop->type](sota, mc_topop);

    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Select, NULL, NULL);
}

void fsm_eAcpt_sGmpMap_ssMapUnitMv(struct Game *sota, tnecs_entity accepter_entity) {
    /* - Unit should have been selected - */
    SDL_assert(sota->selected_unit_entity != TNECS_NULL);
    /* - Skip if friendly on tile - */
    const struct Position *cursor_pos;
    cursor_pos          = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    int current_i       = cursor_pos->tilemap_pos.y * sota->map->col_len +
                          cursor_pos->tilemap_pos.x;
    tnecs_entity ontile = sota->map->unitmap[current_i];

    /* - Someone else already occupies tile -> Do nothing - */
    if ((ontile != TNECS_NULL) && (sota->selected_unit_entity != ontile)) {
        // TODO: ontile should be a friendly unit.
        // SDL_assert();
        return;
    }

    tnecs_entity unit_ent = sota->selected_unit_entity;

    /* - Reset potential candidates - */
    sota->candidate     = 0;
    sota->candidates    = NULL;

    /* - Make popup_tile invisible - */
    tnecs_entity popup_ent = sota->popups[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup      = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    if (popup != NULL) {
        popup->visible = false;
    }

    /* - Get selected unit on tile - */
    struct Position *selected_pos;
    struct Unit *unit   = TNECS_GET_COMPONENT(sota->world, unit_ent,            Unit);
    selected_pos        = TNECS_GET_COMPONENT(sota->world, unit_ent,            Position);

    /* - Unit should be PC - */
    SDL_assert(SotA_isPC(unit->army));

    /* -- Creating Unit Action Menu -- */
    tnecs_entity *menu = &sota->player_select_menus[MENU_PLAYER_SELECT_UNIT_ACTION];
    if (*menu == 0)
        Game_PlayerSelectMenu_Create(sota, MENU_PLAYER_SELECT_UNIT_ACTION);
    Game_menuStack_Push(sota, *menu);

    /* - Send event_Menu_Created Event to set substate - */
    strncpy(sota->reason, "friendly unit has moved, time to choose action", sizeof(sota->reason));
    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, menu, NULL);
    sota->cursor_lastpos.x = cursor_pos->tilemap_pos.x;
    sota->cursor_lastpos.y = cursor_pos->tilemap_pos.y;

    /* - Moving unit to new tile - */
    sota->selected_unit_moved_position.x    = cursor_pos->tilemap_pos.x;
    sota->selected_unit_moved_position.y    = cursor_pos->tilemap_pos.y;
    struct Point initial                    = sota->selected_unit_initial_position;
    struct Point moved                      = sota->selected_unit_moved_position;
    if ((initial.x != moved.x) || (initial.y != moved.y))
        Map_Unit_Move(sota->map, initial.x, initial.y, moved.x, moved.y);

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
    Map_Act_To(sota->map, map_to);

    /* - attacktomap - */
    map_to.archetype     = ITEM_ARCHETYPE_WEAPON;
    Map_Act_To(sota->map, map_to);

    // SDL_Log("ATK");
    // matrix_print(sota->map->attacktomap, sota->map->row_len, sota->map->col_len);
    // SDL_Log("HEAL");
    // matrix_print(sota->map->healtomap, sota->map->row_len, sota->map->col_len);

    int rangemap = Unit_Rangemap_Get(unit);
    if (rangemap == RANGEMAP_HEALMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL,
                               unit_ent);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        Map_Palettemap_Autoset(sota->map,
                               MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK, TNECS_NULL);
    }
    Map_Stacked_Dangermap_Compute(sota->map, sota->map->dangermap);

    /* - Update Standard passive aura for all units - */
    SDL_assert(sota->map->army_i >= 0);
    SDL_assert(sota->map->army_i < DARR_NUM(sota->map->army_onfield));
    i32 army = sota->map->army_onfield[sota->map->army_i];
    SDL_assert(army == unit->army); /* only units in current army should be moving */
    Map_Bonus_Remove_Instant(sota->map, unit->army);
    Map_Bonus_Standard_Apply(sota->map, unit->army);

    /* - Pre-menu update computation for content - */
    Game_preUnitAction_Targets(sota, unit_ent);

    /* - Update menu to create content - */
    Game_PlayerSelectMenu_Update(sota, MENU_PLAYER_SELECT_UNIT_ACTION);

    /* - Focusing cursor on Menu - */
    Game_cursorFocus_onMenu(sota);
}

void fsm_eAcpt_sGmpMap_ssMapMini(struct Game *sota, tnecs_entity accepter_entity) {
}

void fsm_eAcpt_sGmpMap_ssAnim(struct Game *sota, tnecs_entity accepter_entity) {
}

void fsm_eAcpt_sGmpMap_ssMapCmbt(struct Game *sota, tnecs_entity accepter_entity) {
}

void fsm_eAcpt_sGmpMap_ssMapNPC(struct Game *sota, tnecs_entity accepter_entity) {
}

/* Input_Stats */
void fsm_eStats_sPrep(  struct Game *sota, tnecs_entity ent) {
    if (fsm_eStats_sPrep_ss[sota->substate] != NULL)
        fsm_eStats_sPrep_ss[sota->substate](sota, TNECS_NULL);
}

void fsm_eStats_sGmpMap(struct Game *sota, tnecs_entity ent) {
    fsm_eStats_sGmpMap_ssStby(sota, ent);
}

void fsm_eStats_sPrep_ssMapCndt(struct Game *sota, tnecs_entity ent) {
    /* Find which unit is hovered on map */
    SDL_assert(sota->entity_cursor);
    struct Position *cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    SDL_assert(cursor_pos != NULL);
    struct Point pos = cursor_pos->tilemap_pos;
    tnecs_entity ontile = sota->map->unitmap[pos.y * sota->map->col_len + pos.x];

    /* Enabling stats menu for hovered unit */
    if (ontile > TNECS_NULL) {
        Game_StatsMenu_Enable(sota, ontile);
    }
}

void fsm_eStats_sPrep_ssMenu(  struct Game *sota, tnecs_entity ent) {
    SDL_assert(sota->deployment_menu > TNECS_NULL);

    struct Menu *mc;
    mc = TNECS_GET_COMPONENT(sota->world, sota->deployment_menu, Menu);

    if (fsm_eStats_sPrep_ssMenu_m[mc->type] != NULL)
        fsm_eStats_sPrep_ssMenu_m[mc->type](sota, mc);
}

/* Displaying stats menu */
void fsm_eStats_sGmpMap_ssStby(struct Game *sota, tnecs_entity accepter) {
    SDL_assert((sota->state == GAME_STATE_Gameplay_Map) ||
               (sota->state == GAME_STATE_Preparation));

    /* Find which unit is hovered on map */
    SDL_assert(sota->entity_cursor);
    struct Position *cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    SDL_assert(cursor_pos != NULL);
    struct Point pos = cursor_pos->tilemap_pos;
    tnecs_entity ontile = sota->map->unitmap[pos.y * sota->map->col_len + pos.x];

    /* Enabling stats menu for hovered unit */
    if (ontile > TNECS_NULL)
        Game_StatsMenu_Enable(sota, ontile);
    else {
        /* -- TODO: draw a map marker here, like tear ring saga -- */
        // SDL_Log("NO UNIT ON TILE");
    }

    /* - Make popup_tile invisible - */
    tnecs_entity popup_ent = sota->popups[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup    = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    if (popup != NULL)
        popup->visible = false;

    /* - Make popup_unit invisible - */
    popup_ent   = sota->popups[POPUP_TYPE_HUD_UNIT];
    popup       = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    if (popup != NULL)
        popup->visible = false;
}

// -- FSM: UNIT_SELECT EVENT --
void fsm_eUnitSel_ssStby(struct Game *sota, tnecs_entity selector_entity) {
    struct Unit *selected_unit;
    selected_unit = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Unit);
    struct Position *selected_pos;
    selected_pos = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Position);
    sota->selected_unit_initial_position.x = selected_pos->tilemap_pos.x;
    sota->selected_unit_initial_position.y = selected_pos->tilemap_pos.y;

    if (!SotA_isPC(selected_unit->army)) {
        /* - Enemy unit was selected - */
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Danger, data1_entity, data2_entity);
        return;
    }

    /* - Friendly unit was selected - */
    if (!selected_unit->waits) {
        /* - Friendly unit can move - */
        sota->aggressor             = sota->selected_unit_entity;

        /* Make popup_unit invisible */
        // TODO: GO OFFSCREEN
        tnecs_entity popup_ent = sota->popups[POPUP_TYPE_HUD_UNIT];
        struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
        SDL_assert(popup != NULL);
        // popup->visible = false;
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Moves, data1_entity, data2_entity);
    }
}

// -- FSM: UNIT_DESELECT EVENT --
void fsm_eUnitDsel_ssMenu(struct Game *sota, tnecs_entity selector) {
    strncpy(sota->reason, "Unit was deselected during menu (unit waits?)", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_STANDBY, sota->reason);
}

void fsm_eUnitDsel_ssMapUnitMv(struct Game *sota, tnecs_entity selector) {
    /*  -- Hide arrow -- */
    sota->map->arrow->show = false;

    /*  -- Reset map overlay to danger only -- */
    struct Position *selected_pos;
    selected_pos = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Position);
    struct Position *pos = TNECS_GET_COMPONENT(sota->world, selector, Position);
    struct Point initial = sota->selected_unit_initial_position;
    if ((pos->tilemap_pos.x != initial.x) || (pos->tilemap_pos.y != initial.y)) {
        // Only if cursor not on unit.
        // If cursor is on unit, movemap and attackmap should be shown
        Map_Palettemap_Reset(sota->map);
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_DANGER, TNECS_NULL);
        Map_Stacked_Dangermap_Reset(sota->map);
    }

    strncpy(sota->reason, "Unit was deselected during movement", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_STANDBY, sota->reason);
}

// -- FSM: INPUT_MENURIGHT EVENT --
void fsm_eMenuRight_sGmpMap(struct Game *sota, i32 controller_type) {
    if (fsm_eMenuRight_sGmpMap_ss[sota->substate] != NULL)
        fsm_eMenuRight_sGmpMap_ss[sota->substate](sota, controller_type);
}

void fsm_eMenuRight_sScnTalk(struct Game *sota, i32 controller_type) {
    fsm_eAcpt_sScnTalk(sota, TNECS_NULL);
}

void fsm_eMenuRight_sGmpMap_ssMenu(struct Game *sota, i32 controller_type) {
    /* -- Pop previous menu -- */
    b32 destroy = false;
    tnecs_entity popped = Game_menuStack_Pop(sota, destroy);
    SDL_assert(popped > 0);

    /* - Hide previous menu - */
    struct Menu *mc_popped = TNECS_GET_COMPONENT(sota->world, popped, Menu);
    mc_popped->visible = false;

    /* -- Create New menu -- */
    /* - Get unit ontile - */
    struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    struct Point pos = cursor_pos->tilemap_pos;
    tnecs_entity ontile = sota->map->unitmap[pos.y * sota->map->col_len + pos.x];

    /* - Determine which menu is the next one - */
    SDL_assert((mc_popped->type == MENU_TYPE_STATS) || (mc_popped->type == MENU_TYPE_GROWTHS));
    i32 current_id = stats_menu_cycle_inv[mc_popped->type];
    i32 new_id = (current_id == (STATS_MENU_CYCLE_NUM - 1)) ? 0 : ++current_id;
    struct Menu *new_menu_comp;
    switch (stats_menu_cycle[new_id]) {
        case MENU_TYPE_STATS:
            SDL_Log("NEW MENU_TYPE_STATS");
            Game_StatsMenu_Enable(sota, ontile);
            new_menu_comp = TNECS_GET_COMPONENT(sota->world, sota->stats_menu, Menu);
            new_menu_comp->visible = true;
            break;
        case MENU_TYPE_GROWTHS:
            SDL_Log("NEW MENU_TYPE_GROWTHS");
            Game_GrowthsMenu_Enable(sota, ontile);
            new_menu_comp = TNECS_GET_COMPONENT(sota->world, sota->growths_menu, Menu);
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
    if (fsm_eMenuLeft_sGmpMap_ss[sota->substate] != NULL)
        fsm_eMenuLeft_sGmpMap_ss[sota->substate](sota, controller_type);
}

void fsm_eMenuLeft_sPrep(struct Game *sota, i32 controller_type) {
    if (fsm_eMenuLeft_sPrep_ss[sota->substate] != NULL)
        fsm_eMenuLeft_sPrep_ss[sota->substate](sota);
}

void fsm_eMenuLeft_sGmpMap_ssMenu(struct Game *sota, i32 controller_type) {
    /* -- Pop previous menu -- */
    b32 destroy = false;
    tnecs_entity popped = Game_menuStack_Pop(sota, destroy);
    SDL_assert(popped > 0);

    /* - Hide previous menu - */
    struct Menu *mc_popped = TNECS_GET_COMPONENT(sota->world, popped, Menu);
    mc_popped->visible = false;

    /* -- Create New menu -- */
    /* - Get unit ontile - */
    struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    struct Point pos = cursor_pos->tilemap_pos;
    tnecs_entity ontile = sota->map->unitmap[pos.y * sota->map->col_len + pos.x];

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
            new_menu_comp = TNECS_GET_COMPONENT(sota->world, sota->stats_menu, Menu);
            new_menu_comp->visible = true;
            break;
        case MENU_TYPE_GROWTHS:
            Game_GrowthsMenu_Enable(sota, ontile);
            new_menu_comp = TNECS_GET_COMPONENT(sota->world, sota->growths_menu, Menu);
            new_menu_comp->visible = true;
            break;
        default:
            SDL_assert(false);
            break;
            break;
    }
}

void fsm_eMenuLeft_sGmpMap_ssStby(struct Game *sota, i32 controller_type) {

}

void fsm_eMenuLeft_sPrep_ssMenu(struct Game *sota) {
    /* --- Deployment Menu -> Starting Position --- */
    SDL_assert(sota->deployment_menu > TNECS_NULL);
    SDL_assert(sota->menu_stack[0] == sota->deployment_menu);
    SDL_assert(DARR_NUM(sota->menu_stack) == 1);
    DARR_POP(sota->menu_stack);

    strncpy(sota->reason, "Change to map candidates", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_MAP_CANDIDATES, sota->reason);

    /* - Reset potential candidates - */
    sota->candidate     = 0;

    /* - Focus on map - */
    struct Menu *mc;
    mc = TNECS_GET_COMPONENT(sota->world, sota->deployment_menu, Menu);
    mc->visible = false;

    Game_cursorFocus_onMap(sota);
    /* - Set candidates starting positions to dm - */
    SDL_assert(sota->state == GAME_STATE_Preparation);
}

void fsm_eMenuLeft_sPrep_ssMapCndt(struct Game *sota) {
    /* --- Starting Position -> Deployment Menu --- */
    SDL_assert(sota->deployment_menu > TNECS_NULL);
    SDL_assert(DARR_NUM(sota->menu_stack) == 0);

    strncpy(sota->reason, "Change to menu", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->reason);

    /* - Focus on menu - */
    DARR_PUT(sota->menu_stack, sota->deployment_menu);

    struct Menu *mc;
    mc = TNECS_GET_COMPONENT(sota->world, sota->deployment_menu, Menu);
    mc->visible = true;
    Game_cursorFocus_onMenu(sota);

    struct DeploymentMenu *dm = mc->data;
    DeploymentMenu_UnitOrder_Reset(dm);

    /* - Place cursor on dm->elem == map candidate - */
    SDL_assert(sota->state == GAME_STATE_Preparation);
}

void fsm_eCmbtEnd_ssMapNPC(  struct Game *sota) {
    // Make all popups invisible

    /* --- Invisible Popups --- */
    /* -- Popup_Loadout_stats -- */
    tnecs_entity popup_ent = sota->popups[POPUP_TYPE_HUD_LOADOUT_STATS];
    if (popup_ent > TNECS_NULL) {
        struct PopUp *popup_ptr = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
        SDL_assert(popup_ptr != NULL);
        popup_ptr->visible = false;
    }

    /* -- Setting game substate -- */
    strncpy(sota->reason, "Control returns to AI", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_MAP_NPCTURN, sota->reason);
}

void fsm_eCmbtEnd_ssMapCndt( struct Game *sota) {
    // Return to standby substate -> ONLY IF ON FRIENDLY TURN
    *data1_entity = sota->entity_cursor;
    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, NULL, NULL);
}
