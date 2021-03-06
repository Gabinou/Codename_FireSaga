#include "narrative.h"
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
#include "stb_ds.h"
// #endif // STB_SPRINTF_IMPLEMENTATION

struct Line Line_default = {
    .line = NULL,
    .condition = {0, 0, 0, 0},
};

struct Narrative_Conditions Narrative_Conditions_default = {
    .pc_alive = UINT64_MAX,
    .pc_recruited = 0,
    .npc_alive = UINT64_MAX,
    .misc = 0,
};

struct Narrative_Conditions Narrative_Conditions_invalid = {
    .pc_alive = 0,
    .pc_recruited = 0,
    .npc_alive = 0,
    .misc = 0,
};

struct Book Book_default = {
    .pages = NULL,
    .id = 0,
    .sellable = false,
};

struct Page Page_default = {
    .paragraphs = NULL,
};

struct Scene Scene_default =  {
    .json_element = JSON_SCENE,
    .id = 0,
    .lines = NULL,
    .lines_chosen = NULL,
    .condition = {0, 0, 0, 0},
    .participants = NULL,
};

int_id_t * Scene_Participants_Determine(struct Scene * in_scene, size_t * in_chosen, size_t chosen_num) {
    int_id_t * out_participants = NULL;
    int_id_t current_participant = 0;
    size_t participants_num = 0;
    for (size_t i = 0; i < chosen_num; i++) {
        current_participant = in_scene->lines[in_chosen[i]].speaker_id;
        if (current_participant > UNIT_NAME_START) {
            if (!linalg_isIn_uint16_t(out_participants, current_participant, participants_num)) {
                arrput(out_participants, current_participant);
                participants_num++;
            }
        }
    }
    return (out_participants);
}

size_t * Scene_Lines_Choose(struct Scene * in_scene, struct Narrative_Conditions narr_current, int_chapter_t in_chapter) {
    size_t * out_chosen = NULL;
    bool line_found = false;
    if (!Narrative_isInvalid(narr_current)) {
        for (size_t i = 0; i < in_scene->line_num; i++) {
            if ((Narrative_isInvalid(in_scene->lines[i].condition)) && (in_scene->lines[i].chapter_min == 0)) {
                if (line_found) {
                    line_found = false;
                } else {
                    arrput(out_chosen, i);
                }
            } else {
                if (Narrative_Match(narr_current, in_scene->lines[i].condition) && (in_scene->lines[i].chapter_min >= in_chapter)) {
                    if (!line_found) {
                        arrput(out_chosen, i);
                        line_found = true;
                    }
                }
            }
        }
    }
    return (out_chosen);
}

bool Narrative_isInvalid(struct Narrative_Conditions narr_current) {
    return (!narr_current.pc_alive && !narr_current.pc_recruited && !narr_current.npc_alive && !narr_current.misc);
}

bool Narrative_Match(struct Narrative_Conditions narr_current, struct Narrative_Conditions conditions) {
    bool match = true;
    uint64_t * narr_elems = (uint64_t *)(&narr_current);
    uint64_t * conditions_elems = (uint64_t *)(&conditions);
    for (uint8_t i = 0; i < sizeof(struct Narrative_Conditions) / sizeof(uint64_t); i++) {
        // printf("i %d \n", i);
        match &= (*(narr_elems + i) & *(conditions_elems + i)) == *(conditions_elems + i);
        // printf("match %d \n", match);
    }
    return (match);
}

// void baseBooks() {
//     struct Page temp_page;
//     struct Book temp_book;
//     char * temp_paragraph = NULL;

//     strcpy(temp_book.title, "Basics of Army Management");
//     strcpy(temp_book.author, "Ancestor of Erwin?");
//     temp_book.id = ITEM_NAME_BOOKBASICS;


