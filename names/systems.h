/* -- Animating and sliding systems before drawing --  */
/* TODO: Render pipeline, animate phase  */
REGISTER_ENUM(Draw_Scene,               TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_BACKGROUND, 0,
              Scene_ID);
REGISTER_ENUM(Draw_Map,                 TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_BACKGROUND, 0,
              Map_ID);
REGISTER_ENUM(Animate_Cutscene,         TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_ANIMATE, 1,
              Cutscene_ID, Position_ID,
              Text_ID, Timer_ID);

/* TODO: Slide pipeline? render pipeline, animate phase?   */
REGISTER_ENUM(Slide_Sprite,            TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_MOVE, 0, Sprite_ID,
              Position_ID,
              Slider_ID);
REGISTER_ENUM(Slide_Actor,             TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_MOVE, 1, Actor_ID,
              Position_ID,
              Slider_ID);
REGISTER_ENUM(Slide_PopUp_Offscreen,   TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_MOVE, 1, PopUp_ID,
              Slider_ID,
              SliderOffscreen_ID,
              Position_ID);
REGISTER_ENUM(Hover_Any,               TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_MOVE, 0, Hover_ID,
              Position_ID);

REGISTER_ENUM(Animate_Sprite,          TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_ANIMATE, 0,
              Sprite_ID,   Position_ID,
              Timer_ID);
// Remove animated flag. Animated sprites must have a timer! Still sprites dont!

/* -- Scrolling Text -- */
/* -- Scrolling Cursor -- */
REGISTER_ENUM(Scroll_Text,              TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_ANIMATE, 0,
              PixelFont_ID, Timer_ID);
/* TODO: Control pipeline  */
REGISTER_ENUM(Control_Gamepad,          TNECS_PIPELINE_CONTROL, 0, 0, Slider_ID, Position_ID,
              controllerGamepad_ID);
REGISTER_ENUM(Control_Keyboard,         TNECS_PIPELINE_CONTROL, 0, 0, Slider_ID, Position_ID,
              controllerKeyboard_ID);
REGISTER_ENUM(Control_Touchpad,         TNECS_PIPELINE_CONTROL, 0, 0, Slider_ID, Position_ID,
              controllerTouchpad_ID);
REGISTER_ENUM(Cursor_Follows_Mouse,     TNECS_PIPELINE_CONTROL, 0, 0, Sprite_ID, Position_ID,
              CursorFlag_ID);
REGISTER_ENUM(Cursor_Moves,             TNECS_PIPELINE_CONTROL, 0, 0, Sprite_ID, Position_ID,
              CursorFlag_ID);

/* -- Drawing -- */
/* TODO: phase ?? of rendering pipeline  */
/* - for now only drawn sprites are map_units - */
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 1, Unit_ID,
              Position_ID, Sprite_ID, Timer_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 1, Unit_ID,
              Position_ID, Sprite_ID, Timer_ID, MapHPBar_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 1, Unit_ID,
              Position_ID, Sprite_ID, Timer_ID, MapHPBar_ID, Unit_AI_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 1, Unit_ID,
              Position_ID, Sprite_ID, Timer_ID, MapHPBar_ID, Unit_AI_ID, Boss_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 1, Unit_ID,
              Position_ID, Sprite_ID, MapHPBar_ID, Unit_AI_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 1, Unit_ID,
              Position_ID, Sprite_ID, MapHPBar_ID, Unit_AI_ID, Boss_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 1, Unit_ID,
              Position_ID, Sprite_ID, Timer_ID, MapHPBar_ID, Unit_AI_ID, UnitMoveAnimation_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 1, Unit_ID,
              Position_ID, Sprite_ID, Timer_ID, MapHPBar_ID, Unit_AI_ID, UnitMoveAnimation_ID, Boss_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 1, Unit_ID,
              Position_ID, Sprite_ID, MapHPBar_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 1, Unit_ID,
              Position_ID, Sprite_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 0, Unit_ID,
              Position_ID, Sprite_ID, RenderTop_ID);
REGISTER_ENUM(Draw_Actor,              TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 1, Actor_ID,
              Position_ID, Slider_ID);
REGISTER_ENUM(Draw_Map_Boss_Icon,      TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 0, Unit_ID,
              Position_ID, Sprite_ID, Boss_ID);
REGISTER_ENUM(Draw_Map_HPBar,          TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 0, Unit_ID,
              Position_ID, MapHPBar_ID);
REGISTER_ENUM(Draw_Menu,               TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 0, Menu_ID);
REGISTER_ENUM(Draw_Text,               TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 1, Text_ID,
              Position_ID);
REGISTER_ENUM(Draw_Text_Timer,         TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 0, Text_ID,
              Position_ID, Timer_ID);
REGISTER_ENUM(Draw_PopUp,              TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_DRAW, 0, PopUp_ID,
              Position_ID);

/* - draw Cursor and Mouse last -> on top - */
/* TODO: Cursor phase of rendering pipeline  */
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_CURSOR, 0,
              Sprite_ID,
              Position_ID, CursorFlag_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_CURSOR, 1,
              controllerMouse_ID,
              Position_ID, Sprite_ID, MouseFlag_ID);

REGISTER_ENUM(Animate_Combat_onMap,    TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_ANIMATE, 1,
              CombatAnimation_ID,   Timer_ID);
REGISTER_ENUM(Animate_Map_Animation,   TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_ANIMATE, 1,
              MapAnimation_ID,
              Position_ID, Text_ID, Timer_ID);
REGISTER_ENUM(Animate_Unit_Move_onMap, TNECS_PIPELINE_RENDER, TNECS_RENDER_PHASE_ANIMATE, 0,
              UnitMoveAnimation_ID,
              Position_ID, Timer_ID, Unit_ID);

/* - Turn End - */
REGISTER_ENUM(System_Cooldown_Tick, TNECS_PIPELINE_TURN_END, 0, 0,
              Inventory_item_ID, Cooldown_ID);
