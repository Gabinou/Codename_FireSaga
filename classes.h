#pragma once
#define classes_H

class character {
    public:
        std::vector<int> stats, stat_bonus, wpn_exp, equipment, position;
        bool mounted, flying;
        int equipped, id, unit_class, level, promotion ;
        std::vector<std::string> weapons;
        int combat_damage(void); 
        character(std::vector<int> in_stats,
                    std::vector<int> in_stat_bonus,
                    std::vector<int> in_wpn_exp,
                    std::vector<int> in_equipment,
                    std::vector<int> in_position);
};

class weapon {
    public:
        std::vector<int> stats;
        std::vector<int> stat_bonus;
        std::string name;
        std::vector<std::string> effective;
        int id;
};

class tile {
    public:
        int id;
        int type;
        std::vector<int> stats;
    
};

class map {
    public:
        std::vector<int> map_size;
};