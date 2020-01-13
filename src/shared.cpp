// Code créé par Gabriel Taillon
#include "shared.hpp"
#include "game.hpp"

void writeText(int in_fontsize, int in_position[2], float in_sizefactor[2], std::string in_text, SDL_Color in_color, TTF_Font * in_font, SDL_Renderer * in_renderer) {
    std::string text = "FPS";
    SDL_Texture * texture = textToTexture(text, in_color, in_font);
    SDL_Rect srcrect, destrect; // background always first?
    srcrect.x = srcrect.y = 0;
    destrect.x = in_position[0];
    destrect.y = in_position[1];
    srcrect.h = in_fontsize;
    srcrect.w = text.length() * in_fontsize;
    destrect.h = (int)in_fontsize * in_sizefactor[0];
    destrect.w = (int)text.length() * in_fontsize * in_sizefactor[1];
    SDL_RenderCopy(in_renderer, texture, &srcrect, &destrect);
    SDL_RenderPresent(in_renderer);
}

SDL_Texture * loadTexture(const char * filename) {
    SDL_Surface * tempsurface = IMG_Load(filename); //Not that fast.
    if (!tempsurface) {
        printf("loadTexture. IMG_Load: %s\n", IMG_GetError());
    }
    SDL_Texture * texture = SDL_CreateTextureFromSurface(Game::renderer, tempsurface);
    SDL_FreeSurface(tempsurface);
    return (texture);
}

SDL_Texture * textToTexture(std::string textureText, SDL_Color textColor, TTF_Font * in_font) {
    SDL_Surface * textsurface = TTF_RenderText_Blended(in_font, textureText.c_str(), textColor); // Blended is better than Solid
    // TTF_RenderText_Shaded shades the whole destrect background.
    
    SDL_Texture * texture;

    if (textsurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        //Create texture from surface pixels
        texture = SDL_CreateTextureFromSurface(Game::renderer, textsurface);

        if (texture == NULL) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(textsurface); //Get rid of old surface
    }
    return (texture);
}

int h_manhattan (int start[], int end[]){
    // fast. Exact on a grid.
    return(abs(start[0] - end[0]) + abs(start[1] - end[1]));
}

int h_manhattan (std::vector<int> start, std::vector<int> end){
    // fast. Exact on a grid.
    return(abs(start[0] - end[0]) + abs(start[1] - end[1]));
}

int h_manhattan (std::vector<int> start, int end[]){
    // fast. Exact on a grid.
    return(abs(start[0] - end[0]) + abs(start[1] - end[1]));
}

int h_manhattan (int start[], std::vector<int> end){
    // fast. Exact on a grid.
    return(abs(start[0] - end[0]) + abs(start[1] - end[1]));
}

double h_euclidean (int start[], int end[]){
    // Slower. More accurate.
    return(std::sqrt(pow(start[0] - end[0], 2.0) + pow(start[1] - end[1], 2.0)));
}

int find_row (int start[], std::vector<std::vector<int>> list) {
    int row;
    for (row = 0; row < list.size(); row++){
        if ((list[row][0] == start[0]) && (list[row][1] == start[1])) {
            return(row);
        }
    }
    return(-1);
}

