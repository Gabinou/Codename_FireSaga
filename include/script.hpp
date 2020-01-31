#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "shared.hpp"

//For narrative purposes.
std::unordered_map<std::string, bool> died;
std::unordered_map<std::string, bool> promoted;
std::unordered_map<std::string, bool> happened;

struct Dialog_line {
    // How to make the line dynamic?
    std::string id;
    std::string speaker;
    std::string line;
};

class Scene {
    private:
        std::string id;
        std::unordered_map<std::string, Dialog_line> lines;
        std::vector<std::string> participants;
        std::vector<std::string> all_lines_id;
        char current_line = -1;
    public:
        Scene();
        Scene(std::string in_id);
        Scene(std::unordered_map<std::string, Dialog_line> in_lines, std::vector<std::string> in_lines_id);
        void addLine(const Dialog_line in_line);
        void addLines(const  std::unordered_map<std::string, Dialog_line> in_lines);
        Dialog_line getLine(const std::string in_id);
        Dialog_line nextLine();
        std::string getID();
        void setID(std::string in_id);
        void setParticipants(const std::vector<std::string> in_participants);
        std::vector<std::string> getParticipants();
};

class Script {
    private:
        std::string id;
        std::vector<std::string> scenes_id;
        std::unordered_map<std::string, Scene> scenes;
    public:
        Script();
        std::string getID();
        Scene getScene(const std::string scene_id);
        void addScene(Scene in_scene);
};

extern std::unordered_map<std::string, Script> all_scripts;

void baseScript();

#endif /* SCRIPT_HPP */