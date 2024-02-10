
#include "menu/stats.h"

/* --- STATIC FUNCTIONS DECLARATIONS --- */
/* --- Constructors/Destructors --- */
static void _StatsMenu_Free_Icons(struct StatsMenu *sm);

/* --- Loading --- */
static void _StatsMenu_Load_Icons(struct StatsMenu *sm, SDL_Renderer   *r);

/* --- Drawing --- */
static void _StatsMenu_Draw_Unit(   struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Mount(  struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Weapons(struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Headers(struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Content(struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Scroll_Bar(struct StatsMenu *sm, SDL_Renderer *r);
