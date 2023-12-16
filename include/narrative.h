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
    u16 bits;
    u32 *alive;
    u32 *recruited;
    u32 *dead;
};

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

    u16 id;
    u16 line_num;
    u16 line_len;
    u16 replace_num;
    u16 actors_num;
    struct Conditions *line_conds;
    // TODO: Didascalies

    char **lines;
    // All to_replace are *between stars*
    // Better for replaces to be LONGER than withs
    char **replace;
    char **with;
    u16   *speakers;
    u16   *actors;
} extern Scene_default;

void Scene_Free(     struct Scene *scene);
void Scene_Free_Read(struct Scene *scene);

void Scene_Prune(  struct Scene *scene);
void Scene_Replace(struct Scene *scene);

struct Scene *Scenes_Load(struct Scene *sdarr, struct Conditions *scene_concs,
                          i16 chapter, u16 scene_time);

/* --- Rendering Scene --- */
/* Read game condition and render text lines */
void Scene_Render(struct Scene *scene);

/* Output scene to text file */
void Scene_Render_output(struct Scene *scene);

#endif /* NARRATIVE_H */
