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
    page.paragraphs.push_back("A novice enemy may very well carry the Goddess's favor and evade all attacks. While you, reader no doubt skilled in the arts of war, might only earn her scorn and receive a swift death at the point of a spear.");
    page.paragraphs.push_back("Such is the tapestry She weaves... May She have mercy on all. Blasphemers say She is a capricious Goddess, that her judgements are cruel, unfair and arbitrary. Those of weak faith may yet be convinced already. But the tapestry of the world is never woven carelessly. Rules guide randomness.");
    page.paragraphs.push_back("Mathemagics is the study these rules. We Mathemagicians believe that to fully understand the rules of randomness brings us closer to the Goddess. It has taken much work to test these theories and compile all these equations. We write them all here so that the curious might benefit from a ascending into the realm of arcane Mathemagics.");
    std::vector<std::string> paragraphs;
    book.addPage(page);
    page.paragraphs.clear();

    page.title = "In our world, Random Events are all Independent";
    page.paragraphs.push_back("Random events have a certain chance of occuring, generally noted in percents (%). For example, a simple coin has a 50\% chance of landing on its face. Flipping the coin once has no influence on the future coin flips. As such, we call these independent random events.");
    page.paragraphs.push_back("These constrast directly with dependent events. For example, drawing cards. Imagine a standard 52 card deck, from which I draw a Jack of Diamond. The chance of me drawing next a Jack of Diamond is zero, while the chance of me drawing any other card from the deck is increased slightly (unless of course, I put the Jack of Diamond back in the deck.)");
    page.paragraphs.push_back("All of the chance events related to war in this world are independent. Having dodged an attack does not increase (nor decrease!) the likelihood of dodging the next attack.");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "The Cosmic Background Random Number Generator (RNG)";
    page.paragraphs.push_back("All random events in this world are compared to the Cosmic Background Random Number Generator (RNG). All soldiers in this world have combat statistics, such as a hit rate (%). If this rate is greater than the next Random Number (RN) generated by the Cosmic RNG, the hit will land. The same applies to all other random events in this world.");
    page.paragraphs.push_back("The interpreters of the Goddess's divine will always recite the following explanation when asked about the Cosmic Background Random Number Generator:");
    page.paragraphs.push_back("Random numbers are created using a pseudo-random number generator called the Mersenne Twister. Pseudo-random numbers are generated via an algorithm that makes use of Mersenne Primes. The generator is always set to the same state upon creation (first startup). Its state is updated everytime a RN is generated. Knowing the state of the Mersenne Twister, one can perfectly predict the will of the Goddess.");
    page.paragraphs.push_back("Interpretation of the words is beyond this humble mathemagician. What even are pseudo-random numbers? How do they compare to 'truly' random numbers? Those are questions better left for the Metamagicians. Note that all Oracles strongly discourage finding the internal state of the Cosmic RNG, so as not, as they say 'diminish Creation'. We Mathemagicians agree.");
    book.addPage(page);
    
    page.paragraphs.clear();
    page.title = "The Cosmic RNG is discrete and uniform";
    page.paragraphs.push_back("Let us now take a moment to define important properties of the Cosmic RNG. We specifically wish to highlight two:");
    page.paragraphs.push_back("1-The random numbers are discrete. This means that the numbers the Goddess gives are whole integers: 0, 1, 2... up to 99. This is in opposition to continuous random numbers, for example human height is a continuous random number expressed as a length in meters. Continuous random numvers can take any decimal value, for example 1.45.");
    page.paragraphs.push_back("2-The RNs are uniform, between 0 and 99. This means thay all numbers between these bounds have equal likelihood to be ordained. In the Cosmic RNG's case, it means that all numbers have an equal 1/100, 1% chance of being anointed. For a discrete RNG, these values are known as the probability mass function (PMF) of the distribution. An uniform distributions PMF, like that of the Cosmic RNG, is very simple, since all possibilities are equality likely.") 
    page.paragraphs.push_back("We write the PMF of a uniform discrete distribution so: P(X = x_i) = 1/n. Or read aloud, the probability P that the next RN noted X is equal to a certain RN noted x_i, is equal to one over the total number of possible values, noted n. In our case, n is 100.") 
    page.paragraphs.push_back("These are all properties are all that is required to mathematically derive useful values later on.");
    book.addPage(page);
 
    page.paragraphs.clear();
    page.title = "Single and Double Roll";
    page.paragraphs.push_back("Two pages ago, we mentionned that 'if the hit rate is greater than the next Random Number (RN), the hit will land'. That is only true if we live in a Single Roll world. Unfortunately, we live in a Double Roll world. So was ordained Above.");
    page.paragraphs.push_back("In a Double Roll universe, two RNs are averaged together, then compared to the Hit rate. If the hit rate is higher than the average of two RNs, the hit lands. The Goddess chose the double Roll for its simplicity, and to reward the skillful.");
