#ifndef UNIT_HPP
#define UNIT_HPP
using namespace std;
#include "shared.hpp"
#include "generic.hpp"
#include "inventory_item.hpp"
/*! \file unit.hpp
* \brief Unit class: For everything from allies to neutrals to enemies.
*/
class unit: public generic {
    // Lightweight implementation.
private: 
    char equipped[1]; //needs setters for stats update. 
    // DOUBLE DUTY: equals -1 if nothing is equipped.
    // DOUBLE DUTY: is also the number of the last element in equipment. Maybe in weapons also.
    char current_hp; //needs setters for death check on update
    
public:
    char stats_bonus[10];
    /// \var stats_bonus
    /// \brief Bonuses can be negative -> maluses
    /// \var stats
    /// \brief current stats. Computed every level up using stats_grown.
    // for seom reason, stats array of len<19 cause cygwin_exception
    unsigned char stats[19], stats_base[10],
    growths[10], wpn_exp[10], position[3], skills[3],
    love_pts[5], love_growths[5],
    attack_probs[4], combat_probs[2];
    /// \var unsigned char attack_probs[4]
    /// \brief Probabilities associated with attacking, computed when alone.
    /// Accuracy, Avoid, Crit and Favor.
    /// \var unsigned char combat_probs[4]
    /// \brief probabilities associated with combat with an enemy unit.
    /// Accuracy, Avoid, Crit and Favor.
    /// \var unsigned char love_growths
    /// \brief Number of points units that have love potential get for *DESIGN QUESTION*.
    /// \var unsigned char growths
    /// \brief Unit stats growth: percent probability that upon level up, unit stats grows by one. Growth>100% are interpreted as meaning a unit can grow +1 or +2.
    /// \var unsigned char position
    /// \brief Position on the map. 3D because of the possibility of fliers to fly higher. And underground paths.
    /// \var unsigned char skills
    /// \brief Skill names that unit possesses.    
    /// \var unsigned char wpn_exp
    /// \brief Wpn experience possessed by the unit. If a wpn_exp is equal to 0, unit cannot equip associated weapon type.    
    /// \var unsigned char love_pts
    /// \brief current love points. Refer to ...
    /// \var equipped
    /// \brief Equipped weapon. Index of of weapon in equipment vector or weapon vector. Is a vector in case not only weapon can be equipped. In such case equipped[0] would be the weapon, and equipped[1] an item, etc.
    std::vector<std::vector<unsigned char>> stats_grown;
    /// \var stats_grown
    /// \brief All stats grown for level ups. Stack of 10-length rows.
    /// \var current_hp
    /// \brief Current hit points.
    /// \var stats_base
    /// \brief Base statistics. Separated from growths to be able to plot growth over time.
    /// stats_grown
    struct ::inventory_item equipment[7], weapons[4], items[4];
    /// \var equipment
    /// \brief If equipment is not empty, weapons and items are not used. And vice versa.
    /// \var items
    /// \brief Only used if equipment is not. Slots only for items/not weapons.
    /// \var weapons
    /// \brief Only used if equipment is not. Slots only for weapons.
    /// The contents of the equipment array are inventory_item structs.
    unsigned short int exp;
    /*! \var unsigend short int exp
    * \brief Total Unit Experience points.
    * Not unit level in data. The experience points left most digits are the level. Example: 423exp means unit level 5 with 23 exp.
    */
    char lovers[5][14]; 
    /// \var char lovers
    /// \brief Names of possible lovers.
    bool mounted, flying, armored, promoted;
    /// \var bool mounted
    /// \brief. 1/0 mounted. For effective weapons
    /// \var bool flying
    /// \brief. 1/0 flying. For effective weapons and movement    
    /// \var bool armored
    /// \brief. 1/0 unit amored. For effective weapons
    /// \var bool promoted
    /// \brief. 1/0 unit promoted. For exp on enemy kill.
    /// \fn w  unit(std::string, std::string, char,         std::vector<unsigned int>, std::vector<unsigned int>,       std::vector<unsigned int>, std::vector<unsigned int>, std::vector<unsigned int>, std::vector<unsigned int>,      std::vector<unsigned int>, std::vector<unsigned int>, std::vector<inventory_item>, std::vector<inventory_item>, std::vector<inventory_item>, unsigned short, std::vector<std::string>, bool, bool, bool, bool)
    /// \brief constructor for unit.
    unit(std::string, std::string, char,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<inventory_item>,
        std::vector<inventory_item>, std::vector<inventory_item>, 
        unsigned short, std::vector<std::string>,
        bool, bool, bool, bool
        );
    /*! \fn unsigned char combat_damage(onst unit& enemy, bool critical)
    *  \brief Computes unit's damage during combat for a single attack, taking the critical into account.
    */
    unsigned char combat_damage(const unit&, bool critical);
    /*! \fn attack_damage()
    *  \brief Standalone unit's theoretical max damage for a single attack. Essentially, Str/Mag + weapon damage.
    */
    unsigned char attack_damage();
    /*! \fn accuracy()
    *  \brief Percent "probability" of standalone unit to hit enemy unit.
    *  Becomes a real probablity when substracted to enemy avoid.
    */
    unsigned char accuracy();
    /*! \fn combat_hit()
    *  \brief Percent probability of unit to hit enemy in combat.
    */
    unsigned char combat_hit(const unit&);
    /*! \fn avoid()
    *  \brief Percent "probability" of standalone unit to avoid incoming attack.
    *  Becomes a real probablity when substracted to enemy accuracy.
    */
    unsigned char avoid();
    /*! \fn critical()
    *  \brief Percent "probability" of standalone unit to perform critical hit on enemies.
    *  Becomes a real probablity when substracted to enemy avoid.
    */
    unsigned char critical();
    /*! \fn combat_critical(const unit& enemy)
    *  \brief Percent probability to perform a critical hit on your enemy in combat.
    */
    unsigned char combat_critical(const unit&);
    /*! \fn char favor()
    *  \brief Percent "probability" by which attacking enemy unit's critical chance get reduced. Essentially: crit avoid.
    *   I struggled to name this one. Alternatives include: blessings, blessed, divine, etc. All names though point to RNJesus: reducing the likelihood of getting *   your face critted off is divine after all. So yeah, I think of this value as RNJesus's divine favor.
    */
    unsigned char favor();
    /*! \fn take_damage()
    *  \brief Unit takes damage, decreases current_hp. Decided to keep take_damage and heal functions separate. Why? Dunno.
    *   Also checks for death.
    */  
    void take_damage(unsigned char);
    /*! \fn heal()
    *  \brief Heal unit, increases current_hp. Decided to keep take_damage and heal functions separate. Why? Dunno. Does not check for death.
    */
    void heal(unsigned char);
    /// \fn get_hp()    
    /// \brief getter for current_hp   
    unsigned char get_hp() const;
    /// \fn equip_weapon(std::vector<unsigned int>)
    /// \brief Setter for private equipped.
    void equip_weapon(std::vector<unsigned int>);
    /// \fn double_attack
    /// \brief Bool that returns if unit double attacks.
    bool double_attack(const unit&);
    /// \fn enemy_select
    /// \brief On enemy selection, compute combat probabilities, values, statistics, etc.
    void enemy_select(const unit&);
    /// \fn get_equipped
    /// \brief Getter for private equipped.
    const char* get_equipped() const;
    /// \fn read
    /// \brief read txt file, with format of write function.
    void read(const char*, char);
    /*! \fn retaliation(const unit& enemy)
    *  \brief When unit gets attacked in combat, does he attacks back? 1/0. Mainly a check for range.
    */  
    bool retaliation(const unit& enemy) const;
    /*! \fn bool combat_double(const unit& enemy)
    *  \brief Does unit perform a double hit on enemy? 1/0. Normally called in combat.
    */
    bool combat_double(const unit& enemy) const;
    /// \fn combat(unit& enemy)
    /// \brief Makes a single combat phase. Does the enemy retaliate? Does unit or enemy double? Makes all checks, then perform all attacks in order.
    void combat(unit& enemy);
    /*! \fn attack(unit enemy)
    *  \brief Perform a single attack on the enemy.
    *   An attack checks if hit connects, if it crits and then computes the damage, for a single attack.
    *   Doubling, brave effects and other skills detemine the number of attacks in function combat.
    */
    unsigned char attack(unit& enemy);
    /*! \fn unsigned char wpn_weighed_down()
    *  \brief Amount substracted to speed in combat because of weapon weight.
    */
    // I think this should be shown in the UI. It should show the max value and current value as a function of equipped weapon, with something saying 'WEIGHED DOWN' or something.
    unsigned char wpn_weighed_down() const;
    /// \fn void write
    /// \brief write the object to file. Keeps the exact same formatting as the friend << function.
    void write(const char *filename) {
        FILE *f = fopen(filename, "a");
        fprintf(f, "%s \n", name);
        fprintf(f, "Class: \t\t %s \n", type);
        fprintf(f, "Class: \t\t %s \n", type);
        fclose(f);
    }; 
    
