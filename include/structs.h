#ifndef STRUCTS_H
#define STRUCTS_H

#include "SDL2/SDL.h"
#include "types.h"
#include "enums.h"
#include "flecs.h"
#include "tnecs.h"
#include <stdint.h>
#include <stdbool.h>

extern struct Input_Arguments {
    uint16_t cutScene; // plays the scene then exit
    uint16_t talkScene;
    uint8_t map_index;
    uint8_t startup_mode;
    bool print_help; // if help requested anywhere, print help and exit
    char * save_filename; // debug saves are outside normal save integers
} Input_Arguments_default;

struct String_hash {
    int16_t key;
    char * value;
};

extern struct Tile_stats {
    int8_t dodge;
    int8_t Pprot;
    int8_t Mprot;
    int8_t heal; // % Negative means damage.
} Tile_stats_default;

extern struct Unit_status_turns {
    // Number of turns to be in this state.
    // <0 means forever. -1 not
    int8_t poisoned;
    int8_t sleep; // sleep is dumb->stunned or unconcious?
    int8_t stone;
    int8_t rooted;
    int8_t stunned;
    int8_t silenced;
    int8_t slowed; // cuts mvt in half?
} Unit_status_turns_default;

struct Map_condition {
    int16_t army;
    int16_t unit;
    int16_t col_min;
    int16_t col_max;
    int16_t row_min;
    int16_t row_max;
};

struct SDL_Textures_phash {
    int16_t key;
    SDL_Texture * value;
};

struct Combat_Forecast_Flow {
    bool defender_retaliates;
    bool defender_doubles; // (attacker_doubles) -> (!defender_doubles)
    bool attacker_doubles; // (defender_doubles) -> (!attacker_doubles)
    uint8_t attacker_brave; // number of additional attacks
    uint8_t defender_brave; // number of additional attacks
};

struct Computed_Stats {
    uint8_t hp;
    uint8_t attack[DMG_TYPES];
    uint8_t defense[DMG_TYPES];
    uint8_t hit;
    uint8_t dodge;
    uint8_t crit;
    uint8_t favor;
    uint8_t move;
    int8_t speed;
    uint8_t agony;
};

struct Combat_Outcome_Attack {
    bool aggressor;
    bool hit;
    bool crit;
};

extern struct Combat_Forecast_Death {
    bool attacker_certain;
    bool defender_certain;
    bool attacker_possible;
    bool defender_possible;
} Combat_Forecast_Death_default;


extern struct Damage {
    uint8_t dmg[DMG_TYPES];
    uint8_t dmg_crit[DMG_TYPES];
} Damage_default;

extern struct Combat_Forecast_Rates {
    uint8_t hit;
    uint8_t crit;
} Combat_Forecast_Rates_default;

extern struct Combat_Forecast_Stats {
    struct Combat_Forecast_Rates attacker_rates;
    struct Combat_Forecast_Rates defender_rates;
    struct Damage defender_damage;
    struct Damage attacker_damage;
    struct Computed_Stats attacker_computed;
    struct Computed_Stats defender_computed;
    int8_t defender_equipment[2];
    int8_t attacker_equipment[2];
} Combat_Forecast_Stats_default;

struct Combat_Forecast {
    struct Combat_Forecast_Flow flow;
    struct Combat_Forecast_Death death;
    struct Combat_Forecast_Stats stats;
};

struct Job_talent {
    bool librarian; // find new magic randomly.
    bool cook; // -> cooking bonus
    bool guard; // -> gets prof bonus
    bool scribe; // -> Write books
    bool stable_hand; // Horse bonus. What kind of horse bonus.
    bool assistant; // -> Makes orders. Gets better prices. Buys things for you. reminds you that you already ordered
    bool clergyman; // -> Religious bonus
    bool storage_master; // Good storage masters find weapons randomly.
    bool magic_researcher; // Good researchers invent new magic/ writes copies of new magic. Staffs also? or Staffs only?
};

