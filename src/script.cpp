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
Script (*scriptChapter[15]) ();

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
    temp_lines["8"] = {"8", "Erwin", "Right, then... Mind if I sit with you?"}; 
    temp_lines["9"] = {"9", "Kiara", "...come."}; 
    temp_lines["10"] = {"10", "Erwin", "You know, you aren't hiding very well."}; 
    temp_lines["11"] = {"11", "Kiara", "...sniff..."}; 
    temp_lines["12"] = {"12", "Erwin", "What's happening to you Kiara?"}; 
    temp_lines["13"] = {"13", "Kiara", "I\'ve been thinking too much... Ugh..."}; 
    temp_lines["14"] = {"14", "Erwin", "About what?"}; 
    temp_lines["15"] = {"15", "Kiara", "What I\'ve been doing until now... I\'ve made vows Erwin."}; 
    temp_lines["16"] = {"16", "Erwin", "I\'ve heard about those"}; 
    temp_lines["17"] = {"17", "Kiara", "I still remember them.\"Obey your father and mother your Parish father and your elders. Serve humbly and faithfully. Help the needy. Heal the sick. Remain celibate, may you devout your whole being to your calling. Stay true to God and the mission he has chosen for you.\'"}; 
    temp_lines["18"] = {"18", "Erwin", "We all appreciate your devotion Kiara. Go ask any knight or squire, or even villagers and peasants we passed by. They all love you."}; 
    temp_lines["19"] = {"19", "Kiara", "uuuh... sniff... Thank you... Ugh... Aaaaaah! Uuuuh! "}; 
    temp_lines["20"] = {"20", "Erwin", "Oh! What is it? Kiara. I\'m sorry."}; 
    temp_lines["21"] = {"21", "Kiara", "Don\'t... Ooooh... Why do you apologize? uuh... You've neither said nor done anything wrong."}; 
    temp_lines["22"] = {"22", "Erwin", "... Then why are you still crying, harder than before? If is not me, than what is it?"}; 
    temp_lines["23"] = {"23", "Kiara", "Uuuuh. It's me... Aaaagh... I... Sniff... I\'ve been feeling that I... don\'t want the vows anymore!"}; 
    temp_lines["24"] = {"24", "Erwin", "You don\'t want to be a priestess anymore? To help and heal people? That sounds absolutely not like you."}; 
    temp_lines["25"] = {"25", "Kiara", "No! No... As you say... I want... sniff... I want children, Erwin. Uuuuh..."}; 
    temp_lines["26"] = {"26", "Erwin", "Oh."}; 
    temp_lines["27"] = {"27", "Kiara", "I can\'t help. I try to ignore it... aah... I try to think about other things, think about the army, the people, to focus on work... The more I suppress it, the more numerous my children are in my dreams... What should I do?"}; 
    temp_lines["28"] = {"28", "Erwin", "Kiara... I can\'t... You should..."}; 
    temp_lines["29"] = {"29", "Kiara", "... Uhhhh.... Ugh!!! aaah... I remember their faces... Beautiful and bright smiles... They look like their father, Erwin."}; 
    temp_lines["30"] = {"30", "Erwin", "Kiara! I can\'t let this one go Kiara. Marry me."};
    temp_lines["31"] = {"31", "Kiara", "Ah! Erwin! Yes! No! Don't do this to me!"}; 
    temp_lines["32"] = {"32", "Erwin", "Kiara, I'll get you a ring. It will take some time. You can think about it more. But I want you to be my wife, so that we can have the children in your dreams."};
    temp_lines["33"] = {"33", "Kiara", "...Sniff... I would like that."}; 
    temp_lines["34"] = {"34", "Erwin", "Wanna go back to camp, to sleep?"};
    temp_lines["35"] = {"35", "Kiara", "No, not yet. Stay with me a little more..."}; 
    temp_scene.addLines(temp_lines); 
    Supports.addScene(temp_scene);
    temp_lines.clear(); 
    all_scripts["Supports"] = Supports;
}