std::vector<std::vector<int>> movemap(std::vector<std::vector<int>> map, int start[], int move, std::string mode){
    // Using the map, computes all moveable tiles.
    // outputs either a list of points, or a map of 1 and zeros.
    // Both outputs are 2D vectors.
    
    struct node{
        int x;
        int y;
        int distance;
    };
    std::vector<std::vector<int>> movemap;
    if (mode == "matrix"){
        movemap = map;
        for (int i = 0; i < movemap.size(); i++){
            std::fill(movemap[i].begin(), movemap[i].end(), 0);
        }
    }
    std::vector<node> open, closed;
    node current, neighbor;
    current.x = start[0];
    current.y = start[1];
    current.distance = 0;
    open.push_back(current);
    int index[2] = {-1, 1};
    while (!open.empty()) {
        current = open.back();
        open.pop_back();
        closed.push_back(current);
        if (mode == "matrix") { 
            movemap[current.y][current.x] = 1;
        }
        if (mode == "list") {
            movemap.push_back({current.x, current.y});
        } 
        // Two next whiles check the 4 neighbors. 
        // (i-j)/2 == 1 when (i+j)/2 == 0 and vice versa.
        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 2; j++) {
                neighbor.x = std::min(std::max(current.x + ((index[i]+index[j])/2),0), int(map[0].size())-1);
                neighbor.y = std::min(std::max(current.y + ((index[i]-index[j])/2),0), int(map.size())-1);
                neighbor.distance = current.distance + map[neighbor.y][neighbor.x];
                if ((neighbor.distance <= move) && (map[neighbor.y][neighbor.x] > 0)) {
                    bool inclosed = false;
                    for(int k = 0; k < closed.size(); k++) {
                        if ((neighbor.x == closed[k].x) && (neighbor.y == closed[k].y)) {
                            inclosed = true;
                            if (neighbor.distance < closed[k].distance){
                                inclosed = false;
                                closed.erase(closed.begin() + k);
                            }
                            break;
                        }
                    }
                    if (!inclosed) {
                        open.push_back(neighbor);
                    }
                } 
            } 
        }
    }
    return(movemap);
}

std::vector<std::vector<int>> attackmap(std::vector<std::vector<int>> movemap, int start[], int move, int attack, std::string mode){
    // Using the movemap to computes all attackable tiles.
    // EXCLUDING moveable tiles.

    std::vector<std::vector<int>> attackmap;
    if (mode == "matrix"){
        attackmap = movemap;
        for (int i = 0; i < attackmap.size(); i++){
            std::fill(attackmap[i].begin(), attackmap[i].end(), 0);
        }
    }
    bool add;

    int min_rows = std::max(start[0] - move - attack - 1, 0);
    int max_rows = std::min(start[0] + move + attack + 1, int(movemap[0].size()));
    int min_cols = std::max(start[1] - move - attack - 1, 0);
    int max_cols = std::min(start[1] + move + attack + 1, int(movemap.size()));

    for (int row = min_rows; row < max_rows; row++){
        for (int col = min_cols; col < max_cols; col++){
            add = false;
            for (int att = 1; att <= attack; att++){
                if ((movemap[col][row] == 0)) {
                    if (movemap[col][std::min(row + att, int(movemap[0].size() - 1))] == 1) {
                        add = true;
                        break;
                    }
                    if (movemap[col][std::max(row - att, 0)] == 1) {
                        add = true;
                        break;
                    }
                    if (movemap[std::min(col + att, int(movemap.size() - 1))][row] == 1) {
                        add = true;
                        break;
                    }
                    if (movemap[std::max(col - att, 0)][row] == 1) {
                        add = true;
                        break;
                    }
                }
            }
            if (add) {
                if (mode == "matrix") {
                    attackmap[col][row] = 1;
                }
                if (mode == "list"){
                    attackmap.push_back({col, row});
                }
            }
        }
    }
    return(attackmap);
}

int find_node (int start[], std::vector<std::vector<int>> list) {
    int row;
    for (row = 0; row < list.size(); row++){
        if ((list[row][0] == start[0]) && (list[row][1] == start[1])) {
            return(row);
        }
    }
    return(-1);
}


