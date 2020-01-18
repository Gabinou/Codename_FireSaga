#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "shared.hpp"

//Script structure :
//Dialog line: one person says one line.
//Scene: contains all dialog lines, in order. Should also contain the animations? or at least point to them or something. Contains conditionals for dialog changes.
//1 Chapter 1 script?
//Scripts: contains in what chapter the scene occurs, or which event triggers the scene. Contains conditionals for triggering scenes.
// the game should call unto the script to execute a scene, when it goes into. scene mode or something. The game should give some basic state information. According to the state, triggers a scene.
//How to have dynamic dialogue ie changing dialog with gameplay? Dialog_lines could be dynamic and change according to input: char name, who died, who is alive, etc. Should scenes handle dialog linw modifications? Should I just go at it raw and write all dialog myself? Simpler, but conditions are hard to understand. I think having dynamic dialog actually makes the script more understandable in code.

// I think Game should contain a struct containing its state from the point of view of the script.
// What states of the game are important to the script?
//      ->Characters alive or dead.
//      ->Characters promoted or not?
//      ->Events happened?

struct Script_state {
    std::unordered_map<std::string, bool> isAlive;
    std::unordered_map<std::string, bool> happened;
};

// Fist implementation: only two line switching, depending if someone is present or not.
// NO FORGET THESE COMPLICATED THINGS. the objects are there, and other functions will manipulate them.


struct Dialog_line {
    // How to make the line dynamic?
    std::string id;
    std::string speaker;
    std::string line;
};

class Scene {
        // Should list of participants and deaths?
        // How about having a list of conditionals?
    private:
        std::string id;
        std::unordered_map<std::string, Dialog_line> lines;
        std::vector<std::string> participants;
        std::vector<std::string> all_lines_id;
        char current_line = -1;
    public:
        Scene();
        Scene(std::unordered_map<std::string, Dialog_line> in_lines, std::vector<std::string> in_lines_id);
        void addLine(const Dialog_line in_line);
        Dialog_line getLine(const std::string in_id);
        Dialog_line nextLine();
        std::string getID();
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

#endif /* SCRIPT_HPP */