extern struct Quantity {
    uint8_t swords;
    uint8_t lances;
    uint8_t axes;
    uint8_t bows;
    uint8_t trinkets;
    uint8_t offhands;
    uint8_t elemental;
    uint8_t demonic;
    uint8_t angelic;
    uint8_t shields;
    uint8_t staffs;
    uint8_t claws;
    uint8_t items;
    uint8_t books;
} Quantity_default;

extern struct Condition {
    int16_t unitid;
    bool dead;
    bool recruited;
} Condition_default;

extern struct Unit_stats {
    uint8_t hp;   // hit points
    uint8_t str;  // strength
    uint8_t mag;  // magic
    uint8_t agi;  // agility
    uint8_t dex;  // dexterity
    uint8_t luck;
    uint8_t def;  // defense
    uint8_t res;  // resistance
    uint8_t con;  // constitution
    uint8_t move; // movement
    uint8_t prof; // proficiency
} Unit_stats_default;

struct Infusion {
    int8_t power; // 0 means weapon was already infused/is a magic weapon.
    int_wpnType_t type;
};

struct Node {
    int_point_t x;
    int_point_t y;
    int_point_t distance;
};

struct Node3D {
    int_point_t x;
    int_point_t y;
    int_point_t z;
    int_point_t distance;
};

#define NodeHex Node3D
#define HexNode Node3D

struct TilePoint {
    int_tile_t x;
    int_tile_t y;
};

extern struct Point {
    int_point_t x;
    int_point_t y;
} Point_default;

extern struct Pointf {
    float x;
    float y;
} Pointf_default;

#define PointHex Point3D
#define HexPoint Point3D

extern struct Point3D {
    int_point_t x;
    int_point_t y;
    int_point_t z;
} Point3D_default;

extern struct Point3D Cube_Direction_xp;
extern struct Point3D Cube_Direction_xm;
extern struct Point3D Cube_Direction_yp;
extern struct Point3D Cube_Direction_ym;
extern struct Point3D Cube_Direction_zp;
extern struct Point3D Cube_Direction_zm;
extern struct Point3D Cube_Diagonal_xp;
extern struct Point3D Cube_Diagonal_xm;
extern struct Point3D Cube_Diagonal_yp;
extern struct Point3D Cube_Diagonal_ym;
extern struct Point3D Cube_Diagonal_zp;
extern struct Point3D Cube_Diagonal_zm;

extern struct Point3Df {
    float x;
    float y;
    float z;
} Point3Df_default;

#define PointHexf Point3Df
#define HexPointf Point3Df

extern struct Padding {
    int_point_t right;
    int_point_t top;
    int_point_t left;
    int_point_t bottom;
} Padding_default;

#define Square_Neighbors Padding
#define Square_Neighbors_default Padding_default

struct Agony_timer {
    struct Point pos;
    int_unit_t unit_index;
    int_turn_t turns_left;
};

extern struct Weapon_stats {
    struct Computed_Stats combat;
    int_tile_t range[2]; // [min_range, max_range]
    uint8_t Pmight; //Physical might Pprot for shields.
    uint8_t Mmight; //Magic might. Mprot for shields.
    uint8_t wgt; // weight
    uint8_t uses;
    uint8_t prof; // proficiency
    uint8_t hand[2]; //[1,0], [2,0] or [1,2]
    bool dmg_type; // 0 is physical. 1 magic.
} Weapon_stats_default;

extern struct Shop_item {
    int16_t id;
    int8_t num; // <0 means infinity
} Shop_item_default;

struct Shops_hash {
    int16_t key;
    struct Shop_item * value;
};

struct ecs_ent_hash {
    uint64_t key;
    uint64_t value;
};

extern struct Inventory_item {
    int16_t id;
    uint8_t used;
    bool highlighted;
    int8_t infused;
    // item images are highlighted by default.
    // Only dark when in unit inventory and unequippable
} Inventory_item_default;

extern struct Map_arrival {
    struct Point position;
    int16_t army;
    int16_t id;
    uint8_t turn;
    uint8_t levelups;
} Map_arrival_default;

struct Support {
    int16_t index;
    int8_t level;
};