std::vector<std::vector<int>> A_star(std::vector<std::vector<int>> map, int start[], int end[], std::string mode){
    // Arrays containing points are arrays:
    // [0-1] = point [2] = f value [3-4] previous point.
    std::vector<std::vector<int>> openlist;
    std::vector<std::vector<int>> closedlist;
    std::vector<int> current;
    openlist.push_back({start[0], start[1], 0, 0, 0, 0});
    current = openlist.back();  
    int swap_index;
    int inclosedlist_index;
    int inopenlist_index;
    int cost;
    int g_neighbor;
    int h_neighbor;
    int neighborxp[2];    
    int neighborxm[2];    
    int neighboryp[2];    
    int neighborym[2];    
    int (*neighbors[4])[2];
    printf("%s\n", mode.c_str());
    while(!openlist.empty()){
        current = openlist.back();  
        closedlist.push_back(current);
        if ((current[0] == end[0]) && (current[1] == end[1])){
            break;
        } else {
            openlist.pop_back();
        }
        neighborxp[0] = std::min(255, current[0] + 1);
        neighborxp[1] = current[1];
        neighborxm[0] = std::max(0, current[0] - 1);
        neighborxm[1] = current[1];
        neighboryp[0] = current[0];
        neighboryp[1] = std::min(255, current[1] + 1);
        neighborym[0] = current[0];
        neighborym[1] = std::max(0, current[1] - 1);

        neighbors[0] = &neighborxp;
        neighbors[1] = &neighborxm;
        neighbors[2] = &neighboryp;
        neighbors[3] = &neighborym;

        for (int i = 0; i<4; i++) {
            if (map[(*neighbors[i])[0]][(*neighbors[i])[1]] > 0){
                inopenlist_index = find_row(*neighbors[i], openlist);
                inclosedlist_index = find_row(*neighbors[i], closedlist);

                h_neighbor = h_manhattan(*neighbors[i], end);
                g_neighbor = current[2] + map[(*neighbors[i])[0]][(*neighbors[i])[1]];

                if (inopenlist_index > 0) {
                    if (g_neighbor < openlist[inopenlist_index][2]) {
                        // printf("erase open\n");
                        openlist.erase(openlist.begin() + inopenlist_index);
                    }
                }
                if (inclosedlist_index > 0) {
                    if (g_neighbor < closedlist[inclosedlist_index][2]) {
                        closedlist.erase(closedlist.begin() + inclosedlist_index);
                    }
                }
                if ((inopenlist_index < 0) && (inclosedlist_index < 0)) {
                    openlist.push_back({(*neighbors[i])[0],(*neighbors[i])[1], g_neighbor, h_neighbor + g_neighbor, current[0], current[1]});
                }
                if (openlist.size()>1) {
                    for (swap_index = (openlist.size()-1); swap_index >= 1; swap_index--) {
                        if (openlist[swap_index-1][3] < openlist[swap_index][3]) {
                            std::iter_swap(openlist.begin() + swap_index - 1, openlist.begin() + swap_index);
                        }
                    }
                }
            }
        }
    }
    std::vector<std::vector<int>> path;
    std::vector<std::vector<int>> pathmap;
    if (mode == "matrix") {
        pathmap = map;
        for (int i = 0; i < pathmap.size(); i++){
            std::fill(pathmap[i].begin(), pathmap[i].end(), 0);
        }
    }  
    path.push_back(closedlist.back());
    closedlist.pop_back();
    while (!closedlist.empty()){
        if ((closedlist.back()[0] == path.back()[4]) && (closedlist.back()[1] == path.back()[5])) {
            printf("d");
            path.push_back(closedlist.back());
            if (mode == "matrix") {
                pathmap[closedlist.back()[0]][closedlist.back()[1]] = 1;
            }
        }
        closedlist.pop_back();
    }
    if (mode == "matrix") {
        path = pathmap;
    }
    return(path);
}

void permutations_binary(int len, int num_0, int out[], int i) {
// cannot be used for pathfinding.
    out[i] = 0;
    if (num_0 > std::count(out, out + i, 0)) { 
        permutations_binary(len, num_0, out, i+1);
    }
    out[i] = 1;
    if ((len-num_0) > std::count(out, out + i, 1)) { 
        permutations_binary(len, num_0, out, i+1);
    }
    if (i == len){
        for (int i = 0; i < len; i++) { 
            printf("%d",out[i]);
        } 
        printf("\n");
    }
}


