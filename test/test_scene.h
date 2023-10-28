#include "nourstest.h"
#include "narrative.h"
#include "enums.h"
#include "types.h"
#include "jsonio.h"

void test_scene() {
    
    /* -- Setup conditions -- */
    struct Conditions conditions;
    u32 *alive = BITFIELD_INIT_BITS(UNIT_ID_END);
    Bitfield_On(alive, UNIT_ID_ERWIN - 1);
    u32 *dead = BITFIELD_INIT_BITS(UNIT_ID_END);
    Bitfield_On(dead, UNIT_ID_HAMILCAR - 1);
    Bitfield_On(dead, UNIT_ID_ZIDINE - 1);
    u32 *recruited = BITFIELD_INIT_BITS(UNIT_ID_END);
    Bitfield_On(recruited, UNIT_ID_SILOU - 1);
    struct Scene scene = Scene_default;
    conditions.alive = alive;
    conditions.recruited = recruited;
    conditions.dead = dead;
    conditions.bits = UNIT_ID_END;

    /* -- Loading lines with conditions -- */
    scene.line_conds = &conditions;
    PHYSFS_mount("scenes", NULL, 1);
    jsonio_readJSON("scenes"PHYSFS_SEPARATOR"example.json", &scene);
    nourstest_true(strcmp("I am a line with conditions. I only show up if all conditions are met.",
                          scene.lines[0]) == 0);
    nourstest_true(strcmp("I am a line with no conditions.", scene.lines[1]) == 0);
    nourstest_true(strcmp("I am also a line with no conditions.", scene.lines[2]) == 0);
    nourstest_true(strcmp("I am a also line with conditions. I only show up if all conditions are met.",
                          scene.lines[3]) == 0);
    nourstest_true(scene.speakers[0] == UNIT_ID_ERWIN);
    nourstest_true(scene.speakers[1] == UNIT_ID_OTTO);
    nourstest_true(scene.speakers[2] == UNIT_ID_SEBASTIAN);
    nourstest_true(scene.speakers[3] == UNIT_ID_SILOU);
    nourstest_true(scene.line_num == 5);

    /* -- Loading scenes with conditions -- */
    struct Scene *scenes = NULL;
    scenes = Scenes_Load(scenes, &conditions, CHAPTER_FOLDER_TEST, SCENE_TIME_PRE_CHAPTER);
    nourstest_true(DARR_NUM(scenes) == 1);
    scene = scenes[0];
    nourstest_true(strcmp("You know what I'll get fer that?", scene.lines[0]) == 0);
    nourstest_true(strcmp("Treason to the Empire!", scene.lines[1]) == 0);
    nourstest_true(strcmp("You know what you'll get fer that? Harboring traitors!",
                          scene.lines[2]) == 0);
    nourstest_true(strcmp("Even country boys know what the Empire does to people like that!",
                          scene.lines[3]) == 0);
    nourstest_true(strcmp("We needs your protection! The savages keep stealing!", scene.lines[4]) == 0);
    nourstest_true(strcmp("Now, the Duke of Gaolin tells me to march.", scene.lines[5]) == 0);

    nourstest_true(scene.speakers[0] == UNIT_ID_IGNELL);
    nourstest_true(scene.speakers[1] == UNIT_ID_IGNELL);
    nourstest_true(scene.speakers[2] == UNIT_ID_IGNELL);
    nourstest_true(scene.speakers[3] == UNIT_ID_IGNELL);
    nourstest_true(scene.speakers[4] == UNIT_ID_SEBASTIAN);
    nourstest_true(scene.speakers[5] == UNIT_ID_IGNELL);
    nourstest_true(scene.line_num == 6);

    /* -- Setup conditions for OR -- */
    Bitfield_On(alive, UNIT_ID_SILOU - 1);
    Bitfield_On(recruited, UNIT_ID_OTTO - 1);
    Bitfield_Off(recruited, UNIT_ID_SILOU - 1);
    Bitfield_Off(alive, UNIT_ID_ERWIN - 1);
    Bitfield_Off(dead, UNIT_ID_SEBASTIAN - 1);
    Bitfield_Off(dead, UNIT_ID_HAMILCAR - 1);
    conditions.alive = alive;
    conditions.recruited = recruited;
    conditions.dead = dead;
    scene.line_conds = &conditions;
    jsonio_readJSON("scenes"PHYSFS_SEPARATOR"example.json", &scene);
    nourstest_true(strcmp("I am a line with no conditions.", scene.lines[0]) == 0);
    nourstest_true(strcmp("I am also a line with no conditions.", scene.lines[1]) == 0);
    nourstest_true(scene.line_num == 3);

    Bitfield_On(dead, UNIT_ID_HAMILCAR - 1);
    jsonio_readJSON("scenes"PHYSFS_SEPARATOR"example.json", &scene);
    nourstest_true(scene.line_num == 4);
    nourstest_true(strcmp("I should show up.", scene.lines[2]) == 0);

    Bitfield_Off(dead, UNIT_ID_HAMILCAR - 1);
    Bitfield_On(dead, UNIT_ID_SEBASTIAN - 1);
    jsonio_readJSON("scenes"PHYSFS_SEPARATOR"example.json", &scene);
    nourstest_true(scene.line_num == 4);
    nourstest_true(strcmp("I should show up.", scene.lines[2]) == 0);
    Scene_Free(&scene);
    DARR_FREE(scenes);

    /* -- Replace -- */
    Bitfield_Off(alive, UNIT_ID_SILOU - 1);
    Bitfield_Off(recruited, UNIT_ID_OTTO - 1);
    Bitfield_Off(recruited, UNIT_ID_SILOU - 1);
    Bitfield_Off(alive, UNIT_ID_ERWIN - 1);
    Bitfield_Off(dead, UNIT_ID_SEBASTIAN - 1);
    Bitfield_Off(dead, UNIT_ID_HAMILCAR - 1);
    jsonio_readJSON("scenes"PHYSFS_SEPARATOR"example.json", &scene);
    nourstest_true(strcmp(scene.lines[2], "I am a dynamic line spoken by *biggestbaka*.") == 0);

    size_t len = 24;
    scene.replace_num = 1;
    scene.replace = SDL_malloc(sizeof(*scene.replace));
    scene.replace[0] = SDL_calloc(len, 1);
    strncpy(scene.replace[0], "*biggestbaka*\0", len);

    scene.with = SDL_malloc(sizeof(*scene.replace));
    scene.with[0] = SDL_calloc(len, 1);
    strncpy(scene.with[0], "LONGERRRR WORDDDDDD\0", len);
    Scene_Replace(&scene);
    nourstest_true(strcmp(scene.lines[0], "I am a line with no conditions.") == 0);
    nourstest_true(strcmp(scene.lines[1], "I am also a line with no conditions.") == 0);
    nourstest_true(strcmp(scene.lines[2], "I am a dynamic line spoken by LONGERRRR WORDDDDDD.") == 0);

    jsonio_readJSON("scenes"PHYSFS_SEPARATOR"example.json", &scene);
    nourstest_true(strcmp(scene.lines[2], "I am a dynamic line spoken by *biggestbaka*.") == 0);
    strncpy(scene.with[0], "a baka\0", len);

    Scene_Replace(&scene);
    nourstest_true(strcmp(scene.lines[0], "I am a line with no conditions.") == 0);
    nourstest_true(strcmp(scene.lines[1], "I am also a line with no conditions.") == 0);
    nourstest_true(strcmp(scene.lines[2], "I am a dynamic line spoken by a baka.") == 0);
    Scene_Prune(&scene);
    nourstest_true(strcmp(scene.lines[0], "I am a line with no conditions.") == 0);
    nourstest_true(strcmp(scene.lines[1], "I am also a line with no conditions.") == 0);
    nourstest_true(strcmp(scene.lines[2], "I am a dynamic line spoken by a baka.") == 0);
}
