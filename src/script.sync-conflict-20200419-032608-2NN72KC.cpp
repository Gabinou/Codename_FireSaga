#include "script.hpp"
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif // STB_SPRINTF_IMPLEMENTATION
Scene::Scene() {

}

Scene::Scene(const short unsigned int in_id) {
    id = in_id;
}

Scene::Scene(const std::vector<Dialog_line> in_lines, const std::vector<short unsigned int> in_lines_id) {
    lines = in_lines;
    all_lines_id = in_lines_id;
}

Dialog_line Scene::getLine(const short unsigned int in_id) {
    return (lines[in_id]);
}

Dialog_line Scene::nextLine() {
    current_line++;
    return (lines[all_lines_id[current_line]]);
}

short unsigned int Scene::getID() {
    return (id);
}

void Scene::setID(short unsigned int in_id) {
    id = in_id;
}

void Scene::addLine(const Dialog_line in_line) {
    lines[in_line.id] = in_line;
}
void Scene::addLines(const std::vector<Dialog_line> in_lines) {
    lines = in_lines;
}

void Scene::setParticipants(const std::vector<short unsigned int> in_participants) {
    participants = in_participants;
}

std::vector<short unsigned int> Scene::getParticipants() {
    return (participants);
}

Script::Script() {

}

Scene Script::getScene(const short unsigned int scene_id) {
    return (scenes[scene_id]);
}

void Script::addScene(Scene in_scene) {
    scenes[in_scene.getID()] = in_scene;
}

short unsigned int Script::getID() {
    return (id);
}

Book::Book() {

}

Book::Book(std::string in_title, unsigned short int in_id) {
    title = in_title;
    id = in_id;
}

void Book::setid(unsigned short int in_id) {
    id = in_id;
}

void Book::setTitle(std::string in_title) {
    title = in_title;
}
void Book::setAuthor(std::string in_author) {
    author = in_author;
}

void Book::addPage(Page in_page) {
    pages.push_back(in_page);
}

std::vector<Script> all_scripts;
std::vector<bool> died;
std::vector<bool> promoted;
std::vector<bool> happened;
Script(*scriptChapter[15])();

void baseNarrativeState() {
    // SDL_Log("Establishing base narrative state.\n");
    // died[UNIT::ERWIN] =  false;
    // died[UNIT::KIARA] =  false;
    // died["Reliable"] =  false;
    // died["Coward"] =  false;
    // died["Silou"] =  false;
    // died["Perignon"] =  false;
    // died["Poet"] =  false;
    // promoted[UNIT::ERWIN] =  false;
    // promoted["Reliable"] =  false;
    // promoted["Coward"] =  false;
    // promoted["Silou"] =  false;
    // promoted["Perignon"] =  false;
    // promoted["Poet"] =  false;
    // happened["ArmCut"] =  false;
}

