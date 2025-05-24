// TODO pfunc, phase, excl
/* -- Animating and sliding systems before drawing --  */
REGISTER_ENUM(Animate_Cutscene,        TNECS_PIPELINE_RENDER, 0, 1, Cutscene_ID, Position_ID,
              Text_ID, Timer_ID);
REGISTER_ENUM(Slide_Sprite,            TNECS_PIPELINE_RENDER, 0, 0, Sprite_ID,   Position_ID,
              Slider_ID);
REGISTER_ENUM(Slide_Actor,             TNECS_PIPELINE_RENDER, 0, 1, Actor_ID,    Position_ID,
              Slider_ID);
REGISTER_ENUM(Slide_PopUp_Offscreen,   TNECS_PIPELINE_RENDER, 0, 1, PopUp_ID,    Slider_ID,
              SliderOffscreen_ID,
              Position_ID);
REGISTER_ENUM(Hover_Any,               TNECS_PIPELINE_RENDER, 0, 0, Hover_ID,    Position_ID);
REGISTER_ENUM(Animate_Sprite,          TNECS_PIPELINE_RENDER, 0, 0, Sprite_ID,   Position_ID,
              Timer_ID);
// Remove animated flag. Animated sprites must have a timer! Still sprites dont!

/* -- Scrolling Text -- */
/* -- Scrolling Cursor -- */
REGISTER_ENUM(Scroll_Text,             TNECS_PIPELINE_RENDER, 0, 0, PixelFont_ID, Timer_ID);
// REGISTER_ENUM(Cursor_Scroll_Camera,    TNECS_PIPELINE_CONTROL, 0, 0, Sprite_ID, Position_ID, CursorFlag_ID);

/* -- Drawing -- */
/* - for now only drawn sprites are map_units - */
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, 0, 1, Unit_ID, Position_ID, Sprite_ID,
              Timer_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, 0, 1, Unit_ID, Position_ID, Sprite_ID,
              Timer_ID, MapHPBar_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, 0, 1, Unit_ID, Position_ID, Sprite_ID,
              Timer_ID, MapHPBar_ID, AI_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, 0, 1, Unit_ID, Position_ID, Sprite_ID,
              Timer_ID, MapHPBar_ID, AI_ID, Boss_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, 0, 1, Unit_ID, Position_ID, Sprite_ID,
              MapHPBar_ID, AI_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, 0, 1, Unit_ID, Position_ID, Sprite_ID,
              MapHPBar_ID, AI_ID, Boss_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, 0, 1, Unit_ID, Position_ID, Sprite_ID,
              Timer_ID, MapHPBar_ID, AI_ID, UnitMoveAnimation_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, 0, 1, Unit_ID, Position_ID, Sprite_ID,
              Timer_ID, MapHPBar_ID, AI_ID, UnitMoveAnimation_ID, Boss_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, 0, 1, Unit_ID, Position_ID, Sprite_ID,
              MapHPBar_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, 0, 1, Unit_ID, Position_ID,
              Sprite_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, 0, 0, Unit_ID, Position_ID, Sprite_ID,
              RenderTop_ID);
REGISTER_ENUM(Draw_Actor,              TNECS_PIPELINE_RENDER, 0, 1, Actor_ID, Position_ID,
              Slider_ID);
REGISTER_ENUM(Draw_Map_Boss_Icon,      TNECS_PIPELINE_RENDER, 0, 0, Unit_ID, Position_ID, Sprite_ID,
              Boss_ID);
REGISTER_ENUM(Draw_Map_HPBar,          TNECS_PIPELINE_RENDER, 0, 0, Unit_ID, Position_ID,
              MapHPBar_ID);
REGISTER_ENUM(Draw_Menu,               TNECS_PIPELINE_RENDER, 0, 0, Menu_ID);
REGISTER_ENUM(Draw_Text,               TNECS_PIPELINE_RENDER, 0, 1, Text_ID, Position_ID);
REGISTER_ENUM(Draw_Text_Timer,         TNECS_PIPELINE_RENDER, 0, 0, Text_ID, Position_ID, Timer_ID);
REGISTER_ENUM(Draw_PopUp,              TNECS_PIPELINE_RENDER, 0, 0, PopUp_ID, Position_ID);

/* - draw Cursor and Mouse last -> on top - */
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, 0, 0, Sprite_ID,
              Position_ID, CursorFlag_ID);
REGISTER_ENUM(Draw_Sprite,             TNECS_PIPELINE_RENDER, 0, 1, controllerMouse_ID,
              Position_ID, Sprite_ID, MouseFlag_ID);
REGISTER_ENUM(Animate_Combat_onMap,    TNECS_PIPELINE_RENDER, 0, 1, CombatAnimation_ID,   Timer_ID);
REGISTER_ENUM(Animate_Map_Animation,   TNECS_PIPELINE_RENDER, 0, 1, MapAnimation_ID,
              Position_ID, Text_ID, Timer_ID);
REGISTER_ENUM(Animate_Unit_Move_onMap, TNECS_PIPELINE_RENDER, 0, 0, UnitMoveAnimation_ID,
              Position_ID, Timer_ID, Unit_ID);