    /// \brief friend << (it overloads it) for unit to write the unit to text file.
    friend std::ostream & operator << (std::ostream &out, const unit &in_unit) {
        out << in_unit.name << "\n";
        out << "Class: \t\t" << in_unit.type << "\n";
        out << "Bases: \t\t";
        for (int i = 0; i < sizeof(in_unit.stats_base)/sizeof(in_unit.stats_base[0]); i++) {
            if (i == (sizeof(in_unit.stats_base)/sizeof(in_unit.stats_base[0])) - 1) {
                out << (int) in_unit.stats_base[i] << "\n";
            } else {
                out << (int) in_unit.stats_base[i] << ", ";
            }
        };
        out << "Stats: \t\t" // must stay like this cause of busted stats variable that needs more space to not crash.
        << (int) in_unit.stats[0] << ", " << (int) in_unit.stats[1] << ", " 
        << (int) in_unit.stats[2] << ", " << (int) in_unit.stats[3] << ", " 
        << (int) in_unit.stats[4] << ", " << (int) in_unit.stats[5] << ", " 
        << (int) in_unit.stats[6] << ", " << (int) in_unit.stats[7] << ", " 
        << (int) in_unit.stats[8] << ", " << (int) in_unit.stats[9] << "\n"
        << "Growths: \t";
        for (int i = 0; i < sizeof(in_unit.growths)/sizeof(in_unit.growths[0]); i++) {
            if (i == (sizeof(in_unit.growths)/sizeof(in_unit.growths[0])) - 1) {
                out << (int) in_unit.growths[i] << "\n";
            } else {
                out << (int) in_unit.growths[i] << ", ";
            }
        };
        out << "Equipped: \t" << (int) in_unit.get_equipped()[0] << "\n"
        << "Equipment: \t";
        for (int i = 0; i < sizeof(in_unit.equipment)/sizeof(in_unit.equipment[0]); i++) {
            out << in_unit.equipment[i].name << ", ";
        };
        out << "\n" << "Weapons: \t";
        for (int i = 0; i < sizeof(in_unit.weapons)/sizeof(in_unit.weapons[0]); i++) {
            out << in_unit.weapons[i].name << ", ";
        };
        out << "\n" << "Items: \t\t"; 
        for (int i = 0; i < sizeof(in_unit.items)/sizeof(in_unit.items[0]); i++) {
            out << in_unit.items[i].name << ", ";
        };
        out << "\n" << "Lovers: \t";
        for (int i = 0; i < sizeof(in_unit.lovers)/sizeof(in_unit.lovers[0]); i++) {
            if (i == (sizeof(in_unit.lovers)/sizeof(in_unit.lovers[0])) - 1) {
                out << in_unit.lovers[i] << "\n";
            } else {
                out << in_unit.lovers[i] << ", ";
            }
        };
        out << "love_pts: \t";
        for (int i = 0; i < sizeof(in_unit.love_pts)/sizeof(in_unit.love_pts[0]); i++) {
            if (i == (sizeof(in_unit.love_pts)/sizeof(in_unit.love_pts[0])) - 1) {
                out << (int) in_unit.love_pts[i] << "\n";
            } else {
                out << (int) in_unit.love_pts[i] << ", ";
            }
        };
        out << "love_grt: \t";
        for (int i = 0; i < sizeof(in_unit.love_growths)/sizeof(in_unit.love_growths[0]); i++) {
            if (i == (sizeof(in_unit.love_growths)/sizeof(in_unit.love_growths[0])) - 1) {
                out << (int) in_unit.love_growths[i] << "\n";
            } else {
                out << (int) in_unit.love_growths[i] << ", ";
            }
        };
        out << "wpn_exp: \t";
        for (int i = 0; i < sizeof(in_unit.wpn_exp)/sizeof(in_unit.wpn_exp[0]); i++) {
            if (i == (sizeof(in_unit.wpn_exp)/sizeof(in_unit.wpn_exp[0])) - 1) {
                out << (int) in_unit.wpn_exp[i] << "\n";
            } else {
                out << (int) in_unit.wpn_exp[i] << ", ";
            }
        };
        out << "Exp: \t\t" << (int) in_unit.exp << "\n";
        out << "Mounted: \t" << in_unit.mounted << "\n";
        out << "Flying: \t"  << in_unit.flying << "\n";
        out << "Armored: \t" << in_unit.armored << "\n";
        std::cout << "is it flying?" << in_unit.flying << endl;
        out << "Promoted: \t" << in_unit.promoted << "\n";
        out << std::endl;
        return(out);
    }
     
    /*! \fn death()
    *  \brief What happens when character dies.
    */
    void death();
    ~unit();
    unit();
};

/*! \var all_units
* \brief Contains all instances of units.
*  Uses the unit's name as std::unordered_map's key.
*/
// I think a unordered map all_units should exist for every save.
extern std::unordered_map<string, unit> all_units;

#endif /* UNIT_HPP */