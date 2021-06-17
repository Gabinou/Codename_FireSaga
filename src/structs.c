
#include "narrative.h"

struct Point Point_default = {0, 0};
struct Pointf Pointf_default = {0.0f, 0.0f};

struct Point3D Point3D_default = {0, 0, 0};
struct Point3D Cube_Direction_xp = {0, 1, -1};
struct Point3D Cube_Direction_xm = {0, -1, 1};
struct Point3D Cube_Direction_yp = {1, 0, -1};
struct Point3D Cube_Direction_ym = {-1, 0, 2};
struct Point3D Cube_Direction_zp = {1, -1, 0};
struct Point3D Cube_Direction_zm = {-1, 1, 0};
struct Point3D Cube_Diagonal_xp = {1, 1, -2};
struct Point3D Cube_Diagonal_xm = {-1, -1, 2};
struct Point3D Cube_Diagonal_yp = {-2, 1, 1};
struct Point3D Cube_Diagonal_ym = {2, -1, -1};
struct Point3D Cube_Diagonal_zp = {1, -2, 1};
struct Point3D Cube_Diagonal_zm = {-1, 2, -1};

struct Damage Damage_default = {{0, 0}, {0, 0}};

struct Input_Arguments Input_Arguments_default = {
    .map_index = CHAPTER_TEST,
    .startup_mode = STARTUP_TESTING,
    .cutScene = 0,
    .talkScene = 0,
    .save_filename = NULL,
    .print_help = false,
};

struct Combat_Forecast_Rates Combat_Forecast_Rates_default = {0, 0};

struct Combat_Forecast_Death Combat_Forecast_Death_default = {
    .attacker_possible = false,
    .attacker_certain = false,
    .attacker_possible = false,
    .defender_certain = false,
};

struct Tile_stats Tile_stats_default = {
    .dodge = -1,
    .Pprot = -1,
    .Mprot = -1,
    .heal = -1
};

struct Inventory_item Inventory_item_default = {
    .id = -1,
    .used = 0,
    .highlighted = true,
    .infused = -1
};

struct Shop_item Shop_item_default = {
    .id = 0,
    .num = -1,
};

struct Map_condition Map_condition_default = {
    .unit = -1,
    .army = -1,
    .row_min = -1,
    .row_max = -1,
    .col_min = -1,
    .col_max = -1
};

struct Infusion Infusion_default = {
    .power = -1,
    .type = 0
};

struct Support Support_default = {
    .index = -1,
    .level = -1
};

struct Unit_status_turns Unit_status_turns_default = {
    .poisoned = 0,
    .sleep = 0,
    .stone = 0,
    .rooted = 0,
    .stunned = 0,
    .silenced = 0,
    .slowed = 0,
};

struct Padding Padding_default = {
    .right = 0,
    .top = 0,
    .left = 0,
    .bottom = 0,
};

struct Weapon_stats Weapon_stats_default = {
    .Pmight = 0, //Physical might Pprot for shields.
    .Mmight = 0, //Magic might. Mprot for shields.
    .combat = {0, {0, 0}, {0, 0}},
    .wgt = 0, // weight
    .uses = 0,
    .prof = 0, // proficiency
    .range = {0, 0}, // [min_range, max_range]
    .hand = {0, 0}, //[1,0], [2,0] or [1,2]
    .dmg_type = 0, // 0 is physical. 1 magic.
};

struct Unit_stats Unit_stats_default = {
    .hp = 0,
    .str = 0,
    .mag = 0,
    .dex = 0,
    .agi = 0,
    .luck = 0,
    .def = 0,
    .res = 0,
    .move = 0,
    .con = 0,
    .prof = 0
};

struct Movement_cost Movement_cost_default = {
    .foot_slow = 0,
    .foot_fast = 0,
    .mages = 0,
    .riders_fast = 0,
    .riders_slow = 0,
    .fliers = 0,
    .armors = 0,
    .pirates = 0,
    .bandits = 0
};

struct Condition Condition_default = {
    .unitid = -1,
    .dead = false,
    .recruited = true,
};

struct Map_arrival Map_arrival_default = {
    .turn = 0,
    .levelups = 0,
    .army = -1,
    .id = -1,
    .position = {0, 0},
};

struct Quantity Quantity_default = {
    .swords = 0,
    .lances = 0,
    .axes = 0,
    .bows = 0,
    .trinkets = 0,
    .offhands = 0,
    .elemental = 0,
    .demonic = 0,
    .angelic = 0,
    .shields = 0,
    .staffs = 0,
    .claws = 0,
    .items = 0,
    .books = 0,
};

struct Fps Fps_default = {
    .pos = {FPS_DEFAULT_POSX, FPS_DEFAULT_POSY},
    .show = FPS_DEFAULT_SHOW,
    .sizefactor = {FPS_DEFAULT_SIZEFACTORX, FPS_DEFAULT_SIZEFACTORY},
    .cap = FPS_DEFAULT_CAP,
    .textcolor = {FPS_DEFAULT_COLORR, FPS_DEFAULT_COLORG, FPS_DEFAULT_COLORB}
};

struct Mouse Mouse_default = {
    .onhold = MOUSE_DEFAULT_ONHOLD,
    .move = MOUSE_DEFAULT_MOVE
};

