#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "shared.hpp"

struct Dialog_line {
    // How to make the line dynamic?
    unsigned short int id;
    unsigned char speaker;
    std::string line;
};

class Scene {
    private:
        short unsigned int id;
        std::vector<Dialog_line> lines;
        std::vector<short unsigned int> participants;
        std::vector<short unsigned int> all_lines_id;
        char current_line = -1;
    public:
        Scene();
        Scene(const short unsigned int in_id);
        Scene(const std::vector<Dialog_line> in_lines, const std::vector<short unsigned int> in_lines_id);
        void addLine(const Dialog_line in_line);
        void addLines(const std::vector<Dialog_line> in_lines);
        Dialog_line getLine(const short unsigned int in_id);
        Dialog_line nextLine();
        short unsigned int  getID();
        void setID(const short unsigned int in_id);
        void setParticipants(const std::vector<short unsigned int> in_participants);
        std::vector<short unsigned int> getParticipants();
};

class Script {
    private:
        short unsigned int  id;
        std::vector<short unsigned int> scenes_id;
        std::vector<Scene> scenes;
    public:
        Script();
        short unsigned int getID();
        Scene getScene(const short unsigned int scene_id);
        void addScene(Scene in_scene);
};

extern std::vector<Script> all_scripts;
//For narrative purposes.
extern std::vector<bool> died;
extern std::vector<bool> promoted;
extern std::vector<bool> happened;
extern Script(*scriptChapter[15])();
void baseScript();
void baseNarrativeState();

namespace SCENE {
enum SCENES {

};
}
namespace LINE {
enum LINES {

};
}

#endif /* SCRIPT_HPP */