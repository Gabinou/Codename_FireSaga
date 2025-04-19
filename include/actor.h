
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
// Enum example:
//  - FACE_WIDTH    = 96; // FE8
//  - FACE_HEIGHT   = 80; // FE8
//  - FACE_X = 0;
//  - LIPS_X = FACE_X + FACE_WIDTH;
//  - EYES_X = LIPS_X + LIPS_WIDTH;
//  - FACE_<>_Y = EXPRESSION_<> * FACE_HEIGHT;
//  - LIPS_FRAMES_NUM = 4;
//  - EYES_FRAMES_NUM = 3;
//  - LIPS_<>_Y = EXPRESSION_<> * LIPS_FRAMES_NUM * LIPS_HEIGHT;
//  - EYES_<>_Y = EXPRESSION_<> * EYES_FRAMES_NUM * EYES_HEIGHT;

