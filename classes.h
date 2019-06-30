#pragma once
#define classes_H

class generic {
    public:
        static int object_count;
        std::string name;
    generic();
};

class character: public generic {
    public:
        static int character_count;
        int id;
        std::vector<int> stats, stat_bonus, wpn_exp, equipment, position;
        bool mounted, flying, armored, promoted;
        int equipped, unit_class, level, current_hp;
        std::vector<std::string> weapons;
        std::string name;
        int combat_damage(void); 
        character(std::vector<int> in_stats,
                  std::vector<int> in_stat_bonus,
                  std::vector<int> in_wpn_exp,
                  std::vector<int> in_equipment,
                  std::vector<int> in_position);
        character();
};

class weapon: public generic  {
    public:
        static int wpn_count;
        int id;
        std::vector<int> stats, stat_bonus, range;
        std::vector<std::string> effective;
        weapon(std::string in_name,
               std::vector<int> in_stats,
               std::vector<int> in_stat_bonus,
               std::vector<std::string> in_effective);
               // Can either be a unit class, type or any other characteritic. Make a functions that recognizes the correct characteristic.
        weapon();
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