int pingpong(int current, int upper, int lower) {
    // returns pingpong index.
    // modulo: x % 2 = 0,1,2,0,1,2,0...for x++
    // pingpong(x, 2) = 0,1,2,1,0,1,2... for x++
    int mod_factor = (2 * (upper - lower) - 2);
    int term1 = mod_factor - (current % mod_factor);
    int term2 = current % mod_factor;
    return (std::min(term1, term2) + lower);
}


int geometricslide(int distance, float geo_factor) {
    // geometric slide cause the series is geometric:
    // distance/2 + distance/4 + distance/16....
    // animates slides
    int sign = sgn(distance);
    int out = sign * std::max(sign * (int)(distance / geo_factor), 1);
    return (out);
    // std::abs() possible instead of sign*distance,
    // but this is more elegant.
}

bool is_pressed(const Uint8 * state_array, std::vector<SDL_Scancode> to_find) {
    for (auto it = std::begin(to_find); it != std::end(to_find); ++it) {
        if (state_array[*it]) {
            return (true);
        }
    }
    return (false);
}

std::vector<int> extract_int_string(std::string str) {
    // cannot read integers if not separated by at least one space and other chars.
    std::stringstream ss;
    /* Storing the whole string into string stream */
    ss << str;
    /* Running loop till the end of the stream */
    std::string temp;
    int found;
    std::vector<int> founds;

    while (!ss.eof()) {
        /* extracting word by word from stream */
        ss >> temp;

        /* Checking the given word is integer or not */
        if (std::stringstream(temp) >> found)
            // cout << found << " ";
        { founds.push_back(found); }

        /* To save from space at the end of string */
        temp = "";
    }

    return (founds);
}

std::string words2str(std::vector<std::string > words) {
    std::string new_str;

    for (int i = 0; i < words.size(); i++) {
        new_str += words[i];

        if (i != (words.size() - 1)) {
            new_str += " ";
        }
    }

    return (new_str);
}

std::vector<std::string> get_words(std::string line) {
    std::vector<std::string> words;
    std::string word;
    std::stringstream iss(line);

    while (iss >> word)
        // cout << word << endl;
    { words.push_back(word); }

    return (words);
}

std::vector<int> csv_from_line(std::string line, std::string delimiter) {
    std::vector<int> names;
    std::size_t found;

    while ((found = line.find(delimiter)) != std::string::npos) {
        // std::cout << line.substr(0, found) << endl;
        names.push_back(std::stoi(line.substr(0, found).c_str()));
        line.erase(0, found + delimiter.length());
    }

    // std::cout << line << endl;
    names.push_back(std::stoi(line.c_str()));
    return (names);
}

std::vector<std::string> css_from_line(std::string line, std::string delimiter) {
    std::vector<std::string> names;
    std::size_t found;

    while ((found = line.find(delimiter)) != std::string::npos) {
        names.push_back(words2str(get_words(line.substr(0, found))));
        line.erase(0, found + delimiter.length());
    }

    names.push_back(words2str(get_words(line)));
    return (names);
}

std::vector<std::string> css_from_line(char * line) {
    char * pch;
    std::string word;
    std::string name;
    std::vector<std::string> names;
    char word_num;
    pch = strtok(line, ",");

    while (pch != NULL) {
        name = "";
        std::string temp(pch);
        std::stringstream iss(temp);
        word_num = 0;

        while (iss >> word) {
            if (word_num > 0) {name += " ";};

            name += word;

            word_num++;
        }

        names.push_back(name);
        pch = strtok(NULL, ",");
    }

    return (names);
}

/// \fn dist
/// \brief gets the next random number, using pre-defined Mersenne-Twister object applied to pre-defined uniform distribution.
std::uniform_int_distribution<int> dir_99(0, 99); // more twice faster than Ureal_1

bool single_roll(const int RN, const int hit) {
    return((RN<hit));
}

bool double_roll(const int RN1, const int RN2, const int hit) {
    return((((RN1+RN2)/2)<hit));
}