//     strcpy(temp_page.title, "Knights on the field");
//     SDL_Log("%s", temp_page.title);
// strcpy(temp_paragraph, "In serving your King properly, there cones a tine where a Knight may be required to march out and fight battles in the field. This handbook intends to properly guide even the most lowly of squires into capable knights. ");
// arrput(temp_paragraph, {"aa"});
// SDL_Log("%s", temp_paragraph);
// arrput(temp_page.paragraphs, temp_paragraph);
// SDL_Log("%s", temp_page.paragraphs[0]);
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Knights on the field";
//     page.paragraphs.push_back("");
//     book.addPage(page);


//     book = Book("Weapons of the World", ITEM_NAME_BOOKWEAPONS);
//     book.setAuthor("Member of the Guild of blacksmiths?.");
//     Weapon_stats wpn_stats;
//     Unit_stats unit_stats;
//     std_string temp_str;
//     std_string unit_stats_labels = "HP, Str, Mag, Skl, Spd, Luck, Def, Res, Con, Move";
//     std_string wpn_stats_labels = "Pmight, Mmight, hit, dodge, crit, favor, wgt, uses, wpnlvl, range, hand, dmg_type, price";

//     // for (int i = ITEM_NAME_KITCHEN_KNIFE; i < ITEM_NAME_CROSS; i++) {
//     //     wpn_stats = all_weapons[i].getStats();
//     //     page.title = itemNames[i];
//     //     page.paragraphs.clear();
//     //     temp_str = stats2str(wpn_stats);
//     //     page.paragraphs.push_back("Stats");
//     //     page.paragraphs.push_back(wpn_stats_labels);
//     //     page.paragraphs.push_back(temp_str);
//     //     page.paragraphs.push_back("Bonus");
//     //     unit_stats = all_weapons[i].getBonus();
//     //     temp_str = stats2str(unit_stats);
//     //     page.paragraphs.push_back(unit_stats_labels);
//     //     page.paragraphs.push_back(temp_str);
//     //     page.paragraphs.push_back("Malus");
//     //     unit_stats = all_weapons[i].getMalus();
//     //     temp_str = stats2str(unit_stats);
//     //     page.paragraphs.push_back(unit_stats_labels);
//     //     page.paragraphs.push_back(temp_str);
//     //     page.paragraphs.push_back("Effects");
//     //     page.paragraphs.push_back("");
//     //     page.paragraphs.push_back("Users:");
//     //     page.paragraphs.push_back("");
//     //     page.paragraphs.push_back("Type:");
//     //     page.paragraphs.push_back("");
//     //     page.paragraphs.push_back("Description");
//     //     page.paragraphs.push_back(all_weapons[i].getDescription());
//     //     book.addPage(page);
//     // }

//     book = Book("Magic: The Definitive Guide", ITEM_NAME_BOOKELEMENTAL);
//     book.setAuthor("Ancestor of the gifted brothers.");


//     book = Book("Celestial Mathemagics", ITEM_NAME_BOOKMATHEMAGICS);
//     // Celestial Mathemagics?
//     //Pierre-Simon, marquis de Laplace-> Il est aisé de voir que... Mentions of the goddess go against the spirit of his books, but I don't really care. I this universe God and the Goddess exist for real.
//     book.setAuthor("Le marquis de Laplace");
//     page.title = "Introduction to Mathemagics";
//     page.paragraphs.push_back("Il est aisé de voir que...");
//     page.paragraphs.push_back("It is easy to see that this world is governed by randomness. Mages, soldiers and mathemagicians all realize this fact: nothing is certain. Taking but a single swing of a sword means to take a risk, to bet on your skills against your opponent's capacity to dodge.");
//     page.paragraphs.push_back("A novice enemy may very well carry the Goddess's favor and evade all attacks. While you, reader no doubt skilled in the arts of war, might only earn her scorn and receive a swift, painful death at the point of a spear.");
//     page.paragraphs.push_back("Such is the tapestry She weaves... May She have mercy on us all. Blasphemers say She is a capricious Goddess, that her judgements are cruel, unfair and arbitrary. Those of weak faith may yet be convinced already. But the tapestry of the world is never woven carelessly. Rules guide randomness.");
//     page.paragraphs.push_back("Mathemagics is the study of the rules that underline chance. We Mathemagicians believe that to fully understand the rules of randomness brings us closer to the Goddess. It has taken much work to test these theories and compile all these equations. We write them all here so that the curious might benefit from ascending into the realm of arcane Mathemagics.");
//     std_vector<std_string> paragraphs;
//     book.addPage(page);
//     page.paragraphs.clear();

