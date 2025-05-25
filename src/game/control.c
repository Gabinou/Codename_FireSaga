#include "game/control.h"
#include "controller/gamepad.h"
#include "controller/keyboard.h"
#include "slider.h"
#include "events.h"
#include "position.h"

// void Control_Cursor_Moves(struct Game *sota,    struct Point cursor_move,
//                           struct Point target,  struct Point px_pos,
//                           i32 controller_type) {
//     /* - Immobile cursor - */
//     if ((cursor_move.x == 0) && (cursor_move.y == 0)) {
//         return;
//     }

//     sota->cursor.frame_moved = true;

//     /* - Pause cursor after moving - */
//     i32 min     = 0;
//     i32 max     = CURSOR_FIRSTMOVE_PAUSE_ms;
//     i32 moved   = sota->cursor.moved_time_ms;
//     if ((moved > min) && (moved < max)) {
//         return;
//     }

//     sota->cursor.move     = cursor_move;
//     sota->inputs.controller_code = controller_type;
// }

// void Gamepad_Pressed(i8 sota_b, i8 *press, i8 *pressed_num, i32 *controller_type,
//                      u32 event, struct controllerGamepad *gp) {
//     i32 theld       = gp->timeheld_button_ns;
//     i32 min_held    = GAMEPAD_MINHELD_ns;
//     b32 butblk      = gp->block_buttons;

//     Control_Pressed(sota_b, press, pressed_num, controller_type,
//                     event, butblk, theld, min_held);
// }

// void Keyboard_Pressed(i8 sota_b, i8 *press, i8 *pressed_num, i32 *controller_type,
//                       u32 event, struct controllerKeyboard *kb) {
//     i32 theld       = kb->timeheld_button_ns;
//     i32 min_held    = KEYBOARD_MINHELD_ns;
//     b32 butblk      = kb->block_buttons;

//     Control_Pressed(sota_b, press, pressed_num, controller_type,
//                     event, butblk, theld, min_held);
// }

// void Control_Pressed(i8 sota_b, i8 *press, i8 *pressed_num, i32 *controller_type,
//                      u32 event, b32 block, i32 t_held_ns, i32 t_min_ns) {
//     press[(*pressed_num)++] = sota_b;
//     if (block) {
//         return;
//     }
//     if (event <= 0) {
//         return;
//     }
//     if ((t_min_ns <= 0) || (t_held_ns > t_min_ns)) {
//         // NOTE: 1- User input is blocked in FSM.
//         //       2- Control System always sends events.
//         Event_Emit(__func__, SDL_USEREVENT, event, controller_type, NULL);
//     }
// }

// void Game_Control_Keyboard(struct Game *sota) {
//     /* --- PRELIMINARIES --- */
//     SDL_assert(sota != NULL);
//     struct Point cursor_move = {0};

//     struct Slider             *sld;
//     struct Position           *pos;
//     struct controllerKeyboard *kb;

//     sld = IES_GET_COMPONENT(sota->ecs.world, sota->cursor.entity, Slider);
//     pos = IES_GET_COMPONENT(sota->ecs.world, sota->cursor.entity, Position);
//     kb  = IES_GET_COMPONENT(sota->ecs.world, sota->cursor.entity, controllerKeyboard);

//     /* -- Preliminaries -- */
//     i32                       *ct       = &kb->controller_type;
//     const u8                  *kb_state = SDL_GetKeyboardState(NULL);

//     /* -- Keyboard button checking -- */
//     i8 press[SOTA_BUTTON_END];
//     i8 pnum          = 0;
//     size_t *mheld    = &kb->held_move_num;
//     i32    *theld    = &kb->timeheld_button_ns;
//     i32    *theld_ns = &kb->timeheld_move_ns;
//     size_t *bheld    = &kb->held_button_num;

//     for (int sota_b = SOTA_BUTTON_A; sota_b < SOTA_BUTTON_END; sota_b++) {
//         if (Keyboard_isPressed(kb, kb_state, sota_b))
//             Keyboard_Pressed(sota_b, press, &pnum, ct, sota->inputs.arr[sota_b], kb);
//     }
//     u64 updateTime_ns = SOTA_ns / sota->settings.FPS.cap;
//     Keyboard_Held(kb->held_button, bheld, theld, press, pnum, updateTime_ns);
//     /* - Unblock keyboard - */
//     if ((*theld) == 0)
//         kb->block_buttons = false;

//     /* -- Keyboard move checking -- */
//     // i8 pmove[SOTA_DIRECTIONS_MAIN_NUM];
//     // i8 pmove_num = 0;
//     b32 right = Keyboard_isPressed(kb, kb_state, SOTA_INPUT_RIGHT);
//     b32 up    = Keyboard_isPressed(kb, kb_state, SOTA_INPUT_UP);
//     b32 left  = Keyboard_isPressed(kb, kb_state, SOTA_INPUT_LEFT);
//     b32 down  = Keyboard_isPressed(kb, kb_state, SOTA_INPUT_DOWN);

