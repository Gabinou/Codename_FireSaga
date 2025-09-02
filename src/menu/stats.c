
#include "menu/stats.h"
#include "unit/equipment.h"
#include "bars/simple.h"
#include "bars/stat.h"
#include "unit/status.h"
#include "unit/mount.h"
#include "pixelfonts.h"
#include "filesystem.h"
#include "platform.h"
#include "weapon.h"
#include "globals.h"
#include "utilities.h"
#include "nmath.h"
#include "macros.h"
#include "names.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/stats.h"
#include "unit/loadout.h"
#include "unit/equipment.h"
#include "stb_sprintf.h"

/* --- STATIC FUNCTIONS DECLARATIONS --- */
/* --- Constructors/Destructors --- */
static void _StatsMenu_Free_Face( struct StatsMenu *sm);
static void _StatsMenu_Free_Icons(struct StatsMenu *sm);

/* --- Loading --- */
static void _StatsMenu_Load_Face( struct StatsMenu *sm);
static void _StatsMenu_Load_Icons(struct StatsMenu *sm, SDL_Renderer   *r);

/* --- Drawing --- */
static void _StatsMenu_Draw_Item(         struct StatsMenu *sm, SDL_Renderer *r, int i);
static void _StatsMenu_Draw_Name(         struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Mount(        struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Stats(        struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Hands(        struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Rescue(       struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Skills(       struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Statuses(     struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_WpnTypes(     struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Equipment(    struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_ComputedStats(struct StatsMenu *sm, SDL_Renderer *r);

const i8 stats_menu_cycle[STATS_MENU_CYCLE_NUM] = {MENU_TYPE_STATS, MENU_TYPE_GROWTHS};
const i8 stats_menu_cycle_inv[MENU_TYPE_END] = {
    /* NULL */          -1,
    /* PLAYER_SELECT */ -1,
    /* ISM */           -1,
    /* TEXT_ICONS */    -1,
    /* STATS */          0,
    /* RESCUE */        -1,
    /* SUPPORTS */      -1,
    /* GROWTHS */        1,
    /* TILE_HUD */      -1,
    /* TILE_COSTS */    -1,
    /* PRE_COMBAT */    -1
};

const int status_offset_x[UNIT_STATUS_NUM] = {
    /* NULL     */  0,
    /* POISONED */ -5,
    /* STONE    */ -4,
    /* ROOTED   */ -4,
    /* STUNNED  */ -2,
    /* SEDUCED  */ -2,
    /* SLOWED   */ -2,
    /* TERROR   */ -4,
    /* BERSERK  */ -5,
    /* STASIS   */ -5,
    /* SILENCE  */ -3,
};

const int status_offset_y[UNIT_STATUS_NUM] = {
    /* NULL     */  0,
    /* POISONED */  0,
    /* STONE    */  0,
    /* ROOTED   */  0,
    /* STUNNED  */  0,
    /* SEDUCED  */  1,
    /* SLOWED   */  0,
    /* TERROR   */  0,
    /* BERSERK  */  0,
    /* STASIS   */  0,
    /* SILENCE  */  0,
};

/* --- STATS_MENU --- */
const char stats_menu_description[SM_ELEM_NUM][STATSMENU_BUFFER_LEN] = {
    /* SM_ELEM_SEX */ {"Sex: Male, Female, Unknown."},// TODO: automatic sex?
    /* SM_ELEM_TITLE */ {"Title: Indicates Nobility, denotes feats or a popular nickname"}, // TODO: automatic title
    /* SM_ELEM_NAME */ {"Unit Name"},
    /* SM_ELEM_CLASS */ {"Unit class: "}, // TODO: automatic skill description
    /* SM_ELEM_PORTRAIT */ {""}, // TODO: automatic unit description
    /* SM_ELEM_HP */ {"Hit Points. Unit agonizes if drops to 0 by normal hit. If crit, unit dies."},
    /* SM_ELEM_EXP */       {"Current Exp. Unit levels up at 100 Exp."},
    /* SM_ELEM_LVL */       {"Current Unit level. Maximum Level: 40"},
    /* SM_ELEM_MOVE */      {"Maximum Movement range on normal terrain"},
    /* SM_ELEM_MOUNT */     {"Mount assigned to Unit:"}, // todo put Mount name and type
    /* SM_ELEM_WEAPONS */   {"Usable weapon types"},
    /* SM_ELEM_RESCUE */    {"Currently rescuing unit:"},
    /* SM_ELEM_REGRETS */   {"For every 1 regret above FTH, get -1/0 Hit -1/0 Crit"},
    /* SM_ELEM_STATUS */    {"Status Ailment and turns remaining"},
    /* SM_ELEM_SKILLS */ {""}, // TODO: automatic skill description
    /* SM_ELEM_SKILL1 */ {""},
    /* SM_ELEM_SKILL2 */ {""},
    /* SM_ELEM_SKILL3 */ {""},
    /* SM_ELEM_SKILL4 */ {""},
    /* SM_ELEM_SKILL5 */ {""},
    /* SM_ELEM_STR */ {"Strength: 1 point -> ATK up 1/0/0 with physical weapon, "STRINGIZE(SPEED_STR_FACTOR)" points -> weapon weight down 1"},
    /* SM_ELEM_DEX */ {"Dexterity: 1 point increases HIT by "STRINGIZE(HIT_DEX_FACTOR)"/0, " STRINGIZE(CRIT_DEX_FACTOR) " points -> CRIT up 1/0"},
    /* SM_ELEM_DEF */ {"Defense: 1 point -> PROT up 1/0"},
    /* SM_ELEM_FTH */ {"Faith: 1 point -> resists 1 regret, "STRINGIZE(DODGE_FTH_FACTOR)" points -> HIT up 0/1, " STRINGIZE(FAVOR_FTH_FACTOR)" points -> CRIT up 0/1 "},
    /* SM_ELEM_CON */ {"itution: Units with lower con rescueable, "STRINGIZE(SPEED_CON_FACTOR)" points -> weapon weight down 1"},
    /* SM_ELEM_MAG */ {"Magic: 1 point -> ATK up 0/1/0 with magic weapon"},
    /* SM_ELEM_AGI */ {"Agility: 1 point -> SPEED up 1"},
    /* SM_ELEM_RES */ {"Resistance: 1 point -> DEF up 0/1"},
    /* SM_ELEM_LUCK */ {"Luck: " STRINGIZE(HIT_LUCK_FACTOR)" points -> HIT up 1/0, " STRINGIZE(DODGE_LUCK_FACTOR)" points -> HIT up 0/1, " STRINGIZE(CRIT_LUCK_FACTOR)" points -> CRIT up 0/1"},
    /* SM_ELEM_PROF */ {"Proficiency: Can use weapons with PROF equal or lower"},
    /* SM_ELEM_ITEM1 */ {""},
    /* SM_ELEM_ITEM2 */ {""},
    /* SM_ELEM_ITEM3 */ {""},
    /* SM_ELEM_ITEM4 */ {""},
    /* SM_ELEM_ITEM5 */ {""},
    /* SM_ELEM_ITEM6 */ {""},
    /* SM_ELEM_HAND1 */ {""},
    /* SM_ELEM_HAND2 */ {""},
    /* SM_ELEM_ATK */ {"Computed Attack: Physical/Magical/True. Damage is Attack - Defense. "},
    /* SM_ELEM_PROT */ {"Computed Defence: Physical/Magical "},
    /* SM_ELEM_RANGE */ {"Computed Range with currently equipped weapons: Minimum - Maximum"},
    /* SM_ELEM_HIT */ {"HIT rates: hit/dodge"},
    /* SM_ELEM_CRIT */ {"CRIT rates: crit/favor"},
    /* SM_ELEM_SPEED */ {"Speed. If defender speed is 5 or less than attacker's, attacker doubles. Weapon weight 1 -> SPEED down 1 "},
};

const struct Point stats_menu_cursor_box[SM_ELEM_NUM] = {
    /* SM_ELEM_SEX */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_TITLE */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_NAME */      {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_CLASS */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_PORTRAIT */  {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_HP */        {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_EXP */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_LVL */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_MOVE */      {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_MOUNT */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_WEAPONS */   {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_RESCUE */    {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_REGRETS */   {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_STATUS */    {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_SKILLS */    {SOTA_TILESIZE, SOTA_TILESIZE / 2},
    /* SM_ELEM_SKILL1 */    {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_SKILL2 */    {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_SKILL3 */    {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_SKILL4 */    {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_SKILL5 */    {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_STR */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_DEX */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_DEF */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_FTH */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_CON */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_MAG */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_AGI */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_RES */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_LUCK */      {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_PROF */      {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_ITEM1 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_ITEM2 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_ITEM3 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_ITEM4 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_ITEM5 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_ITEM6 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_HAND1 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_HAND2 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_ATK */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_PROT */      {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_RANGE */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_HIT */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_CRIT */      {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_SPEED */     {SOTA_TILESIZE, SOTA_TILESIZE},
};

const struct Point stats_menu_cursor_pos[SM_ELEM_NUM] = {
    /* SM_ELEM_SEX */           {SEX_ICON_X_OFFSET,  SEX_ICON_Y_OFFSET},
    /* SM_ELEM_TITLE */         {TITLE_X_OFFSET,     TITLE_Y_OFFSET},
    /* SM_ELEM_NAME */          {NAME_X_OFFSET,      NAME_Y_OFFSET},
    /* SM_ELEM_CLASS */         {CLASS_X_OFFSET,     CLASS_Y_OFFSET},
    /* SM_ELEM_PORTRAIT */      {PORTRAIT_X_OFFSET,  PORTRAIT_Y_OFFSET},
    /* SM_ELEM_HP */            {HP_X_OFFSET,        HP_Y_OFFSET},
    /* SM_ELEM_EXP */           {EXP_X_OFFSET,       EXP_Y_OFFSET},
    /* SM_ELEM_LVL */           {LV_X_OFFSET,        LV_Y_OFFSET},
    /* SM_ELEM_MOVE */          {MOVE_X_OFFSET,      MOVE_Y_OFFSET},
    /* SM_ELEM_MOUNT */         {MOUNT_X_OFFSET,     MOUNT_Y_OFFSET},
    /* SM_ELEM_WEAPONS */       {WEAPONS_X_OFFSET,   WEAPONS_Y_OFFSET},
    /* SM_ELEM_RESCUE */        {RESCUE_X_OFFSET,    RESCUE_Y_OFFSET},
    /* SM_ELEM_REGRETS */       {REGRETS_X_OFFSET,   REGRETS_Y_OFFSET},
    /* SM_ELEM_STATUS */        {STATUSES_X_OFFSET,  STATUSES_Y_OFFSET},
    /* SM_ELEM_SKILLS */        {REGRETS_X_OFFSET,   SKILLS_Y_OFFSET},
    /* SM_ELEM_SKILL1 */        {REGRETS_X_OFFSET,   SKILL1_Y_OFFSET},
    /* SM_ELEM_SKILL2 */        {SKILL2_X_OFFSET,    SKILL2_Y_OFFSET},
    /* SM_ELEM_SKILL3 */        {SKILL3_X_OFFSET,    SKILL3_Y_OFFSET},
    /* SM_ELEM_SKILL4 */        {SKILL4_X_OFFSET,    SKILL4_Y_OFFSET},
    /* SM_ELEM_SKILL5 */        {SKILL5_X_OFFSET,    SKILL5_Y_OFFSET},
    /* SM_ELEM_STR */           {STR_X_OFFSET,       STR_Y_OFFSET},
    /* SM_ELEM_DEX */           {DEX_X_OFFSET,       DEX_Y_OFFSET},
    /* SM_ELEM_DEF */           {DEF_X_OFFSET,       DEF_Y_OFFSET},
    /* SM_ELEM_FTH */           {FTH_X_OFFSET,       FTH_Y_OFFSET},
    /* SM_ELEM_CON */           {CON_X_OFFSET,       CON_Y_OFFSET},
    /* SM_ELEM_MAG */           {MAG_X_OFFSET,       MAG_Y_OFFSET},
    /* SM_ELEM_AGI */           {AGI_X_OFFSET,       AGI_Y_OFFSET},
    /* SM_ELEM_RES */           {RES_X_OFFSET,       RES_Y_OFFSET},
    /* SM_ELEM_LUCK */          {LUCK_X_OFFSET,      LUCK_Y_OFFSET},
    /* SM_ELEM_PROF */          {PROF_X_OFFSET,      PROF_Y_OFFSET},
    /* SM_ELEM_ITEM1 */         {ITEM1_X_OFFSET,     ITEM1_Y_OFFSET},
    /* SM_ELEM_ITEM2 */         {ITEM2_X_OFFSET,     ITEM2_Y_OFFSET},
    /* SM_ELEM_ITEM3 */         {ITEM3_X_OFFSET,     ITEM3_Y_OFFSET},
    /* SM_ELEM_ITEM4 */         {ITEM4_X_OFFSET,     ITEM4_Y_OFFSET},
    /* SM_ELEM_ITEM5 */         {ITEM5_X_OFFSET,     ITEM5_Y_OFFSET},
    /* SM_ELEM_ITEM6 */         {ITEM6_X_OFFSET,     ITEM6_Y_OFFSET},
    /* SM_ELEM_HAND1 */         {0,                  0},
    /* SM_ELEM_HAND2 */         {0,                  0},
    /* SM_ELEM_ATK */           {ATK_X_OFFSET   - 6, ATK_Y_OFFSET},
    /* SM_ELEM_PROT */          {PROT_X_OFFSET  - 4, PROT_Y_OFFSET},
    /* SM_ELEM_RANGE */         {RANGE_X_OFFSET,     RANGE_Y_OFFSET},
    /* SM_ELEM_HIT */           {HIT_X_OFFSET   - 5, HIT_Y_OFFSET},
    /* SM_ELEM_CRIT */          {CRIT_X_OFFSET  - 3, CRIT_Y_OFFSET},
    /* SM_ELEM_SPEED */         {SPEED_X_OFFSET,     SPEED_Y_OFFSET},
};

struct Point stats_menu_elem_box[SM_ELEM_NUM] = {
    /* SM_ELEM_SEX */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_TITLE */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_NAME */      {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_CLASS */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_PORTRAIT */  {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_HP */        {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_EXP */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_LVL */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_MOVE */      {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_MOUNT */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_WEAPONS */   {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_RESCUE */    {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_REGRETS */   {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_STATUS */    {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_SKILLS */    {SOTA_TILESIZE, SOTA_TILESIZE / 2},
    /* SM_ELEM_SKILL1 */    {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_SKILL2 */    {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_SKILL3 */    {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_SKILL4 */    {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_SKILL5 */    {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_STR */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_DEX */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_DEF */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_FTH */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_CON */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_MAG */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_AGI */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_RES */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_LUCK */      {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_PROF */      {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_ITEM1 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_ITEM2 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_ITEM3 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_ITEM4 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_ITEM5 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_ITEM6 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_HAND1 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_HAND2 */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_ATK */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_PROT */      {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_RANGE */     {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_HIT */       {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_CRIT */      {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SM_ELEM_SPEED */     {SOTA_TILESIZE, SOTA_TILESIZE},
};
const struct Point stats_menu_elem_pos[STATS_MENU_ELEMS_NUM] = {0};

struct Point sm_elem_pos[SM_ELEM_NUM] = {
    /* SM_ELEM_SEX */       {SEX_ICON_X_OFFSET, SEX_ICON_Y_OFFSET},
    /* SM_ELEM_TITLE */     {TITLE_X_OFFSET,     TITLE_Y_OFFSET},
    /* SM_ELEM_NAME */      {NAME_X_OFFSET,      NAME_Y_OFFSET},
    /* SM_ELEM_CLASS */     {CLASS_X_OFFSET,     CLASS_Y_OFFSET},
    /* SM_ELEM_PORTRAIT */  {PORTRAIT_X_OFFSET,  PORTRAIT_Y_OFFSET},
    /* SM_ELEM_HP */        {HP_X_OFFSET,        HP_Y_OFFSET},
    /* SM_ELEM_EXP */       {EXP_X_OFFSET,       EXP_Y_OFFSET},
    /* SM_ELEM_LVL */       {LV_X_OFFSET,        LV_Y_OFFSET},
    /* SM_ELEM_MOVE */      {MOVE_X_OFFSET,      MOVE_Y_OFFSET},
    /* SM_ELEM_MOUNT */     {MOUNT_X_OFFSET,     MOUNT_Y_OFFSET},
    /* SM_ELEM_WEAPONS */   {WEAPONS_X_OFFSET,   WEAPONS_Y_OFFSET},
    /* SM_ELEM_RESCUE */    {RESCUE_X_OFFSET,    RESCUE_Y_OFFSET},
    /* SM_ELEM_REGRETS */   {REGRETS_X_OFFSET,   REGRETS_Y_OFFSET},
    /* SM_ELEM_STATUS */    {STATUSES_X_OFFSET,  STATUSES_Y_OFFSET},
    /* SM_ELEM_SKILLS */    {REGRETS_X_OFFSET,   SKILLS_Y_OFFSET},
    /* SM_ELEM_SKILL1 */    {REGRETS_X_OFFSET,   SKILL1_Y_OFFSET},
    /* SM_ELEM_SKILL2 */    {SKILL2_X_OFFSET,    SKILL2_Y_OFFSET},
    /* SM_ELEM_SKILL3 */    {SKILL3_X_OFFSET,    SKILL3_Y_OFFSET},
    /* SM_ELEM_SKILL4 */    {SKILL4_X_OFFSET,    SKILL4_Y_OFFSET},
    /* SM_ELEM_SKILL5 */    {SKILL5_X_OFFSET,    SKILL5_Y_OFFSET},
    /* SM_ELEM_STR */       {STR_X_OFFSET,       STR_Y_OFFSET},
    /* SM_ELEM_DEX */       {DEX_X_OFFSET,       DEX_Y_OFFSET},
    /* SM_ELEM_DEF */       {DEF_X_OFFSET,       DEF_Y_OFFSET},
    /* SM_ELEM_FTH */       {FTH_X_OFFSET,       FTH_Y_OFFSET},
    /* SM_ELEM_CON */       {CON_X_OFFSET,       CON_Y_OFFSET},
    /* SM_ELEM_MAG */       {MAG_X_OFFSET,       MAG_Y_OFFSET},
    /* SM_ELEM_AGI */       {AGI_X_OFFSET,       AGI_Y_OFFSET},
    /* SM_ELEM_RES */       {RES_X_OFFSET,       RES_Y_OFFSET},
    /* SM_ELEM_LUCK */      {LUCK_X_OFFSET,      LUCK_Y_OFFSET},
    /* SM_ELEM_PROF */      {PROF_X_OFFSET,      PROF_Y_OFFSET},
    /* SM_ELEM_ITEM1 */     {ITEM1_X_OFFSET,     ITEM1_Y_OFFSET},
    /* SM_ELEM_ITEM2 */     {ITEM2_X_OFFSET,     ITEM2_Y_OFFSET},
    /* SM_ELEM_ITEM3 */     {ITEM3_X_OFFSET,     ITEM3_Y_OFFSET},
    /* SM_ELEM_ITEM4 */     {ITEM4_X_OFFSET,     ITEM4_Y_OFFSET},
    /* SM_ELEM_ITEM5 */     {ITEM5_X_OFFSET,     ITEM5_Y_OFFSET},
    /* SM_ELEM_ITEM6 */     {ITEM6_X_OFFSET,     ITEM6_Y_OFFSET},
    // todo: COMPUTE HAND POSITION PROCEDURALLY
    /* SM_ELEM_HAND1 */     {0,                  0},
    /* SM_ELEM_HAND2 */     {0,                  0},
    /* SM_ELEM_ATK */       {ATK_X_OFFSET   - 6, ATK_Y_OFFSET},
    /* SM_ELEM_PROT */      {PROT_X_OFFSET  - 4, PROT_Y_OFFSET},
    /* SM_ELEM_RANGE */     {RANGE_X_OFFSET,     RANGE_Y_OFFSET},
    /* SM_ELEM_HIT */       {HIT_X_OFFSET   - 5, HIT_Y_OFFSET},
    /* SM_ELEM_CRIT */      {CRIT_X_OFFSET  - 3, CRIT_Y_OFFSET},
    /* SM_ELEM_SPEED */     {SPEED_X_OFFSET,     SPEED_Y_OFFSET},
};

struct n4Directions sm_links[SM_ELEM_NUM] = {
    /*right, top, left, bottom */
    /* SM_ELEM_SEX */        {SM_ELEM_NAME,     SM_ELEM_NULL,     SM_ELEM_NULL,    SM_ELEM_TITLE},
    /* SM_ELEM_TITLE */      {SM_ELEM_PORTRAIT, SM_ELEM_SEX,      SM_ELEM_NULL,    SM_ELEM_CLASS},
    /* SM_ELEM_NAME */       {SM_ELEM_PORTRAIT, SM_ELEM_NULL,     SM_ELEM_SEX,     SM_ELEM_TITLE},
    /* SM_ELEM_CLASS */      {SM_ELEM_PORTRAIT, SM_ELEM_TITLE,    SM_ELEM_NULL,    SM_ELEM_HP},
    /* SM_ELEM_PORTRAIT */   {SM_ELEM_ATK,      SM_ELEM_NULL,     SM_ELEM_HP,      SM_ELEM_RESCUE},
    /* SM_ELEM_HP */         {SM_ELEM_PORTRAIT, SM_ELEM_CLASS,    SM_ELEM_NULL,    SM_ELEM_EXP},
    /* SM_ELEM_EXP */        {SM_ELEM_LVL,      SM_ELEM_HP,       SM_ELEM_NULL,    SM_ELEM_REGRETS},
    /* SM_ELEM_LVL */        {SM_ELEM_MOVE,     SM_ELEM_HP,       SM_ELEM_EXP,     SM_ELEM_REGRETS},
    /* SM_ELEM_MOVE */       {SM_ELEM_PORTRAIT, SM_ELEM_HP,       SM_ELEM_LVL,     SM_ELEM_WEAPONS},
    /* SM_ELEM_MOUNT */      {SM_ELEM_STATUS,   SM_ELEM_CRIT,     SM_ELEM_RESCUE,  SM_ELEM_ITEM1},
    /* SM_ELEM_WEAPONS */    {SM_ELEM_RESCUE,   SM_ELEM_MOVE,     SM_ELEM_REGRETS, SM_ELEM_SKILLS},
    /* SM_ELEM_RESCUE */     {SM_ELEM_MOUNT,    SM_ELEM_PORTRAIT, SM_ELEM_WEAPONS, SM_ELEM_ITEM1},
    /* SM_ELEM_REGRETS */    {SM_ELEM_WEAPONS,  SM_ELEM_EXP,      SM_ELEM_NULL,    SM_ELEM_SKILLS},
    /* SM_ELEM_STATUS */     {SM_ELEM_NULL,     SM_ELEM_SPEED,    SM_ELEM_MOUNT,   SM_ELEM_ITEM1},
    /* SM_ELEM_SKILLS */     {SM_ELEM_RESCUE,   SM_ELEM_REGRETS,  SM_ELEM_NULL,    SM_ELEM_SKILL3},
    /* SM_ELEM_SKILL1 */     {SM_ELEM_SKILL2,   SM_ELEM_SKILLS,   SM_ELEM_NULL,    SM_ELEM_STR},
    /* SM_ELEM_SKILL2 */     {SM_ELEM_SKILL3,   SM_ELEM_SKILLS,   SM_ELEM_SKILL1,  SM_ELEM_STR},
    /* SM_ELEM_SKILL3 */     {SM_ELEM_SKILL4,   SM_ELEM_SKILLS,   SM_ELEM_SKILL2,  SM_ELEM_STR},
    /* SM_ELEM_SKILL4 */     {SM_ELEM_SKILL5,   SM_ELEM_SKILLS,   SM_ELEM_SKILL3,  SM_ELEM_MAG},
    /* SM_ELEM_SKILL5 */     {SM_ELEM_ITEM1,    SM_ELEM_SKILLS,   SM_ELEM_SKILL4,  SM_ELEM_MAG},
    /* SM_ELEM_STR */        {SM_ELEM_MAG,      SM_ELEM_SKILL1,   SM_ELEM_NULL,    SM_ELEM_DEX},
    /* SM_ELEM_DEX */        {SM_ELEM_AGI,      SM_ELEM_STR,      SM_ELEM_NULL,    SM_ELEM_DEF},
    /* SM_ELEM_DEF */        {SM_ELEM_RES,      SM_ELEM_DEX,      SM_ELEM_NULL,    SM_ELEM_FTH},
    /* SM_ELEM_FTH */        {SM_ELEM_LUCK,     SM_ELEM_DEF,      SM_ELEM_NULL,    SM_ELEM_CON},
    /* SM_ELEM_CON */        {SM_ELEM_PROF,     SM_ELEM_FTH,      SM_ELEM_NULL,    SM_ELEM_NULL},
    /* SM_ELEM_MAG */        {SM_ELEM_ITEM2,    SM_ELEM_SKILL3,   SM_ELEM_STR,     SM_ELEM_AGI},
    /* SM_ELEM_AGI */        {SM_ELEM_ITEM3,    SM_ELEM_MAG,      SM_ELEM_DEX,     SM_ELEM_RES},
    /* SM_ELEM_RES */        {SM_ELEM_ITEM4,    SM_ELEM_AGI,      SM_ELEM_DEF,     SM_ELEM_LUCK},
    /* SM_ELEM_LUCK */       {SM_ELEM_ITEM5,    SM_ELEM_RES,      SM_ELEM_FTH,     SM_ELEM_PROF},
    /* SM_ELEM_PROF */       {SM_ELEM_ITEM6,    SM_ELEM_LUCK,     SM_ELEM_CON,     SM_ELEM_NULL},
    /* SM_ELEM_ITEM1 */      {SM_ELEM_HAND1,    SM_ELEM_MOUNT,    SM_ELEM_SKILL5,  SM_ELEM_ITEM2},
    /* SM_ELEM_ITEM2 */      {SM_ELEM_HAND2,    SM_ELEM_ITEM1,    SM_ELEM_MAG,     SM_ELEM_ITEM3},
    /* SM_ELEM_ITEM3 */      {SM_ELEM_NULL,     SM_ELEM_ITEM2,    SM_ELEM_AGI,     SM_ELEM_ITEM4},
    /* SM_ELEM_ITEM4 */      {SM_ELEM_NULL,     SM_ELEM_ITEM3,    SM_ELEM_RES,     SM_ELEM_ITEM5},
    /* SM_ELEM_ITEM5 */      {SM_ELEM_NULL,     SM_ELEM_ITEM4,    SM_ELEM_LUCK,    SM_ELEM_ITEM6},
    /* SM_ELEM_ITEM6 */      {SM_ELEM_NULL,     SM_ELEM_ITEM5,    SM_ELEM_PROF,    SM_ELEM_NULL},
    /* SM_ELEM_HAND1 */      {SM_ELEM_NULL,     SM_ELEM_STATUS,   SM_ELEM_ITEM1,   SM_ELEM_HAND2},
    /* SM_ELEM_HAND2 */      {SM_ELEM_NULL,     SM_ELEM_HAND1,    SM_ELEM_ITEM2,   SM_ELEM_ITEM3},
    /* SM_ELEM_ATK */        {SM_ELEM_HIT,      SM_ELEM_NULL,     SM_ELEM_PORTRAIT, SM_ELEM_PROT},
    /* SM_ELEM_PROT */       {SM_ELEM_CRIT,     SM_ELEM_ATK,      SM_ELEM_PORTRAIT, SM_ELEM_MOUNT},
    /* SM_ELEM_RANGE */      {SM_ELEM_NULL,     SM_ELEM_NULL,     SM_ELEM_HIT,     SM_ELEM_SPEED},
    /* SM_ELEM_HIT */        {SM_ELEM_RANGE,    SM_ELEM_NULL,     SM_ELEM_ATK,     SM_ELEM_CRIT},
    /* SM_ELEM_CRIT */       {SM_ELEM_SPEED,    SM_ELEM_HIT,      SM_ELEM_PROT,    SM_ELEM_MOUNT},
    /* SM_ELEM_SPEED */      {SM_ELEM_NULL,     SM_ELEM_RANGE,    SM_ELEM_CRIT,    SM_ELEM_STATUS},
};

const struct StatsMenu StatsMenu_default = {
    .update             = true,
    .update_stats       = true,
};

struct StatsMenu *StatsMenu_Alloc(void) {
    struct StatsMenu *stats_menu = SDL_malloc(sizeof(struct StatsMenu));
    SDL_assert(stats_menu);
    *stats_menu = StatsMenu_default;
    SDL_assert(stats_menu->texture == NULL);
    return (stats_menu);
}

void StatsMenu_Free(struct StatsMenu *stats_menu) {
    if (stats_menu == NULL) {
        return;
    }
    SDL_assert(stats_menu != NULL);
    if (stats_menu->texture != NULL) {
        SDL_DestroyTexture(stats_menu->texture);
        stats_menu->texture = NULL;
    }
    _StatsMenu_Free_Face(stats_menu);
    _StatsMenu_Free_Icons(stats_menu);
    if (stats_menu->font_wpns != NULL) {
        PixelFont_Free(stats_menu->font_wpns, true);
        stats_menu->font_wpns = NULL;
    }

    SDL_free(stats_menu);
}

void StatsMenu_Load(struct StatsMenu *stats_menu, struct Unit *unit,
                    SDL_Renderer *renderer, struct n9Patch *n9patch) {
    SDL_assert(stats_menu       != NULL);
    SDL_assert(unit             != NULL);
    SDL_assert(gl_weapons_dtab  != NULL);

    stats_menu->unit    = unit;
    stats_menu->update  = true;
    _StatsMenu_Load_Face(stats_menu);
    _StatsMenu_Load_Icons(stats_menu, renderer);

    n9patch->px.x  = MENU_PATCH_PIXELS;
    n9patch->px.y  = MENU_PATCH_PIXELS;
    n9patch->num.x  = STATS_MENU_PATCH_X_SIZE;
    n9patch->num.y  = STATS_MENU_PATCH_Y_SIZE;
    n9patch->scale.x         = STATS_MENU_N9PATCH_SCALE_X;
    n9patch->scale.y         = STATS_MENU_N9PATCH_SCALE_Y;

    Point size = {
        .x  = (MENU_PATCH_PIXELS * STATS_MENU_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * STATS_MENU_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9patch, size);

    if (n9patch->texture == NULL) {
        char *path       = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }

}

static void _StatsMenu_Free_Face(struct StatsMenu *stats_menu) {
    if (stats_menu->texture_face != NULL) {
        SDL_DestroyTexture(stats_menu->texture_face);
        stats_menu->texture_face = NULL;
    }
}

static void _StatsMenu_Load_Face(struct StatsMenu *stats_menu) {
    SDL_assert(stats_menu);
    SDL_assert(stats_menu->unit);
    // TODO: find face file from unit name
}

static void _StatsMenu_Free_Icons(struct StatsMenu *stats_menu) {
    if (stats_menu->texture_sex != NULL) {
        SDL_DestroyTexture(stats_menu->texture_sex);
        stats_menu->texture_sex = NULL;
    }
    if (stats_menu->texture_mount != NULL) {
        SDL_DestroyTexture(stats_menu->texture_mount);
        stats_menu->texture_mount = NULL;
    }
    if (stats_menu->texture_hands != NULL) {
        SDL_DestroyTexture(stats_menu->texture_hands);
        stats_menu->texture_hands = NULL;
    }
    if (stats_menu->texture_statuses != NULL) {
        SDL_DestroyTexture(stats_menu->texture_statuses);
        stats_menu->texture_statuses = NULL;
    }
}

static void _StatsMenu_Load_Icons(struct StatsMenu *stats_menu, SDL_Renderer *renderer) {
    SDL_assert(stats_menu);
    SDL_assert(renderer != NULL);

    char *path = PATH_JOIN("..", "assets", "GUI", "Menu", "StatsMenu_Icons_Sex.png");
    stats_menu->texture_sex = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(stats_menu->texture_sex);

    path = PATH_JOIN("..", "assets", "GUI", "Menu", "StatsMenu_Icons_Mount.png");
    stats_menu->texture_mount = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(stats_menu->texture_mount);

    /* Load weapon icons as a pixelfont to write them with spacing easily. */
    path = PATH_JOIN("..", "assets", "GUI", "Icon", "StatsMenu_Icons_Weapons.png");
    stats_menu->font_wpns = TextureFont_Alloc(2, 8);
    PixelFont_Load(stats_menu->font_wpns, renderer, path);

    path = PATH_JOIN("..", "assets", "GUI", "Menu", "StatsMenu_Icons_Hands.png");
    stats_menu->texture_hands = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(stats_menu->texture_hands);

    path = PATH_JOIN("..", "assets", "GUI", "Menu", "StatsMenu_Icons_Statuses.png");
    stats_menu->texture_statuses = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(stats_menu->texture_statuses);

}

/* --- Positioning --- */
void StatsMenu_Elem_Pos(struct StatsMenu *sm, struct Menu *mc) {
    /* Scales elem_pos to menu size. */
    /* 1. Makes the cursor focus on right place on the Screen       */
    /* 2. Box lined are drawn in menu frame, making thinner lines   */

    /* - Skip if already in screen frame - */
    if (mc->elem_pos_frame == ELEM_POS_SCREEN_FRAME) {
        return;
    }

    for (size_t i = 0; i < mc->elem_num; i++) {
        int scale_x = mc->n9patch.scale.x;
        int scale_y = mc->n9patch.scale.y;
        int x       = sm->pos.x + mc->n9patch.pos.x;
        int y       = sm->pos.y;
        int elem_x  = mc->elem_pos[i].x;
        int elem_y  = mc->elem_pos[i].y;
        mc->elem_pos[i].x = x + elem_x * scale_x;
        mc->elem_pos[i].y = y + elem_y * scale_y;
    }

    mc->elem_pos_frame = ELEM_POS_SCREEN_FRAME;
}

void StatsMenu_Elem_Pos_Revert(struct StatsMenu *sm, struct Menu *mc) {
    /* Reset elem_pos to menu size. */

    /* - Skip if already in menu frame - */
    if (mc->elem_pos_frame == ELEM_POS_MENU_FRAME) {
        return;
    }

    for (size_t i = 0; i < mc->elem_num; i++) {
        int scale_x = mc->n9patch.scale.x;
        int scale_y = mc->n9patch.scale.y;
        int x       = sm->pos.x + mc->n9patch.pos.x;
        int y       = sm->pos.y;
        int elem_x  = mc->elem_pos[i].x;
        int elem_y  = mc->elem_pos[i].y;
        mc->elem_pos[i].x = (elem_x - x) / scale_x;
        mc->elem_pos[i].y = (elem_y - y) / scale_y;
    }

    mc->elem_pos_frame = ELEM_POS_MENU_FRAME;
}

/* --- Drawing --- */
static void _StatsMenu_Draw_Name(struct StatsMenu *stats_menu, SDL_Renderer *renderer) {
    /* -- HEADER WRITING -- */
    SDL_Rect dstrect, srcrect;
    struct Unit_stats effective_stats = Unit_effectiveStats(stats_menu->unit);
    char numbuff[10];
    int x, y;

    /* - sex icon - */
    SDL_assert(stats_menu->texture_sex != NULL);
    srcrect.w = SM_SEX_ICON_SIZEX;
    srcrect.h = SM_SEX_ICON_SIZEY;
    srcrect.x = Unit_Sex(stats_menu->unit) * SM_SEX_TILESIZE;
    srcrect.y = 0;
    dstrect.w = srcrect.w;
    dstrect.h = srcrect.h;
    dstrect.x = SEX_ICON_X_OFFSET;
    dstrect.y = SEX_ICON_Y_OFFSET;
    SDL_RenderCopy(renderer, stats_menu->texture_sex, &srcrect, &dstrect);
    /* - Portrait/Face - */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    srcrect.w = PORTRAIT_W;
    srcrect.h = PORTRAIT_H;
    srcrect.x = PORTRAIT_X_OFFSET;
    srcrect.y = PORTRAIT_Y_OFFSET;
    SDL_RenderFillRect(renderer, &srcrect);

    /* - name - */
    const s8 name = Unit_Name(stats_menu->unit);
    x = NAME_X_OFFSET, y = NAME_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, name.data, x, y);

    /* - class - */
    s8 class = s8_mut(classNames[Unit_Class(stats_menu->unit)].data);
    class    = s8_toUpper(class);
    x = CLASS_X_OFFSET, y = CLASS_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours, renderer, class.data, x, y);
    s8_free(&class);

    /* - title - */
    s8 title = global_unitTitles[Unit_id(stats_menu->unit)];
    if (title.data != NULL) {
        x = TITLE_X_OFFSET, y = TITLE_Y_OFFSET;
        PixelFont_Write_Len(stats_menu->pixelnours, renderer, title.data, x, y);
    }

    /* - HP simplebar - */
    struct SimpleBar hp_bar = SimpleBar_default;
    hp_bar.scale.x = 1, hp_bar.scale.y = 1;
    StatBar_Colors(&hp_bar,
                   STATS_MENU_HPBAR_BD,
                   STATS_MENU_HPBAR_BL,
                   STATS_MENU_HPBAR_FD,
                   STATS_MENU_HPBAR_FL);

    x = (HP_X_OFFSET + HPBAR_X_OFFSET), y = (HP_Y_OFFSET + HPBAR_Y_OFFSET);
    i32 current_hp = Unit_Current_HP(stats_menu->unit);
    HPBar_Init(&hp_bar, current_hp, effective_stats.hp, x, y);

    /* - HP - */
    x = HP_X_OFFSET, y = HP_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours_big, renderer, "HP", 2, x, y);
    HPBar_Draw(&hp_bar, renderer);
    stbsp_sprintf(numbuff, "%02d/%02d", current_hp, effective_stats.hp);
    x = HP_STAT_X_OFFSET, y = HP_STAT_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - EXP - */
    x = EXP_X_OFFSET, y = EXP_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours_big, renderer, "Exp", 3, x, y);
    stbsp_sprintf(numbuff, "%02d\0\0\0\0", Unit_Experience(stats_menu->unit));
    x = EXP_STAT_X_OFFSET, y = EXP_STAT_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - Level - */
    x = LV_X_OFFSET, y = LV_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours_big, renderer, "Lv", 2, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", Unit_Level(stats_menu->unit));
    x = LV_STAT_X_OFFSET, y = LV_STAT_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - Move - */
    x = MOVE_X_OFFSET, y = MOVE_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours_big, renderer, "Mv", 2, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_stats.move);
    x = MOVE_STAT_X_OFFSET, y = MOVE_STAT_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);
}

static void _StatsMenu_Draw_Mount(struct StatsMenu *stats_menu, SDL_Renderer *renderer) {
    /* -- MOUNT -- */
    /* - preliminaries - */
    int x = MOUNT_X_OFFSET, y = MOUNT_Y_OFFSET;

    /* - Write mount - */
    PixelFont_Write(stats_menu->pixelnours, renderer, "MOUNT", 5, x, y);

    /* - TODO: Get mount type - */
    // i8 mount_type = MOUNT_NULL;
    // if (stats_menu->unit->mount.ptr != NULL)
    // mount_type = stats_menu->unit->mount.ptr->type;

    // if (mount_type == MOUNT_NULL) {
    //     x = MOUNT_NONE_X_OFFSET, y = MOUNT_NONE_Y_OFFSET;
    //     PixelFont_Write(stats_menu->pixelnours, renderer, "-", 1, x, y);
    //     return;
    // }

    // switch (mount_type) {
    //     case MOUNT_HORSE:
    //     case MOUNT_SALAMANDER:
    //         mount_offset_x = 6;
    //         mount_offset_y = 2;
    //         break;
    //     case MOUNT_PEGASUS:
    //     case MOUNT_EAGLE:
    //         mount_offset_x =  0;
    //         mount_offset_y = -3;
    //         break;
    // }

    // srcrect.w = SM_MOUNTS_TILESIZE;
    // srcrect.h = SM_MOUNTS_TILESIZE;
    // srcrect.x = mount_type % SOTA_COL_LEN * srcrect.w;
    // srcrect.y = mount_type / SOTA_COL_LEN * srcrect.h;
    // dstrect.w = srcrect.w;
    // dstrect.h = srcrect.h;
    // dstrect.x = (MOUNT_ICON_X_OFFSET + mount_offset_x);
    // dstrect.y = (MOUNT_ICON_Y_OFFSET + mount_offset_y);
    // SDL_RenderCopy(renderer, stats_menu->texture_mount, &srcrect, &dstrect);
}

static void _StatsMenu_Draw_Stats(struct StatsMenu *stats_menu, SDL_Renderer *renderer) {
    /* -- STATS -- */
    struct Unit       *unit             =  stats_menu->unit;
    struct Unit_stats effective_stats = Unit_effectiveStats(unit);
    char numbuff[10];
    int x, y;
    // TODO: BONUS/MALUS in GREEN/RED

    /* - statbar ants - */
    struct SimpleBar stat_bar = SimpleBar_default;
    stat_bar.scale.x = 1;
    stat_bar.scale.y = 1;
    StatBar_Colors(&stat_bar,
                   STATS_MENU_STATBAR_BD,
                   STATS_MENU_STATBAR_BL,
                   STATS_MENU_STATBAR_FD,
                   STATS_MENU_STATBAR_FL);

    /* - str - */
    x = STR_X_OFFSET, y = STR_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "STR", 3, x, y);
    x += STATBAR_COL1_X_OFFSET, y += STATBAR_COL1_Y_OFFSET;
    StatBar_Init(&stat_bar, effective_stats.str, Unit_Stats_Caps(unit).str, x, y);
    StatBar_Draw(&stat_bar, renderer);
    // WRITING COLORED TEXT:
    // SDL_SetTextureColorMod(stats_menu->pixelnours->texture, SOTA_BONUS_RED, SOTA_BONUS_GREEN, SOTA_BONUS_BLUE); /* src texture*/
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_stats.str);
    x = STR_STAT_X_OFFSET, y = STR_STAT_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);
    // SDL_SetTextureColorMod(stats_menu->pixelnours->texture, 0xFF, 0xFF, 0xFF); /* src texture*/

    /* - mag - */
    x = MAG_X_OFFSET, y = MAG_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "MAG", 3, x, y);
    x += STATBAR_COL2_X_OFFSET, y += STATBAR_COL2_Y_OFFSET;
    StatBar_Init(&stat_bar, effective_stats.mag, unit->stats.caps.mag, x, y);
    StatBar_Draw(&stat_bar, renderer);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_stats.mag);
    x = MAG_STAT_X_OFFSET, y = MAG_STAT_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - dex - */
    x = DEX_X_OFFSET, y = DEX_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "DEX", 3, x, y);
    x += STATBAR_COL1_X_OFFSET, y += STATBAR_COL1_Y_OFFSET;
    StatBar_Init(&stat_bar, effective_stats.dex, unit->stats.caps.dex, x, y);
    StatBar_Draw(&stat_bar, renderer);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_stats.dex);
    x = DEX_STAT_X_OFFSET, y = DEX_STAT_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - agi - */
    x = AGI_X_OFFSET, y = AGI_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "AGI", 3, x, y);
    x += STATBAR_COL2_X_OFFSET, y += STATBAR_COL2_Y_OFFSET;
    StatBar_Init(&stat_bar, effective_stats.agi, unit->stats.caps.agi, x, y);
    StatBar_Draw(&stat_bar, renderer);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_stats.agi);
    x = AGI_STAT_X_OFFSET, y = AGI_STAT_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - fth - */
    x = FTH_X_OFFSET, y = FTH_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "FTH", 3, x, y);
    x += STATBAR_COL1_X_OFFSET, y += STATBAR_COL1_Y_OFFSET;
    StatBar_Init(&stat_bar, effective_stats.fth, unit->stats.caps.fth, x, y);
    StatBar_Draw(&stat_bar, renderer);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_stats.fth);
    x = FTH_STAT_X_OFFSET, y = FTH_STAT_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - luck - */
    x = LUCK_X_OFFSET, y = LUCK_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "LUCK", 4, x, y);
    x += STATBAR_COL2_X_OFFSET, y += STATBAR_COL2_Y_OFFSET;
    StatBar_Init(&stat_bar, effective_stats.luck, unit->stats.caps.luck, x, y);
    StatBar_Draw(&stat_bar, renderer);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_stats.luck);
    x = LUCK_STAT_X_OFFSET, y = LUCK_STAT_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - def - */
    x = DEF_X_OFFSET, y = DEF_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "DEF", 3, x, y);
    x += STATBAR_COL1_X_OFFSET, y += STATBAR_COL1_Y_OFFSET;
    StatBar_Init(&stat_bar, effective_stats.def, unit->stats.caps.def, x, y);
    StatBar_Draw(&stat_bar, renderer);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_stats.def);
    x = DEF_STAT_X_OFFSET, y = DEF_STAT_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - res - */
    x = RES_X_OFFSET, y = RES_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "RES", 3, x, y);
    x += STATBAR_COL2_X_OFFSET, y += STATBAR_COL2_Y_OFFSET;
    StatBar_Init(&stat_bar, effective_stats.res, unit->stats.caps.res, x, y);
    StatBar_Draw(&stat_bar, renderer);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_stats.res);
    x = RES_STAT_X_OFFSET, y = RES_STAT_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - con - */
    x = CON_X_OFFSET, y = CON_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "CON", 3, x, y);
    x += STATBAR_COL1_X_OFFSET, y += STATBAR_COL1_Y_OFFSET;
    StatBar_Init(&stat_bar, effective_stats.con, unit->stats.caps.con, x, y);
    StatBar_Draw(&stat_bar, renderer);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_stats.con);
    x = CON_STAT_X_OFFSET, y = CON_STAT_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - prof - */
    x = PROF_X_OFFSET, y = PROF_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "PROF", 4, x, y);
    x += STATBAR_COL2_X_OFFSET, y += STATBAR_COL2_Y_OFFSET;
    StatBar_Init(&stat_bar, effective_stats.prof, unit->stats.caps.prof, x, y);
    StatBar_Draw(&stat_bar, renderer);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_stats.prof);
    x = PROF_STAT_X_OFFSET, y = PROF_STAT_Y_OFFSET;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);
}

static void _StatsMenu_Draw_Hands(struct StatsMenu *stats_menu, SDL_Renderer *renderer) {
    SDL_Rect dstrect, srcrect;
    /* - HANDS - */
    int stronghand = Unit_Hand_Strong(stats_menu->unit);

    int ly_offset = 0;
    if (Unit_istwoHanding(stats_menu->unit))
        ly_offset = SM_TWOHAND_Y_OFFSET;

    srcrect.w = SM_HANDS_TILESIZE;
    srcrect.h = SM_HANDS_TILESIZE;
    dstrect.w = srcrect.w;
    dstrect.h = srcrect.h;

    /* Left hand */
    int index = (stronghand == UNIT_HAND_RIGHT) ? SM_HANDS_SMALL_L : SM_HANDS_BIG_L;
    srcrect.x = index * srcrect.w;
    srcrect.y = 0;

    /* Moving hand if two handing or small hand */
    dstrect.x = SM_HANDL_X;
    dstrect.y = (stronghand == UNIT_HAND_LEFT) ? SM_HAND_STRONG_Y : SM_HAND_WEAK_Y;
    dstrect.y += ly_offset ;

    // Moving hand if small
    if (stronghand == UNIT_HAND_RIGHT) {
        dstrect.x += SM_HAND_SMALLX_OFFSET;
        dstrect.y += SM_HAND_SMALLY_OFFSET;
    }

    SDL_RenderCopy(renderer, stats_menu->texture_hands, &srcrect, &dstrect);

    /* Right hand */
    srcrect.w = SM_HANDS_TILESIZE;
    srcrect.h = SM_HANDS_TILESIZE;
    dstrect.w = srcrect.w;
    dstrect.h = srcrect.h;

    index = (stronghand == UNIT_HAND_LEFT) ? SM_HANDS_SMALL_R : SM_HANDS_BIG_R;
    srcrect.x = index * srcrect.w;
    srcrect.y = 0;

    /* Moving hand if two handing or small hand */
    // if ambidextrous, LEFT hand is strong hand/on top
    dstrect.x = SM_HANDR_X;
    dstrect.y = (stronghand == UNIT_HAND_RIGHT) ? SM_HAND_STRONG_Y : SM_HAND_WEAK_Y;
    dstrect.y += ly_offset;

    // Moving hand if small
    if (stronghand == UNIT_HAND_LEFT) {
        dstrect.x += SM_HAND_SMALLX_OFFSET;
        dstrect.y += SM_HAND_SMALLY_OFFSET;
    }

    SDL_RenderCopy(renderer, stats_menu->texture_hands, &srcrect, &dstrect);
}

static void _StatsMenu_Draw_Rescue(struct StatsMenu *stats_menu, SDL_Renderer *renderer) {
    struct Unit *unit = stats_menu->unit;
    int x, y;

    /* -- RESCUE -- */
    // TODO: Rescue icon
    x = RESCUE_X_OFFSET, y = RESCUE_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "RESCUE", 7, x, y);
    if (unit->rescue.id > UNIT_ID_START) {
        s8 name = _Unit_Name_id(unit->rescue.id);
        int width = PixelFont_Width_Len(stats_menu->pixelnours, name.data);
        x = RESCUEE_X_OFFSET - width / 2, y = RESCUEE_Y_OFFSET;
        PixelFont_Write_Len(stats_menu->pixelnours, renderer, name.data, x, y);
    } else {
        x = RESCUE_NONE_X_OFFSET, y = RESCUE_NONE_Y_OFFSET;
        PixelFont_Write(stats_menu->pixelnours, renderer, "-", 1, x, y);
    }

    /* -- STATUSES -- */
    // TODO: iterate over statuses
    x = STATUSES_X_OFFSET, y = STATUSES_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "STATUS", 6, x, y);

}

static void _StatsMenu_Draw_Skills(struct StatsMenu *stats_menu, SDL_Renderer *renderer) {
    SDL_Rect srcrect;

    /* -- SKILLS -- */
    // TODO: f and Render Skills icons 16*16 or 32*32
    int x = SKILLS_X_OFFSET, y = SKILLS_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "SKILLS", 6, x, y);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    srcrect.w = SKILL_ICON_W;
    srcrect.h = SKILL_ICON_H;
    srcrect.x = SKILL1_X_OFFSET;
    srcrect.y = SKILL1_Y_OFFSET;
    SDL_RenderFillRect(renderer, &srcrect);
    srcrect.x = SKILL2_X_OFFSET;
    srcrect.y = SKILL2_Y_OFFSET;
    SDL_RenderFillRect(renderer, &srcrect);
    srcrect.x = SKILL3_X_OFFSET;
    srcrect.y = SKILL3_Y_OFFSET;
    SDL_RenderFillRect(renderer, &srcrect);
    srcrect.x = SKILL4_X_OFFSET;
    srcrect.y = SKILL4_Y_OFFSET;
    SDL_RenderFillRect(renderer, &srcrect);
    srcrect.x = SKILL5_X_OFFSET;
    srcrect.y = SKILL5_Y_OFFSET;
    SDL_RenderFillRect(renderer, &srcrect);
}

static void _StatsMenu_Draw_Statuses(struct StatsMenu *stats_menu, SDL_Renderer *renderer) {
    /* SDL_Rect dstrect, srcrect; */

    /* -- Statuses -- */
    // SDL_assert(stats_menu->unit->statuses.queue != NULL);
    // if (DARR_NUM(stats_menu->unit->statuses.queue) <= 0) {
    //     int x = STATUSES_NONE_X_OFFSET, y = STATUSES_NONE_Y_OFFSET;
    //     PixelFont_Write(stats_menu->pixelnours, renderer, "-", 1, x, y);
    //     return;
    // }

    // struct Unit_status status = stats_menu->unit->statuses.queue[0];

    // srcrect.w = SM_STATUSES_TILESIZE;
    // srcrect.h = SM_STATUSES_TILESIZE;
    // srcrect.x = sota_ss_x(status.status, SOTA_COL_LEN) * srcrect.w;
    // srcrect.y = sota_ss_y(status.status, SOTA_COL_LEN) * srcrect.h;
    // dstrect.w = srcrect.w;
    // dstrect.h = srcrect.h;
    // dstrect.x = (STATUSES_ICON_X_OFFSET + status_offset_x[status.status]);
    // dstrect.y = (STATUSES_ICON_Y_OFFSET + status_offset_y[status.status]);
    // SDL_RenderCopy(renderer, stats_menu->texture_statuses, &srcrect, &dstrect);
}

static void _StatsMenu_Draw_WpnTypes(struct StatsMenu *stats_menu, SDL_Renderer *renderer) {
    struct Unit *unit = stats_menu->unit;
    // SDL_Rect srcrect;

    /* -- WEAPON TYPES -- */
    int x = WEAPONS_X_OFFSET, y = WEAPONS_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "WPN TYPE", 8, x, y);
    // u64 wpntypecode = 1;
    // srcrect.h = SM_WEAPONS_TILESIZE;
    // srcrect.w = SM_WEAPONS_TILESIZE;

    u8 equippables[ITEM_TYPE_EXP_END];
    u8 equippable_num  = Unit_canEquip_allTypes(unit, equippables);
    SDL_assert(equippable_num < stats_menu->font_wpns->charset_num);

    /* render equippable type icons, centering */
    if (equippable_num == 0) {
        x = WEAPONS_NONE_X_OFFSET, y = WEAPONS_NONE_Y_OFFSET;
        PixelFont_Write(stats_menu->pixelnours, renderer, "-", 1, x, y);
    } else {
        x = WEAPONS_ICON_X_OFFSET, y = WEAPONS_ICON_Y_OFFSET;
        PixelFont_Write_Centered(stats_menu->font_wpns, renderer, equippables, equippable_num, x, y);
    }
}

static void _StatsMenu_Draw_Item(struct StatsMenu *stats_menu, SDL_Renderer *renderer, int eq) {
    /* -- Preliminaries -- */
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq < SOTA_EQUIPMENT_SIZE);
    SDL_Rect srcrect;
    char numbuff[10];
    struct Unit *unit = stats_menu->unit;
    SDL_assert(gl_weapons_dtab  != NULL);
    SDL_assert(gl_items_dtab    != NULL);

    i16 item_y_offset, item_dura_y_offset;

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    srcrect.w = ITEM_ICON_W;
    srcrect.h = ITEM_ICON_H;
    srcrect.x = SM_ITEML_X;
    srcrect.y = SM_ITEM_Y + (eq - ITEM1) * SM_LINE;
    if (Unit_istwoHanding(stats_menu->unit))
        srcrect.y = SM_ITEM_TWOHAND_Y;
    else if (eq == UNIT_HAND_RIGHT)
        srcrect.x = SM_ITEMR_X;

    if (eq == ITEM1)
        srcrect.x += SM_HANDL_X_OFFSET;

    SDL_RenderFillRect(renderer, &srcrect);
    /* - Iem icon - */
    item_y_offset      = ITEM1_NAME_Y_OFFSET + (eq - ITEM1) * (ITEM_ICON_H + ITEM_ICON_SPACE);
    item_dura_y_offset = ITEM1_DURA_Y_OFFSET + (eq - ITEM1) * (ITEM_ICON_H + ITEM_ICON_SPACE);

    /* Writing - if no item, then next*/
    struct Inventory_item *invitem = Unit_InvItem(unit, eq);

    if (invitem == NULL) {
        return;
    }
    if (invitem->id <= ITEM_NULL) {
        int x = ITEM1_NAME_X_OFFSET, y = item_y_offset;
        PixelFont_Write(stats_menu->pixelnours, renderer, "-", 1, x, y);
        return;
    }

    /* Writing - number of uses left */
    int uses_left = 0;
    if (Weapon_ID_isValid(invitem->id)) {
        Weapon_Load(gl_weapons_dtab, invitem->id);
        const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, invitem->id);
        SDL_assert(weapon                   != NULL);

        /* - uses left - */
        SDL_assert(weapon->item.stats.uses > 0);
        uses_left = (weapon->item.stats.uses - invitem->used);
    } else if (Item_ID_isValid(invitem->id)) {
        SDL_assert(gl_items_dtab != NULL);
        Item_Load(gl_items_dtab, invitem->id);
        const Item *item = DTAB_GET_CONST(gl_items_dtab, invitem->id);
        uses_left = (item->stats.uses - invitem->used);
    }
    // SDL_assert(uses_left > 0);

    stbsp_sprintf(numbuff, "%d\0\0\0\0", uses_left);

    int width_uses_left = PixelFont_Width_Len(stats_menu->pixelnours_big, numbuff);
    int x = ITEM1_DURA_X_OFFSET - width_uses_left / 2, y = item_dura_y_offset;
    if (eq == UNIT_HAND_LEFT)
        x += ITEM_ICON_W;
    else if (eq == UNIT_HAND_RIGHT)
        x = SM_ITEMR_X - width_uses_left - 1;

    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - Item name - */
    int *order = DTAB_GET(global_itemOrders, invitem->id);
    SDL_assert(order != NULL);
    s8 item_name = global_itemNames[*order];
    s8 item_name_upper = s8_mut(item_name.data);
    item_name_upper = s8_toUpper(item_name_upper);
    int width = PixelFont_Width_Len(stats_menu->pixelnours_big, item_name.data);
    int limit = (eq - ITEM1) < ITEM_HANDS_INDEX ? ITEM1_NAME_W_MAX : ITEM3_NAME_W_MAX;
    x = ITEM1_NAME_X_OFFSET;
    if (eq == UNIT_HAND_LEFT)
        x += ITEM_ICON_W;

    if (width <= limit) {
        if (eq == UNIT_HAND_RIGHT)
            x = SM_ITEMR_X - width_uses_left - width;

        /* Name is short enough: write on one line */
        y = item_y_offset;
        PixelFont_Write_Len(stats_menu->pixelnours, renderer, item_name_upper.data, x, y);
        s8_free(&item_name_upper);
        return;
    }

    /* Name too long: write on two lines if too long */
    stats_menu->pixelnours->linespace = -1;
    /* find last space to replace with \n */
    char *last_space = strrchr(item_name_upper.data, ' ');

    if (eq == UNIT_HAND_RIGHT) {
        size_t len2 = strlen(last_space + 1);
        size_t len1 = item_name_upper.num - len2 - 1;

        int w1 = PixelFont_Width(stats_menu->pixelnours_big, item_name_upper.data,  len1);
        int w2 = PixelFont_Width(stats_menu->pixelnours_big, last_space + 1, len2);

        size_t offset = w1 > w2 ? w1 : w2;
        x = SM_ITEMR_X - width_uses_left - offset - 1;
    }

    nstr_replaceSingle(last_space, ' ', '\n');
    y = item_y_offset - ITEM_TWOLINES_OFFSET_Y;
    PixelFont_Write_Len(stats_menu->pixelnours, renderer, item_name_upper.data, x, y);

    s8_free(&item_name_upper);
}

static void _StatsMenu_Draw_Equipment(struct StatsMenu *stats_menu, SDL_Renderer *renderer) {
    /* --- Equipment --- */
    for (u8 i = ITEM1; i < SOTA_EQUIPMENT_SIZE; i++) {
        _StatsMenu_Draw_Item(stats_menu, renderer, i);
    }

}

static void _StatsMenu_Draw_ComputedStats(struct StatsMenu *stats_menu, SDL_Renderer *renderer) {
    /* -- COMPUTED STATS -- */
    int dist = 1; // TODO: get distance from equipped weapon
    Unit_stats eff_stats = Unit_effectiveStats(stats_menu->unit);
    struct Computed_Stats computed_stats = Unit_computedStats(stats_menu->unit, dist, eff_stats);
    struct Unit *unit = stats_menu->unit;
    char numbuff[10];

    /* - ATK - */
    int width, x, y;
    x = ATK_X_OFFSET, y = ATK_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "ATK", 3, x, y);
    if (computed_stats.attack.True > 0) {
        /* Compute width of LEFTWARD STAT to center the "/" */
        stbsp_sprintf(numbuff, "%d\0\0\0\0", computed_stats.attack.physical);
        width = PixelFont_Width_Len(stats_menu->pixelnours_big, numbuff);
        stbsp_sprintf(numbuff, "%d/%d/%d", computed_stats.attack.physical, computed_stats.attack.magical,
                      computed_stats.attack.True);
        x = ATK_X_OFFSET_STAT2 - width, y = ATK_Y_OFFSET_STAT1;
    } else {
        stbsp_sprintf(numbuff, "%d\0\0\0\0", computed_stats.attack.physical);
        width = PixelFont_Width_Len(stats_menu->pixelnours_big, numbuff);
        stbsp_sprintf(numbuff, "%d/%d\0\0", computed_stats.attack.physical, computed_stats.attack.magical);
        x = ATK_X_OFFSET_STAT1 - width, y = ATK_Y_OFFSET_STAT1;
    }
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - DEF - */
    x = PROT_X_OFFSET, y = PROT_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "DEF", 3, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", computed_stats.protection.physical);
    width = PixelFont_Width_Len(stats_menu->pixelnours_big, numbuff);
    stbsp_sprintf(numbuff, "%d/%d\0\0", computed_stats.protection.physical,
                  computed_stats.protection.magical);
    x = PROT_X_OFFSET_STAT1 - width, y = PROT_Y_OFFSET_STAT1;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - HIT - */
    x = HIT_X_OFFSET, y = HIT_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "HIT", 3, x, y);
    // Compute width of LEFTWARD STAT to center the "/"
    stbsp_sprintf(numbuff, "%d\0\0\0\0", computed_stats.hit);
    width = PixelFont_Width_Len(stats_menu->pixelnours_big, numbuff);
    stbsp_sprintf(numbuff, "%d/%d\0\0", computed_stats.hit, computed_stats.dodge);
    x = HIT_X_OFFSET_STAT - width, y = HIT_Y_OFFSET_STAT;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - CRIT - */
    x = CRIT_X_OFFSET, y = CRIT_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "CRIT", 4, x, y);
    // Compute width of LEFTWARD STAT to center the "/"
    stbsp_sprintf(numbuff, "%d\0\0\0\0", computed_stats.crit);
    width = PixelFont_Width_Len(stats_menu->pixelnours_big, numbuff);
    stbsp_sprintf(numbuff, "%d/%d\0\0", computed_stats.crit, computed_stats.favor);
    x = CRIT_X_OFFSET_STAT - width, y = CRIT_Y_OFFSET_STAT;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - RANGE - */
    x = RANGE_X_OFFSET, y = RANGE_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "RANGE", 5, x, y);
    struct Range *range = &computed_stats.range_loadout;
    stbsp_sprintf(numbuff, "%d - %d", range->min, range->max);
    x = RANGE_X_OFFSET_STAT, y = RANGE_Y_OFFSET_STAT;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* - SPEED - */
    x = SPEED_X_OFFSET, y = SPEED_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "SPEED", 5, x, y);
    stbsp_sprintf(numbuff, "%2d\0\0\0\0", computed_stats.speed);
    x = SPEED_X_OFFSET_STAT, y = SPEED_Y_OFFSET_STAT;
    PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);

    /* -- REGRETS -- */
    x = REGRETS_X_OFFSET, y = REGRETS_Y_OFFSET;
    PixelFont_Write(stats_menu->pixelnours, renderer, "REGRETS", 7, x, y);
    i32 regrets = Unit_Current_Regrets(unit);
    if (regrets <= 0) {
        x = REGRETS_X_STAT, y = REGRETS_Y_STAT;
        PixelFont_Write(stats_menu->pixelnours, renderer, "-", 1, x, y);
    } else {
        stbsp_sprintf(numbuff, "%d\0\0\0\0", regrets);
        width = PixelFont_Width_Len(stats_menu->pixelnours_big, numbuff);
        x = REGRETS_X_STAT - width / 2, y = REGRETS_Y_STAT;
        PixelFont_Write_Len(stats_menu->pixelnours_big, renderer, numbuff, x, y);
    }
}

void StatsMenu_Draw(struct Menu *mc, SDL_Texture *rt, SDL_Renderer *renderer) {
    struct StatsMenu *stats_menu = (struct StatsMenu *)mc->data;
    struct n9Patch   *n9patch    = &mc->n9patch;

    SDL_assert(stats_menu != NULL);

    if (stats_menu->update) {
        StatsMenu_Update(stats_menu, n9patch, rt, renderer);
        stats_menu->update = false;
    }
#ifndef __SOTA_RELEASE__
    /* if unit has nothing equipped, but item in hand is equippable, equip it. */
    SDL_assert(stats_menu->unit);

    canEquip can_equip  = canEquip_default;

    i32 eq = Unit_Eq_Equipped(stats_menu->unit, UNIT_HAND_LEFT);
    if (eq_valid(eq))
        canEquip_Loadout(&can_equip, UNIT_HAND_LEFT, eq);

    eq = Unit_Eq_Equipped(stats_menu->unit, UNIT_HAND_RIGHT);
    if (eq_valid(eq))
        canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, eq);

    canEquip_Eq(&can_equip, UNIT_HAND_LEFT);
    can_equip.hand      = UNIT_HAND_LEFT;
    b32 iscan = Unit_canEquip(stats_menu->unit, can_equip);
    if (iscan) {
        stats_menu->update = true;
        Unit_Equip(stats_menu->unit, UNIT_HAND_LEFT, UNIT_HAND_LEFT);
    }
#endif
    /* TODO: set position of statsmenu */
    Point size = n9Patch_Pixels_Total(n9patch);

    SDL_Rect dstrect = {
        .w = size.x * n9patch->scale.x,
        .h = size.y * n9patch->scale.y,
        .x = stats_menu->pos.x,
        .y = stats_menu->pos.y,
    };
    SDL_assert(stats_menu->texture != NULL);
    SDL_RenderCopy(renderer, stats_menu->texture, NULL, &dstrect);
    Utilities_DrawColor_Reset(renderer);
}

void StatsMenu_Update(struct StatsMenu *stats_menu, struct n9Patch *n9patch,
                      SDL_Texture *rt, SDL_Renderer *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(renderer         != NULL);
    SDL_assert(stats_menu       != NULL);
    SDL_assert(stats_menu->unit != NULL);
    /* - variable declaration/ ants definition - */
    Point size = n9Patch_Pixels_Total(n9patch);
    SDL_assert(size.x > 0);
    SDL_assert(size.y > 0);
    SDL_assert(n9patch->scale.x       > 0);
    SDL_assert(n9patch->scale.y       > 0);

    if (stats_menu->update_stats) {
        Unit_stats effective_stats = Unit_effectiveStats(stats_menu->unit);
        /* Stats Menu shows dual wielding stats ASSUMING BOTH WEAPONS IN RANGE */
        Unit_computedStats(stats_menu->unit, -1, effective_stats);
        // TODO: stop computing stats every iteration
        // stats_menu->update_stats = false;
    }

    /* - create render target texture - */
    if (stats_menu->texture == NULL) {
        stats_menu->texture = SDL_CreateTexture(renderer,
                                                SDL_PIXELFORMAT_ARGB8888,
                                                SDL_TEXTUREACCESS_TARGET,
                                                size.x, size.y);
        SDL_assert(stats_menu->texture != NULL);
        SDL_SetTextureBlendMode(stats_menu->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, stats_menu->texture);
    SDL_assert(stats_menu->texture != NULL);

    /* --- RENDERING STATS-MENU --- */
    /* -- PATCHES DRAW -- */
    int scale_x         = n9patch->scale.x;
    int scale_y         = n9patch->scale.y;
    n9patch->scale.x    = 1;
    n9patch->scale.y    = 1;
    n9patch->pos.x      = 0;
    n9patch->pos.y      = 0;
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x    = scale_x;
    n9patch->scale.y    = scale_y;

    _StatsMenu_Draw_Name(            stats_menu, renderer);
    _StatsMenu_Draw_Equipment(       stats_menu, renderer);
    _StatsMenu_Draw_ComputedStats(   stats_menu, renderer);
    _StatsMenu_Draw_Mount(           stats_menu, renderer);
    _StatsMenu_Draw_WpnTypes(        stats_menu, renderer);
    _StatsMenu_Draw_Rescue(          stats_menu, renderer);
    _StatsMenu_Draw_Statuses(        stats_menu, renderer);
    _StatsMenu_Draw_Skills(          stats_menu, renderer);
    _StatsMenu_Draw_Stats(           stats_menu, renderer);
    _StatsMenu_Draw_Hands(           stats_menu, renderer);

    SDL_SetRenderTarget(renderer, rt);

    SDL_assert(stats_menu->texture);
}
