
#include "pathfinding.hpp"

void plot2Darray(int array[][10], short unsigned int imax, short unsigned int jmax) {
    //Don't know how to pass array that at least do not have 1D known before hand
    for (short unsigned int i = 0; i < imax; i++) {
        for (short unsigned int j = 0; j < jmax; j++) {
            printf("%d ", array[i][j]);
        }

        printf("\n");
    }
}

int h_manhattan(int start[], int end[]) {
    // fast. Exact on a grid.
    return (abs(start[0] - end[0]) + abs(start[1] - end[1]));
}

int h_manhattan(std::vector<int> start, std::vector<int> end) {
    // fast. Exact on a grid.
    return (abs(start[0] - end[0]) + abs(start[1] - end[1]));
}

int h_manhattan(std::vector<int> start, int end[]) {
    // fast. Exact on a grid.
    return (abs(start[0] - end[0]) + abs(start[1] - end[1]));
}

int h_manhattan(int start[], std::vector<int> end) {
    // fast. Exact on a grid.
    return (abs(start[0] - end[0]) + abs(start[1] - end[1]));
}

double h_euclidean(int start[], int end[]) {
    // Slower. More accurate.
    return (std::sqrt(pow(start[0] - end[0], 2.0) + pow(start[1] - end[1], 2.0)));
}


int find_row(int start[], std::vector<std::vector<int>> list) {
    int row;

    for (row = 0; row < list.size(); row++) {
        if ((list[row][0] == start[0]) && (list[row][1] == start[1])) {
            return (row);
        }
    }

    return (-1);
}

int find_node(int start[], std::vector<std::vector<int>> list) {
    int row;

    for (row = 0; row < list.size(); row++) {
        if ((list[row][0] == start[0]) && (list[row][1] == start[1])) {
            return (row);
        }
    }

    return (-1);
}

std::vector<std::vector<int>> A_star(std::vector<std::vector<int>> map, int start[], int end[], std::string mode) {
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
    SDL_Log("%s\n", mode.c_str());

    while (!openlist.empty()) {
        current = openlist.back();
        closedlist.push_back(current);

        if ((current[0] == end[0]) && (current[1] == end[1])) {
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

        for (int i = 0; i < 4; i++) {
            if (map[(*neighbors[i])[0]][(*neighbors[i])[1]] > 0) {
                inopenlist_index = find_row(*neighbors[i], openlist);
                inclosedlist_index = find_row(*neighbors[i], closedlist);

                h_neighbor = h_manhattan(*neighbors[i], end);
                g_neighbor = current[2] + map[(*neighbors[i])[0]][(*neighbors[i])[1]];

                if (inopenlist_index > 0) {
                    if (g_neighbor < openlist[inopenlist_index][2]) {
                        // SDL_Log("erase open\n");
                        openlist.erase(openlist.begin() + inopenlist_index);
                    }
                }

                if (inclosedlist_index > 0) {
                    if (g_neighbor < closedlist[inclosedlist_index][2]) {
                        closedlist.erase(closedlist.begin() + inclosedlist_index);
                    }
                }

                if ((inopenlist_index < 0) && (inclosedlist_index < 0)) {
                    openlist.push_back({(*neighbors[i])[0], (*neighbors[i])[1], g_neighbor, h_neighbor + g_neighbor, current[0], current[1]});
                }

                if (openlist.size() > 1) {
                    for (swap_index = (openlist.size() - 1); swap_index >= 1; swap_index--) {
                        if (openlist[swap_index - 1][3] < openlist[swap_index][3]) {
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

        for (int i = 0; i < pathmap.size(); i++) {
            std::fill(pathmap[i].begin(), pathmap[i].end(), 0);
        }
    }

    path.push_back(closedlist.back());
    closedlist.pop_back();

    while (!closedlist.empty()) {
        if ((closedlist.back()[0] == path.back()[4]) && (closedlist.back()[1] == path.back()[5])) {
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

    return (path);
}