//     page.title = "In our world, Random Events are all Independent";
//     page.paragraphs.push_back("Random events have a certain chance of occuring, generally noted in percents (%). For example, a simple coin has a 50\% chance of landing on its face. Flipping the coin once has no influence on the future coin flips. As such, we call these independent random events.");
//     page.paragraphs.push_back("These constrast directly with dependent events. For example, drawing cards. Imagine a standard 52 card deck, from which I draw a Jack of Diamond. The chance of me drawing next a Jack of Diamond is zero, while the chance of me drawing any other card from the deck is increased slightly (unless of course, I put the Jack of Diamond back in the deck.)");
//     page.paragraphs.push_back("All of the chance events in this world are independent. Having dodged an attack does not increase (nor decrease!) your likelihood of dodging the next attack.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Certainty is possible.";
//     page.paragraphs.push_back("This humble mathemagician has heard even Priests and Oracles of strong faith utter this maxim: Nothing is certain. This is untrue.");
//     page.paragraphs.push_back("Though certainty in the study of chance is very rare, it is also very much possible. The chance of an event happening is generally given either in percents, from 0\% (certain failure) to 100\% (certain success). It is also expressed as a decimal value between 0 and 1. Now, events that have a 0\% or 100\% chance of occuring are exceedingly rare. But one can turn to card games also to realize this fact. What is the chance of a cheater winning a game of cards? Why it is absolutely certain of course! Unless the cheater is of particularly bad skill...");
//     page.paragraphs.push_back("Given enough training, war mages and soliders may very well attain certainty, at least when facing enemies of particularly low skill. Again, in war as in life this is incredibly rare.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "The Cosmic Background Random Number Generator (RNG)";
//     page.paragraphs.push_back("All random events in this world are compared to the Cosmic Background Random Number Generator (RNG). All soldiers in this world have combat statistics, such as a hit rate (%). If this rate is greater than the next Random Number (RN) generated by the Cosmic RNG, the hit will land. The same applies to all other random events in this world.");
//     page.paragraphs.push_back("The interpreters of the Goddess's divine will always recite the following explanation when asked about the Cosmic Background Random Number Generator:");
//     page.paragraphs.push_back("Random numbers are created using a pseudo-random number generator called the Mersenne Twister: an algorithm that makes use of Mersenne Primes. The generator is always set to the same state upon creation (first startup). Its state is updated everytime a RN is generated. Knowing the state of the Mersenne Twister, one can perfectly predict the will of the Goddess.");
//     page.paragraphs.push_back("Interpretation of the words is beyond this humble mathemagician. What even are pseudo-random numbers? How do they compare to 'truly' random numbers? Those are questions better left for the Metamagicians and magical philosophers. Note that all Oracles strongly discourage finding the internal state of the Cosmic RNG, so as not, as they say 'diminish Creation'. We Mathemagicians agree.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "The Cosmic RNG is Discrete and Uniform";
//     page.paragraphs.push_back("Let us now take a moment to define important properties of the Cosmic RNG. We specifically wish to highlight two:");
//     page.paragraphs.push_back("1-The random numbers are Discrete. This means that the numbers the Goddess gives are whole integers: 0, 1, 2... up to 99. This is in opposition to continuous random numbers, for example human height is a continuous random number expressed as a length in meters. Continuous random numvers can take any decimal value, for example 1.45.");
//     page.paragraphs.push_back("2-The RNs are Uniform, between 0 and 99. This means that all numbers between these bounds have equal likelihood to be ordained. In the Cosmic RNG's case, it means that all numbers have an equal 1/100, 1% chance of being anointed. For a discrete RNG, these values are known as the probability mass function (PMF) of the distribution. An uniform distribution's PMF, like that of the Cosmic RNG, is very simple, since all possibilities are equality likely.");
//     page.paragraphs.push_back("We write the PMF of a uniform discrete distribution so: P(X = x_i) = 1/n. Or read aloud, the probability P that the next RN noted X is equal to a certain RN noted x_i, is equal to one over the total number of possible values, noted n. In our case, n is 100. Astute readers will note that the probability P does not depend on the value of the RN x_i, most unusually compared to other random distributions.");
//     page.paragraphs.push_back("These are all properties are all that is required to mathemagically derive useful values later on.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Single and Double Roll";
//     page.paragraphs.push_back("Two pages ago, we mentionned that 'if the hit rate is greater than the next Random Number (RN), the hit will land'. That is only true if we live in a Single Roll world. In reality, Creation is Double Roll. So was ordained Above.");
//     page.paragraphs.push_back("In a Double Roll universe, two RNs are averaged together, then compared to the Hit rate. If the hit rate is higher than the average of two RNs, the hit lands. The Goddess chose the double Roll for its simplicity, and to reward the skillful.");
//     page.paragraphs.push_back("An unfortunate side-effect of this universal constant is that the less skilled are punished. Above a hit rate of 50%, Double Roll increase the likelihood of a hit, but under 50\% decreases it instead. 'To everyone who has, more shall be given; but from those who do not have, everything shall be taken away.'");
//     page.paragraphs.push_back("The actual True hit probabilities will be derived later, using the properties of the Cosmic RNG defined in the previous page.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Randomn: hard to grasp by peasantry and aristocracy alike";
//     page.paragraphs.push_back("As mentionned in the previous page, the Goddess has woven for us a Double Roll world. We wish to expand on the effects and probable cause of this divine decision.");
//     page.paragraphs.push_back("In truth, both peasants and aristocrats are rather mediocre at judging chance. We all naturally tend to overestimate high chance events and underestimate low chance events. It may take years for a serious Mathemagician to get an intuitive grasp of chance. Some never do. ");
//     page.paragraphs.push_back("Gambling houses routinely swindle the public by exploiting this. The odds of winning are so astronomically low as to be impossible to grasped.");
//     page.paragraphs.push_back("Laymen may also mistakenly believe that regular patterns are random, while random patterns are orderly! Those of little faith regularly cry out 'The RNG is busted!' or 'What is going on? The RNG is broken!' when witnessing events ordained by the Goddess. No, unbeliever! the Cosmic RNG is working just as Her Divine Will commands! It is your lack of faith that makes you see order in the pseudo-chaos created by the Cosmic RNG. Divine is Her Will! Final is Her Decision! ");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Double Roll: PMF and CDF.";
//     page.paragraphs.push_back("To derive the true Hit rates in a Double roll World, what needs to be computed is the chance that the average of two numbers given by the Cosmic RNG is lower than the Hit rate. In mathemagical terms, we can write it so: P(X<x_i), the probability that the random variable X, which is an average of two RNs produced by the Cosmic RNG, is lower than a certain x_i, being the Hit Rate in the present case. In Mathemagics, we call the P(X<x_i) the Cumulative Distribution Function (CDF). Then, the True rates are simply the values obtained by the CDF. This function is closely linked to the previously mentionned Probability Mass Function (PMF).");
//     page.paragraphs.push_back("To obtain the CDF from the PMF, you just need the cumulative sum of the PMF. Mathemagically, to find P(X < 3), you need to add P(X = 2), P(X = 1), P(X = 0). For example, in a Single Roll universe the Hit rate is equal to the True hit. This is because we find P(X < 3) = 3% because P(X = 2) = P(X = 1) = P(X = 0) = 1/100. In a Double Roll world, True Hit is not equal to the Hit Rate. It is also less simple to derive the CDF from the PMF. We show the first step in this derivation on the next page: determining the probability mass function (PMF) of the average of two RN.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Double Roll: Deriving the PMF of the average of 2 RNs";
//     page.paragraphs.push_back("As was mentionned in the previous page, the True Hit rate in a Double roll universe is simply the CDF, which can be computed using the PMF, the chances of individual event occurance. Using the previously mentionned properties of the Cosmic RNG, we can derive the probabilities that a value is obtained by averaging two RNs. In total, there is 100x100 = 10000 possible combinations of two RNs ordained by the RNG. The derivation can be understood clearly with few examples.");
//     page.paragraphs.push_back("There is only a single combination of two RNs that can be averaged to 0: 0-0. This means that the probability P(X=0), for X the average of two RNs, is equal to 1/10000. The next possible average value is 1/2, with two possibilities, 0-1 and 1-0. This means that 1/2 is twice as likely to be observed as 0 when two uniform RNs are averaged! Note that averaging two RNGs increase the resolution so to speak. Instead of the possible values being 0,1,2.. . they are now 0, 0.5, 1, 1.5... ");
//     page.paragraphs.push_back("Anyhow, the number of possibilities that average to 1 and 1.5. 1, can be obtained by three combinations of two RNGs: 0-2, 2-0, and 1-1. This makes 1 thrice as likely to be observed compared to 0 in a Double Roll world! Similarly, 1.5 can be obtained by 4 combinations of two RNGs: 0-3, 2-1, 1-2, 3-0. The pattern is clear! Each averaged value has a 1/10000 more chance to be ordained by the Goddess in a Double Roll world! Until a maximal value of chance is obtained in the middle of the interval");
//     page.paragraphs.push_back("The same reasoning can be applied until we get to the value with maximal probability to be averaged at the middle: 49.5. There are 100 possible ways to average to RNs to be equal to 49.5: 0-99, 1-98, 2-97... 98-1, 99-0. This leads to a 100/10000 or 1% chance of obtaining 49.5 when averaging two RNs. The chances of obtaining the average 50 then starts decreasing, because there are only 99 possible combinations: 1-99, 2-98...99-1. The probability of two RNs being averaged then decreases until we reach 99, which can only be ordained by the combination 99-99, with a probability 1/10000.");
//     page.paragraphs.push_back("Then, the PMF of the average of two RNs outputted by the uniform RNG has this structure: Average values can take values 0, 0.5, 1, 1.5... 99. The increment is the 0.5, or the base step of the original RNGs divided by the number of RN being averaged. The change of obtaining 0 is 0.01%, 0.5 is 0.02%, increasing linearly until 49.5 at 1%, decreasing similarly at 50 to 0.99%, down to 0.01% at 99. A function that plots a triangle, essentially.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Double Roll: Computing the True hits";
//     page.paragraphs.clear();
//     page.paragraphs.push_back("It is trivial to derive the True Hits, or CDF, now that we have the PMF. We simply need to calculate the value P(X < x_i), which is the sum of all values of the PMF below the Hit rate x_i. For example, if your hit rate is 3, True hit is P(X < 3) = P(X = 0) + P(X = 0.5) + P(X = 1) + P(X = 1.5) + P(X = 2) + P(X = 2.5) = 0.01\% + 0.02\% + 0.03\% + 0.04\% + 0.05\% + 0.06% = 0.21\%");
//     page.paragraphs.push_back("This is the reasoning behind computing the True Hit, or the PDF, using the PMF. The table of Hit Rate to True Hit rates can be found on the following page.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "True Hit Table.";
//     page.paragraphs.push_back("");
//     book.addPage(page);


