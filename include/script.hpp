#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <string>
#include "enums.hpp"
#include "filesystem.hpp"
#include "unit.hpp"
#include "utilities.hpp"
#include "structs.hpp"

class Book {
private:
    std::vector<Page> pages;
    std::string title;
    std::string author;
    uint16_t id;
    bool sellable = false;
public:
    Book();
    Book(std::string in_title, uint16_t in_id);
    void setAuthor(std::string in_author);
    std::string getAuthor();
    void setTitle(std::string in_title);
    std::string getTitle();
    void setid(uint16_t in_id);
    uint16_t getid();
    void addPage(Page in_page);
};

extern void baseBooks();

class Scene : public JSON_IO {
private:
    uint16_t id;
    std::vector<Dialog_line> lines;
    std::vector<std::vector<Dialog_line>> raw_lines;
    Narrative * narrative = NULL;
    std::vector<uint16_t> participants;
    std::vector<uint16_t> all_lines_id;
    int16_t current_line = -1;
public:
    Scene();
    Dialog_line chooseLine(std::vector<Dialog_line> raw_line);
    void makeLines();
    void makeParticipants(); // AFTER lines is made.
    void setParticipants(const std::vector<uint16_t> in_participants);
    std::vector<uint16_t> getParticipants();

    void setNarrative(Narrative * in_narrative);
    Narrative * getNarrative();

    std::vector<std::vector<Dialog_line>> getRawLines();
    std::vector<Dialog_line> getLines();
    Dialog_line nextLine();
    void restartScene();

    uint16_t getID();
    void setID(const uint16_t in_id);


    using JSON_IO::writeJSON;
    using JSON_IO::readJSON;
    void readJSON(cJSON * in_jscene);
    void writeJSON(cJSON * in_jscene);
};

// extern std::vector<Script> all_scripts;
//For narrative purposes.
extern std::vector<bool> died;
extern std::vector<bool> promoted;
extern std::vector<bool> happened;
// extern Script(*scriptChapter[15])();
// void baseScript();
void baseNarrativeState();

extern std::string stats2str(Weapon_stats in_stats);
extern struct Page unit2page(Unit in_unit);

// Scenes should contain a narrative state builder.
//Function takes scene as input and check which lines.
// Each line also contains a narrative object?
// 2D vector of possible lines. Choose which ones by comparing with narrative state.
// Default line at 0. if game_state == line_state, scene takes the line.
// Or maybe line should also have narrative comparer function, that checks a specific variable? or something?

#endif /* SCRIPT_HPP */