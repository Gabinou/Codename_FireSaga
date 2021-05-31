#ifndef SCRIPT_INCLUDER_H
#define SCRIPT_INCLUDER_H

#include "narrative.h"
#include "enums.h"
#include "macros.h"

#define _REGISTER_ENUM(x) SCENE_##x, // cause +1 expansion step?
#define REGISTER_ENUM(x) _REGISTER_ENUM(x)
#define REGISTER_SCENE_START(x)
#define REGISTER_SCENE_END(x)
enum SCENES {
	NO_SCENE = 0,
	SCENE_START = 0,
#include "scene_files.h"
	SCENE_END,
};
#undef REGISTER_ENUM
#undef _REGISTER_ENUM
#undef REGISTER_SCENE_START
#undef REGISTER_SCENE_END

#define SCRIPT_IMPORT
#define REGISTER_SCENE_START(x) REGISTER_SCENE_START_(x)
#define REGISTER_SCENE_START_(x) struct Scene makeScene_##x() {\
SDL_Log(STRINGIZE(makeScene_##x));\
struct Scene temp_scene;\
size_t line_num = 0;\
temp_scene.id = SCENE_##x;\
temp_scene.lines = NULL;\
struct Line temp_line;
#define REGISTER_LINE(a,b,c,d,e,f,g) temp_line = Line_default;\
temp_line.speaker_id = a;\
temp_line.condition.pc_alive = b;\
temp_line.condition.pc_recruited = c;\
temp_line.condition.npc_alive = d;\
temp_line.condition.misc = e;\
temp_line.chapter_min = f;\
temp_line.line = (char *)malloc(8*DEFAULT_BUFFER_SIZE);\
strncpy(temp_line.line, g, sizeof(g));\
arrput(temp_scene.lines, temp_line);\
line_num++;
// printf("HERE1\n");\
// printf(#g);\
// printf("HERE2\n");\

#define REGISTER_SCENE_END(x) REGISTER_SCENE_END_(x)
#define REGISTER_SCENE_END_(x) temp_scene.line_num = line_num;\
return(temp_scene);\
}

#define REGISTER_ENUM(x) STRINGIFY(x.h)

#include "scene_files.h"

#undef REGISTER_SCENE_START 
#undef REGISTER_SCENE_START_ 
#undef REGISTER_LINE 
#undef REGISTER_SCENE_END 
#undef REGISTER_SCENE_END_
#undef REGISTER_ENUM
#undef SCRIPT_IMPORT

struct Scene (**Makers_Scene)();

#define _REGISTER_ENUM(x) arrput(Makers_Scene, &makeScene_##x); // cause +1 expansion step?
#define REGISTER_ENUM(x) _REGISTER_ENUM(x)
#define REGISTER_SCENE_START(x)
#define REGISTER_SCENE_END(x)

bool Makers_Scene_init(struct Game * in_game) {
    bool crashed = 0;
    if (Makers_Scene != NULL) {
        arrfree(Makers_Scene);
    }
    arrput(Makers_Scene, NULL);
#include "scene_files.h"
    return (crashed);
}
#undef REGISTER_ENUM
#undef _REGISTER_ENUM
#undef REGISTER_SCENE_START
#undef REGISTER_SCENE_END

#endif /* SCRIPT_H */