//     page.paragraphs.clear();
//     page.title = "Conclusion";
//     page.paragraphs.push_back("Thank you for reading this book. As the reader has no doubt realized, this book is intented to anyone tgat wish to deepen their gratitude for creation. Srikl mathemagics can be applied to anything, including war. A clear understanding of the chances of hits occuring, of critical hits landing helps everyone. We hope all readers can renew their fate in the Goddess' Tapestry, through the shining light of Mathemagics.");
//     book.addPage(page);

//     book = Book("Love blooms on the Battlefield", ITEM_NAME_BOOKLOVE);
//     book.setAuthor("A girl PC that likes Shipping. Kiara?.");

//     page.paragraphs.clear();
//     page.title = "Gaussian Roll";
//     page.paragraphs.push_back("");
//     book.addPage(page);

//     book = Book("Growth & Potential", ITEM_NAME_BOOKGROWTHS);
//     book.setAuthor("Scribe in Erwin's army.");
//     Unit_stats temp_stats;

//     // for (int i = UNIT_NAME_ERWIN; i < UNIT_NAME_PC_END; i++) {
//     //     page.paragraphs.clear();
//     //     page.title = unitNames[i];
//     //     temp_stats = units[i].getStats();
//     //     stats2str(temp_stats)
//     //     page.paragraphs.push_back("Growths:");
//     //     page.paragraphs.push_back("Caps:");
//     //     page.paragraphs.push_back("");
//     //     book.addPage(page);
//     // }

