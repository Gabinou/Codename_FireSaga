#include "script.hpp"

Scene::Scene() {

}

Scene::Scene(const std::string in_id) {
    id = in_id;
}

Scene::Scene(const std::unordered_map<std::string, Dialog_line> in_lines, const std::vector<std::string> in_lines_id) {
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
    died[UNIT::ERWIN] =  false;
    died[UNIT::KIARA] =  false;
    died["Reliable"] =  false;
    died["Coward"] =  false;
    died["Silou"] =  false;
    died["Perignon"] =  false;
    died["Poet"] =  false;
    promoted[UNIT::ERWIN] =  false;
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
    temp_lines["1"] = {"1", UNIT::SILOU, "the line"};
    temp_scene.addLines(temp_lines);
    Chapter_1.addScene(temp_scene);
    all_scripts["Chapter 1"] = Chapter_1;

    Script Supports;
    temp_scene.setID("Erwin_Kiara_C");
    temp_lines["1"] = {"1", UNIT::ERWIN, "the line2"};
    temp_lines["2"] = {"2", UNIT::KIARA, "the line"}; 
    temp_scene.addLines(temp_lines);
    temp_lines.clear();
    Supports.addScene(temp_scene);
    temp_scene.setID("Erwin_Kiara_B");
    temp_lines["1"] = {"1", UNIT::ERWIN, "the line"};
    temp_lines["2"] = {"2", UNIT::KIARA, "the line"}; 
    temp_scene.addLines(temp_lines);
    temp_lines.clear(); 
    Supports.addScene(temp_scene);
    temp_scene.setID("Erwin_Kiara_A");
    temp_lines["1"] = {"1", UNIT::ERWIN, "\"...\""};
    temp_lines["2"] = {"2", UNIT::KIARA, "Uuuu... Sniff... Ahhh... Ugh... Oh!"}; 
    temp_lines["3"] = {"3", UNIT::KIARA, "\"... ... ... ... sniff... \""}; 
    temp_lines["4"] = {"4", UNIT::ERWIN, "Kiara, you\'re crying. Why are you crying? Come out. Talk to me."};
    temp_lines["5"] = {"5", UNIT::KIARA, "\"...no... sniff...\""}; 
    temp_lines["6"] = {"6", UNIT::ERWIN, "It was raining this morning Kiara, your skirt must be wet and dirty. Get up, I'll walk you back to camp."}; 
    temp_lines["7"] = {"7", UNIT::KIARA, "...sniff... I\'m fine here..."}; 
    temp_lines["8"] = {"8", UNIT::ERWIN, "Right, then... Mind if I sit with you?"}; 
    temp_lines["9"] = {"9", UNIT::KIARA, "...come."}; 
    temp_lines["10"] = {"10", UNIT::ERWIN, "You know, you aren't hiding very well."}; 
    temp_lines["11"] = {"11", UNIT::KIARA, "...sniff..."}; 
    temp_lines["12"] = {"12", UNIT::ERWIN, "What's happening to you Kiara?"}; 
    temp_lines["13"] = {"13", UNIT::KIARA, "I\'ve been thinking too much... Ugh..."}; 
    temp_lines["14"] = {"14", UNIT::ERWIN, "About what?"}; 
    temp_lines["15"] = {"15", UNIT::KIARA, "What I\'ve been doing until now... I\'ve made vows Erwin."}; 
    temp_lines["16"] = {"16", UNIT::ERWIN, "I\'ve heard about those"}; 
    temp_lines["17"] = {"17", UNIT::KIARA, "I still remember them.\"Obey your father and mother your Parish father and your elders. Serve humbly and faithfully. Help the needy. Heal the sick. Remain celibate, may you devout your whole being to your calling. Stay true to God and the mission he has chosen for you.\'"}; 
    temp_lines["18"] = {"18", UNIT::ERWIN, "We all appreciate your devotion Kiara. Go ask any knight or squire, or even villagers and peasants we passed by. They all love you."}; 
    temp_lines["19"] = {"19", UNIT::KIARA, "uuuh... sniff... Thank you... Ugh... Aaaaaah! Uuuuh! "}; 
    temp_lines["20"] = {"20", UNIT::ERWIN, "Oh! What is it? Kiara. I\'m sorry."}; 
    temp_lines["21"] = {"21", UNIT::KIARA, "Don\'t... Ooooh... Why do you apologize? uuh... You've neither said nor done anything wrong."}; 
    temp_lines["22"] = {"22", UNIT::ERWIN, "... Then why are you still crying, harder than before? If is not me, than what is it?"}; 
    temp_lines["23"] = {"23", UNIT::KIARA, "Uuuuh. It's me... Aaaagh... I... Sniff... I\'ve been feeling that I... don\'t want the vows anymore!"}; 
    temp_lines["24"] = {"24", UNIT::ERWIN, "You don\'t want to be a priestess anymore? To help and heal people? That sounds absolutely not like you."}; 
    temp_lines["25"] = {"25", UNIT::KIARA, "No! No... As you say... I want... sniff... I want children, Erwin. Uuuuh..."}; 
    temp_lines["26"] = {"26", UNIT::ERWIN, "Oh."}; 
    temp_lines["27"] = {"27", UNIT::KIARA, "I can\'t help. I try to ignore it... aah... I try to think about other things, think about the army, the people, to focus on work... The more I suppress it, the more numerous my children are in my dreams... What should I do?"}; 
    temp_lines["28"] = {"28", UNIT::ERWIN, "Kiara... I can\'t... You should..."}; 
    temp_lines["29"] = {"29", UNIT::KIARA, "... Uhhhh.... Ugh!!! aaah... I remember their faces... Beautiful and bright smiles... They look like their father, Erwin."}; 
    temp_lines["30"] = {"30", UNIT::ERWIN, "Kiara! I can\'t let this one go Kiara. Marry me."};
    temp_lines["31"] = {"31", UNIT::KIARA, "Ah! Erwin! Yes! No! Don't do this to me!"}; 
    temp_lines["32"] = {"32", UNIT::ERWIN, "Kiara, I'll get you a ring. It will take some time. You can think about it more. But I want you to be my wife, so that we can have the children in your dreams."};
    temp_lines["33"] = {"33", UNIT::KIARA, "...Sniff... I would like that."}; 
    temp_lines["34"] = {"34", UNIT::ERWIN, "Wanna go back to camp, to sleep?"};
    temp_lines["35"] = {"35", UNIT::KIARA, "No, not yet. Stay with me a little more..."}; 
    temp_scene.addLines(temp_lines); 
    Supports.addScene(temp_scene);
    temp_lines.clear(); 
    all_scripts["Supports"] = Supports;
}