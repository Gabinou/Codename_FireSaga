#ifndef NARRATIVE_H
#define NARRATIVE_H

#include "types.h"
#include "enums.h"
#include "jsonio.h"
#include <stdio.h>
#include <string.h>

struct Page {
    char ** paragraphs;
    char title[DEFAULT_BUFFER_SIZE];
    char image[DEFAULT_BUFFER_SIZE];
} extern Page_default;

struct Book {
    struct Page * pages;
    char title[DEFAULT_BUFFER_SIZE];
    char author[DEFAULT_BUFFER_SIZE];
    int_id_t id;
    bool sellable;
} extern Book_default;

struct Narrative_Conditions {
    uint64_t pc_alive;
    uint64_t pc_recruited;
    uint64_t npc_alive;
    uint64_t misc;
} extern Narrative_Conditions_default, Narrative_Conditions_invalid;

struct Line {
    int_id_t speaker_id;
    struct Narrative_Conditions condition; // default line is Narrative_Conditions_invalid
    uint8_t chapter_min;
    char * line;
} extern Line_default;

struct Scene {
    uint8_t json_element;
    int_id_t id;
    struct Narrative_Conditions condition;
    struct Line * lines; // DEFAULT LINE LAST with PRIORITY CONDITIONS FIRST
    size_t * lines_chosen;
    size_t line_num;
    int_id_t * participants;
} extern Scene_default;

extern size_t * Scene_Lines_Choose(struct Scene * in_scene, struct Narrative_Conditions narr_current, int_chapter_t in_chapter);
extern int_id_t * Scene_Participants_Determine(struct Scene * in_scene, size_t * in_chosen, size_t chosen_num);
extern bool Narrative_Match(struct Narrative_Conditions narr_current, struct Narrative_Conditions conditions);
extern bool Narrative_isInvalid(struct Narrative_Conditions narr_current);

#endif /* NARRATIVE_H */