struct Cursor Cursor_default = {DEFAULT_CURSOR_FRAMES, DEFAULT_CURSOR_SPEED};

struct Settings Settings_default = {
    .title = "Codename:Firesaga",
    .res = {1000, 1000},
    .pos = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
    .fontsize = 28,
    .fullscreen =  false,
    .tilesize = {DEFAULT_TILESIZE, DEFAULT_TILESIZE},
    .FPS  = {
        .pos = {.x = FPS_DEFAULT_POSX, .y = FPS_DEFAULT_POSY},
        .textcolor = {FPS_DEFAULT_COLORR, FPS_DEFAULT_COLORG, FPS_DEFAULT_COLORB, FPS_DEFAULT_COLORA},
        .sizefactor = {FPS_DEFAULT_SIZEFACTORX, FPS_DEFAULT_SIZEFACTORY},
        .show = FPS_DEFAULT_SHOW,
        .cap = FPS_DEFAULT_CAP,
    },
    .mouse = {MOUSE_DEFAULT_ONHOLD, MOUSE_DEFAULT_MOVE},
    .cursor = {DEFAULT_CURSOR_FRAMES, DEFAULT_CURSOR_SPEED}
};

struct MouseInputMap MouseInputMap_default = {
    .accept = {SDL_BUTTON_LEFT},
    .cancel = {SDL_BUTTON_RIGHT},
    .stats = {SDL_BUTTON_MIDDLE},
    .menuright = {0},
    .menuleft = {0},
    .minimap = {0},
    .faster = {0},
    .pause = {0},
    .accept_mapped = 1,
    .cancel_mapped = 1,
    .stats_mapped = 1,
    .menuright_mapped = 1,
    .menuleft_mapped = 1,
    .minimap_mapped = 1,
    .faster_mapped = 1,
    .pause_mapped = 1,
};

struct KeyboardInputMap KeyboardInputMap_default = {
    .moveright = {SDL_SCANCODE_RIGHT, SDL_SCANCODE_D},
    .moveup = {SDL_SCANCODE_W, SDL_SCANCODE_UP},
    .movedown = {SDL_SCANCODE_S, SDL_SCANCODE_DOWN},
    .moveleft = {SDL_SCANCODE_A, SDL_SCANCODE_LEFT},
    .accept = {SDL_SCANCODE_SPACE},
    .cancel = {SDL_SCANCODE_BACKSPACE},
    .stats = {SDL_SCANCODE_Q},
    .menuright = {SDL_SCANCODE_Q},
    .menuleft = {SDL_SCANCODE_E},
    .minimap = {SDL_SCANCODE_R},
    .faster = {SDL_SCANCODE_TAB},
    .pause = {SDL_SCANCODE_RETURN},

    .moveleft_mapped = 2,
    .moveright_mapped = 2,
    .moveup_mapped = 2,
    .movedown_mapped = 2,

    .accept_mapped = 1,
    .cancel_mapped = 1,
    .stats_mapped = 1,
    .menuright_mapped = 1,
    .menuleft_mapped = 1,
    .minimap_mapped = 1,
    .faster_mapped = 1,
    .pause_mapped = 1,
};

struct GamepadInputMap GamepadInputMap_default = {
    .mainxaxis = {SDL_CONTROLLER_AXIS_LEFTX},
    .mainyaxis = {SDL_CONTROLLER_AXIS_LEFTY},
    .secondxaxis = {SDL_CONTROLLER_AXIS_RIGHTX},
    .secondyaxis = {SDL_CONTROLLER_AXIS_RIGHTY},
    .triggerleft = {SDL_CONTROLLER_AXIS_TRIGGERLEFT},
    .triggerright = {SDL_CONTROLLER_AXIS_TRIGGERRIGHT},
    .moveright = {SDL_CONTROLLER_BUTTON_DPAD_RIGHT},
    .moveup = {SDL_CONTROLLER_BUTTON_DPAD_UP},
    .movedown = {SDL_CONTROLLER_BUTTON_DPAD_DOWN},
    .moveleft = {SDL_CONTROLLER_BUTTON_DPAD_LEFT},
    .accept = {SDL_CONTROLLER_BUTTON_A},
    .cancel = {SDL_CONTROLLER_BUTTON_B},
    .stats = {SDL_CONTROLLER_BUTTON_X},
    .menuright = {SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
    .menuleft = {SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
    .minimap = {SDL_CONTROLLER_BUTTON_Y},
    .faster = {SDL_CONTROLLER_BUTTON_START},
    .pause = {SDL_CONTROLLER_BUTTON_START},

    .mainxaxis_mapped = 1,
    .mainyaxis_mapped = 1,
    .secondxaxis_mapped = 1,
    .secondyaxis_mapped = 1,

    .triggerleft_mapped = 1,
    .triggerright_mapped = 1,

    .moveleft_mapped = 1,
    .moveright_mapped = 1,
    .moveup_mapped = 1,
    .movedown_mapped = 1,

    .accept_mapped = 1,
    .cancel_mapped = 1,
    .stats_mapped = 1,
    .menuright_mapped = 1,
    .menuleft_mapped = 1,
    .minimap_mapped = 1,
    .faster_mapped = 1,
    .pause_mapped = 1,
};