void baseScript() {
    SDL_Log("Making base script\n");
    Dialog_line temp_line;
    std::vector<Dialog_line> temp_lines;
    Scene temp_scene;
    Script temp_script;

    // SDL_Log("done");

    // temp_script = Script();
    // temp_scene.setID(1);
    // temp_line = {1, UNIT::SILOU, "the line"};
    // temp_lines.push_back(temp_line);
    // temp_scene.addLines(temp_lines);
    // temp_script.addScene(temp_scene);
    // // all_scripts.push_back(temp_script);
    // // temp_lines.clear();

    // SDL_Log("REALLY done");
    // Script Supports;
    // temp_scene.setID("Erwin_Kiara_C");
    // temp_lines.push_back({1, UNIT::ERWIN, "the line2"});
    // temp_lines.push_back({2, UNIT::KIARA, "the line"});
    // temp_scene.addLines(temp_lines);
    // temp_lines.clear();
    // Supports.addScene(temp_scene);
    // temp_scene.setID("Erwin_Kiara_B");
    // temp_lines["1"] = {1, UNIT::ERWIN, "the line"};
    // temp_lines["2"] = {2, UNIT::KIARA, "the line"};
    // temp_scene.addLines(temp_lines);
    // temp_lines.clear();
    // Supports.addScene(temp_scene);
    // temp_scene.setID("Erwin_Kiara_A");
    // temp_lines["1"] = {"1", UNIT::ERWIN, "\"...\""};
    // temp_lines["2"] = {"2", UNIT::KIARA, "Uuuu... Sniff... Ahhh... Ugh... Oh!"};
    // temp_lines["3"] = {"3", UNIT::KIARA, "\"... ... ... ... sniff... \""};
    // temp_lines["4"] = {"4", UNIT::ERWIN, "Kiara, you\'re crying. Why are you crying? Come out. Talk to me."};
    // temp_lines["5"] = {"5", UNIT::KIARA, "\"...no... sniff...\""};
    // temp_lines["6"] = {"6", UNIT::ERWIN, "It was raining this morning Kiara, your skirt must be wet and dirty. Get up, I'll walk you back to camp."};
    // temp_lines["7"] = {"7", UNIT::KIARA, "...sniff... I\'m fine here..."};
    // temp_lines["8"] = {"8", UNIT::ERWIN, "Right, then... Mind if I sit with you?"};
    // temp_lines["9"] = {"9", UNIT::KIARA, "...come."};
    // temp_lines["10"] = {"10", UNIT::ERWIN, "You know, you aren't hiding very well."};
    // temp_lines["11"] = {"11", UNIT::KIARA, "...sniff..."};
    // temp_lines["12"] = {"12", UNIT::ERWIN, "What's happening to you Kiara?"};
    // temp_lines["13"] = {"13", UNIT::KIARA, "I\'ve been thinking too much... Ugh..."};
    // temp_lines["14"] = {"14", UNIT::ERWIN, "About what?"};
    // temp_lines["15"] = {"15", UNIT::KIARA, "What I\'ve been doing until now... I\'ve made vows Erwin."};
    // temp_lines["16"] = {"16", UNIT::ERWIN, "I\'ve heard about those"};
    // temp_lines["17"] = {"17", UNIT::KIARA, "I still remember them.\"Obey your father and mother your Parish father and your elders. Serve humbly and faithfully. Help the needy. Heal the sick. Remain celibate, may you devout your whole being to your calling. Stay true to God and the mission he has chosen for you.\'"};
    // temp_lines["18"] = {"18", UNIT::ERWIN, "We all appreciate your devotion Kiara. Go ask any knight or squire, or even villagers and peasants we passed by. They all love you."};
    // temp_lines["19"] = {"19", UNIT::KIARA, "uuuh... sniff... Thank you... Ugh... Aaaaaah! Uuuuh! "};
    // temp_lines["20"] = {"20", UNIT::ERWIN, "Oh! What is it? Kiara. I\'m sorry."};
    // temp_lines["21"] = {"21", UNIT::KIARA, "Don\'t... Ooooh... Why do you apologize? uuh... You've neither said nor done anything wrong."};
    // temp_lines["22"] = {"22", UNIT::ERWIN, "... Then why are you still crying, harder than before? If is not me, than what is it?"};
    // temp_lines["23"] = {"23", UNIT::KIARA, "Uuuuh. It's me... Aaaagh... I... Sniff... I\'ve been feeling that I... don\'t want the vows anymore!"};
    // temp_lines["24"] = {"24", UNIT::ERWIN, "You don\'t want to be a priestess anymore? To help and heal people? That sounds absolutely not like you."};
    // temp_lines["25"] = {"25", UNIT::KIARA, "No! No... As you say... I want... sniff... I want children, Erwin. Uuuuh..."};
    // temp_lines["26"] = {"26", UNIT::ERWIN, "Oh."};
    // temp_lines["27"] = {"27", UNIT::KIARA, "I can\'t help. I try to ignore it... aah... I try to think about other things, think about the army, the people, to focus on work... The more I suppress it, the more numerous my children are in my dreams... What should I do?"};
    // temp_lines["28"] = {"28", UNIT::ERWIN, "Kiara... I can\'t... You should..."};
    // temp_lines["29"] = {"29", UNIT::KIARA, "... Uhhhh.... Ugh!!! aaah... I remember their faces... Beautiful and bright smiles... They look like their father, Erwin."};
    // temp_lines["30"] = {"30", UNIT::ERWIN, "Kiara! I can\'t let this one go Kiara. Marry me."};
    // temp_lines["31"] = {"31", UNIT::KIARA, "Ah! Erwin! Yes! No! Don't do this to me!"};
    // temp_lines["32"] = {"32", UNIT::ERWIN, "Kiara, I'll get you a ring. It will take some time. You can think about it more. But I want you to be my wife, so that we can have the children in your dreams."};
    // temp_lines["33"] = {"33", UNIT::KIARA, "...Sniff... I would like that."};
    // temp_lines["34"] = {"34", UNIT::ERWIN, "Wanna go back to camp, to sleep?"};
    // temp_lines["35"] = {"35", UNIT::KIARA, "No, not yet. Stay with me a little more..."};
    // temp_scene.addLines(temp_lines);
    // Supports.addScene(temp_scene);
    // temp_lines.clear();
    // all_scripts["Supports"] = Supports;
}

