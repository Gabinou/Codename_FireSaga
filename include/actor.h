#ifndef ACTOR_H
#define ACTOR_H

// Actor in the theater/play sense
// Spritesheet requirements
//  - Mouth
//      - Speaking: 4 frames
//      - X expressions: neutral, happy, angry?
//  - Eyes
//      - Blinking: 3-4 frames
//      - X expressions: neutral, happy, angry?
//  - Clothes (multiple bodies)
//      - Y per character/actor spritesheet
//
//  - Must have 1 spritesheet per character.
//      - Share enums (neutral, happy, angry) as possible
//  - Extensible.
//      - Any new body, eyes or lips must be easy to add
//  - Rigid limits.
//      - Frame number (blinking, lips)
//      - Resolution of portraits, lips, eyes
//

enum ACTOR_SPRITESHEEET {
    /* Size of elements on spritesheet */
    ACTOR_FACE_WIDTH    = 96, // FE8
    ACTOR_FACE_HEIGHT   = 80, // FE8
    ACTOR_LIPS_WIDTH    = 24,
    ACTOR_LIPS_HEIGHT   = 24,
    ACTOR_EYES_WIDTH    = 24,
    ACTOR_EYES_HEIGHT   = 24,
    ACTOR_LIPS_FRAMES   = 4,
    ACTOR_EYES_FRAMES   = 3,

    /* Position of elements columns on spritesheet */
    ACTOR_FACE_X        = 0,
    ACTOR_LIPS_X        = ACTOR_FACE_X + ACTOR_FACE_WIDTH,
    ACTOR_EYES_X        = ACTOR_LIPS_X + ACTOR_LIPS_WIDTH,
};

/* Position of elements rows on spritesheet */
i32 Actor_Face_Y(i32 face_id);
i32 Actor_Lips_Y(i32 lips_id);
i32 Actor_Eyes_Y(i32 eyes_id);

#endif /* SCENE_H */
