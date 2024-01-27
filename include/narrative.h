#ifndef NARRATIVE_H
#define NARRATIVE_H

#include <string.h>
#include "types.h"
#include "utilities.h"
#include "filesystem.h"
#include "jsonio.h"
#include "enums.h"
#include "macros.h"
#include "debug.h"
#include "nmath.h"
#include "cJSON.h"
#include "bitfields.h"
#include "SDL2/SDL.h"

/* -- Didascalie (theater vocabulary) -- */
// - Note to actors of a scene about what to *do* during a scene
// TODO
// Example instructions:
//  - Face direction
//  - Left, right
//  - Stand
//    - Out of frame (OoF): left, right,
//    - In frame: Left, Left-Mid, mid, Right-Mid, Right,,
//    - Pixel precise
//  - Move to
//    - From OoF left to IF Mid
//    - Fast/Slow
//  - Slapstick
//    - Do a flip
//    - Magic?
//    - Swords?
//  - Shake screen
//  - Shake Actor
//    - Do a flip
//  - Emphasis nod
//    - Sprite moves down a bit.

struct Didascalie {
};

/* -- Game Narrative Conditions -- */
// 1- To load and play Scenes,
// 2- To choose between lines in a Scene dynamically
// NOTE: Conditions are naturally AND
struct Conditions {
    u32 dead     [BITFIELD_LEN(UNIT_NUM)];
    u32 alive    [BITFIELD_LEN(UNIT_NUM)];
    u32 recruited[BITFIELD_LEN(UNIT_ORDER_PC_END)];
};
// NOTE: Alive and dead are separate for those conditions:
//      - line REQUIRES unit to be alive        -> alive bit 1
//      - line does not care if unit is alive   -> alive bit 0
//      - line REQUIRES unit to be dead         -> dead bit  1
//      - line does not care if unit is dead    -> dead bit  0
extern struct Conditions Conditions_Game_default;
extern struct Conditions Conditions_Line_default;

struct RawLine {
    struct Conditions conditions;
    s8  speaker;
    s8  rawline;
    u16 speaker_order;
};
extern struct RawLine RawLine_default;

struct Line {
    s8 speaker;
    s8 line;
};
extern struct Line Line_default;

/* A scene is a conversation.
*   - Up to 4 characters sprites on screen at once
*   - Characters can move around screen
*       - Limited animation
*            - Lip sync
*            - blinking
*            - doing a flip
*   - Text bubbles with scrollling text
*/
struct Scene {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

    /* -- Current game condition -- */
    struct Conditions    game_cond;

    /* -- Condition to play scene -- */
    struct Conditions    cond;

    /* -- Raw text tiles -- */
    struct RawLine      *lines_raw;

    /* -- Rendered text tiles -- */
    struct Line         *lines;
    int *rendered; /* indices of rendered lines */

    /* -- Speakers -- */
    u16 id;
    u16 lines_raw_num;
    u16 lines_num;
    u16 actors_num;
    /* -- Didascalies -- */
    // TODO
    struct Didascalie   *didascalies;

    /* -- Dynamic lines -- */
    // Tokens are replaced with
    // Tokens: REPLACE_TOKEN_X, X is int from 0
    s8 *with;

    /* -- Speakers -- */
    u16 *speakers; /* Unit_order */
};
extern struct Scene Scene_default;

/* --- Conditions --- */
b32 Conditions_Compare(struct Conditions *line_cond, struct Conditions *game_cond);

void Conditions_Dead_char(     struct Conditions *cond, char *u);
void Conditions_Alive_char(    struct Conditions *cond, char *u);
void Conditions_Recruited_char(struct Conditions *cond, char *u);

void Conditions_Dead(     struct Conditions *cond, size_t uo);
void Conditions_Alive(    struct Conditions *cond, size_t uo);
void Conditions_Recruited(struct Conditions *cond, size_t uo);

/* --- Constructors/Destructors --- */
void Scene_Free(     struct Scene *scene);

/* --- Replace --- */
// TODO: replace text dynamically
void Scene_Replace_Add(struct Scene *scene, s8 with);

/* --- I/O --- */
void Scene_readJSON( void *s, cJSON *js);
void Scene_writeJSON(void *s, cJSON *js);

// TODO: Split into non human readable conditions/hashes for game
void Conditions_readJSON( void *c, cJSON *jc);
void Conditions_writeJSON(void *c, cJSON *jc);

/* -- Load all scenes with conditions -- */
struct Scene *Scenes_Load(struct Scene *sdarr, struct Conditions *scene_concs,
                          i16 chapter, u16 scene_time);

/* --- Speakers --- */
void Scene_Speaker_Add(struct Scene *scene, u16 order);

/* --- Rendering --- */
/* Read game condition and render text lines */
void Scene_Render(struct Scene *scene);

/* --- Output --- */
void Scene_Raw_Output(   struct Scene *scene, s8 path);
void Scene_Render_Output(struct Scene *scene, s8 path);

/* --- Print --- */
void Scene_Raw_Print(   struct Scene *scene);
void Scene_Render_Print(struct Scene *scene);

#endif /* NARRATIVE_H */