Page unit2page(Unit in_unit) {
    Page out;


    return (out);
}

void baseBooks() {
    Page page;
    Book book;

    book = Book("Basics of Army Management", ITEM::NAME::BOOKBASICS);
    book.setAuthor("Ancestor of Erwin?");

    book = Book("Weapons of the World", ITEM::NAME::BOOKWEAPONS);
    book.setAuthor("Member of the Guild of blacksmiths?.");

    book = Book("Magic: The Definitive Guide", ITEM::NAME::BOOKELEMENTAL);
    book.setAuthor("Ancestor of the gifted brothers.");
    book = Book("Deep and Arcane Mathemagics", ITEM::NAME::BOOKMATHEMAGICS);
    book.setAuthor("Ancestor of the gifted brothers.");
    page.title = "Introduction to Mathemagics";
    page.paragraphs.push_back("This world is governed by randomness. All soldiers realize this fact: nothing is certain. Taking but a single swing of a sword means to take a risk, to bet on your skills against your opponent's capacity to dodge.");
    page.paragraphs.push_back("An enemy may very well carry the Goddess's favor and evade all the attacks of a master. While you, no doubt skilled reader, might only earn her scorn and receive a swift death at the point of a spear wielded by a novice.");
    page.paragraphs.push_back("Such is the tapestry She weaves... May She have mercy on all. Blasphemers say She is a capricious Goddess, that her judgements are cruel, unfair and arbitrary. Those of weak faith may yet be convinced already. But the tapestry of the world is never woven carelessly. Rules guide randomness.");
    page.paragraphs.push_back("Mathemagics is the study these rules. We Mathemagicians believe that to fully understand the rules of randomness brings us closer to the Goddess. It has taken much work to test these theories and compile all these equations. We write them all here so that the curious might benefit from a deep dive into arcane Mathemagics.");
    std::vector<std::string> paragraphs;
    book.addPage(page);
    page.paragraphs.clear();

    page.title = "Random Events";
    page.paragraphs.push_back("Random events have a certain chance of occuring, generally noted in percents (%). For example, a simple coin has a 50\% chance of landing on its face. Flipping the coin once has no influence on the future coin flips. As such, we call these independent random events.");
    page.paragraphs.push_back("These constrast directly with dependent events. For example, drawing cards. Imagine a standard 52 card deck, from which I draw a Jack of Diamond. The chance of me drawing next a Jack of Diamond is zero, while the chance of me drawing any other card from the deck is increased slightly (unless of course, I put the Jack of Diamond back in the deck.)");
    page.paragraphs.push_back("All of the chance events related to war in this world are independent. Having dodged an attack does not increase (nor decrease!) the likelihood of dodging the next attack.");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "The Cosmic Background Random Number Generator (RNG)";
    page.paragraphs.push_back("All random events in this world are compared to the cosmic background RNG. All soldiers in this world have combat statistics, such as a hit rate (%). If this rate is greater than the next Random Number (RN) generated by the cosmic RNG, the hit will land. The same applies to all other random events in this world.");
    page.paragraphs.push_back("The interpreters of the Goddess's divine will always recite the following explanation when asked about the Cosmic Background Random Number Generator.");
    page.paragraphs.push_back("Random numbers are created using the Mersenne twister. Pseudo-random numbers are generated via an algorithm that makes use of Mersenne Primes. The generator is always set to the same state upon creation (first startup). Its state is updated everytime a RN is generated.");
    page.paragraphs.push_back("Interoretation of the words is beyond this humble mathemagician. ");
    book.addPage(page);
    
    page.paragraphs.clear();
    page.title = "The RNG is discrete and uniform";
    page.paragraphs.push_back("Let us now taje a moment to define important properties of the cosmic RNG. We specifically wish to highlight two:.");
    page.paragraphs.push_back("1-The random numbers are discrete. This means that the numbers the Goddess gives are whole integers: 0, 1, 2... up to 99. This is in opposition to continuous random numbers, for example human height is a continuous random number generally between 1 and 3 meters. Continuous random numvers can take any decimal value, for example 1.45.");
    page.paragraphs.push_back("2-The RNs are uniform, between 0 and 99. This means thay all numbers between these bounds have equal likelihood to be ordained. In the cosmic RNGs case, it means that all numbers gave an equal 1/100, 1% chance. For a discrete RNG, these values are known as the probability mass function (PMF) of the distribution. An uniform distributions PMF, like that of the cosmic RNG, is very simple, since all possibilities are equality likely. ") 
    page.paragraphs.push_back("These are all properties are all that is required to mathematically derive useful values later on.");
    book.addPage(page);
 
    page.paragraphs.clear();
    page.title = "Single and Double Roll";
    page.paragraphs.push_back("In the previous page, we mentionned that 'if the hit rate is greater than the next Random Number (RN), the hit will land'. That is only true if we live in a Single Roll world. Unfortunately, we live in a Double Roll universe.");
    page.paragraphs.push_back("In a Double Roll universe, two RNs are averaged together, then compared to the Hit rate. The Goddess chose the double Roll for its simplicity, and to reward the skillful.");
page.paragraphs.push_back("An unfortunate side-effect of this universal constant is that the less skilled are punished. Above a hit rate of 50%, Double Roll increase the likelihood of a hit, but under 50\% decreases it instead. 'To everyone who has, more shall be given; but from those who does not have, everything shall be taken away.'");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Randomness is hard for humans";
    page.paragraphs.push_back("As mentionned in the previous page, the goddess has woven for us a Double Roll world. We wish to expand on the effects and probable cause of this divine decision. ");
    page.paragraphs.push_back("In truth, humans are rather mediocre at judging chance. We all tend to overestimate high chance events and underestimate low chance events.");
    page.paragraphs.push_back("Gambling houses routinely swindle the public vy exploiting this. The odds of winning are so astronomically low as to be impossible to grasped by the peasantry or aristocracy alike." );
    page.paragraphs.push_back("Laymen may also mistakenly believe that regular patterns are random, while random patterns are orderly! Those of little faith regularly cry out 'The RNG is busted!' when witnessing events ordained by the Goddess. No, my dear unbeliever, the cosmic RNG is working just as her divine will commands! It is your lack of faith that makes you see ordee in the pseudo-chaos created by the background RNG.");
    takeItem 
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Double Roll: True rates";
    page.paragraphs.push_back("Using the previously mentionned rules, we can derive the true chance that a hit occurs in a Double Roll world using the Hit Rates.");
    page.paragraphs.push_back("In truth, what needs to be computed is the chance that the average of two numbers given by the cosmic RNG is lower than the Hit rate. In this case the derivation can be understood clearly with some examples:");
    page.paragraphs.push_back("There is only one combination of two RNs tgat can be averafed to 0, 0 and 0. The next possible average value is 1/2,with two possibilities, 0-1 and 1-0. This mrans that 1/2 is twice as likely to be observed as 0 when two uniform RNs are averaged! ");
    page.paragraphs.push_back("Similarly, the next possiblw average value, 1, has three possibilities, 0-2, 2-0, and 1-1. This makes 1 thrice as likely to be observed compared to 0 in a double roll world! " );
    book.addPage(page);



    page.paragraphs.clear();
    page.title = "Gaussian Roll";
    page.paragraphs.push_back("");
    book.addPage(page);

    book = Book("Love blooms on the Battlefield", ITEM::NAME::BOOKLOVE);
    book.setAuthor("A girl PC that likes Shipping. Kiara?.");

    page.paragraphs.clear();
    page.title = "Gaussian Roll";
    page.paragraphs.push_back("");
    book.addPage(page);

    book = Book("Growth & Potential", ITEM::NAME::BOOKGROWTHS);
    book.setAuthor("");

    page.paragraphs.clear();
    page.title = "Gaussian Roll";
    page.paragraphs.push_back("");
    book.addPage(page);

    book = Book("Angelic and Demonic Possession", ITEM::NAME::BOOKPOSSESSION);
    book.setAuthor("Twinborn");

    page.paragraphs.clear();
    page.title = "Foreword";
    page.paragraphs.push_back("I dedicate this book all magicians who, like me, has been lied to by the School of Magic. How presumptuous were the fathers of Magic, to name the discipline so, implicitely declaring their field to be complete and all-emcompassing. We know today that their Magic is in reality Elemental. A single branch among three branches of Magic. There probably exists more, and if they do I will find them.");
    page.paragraphs.push_back("I was the first to use the chaotic and harmonious energies around us. Later, the possessed thought it impossible to use more than one branch of magic, and to reverse possession. They doubted I even existed, and doubted I could even do it. If you doubt, come find me. I can dive deep into what idiots call possession and come back unscathed. They lose their humanities and become lower, almost like beasts.");
    page.paragraphs.push_back("Stop doubting me, if you wish to use any kind of magic. I can use them all. I will show you how.");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Harmony and disorder";
    page.paragraphs.push_back("We swim in a sea of mixed chaos and order. Some days I feel like walking is like swimming thourgh molasses, other times I feel like burning my house. Sometimes, I feel like writing a book... You can feel this as the winds of magic blow through you. You don't know it, but your body does. If you listen to it, you can learn to feel the thickness of the magic that surrounds you.");
    page.paragraphs.push_back("Stuff we can see, we can also touch, move it around, or even destroy it. We can touch the things we see. Magic is the same. Before learning to touch it, you must see it. It is hard to find something in the dark, even in your own room. I doubt you could find it. Magic is the same. You won't find it if you can't feel it. Then, you can see how it changes when you touch it, if it gets stronger or weaker. And after that maybe you can use it to kill someone... Or write spells that can be use by idiots.");
    page.paragraphs.push_back("This is why people think using angelic or demonic leads to possession. Spells make it too easy on stupid people. Reading is much easier than feeling magic. You can't know what demonic does to you if you can't feel demonic at all. And most 'possessed' can't. You can feel them sliding in, see them and even hear them falling. Everything about them changes!");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Where is magic";
    page.paragraphs.push_back("It's not hard to where angelic or demonic wind accumulates. A cemetary. A castle, a church, a brothel. A fresh dug mass grave, a library, a drunkard's house... In all these places do angelic and demonic energies live. If you want to feel Elemental magic, go outside.");
    page.paragraphs.push_back("In a place where you know a lot of energy lives... Maybe even in a person! You should make use of someone like that as much as possible. In their presence, practice feeling it. Listen to your body, feel it in your guts. Feel the winds change direction, the magic getting more or less thick. Your body can feel it, you just need to listen.");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Feeling the magic";
    page.paragraphs.push_back("At first, you will need to focus to just be able to feel the magic. Don't worry, it's like a muscle. The more you use it, the stronger it gets. I will become easy, almost natural. You'll be able to feel magic all the time, even while working or being focused on something else.");
    page.paragraphs.push_back("Once you can feel the magic surrounding you, you can start taking notes on the general patterns. In this city, chaos reigns. You can even use this sense to feel major changes happening, before you can hear about them. This is also why mages flee castle towns days weeks or even months before a siege. Have you ever heard of many mages being stuck in sieges? No. You hear of how armies being sieged are lacking mages. This is why.");
    page.paragraphs.push_back("This is why even if you have no talent in magic, you should still learn to feel the magic. This is a very useful ability that everone can learn. You can feel magic before you can hear, touch, or even see. Everyone benefits.");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Touching the magic";
    page.paragraphs.push_back("If you get bored of only feeling the magic, reach out and touch the magic. You can flap your arms like a chicken and make wind. Magic wind can be influenced by you exactly like that. Contrarily to wind though, magic is influenced by thoughts themselves. The harder you focus, the more powerful your influence becomes.");
    page.paragraphs.push_back("once you can feel and touch magic, you can force it to take shape in the physical world in wasy that are useful to you. The classic example is the Angelic light spear, which you can throw at demons to pierce their heart. Very useful! Very simple. Accessible to all.");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Writing magic spells.";
    page.paragraphs.push_back("Do not write magic spells.");
    page.paragraphs.push_back("Angelic, demonic or elemental. Do not write any magic spells!");
    page.paragraphs.push_back("Written spells can get in the hands of stupid people. Spells in your mind can't.");
    book.addPage(page);
        

    page.paragraphs.clear();
    page.title = "Gaussian Roll";
    page.paragraphs.push_back("");
    book.addPage(page);

}
