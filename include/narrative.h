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

/* -- Game Narrative Conditions -- */
// 1- To load and play Scenes,
// 2- To choose between lines in a Scene dynamically
// NOTE: Conditions are naturally AND
struct Conditions {
    uf16 bits;
    u32 *alive;
    u32 *recruited;
    u32 *dead;
};

struct Scene {
    u8 json_element;
    uf16 id;
    uf16 line_num;
    uf16 line_len;
    uf16 replace_num;
    uf16 actors_num;
    struct Conditions *line_conds;
    char **lines;
    // All to_replace are *between stars*
    // Better for replaces to be LONGER than withs
    char **replace;
    char **with;
    uf16 *speakers;
    uf16 *actors;
} extern Scene_default;

// Note: Scenes don't get written, no writing
extern bool Conditions_Read(struct Conditions *conditions, cJSON *jconditions);
extern bool Condition_Read(u32 *bitfield, size_t bits, cJSON *jcondition);

extern void Scene_Free(struct Scene *scene);
extern void Scene_Free_Read(struct Scene *scene);
extern void Scene_Replace(struct Scene *scene);
extern void Scene_Prune(struct Scene *scene);
extern struct Scene *Scenes_Load(struct Scene *sdarr, struct Conditions *scene_concs,
                                 if16 chapter, uf16 scene_time);

/* --- I/O --- */
extern void Scene_readJSON(void *input, const struct cJSON *const jnarr);

#endif /* NARRATIVE_H */
