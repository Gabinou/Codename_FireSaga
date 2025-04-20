#ifndef ACTOR_H
#define ACTOR_H

#include "enums.h"
// Actor in the theater/play sense
// Spritesheet requirements
//  - Body
//      - Y per character/actor spritesheet
//      - Promotions, clothes changes
//      - expressions?
//  - Faces
//      - X expressions: neutral, happy, angry?
//  - Lips
//      - Speaking: 4 frames
//      - X expressions: neutral, happy, angry?
//  - Eyes
//      - Blinking: 3-4 frames
//      - X expressions: neutral, happy, angry?
//
//  - Must have 1 spritesheet per character.
//      - Share enums (neutral, happy, angry) as possible
//  - Extensible.
//      - Any new body, eyes or lips must be easy to add
//  - Rigid limits.
//      - Frame number (blinking, lips)
//      - Resolution of portraits, lips, eyes

enum ACTOR_SPRITESHEEET {
    /* Order of element columns on spritesheets */
    ACTOR_BODY_ELEM     = 0,
    ACTOR_FACE_ELEM     = 1,
    ACTOR_LIPS_ELEM     = 2,
    ACTOR_EYES_ELEM     = 3,

    /* Size of elements on spritesheet */
    ACTOR_BODY_WIDTH    = 96, // FE8
    ACTOR_FACE_WIDTH    = 24,
    ACTOR_LIPS_WIDTH    = 24,
    ACTOR_EYES_WIDTH    = 24,
    ACTOR_BODY_HEIGHT   = 80, // FE8
    ACTOR_FACE_HEIGHT   = 24,
    ACTOR_LIPS_HEIGHT   = 24,
    ACTOR_EYES_HEIGHT   = 24,

    /* Number of frames per animated element */
    ACTOR_LIPS_FRAMES   = 4,
    ACTOR_EYES_FRAMES   = 3,

    /* Position of elements columns on spritesheet */
    ACTOR_BODY_X        = 0,
    ACTOR_FACE_X        = ACTOR_BODY_X + ACTOR_BODY_WIDTH,
    ACTOR_LIPS_X        = ACTOR_FACE_X + ACTOR_FACE_WIDTH,
    ACTOR_EYES_X        = ACTOR_LIPS_X + ACTOR_LIPS_WIDTH,
};

/* Position of elements rows on spritesheet */
i32 Actor_Body_Y(i32 face_id);
i32 Actor_Face_Y(i32 face_id);
i32 Actor_Lips_Y(i32 lips_id);
i32 Actor_Eyes_Y(i32 eyes_id);

typedef struct Actor {
    /* Current rendered element */
    i32 body_id;
    i32 face_id;
    i32 lips_id;
    i32 eyes_id;
} Actor;

#endif /* ACTOR_H */
