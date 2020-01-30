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

void baseScript() {
    printf("Making base script\n");
    Dialog_line temp_line;
    std::unordered_map<std::string, Dialog_line> temp_lines;
    Scene temp_scene;
    Script Chapter_1; 

    Scene intro("Intro"); 
    temp_line = {"1", "Silou", "the line"};
    intro.addLine(temp_line);
    Chapter_1.addScene(intro);
    all_scripts["Chapter 1"] = Chapter_1;

    Script Support_Main_Lovely;
    temp_scene.setID("Support_Main_Lovely_C"); 
    temp_lines["1"] = {"1", "Main", "the line"}; 
    temp_lines["2"] = {"2", "Lovely", "the line"}; 
    // temp_scene.addLine(temp_line);


}