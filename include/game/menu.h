#ifndef GAME_MENU_H
#define GAME_MENU_H

#include "tnecs.h"
#include "types.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;
struct Graph;
struct Input_Arguments;

void Game_Switch_toCandidates(  struct Game *sota, 
                                tnecs_E *candidates);

/* --- All Menus --- */
void Game_Menus_Init(struct Game *sota);

/* --- Menu stack --- */
tnecs_E Game_menuStack_Pop( struct Game *sota, b32 destroy);
void    Game_menuStack_Push(struct Game *sota, tnecs_E in);
void    Game_menuStack_Free(struct Game *sota);

/* --- Deployment --- */
void Game_DeploymentMenu_Free(  struct Game *sota);
void Game_DeploymentMenu_Create(struct Game *sota);
void Game_DeploymentMenu_Update(struct Game *sota);
void Game_DeploymentMenu_Enable(struct Game *sota);

/* --- GrowthsMenu --- */
void Game_GrowthsMenu_Create(struct Game *sota);
void Game_GrowthsMenu_Update(struct Game *sota, tnecs_E ent);
void Game_GrowthsMenu_Enable(struct Game *sota, tnecs_E ent);

/* --- StatsMenu --- */
void Game_StatsMenu_Create(struct Game *sota);
void Game_StatsMenu_Update(struct Game *sota, tnecs_E ent);
void Game_StatsMenu_Enable(struct Game *sota, tnecs_E ent);

/* --- TradeMenu --- */
void Game_TradeMenu_Create( struct Game *sota);
void Game_TradeMenu_Update( struct Game *sota,  tnecs_E ent, 
                            tnecs_E      pat);
void Game_TradeMenu_Enable(struct Game  *sota,  tnecs_E ent, 
                            tnecs_E      pat);

/* --- WeaponSelectMenu --- */
void Game_WeaponSelectMenu_Create(struct Game *sota);
void Game_WeaponSelectMenu_Update(struct Game *sota, tnecs_E ent);
void Game_WeaponSelectMenu_Enable(struct Game *sota, tnecs_E ent);

/* --- StaffSelectMenu --- */
void Game_StaffSelectMenu_Create(struct Game *sota);
void Game_StaffSelectMenu_Update(struct Game *sota, tnecs_E ent);
void Game_StaffSelectMenu_Enable(struct Game *sota, tnecs_E ent);

/* --- ItemActionMenu --- */
void Game_ItemActionMenu_Create(struct Game *sota);
void Game_ItemActionMenu_Update(struct Game *sota, tnecs_E ent);
void Game_ItemActionMenu_Enable(struct Game *sota, tnecs_E ent);

/* --- ItemSelectMenu --- */
void Game_ItemSelectMenu_Create(struct Game *sota);
void Game_ItemSelectMenu_Update(struct Game *sota, tnecs_E ent);
void Game_ItemSelectMenu_Enable(struct Game *sota, tnecs_E ent);

/* --- PlayerSelectMenu --- */
void Game_PlayerSelectMenu_Create(struct Game *sota, i8 in_menu);
void Game_PlayerSelectMenu_Update(struct Game *sota, i8 in_menu);
void Game_PlayerSelectMenus_Free( struct Game *sota);
void Game_PlayerSelectMenu_Disable_Entity(struct Game *sota, tnecs_E in_menu_entity);

/* --- Game_preLoadout_... --- */
/* These stop after finding only ONE target. For options in PSM_UNIT_ACTION.
* Used before user chooses unit loadout. */
void Game_preLoadout_Patients(  struct Game *sota, tnecs_E actor);
void Game_preLoadout_Defendants(struct Game *sota, tnecs_E actor);
/* --- Game_preUnitAction_Targets... --- */
/* Does a bunch of pre-computation to find which options go in PSM_UNIT_ACTION. */
void Game_preUnitAction_Targets(struct Game *sota, tnecs_E actor);

/* --- Game_postLoadout_... --- */
/* These find ALL possible targets.
* Used after user chooses unit loadout. */
void Game_postLoadout_Patients(  struct Game *sota, tnecs_E actor);
void Game_postLoadout_Defendants(struct Game *sota, tnecs_E actor);

//  void Game_uniqueMenus_Draw(struct Game * sota);

/* --- Menu location --- */
void Game_Menu_LocationfromUnit(  struct Game *sota, tnecs_E m, tnecs_E u);
void Game_Menu_LocationfromMouse( struct Game *sota, tnecs_E m);
void Game_Menu_LocationfromCursor(struct Game *sota, tnecs_E m);

/* --- Title Screen --- */
void Game_FirstMenu_Create( struct Game *sota);
void Game_FirstMenu_Update( struct Game *sota);
void Game_FirstMenu_Destroy(struct Game *sota);

void Game_Title_Create( struct Game *sota);
void Game_Title_Destroy(struct Game *sota);

void Game_titleScreen_Load( struct Game *sota, 
                            struct Input_Arguments in_args);

#endif /* GAME_MENU_H */