extern struct Movement_cost {
    uint8_t foot_slow; // fencer, mousquetaire, duelist, lord, duke, archer, marksman, mercenary, hero, trooper
    uint8_t foot_fast; // pickpocket thief assassin, demon
    uint8_t mages; // mage, battlemage, sage, oracle, priest, cleric, bishop, incarnate, possessed
    uint8_t riders_slow; // cavalier, archer and marksman rider, lord and duke rider.
    uint8_t riders_fast; // paladin, troubadour
    uint8_t fliers; // pegasus knight, angel
    uint8_t armors; // knight, general
    uint8_t pirates; // Corsair, viking
    uint8_t bandits; // bandit, ravager
} Movement_cost_default;

struct Fps {
    struct Point pos;
    SDL_Color textcolor;
    float sizefactor[2];
    bool show;
    uint8_t cap;
};

struct Mouse {
    uint8_t onhold;
    uint8_t move;
};

struct Cursor {
    int16_t frames;
    int16_t speed;
};

extern struct Settings {
    struct Point res; // resolution
    struct Point pos;
    struct Fps FPS;

    struct Cursor cursor; // 32 bits
    uint16_t tilesize[2];
    uint8_t fontsize;
    bool fullscreen;

    struct Mouse mouse; // 16 bits
    char title[DEFAULT_BUFFER_SIZE];
} Settings_default;

extern struct MouseInputMap {
    uint8_t accept[DEFAULT_MAPPABLE_BUTTONS];
    uint8_t cancel[DEFAULT_MAPPABLE_BUTTONS];
    uint8_t stats[DEFAULT_MAPPABLE_BUTTONS];
    uint8_t menuright[DEFAULT_MAPPABLE_BUTTONS];
    uint8_t menuleft[DEFAULT_MAPPABLE_BUTTONS];
    uint8_t minimap[DEFAULT_MAPPABLE_BUTTONS];
    uint8_t faster[DEFAULT_MAPPABLE_BUTTONS];
    uint8_t pause[DEFAULT_MAPPABLE_BUTTONS];

    uint8_t accept_mapped;
    uint8_t cancel_mapped;
    uint8_t stats_mapped;
    uint8_t menuright_mapped;
    uint8_t menuleft_mapped;
    uint8_t minimap_mapped;
    uint8_t faster_mapped;
    uint8_t pause_mapped;
} MouseInputMap_default;

extern struct KeyboardInputMap {
    SDL_Scancode moveright[DEFAULT_MAPPABLE_BUTTONS];
    SDL_Scancode moveup[DEFAULT_MAPPABLE_BUTTONS];
    SDL_Scancode movedown[DEFAULT_MAPPABLE_BUTTONS];
    SDL_Scancode moveleft[DEFAULT_MAPPABLE_BUTTONS];

    SDL_Scancode accept[DEFAULT_MAPPABLE_BUTTONS];
    SDL_Scancode cancel[DEFAULT_MAPPABLE_BUTTONS];
    SDL_Scancode stats[DEFAULT_MAPPABLE_BUTTONS];
    SDL_Scancode menuright[DEFAULT_MAPPABLE_BUTTONS]; // Switches between units of same affilition, when in 'map' states.
    SDL_Scancode menuleft[DEFAULT_MAPPABLE_BUTTONS]; // Switches between units of same affilition, when in 'map' states.
    SDL_Scancode minimap[DEFAULT_MAPPABLE_BUTTONS]; // Same controls as when in map state.
    SDL_Scancode faster[DEFAULT_MAPPABLE_BUTTONS]; // Toggle or Hold? choosable by user
    SDL_Scancode globalRange[DEFAULT_MAPPABLE_BUTTONS]; // Toggle or Hold?

    SDL_Scancode pause[DEFAULT_MAPPABLE_BUTTONS];

    uint8_t accept_mapped;
    uint8_t cancel_mapped;
    uint8_t stats_mapped;
    uint8_t menuright_mapped;
    uint8_t menuleft_mapped;
    uint8_t minimap_mapped;
    uint8_t faster_mapped;
    uint8_t pause_mapped;

    uint8_t moveleft_mapped;
    uint8_t moveright_mapped;
    uint8_t moveup_mapped;
    uint8_t movedown_mapped;
} KeyboardInputMap_default;

