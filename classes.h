#ifndef classes_H
#define classes_H

class character {
    public:
        std::vector<int> stats, stat_bonus, wpn_lvls, equipment, position;
        bool mounted, flying;
        int equipped, id, unit_class;
        std::vector<std::string> weapons;
        int combat_damage(void); 
};

class weapon {
    public:
        std::vector<int> stats;
        std::vector<int> stat_bonus;
        std::string name;
        std::vector<std::string> effective;
        int id;
};

class map {
    public:
        std::vector<int> map_size;
};