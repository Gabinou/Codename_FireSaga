#include "minctest.h"
#include "narrative.h"
#include "scene_includer.h"
#include "enums.h"
#include "types.h"
#include "jsonio.h"

void test_narrative() {
    SDL_Log("test_narrative");
    struct Scene test_scene = makeScene_test_scene();
    struct Narrative_Conditions narr1 = Narrative_Conditions_invalid;
    struct Narrative_Conditions narr2 = Narrative_Conditions_invalid;
    struct Narrative_Conditions narr3 = Narrative_Conditions_default;

    narr2.pc_alive = UNIT_PC_ALIVE_KIARA;
    narr3.pc_alive = UNIT_PC_ALIVE_ERWIN;

    narr2.pc_recruited = UNIT_PC_RECRUITED_KIARA;
    narr3.pc_recruited = UNIT_PC_RECRUITED_ERWIN;

    narr2.npc_alive = UNIT_NPC_ALIVE_ZIDINE;
    narr3.npc_alive = UNIT_NPC_ALIVE_ZINEDAN;

    narr1.pc_alive = UNIT_PC_ALIVE_ERWIN + UNIT_PC_ALIVE_KIARA;
    narr1.pc_recruited = UNIT_PC_RECRUITED_ERWIN + UNIT_PC_RECRUITED_KIARA;
    narr1.npc_alive = UNIT_NPC_ALIVE_ZINEDAN + UNIT_NPC_ALIVE_ZIDINE;

    int_chapter_t test_chapter = 0;
    test_scene.lines_chosen = Scene_Lines_Choose(&test_scene, narr1, test_chapter);

    lok((strncmp(test_scene.lines[test_scene.lines_chosen[0]].line, "Erwin First Line: Default", sizeof(test_scene.lines[test_scene.lines_chosen[0]].line)) == 0));
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[1]].line, "Kiara First Line: First conditional of a group of lines", sizeof(test_scene.lines[test_scene.lines_chosen[1]].line)) == 0));
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[2]].line, "Erwin Second Line: Second Conditional", sizeof(test_scene.lines[test_scene.lines_chosen[2]].line)) == 0));
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[3]].line, "Kiara Second Line: Third conditional", sizeof(test_scene.lines[test_scene.lines_chosen[3]].line)) == 0));
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[4]].line, "Erwin Third Line: Default", sizeof(test_scene.lines[test_scene.lines_chosen[4]].line)) == 0));
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[5]].line, "Kiara Third Line: First Conditional", sizeof(test_scene.lines[test_scene.lines_chosen[5]].line)) == 0));

    test_scene.lines_chosen = Scene_Lines_Choose(&test_scene, narr2, test_chapter);
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[0]].line, "Erwin First Line: Default", sizeof(test_scene.lines[test_scene.lines_chosen[0]].line)) == 0));
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[1]].line, "Kiara First Line: Second conditional of a group of lines", sizeof(test_scene.lines[test_scene.lines_chosen[1]].line)) == 0));
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[2]].line, "Erwin Second Line: First Conditional", sizeof(test_scene.lines[test_scene.lines_chosen[2]].line)) == 0));
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[3]].line, "Kiara Second Line: Second conditional", sizeof(test_scene.lines[test_scene.lines_chosen[3]].line)) == 0));
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[4]].line, "Erwin Third Line: Default", sizeof(test_scene.lines[test_scene.lines_chosen[4]].line)) == 0));
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[5]].line, "Kiara Third Line: Default", sizeof(test_scene.lines[test_scene.lines_chosen[5]].line)) == 0));

    test_scene.lines_chosen = Scene_Lines_Choose(&test_scene, narr3, test_chapter);
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[0]].line, "Erwin First Line: Default", sizeof(test_scene.lines[test_scene.lines_chosen[0]].line)) == 0));
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[1]].line, "Kiara First Line: Third conditional of a group of lines", sizeof(test_scene.lines[test_scene.lines_chosen[1]].line)) == 0));
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[2]].line, "Erwin Second Line: Third Conditional", sizeof(test_scene.lines[test_scene.lines_chosen[2]].line)) == 0));
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[3]].line, "Kiara Second Line: First conditional", sizeof(test_scene.lines[test_scene.lines_chosen[3]].line)) == 0));
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[4]].line, "Erwin Third Line: Default", sizeof(test_scene.lines[test_scene.lines_chosen[4]].line)) == 0));
    lok((strncmp(test_scene.lines[test_scene.lines_chosen[5]].line, "Kiara Third Line: Default", sizeof(test_scene.lines[test_scene.lines_chosen[5]].line)) == 0));

    lok((SCENE_START == 0));
    lok((SCENE_test_scene == 1));

    lok(Narrative_isInvalid(Narrative_Conditions_invalid));
    lok(!Narrative_isInvalid(narr1));

    narr1.pc_alive = UNIT_PC_ALIVE_ERWIN + UNIT_PC_ALIVE_KIARA;
    narr2.pc_alive = UNIT_PC_ALIVE_ERWIN;
    lok(Narrative_Match(narr1, narr2));

    narr1.pc_alive = UNIT_PC_ALIVE_ERWIN + UNIT_PC_ALIVE_KIARA;
    narr2.pc_alive = UNIT_PC_ALIVE_KIARA;
    lok(Narrative_Match(narr1, narr2));

    narr1.pc_alive = UNIT_PC_ALIVE_ERWIN + UNIT_PC_ALIVE_KIARA;
    narr2.pc_alive = UNIT_PC_ALIVE_ERWIN + UNIT_PC_ALIVE_KIARA;
    lok(Narrative_Match(narr1, narr2));

    narr1.pc_alive = UNIT_PC_ALIVE_ERWIN + UNIT_PC_ALIVE_KIARA;
    narr2.pc_alive = UNIT_PC_ALIVE_LUCRECE;
    lok(!Narrative_Match(narr1, narr2));

    narr1.pc_alive = 0;
    narr2.pc_alive = UNIT_PC_ALIVE_LUCRECE;
    lok(!Narrative_Match(narr1, narr2));

    narr1.pc_alive = 0;
    narr2.pc_alive = 0;
    narr1.pc_recruited = UNIT_PC_RECRUITED_ERWIN + UNIT_PC_RECRUITED_KIARA;
    narr2.pc_recruited = UNIT_PC_RECRUITED_ERWIN;
    lok(Narrative_Match(narr1, narr2));

    narr1.pc_recruited = UNIT_PC_RECRUITED_ERWIN + UNIT_PC_RECRUITED_KIARA;
    narr2.pc_recruited = UNIT_PC_RECRUITED_KIARA;
    lok(Narrative_Match(narr1, narr2));

    narr1.pc_recruited = UNIT_PC_RECRUITED_ERWIN + UNIT_PC_RECRUITED_KIARA;
    narr2.pc_recruited = UNIT_PC_RECRUITED_ERWIN + UNIT_PC_RECRUITED_KIARA;
    lok(Narrative_Match(narr1, narr2));

    narr1.pc_recruited = UNIT_PC_RECRUITED_ERWIN + UNIT_PC_RECRUITED_KIARA;
    narr2.pc_recruited = UNIT_PC_RECRUITED_LUCRECE;
    lok(!Narrative_Match(narr1, narr2));

    narr1.pc_recruited = 0;
    narr2.pc_recruited = UNIT_PC_RECRUITED_LUCRECE;
    lok(!Narrative_Match(narr1, narr2));

    narr1.pc_recruited = 0;
    narr2.pc_recruited = 0;
    narr1.npc_alive = UNIT_NPC_ALIVE_ZINEDAN + UNIT_NPC_ALIVE_ZIDINE;
    narr2.npc_alive = UNIT_NPC_ALIVE_ZINEDAN;
    lok(Narrative_Match(narr1, narr2));

    narr1.npc_alive = UNIT_NPC_ALIVE_ZIDINE;
    narr2.npc_alive = UNIT_NPC_ALIVE_ZINEDAN;
    lok(!Narrative_Match(narr1, narr2));
    lok(!Narrative_Match(narr2, narr1));

    narr1.npc_alive = 0;
    narr2.npc_alive = UNIT_NPC_ALIVE_ZINEDAN;
    lok(!Narrative_Match(narr1, narr2));
}