page.paragraphs.push_back("An unfortunate side-effect of this universal constant is that the less skilled are punished. Above a hit rate of 50%, Double Roll increase the likelihood of a hit, but under 50\% decreases it instead. 'To everyone who has, more shall be given; but from those who do not have, everything shall be taken away.'");
page.paragraphs.push_back("The actual True hit probabilities will be derived later, using the properties of the Cosmic RNG defined in the previous page.");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Randomness is hard for humans";
    page.paragraphs.push_back("As mentionned in the previous page, the goddess has woven for us a Double Roll world. We wish to expand on the effects and probable cause of this divine decision.");
    page.paragraphs.push_back("In truth, humans are rather mediocre at judging chance. We all tend to overestimate high chance events and underestimate low chance events.");
    page.paragraphs.push_back("Gambling houses routinely swindle the public by exploiting this. The odds of winning are so astronomically low as to be impossible to grasped by the peasantry or aristocracy alike.");
    page.paragraphs.push_back("Laymen may also mistakenly believe that regular patterns are random, while random patterns are orderly! Those of little faith regularly cry out 'The RNG is busted!' when witnessing events ordained by the Goddess. No, my dear unbeliever, the Cosmic RNG is working just as her divine will commands! It is your lack of faith that makes you see order in the pseudo-chaos created by the Background RNG.");
    takeItem 
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Double Roll: PMF and CDF.";
    page.paragraphs.push_back("To derive the true Hit rates in a Double roll World, what needs to be computed is the chance that the average of two numbers given by the Cosmic RNG is lower than the Hit rate. In mathemagical terms, we can write it so: P(X<x_i), the probability that the random variable X, which is an average of two RNs produced by the Cosmic RNG, is lower than a certain x_i, being the hit rate in the present case. In Mathemagics, we call the P(X<x_i) the Cumulative Distribution Function (CDF). Then, the True rates are simply the values obtained by the CDF. This function is closely linked to the previously mentionned Probability Mass Function (PMF).");
    page.paragraphs.push_back("To obtain the CDF from the PMF, you just need the cumulative sum of the PMF. Mathemagically, to find P(X < 3), you need to add P(X = 2), P(X = 1), P(X = 0). For example, in a Single Roll universe the Hit rate is equal to the True hit. This is because we find P(X < 3) = 3% because P(X = 2) = P(X = 1) = P(X = 0) = 1/100. In a Double Roll world, it is less easy to derive the CDF from the PMF. We show the first step in this derivation: determining the probability mass function of the average of two RN.");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Double Roll: Deriving the PMF";
    page.paragraphs.push_back("As was mentionned in the previous page, the True Hit rate in a Double roll universe is simply the CDF, which can be computed using the PMF, the chances of individual event occurance. Using the previously mentionned properties of the Cosmic RNG, we can derive the probabilities that a value is obtained by averaging two RNs. In total, there is 100x100 = 10000 possible combinations of two RNs ordained by the RNG. The derivation can be understood clearly with few examples:");
    page.paragraphs.push_back("If the hit rate is 1, there is only a single combination of two RNs that can be averaged to 0: 0-0. This means that the probability P(X=0), for X the average of two RNs, is equal to 1/10000. The next possible average value is 1/2, with two possibilities, 0-1 and 1-0. This means that 1/2 is twice as likely to be observed as 0 when two uniform RNs are averaged! Note that averaging two RNGs increase the resolution so to speak. Instead of the possible values being 0,1,2.. . they are now 0, 0.5, 1, 1.5... ");
    page.paragraphs.push_back("Anyhow, the next possible hit rate is 2, there are more average values that need to be taken into account: the number of possibilities that average to 1 and 1.5. 1, can be obtained by three combinations of two RNGs: 0-2, 2-0, and 1-1. This makes 1 thrice as likely to be observed compared to 0 in a double roll world! Similarly, 1.5 can be obtained by 4 combinations of two RNGs: 0-3, 2-1, 1-2, 3-0. The pattern is clear! Each averaged value has a 1/10000 more chance to be ordained by the Goddess in a Double Roll world!" );
    page.paragraphs.push_back("The same reasoning can be applied until we get to the value with maximal probability to be averaged: 49.5. There are 100 possible ways to average to RNs to be equal to 49.5: 0-99, 1-98, 2-97... 98-1, 99-0. Then there is a 100/10000 or 1% chance of obtaining 49.5 when averaging two RNs. The chances of obtaning the average 50 then starts decreasing, because there are only 99 possible combinations: 1-99, 2-98...1-99. The probability of two RNs being averaged then decreases until we reach 99, which can only be ordained by the combination 99-99, with a probability 1/10000." );
    page.paragraphs.push_back("Then, the PMF of the average of two RNs outputted by the uniform RNG has this structure: Average values can take values 0, 0.5, 1, 1.5... 99. The increment is the 0.5, or the base step of the original RNGs divided by the number of RN being averaged. The change of obtaining 0 is 0.01%, 0.5 is 0.02%, increasing linearly until 49.5 at 1%, decreasing similarly at 50 to 0.99%, down to 0.01% at 99. ");
    book.addPage(page);


    page.paragraphs.clear();
    page.title = "Double Roll: Computing the True hits";
    page.paragraphs.clear();
    page.paragraphs.push_back("So, if the hit rate is 1, there are only two averages that are below it: 0 and 1/2. The total probability of these two averages occuring is the sum of 1/10000 and 2/10000. Which gives the true chance of hitting in a Double Roll world when the Hit Rate is 1 to be 3/10000 or 0.03%." );
    page.paragraphs.push_back("So this means that the probability that the average of two RNs is lower than 2 to be: 1/10000+2/10000+3/10000+4/10000 which is 10/10000 or 0.10%." );
    page.title = "Gaussian Roll";
    page.paragraphs.push_back("");
    book.addPage(page);    

    page.paragraphs.clear();
    page.title = "True hits Table.";
    page.paragraphs.push_back("");
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
    page.paragraphs.push_back("I dedicate this book all magicians who, like me, has been lied to by the School of Magic. The fathers of Magic thought that the only magic that existed is the one that they wielded. We know today that their Magic is Elemental: a single branch of magic. I discovered there exists Angelic and Demonic magic. If more magic types exist, I will find them.");
    page.paragraphs.push_back("I was the first to use the chaotic and harmonious energies around us. Later, the possessed thought it was impossible to use more than one branch of magic, or to reverse possession. Some still doubt today that I even exist. If you doubt, come find me. I can dive deep into what idiots call possession and come back. They lose their humanities and become lower, almost like beasts.");
    page.paragraphs.push_back("Listen to me, if you wish to learn to use any kind of magic. I can use them all. I will show you how.");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Harmony and disorder";
    page.paragraphs.push_back("We swim in a sea of mixed chaos and order. Some days I feel like walking is like swimming through molasses, other times I feel like burning my house. Sometimes, I feel inspired enough to write a book... You can feel this as the winds of magic blow through you. You don't know it, but your body does. It changes your mood, lifts your spirit or crushes your soul. If you listen to it, you can learn to feel the thickness and flow of the magic that surrounds you.");
    page.paragraphs.push_back("Everything we can see, we can also touch, move it around, or even destroy it. We can touch the things we see. Magic is the same. Before learning to touch it, you must see it. It is hard to find something in the dark, even in your own room. I doubt you could find anything in true darkness. Magic is the same. You won't find it if you can't feel it. This is the first step.");
    page.paragraphs.push_back("When you can feel it, you will be able to see how it changes when you touch it, if it gets stronger or weaker. And after that maybe you can use it to kill someone... Or write spells that can be used by idiots.");
    page.paragraphs.push_back("This is why people think using angelic or demonic leads to possession. Spells make it too easy on stupid people. Reading a spell is much easier than forging it. No need to feel the flow of magic in you or around you. You can't know what demonic does to you if you can't feel demonic at all. And most 'possessed' can't. I have seen many people become possessed. You can feel them sliding in, way before you can see them falling. Everything about them changes! Their angelic thoughts change their inner balance completely, dropping into a vicious circle. A large amount of inner Angelic energies encourage angelic type thoughts. Angelic thoughts attract more angelic energy to yourself. Down, down, down it goes, until the person disappears inside the Angel.");
    page.paragraphs.push_back("Keep reading if you don't want this to happen to you.");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Where does Magic live?";
    page.paragraphs.push_back("It's not hard to know where angelic or demonic wind accumulates. A cemetary. A castle, a church, a brothel. A fresh dug mass grave, a library, a drunkard's house... In all these places do angelic and demonic energies live. Maybe even in a person! You should make use of someone like that as much as possible.  If you want to feel Elemental magic, go outside.");
    page.paragraphs.push_back("In a place where you know a lot of magic lives... Close to an object or person in which magic resides... Listen to your body, feel it in your guts, in your heart. Feel the winds change direction, the magic getting more or less thick. Your body can feel it, you just need to listen. It always flow through you, and around you.");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Feeling the magic";
    page.paragraphs.push_back("At first, you will need to focus to just be able to feel the magic. Don't worry, it's like a muscle. The more you use it, the stronger it gets. It helps to clear your thoughts, and to focus on your senses. Your smell, your taste, your hearing, the surrounding heat and cold, and especially your sense of balance. The feeling of magic is closely tied to your own sense of balance. Once you feel where down and up are, you will start feeling the magic energies around you.");
    page.paragraphs.push_back("Practice. It will become easy, almost natural. You'll be able to feel magic all the time, even while working or being focused on something else. Once you can feel the magic surrounding you, you can start taking notes on the general patterns. In this city, chaos reigns. You can even use this sense to feel major changes happening, before you can hear about them. This is also why mages flee castle towns days weeks or even months before a siege. Have you ever heard of many mages being stuck in a siege? No. You hear of how there are never enough mages in armies during a siege. This is the reason why.");
    page.paragraphs.push_back("This is also why even if you have no talent in magic, you should still learn to feel the magic. This is a very useful ability that everyone can learn. You can feel magic before you can hear, touch, or even see. Everyone benefits. It can be used in many ways.");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Touching the magic";
    page.paragraphs.push_back("If you get bored of only feeling the magic, reach out and touch the magic. You can flap your arms like a chicken and make wind. Magic can be influenced by you exactly like that. Contrarily to wind though, magic is also influenced by your thoughts. The harder you focus, the more powerful your influence becomes.");
    page.paragraphs.push_back("Once you can feel and touch magic, you can force it to take shape in the physical world in ways that are useful to you. The classic example is the Angelic Light spear, which you can throw at demons to pierce their heart. Very useful! Very simple. Accessible to all.");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Casting spells.";
    page.paragraphs.push_back("Do not cast spells written by others. You can perfectly cast spells without them, of even create your own. That requires a deep understand of magic, which is difficult to get: hard work over many years of training. It will make you a better Mage. Spells can be recited by newborn idiots.");
    page.paragraphs.push_back("Written spells make it easy to blast enemies without understanding magic. Spells are weapons of war, not tools of the student of Magic.Though I think even war mages should learn to create their own spells. Military mages disagree with me, sometimes very violently. They have their own very convincing stupid reasons.");
    book.addPage(page);    

    page.paragraphs.clear();
    page.title = "Writing magic spells.";
    page.paragraphs.push_back("Do not write magic spells.");
    page.paragraphs.push_back("Angelic, demonic or elemental. Do not write any magic spells!");
    page.paragraphs.push_back("Written spells can get in the hands of stupid people. Spells in your mind can't be stolen or used by anyone else.");
    book.addPage(page);
        
    page.paragraphs.clear();
    page.title = "What is possession?";
    page.paragraphs.push_back("You can hear many magic users talk about possession. 'He became possessed by the demonic energies'. 'He turned into a demon and devoured her!' That is a horrible way to say it. Demonic energies do not randomly concentrate themselves in a body. A human does not naturally get rid of almost all of it's harmonious and elemental energies. The people do it, and keep it that way. Whether they realize it or not, this is what they are doing.");
    page.paragraphs.push_back("The bodies of the 'possessed' are simply adjusting themselves to an incredible amount of demonic energies. A frail human body cannot contain so much ill will. Only the body of a Demon can. The 'possessed' are just transforming into Demons. Some people think it is slow. But it can take only only a few moments to become a Demon, if you are used to it. For novices, it might take weeks. Many poisons take much more time to kill you. Ills can take root for many years before rising to the surface. But demonic energies act on the heart and body almost instantly.");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Reverting possession";
    page.paragraphs.push_back("Stopping the transformation is rather simple. Magic energies normally fill up our bodies in balance. To stop the transformation, you must go back to your original point of balance. Finding places that are full of your missing energies and have a lack of your excesses can help with that.");
    // Implicit-> need to feel all three types of magic + how to manipulate them willingly 3 types of magic! No wonder people fall into the trap of so-called possession. They can almost always willingly manipulate only one type of magic, at best.

    book.addPage(page);    

    page.paragraphs.clear();
    page.title = "Transforming at will";
    page.paragraphs.push_back("It is easy to feel the balance in normal humans. It is also easy to notice Angels barely contain any demonic or elemental energy. To transform into a Demon, an Angel or an Elemental, you must destroy your internal magic balance. Or at least, accumulate enough magic for you to lean very hard in only one direction.");
    page.paragraphs.push_back("Your body will react quite quickly to dramatic changes. For your first time, you should unbalance yourself very slowly, in a couple hours. The more you practice, the shorter time it will take you to completely change from a 'possessed' to a full Demon, all the way back to human all over again.");
    page.paragraphs.push_back("Before diving, you should fully understand the original balance in you. And when you dive, make sure to Remember your balance. Remember your balance! The feel of it, the shape of it, it's flow and direction. That way, you will never be lost. Remember your balance! Even in the most demonic of places, incarnating the most evil Demon King himself, remember your balance! Remember who you are! Only then will you be able to make your way back.");
    book.addPage(page);

    page.paragraphs.clear();
    page.title = "Creating new spells from nothing";
    page.paragraphs.push_back("I have been asked many times: 'How is there so many spells? Just use magic to kill people!'. This is wrong.");
    page.paragraphs.push_back("There are many weapons. All with the objective of hurting or killing people. Some cut, some crush, some stab, some crit... Some are better at cutting then others. Some weapons are made to be thrown, some can be thrown but will not fly true. The same metal can be forged into completely different weapons by blacksmiths of different skill.");
    page.paragraphs.push_back("The same is true of magic. The strength of the spells depends not only on the skill of the caster, but on the spellforger. Nothing is easy. Spellcasting isn't easy and neither is spellforging. But both skills are required for a pupil to truly become a Mage.");
    page.paragraphs.push_back("I will not tell you how to spellforge. There is a way, but you should embark on it yourself. Maybe I will write a book on spellforging, for Mages that already started forging. For later.");
    book.addPage(page);


    page.paragraphs.clear();
    page.title = "From mage to sage.";
    page.paragraphs.push_back("Who knows how to where wisdom lives and how to acquire it? I did not find it at the School of Magic, even though they can teach you some very useful tricks. If any Mage finds a reliable source of Wisdom, tell me.");
    book.addPage(page);    


    page.paragraphs.clear();
    page.title = "The Purpose of Magic";
    page.paragraphs.push_back("This book is a results of years of experimentation, of hard study and of asking questions to myself, people around me, even the trees and rivers. I have not yet found all my answers, but the Angelic and Demonic energies set my life on a new course of discovery. Now, I want to share my knowledge with as many people as possible.");
    page.paragraphs.push_back("Magic has a purpose, in this world. I do not understand it yet. And I wish to recruit you in my quest to discover the Purpose of Magic. Come find me, if you wish to help me on this quest. Everyone knows my name, now...");
    book.addPage(page);


    page.paragraphs.clear();
    page.title = "Gaussian Roll";
    page.paragraphs.push_back("");
    book.addPage(page);

}
