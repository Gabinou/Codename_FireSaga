#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <string>
#include "enums.hpp"
#include "filesystem.hpp"
#include "unit.hpp"
#include "structs.hpp"

class Book {
private:
    std::vector<Page> pages;
    std::string title;
    std::string author;

    unsigned short int id;

public:
    Book();
    Book(std::string in_title, unsigned short int in_id);
    void setAuthor(std::string in_author);
    std::string getAuthor();
    void setTitle(std::string in_title);
    std::string getTitle();
    void setid(unsigned short int in_id);
    unsigned short int getid();
    void addPage(Page in_page);
};

extern void baseBooks();
extern void testXML_books();

class Scene : public JSON_IO {
private:
    short unsigned int id;
    std::vector<Dialog_line> lines;
    Narrative narrative;
    std::vector<<Dialog_line>> raw_lines;
    std::vector<short unsigned int> participants;
    std::vector<short unsigned int> all_lines_id;
    char current_line = -1;
public:
    Scene();
    Scene(const short unsigned int in_id);
    Scene(const std::vector<Dialog_line> in_lines, const std::vector<short unsigned int> in_lines_id);
    void addLine(const Dialog_line in_line);
    void addLines(const std::vector<Dialog_line> in_lines);
    
    void setNarrative(Narrative in_narrative);
    Narrative getNarrative();

    Dialog_line getLine(const short unsigned int in_id);
    Dialog_line nextLine();

    short unsigned int getID();
    void setID(const short unsigned int in_id);
    void setParticipants(const std::vector<short unsigned int> in_participants);
    void addParticipant(const short unsigned int in_participant);
    std::vector<short unsigned int> getParticipants();

    using JSON_IO::writeJSON;
    using JSON_IO::readJSON;
    void readJSON(cJSON * in_jscene);
    void writeJSON(cJSON * in_jscene);
};

// class Script {
// private:
//     short unsigned int  id;
//     std::vector<short unsigned int> scenes_id;
//     std::vector<Scene> scenes;
// public:
//     Script();
//     short unsigned int getID();
//     Scene getScene(const short unsigned int scene_id);
//     void addScene(Scene in_scene);
// };

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