std::string read_line(const char * filename, int skip) {
    // 2019/07/30: skip should be a multiple of *number of lines written to weapon.txt* which is 8.
    FILE * f = fopen(filename, "r");
    char line[500];

    for (int i = 0; i < skip; i++) {
        if (fgets(line, sizeof(line), f) == NULL) {
            throw "eof Reached\n";
        }
    }

    fgets(line, sizeof(line), f);
    std::string out(line);
    out.pop_back(); // fgets include the \n character at the end of the line. This removes it.
    fclose(f);
    return (out);
}

/// \fn void write_all_units(const char *filename, char const *savestyle)
/// \brief Write all_units stats to file.
// write_all_units must be implemented here.
void write_all_units(const char * filename, char const * savestyle) {
    // if (savestyle == "cpp") {
    //     std::ofstream out(filename);

    //     for (auto & it : all_units) {
    //         out << it.second;
    //     }

    //     out.close();
    // } else if (savestyle == "c") {
    //     remove(filename);

    //     for (auto & it : all_units) {
    //         // std::cout << it.second.name << endl;
    //         it.second.write(filename);
    //     }
    // }
}

void read_all_unit_classes(const char * filename) {
    // std::string line;
    // std::ifstream inFile(filename);
    // short int line_num = std::count(std::istreambuf_iterator<char>(inFile),
    //                                 std::istreambuf_iterator<char>(), '\n') + 1;

    // for (int i = 0 ; i < line_num; i += 9) {
    //     line = "";

    //     try {
    //         line = read_line(filename, i);
    //     } catch (const char * msg) {
    //         break;
    //     }

    //     if (!line.empty() && line != "") {
    //         all_unit_classes[line.substr(9, line.size()).c_str()] = unit_class();
    //         all_unit_classes[line.substr(9, line.size()).c_str()].read(filename, i);
    //     }

    //     inFile.close();
    // }
}

void write_all_unit_classes(const char * filename) {
    // std::remove(filename);

    // for (auto & it : all_unit_classes) { // Iterate over unordered_map
    //     it.second.write(filename);
    // }
}


void write_all_weapons(const char * filename, char const * savestyle) {
    // std::remove(filename);

    // if (savestyle == "cpp") {
    //     for (auto & it : all_weapons) { // Iterate over unordered_map
    //         it.second.write(filename);
    //     }
    // }
}

void write_all_maps(const char * filename) {
}


void read_all_maps(const char * filename) {
}

void read_all_weapons(const char * filename) {
    // std::string line;
    // std::ifstream inFile(filename);
    // short int line_num = std::count(std::istreambuf_iterator<char>(inFile),
    //                                 std::istreambuf_iterator<char>(), '\n') + 1;

    // for (int i = 0 ; i < line_num; i += 10) {
    //     line = "";

    //     try {
    //         line = read_line(filename, i);
    //     } catch (const char * msg) {
    //         break;
    //     }

    //     if (!line.empty() && line != "") {
    //         all_weapons[line.c_str()] = weapon();
    //         all_weapons[line.c_str()].read(filename, i);
    //     }

    //     inFile.close();
    // }

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

void read_all_units(const char * filename) {
    // std::string line;
    // std::ifstream inFile(filename);
    // short int line_num = std::count(std::istreambuf_iterator<char>(inFile),
    //                                 std::istreambuf_iterator<char>(), '\n') + 1;

    // for (int i = 0 ; i < line_num; i += 16) {
    //     line = "";

    //     try {
    //         line = read_line(filename, i);
    //     } catch (const char * msg) {
    //         break;
    //     }

    //     if (!line.empty() && line != "") {
    //         all_units[line.c_str()] = unit();
    //         all_units[line.c_str()].read(filename, i);
    //     }

    //     inFile.close();
    // }
}

std::mt19937_64 mt_64(123);
std::mt19937 mt(123); // negligible perfomance difference
std::uniform_int_distribution<int> Uint_99(0, 99); // more twice faster than Ureal_1

int getRN(){
    return(Uint_99(mt_64));
}

std::unordered_map<std::string, Entity> all_units;
std::unordered_map<std::string, int> wpn_indexes;