//     book = Book("Angelic and Demonic Magics", ITEM_NAME_BOOKPOSSESSION);
//     book.setAuthor("Laplace the Second");

//     page.paragraphs.clear();
//     page.title = "Foreword";
//     page.paragraphs.push_back("I dedicate this book all magicians who, like me, has been lied to by the School of Magic. The fathers of Magic thought that the only magic that existed is the one that they wielded. We know today that their Magic is Elemental: a single branch of magic. I discovered there exists Angelic and Demonic magic. If more magic types exist, I will find them.");
//     page.paragraphs.push_back("I was the first to use the chaotic and harmonious energies around us. Later, the possessed thought it was impossible to use more than one branch of magic, or to reverse what they named 'possession'. Some still doubt today that I even exist. If you doubt, come find me at my Dual School of Order&Chaos. I can dive deep into what idiots call possession and come back. They lose their humanities and become lower, almost like beasts.");
//     page.paragraphs.push_back("Listen to me, if you wish to learn to use any kind of magic. I can use all magics. I will show you how.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Harmony and disorder";
//     page.paragraphs.push_back("We swim in a sea of mixed chaos and order. Some days I feel like walking is like swimming through molasses, other times I feel I want to burn my house down out of rage. Sometimes, I feel inspired enough to write a book... You can feel this as the winds of magic blow through you. You don't know it, but your body does. It changes your mood, lifts your spirit or crushes your soul. If you listen to it, you can learn to feel the thickness and flow of the magic that surrounds you.");
//     page.paragraphs.push_back("Everything we can see, we can also touch, move it around, or even destroy it. We can touch the things we see. Magic is the same. Before learning to touch it, you must see it. It is hard to find something in the dark, even in your own room. I doubt you could find anything in true darkness. Magic is exactly the same. You won't find it if you can't feel it. This is the first step.");
//     page.paragraphs.push_back("When you can feel it, you will be able to see how it changes when you touch it, if it gets stronger or weaker. And after that maybe you can use it to kill someone... Or write spells that can be used by idiots.");
//     page.paragraphs.push_back("This is why people think using angelic or demonic leads to possession. Spells make it too easy on stupid people. Reading a spell is much easier than forging it. No need to feel the flow of magic in you or around you. You can't know what demonic energy does to you if you can't feel it at all. And most 'possessed' can't. I have seen many people become possessed. You can feel the magic filling their body, way before you can see it with your eyes. Everything about them changes! Their angelic thoughts change their inner balance completely, dropping into a vicious circle. A large amount of inner Angelic energies encourage angelic thoughts. Angelic thoughts attract more angelic energy. Down, down, down it goes, until the person disappears inside the Angel.");
//     page.paragraphs.push_back("Keep reading if you don't want this to happen to you.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Where does Magic live?";
//     page.paragraphs.push_back("It's not hard to know where angelic or demonic wind accumulates. A cemetary. A castle, a church, a brothel. A fresh dug mass grave, a library, a drunkard's house... In all these places do angelic and demonic energies live. Maybe even in a person! You should make use of someone like that as much as possible. If you want to feel Elemental magic, go outside.");
//     page.paragraphs.push_back("In a place where you know a lot of magic lives... Close to an object or person in which magic resides... Listen to your body, feel it in your guts, in your heart. Feel the winds change direction, the magic getting more or less thick. Your body can feel it, you just need to listen. It always flow through you and around you.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Feeling the magic";
//     page.paragraphs.push_back("At first, you will need to focus to just be able to feel the magic. Don't worry, it's like a muscle. The more you use it, the stronger it gets. It helps to clear your thoughts, and to focus on your senses. Your smell, your taste, your hearing, the surrounding heat and cold, and especially your sense of balance. The feeling of magic is closely tied to your own sense of balance. Once you feel where down and up are, you will start feeling the magic energies around you.");
//     page.paragraphs.push_back("Practice. It will become easy, almost natural. You'll be able to feel magic all the time, even while working or being focused on something else. Once you can feel the magic surrounding you, you can start taking notes on the general patterns. In this city, chaos reigns. You can even use this sense to feel major changes happening, before you can hear about them. This is also why mages flee castle towns days weeks or even months before a siege. Have you ever heard of many mages being stuck in a siege? No. You hear of how there are never enough mages in armies during a siege. This is the reason why.");
//     page.paragraphs.push_back("This is also why even if you have no talent in magic, you should still learn to feel the magic. This is a very useful ability that everyone can learn. You can feel magic before you can hear, touch, or even see. Everyone benefits form feeling magic. It can be used in many ways.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Touching the magic";
//     page.paragraphs.push_back("If you get bored of only feeling the magic, reach out and touch the magic. You can flap your arms like a chicken and make wind. Magic can be influenced by you exactly like that. Contrarily to wind though, magic is also influenced by your thoughts. The harder you focus, the more powerful your influence becomes.");
//     page.paragraphs.push_back("Once you can feel and touch magic, you can force it to take shape in the physical world in ways that are useful to you. The classic example is the Angelic Light spear, which you can throw at demons to pierce their heart. Very useful! Very simple. Accessible to all. Practice making magic into many kinfs of useful shapes and sizes.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Casting spells.";
//     page.paragraphs.push_back("Do not cast spells written by others. You can perfectly cast spells without them, of even create your own. That requires a deep understand of magic, which is difficult to get: hard work over many years of training. It will make you a better Mage. Spells can be recited by newborn idiots.");
//     page.paragraphs.push_back("Written spells make it easy to blast enemies without understanding magic. Spells are weapons of war, not tools of the student of Magic. Though I think even war mages should learn to create their own spells. Military mages disagree with me, sometimes very violently. They have their own very convincing stupid reasons.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Writing magic spells.";
//     page.paragraphs.push_back("Do not write magic spells.");
//     page.paragraphs.push_back("Angelic, demonic or elemental. Do not write any magic spells!");
//     page.paragraphs.push_back("Written spells can get in the hands of idiots. Spells in your mind can't be stolen or used by anyone else.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "What is possession?";
//     page.paragraphs.push_back("You can hear many magic users talk about possession. 'He became possessed by the demonic energies'. 'He turned into a demon and devoured her!' That is a horrible way to explain it. Demonic energies do not randomly concentrate themselves in a body. A human does not naturally get rid of almost all of it's harmonious and elemental energies. The people do it, and keep it that way. Whether they realize it or not, this is what they are doing.");
//     page.paragraphs.push_back("The bodies of the 'possessed' are simply adjusting themselves to an incredible amount of demonic energies. A frail human body cannot contain so much ill will. Only the body of a Demon can. The 'possessed' are just transforming into Demons. Some people think it is slow. But it can take only only a few moments to become a Demon, if you are used to it. For novices, it might take weeks. Many poisons take much more time to kill you. Ills can take root for many years before risIng to the surface. But demonic energies act on the heart and body almost instantly.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Reverting possession";
//     page.paragraphs.push_back("Stopping the transformation is rather simple. Magic energies normally fill up our bodies in balance. To stop the transformation, you must go back to your original point of balance. Finding places that are full of your missing energies and have a lack of your excesses can help with that.");
//     // Implicit-> need to feel all three types of magic + how to manipulate them willingly 3 types of magic! No wonder people fall into the trap of so-called possession. They can almost always willingly manipulate only one type of magic, at best.

