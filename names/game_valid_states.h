//            state,            substate
REGISTER_ENUM(Combat,           STANDBY)

REGISTER_ENUM(Scene_Talk,       STANDBY)
REGISTER_ENUM(Scene_Talk,       PAUSED)

REGISTER_ENUM(Scene_FMV,        STANDBY)

/* All substates valid for Gameplay_Map */
REGISTER_ENUM(Gameplay_Map,     MAP_MINIMAP) /* Gameplay_Map only */
REGISTER_ENUM(Gameplay_Map,     MENU)
REGISTER_ENUM(Gameplay_Map,     MAP_UNIT_MOVES) /* Gameplay_Map only */
REGISTER_ENUM(Gameplay_Map,     MAP_COMBAT) /* Gameplay_Map only */
REGISTER_ENUM(Gameplay_Map,     MAP_NPCTURN) /* Gameplay_Map only */
REGISTER_ENUM(Gameplay_Map,     SAVING)
REGISTER_ENUM(Gameplay_Map,     STANDBY)
REGISTER_ENUM(Gameplay_Map,     PAUSED)
REGISTER_ENUM(Gameplay_Map,     MAP_CANDIDATES) /* Gameplay_Map only */
REGISTER_ENUM(Gameplay_Map,     CUTSCENE)
REGISTER_ENUM(Gameplay_Map,     MAP_ANIMATION) /* Gameplay_Map only */

REGISTER_ENUM(Gameplay_Camp,    MENU)
REGISTER_ENUM(Gameplay_Camp,    SAVING)
REGISTER_ENUM(Gameplay_Camp,    STANDBY)
REGISTER_ENUM(Gameplay_Camp,    PAUSED)
REGISTER_ENUM(Gameplay_Camp,    CUTSCENE)

REGISTER_ENUM(Preparation,      MENU)
REGISTER_ENUM(Preparation,      SAVING)
REGISTER_ENUM(Preparation,      PAUSED)

REGISTER_ENUM(Title_Screen,     MENU)
REGISTER_ENUM(Title_Screen,     SAVING)
REGISTER_ENUM(Title_Screen,     PAUSED)

REGISTER_ENUM(Animation,        PAUSED)
REGISTER_ENUM(Animation,        STANDBY)