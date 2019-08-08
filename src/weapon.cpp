// Code créé par Gabriel Taillon
/*! \page handle weapon.cpp
* \file weapon.cpp 
* \brief weapon class implementation.
*/
using namespace std;
#include "weapon.hpp"
#include "generic.hpp"
#include "shared.hpp"

weapon::weapon(){
    strncpy(name, "", 30); // for some reason in_name is always of size 8. Whatever.
    strncpy(type, "", 30);
    dmg_type = 0;
    for (int i = 0; i < 2; i++) {
        strncpy(effective[i], "", sizeof(""));
    }
    for (int i = 0; i < 10; i++) {
        stats_bonus[i] = 0;
    }
    for (int i = 0; i < 10; i++) {
        stats[i] = 0;
    }
    for (int i = 0; i < 2; i++) {
        range[i] = 0;      
    }
    cost = 0;
    exp = 0;
}

weapon::~weapon(void) {
    // printf("Weapon %s is being deleted.\n" , name);
}

string extension(string filename) {
    return(filename.substr(filename.find("."), filename.size())); 
}

void weapon::write(string filename) {
    // This is c++-style.
    // Compare with c-style write.
    std::ofstream myfile;
    if (extension(filename) == ".bin") {
        printf("this is a bin\n");
        myfile.open(filename, std::ios::binary);
        // myfile <<name << "\n";
        int testint = 7;
        myfile.write(reinterpret_cast<const char*>(&testint), sizeof(testint));
        // myfile <<type << "\n";
        // myfile <<exp << "\n";
        // for (int i = 0; i < sizeof(stats)/sizeof(stats[0]); i++) {
            // myfile <<(int) stats[i];
        // };
        // myfile <<"\n";
        // for (int i = 0; i < sizeof(stats_bonus)/sizeof(stats_bonus[0]); i++) {
            // myfile <<(int) stats_bonus[i] << ", ";
        // };
        // myfile <<cost << "\n";
        // myfile <<dmg_type << "\n";
        // if (range[1] == 0) {
            // myfile <<(int) range[0] << "\n";
        // } else {
            // myfile <<(int) range[0] << (int) range[1] << "\n";
        // }
        // myfile <<effective[0] << effective[1] << "\n";
        // myfile <<"\n";
    } 
    else if (extension(filename) == ".txt") {
        myfile.open(filename);
        myfile <<name << "\n";
        myfile <<"Type: \t\t";
        myfile <<type << "\n";
        myfile <<"Wpn_Exp: \t";
        myfile <<exp << "\n";
        myfile <<"Stats: \t\t";
        for (int i = 0; i < sizeof(stats)/sizeof(stats[0]); i++) {
            myfile <<(int) stats[i] << ", ";
        };
        myfile <<"\n" << "Bonus: \t\t";
        for (int i = 0; i < sizeof(stats_bonus)/sizeof(stats_bonus[0]); i++) {
            myfile <<(int) stats_bonus[i] << ", ";
        };
        myfile <<"\n" << "Cost: \t\t" << cost << "\n";
        myfile <<"Dmg_type: \t" << dmg_type << "\n";
        myfile <<"Range: \t\t";
        if (range[1] == 0) {
            myfile <<(int) range[0] << "\n";
        } else {
            myfile <<(int) range[0] << ", " << (int) range[1] << "\n";
        }
        myfile <<"Effective: \t";
        myfile <<effective[0] << ", " << effective[1] << "\n";
        myfile <<"\n";
    }
    myfile.close();
}

void weapon::read(const char *filename, char skip) {
    // 2019/07/30: skip should be a multiple of *number of lines written to weapon.txt* which is 10.
    // This is c-style.
    FILE *f = fopen(filename, "r");
    char line[500];
    for (int i = 0; i < skip; i++) {
        fgets(line, sizeof(line), f); // skips skip lines.
    }
    fgets(line, sizeof(line), f);
    line[strlen(line)-1] = 0;  //fgets also includes the \n in the line. This removes it.
    strncpy(name, line, sizeof(name));        
    fscanf(f, "%*s %s", type);
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    std::vector<int> temp = extractIntegerWords(line);
    fgets(line, sizeof(line), f);
    exp = temp[0];
    temp = extractIntegerWords(line);
    for (int i = 0; i < temp.size(); i++) {
        stats[i] = temp[i];
    }
    fgets(line, sizeof(line), f);
    temp = extractIntegerWords(line);
    for (int i = 0; i < temp.size(); i++) {
        stats_bonus[i] = temp[i];
    }     
    fgets(line, sizeof(line), f);      
    cost = extractIntegerWords(line)[0];          
    fgets(line, sizeof(line), f);
    dmg_type = extractIntegerWords(line)[0];
    fgets(line, sizeof(line), f);
    temp = extractIntegerWords(line);
    range[0] = temp[0];
    range[1] = temp[1];
    fseek(f, 10, SEEK_CUR);
    fgets(line, sizeof(line), f);
    char * pch;
    char eff = 0, i = 0;
    strncpy(effective[0], "", sizeof(""));    
    strncpy(effective[1], "", sizeof(""));    
    while (pch != NULL) {
        if (i == 0) {
            pch = strtok(line, ":,");
        } else {
            pch = strtok(NULL, ":,");       
        }
        if (pch!=NULL) {
            for (int j = 0; j < 33; j++) {
                if (strstr(pch, ::unit_classes[j]) != 0) {
                    strcpy(effective[eff], unit_classes[j]);
                    eff++;
                }
            }
            for (int j = 0; j < 4; j++) {
                if (strstr(pch, ::unit_attributes[j]) != 0) {
                    strcpy(effective[eff], unit_attributes[j]);
                    eff++;
                }
            }
        }
        i++;
    }
    fclose(f);
}; 

weapon::weapon(std::string in_name, std::string in_type, char in_id, unsigned short int in_cost,
       std::vector<int> in_stats, std::vector<int> in_range, std::vector<char> in_stats_bonus, 
       std::vector<std::string> in_effective, bool in_dmg_type, int in_exp) {
    strncpy(name, in_name.c_str(), 30); // for some reason in_name is always of size 8. Whatever.
    strncpy(type, in_type.c_str(), 30);
    dmg_type = (bool) in_dmg_type;
    for (int i = 0; i < in_effective.size(); i++) {
        strncpy(effective[i], in_effective[i].c_str(), sizeof(in_effective));
    }
    for (int i = 0; i < in_stats_bonus.size(); i++) {
        stats_bonus[i] = (int) in_stats_bonus[i];
    }
    for (int i = 0; i < in_stats.size(); i++) {
        stats[i] = in_stats[i];
    }
    for (int i = 0; i < in_range.size(); i++) {
        range[i] = in_range[i];      
    }
    cost = in_cost;
    exp = in_exp;
}