//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Transforming at will";
//     page.paragraphs.push_back("It is easy to feel the balance in normal humans. It is also easy to notice Angels barely contain any demonic or elemental energy. To transform into a Demon, an Angel or an Elemental, you must destroy your internal magic balance. Or at least, accumulate enough magic for you to lean very hard in only one direction.");
//     page.paragraphs.push_back("Your body will react quite quickly to dramatic changes. For your first time, you should unbalance yourself very slowly, in a couple hours. The more you practice, the shorter time it will take you to completely change from a 'possessed' to a full Demon, all the way back to human all over again.");
//     page.paragraphs.push_back("Before diving, you should fully understand the original balance in you. And when you dive, make sure to Remember your balance. Remember your balance! The feel of it, the shape of it, it's flow and direction. That way, you will never be lost. Remember your balance! Even in the most demonic of places, incarnating the most evil Demon, remember your balance! Remember who you are! Only then will you be able to make your way back.");
//     book.addPage(page);

//     page.paragraphs.clear();
//     page.title = "Creating new spells from nothing";
//     page.paragraphs.push_back("I have been asked many times: 'How is there so many spells? Just use magic to kill people!'. This is wrong.");
//     page.paragraphs.push_back("There are many weapons. All with the objective of hurting or killing people. Some cut, some crush, some stab, some increase your critical hit chance... Some are better at cutting then others. Some weapons are made to be thrown, some can be thrown but will not fly true. The same metal can be forged into completely different weapons by blacksmiths of different skill.");
//     page.paragraphs.push_back("The same is true of magic. The strength of the spells depends not only on the skill of the caster, but on the spellforger. Nothing is easy. Spellcasting isn't easy and neither is spellforging. But both skills are required for a pupil to truly become a Mage.");
//     page.paragraphs.push_back("I will not tell you how to spellforge. There is a way, but you should embark on it yourself. Maybe I will write a book on spellforging, for Mages that already started forging. For later.");
//     book.addPage(page);


//     page.paragraphs.clear();
//     page.title = "From mage to sage.";
//     page.paragraphs.push_back("Who knows how to where wisdom lives and how to acquire it? I did not find it at the School of Magic, even though they can teach you some very useful tricks. If any Mage finds a reliable source of Wisdom, tell me.");
//     book.addPage(page);


//     page.paragraphs.clear();
//     page.title = "The Purpose of Magic";
//     page.paragraphs.push_back("This book is a results of years of experimentation, of hard study and of asking questions to myself, people around me, even the trees and rivers. I have not yet found all my answers, but the Angelic and Demonic energies set my life on a new course of discovery. Now, I want to share my knowledge with as many people as possible.");
//     page.paragraphs.push_back("Magic has a purpose, in this world. I do not understand it yet. And I wish to recruit you in my quest to discover the Purpose of Magic. Come find me, if you wish to help me on this quest. Everyone knows my name, now...");
//     book.addPage(page);


//     page.paragraphs.clear();
//     page.title = "Gaussian Roll";
//     page.paragraphs.push_back("");
//     book.addPage(page);

// }
