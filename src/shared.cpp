// Code créé par Gabriel Taillon
#include "shared.hpp"
#include "game.hpp"
#include <algorithm>

int seed = 1990; 
std::mt19937_64 mt_64(1990);
std::mt19937 mt(1990); // negligible perfomance difference
std::uniform_int_distribution<int> Uint_99(0, 99); // more twice faster than Ureal_1

Inventory_item convoy[200];

std::vector<std::string> wpntype2str(short unsigned int in_type){
    std::vector<std::string> types;
    if ((in_type & WPN::TYPE::SWORD) > 0) {
        types.push_back("Sword");
    }
    if ((in_type & WPN::TYPE::LANCE) > 0) {
        types.push_back("Lance");
    }
    if ((in_type & WPN::TYPE::AXE) > 0) {
        types.push_back("Axe");
    }
    if ((in_type & WPN::TYPE::BOW) > 0) {
        types.push_back("Bow");
    }
    if ((in_type & WPN::TYPE::TRINKET) > 0) {
        types.push_back("Trinket");
    }
    if ((in_type & WPN::TYPE::OFFHAND) > 0) {
        types.push_back("Offhand");
    }
    if ((in_type & WPN::TYPE::ELEMENTAL) > 0) {
        types.push_back("Magic");
        // types.push_back("Elemental");
    }
    if ((in_type & WPN::TYPE::DEMONIC) > 0) {
        types.push_back("Demonic");
    }

    if ((in_type & WPN::TYPE::ANGELIC) > 0) {
        types.push_back("Angelic");
    }
    if ((in_type & WPN::TYPE::SHIELD) > 0) {
        types.push_back("Shield");
    }
    if ((in_type & WPN::TYPE::STAFF) > 0) {
        types.push_back("Staff");
    }
    return(types);
}

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

std::vector<std::vector<int>> list2matrix(std::vector<std::vector<int>> list, int col, int row) {
    std::vector<std::vector<int>> matrix(col, std::vector<int> (row, 0));
    for (int ind = 0; ind < list.size(); ind++) {
        matrix[list[ind][0]][list[ind][1]] = 1;
    }
    return(matrix);
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
    // distance/2 + distance/4 + distance/16.... 1
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

int getRN(){
    return(Uint_99(mt_64));
}

int boxmuller_switch(int RN_U[2], bool sin_switch = true, float avg = 50., float std_dev = 20.) {
    // RNs frm 0 to 100.
    // std_dev: standard deviation,  avg: average
    // RN_G can be < 0 and > 100, but its okay.
    //DESIGN: no need to change the int that come out of boxmuller. 
    // -> (-7 < 50) returns true as much as (0 < 50)
    int RN_G;
    float RNreal_U[2];
    if (RN_U[0] == 0) {
        RNreal_U[0] = 0.00001;
    } else {
        RNreal_U[0] = ((float)RN_U[0])/100.;
    }
    if (RN_U[1] == 0) {
        RNreal_U[1] = 0.00001;
    } else {
        RNreal_U[1] = ((float)RN_U[1])/100.;
    }
    float term1 = sqrt(-2 *  log(RNreal_U[0]));
    float term2 = 2 * M_PI * RNreal_U[1];
    // printf("%f %f\n", (term1 * cos(term2)), (term1 * sin(term2)));
    if (sin_switch) {
        RN_G = (int) ((term1 * sin(term2))*std_dev + avg);
    } else {
        RN_G = (int) ((term1 * cos(term2))*std_dev + avg);
    }
    return(RN_G);
}

int * boxmuller(int RN_U[2], float avg, float std_dev) {
    // RNs frm 0 to 100.
    // std_dev: standard deviation,  avg: average
    // RN_G can be < 0 and > 100, but its okay.
    //DESIGN: no need to change the int that come out of boxmuller. 
    // -> (-7 < 50) returns true as much as (0 < 50)
    static int RN_G[2];
    float RNreal_U[2];
    if (RN_U[0] == 0) {
        RNreal_U[0] = 0.00001;
    } else {
        RNreal_U[0] = ((float)RN_U[0])/100.;
    }
    if (RN_U[1] == 0) {
        RNreal_U[1] = 0.00001;
    } else {
        RNreal_U[1] = ((float)RN_U[1])/100.;
    }
    float term1 = sqrt(-2 *  log(RNreal_U[0]));
    float term2 = 2 * M_PI * RNreal_U[1];
    // printf("%f %f\n", (term1 * cos(term2)), (term1 * sin(term2)));
    RN_G[0] = (int) ((term1 * sin(term2))*std_dev + avg);
    RN_G[1] = (int) ((term1 * cos(term2))*std_dev + avg);
    return(RN_G);
}

int * getGRNs(float avg, float std_dev) {
    // get Gaussian Random Numbers
    // RNs frm 0 to 100. Uses box-muller tranform and getRN function to compute it.
    // std_dev: standard deviation,  avg: average
    // RN_G can be < 0 and > 100, but its okay.
    //DESIGN: no need to change the int that come out of boxmuller. 
    // -> (-7 < 50) returns true as much as (0 < 50)
    static int RN_G[2];
    float RNreal_U[2];
    int RN_U[2];
    RN_U[0] = getRN();
    RN_U[1] = getRN();
    if (RN_U[0] == 0) {
        RNreal_U[0] = 0.00001;
    } else {
        RNreal_U[0] = ((float)RN_U[0])/100.;
    }
    if (RN_U[1] == 0) {
        RNreal_U[1] = 0.00001;
    } else {
        RNreal_U[1] = ((float)RN_U[1])/100.;
    }
    float term1 = sqrt(-2 *  log(RNreal_U[0]));
    float term2 = 2 * M_PI * RNreal_U[1];
    // printf("%f %f\n", (term1 * cos(term2)), (term1 * sin(term2)));
    RN_G[0] = (int) ((term1 * sin(term2))*std_dev + avg);
    RN_G[1] = (int) ((term1 * cos(term2))*std_dev + avg);
    return(RN_G);
}