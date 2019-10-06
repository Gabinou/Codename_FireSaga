// Code créé par Gabriel Taillon
using namespace std;
#include "shared.hpp"
#include "generic.hpp"
#include "weapon.hpp"
#include "unit.hpp"
#include "unit_class.hpp"
#include "inventory_item.hpp"

std::vector<int> extract_int_string(string str) 
{ 
    // cannot read integers if not separated by at least one space and other chars.
    stringstream ss;
    /* Storing the whole string into string stream */
    ss << str; 
    /* Running loop till the end of the stream */
    string temp; 
    int found; 
    std::vector<int> founds; 
    while (!ss.eof()) { 
        /* extracting word by word from stream */
        ss >> temp; 
        /* Checking the given word is integer or not */
        if (stringstream(temp) >> found) 
            // cout << found << " "; 
            founds.push_back(found);
        /* To save from space at the end of string */
        temp = ""; 
    } 
    return(founds);
} 

std::string words2str(std::vector<std::string > words) {
    std::string new_str;
    for (int i=0; i < words.size(); i++) {
        new_str += words[i];
        if (i != (words.size() - 1)) {
            new_str += " ";
        }        
    }
    return(new_str);
}

std::vector<std::string> get_words(std::string line) {
    std::vector<std::string> words;
    std::string word;
    std::stringstream iss(line); 
    while (iss >> word) 
        // cout << word << endl;
        words.push_back(word);
    return(words);
}

std::vector<int> csv_from_line(string line, string delimiter) {
    std::vector<int> names;
    std::size_t found;
    while ((found = line.find(delimiter)) != string::npos) {
        // std::cout << line.substr(0, found) << endl;
        names.push_back(std::stoi(line.substr(0, found).c_str()));
        line.erase(0, found + delimiter.length());
    }
    // std::cout << line << endl;
    names.push_back(std::stoi(line.c_str()));
    return(names);
}

std::vector<std::string> css_from_line(string line, string delimiter) {
    std::vector<std::string> names;
    std::size_t found;
    while ((found = line.find(delimiter)) != string::npos) {
        names.push_back(words2str(get_words(line.substr(0, found))));
        line.erase(0, found + delimiter.length());
    }
    names.push_back(words2str(get_words(line)));
    return(names);
}

std::vector<std::string> css_from_line(char *line) {
    char * pch;
    std::string word;
    std::string name;
    std::vector<std::string> names;
    char word_num;
    pch = strtok(line, ",");
    while (pch != NULL){
        name = "";
        std::string temp(pch);
        stringstream iss(temp);
        word_num = 0;
        while (iss >> word) {
            if (word_num > 0) {name+= " ";};
            name += word;
            word_num++;
        }
        names.push_back(name);
        pch = strtok (NULL, ",");
    }
    return(names);
}
 
int get_rand() { 
    return(dist(mt));
}

bool single_roll(int in_prob) {
    bool out = (get_rand() < in_prob);
    return(out);
}

bool double_roll(int in_prob) {
    int rng1 = get_rand();
    int rng2 = get_rand(); 
    bool out = ((rng1 + rng2) < (2 * in_prob));
    return(out);
}

string read_line(const char *filename, int skip){
    // 2019/07/30: skip should be a multiple of *number of lines written to weapon.txt* which is 8.
    FILE *f = fopen(filename, "r");
    char line[500];
    for (int i = 0; i < skip; i++) {
        if (fgets(line, sizeof(line), f)==NULL) {
            throw "eof Reached\n";
        } 
    }
    fgets(line, sizeof(line), f);
    std::string out(line);
    out.pop_back(); // fgets include the \n character at the end of the line. This removes it.
    fclose(f);
    return(out);
}

/// \fn void write_all_units(const char *filename, char const *savestyle)
/// \brief Write all_units stats to file.
// write_all_units must be implemented here.
void write_all_units(const char *filename, char const *savestyle) {
    if (savestyle == "cpp") {
        std::ofstream out(filename);
        for (auto& it: all_units) {
            out << it.second;
        }
        out.close();
    } else if (savestyle == "c") {
        remove(filename);
        for (auto& it: all_units) {
            // std::cout << it.second.name << endl;
            it.second.write(filename);
        }
    }
}

void read_all_unit_classes(const char *filename="classes_FE1.txt") {
    std::string line;
    std::ifstream inFile(filename); 
    short int line_num = std::count(std::istreambuf_iterator<char>(inFile), 
             std::istreambuf_iterator<char>(), '\n') + 1;
    for (int i = 0 ; i < line_num; i+=9) {
        line = "";
        try {
            line = read_line(filename, i);
        } catch (const char* msg) {
            break;
        }
        if (!line.empty() && line != "") {
            all_unit_classes[line.substr(9, line.size()).c_str()] = unit_class();
            all_unit_classes[line.substr(9, line.size()).c_str()].read(filename, i);
        }   
    inFile.close();
    }
}

void write_all_unit_classes(const char *filename) {
    std::remove(filename);
    for (auto& it: all_unit_classes) { // Iterate over unordered_map
        it.second.write(filename);
    }
}


void write_all_weapons(const char *filename, char const *savestyle = "cpp" ) {
    std::remove(filename);
    if (savestyle == "cpp") {
        for (auto& it: all_weapons) { // Iterate over unordered_map
            it.second.write(filename);
        }
    }
}

void write_all_maps(const char *filename) {
      
}


void read_all_maps(const char *filename) {
      
}

void read_all_weapons(const char *filename="weapons.txt") {
    std::string line;
    std::ifstream inFile(filename); 
    short int line_num = std::count(std::istreambuf_iterator<char>(inFile), 
             std::istreambuf_iterator<char>(), '\n') + 1;
    for (int i = 0 ; i < line_num; i+=10) {
        line = "";
        try {
            line = read_line(filename, i);
        } catch (const char* msg) {
            break;
        }
        if (!line.empty() && line != "") {
            all_weapons[line.c_str()] = weapon();
            all_weapons[line.c_str()].read(filename, i);
        }   
    inFile.close();
    }
    
    // This part oif read_all_weapons creates an inventory_item for every weapon in all_weapons.
    // Have to run this anyway, better to put it here.
    // std::unordered_map<std::string, weapon>::iterator it = all_weapons.begin();
    // while(it != all_weapons.end()) {
        // char key[(it->first).size() + 1];
        // strcpy(key, (it->first).c_str());
        // inventory_items[strcat(key,"_0001")] = inventory_item(key, 10);
        // it++;
    // }
    
}

void read_all_units(const char *filename) {
    std::string line;
    std::ifstream inFile(filename); 
    short int line_num = std::count(std::istreambuf_iterator<char>(inFile), 
             std::istreambuf_iterator<char>(), '\n') + 1;
    for (int i = 0 ; i < line_num; i+=16) {
        line = "";
        try {
            line = read_line(filename, i);
        } catch (const char* msg) {
            break;
        }
        if (!line.empty() && line != "") {
            all_units[line.c_str()] = unit();
            all_units[line.c_str()].read(filename, i);
        }   
    inFile.close();
    }
}