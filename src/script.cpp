#include "script.hpp"

Scene::Scene() {

}

Scene::Scene(std::string in_id) {
    id = in_id;
}

Scene::Scene(std::unordered_map<std::string, Dialog_line> in_lines,  std::vector<std::string> in_lines_id) {
    lines = in_lines;
    all_lines_id = in_lines_id;
}

Dialog_line Scene::getLine(const std::string in_id) {
    return(lines[in_id]);
}

Dialog_line Scene::nextLine() {
    current_line++;
    return(lines[all_lines_id[current_line]]);
}

std::string Scene::getID() {
    return(id);    
}

void Scene::setID(std::string in_id) {
    id = in_id;
}

void Scene::addLine(const Dialog_line in_line) {
    lines[in_line.id] = in_line;
}
void Scene::addLines(const std::unordered_map<std::string, Dialog_line> in_lines) {
    lines = in_lines;
}

void Scene::setParticipants(const std::vector<std::string> in_participants) {
    participants = in_participants;
}

std::vector<std::string> Scene::getParticipants() {
    return(participants);
}

Script::Script() {

}

Scene Script::getScene(const std::string scene_id) {
    return(scenes[scene_id]);
}

void Script::addScene(Scene in_scene) {
    scenes[in_scene.getID()] = in_scene;
}

std::string Script::getID() {
    return(id);    
}

std::unordered_map<std::string, Script> all_scripts;
std::unordered_map<std::string, bool> died;
std::unordered_map<std::string, bool> promoted;
std::unordered_map<std::string, bool> happened;


void baseNarrativeState() {
    printf("Establishing base narrative state.\n");
    died["Erwin"] =  false;
    died["Kiara"] =  false;
    died["Reliable"] =  false;
    died["Coward"] =  false;
    died["Silou"] =  false;
    died["Perignon"] =  false;
    died["Poet"] =  false;
    promoted["Erwin"] =  false;
    promoted["Reliable"] =  false;
    promoted["Coward"] =  false;
    promoted["Silou"] =  false;
    promoted["Perignon"] =  false;
    promoted["Poet"] =  false;
    happened["ArmCut"] =  false;
}

void baseScript() {
    printf("Making base script\n");
    Dialog_line temp_line;
    std::unordered_map<std::string, Dialog_line> temp_lines;
    Scene temp_scene;

    Script Chapter_2; 
    Script Chapter_3; 
    Script Chapter_4; 
    Script Chapter_5; 
    Script Chapter_6; 
    Script Chapter_7; 
    Script Chapter_8; 
    Script Chapter_9; 
    Script Chapter_10; 
    Script Chapter_11; 
    Script Chapter_12; 
    Script Chapter_13; 
    Script Chapter_14; 
    Script Chapter_15; 

    Script Chapter_1; 
    temp_scene.setID("Intro");
    temp_lines["1"] = {"1", "Silou", "the line"};
    temp_scene.addLines(temp_lines);
    Chapter_1.addScene(temp_scene);
    all_scripts["Chapter 1"] = Chapter_1;

    Script Supports;
    temp_scene.setID("Erwin_Kiara_C");
    temp_lines["1"] = {"1", "Erwin", "the line2"};
    temp_lines["2"] = {"2", "Kiara", "the line"}; 
    temp_scene.addLines(temp_lines);
    temp_lines.clear();
    Supports.addScene(temp_scene);
    temp_scene.setID("Erwin_Kiara_B");
    temp_lines["1"] = {"1", "Erwin", "the line"};
    temp_lines["2"] = {"2", "Kiara", "the line"}; 
    temp_scene.addLines(temp_lines);
    temp_lines.clear(); 
    Supports.addScene(temp_scene);
    temp_scene.setID("Erwin_Kiara_A");
    temp_lines["1"] = {"1", "Erwin", "\"...\""};
    temp_lines["2"] = {"2", "Kiara", "Uuuu... Sniff... Ahhh... Ugh... Oh!"}; 
    temp_lines["3"] = {"3", "Kiara", "\"... ... ... ... sniff... \""}; 
    temp_lines["4"] = {"4", "Erwin", "Kiara, you\'re crying. Why are you crying? Come out. Talk to me."};
    temp_lines["5"] = {"5", "Kiara", "\"...no... sniff...\""}; 
    temp_lines["6"] = {"6", "Erwin", "It was raining this morning Kiara, your skirt must be wet and dirty. Get up, I'll walk you back to camp."}; 
    temp_lines["7"] = {"7", "Kiara", "...sniff... I\'m fine here..."}; 
    temp_lines["*"] = {"8", "Erwin", "Right, then... Mind if I sit with you?"}; 
    temp_scene.addLines(temp_lines); 
    Supports.addScene(temp_scene);
    temp_lines.clear(); 
    all_scripts["Supports"] = Supports;
}