#ifndef GAME_MENU_H
#define GAME_MENU_H

#include "game/game.h"
#include "game/popup.h"
#include "menu.h"
#include "map/find.h"
#include "map/path.h"
#include "menu/player_select.h"
// #include "menu/item_select.h"
#include "menu/loadout_select.h"
#include "menu/staff_select.h"
#include "menu/trade.h"
#include "menu/stats.h"
#include "title_screen.h"
#include "menu/growths.h"
#include "menu/pre_combat.h"

/* --- All Menus --- */
extern bool Game_Menus_Init(struct Game *sota);
extern void Game_Switch_toCandidates(struct Game *sota, tnecs_entity_t *candidates);

/* --- Menu stack --- */
extern tnecs_entity_t Game_menuStack_Pop(struct Game *sota, bool destroy);
extern void Game_menuStack_Push(struct Game *sota, tnecs_entity_t in_menu_entity);
extern void Game_menuStack_Free(struct Game *sota);

/* --- GrowthsMenu --- */
extern void Game_GrowthsMenu_Create(struct Game *sota);
extern void Game_GrowthsMenu_Update(struct Game *sota, tnecs_entity_t ent);
extern void Game_GrowthsMenu_Enable(struct Game *sota, tnecs_entity_t ent);

/* --- StatsMenu --- */
extern void Game_StatsMenu_Create(struct Game *sota);
extern void Game_StatsMenu_Update(struct Game *sota, tnecs_entity_t ent);
extern void Game_StatsMenu_Enable(struct Game *sota, tnecs_entity_t ent);

/* --- TradeMenu --- */
extern void Game_TradeMenu_Create(struct Game *sota);
extern void Game_TradeMenu_Update(struct Game *sota, tnecs_entity_t ent, tnecs_entity_t pat);
extern void Game_TradeMenu_Enable(struct Game *sota, tnecs_entity_t ent, tnecs_entity_t pat);

/* --- WeaponSelectMenu --- */
extern void Game_WeaponSelectMenu_Create(struct Game *sota);
extern void Game_WeaponSelectMenu_Update(struct Game *sota, tnecs_entity_t ent);
extern void Game_WeaponSelectMenu_Enable(struct Game *sota, tnecs_entity_t ent);

/* --- StaffSelectMenu --- */
extern void Game_StaffSelectMenu_Create(struct Game *sota);
extern void Game_StaffSelectMenu_Update(struct Game *sota, tnecs_entity_t ent);
extern void Game_StaffSelectMenu_Enable(struct Game *sota, tnecs_entity_t ent);

/* --- ItemSelectMenu --- */
extern void Game_ItemSelectMenu_Create(struct Game *sota);
extern void Game_ItemSelectMenu_Update(struct Game *sota, tnecs_entity_t ent);
extern void Game_ItemSelectMenu_Enable(struct Game *sota, tnecs_entity_t ent);

/* --- PlayerSelectMenu --- */
extern void Game_PlayerSelectMenu_Create(struct Game *sota, if8 in_menu);
extern void Game_PlayerSelectMenu_Update(struct Game *sota, if8 in_menu);
extern void Game_PlayerSelectMenus_Free( struct Game *sota);
extern void Game_PlayerSelectMenu_Disable_Entity(struct Game *sota, tnecs_entity_t in_menu_entity);

/* --- Game_preLoadout_... --- */
/* These stop after finding only ONE target. For options in PSM_UNIT_ACTION.
* Used before user chooses unit loadout. */
extern void Game_preLoadout_Patients(  struct Game *sota,    tnecs_entity_t actor);
extern void Game_preLoadout_Defendants(struct Game *sota,  tnecs_entity_t actor);
/* --- Game_preUnitAction_Targets... --- */
/* Does a bunch of pre-computation to find which options go in PSM_UNIT_ACTION. */
extern void Game_preUnitAction_Targets(struct Game *sota,  tnecs_entity_t actor);

/* --- Game_postLoadout_... --- */
/* These find ALL possible targets.
* Used after user chooses unit loadout. */
extern void Game_postLoadout_Patients(  struct Game *sota,   tnecs_entity_t actor);
extern void Game_postLoadout_Defendants(struct Game *sota, tnecs_entity_t actor);

// extern void Game_uniqueMenus_Draw(struct Game * sota);

/* --- Menu location --- */
extern void Game_Menu_LocationfromUnit(  struct Game *sota, tnecs_entity_t m, tnecs_entity_t u);
extern void Game_Menu_LocationfromMouse( struct Game *sota, tnecs_entity_t m);
extern void Game_Menu_LocationfromCursor(struct Game *sota, tnecs_entity_t m);

/* --- Title Screen --- */
extern void Game_FirstMenu_Create( struct Game *sota);
extern void Game_FirstMenu_Update( struct Game *sota);
extern void Game_FirstMenu_Destroy(struct Game *sota);

extern void Game_Title_Create( struct Game *sota);
extern void Game_Title_Destroy(struct Game *sota);

extern void Game_titleScreen_Load(struct Game *sota, struct Input_Arguments in_args);

#endif /* GAME_MENU_H */