//     /* - Collapse diagonals to one of 4 main directions - */
//     if (up && !down) {
//         cursor_move.y       = -1;
//         // pmove[pmove_num++]  = SOTA_INPUT_UP;
//     } else if (down && !up) {
//         cursor_move.y       =  1;
//         // pmove[pmove_num++]  = SOTA_INPUT_DOWN;
//     }
//     if (left && !right) {
//         cursor_move.x       = -1;
//         // pmove[pmove_num++]  = SOTA_INPUT_LEFT;
//     } else if (right && !left) {
//         cursor_move.x       =  1;
//         // pmove[pmove_num++]  = SOTA_INPUT_RIGHT;
//     }

//     Keyboard_Held(kb->held_move, mheld, theld_ns, press, pnum, updateTime_ns);

//     struct Point target    = sld->target;
//     struct Point pixel_pos = pos->pixel_pos;
//     Control_Cursor_Moves(sota, cursor_move, target, pixel_pos, *ct);
// }

// void Game_Control_Gamepad(struct Game *sota) {
//     /* --- PRELIMINARIES --- */
//     SDL_assert(sota != NULL);
//     struct Point cursor_move = {0};

//     /* -- Get game -- */
//     struct Slider             *sld;
//     struct Position           *pos;
//     struct controllerGamepad  *gp;

//     sld = IES_GET_COMPONENT(sota->ecs.world, sota->cursor.entity, Slider);
//     pos = IES_GET_COMPONENT(sota->ecs.world, sota->cursor.entity, Position);
//     gp  = IES_GET_COMPONENT(sota->ecs.world, sota->cursor.entity, controllerGamepad);

//     /* -- Preliminaries -- */
//     i8 press[SOTA_BUTTON_END];
//     i8 pnum = 0;
//     size_t  *mheld    = &gp->held_move_num;
//     i32     *theld_ns = &gp->timeheld_move_ns;
//     i32     *theld    = &gp->timeheld_button_ns;
//     size_t  *bheld    = &gp->held_button_num;

//     /* -- Gamepad button checking -- */
//     for (int sota_b = SOTA_BUTTON_A; sota_b <= SOTA_BUTTON_TRIGGER_RIGHT; sota_b++) {
//         if (Gamepad_isPressed(gp, sota_b))
//             Gamepad_Pressed(sota_b, press, &pnum, &gp->controller_type, sota->inputs.arr[sota_b], gp);
//     }

//     u64 updateTime_ns = SOTA_ns / sota->settings.FPS.cap;
//     Gamepad_Held(gp->held_button, bheld, theld, press, pnum, updateTime_ns);

//     /* - Unblock gamepad - */
//     if (gp->timeheld_button_ns == 0)
//         gp->block_buttons = false;

//     /* -- Gamepad move checking -- */
//     i8 pmove[SOTA_DIRECTIONS_MAIN_NUM];
//     i8 pmove_num = 0;

//     cursor_move = Gamepad_Joystick_Direction(gp);
//     if (cursor_move.x > 0) {
//         pmove[pmove_num++]  = SOTA_INPUT_RIGHT;
//     } else if (cursor_move.x < 0) {
//         pmove[pmove_num++]  = SOTA_INPUT_LEFT;
//     }
//     /* - Up/Down axis - */
//     if  (cursor_move.y > 0) {
//         pmove[pmove_num++]  = SOTA_INPUT_DOWN;
//     } else if  (cursor_move.y < 0)  {
//         pmove[pmove_num++]  = SOTA_INPUT_UP;
//     }

//     /* - Move buttons - */
//     /* Priority of move buttons over joystick */
//     if (Gamepad_isPressed(gp, SOTA_BUTTON_DPAD_RIGHT)) {
//         cursor_move.x       =  1;
//         pmove[pmove_num++]  = SOTA_INPUT_RIGHT;
//     } else if (Gamepad_isPressed(gp, SOTA_BUTTON_DPAD_LEFT)) {
//         cursor_move.x       = -1;
//         pmove[pmove_num++]  = SOTA_INPUT_LEFT;
//     }
//     if (Gamepad_isPressed(gp, SOTA_BUTTON_DPAD_UP)) {
//         cursor_move.y       = -1;
//         pmove[pmove_num++]  = SOTA_INPUT_UP;
//     } else if (Gamepad_isPressed(gp, SOTA_BUTTON_DPAD_DOWN)) {
//         cursor_move.y       =  1;
//         pmove[pmove_num++]  = SOTA_INPUT_DOWN;
//     }

//     Gamepad_Held(gp->held_move, mheld, theld_ns, pmove, pmove_num, updateTime_ns);

//     struct Point target    = sld->target;
//     struct Point pixel_pos = pos->pixel_pos;
//     Control_Cursor_Moves(sota, cursor_move, target, pixel_pos, gp->controller_type);
// }

// void Game_Control_Touchpad(struct Game *sota) {

// }