extern struct GamepadInputMap {
    SDL_GameControllerAxis mainxaxis[DEFAULT_MAPPABLE_BUTTONS];
    SDL_GameControllerAxis mainyaxis[DEFAULT_MAPPABLE_BUTTONS];
    SDL_GameControllerAxis secondxaxis[DEFAULT_MAPPABLE_BUTTONS];
    SDL_GameControllerAxis secondyaxis[DEFAULT_MAPPABLE_BUTTONS];

    SDL_GameControllerAxis triggerleft[DEFAULT_MAPPABLE_BUTTONS];
    SDL_GameControllerAxis triggerright[DEFAULT_MAPPABLE_BUTTONS];

    SDL_GameControllerButton moveright[DEFAULT_MAPPABLE_BUTTONS];
    SDL_GameControllerButton moveup[DEFAULT_MAPPABLE_BUTTONS];
    SDL_GameControllerButton movedown[DEFAULT_MAPPABLE_BUTTONS];
    SDL_GameControllerButton moveleft[DEFAULT_MAPPABLE_BUTTONS];

    SDL_GameControllerButton accept[DEFAULT_MAPPABLE_BUTTONS];
    SDL_GameControllerButton cancel[DEFAULT_MAPPABLE_BUTTONS];
    SDL_GameControllerButton stats[DEFAULT_MAPPABLE_BUTTONS];
    SDL_GameControllerButton menuright[DEFAULT_MAPPABLE_BUTTONS];
    SDL_GameControllerButton menuleft[DEFAULT_MAPPABLE_BUTTONS];
    SDL_GameControllerButton minimap[DEFAULT_MAPPABLE_BUTTONS];
    SDL_GameControllerButton faster[DEFAULT_MAPPABLE_BUTTONS];
    SDL_GameControllerButton pause[DEFAULT_MAPPABLE_BUTTONS];

    uint8_t mainxaxis_mapped;
    uint8_t mainyaxis_mapped;
    uint8_t secondxaxis_mapped;
    uint8_t secondyaxis_mapped;

    uint8_t triggerleft_mapped;
    uint8_t triggerright_mapped;

    uint8_t moveleft_mapped;
    uint8_t moveright_mapped;
    uint8_t moveup_mapped;
    uint8_t movedown_mapped;

    uint8_t accept_mapped;
    uint8_t cancel_mapped;
    uint8_t stats_mapped;
    uint8_t menuright_mapped;
    uint8_t menuleft_mapped;
    uint8_t minimap_mapped;
    uint8_t faster_mapped;
    uint8_t pause_mapped;
} GamepadInputMap_default;

struct Units_stats_hash {
    int16_t key;
    struct Unit_stats value;
};

extern struct Units_stats_hash * promotion_bonus_stats;

struct Uint64_t_hash {
    int16_t key;
    uint64_t value;
};

extern struct Uint64_t_hash * promotion_bonus_skills;

struct Uint64_t_nestedhash {
    int16_t key;
    struct Uint64_t_hash * value;
};

extern struct Uint64_t_nestedhash * lvlup_skills;

// FLECS TRAITS
typedef struct UpdateTimer {
    float last_update;
    float update_time; // 0.0f means update one time
    uint32_t frame_count;
    void (*onUpdate)(ecs_world_t *, ecs_entity_t, uint32_t, float, void *);
} UpdateTimer;

typedef struct tnecs_UpdateTimer {
    float last_update;
    float update_time; // 0.0f means update one time
    uint32_t frame_count;
    void (*onUpdate)(tnecs_world_t *, tnecs_entity_t, uint32_t, float, void *);
} tnecs_UpdateTimer;

typedef struct RenderTimer {
    float last_render;
    float render_time; // 0.0f means update one time
    uint32_t frame_count;
} RenderTimer;

typedef struct TraitsModule {
    ECS_DECLARE_COMPONENT(UpdateTimer);
    ECS_DECLARE_COMPONENT(RenderTimer);
} TraitsModule;

void TraitsModuleImport(ecs_world_t * in_world);
#define TraitsModuleImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, UpdateTimer);\
    ECS_IMPORT_COMPONENT(handles, RenderTimer);


#endif /* STRUCTS_H */
