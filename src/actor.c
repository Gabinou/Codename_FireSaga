#include "actor.h"


/* Position of elements rows on spritesheet */
//  Every character has different numbers of elements
i32 Actor_Body_Y(i32 body_id) {
    // TODO: check for valid index
    return (ACTOR_BODY_HEIGHT * body_id);
}

i32 Actor_Face_Y(i32 face_id) {
    // TODO: check for valid index
    return (ACTOR_FACE_HEIGHT * face_id);
}

i32 Actor_Lips_Y(i32 lips_id) {
    // TODO: check for valid index
    return (ACTOR_LIPS_HEIGHT * ACTOR_LIPS_FRAMES * lips_id);
}

i32 Actor_Eyes_Y(i32 eyes_id) {
    // TODO: check for valid index
    return (ACTOR_EYES_HEIGHT * ACTOR_EYES_FRAMES * eyes_id);
}

