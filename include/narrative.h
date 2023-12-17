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
#include "SDL.h"

/* -- Glossary -- */
// - Didascalie (theater vocabulary)
//      - Note to actors of a scene about what to *do* during a scene

/* -- Game Narrative Conditions -- */
// 1- To load and play Scenes,
// 2- To choose between lines in a Scene dynamically
// NOTE: Conditions are naturally AND

struct Conditions {
    u32 alive    [BITFIELD_LEN(UNIT_ORDER_NPC_END)];
    u32 recruited[BITFIELD_LEN(UNIT_ORDER_PC_END)];
};
extern struct Conditions Conditions_default;

struct RawLine {
    struct Conditions conditions;
    s8 speaker;
    s8 rawline;
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

    // Current game condition
    struct Conditions    game_cond;

    // Condition to play scene
    struct Conditions    cond;

    struct RawLine      *lines_raw;
    struct Line         *lines;

    u16 id;
    u16 lines_raw_num;
    u16 lines_num;
    // u16 line_len;
    u16 replace_num;
    u16 actors_num;
    u64 path_hash;
    // TODO: Didascalies

    // char **lines;

    // All to_replace are *between stars*
    // Better for replaces to be LONGER than widths
    // char **replace;
    // TODO:
    // char **with;
    u16   *speakers;
    // u16   *actors;
};
extern struct Scene Scene_default;

/* --- Conditions --- */
b32 Conditions_Compare(struct Conditions *conds1, struct Conditions *conds2);

/* --- Constructors/Destructors --- */
void Scene_Free(     struct Scene *scene);

/* --- Replace --- */
// TODO: replace text dynamically
void Scene_Replace(struct Scene *scene);
void Scene_Replace_Add(struct Scene *scene, s8 replace, s8 with);

/* --- I/O --- */
void Scene_readJSON( void *s, cJSON *js);
void Scene_writeJSON(void *s, cJSON *js);

void Conditions_readJSON( void *c, cJSON *jc);
void Conditions_writeJSON(void *c, cJSON *jc);

/* -- Load all scenes with conditions -- */
struct Scene *Scenes_Load(struct Scene *sdarr, struct Conditions *scene_concs,
                          i16 chapter, u16 scene_time);

/* --- Rendering --- */
/* Read game condition and render text lines */
void Scene_Render(struct Scene *scene);

/* --- Output --- */
void Scene_Raw_Output(      struct Scene *scene, s8 path);
void Scene_Render_Output(   struct Scene *scene, s8 path);

/* --- Print --- */
void Scene_Raw_Print(   struct Scene *scene);
void Scene_Render_Print(struct Scene *scene);

#endif /* NARRATIVE_H */
